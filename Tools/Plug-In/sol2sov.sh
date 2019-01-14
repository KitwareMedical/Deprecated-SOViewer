#!/usr/bin/perl

use File::Find;
use strict;

my $directory = "/cygdrive/e/itkUNC";

find (\&process, $directory);

sub process
{
    my @outLines;  #Data we are going to output
    my $line;      #Data we are reading line by line

     # print "processing $_ / $File::Find::name\n";

    # Only parse files that end in .h
    if( ( $File::Find::name =~ /\.h$/ )
||   ( $File::Find::name =~ /\.cxx$/ )
||   ( $File::Find::name =~ /\.cpp$/ )
||   ( $File::Find::name =~ /\.txx$/ )
||   ( $File::Find::name =~ /\.fl$/ )
||   ( $File::Find::name =~ /\.fld$/ )
)
	 	    {

        open (FILE, $File::Find::name ) or 
        die "Cannot open file: $!";

        print "\n" . $File::Find::name . "\n";
        while ( $line = <FILE> ) {
        $line =~ s/sol::/sov::/g;
        $line =~ s/sol(.*\.)h/sov$1h/g;
        $line =~ s/SOL_/SOV_/g;
            push(@outLines, $line);
        }
        close FILE;

        open ( OUTFILE, ">$File::Find::name" ) or 
        die "Cannot open file: $!";

        print ( OUTFILE @outLines );
        close ( OUTFILE );
        
        undef( @outLines );
    }
}

