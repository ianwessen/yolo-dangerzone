#include <iostream>
#include <fstream>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "massDependentCuts.cc"

using namespace std;

enum { ee=0, mm=1, em=2, me=3, ll=4 };

float yieldErrPoisson(float nEst1, float n1, float nEst2=0, float n2=0, float nEst3=0, float n3=0, float nEst4=0, float n4=0, float nEst5=0, float n5=0, float nEst6=0, float n6=0);
void HiggsYields(int mH, int njets, float lumiInInvFb, bool showData, bool addDataDrivenEstimates, int imass);
void printLatex(float lumiInInvFb, bool showData, bool addDataDrivenEstimates);

void printLatex(float lumiInInvFb, bool showData, bool addDataDrivenEstimates) {

  ofstream txtfilefordatacard;
  txtfilefordatacard.open("mcyields.txt", ios_base::trunc);

  ofstream txtfileforvgammadatacard;
  txtfileforvgammadatacard.open("vgamma-yields-mc.txt", ios_base::trunc);

  ofstream textfile;
  textfile.open("yields.tex", ios_base::trunc);

  textfile << "\\documentclass{article}" << endl;
  textfile << "\\setlength\\textheight{9.8in}" << endl;
  textfile << "\\usepackage{fullpage}" << endl;
  textfile << "\\begin{document}" << endl << endl;
  textfile << "\\section{Yields for 0 jet}" << endl;

  textfile << "\\subsection{Yields at WW selection level}" << std::endl;
  std::cout << "Evaluating yields at WW selection level" << std::endl;
  textfile << "Yields for an integrated luminosity $L_{int} = $ " << lumiInInvFb << " fb$^{-1}$." << std::endl;
  if(addDataDrivenEstimates) textfile << "All the following yields are corrected by the data driven estimates, where possible. W+jets is taken from T+L sample on data." << std::endl;
  else textfile << "All the following yields are pure MC estimates, without data-driven corrections" << std::endl;
  HiggsYields(-1, 0, lumiInInvFb, showData, addDataDrivenEstimates, -1);

  textfile.open("yields.tex", ios_base::app);
  textfile << "\\subsection{Yields at full H selection level}" << std::endl;

  int mH[27] = {110,115,120,125,130,135,140,145,150,155,160,170,180,190,200,250,300,350,400,450,500,550,600,700,800,900,1000};
  for(int i=0; i<27;i++) {
    std::cout << "mH = " << mH[i] << "\t0 jet" << std::endl;
    HiggsYields(mH[i], 0, lumiInInvFb, showData, addDataDrivenEstimates, i);
  }
  textfile.close();

  ofstream textfile2;
  textfile2.open("yields.tex", ios_base::app);
  textfile2 << "\\cleardoublepage" << endl << endl;
  textfile2 << "\\section{Yields for 1 jet}" << endl;
  textfile2 << "\\subsection{Yields at WW selection level}" << std::endl;
  std::cout << "Evaluating yields at WW selection level" << std::endl;
  HiggsYields(-1, 1, lumiInInvFb, showData, addDataDrivenEstimates, -1);

  textfile2.open("yields.tex", ios_base::app);
  textfile2 << "\\subsection{Yields at full H selection level}" << std::endl;
  for(int i=0; i<27;i++) {
    std::cout << "mH = " << mH[i] << "\t1 jet" << std::endl;
    HiggsYields(mH[i], 1, lumiInInvFb, showData, addDataDrivenEstimates, i);
  }
  textfile2.close();

  ofstream textfile3;
  textfile3.open("yields.tex", ios_base::app);
  textfile3 << "\\end{document}" << endl << endl;

}

void HiggsYields(int mH, int njets, float lumiInInvFb, bool showData, bool addDataDrivenEstimates, int imass) {

  std::vector<std::vector<double> > yields;
  std::vector<std::vector<double> > yields_err;

  std::vector<float> yields_bkgtot, yields_bkgtot_err;
  std::vector<float> yields_data;
  for(int icha=0; icha<5; icha++) {
    yields_bkgtot.push_back(0.);
    yields_bkgtot_err.push_back(0.);
    yields_data.push_back(0.);
  }

  std::vector<std::string> sampleName;
  sampleName.push_back("Z/$\\gamma^*$");
  sampleName.push_back("$t \\bar{t}$");
  sampleName.push_back("single t");
  sampleName.push_back("W+jets");
  sampleName.push_back("W$\\gamma^{(*)}$");
  sampleName.push_back("WZ+ZZ");
  sampleName.push_back("qq$\\to$ WW");
  sampleName.push_back("gg$\\to$ WW");
  char mass[10];
  sprintf(mass,"H%d",mH);
  if(mH!=-1) sampleName.push_back(std::string(mass));
  else  sampleName.push_back("H125"); // take one mass as example

  // open files
  TFile *fileZj = TFile::Open("results/datasets_trees/Zjets_ll.root");
  TFile *fileTTbar = TFile::Open("results/datasets_trees/top_ll.root");
  TFile *fileSingleTop = TFile::Open("results/datasets_trees/top_ll.root");
  TFile *fileWj = TFile::Open("results/datasets_trees/Wjets_ll.root");
  TFile *fileOthers = TFile::Open("results/datasets_trees/others_ll.root");
  TFile *fileqqWW = TFile::Open("results/datasets_trees/WW_ll.root");
  TFile *fileggWW = TFile::Open("results/datasets_trees/WW_ll.root");
  TFile *fileData = 0;
  if(showData) fileData = TFile::Open("results_data/datasets_trees/dataset_ll.root");
  TFile *fileFake = 0;
  if(addDataDrivenEstimates) fileFake = TFile::Open("results_data/datasets_trees/dataset_looseloose.root");

  char signalFile[200];
  if(mH!=-1) sprintf(signalFile, "results/datasets_trees/H%d_ll.root", mH);
  else sprintf(signalFile, "results/datasets_trees/H125_ll.root"); // take one mass as example
  TFile *fileH = TFile::Open(signalFile);

  // get trees
  TTree* treeZj = (TTree*)fileZj->Get("latinoWWskim");
  TTree* treeTTbar = (TTree*)fileTTbar->Get("latinoWWskim");
  TTree* treeSingleTop = (TTree*)fileSingleTop->Get("latinoWWskim");
  TTree* treeWj = (TTree*)fileWj->Get("latinoWWskim");
  TTree* treeOthers = (TTree*)fileOthers->Get("latinoWWskim");
  TTree* treeqqWW = (TTree*)fileqqWW->Get("latinoWWskim");
  TTree* treeggWW = (TTree*)fileggWW->Get("latinoWWskim");
  TTree* treeH = (TTree*)fileH->Get("latinoWWskim");
  TTree* treeggH = (TTree*)fileH->Get("latinoWWskim");
  TTree* treeqqH = (TTree*)fileH->Get("latinoWWskim");
  TTree* treeVgamma = (TTree*)fileOthers->Get("latinoWWskim");
  TTree* treeData = 0;
  if(showData) treeData = (TTree*)fileData->Get("latinoWWskim");
  TTree* treeFake = 0;
  if(addDataDrivenEstimates) {
    treeFake = (TTree*)fileFake->Get("latino");
    treeFake->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");
  }

  std::vector<TTree*> trees;
  trees.push_back(treeZj); // 0
  trees.push_back(treeTTbar); // 1
  trees.push_back(treeSingleTop); // 2
  trees.push_back(treeWj); // 3
  trees.push_back(treeVgamma); // 4
  trees.push_back(treeOthers); // 5
  trees.push_back(treeqqWW); // 6
  trees.push_back(treeggWW); // 7
  trees.push_back(treeH); // 8
  trees.push_back(treeggH); // 9
  trees.push_back(treeqqH); // 10

   // evaluated with LP11 dataset (1.54 fb-1)
   std::vector<float> sfs_0j;
   sfs_0j.push_back(3.0); // 0
   sfs_0j.push_back(1.03); // 1
   sfs_0j.push_back(1.03); // 2
   sfs_0j.push_back(1.0); // 3
   sfs_0j.push_back(1.0); // 4
   sfs_0j.push_back(1.0); // 5
   sfs_0j.push_back(1.2); // 6
   sfs_0j.push_back(1.2); // 7
   sfs_0j.push_back(1.0); // 8
   sfs_0j.push_back(1.0); // 9
   sfs_0j.push_back(1.0); // 10

   std::vector<float> sfs_1j;
   sfs_1j.push_back(3.0); // 0
   sfs_1j.push_back(1.09); // 1
   sfs_1j.push_back(1.09); // 2
   sfs_1j.push_back(1.0); // 3
   sfs_1j.push_back(1.0); // 4
   sfs_1j.push_back(1.0); // 5
   sfs_1j.push_back(0.8); // 6
   sfs_1j.push_back(0.8); // 7
   sfs_1j.push_back(1.0); // 8
   sfs_1j.push_back(1.0); // 9
   sfs_1j.push_back(1.0); // 10

  TH1F *histo = new TH1F("histo","histo",100,0,180);
  
  char njcut[30];
  sprintf(njcut, "njet==%d", njets);
  char wwselcut[700];

  // the final met cut depends on the H mass: included in the massDependentCuts()
  if(njets==0) sprintf(wwselcut,"(step[14] && njet==0)");      
  else if(njets==1) sprintf(wwselcut,"(step[14] && njet==1)");
  else {
    std::cout << "Jet bin must be 0/1" << std::endl;
    return;
  }

  TString higgsMassDependentCut;
  if(mH==-1) higgsMassDependentCut = (njets==0) ? TString("(dymva1>0.6 || !sameflav) && njet==0") : TString("(dymva1>0.3 || !sameflav) && njet==1"); // i.e. give the yields at WW level
  else higgsMassDependentCut = higgsCuts(mH,true,njets);

  for(int isample=0; isample<(int)trees.size(); isample++) {
    TTree *tree = trees[isample];

    std::vector<TString> cutChannel;
    TString HCut_ee = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==1)*baseW*puW*1");
    TString HCut_mm = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==0)*baseW*puW*1");
    TString HCut_em = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==2)*baseW*puW*1");
    TString HCut_me = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==3)*baseW*puW*1");
    
    cutChannel.push_back(HCut_ee);
    cutChannel.push_back(HCut_mm);
    cutChannel.push_back(HCut_em);
    cutChannel.push_back(HCut_me);

    // for W+jets val
    std::vector<TString> cutChannelFake;
    HCut_ee = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==1)*fake2W");
    HCut_mm = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==0)*fake2W");
    HCut_em = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==2)*fake2W");
    HCut_me = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==3)*fake2W");
          
    cutChannelFake.push_back(HCut_ee);
    cutChannelFake.push_back(HCut_mm);
    cutChannelFake.push_back(HCut_em);
    cutChannelFake.push_back(HCut_me);

    // error (statistical only)
    std::vector<TString> cutChannelFakeStatErr;
    HCut_ee = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==1)*fake2W*fake2W");
    HCut_mm = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==0)*fake2W*fake2W");
    HCut_em = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==2)*fake2W*fake2W");
    HCut_me = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==3)*fake2W*fake2W");

    cutChannelFakeStatErr.push_back(HCut_ee);
    cutChannelFakeStatErr.push_back(HCut_mm);
    cutChannelFakeStatErr.push_back(HCut_em);
    cutChannelFakeStatErr.push_back(HCut_me);
    
    std::vector<double> sampleYield, sampleYield_err;
    for(int icha=0; icha<4; icha++) {

      // some ROOT file contains more than 1 bkg
      // split top / ttbar
      TString addCut=TString("");
      if(isample==1) addCut = TString("*(dataset==10)");
      if(isample==2) addCut = TString("*(dataset>=13 && dataset<=20)");
      // split qqWW and ggWW
      if(isample==6) addCut = TString("*(dataset==0)");
      if(isample==7) addCut = TString("*(dataset==1)");
      // separate vbfH for the datacards 
      if(isample==9) addCut = TString("*((dataset>1000 && dataset<3000) || (dataset>9000 && dataset<10000))");
      if(isample==10) addCut = TString("*((dataset>4000 && dataset<6000) || (dataset>8000 && dataset<9000))");
      // separate WZ/ZZ from Vgamma
      if(isample==4) addCut = TString("*((dataset>=82 && dataset<=85))");
      if(isample==5) addCut = TString("*(dataset==71 || dataset==74)");

      TString TheFinalCut = cutChannel[icha]+addCut;

      tree->Project("histo","dphill",TheFinalCut);
      double yield, yield_err;
      if(!addDataDrivenEstimates) {
        yield = histo->Integral();
        yield_err = yieldErrPoisson(yield,histo->GetEntries());
      } else {
        if((isample==0 && icha>1) || ((isample==6 || isample==7) && mH>=200)) { 
          // for em,me the Z+jets background is Z->tautau with real MET, so do not apply the SF used for Z->mumu/elel
          // for WW, mH>=200 GeV, use pure MC
          yield = histo->Integral();
          yield_err = yieldErrPoisson(yield,histo->GetEntries());
        } else if(isample==3) { // W+jets: take from T+F data tree

          treeFake->Project("histo","dphill",cutChannelFake[icha]);
          yield = histo->Integral();
          
          treeFake->Project("histo","dphill",cutChannelFakeStatErr[icha]);
          yield_err = sqrt(histo->Integral());

        } else {
          std::vector<float> sfs;
          if(njets==0) sfs = sfs_0j;
          else if(njets==1) sfs = sfs_1j;
          else {
            std::cout << "ERROR: njets must be 0 or 1" << std::endl;
            return;
          }
          yield = histo->Integral() * sfs[isample];
          yield_err = yieldErrPoisson(yield,histo->GetEntries()) * sfs[isample];
        }
      }

      if(addDataDrivenEstimates && isample==3) { // W+jets is absolute number from data
        sampleYield.push_back(yield);
        sampleYield_err.push_back(yield_err);
      } else {
        sampleYield.push_back(lumiInInvFb * yield);
        sampleYield_err.push_back(lumiInInvFb * yield_err);
      }

      // sum the backgrounds (signal is the last)
      if(isample < (int)trees.size()-3) {
        if(addDataDrivenEstimates && isample==3) { // W+jets is absolute number from data
          yields_bkgtot[icha] += yield;
          yields_bkgtot_err[icha] += yield_err * yield_err;
        } else {
          yields_bkgtot[icha] += lumiInInvFb * yield;
          yields_bkgtot_err[icha] += lumiInInvFb * yield_err * lumiInInvFb * yield_err;
        }
      }
    }
    yields.push_back(sampleYield);
    yields_err.push_back(sampleYield_err);

  }

  // calculate the total of each sample
  for(int isample=0; isample<(int)trees.size(); isample++) {
    // sum the 4 channels for each sample
    std::vector<double> sampleYield = yields[isample];
    std::vector<double> sampleYield_err = yields_err[isample];
    double totSample = 0;
    double totSample_err = 0;
    for(int icha=0; icha<4; icha++) {
      totSample += sampleYield[icha];
      totSample_err += pow(sampleYield_err[icha], 2);
    }
    (yields[isample]).push_back(totSample);
    (yields_err[isample]).push_back(sqrt(totSample_err));
  }

  // calculate the total background
  for(int isample=0; isample<(int)trees.size()-3; isample++) {
    yields_bkgtot[4] += (yields[isample])[4];
    yields_bkgtot_err[4] += pow( ((yields_err[isample])[4]), 2);
  }

  // quadrature sum of the tot bkg error, including the smaple which is the sum of 4 channels
  for(int icha=0; icha<5; icha++) {
    yields_bkgtot_err[icha] = sqrt(yields_bkgtot_err[icha]);
  }

  // data counting
  for(int icha=0; icha<5; icha++) {
    std::vector<TString> cutChannel;
    TString HCut_ee = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==1)");
    TString HCut_mm = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==0)");
    TString HCut_em = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==2)");
    TString HCut_me = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(" && channel==3)");
    TString HCut_all = TString("(")+TString(wwselcut)+TString(" && ")+higgsMassDependentCut+TString(")");
    
    cutChannel.push_back(HCut_ee);
    cutChannel.push_back(HCut_mm);
    cutChannel.push_back(HCut_em);
    cutChannel.push_back(HCut_me);
    cutChannel.push_back(HCut_all);
    
    if(showData) treeData->Project("histo","dphill",cutChannel[icha]);
    double yield = histo->Integral();
    yields_data[icha] = yield;
  }

  ofstream textfile;
  textfile.open("yields.tex", ios_base::app);
  textfile.setf(ios::fixed,ios::floatfield);
  textfile.precision(1);
  textfile << "\\begin{table}[p]" << endl;
  if(mH!=-1) textfile << "\\begin{small}" << endl;
  else textfile << "\\begin{scriptsize}" << endl;
  textfile << "\\begin{center}" << endl;
  if(showData) textfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|c|}" << endl;
  else textfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;

  // header
  for(int isample=0; isample<(int)trees.size()-2; isample++) {
    if(isample==0) textfile << "\t & " << sampleName[isample] << " & ";
    else if(isample==(int)trees.size()-3 && !showData) textfile << sampleName[isample] << " \\\\ " << std::endl;
    else if(isample==(int)trees.size()-3 && showData) textfile << sampleName[isample] << " & data \\\\ " << std::endl;
    else if(isample==(int)trees.size()-4) textfile << sampleName[isample] << " & bkg. tot. & ";
    else textfile << sampleName[isample] << " & ";
  }

  textfile << "\\hline" << std::endl;

  std::vector<TString> chanName;
  chanName.push_back("ee");
  chanName.push_back("$\\mu\\mu$");
  chanName.push_back("e$\\mu$");
  chanName.push_back("$\\mu$e");
  chanName.push_back("all");

  // yields
  for(int icha=0; icha<5; icha++) {
    for(int isample=0; isample<(int)trees.size()-2; isample++) {
      std::vector<double> sampleYiled = yields[isample];
      std::vector<double> sampleYiled_err = yields_err[isample];

      double val = sampleYiled[icha];
      double err = sampleYiled_err[icha];
      
      if(isample==0) textfile << chanName[icha] << " & " << val << " $\\pm$  " << err << " & ";
      else if(isample==(int)trees.size()-3 && !showData) textfile << val << " $\\pm$  " << err << " \\\\ " << std::endl;
      else if(isample==(int)trees.size()-3 && showData) textfile << val << " $\\pm$  " << err << " & " << yields_data[icha] << " \\\\ " << std::endl;
      else if(isample==(int)trees.size()-4) textfile << val << " $\\pm$  " << err << " & " << yields_bkgtot[icha] << " $\\pm$  " << yields_bkgtot_err[icha] << " & ";
      else textfile << val << " $\\pm$  " << err << " & ";
    }
    if(icha==3) textfile << "\\hline" << std::endl;
  }

  // trailer
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl;
  if(mH==-1) textfile << "\\caption{WW selection level, " << njets << " jet.}" << std::endl;
  else textfile << "\\caption{Higgs $m_H$ = " << mH << " GeV/c$^2$, " << njets << " jet.}" << std::endl;
  textfile << "\\end{center}" << endl;
  if(mH!=-1) textfile << "\\end{small}" << endl;
  else textfile << "\\end{scriptsize}" << endl;
  textfile << "\\end{table}" << endl;

  if(imass>0 && imass%10==0) textfile << "\\clearpage" << std::endl;

  if(mH!=-1) {
    // * for the datacards * //
    ofstream txtfilefordatacard;
    txtfilefordatacard.setf(ios::fixed,ios::floatfield);
    txtfilefordatacard.precision(3);
    txtfilefordatacard.open("mcyields.txt", ios_base::app);
    std::vector<std::string> sampleNameCard;
    sampleNameCard.push_back("DY");
    sampleNameCard.push_back("Top");
    sampleNameCard.push_back("WJet");
    sampleNameCard.push_back("Vgamma");
    sampleNameCard.push_back("VV");
    sampleNameCard.push_back("WW");
    sampleNameCard.push_back("ggWW");
    sampleNameCard.push_back("ggH");
    sampleNameCard.push_back("vbfH");

    // * vgamma is in a separate txt file * //
    ofstream txtvgammafilefordatacard;
    txtvgammafilefordatacard.setf(ios::fixed,ios::floatfield);
    txtvgammafilefordatacard.precision(3);
    txtvgammafilefordatacard.open("vgamma-yields-mc.txt", ios_base::app);
    
    // some gymnastic on the indices to use ttbar + tW and dividing ggH/qqH
    for(int isample=0; isample<(int)trees.size();isample++) {
      std::vector<double> val;
      // su ttbar + tW
      if(isample==1) {
        for(int icha=0; icha<5; icha++) val.push_back((yields[1])[icha] + (yields[2])[icha]);
      } else if(isample==2 || isample==8) continue;
      else val = yields[isample];
      
      int samplename = 0;
      if(isample<2) samplename = isample;
      else if(isample==2 || isample==8) samplename = -111;
      else samplename = isample-1;
      
      // signals at the end
      if(isample==9 || isample==10) samplename=isample-2;
      
      if(isample!=4) { // not vgamma
        txtfilefordatacard << mH << "\t" << njets << "\t" << sampleNameCard[samplename] 
                           << "\t" << val[mm] << "\t" << val[me] << "\t" << val[em] << "\t" << val[ee] << std::endl; 
      } else {
        txtvgammafilefordatacard << mH << "\t" << njets << "\t" << "vgamma"
                                 << "\t" << val[mm] << "\t" << val[me] << "\t" << val[em] << "\t" << val[ee] << std::endl;
      }
    }
    txtfilefordatacard.precision(0);
    txtfilefordatacard << mH << "\t" << njets << "\t" << "data" 
                       << "\t" << yields_data[mm] << "\t" << yields_data[me] << "\t" << yields_data[em] << "\t" << yields_data[ee] << std::endl;    
  }

  delete histo;

  fileZj->Close();
  fileTTbar->Close();
  fileSingleTop->Close();
  fileWj->Close();
  fileOthers->Close();
  fileqqWW->Close();
  fileggWW->Close();
  if(fileData) fileData->Close();
  if(fileFake) fileFake->Close();

}

float yieldErrPoisson(float nEst1, float n1, float nEst2, float n2, float nEst3, float n3, float nEst4, float n4, float nEst5, float n5, float nEst6, float n6) {

  float sum=0;
  if(n1>0) sum += pow(nEst1,2)/n1;
  if(n2>0) sum += pow(nEst2,2)/n2;
  if(n3>0) sum += pow(nEst3,2)/n3;
  if(n4>0) sum += pow(nEst4,2)/n4;
  if(n5>0) sum += pow(nEst5,2)/n5;
  if(n6>0) sum += pow(nEst6,2)/n6;
  
  return sqrt(sum);
}
