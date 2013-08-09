#!/usr/bin/perl

use Time::Local;
use Getopt::Std;

getopts('p:y:');
if($opt_p) {$prefix = $opt_p;}
else { die "usage: ./submitMassDependentAnalysis.pl -p <prefix> -y <year>";}
if($opt_y) {$year = $opt_y;}
else { die "usage: ./submitMassDependentAnalysis.pl -p <prefix> -y <year>";}

if($year==2012) {

    @masses = (120);
    
    for($i=0; $i<($#masses+1); $i++) {
        $mass = $masses[$i];
        print "-------------------------->\n";
        print "SUBMITTING MASS SELECTION: mH = $mass ...\n\n";
        open(MASSFILE,">config/higgs/higgsMass.txt");
        print MASSFILE "HiggsMass\t$mass\n";

        print  "submitting double ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Collisions8TeV_V14_52X DoubleElectron 10 HiggsApp 8nh $prefix 0 2012");
        print  "done with double ele. Sleeping 600s to recover.\n";
        
        sleep 600;
        
        print  "submitting double mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Collisions8TeV_V14_52X DoubleMu 10 HiggsApp 8nh $prefix 0 2012");
        print  "done with double mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting mu eg...\n";
        system("python cmst3_submit_manyfilesperjob.py Collisions8TeV_V14_52X MuEG 10 HiggsApp 8nh $prefix 0 2012");
        print  "done with mu eg. Sleeping 600s to recover.\n";

        sleep 600;
        
        print  "submitting single mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Collisions8TeV_V14_52X SingleMu 10 HiggsApp 8nh $prefix 0 2012");
        print  "done with single mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting single ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Collisions8TeV_V14_52X SingleElectron 10 HiggsApp 8nh $prefix 0 2012");
        print  "done with single electron. Sleeping 600s to recover.\n";

        print "\nDONE\n";
        print "<--------------------------";

    }
}

if($year==2011) {
    
    @masses = (120);
    
    for($i=0; $i<($#masses+1); $i++) {
        $mass = $masses[$i];
        print "-------------------------->\n";
        print "SUBMITTING MASS SELECTION: mH = $mass ...\n\n";
        open(MASSFILE,">config/higgs/higgsMass.txt");
        print MASSFILE "HiggsMass\t$mass\n";

        print  "submitting double ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW DoubleElectron 10 HiggsApp 8nh $prefix 0");
        print  "done with double ele. Sleeping 600s to recover.\n";
        
        sleep 600;
        
        print  "submitting double mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW DoubleMu 10 HiggsApp 8nh $prefix 0");
        print  "done with double mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting mu eg...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW MuEG 10 HiggsApp 8nh $prefix 0");
        print  "done with mu eg. Sleeping 600s to recover.\n";

        sleep 600;
        
        print  "submitting single mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW SingleMu 5 HiggsApp 8nh $prefix 0");
        print  "done with single mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting single ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW SingleElectron 10 HiggsApp 8nh $prefix 0");
        print  "done with single electron. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting double ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW DoubleElectron_Run2011B 10 HiggsApp 8nh $prefix 0");
        print  "done with double ele. Sleeping 600s to recover.\n";
        
        sleep 600;
        
        print  "submitting double mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW DoubleMu_Run2011B 10 HiggsApp 8nh $prefix 0");
        print  "done with double mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting mu eg...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW MuEG_Run2011B 10 HiggsApp 8nh $prefix 0");
        print  "done with mu eg. Sleeping 600s to recover.\n";

        sleep 600;
        
        print  "submitting single mu...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW SingleMu_Run2011B 5 HiggsApp 8nh $prefix 0");
        print  "done with single mu. Sleeping 600s to recover.\n";

        sleep 600;

        print  "submitting single ele...\n";
        system("python cmst3_submit_manyfilesperjob.py Data7TeVHWW SingleElectron_Run2011B 10 HiggsApp 8nh $prefix 0");
        print  "done with single electron.\n";
        
        print "\nDONE\n";
        print "<--------------------------";

    }
}
