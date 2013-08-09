#! /usr/bin/perl

use Getopt::Std;
getopts('m:');

$lumi = 1000; # in pb-1
$HiggsMass = $opt_m;
print "creating tex table for Higgs mass $HiggsMass GeV/c^2 and lumi = $lumi pb-1....... \n";

$initial_row=0;
$final_row=80;

$HiggsLogFile = "tmplogs/H".$HiggsMass."/H".$HiggsMass.".log";
$WWLogFile    = "tmplogs/H".$HiggsMass."/WW_incl.log";
$tWLogFile    = "tmplogs/H".$HiggsMass."/tW.log";
$WZLogFile    = "tmplogs/H".$HiggsMass."/WZ.log";
$ZZLogFile    = "tmplogs/H".$HiggsMass."/ZZ.log";
$ChowderLogFile = "tmplogs/H".$HiggsMass."/ChowderPDElectronSkim.log";
$DY10to40LogFile = "tmplogs/H".$HiggsMass."/DY_10_40.log";

$HiggsXsec = 0; # CHECK ALL THESE!!!!!!!!!!!!!!!!!
if($HiggsMass==120) { $HiggsXsec=0.56; }
if($HiggsMass==130) { $HiggsXsec=1.06; }   
if($HiggsMass==140) { $HiggsXsec=1.58; } 
if($HiggsMass==150) { $HiggsXsec=1.98; } 
if($HiggsMass==160) { $HiggsXsec=2.34; } 
if($HiggsMass==165) { $HiggsXsec=2.36; }
if($HiggsMass==170) { $HiggsXsec=2.26; } 
if($HiggsMass==180) { $HiggsXsec=1.99; } 
if($HiggsMass==190) { $HiggsXsec=1.51; } 
if($HiggsMass==200) { $HiggsXsec=1.30; }

# http://ceballos.web.cern.ch/ceballos/hwwlnln/cross-sections_csa07analysis.txt
$WWXsec    = 114.3;                               
$tWXsec    = 62.0;
$WZXsec    = 49.9;
$ZZXsec    = 15.3;
$DY10to40Xsec = 5951.0;

#init table
$textable1 = "eff".$HiggsMass."-1.tex";
open(TEXFILE1,">$textable1");
print TEXFILE1 "\\begin\{sidewaystable\}\[htb\]\n";
print TEXFILE1 "\\begin\{center\}\n";
print TEXFILE1 "\\begin\{tabular\}\{|l|c|c|c|c|c|\}\n";
print TEXFILE1 "\\hline\n";
print TEXFILE1 "\\hline\n";
print TEXFILE1 "& & & & & \\\\ \n";
print TEXFILE1 "& \n";
print TEXFILE1 "\$H \\rightarrow WW\$  \&\n";
print TEXFILE1 "\$WW\$\&\n";
print TEXFILE1 "\$t \\bar\{t\}\$ \&\n";
print TEXFILE1 "\$W + jets \$\&\n";
print TEXFILE1 "\$Z + jets\$\n";
print TEXFILE1 "\\\\\n";
print TEXFILE1 "\\hline\n";
print TEXFILE1 "\\hline\n";



$textable2 = "eff".$HiggsMass."-2.tex";
open(TEXFILE2,">$textable2");
print TEXFILE2 "\\begin\{sidewaystable\}\n";
print TEXFILE2 "\\begin\{center\}\n";
print TEXFILE2 "\\begin\{tabular\}\[t\]\{|l|c|c|c|c|\}\n";
print TEXFILE2 "\\hline\n";
print TEXFILE2 "\\hline\n";
print TEXFILE2 "& & & & \\\\ \n";
print TEXFILE2 "& \n";
print TEXFILE2 "\$DY(10-40\$GeV\$)\$ \&\n";
print TEXFILE2 "\$WZ\$ \&\n";
print TEXFILE2 "\$tW\$ \&\n";
print TEXFILE2 "\$ZZ\$ \n";
print TEXFILE2 "\\\\\n";
print TEXFILE2 "\\hline\n";
print TEXFILE2 "\\hline\n";


$Higgs_event = 0;
$Higgs_trigger = 0;
$Higgs_Preselection = 0;
$Higgs_Preselection2e=0;
$Higgs_eleID=0;
$Higgs_Isol=0;
$Higgs_jetVeto=0;
$Higgs_MET=0;
$Higgs_deltaPhi=0;
$Higgs_maxPtLepton=0;
$Higgs_minPtLepton=0;
$Higgs_eleInvMass=0;
$Higgs_final=0;

open(HIGGSLOGFILE,$HiggsLogFile) or die "cannot open $HiggsLogFile !\n";
seek (HIGGSLOGFILE, -7000, 2);
$irow=1;
while($row=<HIGGSLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$Higgs_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$Higgs_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$Higgs_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$Higgs_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$Higgs_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$Higgs_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$Higgs_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$Higgs_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$Higgs_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$Higgs_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$Higgs_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$Higgs_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$Higgs_final=$1;}
    }
}


# print  "$Higgs_event\n";
# print "$Higgs_trigger\n";
# print "$Higgs_Preselection\n";
# print "$Higgs_Preselection2e\n";
# print "$Higgs_eleID\n";
# print "$Higgs_Isol\n";
# print "$Higgs_jetVeto\n";
# print "$Higgs_MET\n";
# print "$Higgs_deltaPhi\n";
# print "$Higgs_maxPtLepton\n";
# print "$Higgs_minPtLepton\n";
# print "$Higgs_eleInvMass\n";
# print "$Higgs_final\n";


$WW_event = 0;
$WW_trigger = 0;
$WW_Preselection = 0;
$WW_Preselection2e=0;
$WW_eleID=0;
$WW_Isol=0;
$WW_jetVeto=0;
$WW_MET=0;
$WW_deltaPhi=0;
$WW_maxPtLepton=0;
$WW_minPtLepton=0;
$WW_eleInvMass=0;
$WW_final=0;

open(WWLOGFILE,$WWLogFile) or die "cannot open $WWLogFile !\n";
seek (WWLOGFILE, -7000, 2);
$irow=1;
while($row=<WWLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$WW_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$WW_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$WW_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$WW_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$WW_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$WW_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$WW_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$WW_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$WW_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$WW_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$WW_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$WW_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$WW_final=$1;}
    }
}



$tW_event = 0;
$tW_trigger = 0;
$tW_Preselection = 0;
$tW_Preselection2e=0;
$tW_eleID=0;
$tW_Isol=0;
$tW_jetVeto=0;
$tW_MET=0;
$tW_deltaPhi=0;
$tW_maxPtLepton=0;
$tW_minPtLepton=0;
$tW_eleInvMass=0;
$tW_final=0;

open(tWLOGFILE,$tWLogFile) or die "cannot open $tWLogFile !\n";
seek (tWLOGFILE, -7000, 2);
$irow=1;
while($row=<tWLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$tW_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$tW_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$tW_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$tW_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$tW_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$tW_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$tW_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$tW_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$tW_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$tW_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$tW_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$tW_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$tW_final=$1;}
    }
}




$WZ_event = 0;
$WZ_trigger = 0;
$WZ_Preselection = 0;
$WZ_Preselection2e=0;
$WZ_eleID=0;
$WZ_Isol=0;
$WZ_jetVeto=0;
$WZ_MET=0;
$WZ_deltaPhi=0;
$WZ_maxPtLepton=0;
$WZ_minPtLepton=0;
$WZ_eleInvMass=0;
$WZ_final=0;

open(WZLOGFILE,$WZLogFile) or die "cannot open $WZLogFile !\n";
seek (WZLOGFILE, -7000, 2);
$irow=1;
while($row=<WZLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$WZ_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$WZ_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$WZ_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$WZ_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$WZ_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$WZ_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$WZ_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$WZ_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$WZ_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$WZ_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$WZ_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$WZ_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$WZ_final=$1;}
    }
}



$ZZ_event = 0;
$ZZ_trigger = 0;
$ZZ_Preselection = 0;
$ZZ_Preselection2e=0;
$ZZ_eleID=0;
$ZZ_Isol=0;
$ZZ_jetVeto=0;
$ZZ_MET=0;
$ZZ_deltaPhi=0;
$ZZ_maxPtLepton=0;
$ZZ_minPtLepton=0;
$ZZ_eleInvMass=0;
$ZZ_final=0;

open(ZZLOGFILE,$ZZLogFile) or die "cannot open $ZZLogFile !\n";
seek (ZZLOGFILE, -7000, 2);
$irow=1;
while($row=<ZZLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$ZZ_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$ZZ_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$ZZ_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$ZZ_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$ZZ_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$ZZ_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$ZZ_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$ZZ_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$ZZ_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$ZZ_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$ZZ_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$ZZ_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$ZZ_final=$1;}
    }
}

$DY10to40_event = 0;
$DY10to40_trigger = 0;
$DY10to40_Preselection = 0;
$DY10to40_Preselection2e=0;
$DY10to40_eleID=0;
$DY10to40_Isol=0;
$DY10to40_jetVeto=0;
$DY10to40_MET=0;
$DY10to40_deltaPhi=0;
$DY10to40_maxPtLepton=0;
$DY10to40_minPtLepton=0;
$DY10to40_eleInvMass=0;
$DY10to40_final=0;

open(DY10to40LOGFILE,$DY10to40LogFile) or die "cannot open $DY10to40LogFile !\n";
seek (DY10to40LOGFILE, -7000, 2);
$irow=1;
while($row=<DY10to40LOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$DY10to40_event=$1;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$DY10to40_trigger=$1;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$DY10to40_Preselection=$1;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$DY10to40_Preselection2e=$1;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$DY10to40_eleID=$1;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$DY10to40_Isol=$1;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$DY10to40_jetVeto=$1;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$DY10to40_MET=$1;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$DY10to40_deltaPhi=$1;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$DY10to40_maxPtLepton=$1;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$DY10to40_minPtLepton=$1;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$DY10to40_eleInvMass=$1;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$DY10to40_final=$1;}
    }
}





$chowder_lumi = 1000;

$Wj_event = 0;
$Wj_trigger = 0;
$Wj_Preselection = 0;
$Wj_Preselection2e=0;
$Wj_eleID=0;
$Wj_Isol=0;
$Wj_jetVeto=0;
$Wj_MET=0;
$Wj_deltaPhi=0;
$Wj_maxPtLepton=0;
$Wj_minPtLepton=0;
$Wj_eleInvMass=0;
$Wj_final=0;

open(CHOWDERLOGFILE,$ChowderLogFile) or die "cannot open $ChowderLogFile !\n";
seek (CHOWDERLOGFILE, -30000, 2);
$irow=1;
while($row=<CHOWDERLOGFILE>) {
    if($row=~/\*\-\-\-\-\-\-\-\s+for\sPRESELECTION\sEVENT\sCOUNTER\sfor\sprocess\s2\s+\-\-\-\-\-\-\-\-\-\-/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$Wj_event=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$Wj_trigger=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$Wj_Preselection=$1*$lumi/$chowder_lumi;}
    }

}

open(CHOWDERLOGFILE,$ChowderLogFile) or die "cannot open $ChowderLogFile !\n";
seek (CHOWDERLOGFILE, -30000, 2);
$irow=1;
while($row=<CHOWDERLOGFILE>) {
    if($row=~/\*\-\-\-\-\-\-\-\s+\sfor\sEvent\scounter\sfor\sprocess\s2\s+\-\-\-\-\-\-\-\-\-\-/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$Wj_Preselection2e=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$Wj_eleID=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$Wj_Isol=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$Wj_jetVeto=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$Wj_MET=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$Wj_deltaPhi=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$Wj_maxPtLepton=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$Wj_minPtLepton=$1*$lumi/$chowder_lumi;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$Wj_eleInvMass=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$Wj_final=$1*$lumi/$chowder_lumi;}
    }

}

print "Wj_event = $Wj_event\n";
print "Wj_final = $Wj_final\n";



$Zj_event = 0;
$Zj_trigger = 0;
$Zj_Preselection = 0;
$Zj_Preselection2e=0;
$Zj_eleID=0;
$Zj_Isol=0;
$Zj_jetVeto=0;
$Zj_MET=0;
$Zj_deltaPhi=0;
$Zj_maxPtLepton=0;
$Zj_minPtLepton=0;
$Zj_eleInvMass=0;
$Zj_final=0;

open(CHOWDERLOGFILE,$ChowderLogFile) or die "cannot open $ChowderLogFile !\n";
seek (CHOWDERLOGFILE, -30000, 2);
$irow=1;
while($row=<CHOWDERLOGFILE>) {
    if($row=~/\*\-\-\-\-\-\-\-\s+for\sPRESELECTION\sEVENT\sCOUNTER\sfor\sprocess\s3\s+\-\-\-\-\-\-\-\-\-\-/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$Zj_event=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$Zj_trigger=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$Zj_Preselection=$1*$lumi/$chowder_lumi;}
    }

}

open(CHOWDERLOGFILE,$ChowderLogFile) or die "cannot open $ChowderLogFile !\n";
seek (CHOWDERLOGFILE, -30000, 2);
$irow=1;
while($row=<CHOWDERLOGFILE>) {
    if($row=~/\*\-\-\-\-\-\-\-\s+\sfor\sEvent\scounter\sfor\sprocess\s3\s+\-\-\-\-\-\-\-\-\-\-/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$Zj_Preselection2e=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$Zj_eleID=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$Zj_Isol=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$Zj_jetVeto=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$Zj_MET=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$Zj_deltaPhi=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$Zj_maxPtLepton=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$Zj_minPtLepton=$1*$lumi/$chowder_lumi;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$Zj_eleInvMass=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$Zj_final=$1*$lumi/$chowder_lumi;}
    }

}

print "Zj_event = $Zj_event\n";
print "Zj_final = $Zj_final\n";




$ttbarj_event = 0;
$ttbarj_trigger = 0;
$ttbarj_Preselection = 0;
$ttbarj_Preselection2e=0;
$ttbarj_eleID=0;
$ttbarj_Isol=0;
$ttbarj_jetVeto=0;
$ttbarj_MET=0;
$ttbarj_deltaPhi=0;
$ttbarj_maxPtLepton=0;
$ttbarj_minPtLepton=0;
$ttbarj_eleInvMass=0;
$ttbarj_final=0;

open(CHOWDERLOGFILE,$ChowderLogFile) or die "cannot open $ChowderLogFile !\n";
seek (CHOWDERLOGFILE, -30000, 2);
$irow=1;
while($row=<CHOWDERLOGFILE>) {
    if($row=~/Full\sMM\sselections/) {
	last;
    }
    chop $row;
    # normalization
    if($row=~/\*\s+event\:\s+(\S+)$/) {
	{$ttbarj_event=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+trigger\:\s+(\S+)$/) {
	{$ttbarj_trigger=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+finalOURPreselection\:\s+(\S+)$/) {
	{$ttbarj_Preselection=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+dileptonInvMassMin\:\s+(\S+)$/) {
	{$ttbarj_Preselection2e=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+classDepEleId\:\s+(\S+)$/) {
	{$ttbarj_eleID=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+ecalIso\:\s+(\S+)$/) {
	{$ttbarj_Isol=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+jetVeto\:\s+(\S+)$/) {
	{$ttbarj_jetVeto=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+MET\:\s+(\S+)$/) {
	{$ttbarj_MET=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+deltaPhi\:\s+(\S+)$/) {
	{$ttbarj_deltaPhi=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+maxPtLepton\:\s+(\S+)$/) {
	{$ttbarj_maxPtLepton=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+minPtLepton\:\s+(\S+)$/) {
	{$ttbarj_minPtLepton=$1*$lumi/$chowder_lumi;}
    }


    if($row=~/\*\s+dileptonInvMassMax\:\s+(\S+)$/) {
	{$ttbarj_eleInvMass=$1*$lumi/$chowder_lumi;}
    }

    if($row=~/\*\s+final\:\s+(\S+)$/) {
	{$ttbarj_final=$1*$lumi/$chowder_lumi;}
    }

}

print "ttbarj_event = $ttbarj_event\n";
print "ttbarj_final = $ttbarj_final\n";







#--- HLT ---   
print TEXFILE1 "HLT &\n";
print TEXFILE2 "HLT &\n";
$decimals = 0;
$n_Higgs_trigger= sprintf("%.4g", $Higgs_trigger/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_trigger = sprintf("%.0f", 100 * $Higgs_trigger/$Higgs_event);

$n_WW_trigger= sprintf("%.4g", $WW_trigger/$WW_event * $WWXsec * $lumi);
$eff_WW_trigger = sprintf("%.0f", 100 * $WW_trigger/$WW_event);

$n_tW_trigger= sprintf("%.4g", $tW_trigger/$tW_event * $tWXsec * $lumi);
$eff_tW_trigger = sprintf("%.0f", 100 * $tW_trigger/$tW_event);

$n_WZ_trigger= sprintf("%.4g", $WZ_trigger/$WZ_event * $WZXsec * $lumi);
$eff_WZ_trigger = sprintf("%.0f", 100 * $WZ_trigger/$WZ_event);

$n_ZZ_trigger= sprintf("%.4g", $ZZ_trigger/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_trigger = sprintf("%.0f", 100 * $ZZ_trigger/$ZZ_event);

$n_DY10to40_trigger= sprintf("%.4g", $DY10to40_trigger/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_trigger = sprintf("%.0f", 100 * $DY10to40_trigger/$DY10to40_event);

$n_Wj_trigger= sprintf("%.4g", $Wj_trigger);
$eff_Wj_trigger = sprintf("%.0f", 100 * $Wj_trigger/$Wj_event);

$n_Zj_trigger= sprintf("%.4g", $Zj_trigger);
$eff_Zj_trigger = sprintf("%.0f", 100 * $Zj_trigger/$Zj_event);

$n_ttbarj_trigger= sprintf("%.4g", $ttbarj_trigger);
$eff_ttbarj_trigger = sprintf("%.0f", 100 * $ttbarj_trigger/$ttbarj_event);

print TEXFILE1 "$n_Higgs_trigger ($eff_Higgs_trigger \\%)&\n";
print TEXFILE1 "$n_WW_trigger ($eff_WW_trigger \\%)&\n";
print TEXFILE1 "$n_ttbarj_trigger ($eff_ttbarj_trigger \\%)&\n";
print TEXFILE1 "$n_Wj_trigger ($eff_Wj_trigger \\%)&\n";
print TEXFILE1 "$n_Zj_trigger ($eff_Zj_trigger \\%)\\\\\n";

print TEXFILE2 "$n_DY10to40_trigger ($eff_DY10to40_trigger \\%)&\n";
print TEXFILE2 "$n_tW_trigger ($eff_tW_trigger \\%)&\n";
print TEXFILE2 "$n_WZ_trigger ($eff_WZ_trigger \\%)&\n";
print TEXFILE2 "$n_ZZ_trigger ($eff_ZZ_trigger \\%)\\\\\n";




# --- PRESELECTION 2lep 2nu ---
print TEXFILE1 "Pres (2l 2\$\\nu\$) &\n";
print TEXFILE2 "Pres (2l 2\$\\nu\$) &\n";
$decimals = 0;
$n_Higgs_Preselection= sprintf("%.4g", $Higgs_Preselection/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_Preselection = sprintf("%.0f", 100 * $Higgs_Preselection/$Higgs_trigger);

$n_WW_Preselection= sprintf("%.4g", $WW_Preselection/$WW_event * $WWXsec * $lumi);
$eff_WW_Preselection = sprintf("%.0f", 100 * $WW_Preselection/$WW_trigger);

$n_tW_Preselection= sprintf("%.4g", $tW_Preselection/$tW_event * $tWXsec * $lumi);
$eff_tW_Preselection = sprintf("%.0f", 100 * $tW_Preselection/$tW_trigger);

$n_WZ_Preselection= sprintf("%.4g", $WZ_Preselection/$WZ_event * $WZXsec * $lumi);
$eff_WZ_Preselection = sprintf("%.0f", 100 * $WZ_Preselection/$WZ_trigger);

$n_ZZ_Preselection= sprintf("%.4g", $ZZ_Preselection/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_Preselection = sprintf("%.0f", 100 * $ZZ_Preselection/$ZZ_trigger);

$n_DY10to40_Preselection= sprintf("%.4g", $DY10to40_Preselection/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_Preselection = sprintf("%.0f", 100 * $DY10to40_Preselection/$DY10to40_trigger);

$n_Wj_Preselection= sprintf("%.4g", $Wj_Preselection);
$eff_Wj_Preselection = sprintf("%.0f", 100 * $Wj_Preselection/$Wj_trigger);

$n_Zj_Preselection= sprintf("%.4g", $Zj_Preselection);
$eff_Zj_Preselection = sprintf("%.0f", 100 * $Zj_Preselection/$Zj_trigger);

$n_ttbarj_Preselection= sprintf("%.4g", $ttbarj_Preselection);
$eff_ttbarj_Preselection = sprintf("%.0f", 100 * $ttbarj_Preselection/$ttbarj_trigger);


print TEXFILE1 "$n_Higgs_Preselection ($eff_Higgs_Preselection \\%)&\n";
print TEXFILE1 "$n_WW_Preselection ($eff_WW_Preselection \\%)&\n";
print TEXFILE1 "$n_ttbarj_Preselection ($eff_ttbarj_Preselection \\%)&\n";
print TEXFILE1 "$n_Wj_Preselection ($eff_Wj_Preselection \\%)&\n";
print TEXFILE1 "$n_Zj_Preselection ($eff_Zj_Preselection \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_Preselection ($eff_DY10to40_Preselection \\%)&\n";
print TEXFILE2 "$n_tW_Preselection ($eff_tW_Preselection \\%)&\n";
print TEXFILE2 "$n_WZ_Preselection ($eff_WZ_Preselection \\%)&\n";
print TEXFILE2 "$n_ZZ_Preselection ($eff_ZZ_Preselection \\%) \\\\ \n";





# --- PRESELECTION 2e 2nu ---
print TEXFILE1 "Pres (2e 2\$\\nu\$) &\n";
print TEXFILE2 "Pres (2e 2\$\\nu\$) &\n";
$decimals = 0;
$n_Higgs_Preselection2e= sprintf("%.4g", $Higgs_Preselection2e/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_Preselection2e = sprintf("%.0f", 100 * $Higgs_Preselection2e/$Higgs_Preselection);

$n_WW_Preselection2e= sprintf("%.4g", $WW_Preselection2e/$WW_event * $WWXsec * $lumi);
$eff_WW_Preselection2e = sprintf("%.0f", 100 * $WW_Preselection2e/$WW_Preselection);

$n_tW_Preselection2e= sprintf("%.4g", $tW_Preselection2e/$tW_event * $tWXsec * $lumi);
$eff_tW_Preselection2e = sprintf("%.0f", 100 * $tW_Preselection2e/$tW_Preselection);

$n_WZ_Preselection2e= sprintf("%.4g", $WZ_Preselection2e/$WZ_event * $WZXsec * $lumi);
$eff_WZ_Preselection2e = sprintf("%.0f", 100 * $WZ_Preselection2e/$WZ_Preselection);

$n_ZZ_Preselection2e= sprintf("%.4g", $ZZ_Preselection2e/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_Preselection2e = sprintf("%.0f", 100 * $ZZ_Preselection2e/$ZZ_Preselection);

$n_DY10to40_Preselection2e= sprintf("%.4g", $DY10to40_Preselection2e/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_Preselection2e = sprintf("%.0f", 100 * $DY10to40_Preselection2e/$DY10to40_Preselection);

$n_Wj_Preselection2e= sprintf("%.4g", $Wj_Preselection2e);
$eff_Wj_Preselection2e = sprintf("%.0f", 100 * $Wj_Preselection2e/$Wj_Preselection);

$n_Zj_Preselection2e= sprintf("%.4g", $Zj_Preselection2e);
$eff_Zj_Preselection2e = sprintf("%.0f", 100 * $Zj_Preselection2e/$Zj_Preselection);

$n_ttbarj_Preselection2e= sprintf("%.4g", $ttbarj_Preselection2e);
$eff_ttbarj_Preselection2e = sprintf("%.0f", 100 * $ttbarj_Preselection2e/$ttbarj_Preselection);


print TEXFILE1 "$n_Higgs_Preselection2e ($eff_Higgs_Preselection2e \\%)&\n";
print TEXFILE1 "$n_WW_Preselection2e ($eff_WW_Preselection2e \\%)&\n";
print TEXFILE1 "$n_ttbarj_Preselection2e ($eff_ttbarj_Preselection2e \\%)&\n";
print TEXFILE1 "$n_Wj_Preselection2e ($eff_Wj_Preselection2e \\%)&\n";
print TEXFILE1 "$n_Zj_Preselection2e ($eff_Zj_Preselection2e \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_Preselection2e ($eff_DY10to40_Preselection2e \\%)&\n";
print TEXFILE2 "$n_tW_Preselection2e ($eff_tW_Preselection2e \\%)&\n";
print TEXFILE2 "$n_WZ_Preselection2e ($eff_WZ_Preselection2e \\%)&\n";
print TEXFILE2 "$n_ZZ_Preselection2e ($eff_ZZ_Preselection2e \\%) \\\\ \n";




# --- ELECTRON ID ---
print TEXFILE1 "electron ID &\n";
print TEXFILE2 "electron ID &\n";
$decimals = 0;
$n_Higgs_eleID= sprintf("%.4g", $Higgs_eleID/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_eleID = sprintf("%.0f", 100 * $Higgs_eleID/$Higgs_Preselection2e);

$n_WW_eleID= sprintf("%.4g", $WW_eleID/$WW_event * $WWXsec * $lumi);
$eff_WW_eleID = sprintf("%.0f", 100 * $WW_eleID/$WW_Preselection2e);

$n_tW_eleID= sprintf("%.4g", $tW_eleID/$tW_event * $tWXsec * $lumi);
$eff_tW_eleID = sprintf("%.0f", 100 * $tW_eleID/$tW_Preselection2e);

$n_WZ_eleID= sprintf("%.4g", $WZ_eleID/$WZ_event * $WZXsec * $lumi);
$eff_WZ_eleID = sprintf("%.0f", 100 * $WZ_eleID/$WZ_Preselection2e);

$n_ZZ_eleID= sprintf("%.4g", $ZZ_eleID/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_eleID = sprintf("%.0f", 100 * $ZZ_eleID/$ZZ_Preselection2e);

$n_ZZ_eleID= sprintf("%.4g", $ZZ_eleID/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_eleID = sprintf("%.0f", 100 * $ZZ_eleID/$ZZ_Preselection2e);

$n_DY10to40_eleID= sprintf("%.4g", $DY10to40_eleID/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_eleID = sprintf("%.0f", 100 * $DY10to40_eleID/$DY10to40_Preselection2e);

$n_Wj_eleID= sprintf("%.4g", $Wj_eleID);
$eff_Wj_eleID = sprintf("%.0f", 100 * $Wj_eleID/$Wj_Preselection2e);

$n_Zj_eleID= sprintf("%.4g", $Zj_eleID);
$eff_Zj_eleID = sprintf("%.0f", 100 * $Zj_eleID/$Zj_Preselection2e);

$n_ttbarj_eleID= sprintf("%.4g", $ttbarj_eleID);
$eff_ttbarj_eleID = sprintf("%.0f", 100 * $ttbarj_eleID/$ttbarj_Preselection2e);


print TEXFILE1 "$n_Higgs_eleID ($eff_Higgs_eleID \\%)&\n";
print TEXFILE1 "$n_WW_eleID ($eff_WW_eleID \\%)&\n";
print TEXFILE1 "$n_ttbarj_eleID ($eff_ttbarj_eleID \\%)&\n";
print TEXFILE1 "$n_Wj_eleID ($eff_Wj_eleID \\%)&\n";
print TEXFILE1 "$n_Zj_eleID ($eff_Zj_eleID \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_eleID ($eff_DY10to40_eleID \\%)&\n";
print TEXFILE2 "$n_tW_eleID ($eff_tW_eleID \\%)&\n";
print TEXFILE2 "$n_WZ_eleID ($eff_WZ_eleID \\%)&\n";
print TEXFILE2 "$n_ZZ_eleID ($eff_ZZ_eleID \\%) \\\\ \n";





# --- isolation ---
print TEXFILE1 "isolation &\n";
print TEXFILE2 "isolation &\n";
$decimals = 0;
$n_Higgs_Isol= sprintf("%.4g", $Higgs_Isol/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_Isol = sprintf("%.0f", 100 * $Higgs_Isol/$Higgs_eleID);

$n_WW_Isol= sprintf("%.4g", $WW_Isol/$WW_event * $WWXsec * $lumi);
$eff_WW_Isol = sprintf("%.0f", 100 * $WW_Isol/$WW_eleID);

$n_tW_Isol= sprintf("%.4g", $tW_Isol/$tW_event * $tWXsec * $lumi);
$eff_tW_Isol = sprintf("%.0f", 100 * $tW_Isol/$tW_eleID);

$n_WZ_Isol= sprintf("%.4g", $WZ_Isol/$WZ_event * $WZXsec * $lumi);
$eff_WZ_Isol = sprintf("%.0f", 100 * $WZ_Isol/$WZ_eleID);

$n_ZZ_Isol= sprintf("%.4g", $ZZ_Isol/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_Isol = sprintf("%.0f", 100 * $ZZ_Isol/$ZZ_eleID);

$n_DY10to40_Isol= sprintf("%.4g", $DY10to40_Isol/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_Isol = sprintf("%.0f", 100 * $DY10to40_Isol/$DY10to40_eleID);

$n_Wj_Isol= sprintf("%.4g", $Wj_Isol);
$eff_Wj_Isol = sprintf("%.0f", 100 * $Wj_Isol/$Wj_eleID);

$n_Zj_Isol= sprintf("%.4g", $Zj_Isol);
$eff_Zj_Isol = sprintf("%.0f", 100 * $Zj_Isol/$Zj_eleID);

$n_ttbarj_Isol= sprintf("%.4g", $ttbarj_Isol);
$eff_ttbarj_Isol = sprintf("%.0f", 100 * $ttbarj_Isol/$ttbarj_eleID);


print TEXFILE1 "$n_Higgs_Isol ($eff_Higgs_Isol \\%)&\n";
print TEXFILE1 "$n_WW_Isol ($eff_WW_Isol \\%)&\n";
print TEXFILE1 "$n_ttbarj_Isol ($eff_ttbarj_Isol \\%)&\n";
print TEXFILE1 "$n_Wj_Isol ($eff_Wj_Isol \\%)&\n";
print TEXFILE1 "$n_Zj_Isol ($eff_Zj_Isol \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_Isol ($eff_DY10to40_Isol \\%)&\n";
print TEXFILE2 "$n_tW_Isol ($eff_tW_Isol \\%)&\n";
print TEXFILE2 "$n_WZ_Isol ($eff_WZ_Isol \\%)&\n";
print TEXFILE2 "$n_ZZ_Isol ($eff_ZZ_Isol \\%) \\\\ \n";






# --- CJV ---
print TEXFILE1 "CJV &\n";
print TEXFILE2 "CJV &\n";
$decimals = 0;
$n_Higgs_jetVeto= sprintf("%.4g", $Higgs_jetVeto/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_jetVeto = sprintf("%.0f", 100 * $Higgs_jetVeto/$Higgs_Isol);

$n_WW_jetVeto= sprintf("%.4g", $WW_jetVeto/$WW_event * $WWXsec * $lumi);
$eff_WW_jetVeto = sprintf("%.0f", 100 * $WW_jetVeto/$WW_Isol);

$n_tW_jetVeto= sprintf("%.4g", $tW_jetVeto/$tW_event * $tWXsec * $lumi);
$eff_tW_jetVeto = sprintf("%.0f", 100 * $tW_jetVeto/$tW_Isol);

$n_WZ_jetVeto= sprintf("%.4g", $WZ_jetVeto/$WZ_event * $WZXsec * $lumi);
$eff_WZ_jetVeto = sprintf("%.0f", 100 * $WZ_jetVeto/$WZ_Isol);

$n_ZZ_jetVeto= sprintf("%.4g", $ZZ_jetVeto/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_jetVeto = sprintf("%.0f", 100 * $ZZ_jetVeto/$ZZ_Isol);

$n_DY10to40_jetVeto= sprintf("%.4g", $DY10to40_jetVeto/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_jetVeto = sprintf("%.0f", 100 * $DY10to40_jetVeto/$DY10to40_Isol);

$n_Wj_jetVeto= sprintf("%.4g", $Wj_jetVeto);
$eff_Wj_jetVeto = sprintf("%.0f", 100 * $Wj_jetVeto/$Wj_Isol);

$n_Zj_jetVeto= sprintf("%.4g", $Zj_jetVeto);
$eff_Zj_jetVeto = sprintf("%.0f", 100 * $Zj_jetVeto/$Zj_Isol);

$n_ttbarj_jetVeto= sprintf("%.4g", $ttbarj_jetVeto);
$eff_ttbarj_jetVeto = sprintf("%.0f", 100 * $ttbarj_jetVeto/$ttbarj_Isol);


print TEXFILE1 "$n_Higgs_jetVeto ($eff_Higgs_jetVeto \\%)&\n";
print TEXFILE1 "$n_WW_jetVeto ($eff_WW_jetVeto \\%)&\n";
print TEXFILE1 "$n_ttbarj_jetVeto ($eff_ttbarj_jetVeto \\%)&\n";
print TEXFILE1 "$n_Wj_jetVeto ($eff_Wj_jetVeto \\%)&\n";
print TEXFILE1 "$n_Zj_jetVeto ($eff_Zj_jetVeto \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_jetVeto ($eff_DY10to40_jetVeto \\%)&\n";
print TEXFILE2 "$n_tW_jetVeto ($eff_tW_jetVeto \\%)&\n";
print TEXFILE2 "$n_WZ_jetVeto ($eff_WZ_jetVeto \\%)&\n";
print TEXFILE2 "$n_ZZ_jetVeto ($eff_ZZ_jetVeto \\%) \\\\ \n";






# --- MET ---
print TEXFILE1 "MET &\n";
print TEXFILE2 "MET &\n";
$decimals = 0;
$n_Higgs_MET= sprintf("%.4g", $Higgs_MET/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_MET = sprintf("%.0f", 100 * $Higgs_MET/$Higgs_jetVeto);

$n_WW_MET= sprintf("%.4g", $WW_MET/$WW_event * $WWXsec * $lumi);
$eff_WW_MET = sprintf("%.0f", 100 * $WW_MET/$WW_jetVeto);

$n_tW_MET= sprintf("%.4g", $tW_MET/$tW_event * $tWXsec * $lumi);
$eff_tW_MET = sprintf("%.0f", 100 * $tW_MET/$tW_jetVeto);

$n_WZ_MET= sprintf("%.4g", $WZ_MET/$WZ_event * $WZXsec * $lumi);
$eff_WZ_MET = sprintf("%.0f", 100 * $WZ_MET/$WZ_jetVeto);

$n_ZZ_MET= sprintf("%.4g", $ZZ_MET/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_MET = sprintf("%.0f", 100 * $ZZ_MET/$ZZ_jetVeto);

$n_DY10to40_MET= sprintf("%.4g", $DY10to40_MET/$DY10to40_event * $DY10to40Xsec * $lumi);
$eff_DY10to40_MET = sprintf("%.0f", 100 * $DY10to40_MET/$DY10to40_jetVeto);

$n_Wj_MET= sprintf("%.4g", $Wj_MET);
$eff_Wj_MET = sprintf("%.0f", 100 * $Wj_MET/$Wj_jetVeto);

$n_Zj_MET= sprintf("%.4g", $Zj_MET);
$eff_Zj_MET = sprintf("%.0f", 100 * $Zj_MET/$Zj_jetVeto);

$n_ttbarj_MET= sprintf("%.4g", $ttbarj_MET);
$eff_ttbarj_MET = sprintf("%.0f", 100 * $ttbarj_MET/$ttbarj_jetVeto);


print TEXFILE1 "$n_Higgs_MET ($eff_Higgs_MET \\%)&\n";
print TEXFILE1 "$n_WW_MET ($eff_WW_MET \\%)&\n";
print TEXFILE1 "$n_ttbarj_MET ($eff_ttbarj_MET \\%)&\n";
print TEXFILE1 "$n_Wj_MET ($eff_Wj_MET \\%)&\n";
print TEXFILE1 "$n_Zj_MET ($eff_Zj_MET \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_MET ($eff_DY10to40_MET \\%)&\n";
print TEXFILE2 "$n_tW_MET ($eff_tW_MET \\%)&\n";
print TEXFILE2 "$n_WZ_MET ($eff_WZ_MET \\%)&\n";
print TEXFILE2 "$n_ZZ_MET ($eff_ZZ_MET \\%) \\\\ \n";








# --- max lep Pt ---
print TEXFILE1 "\$\\ptlmax \$ &\n";
print TEXFILE2 "\$\\ptlmax \$ &\n";
$decimals = 0;
$n_Higgs_maxPtLepton= sprintf("%.4g", $Higgs_maxPtLepton/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_maxPtLepton = sprintf("%.0f", 100 * $Higgs_maxPtLepton/$Higgs_MET);

$n_WW_maxPtLepton= sprintf("%.4g", $WW_maxPtLepton/$WW_event * $WWXsec * $lumi);
$eff_WW_maxPtLepton = sprintf("%.0f", 100 * $WW_maxPtLepton/$WW_MET);

$n_tW_maxPtLepton= sprintf("%.4g", $tW_maxPtLepton/$tW_event * $tWXsec * $lumi);
$eff_tW_maxPtLepton = sprintf("%.0f", 100 * $tW_maxPtLepton/$tW_MET);

$n_WZ_maxPtLepton= sprintf("%.4g", $WZ_maxPtLepton/$WZ_event * $WZXsec * $lumi);
$eff_WZ_maxPtLepton = sprintf("%.0f", 100 * $WZ_maxPtLepton/$WZ_MET);

$n_ZZ_maxPtLepton= sprintf("%.4g", $ZZ_maxPtLepton/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_maxPtLepton = sprintf("%.0f", 100 * $ZZ_maxPtLepton/$ZZ_MET);

$n_DY10to40_maxPtLepton= sprintf("%.4g", $DY10to40_maxPtLepton/$DY10to40_event * $DY10to40Xsec * $lumi);
if($DY10to40_MET!=0) {
    $eff_DY10to40_maxPtLepton = sprintf("%.0f", 100 * $DY10to40_maxPtLepton/$DY10to40_MET);
}
else {
    $eff_DY10to40_maxPtLepton = 0;
}

$n_Wj_maxPtLepton= sprintf("%.4g", $Wj_maxPtLepton);
$eff_Wj_maxPtLepton = sprintf("%.0f", 100 * $Wj_maxPtLepton/$Wj_MET);

$n_Zj_maxPtLepton= sprintf("%.4g", $Zj_maxPtLepton);
$eff_Zj_maxPtLepton = sprintf("%.0f", 100 * $Zj_maxPtLepton/$Zj_MET);

$n_ttbarj_maxPtLepton= sprintf("%.4g", $ttbarj_maxPtLepton);
$eff_ttbarj_maxPtLepton = sprintf("%.0f", 100 * $ttbarj_maxPtLepton/$ttbarj_MET);


print TEXFILE1 "$n_Higgs_maxPtLepton ($eff_Higgs_maxPtLepton \\%)&\n";
print TEXFILE1 "$n_WW_maxPtLepton ($eff_WW_maxPtLepton \\%)&\n";
print TEXFILE1 "$n_ttbarj_maxPtLepton ($eff_ttbarj_maxPtLepton \\%)&\n";
print TEXFILE1 "$n_Wj_maxPtLepton ($eff_Wj_maxPtLepton \\%)&\n";
print TEXFILE1 "$n_Zj_maxPtLepton ($eff_Zj_maxPtLepton \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_maxPtLepton ($eff_DY10to40_maxPtLepton \\%)&\n";
print TEXFILE2 "$n_tW_maxPtLepton ($eff_tW_maxPtLepton \\%)&\n";
print TEXFILE2 "$n_WZ_maxPtLepton ($eff_WZ_maxPtLepton \\%)&\n";
print TEXFILE2 "$n_ZZ_maxPtLepton ($eff_ZZ_maxPtLepton \\%) \\\\ \n";






# --- min lep Pt ---
print TEXFILE1 "\$\\ptlmin \$ &\n";
print TEXFILE2 "\$\\ptlmin \$ &\n";
$decimals = 0;
$n_Higgs_minPtLepton= sprintf("%.4g", $Higgs_minPtLepton/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_minPtLepton = sprintf("%.0f", 100 * $Higgs_minPtLepton/$Higgs_maxPtLepton);

$n_WW_minPtLepton= sprintf("%.4g", $WW_minPtLepton/$WW_event * $WWXsec * $lumi);
$eff_WW_minPtLepton = sprintf("%.0f", 100 * $WW_minPtLepton/$WW_maxPtLepton);

$n_tW_minPtLepton= sprintf("%.4g", $tW_minPtLepton/$tW_event * $tWXsec * $lumi);
$eff_tW_minPtLepton = sprintf("%.0f", 100 * $tW_minPtLepton/$tW_maxPtLepton);

$n_WZ_minPtLepton= sprintf("%.4g", $WZ_minPtLepton/$WZ_event * $WZXsec * $lumi);
$eff_WZ_minPtLepton = sprintf("%.0f", 100 * $WZ_minPtLepton/$WZ_maxPtLepton);

$n_ZZ_minPtLepton= sprintf("%.4g", $ZZ_minPtLepton/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_minPtLepton = sprintf("%.0f", 100 * $ZZ_minPtLepton/$ZZ_maxPtLepton);

$n_DY10to40_minPtLepton= sprintf("%.4g", $DY10to40_minPtLepton/$DY10to40_event * $DY10to40Xsec * $lumi);
if($DY10to40_maxPtLepton!=0) {
    $eff_DY10to40_minPtLepton = sprintf("%.0f", 100 * $DY10to40_minPtLepton/$DY10to40_maxPtLepton);
}
else {
    $eff_DY10to40_minPtLepton = 0;
}

$n_Wj_minPtLepton= sprintf("%.4g", $Wj_minPtLepton);
$eff_Wj_minPtLepton = sprintf("%.0f", 100 * $Wj_minPtLepton/$Wj_maxPtLepton);

$n_Zj_minPtLepton= sprintf("%.4g", $Zj_minPtLepton);
$eff_Zj_minPtLepton = sprintf("%.0f", 100 * $Zj_minPtLepton/$Zj_maxPtLepton);

$n_ttbarj_minPtLepton= sprintf("%.4g", $ttbarj_minPtLepton);
$eff_ttbarj_minPtLepton = sprintf("%.0f", 100 * $ttbarj_minPtLepton/$ttbarj_maxPtLepton);


print TEXFILE1 "$n_Higgs_minPtLepton ($eff_Higgs_minPtLepton \\%)&\n";
print TEXFILE1 "$n_WW_minPtLepton ($eff_WW_minPtLepton \\%)&\n";
print TEXFILE1 "$n_ttbarj_minPtLepton ($eff_ttbarj_minPtLepton \\%)&\n";
print TEXFILE1 "$n_Wj_minPtLepton ($eff_Wj_minPtLepton \\%)&\n";
print TEXFILE1 "$n_Zj_minPtLepton ($eff_Zj_minPtLepton \\%)\\\\\n";

print TEXFILE2 "$n_DY10to40_minPtLepton ($eff_DY10to40_minPtLepton \\%)&\n";
print TEXFILE2 "$n_tW_minPtLepton ($eff_tW_minPtLepton \\%)&\n";
print TEXFILE2 "$n_WZ_minPtLepton ($eff_WZ_minPtLepton \\%)&\n";
print TEXFILE2 "$n_ZZ_minPtLepton ($eff_ZZ_minPtLepton \\%)\\\\\n";









# --- ee inv mass ---
print TEXFILE1 "\$\\mll \$ &\n";
print TEXFILE2 "\$\\mll \$ &\n";
$decimals = 0;
$n_Higgs_eleInvMass= sprintf("%.4g", $Higgs_eleInvMass/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_eleInvMass = sprintf("%.0f", 100 * $Higgs_eleInvMass/$Higgs_minPtLepton);

$n_WW_eleInvMass= sprintf("%.4g", $WW_eleInvMass/$WW_event * $WWXsec * $lumi);
$eff_WW_eleInvMass = sprintf("%.0f", 100 * $WW_eleInvMass/$WW_minPtLepton);

$n_tW_eleInvMass= sprintf("%.4g", $tW_eleInvMass/$tW_event * $tWXsec * $lumi);
$eff_tW_eleInvMass = sprintf("%.0f", 100 * $tW_eleInvMass/$tW_minPtLepton);

$n_WZ_eleInvMass= sprintf("%.4g", $WZ_eleInvMass/$WZ_event * $WZXsec * $lumi);
$eff_WZ_eleInvMass = sprintf("%.0f", 100 * $WZ_eleInvMass/$WZ_minPtLepton);

$n_ZZ_eleInvMass= sprintf("%.4g", $ZZ_eleInvMass/$ZZ_event * $ZZXsec * $lumi);
$eff_ZZ_eleInvMass = sprintf("%.0f", 100 * $ZZ_eleInvMass/$ZZ_minPtLepton);

$n_DY10to40_eleInvMass= sprintf("%.4g", $DY10to40_eleInvMass/$DY10to40_event * $DY10to40Xsec * $lumi);
if($DY10to40_minPtLepton!=0) {
    $eff_DY10to40_eleInvMass = sprintf("%.0f", 100 * $DY10to40_eleInvMass/$DY10to40_minPtLepton);
}
else {
    $eff_DY10to40_eleInvMass = 0;
}

$n_Wj_eleInvMass= sprintf("%.4g", $Wj_eleInvMass);
$eff_Wj_eleInvMass = sprintf("%.0f", 100 * $Wj_eleInvMass/$Wj_minPtLepton);

$n_Zj_eleInvMass= sprintf("%.4g", $Zj_eleInvMass);
$eff_Zj_eleInvMass = sprintf("%.0f", 100 * $Zj_eleInvMass/$Zj_minPtLepton);

$n_ttbarj_eleInvMass= sprintf("%.4g", $ttbarj_eleInvMass);
$eff_ttbarj_eleInvMass = sprintf("%.0f", 100 * $ttbarj_eleInvMass/$ttbarj_minPtLepton);


print TEXFILE1 "$n_Higgs_eleInvMass ($eff_Higgs_eleInvMass \\%)&\n";
print TEXFILE1 "$n_WW_eleInvMass ($eff_WW_eleInvMass \\%)&\n";
print TEXFILE1 "$n_ttbarj_eleInvMass ($eff_ttbarj_eleInvMass \\%)&\n";
print TEXFILE1 "$n_Wj_eleInvMass ($eff_Wj_eleInvMass \\%)&\n";
print TEXFILE1 "$n_Zj_eleInvMass ($eff_Zj_eleInvMass \\%)\\\\\n";

print TEXFILE2 "$n_DY10to40_eleInvMass ($eff_DY10to40_eleInvMass \\%)&\n";
print TEXFILE2 "$n_tW_eleInvMass ($eff_tW_eleInvMass \\%)&\n";
print TEXFILE2 "$n_WZ_eleInvMass ($eff_WZ_eleInvMass \\%)&\n";
print TEXFILE2 "$n_ZZ_eleInvMass ($eff_ZZ_eleInvMass \\%)\\\\\n";






# --- delta phi ---
print TEXFILE1 "\$\\Delta \\phi\$ &\n";
print TEXFILE2 "\$\\Delta \\phi\$ &\n";
$decimals = 0;
$n_Higgs_deltaPhi= sprintf("%.4g", $Higgs_deltaPhi/$Higgs_event * $HiggsXsec * $lumi);
$eff_Higgs_deltaPhi = sprintf("%.0f", 100 * $Higgs_deltaPhi/$Higgs_eleInvMass);

$n_WW_deltaPhi= sprintf("%.4g", $WW_deltaPhi/$WW_event * $WWXsec * $lumi);
$eff_WW_deltaPhi = sprintf("%.0f", 100 * $WW_deltaPhi/$WW_eleInvMass);

$n_tW_deltaPhi= sprintf("%.4g", $tW_deltaPhi/$tW_event * $tWXsec * $lumi);
$eff_tW_deltaPhi = sprintf("%.0f", 100 * $tW_deltaPhi/$tW_eleInvMass);

$n_WZ_deltaPhi= sprintf("%.4g", $WZ_deltaPhi/$WZ_event * $WZXsec * $lumi);
$eff_WZ_deltaPhi = sprintf("%.0f", 100 * $WZ_deltaPhi/$WZ_eleInvMass);

$n_ZZ_deltaPhi= sprintf("%.4g", $ZZ_deltaPhi/$ZZ_event * $ZZXsec * $lumi);
if($ZZ_eleInvMass!=0) {
    $eff_ZZ_deltaPhi = sprintf("%.0f", 100 * $ZZ_deltaPhi/$ZZ_eleInvMass);
}
else{
    $eff_ZZ_deltaPhi = 0;
}
$n_DY10to40_deltaPhi= sprintf("%.4g", $DY10to40_deltaPhi/$DY10to40_event * $DY10to40Xsec * $lumi);
if($DY10to40_eleInvMass!=0) {
    $eff_DY10to40_deltaPhi = sprintf("%.0f", 100 * $DY10to40_deltaPhi/$DY10to40_eleInvMass);
}
else{
    $eff_DY10to40_deltaPhi = 0;
}
$n_Wj_deltaPhi= sprintf("%.4g", $Wj_deltaPhi);
$eff_Wj_deltaPhi = sprintf("%.0f", 100 * $Wj_deltaPhi/$Wj_eleInvMass);

$n_Zj_deltaPhi= sprintf("%.4g", $Zj_deltaPhi);
$eff_Zj_deltaPhi = sprintf("%.0f", 100 * $Zj_deltaPhi/$Zj_eleInvMass);

$n_ttbarj_deltaPhi= sprintf("%.4g", $ttbarj_deltaPhi);
$eff_ttbarj_deltaPhi = sprintf("%.0f", 100 * $ttbarj_deltaPhi/$ttbarj_eleInvMass);



print TEXFILE1 "$n_Higgs_deltaPhi ($eff_Higgs_deltaPhi \\%)&\n";
print TEXFILE1 "$n_WW_deltaPhi ($eff_WW_deltaPhi \\%)&\n";
print TEXFILE1 "$n_ttbarj_deltaPhi ($eff_ttbarj_deltaPhi \\%)&\n";
print TEXFILE1 "$n_Wj_deltaPhi ($eff_Wj_deltaPhi \\%)&\n";
print TEXFILE1 "$n_Zj_deltaPhi ($eff_Zj_deltaPhi \\%) \\\\ \n";

print TEXFILE2 "$n_DY10to40_deltaPhi ($eff_DY10to40_deltaPhi \\%)&\n";
print TEXFILE2 "$n_tW_deltaPhi ($eff_tW_deltaPhi \\%)&\n";
print TEXFILE2 "$n_WZ_deltaPhi ($eff_WZ_deltaPhi \\%)&\n";
print TEXFILE2 "$n_ZZ_deltaPhi ($eff_ZZ_deltaPhi \\%) \\\\ \n";




# final tables

print TEXFILE1 "\\hline \n";
print TEXFILE1 "\\hline \n";
print TEXFILE1 "\\end\{tabular\}\n";
print TEXFILE1 "\\caption\{\\emph\{Mass hypothesis: $HiggsMass GeV\/c\$^2\$. Number of expected events for an integrated luminosity of $lumi pb\$^\{-1\}\$ after each selection,
for \$H \\rightarrow WW\$, \$WW\$, \$t \\bar\{t\}\$, \$W + jets \$\ and \$Z + jets\$. The relative efficiencies with respect to the previous cut are given in percent within the brackets.\}\}\n";
print TEXFILE1 "\\label\{tab:eff$HiggsMass\}\n";
print TEXFILE1 "\\end\{center\}\n";
print TEXFILE1 "\\end\{sidewaystable\}\n";


print TEXFILE2 "\\hline \n";
print TEXFILE2 "\\hline \n";
print TEXFILE2 "\\end\{tabular\}\n";
print TEXFILE2 "\\caption\{\\emph\{Mass hypothesis: $HiggsMass GeV\/c\$^2\$. Number of expected events for an integrated luminosity of $lumi pb\$^\{-1\}\$ after each selection,
for \$WZ\$, \$tW\$ and \$ZZ\$.
. The relative efficiencies with respect to the previous cut are given in percent within the brackets.\}\}\n";
print TEXFILE2 "\\label\{tab:eff$HiggsMass\}\n";
print TEXFILE2 "\\end\{center\}\n";
print TEXFILE2 "\\end\{sidewaystable\}\n";

print "Done. Include in your tex and compile.\n";


