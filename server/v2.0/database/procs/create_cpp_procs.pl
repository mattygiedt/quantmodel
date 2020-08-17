#!/usr/bin/perl

$current_date = `date`;

##
##  use modules
##

use XML::LibXML;
use Data::Dumper;

##
##  libxml2
##

$parser = XML::LibXML->new();

##
##  output file
##

open( OUTFILE, '>procs.sql' );
open( HPPFILE, '>QuantModelProcedures.hpp' );

print OUTFILE " -- ---------------------\n";
print OUTFILE " -- \n";
print OUTFILE " -- QuantModel Stored Procedures: $current_date";
print OUTFILE " -- \n";
print OUTFILE " -- ---------------------\n\n";
print OUTFILE "use `quantmodel`;\n\n";

print HPPFILE "/*\n QuantModelProcedures.hpp auto generated on:\n $current_date DO NOT EDIT\n*/\n\n#ifndef QUANTMODEL_PROCEDURES_H\n#define QUANTMODEL_PROCEDURES_H\n\n#include <string>\n#include <iostream>\n#include <sstream>\n\nusing namespace std;\n\nnamespace quantmodel\n{\n  class QuantModelProcedures\n  {\n    public:\n\n";

##
##  Loop through each directory reading XML procs
##

opendir( SRCDIR, "./" ) || die( "Cannot open directory: ./" );

while( $f = readdir( SRCDIR ) )
{
    next if( ($f eq ".") || ($f eq "..") || ($f eq ".svn") );

    if( -d $f )
    {
        print "Generating: $f procs...\n";
        chdir( $f );

        opendir( PROCDIR, "./" );
        while( $p = readdir( PROCDIR ) )
        {
            next if( ($p eq ".") || ($p eq "..") || ($p eq ".svn") );

            print " loading $p\n";

            ##
            ##  Substitute the procedure name --> SP_NAME
            ##

            $data = $parser->parse_file( $p );
            $root = $data->getDocumentElement;
            $procedure = $root->findvalue( 'procedure/@name' );
            $query_type = $root->findvalue( 'procedure/@type' );
            $content = $root->findvalue( 'procedure/content' );
            @inputs = $root->findnodes( 'procedure/input' );
            $content =~ s/SP_NAME/$procedure/g;

            ##
            ##  Output the stored procedure
            ##

            print OUTFILE " -- ---------------------\n";
            print OUTFILE " -- \n";
            print OUTFILE " -- $procedure\n";
            print OUTFILE " -- \n";
            print OUTFILE " -- ---------------------\n\n";
            print OUTFILE $content;
            print OUTFILE "\n\n";

            ##
            ##  Output the MySQL++ Query used to call the procedure
            ##

            $func = "    const inline static void $procedure( quantmodel::DatabaseEngineRequest& request,";

            if( scalar @inputs > 0 )
            {
                foreach my $input ( @inputs )
                {
                    $name = $input->findvalue( '@name' );
                    $type = $input->findvalue( '@type' );
                    $func = $func . " const $type& $name,";
                }

                ##
                ##  Remove the last comma
                ##

                chop $func;

                $func = $func . " ) \n    {\n";
                $func = $func . "        std::ostringstream sql;\n";
                $func = $func . "        sql << \"call $procedure( \"";

                foreach my $input ( @inputs )
                {
                    $name = $input->findvalue( '@name' );
                    $quote = $input->findvalue( '@quote' );

                    if( lc( $quote ) eq 'true' )
                    {
                        $func = $func . " << \"'\" << $name << \"',\"";
                    }
                    else
                    {
                        $func = $func . " << $name << \",\"";
                    }
                }

                ##
                ##  Remove the last two chars
                ##

                $func = substr( $func, 0, -2 ) . " )\";\n";
            }
            else
            {
                chop $func;

                $func = $func . " ) \n    {\n";
                $func = $func . "        std::ostringstream sql;\n";
                $func = $func . "        sql << \"call $procedure\";\n";
            }

            ##
            ##  Build the database request
            ##

            $func = $func . "        quantmodel::DatabaseEngineRequest::Query *query = request.add_query();\n";
            $func = $func . "        query->set_query_id( \"$procedure\" );\n";
            $func = $func . "        query->set_query_str( sql.str() );\n";
            $func = $func . "        query->set_type( quantmodel::DatabaseEngineRequest::$query_type );\n";
            $func = $func . "    }\n\n";

            print HPPFILE $func;
        }

        closedir( PROCDIR );
        chdir( ".." );
    }
}

print HPPFILE "  };\n}\n\n#endif // QUANTMODEL_PROCEDURES_H\n";

closedir( SRCDIR );
close( OUTFILE );
close( HPPFILE );
print "Created file: procs.sql\n";
print "Created file: QuantModelProcedures.hpp\n";
