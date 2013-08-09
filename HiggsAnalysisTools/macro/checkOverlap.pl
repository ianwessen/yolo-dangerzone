#! /usr/bin/perl

use Getopt::Std;
use Cwd;

@masses = (120,130,140,150,160);

$curDir = cwd();

@overlaps = ([0,0,0,0,0],
             [0,0,0,0,0],
             [0,0,0,0,0],
             [0,0,0,0,0],
             [0,0,0,0,0]);

for($i=0; $i<($#masses+1); $i++) {
    $imass = $masses[$i];
    for($j=0; $j<($#masses+1); $j++) {

        $jmass = $masses[$j];
        print "imass = $imass , jmass = $jmass\n";
        open(SCRIPT,">script.sh");
        print SCRIPT "#/bin/sh\n";
        print(SCRIPT "rm -f  merged_$imass\_$jmass.txt\n");
        print(SCRIPT "touch merged_$imass\_$jmass.txt\n");
        print(SCRIPT "cat events_data_1_$imass.txt >> merged_$imass\_$jmass.txt\n");
        print(SCRIPT "cat events_data_1_$jmass.txt >> merged_$imass\_$jmass.txt\n");
        print(SCRIPT "sort merged_$imass\_$jmass.txt > sorted_$imass\_$jmass.txt\n");
        print(SCRIPT "uniq -d sorted_$imass\_$jmass.txt > duplicated_$imass\_$jmass.txt\n");
        system("chmod a+x $curDir/script.sh");
        system("source $curDir/script.sh;");
        
        open (FIRST,"<","events_data_1_$imass.txt") or die "can't open file events_data_1_$imass.txt\n";
        $iEvents = 0;
        while(<FIRST>) { $iEvents++; }
        open (SECOND,"<","events_data_1_$jmass.txt") or die "can't open file events_data_1_$jmass.txt\n";
        $jEvents = 0;
        while(<SECOND>) { $jEvents++; }
        open (COMMON,"<","duplicated_$imass\_$jmass.txt") or die "can't open file duplicated_$imass\_$jmass.txt\n";
        $cEvents = 0;
        while(<COMMON>) { $cEvents++; }

        $cOveri = $cEvents / $iEvents;
        $cOverj = $cEvents / $jEvents;

#        print "common $imass,$jmass / $imass = $cEvents / $iEvents = $cOveri \n";
#        print "common $imass,$jmass / $jmass = $cEvents / $jEvents = $cOverj \n";

        $overlaps[$imass][$jmass] = $cOverj;

    }
}

for($i=0; $i<($#masses+1); $i++) {
    $imass = $masses[$i];
    for($j=0; $j<($#masses+1); $j++) {
        
        $jmass = $masses[$j];
        
        $val = $overlaps[$imass][$jmass];
        
        print "$val & \t";
    }
    print "\\\\\n";
}
