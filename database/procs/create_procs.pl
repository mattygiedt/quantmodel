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

print OUTFILE " -- ---------------------\n";
print OUTFILE " -- \n";
print OUTFILE " -- QuantModel Stored Procedures: $current_date";
print OUTFILE " -- \n";
print OUTFILE " -- ---------------------\n\n";
print OUTFILE "use `quantmodel`;\n\n";

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
        }

        closedir( PROCDIR );
        chdir( ".." );
    }
}

closedir( SRCDIR );
close( OUTFILE );
print "Created file: procs.sql\n";
