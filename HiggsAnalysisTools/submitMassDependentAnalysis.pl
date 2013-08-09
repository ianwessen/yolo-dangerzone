#!/usr/bin/perl

use Time::Local;
use Getopt::Std;

getopts('p:y:');
if($opt_p) {$prefix = $opt_p;}
else { die "usage: ./submitMassDependentAnalysis.pl -p <prefix> -y <year>";}
if($opt_y) {$year = $opt_y;}
else { die "usage: ./submitMassDependentAnalysis.pl -p <prefix> -y <year>";}

if($year==2012) {

    @masses = (110,115,120,125,130,135,140,145,150,155,160,170,180,190,200,250,300,350,400,450,500,550,600,700,800,900,1000);
    
    for($i=0; $i<($#masses+1); $i++) {
        $mass = $masses[$i];
        print "-------------------------->\n";
        print "SUBMITTING MASS SELECTION: mH = $mass ...\n\n";
        open(MASSFILE,">config/higgs/higgsMass.txt");
        print MASSFILE "HiggsMass\t$mass\n";
        
        print "submitting signals...\n";
        $higgsList = "GluGluToHToWWTo2LAndTau2Nu_M-".$mass."_8TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X $higgsList 5 HiggsApp 8nh $prefix 1 2012");

        print "submitting signals...\n";
        $higgsList = "VBF_HToWWTo2LAndTau2Nu_M-".$mass."_8TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X $higgsList 5 HiggsApp 8nh $prefix 1 2012");
        
        sleep 5;
    }

    print "done with signals.\n";

    open(MASSFILE,">config/higgs/higgsMass.txt");
    print MASSFILE "HiggsMass\t 160\n";

    print  "submitting top...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X T_t-channel_TuneZ2star_8TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X Tbar_t-channel_TuneZ2star_8TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X TTJets_TuneZ2star_8TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X TTTo2L2Nu2B_8TeV-powheg-pythia6 5 HiggsApp 8nh $prefix 1 2012");
    print  "done with top.\n";

    sleep 600;

    print  "submitting W+jets...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball 10  HiggsApp 8nh $prefix 1 2012");

    sleep 600;
    print  "submitting DY high mass...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball 10 HiggsApp 8nh $prefix 1 2012");

    sleep 600;
    print  "submitting DY low mass...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X DYJetsToLL_M-10To50filter_8TeV-madgraph 10 HiggsApp 8nh $prefix 1 2012");
    print  "done with V+jets.\n";

    sleep 600;

    print  "submitting dibosons...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola 10 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6 10 HiggsApp 8nh $prefix 1 2012");
    # system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X GVJets_7TeV-madgraph 10 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola 5 HiggsApp 8nh $prefix 1 2012");
    print  "done with dibosons.\n";

    sleep 600;

    print  "submitting W+gamma and Z+gamma...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WGToLNuG_TuneZ2star_8TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    # system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X ZGToEEG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2012");
    # system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X ZGToMuMuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2012");
    # system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2012");
    #system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X ZGToNuNuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2012");

    print  "submitting systematics samples for VV...\n";
    system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola 10 HiggsApp 8nh $prefix 1 2012");
    #system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WWTo2L2Nu_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2012");
    #system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2012");
    #system("python cmst3_submit_manyfilesperjob.py Summer12_V14_52X WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2012");

    print "\nDONE WITH MASS $mass GeV\n";
    print "<--------------------------\n";
    
    
} else {
    @masses = (120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600);
    @additionalmasses = (110,115,118,122,124,126,128,135); #these have different names

    for($i=0; $i<($#masses+1); $i++) {
        $mass = $masses[$i];
        print "-------------------------->\n";
        print "SUBMITTING MASS SELECTION: mH = $mass ...\n\n";
        open(MASSFILE,">config/higgs/higgsMass.txt");
        print MASSFILE "HiggsMass\t$mass\n";

        print "submitting signals...\n";
        $higgsList = "GluGluToHToWWTo2L2Nu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 10 HiggsApp 8nh $prefix 1 2011");
        
        $higgsList = "GluGluToHToWWToLNuTauNu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 10 HiggsApp 8nh $prefix 1 2011");
        
        $higgsList = "VBF_HToWWTo2L2Nu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 10 HiggsApp 8nh $prefix 1 2011");
        
        $higgsList = "VBF_HToWWToLNuTauNu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 10 HiggsApp 8nh $prefix 1 2011");

        sleep 5;

    }   

    for($i=0; $i<($#additionalmasses+1); $i++) {
        $mass = $additionalmasses[$i];
        print "-------------------------->\n";
        print "SUBMITTING MASS SELECTION: mH = $mass ...\n\n";
        open(MASSFILE,">config/higgs/higgsMass.txt");
        print MASSFILE "HiggsMass\t$mass\n";

        print "submitting signals...\n";
        $higgsList = "GluGluToHToWWTo2LAndTau2Nu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 5 HiggsApp 8nh $prefix 1 2011");

        print "submitting signals...\n";
        $higgsList = "VBF_HToWWTo2LAndTau2Nu_M-".$mass."_7TeV-powheg-pythia6";
        system("python cmst3_submit_manyfilesperjob.py Fall11_V1 $higgsList 5 HiggsApp 8nh $prefix 1 2011");
        
        sleep 5;

    }

    print "done with signals. Sleeping 600s...\n";
#sleep 600;

    open(MASSFILE,">config/higgs/higgsMass.txt");
    print MASSFILE "HiggsMass\t 160\n";

    print  "submitting top...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_tW-channel-DS_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_tW-channel-DS_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_t-channel_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_t-channel_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_s-channel_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_s-channel_7TeV-powheg-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 TTTo2L2Nu2B_7TeV-powheg-pythia6 5 HiggsApp 8nh $prefix 1 2011");
    print  "done with top.\n";

    sleep 600;

    print  "submitting W+jets...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WJetsToLNu_TuneZ2_7TeV-madgraph-tauola 10  HiggsApp 8nh $prefix 1 2011");

    sleep 600;
    print  "submitting DY high mass...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola 10 HiggsApp 8nh $prefix 1 2011");

    sleep 600;
    print  "submitting DY low mass...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola 10 HiggsApp 8nh $prefix 1 2011");
    print  "done with V+jets.\n";

    sleep 600;

    print  "submitting dibosons...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 GVJets_7TeV-madgraph 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2011");
    print  "done with dibosons.\n";

    sleep 600;

    print  "submitting W+gamma and Z+gamma...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WGToENuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WGToMuNuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WGToTauNuG_TuneZ2_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZGToEEG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZGToMuMuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZGToNuNuG_TuneZ2_7TeV-madgraph 5 HiggsApp 8nh $prefix 1 2011");

    sleep 600;

#print  "submitting systematics samples for top...\n";
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 TT_TuneZ2_7TeV-pythia6-tauola 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 TTJets_TuneZ2_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 TTjets_TuneZ2_scaleup_7TeV-madgraph-tauola 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_scaledown_tW-channel-DR 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_scaledown_tW-channel-DS 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_scaleup_tW-channel-DR 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 T_TuneZ2_scaleup_tW-channel-DS 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_scaledown_tW-channel-DR 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_scaledown_tW-channel-DS 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_scaleup_tW-channel-DR 5 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 Tbar_TuneZ2_scaleup_tW-channel-DS 5 HiggsApp 8nh $prefix 1 2011");

#sleep 600;

    print  "submitting systematics samples for VV...\n";
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WWTo2L2Nu_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo 10 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 ZZ_TuneZ2_7TeV_pythia6_tauola 10 HiggsApp 8nh $prefix 1 2011");
    system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WZTo3LNu_TuneZ2_7TeV_pythia6_tauola 10 HiggsApp 8nh $prefix 1 2011");
#sleep 600;

#print  "submitting systematics samples for Wjets...\n";
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WJetsToLNu_TuneZ2_scaledown_7TeV-madgraph-tauola 10 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WJetsToLNu_TuneZ2_scaleup_7TeV-madgraph-tauola 10 HiggsApp 8nh $prefix 1 2011");
#system("python cmst3_submit_manyfilesperjob.py Fall11_V1 WJetsToLNu_TuneZ2_matchingup_7TeV-madgraph-tauola 10 HiggsApp 8nh $prefix 1 2011");

    print "\nDONE WITH MASS $mass GeV\n";
    print "<--------------------------\n";
    
}

