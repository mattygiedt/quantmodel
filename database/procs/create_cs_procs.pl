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

open( CSFILE, '>QuantModelProcedures.cs' );

print CSFILE "/*\n QuantModelProcedures.cs auto generated on:\n $current_date DO NOT EDIT\n*/\n\nusing System;\nusing quantmodel;\n\n\nnamespace com.quantmodel.common\n{\n  public sealed class QuantModelProcedures\n  {\n";

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
            ##  Output the MySQL++ Query used to call the procedure
            ##

            $func = "    public static Google.ProtocolBuffers.IMessage $procedure( string session_id,";

            if( scalar @inputs > 0 )
            {
                foreach my $input ( @inputs )
                {
                    $name = $input->findvalue( '@name' );
                    $type = $input->findvalue( '@type' );
                    $func = $func . " $type $name,";
                }

                ##
                ##  Remove the last comma
                ##

                chop $func;

                $func = $func . " ) \n    {\n";
                $func = $func . "        string request_id = Guid.NewGuid().ToString();\n";
                $func = $func . "        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();\n";
                $func = $func . "        string sql = \"call $procedure( \"";

                foreach my $input ( @inputs )
                {
                    $name = $input->findvalue( '@name' );
                    $quote = $input->findvalue( '@quote' );

                    if( lc( $quote ) eq 'true' )
                    {
                        $func = $func . " + \"'\" + $name + \"',\"";
                    }
                    else
                    {
                        $func = $func . " + $name + \",\"";
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
                $func = $func . "        string request_id = Guid.NewGuid().ToString();\n";
                $func = $func . "        string timestamp = com.quantmodel.common.network.message.ClientMessage.getTimestamp();\n";
                $func = $func . "        string sql = \"call $procedure\";\n";
            }

            ##
            ##  Build the database request
            ##

            $func = $func . "        return ClientAdapterRequest.CreateBuilder()\n";
            $func = $func . "           .SetRequestId( request_id )\n";
            $func = $func . "           .SetSessionId( session_id )\n";
            $func = $func . "           .SetTimestamp( timestamp )\n";
            $func = $func . "           .SetType( ClientAdapterRequest.Types.RequestType.DATABASE )\n";
            $func = $func . "           .SetDatabaseRequest(\n";
            $func = $func . "               DatabaseEngineRequest.CreateBuilder()\n";
            $func = $func . "                   .SetRequestId( request_id )\n";
            $func = $func . "                   .SetTimestamp( timestamp )\n";
            $func = $func . "                   .AddQuery(\n";
            $func = $func . "                       DatabaseEngineRequest.Types.Query.CreateBuilder()\n";
            $func = $func . "                           .SetQueryId( \"$procedure\" )\n";
            $func = $func . "                           .SetQueryStr( sql )\n";
            $func = $func . "                           .SetType( DatabaseEngineRequest.Types.QueryType.$query_type ) ) )\n";
            $func = $func . "           .Build();\n";
            $func = $func . "    }\n\n";

            print CSFILE $func;
        }

        closedir( PROCDIR );
        chdir( ".." );
    }
}

print CSFILE "  }\n}\n\n";

closedir( SRCDIR );
close( CSFILE );
print "Created file: QuantModelProcedures.cs\n";
