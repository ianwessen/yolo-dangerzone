#! /usr/bin/perl

use Getopt::Std;
use Cwd;

getopts('l:m:n:');
if($opt_l) {$lumiEE = $opt_l;}
else { die "usage: ./makeTables.pl -l <lumiEE (pb-1)> -m <lumiMM (pb-1)> -n <lumiEM (pb-1)>";}
if($opt_m) {$lumiMM = $opt_m;}
else { die "usage: ./makeTables.pl -l <lumiEE (pb-1)> -m <lumiMM (pb-1)> -n <lumiEM (pb-1)>";}
if($opt_n) {$lumiEM = $opt_n;}
else { die "usage: ./makeTables.pl -l <lumiEE (pb-1)> -m <lumiMM (pb-1)> -n <lumiEM (pb-1)>";}

#$topDir="/cmsrm/pc23_2/emanuele/data/Higgs4.1.X/MC2011_WP90_V2/";
#$suffix="Spring11";
#$topDirData="/cmsrm/pc23_2/emanuele/data/Higgs4.1.X/Data2011_WP90_V2";
#$suffixData="Data7TeV";
$curDir = cwd();

system("rm -f HiggsTables/yields_byCut.tex");
system("mkdir -p HiggsTables");
open(TEX,">HiggsTables/yields_byCut.tex");
print TEX "\\documentclass{article}\n";
print TEX "\\setlength\\textheight{9.8in}\n";
print TEX "\\usepackage{rotating}\n";
print TEX "\\begin{document}\n";

# the detailed tables
@masses = (120,130,140,150,160,170,180,190,200,210,220,230,250,300,350,400,450,500,550,600);
#@masses = (120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600);

for($i=0; $i<($#masses+1); $i++) {
    # configure
    $mass = $masses[$i];
    print "-------------------------->\n";
    print "*** COMPUTING YIELDS FOR MASS SELECTION: mH = $mass ... *** \n\n";
    #system("rm -f results results_data");
    #$dirMc=$topDir."OptimMH$mass/".$suffix;
    #$dirData=$topDirData."OptimMH$mass/".$suffixData."/OptimMH$mass";
    #system("/bin/ln -s $dirMc results");
    #system("/bin/ln -s $dirData results_data");

    # prepare the script to be executed
    open(SCRIPT,">script.sh");
    print SCRIPT "#/bin/sh\n";
    print SCRIPT "root -l -b <<EOF\n";
    print SCRIPT ".L HiggsYieldsSteps.cc+\n";
    print SCRIPT "setRunStandalone(false)\n";
    print SCRIPT "setPrintCutsBreakdown(false)\n";
    print SCRIPT "printLatex($lumiEE,$lumiMM,$lumiEM,$mass)\n";
    print SCRIPT "EOF\n";
    
    # run the macro and compile latex table
    system("chmod a+x $curDir/script.sh");
    system("source $curDir/script.sh;");
    system("cat yields_byCut.tex >> HiggsTables/yields_byCut.tex");
}

# the super-summary table
print "NOW DOING THE SUPERSUMMARY....\n\n";
system ("rm -f yieldsSummary_byCut.tex");
for($i=0; $i<7; $i++) {
    print "\tDoing supersummary $i/3...\n";
    open(SCRIPT,">script.sh");
    print SCRIPT "#/bin/sh\n";
    print SCRIPT "root -l -b <<EOF\n"; 
    print SCRIPT ".L HiggsYieldsSteps.cc+\n";
    print SCRIPT "setRunStandalone(false)\n";
    print SCRIPT "setPrintCutsBreakdown(false)\n";
    print SCRIPT "printSuperSummary($lumiEE,$lumiMM,$lumiEM,$i)\n";
    print SCRIPT "EOF\n";

    # run the macro and compile latex table
    system("chmod a+x $curDir/script.sh");
    system("source $curDir/script.sh;");
}

system("cat yieldsSummary_byCut.tex >> HiggsTables/yields_byCut.tex");
system("echo \"\\\"end{document} >> HiggsTables/yields_byCut.tex");

print "DONE. Tables in: HiggsTables/yields_byCut.tex\n";
