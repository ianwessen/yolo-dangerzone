#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include "massDependentCuts.cc"

float quadrSum(float x1, float x2, float x3=0, float x4=0, float x5=0, float x6=0, float x7=0, float x8=0);

float yieldErrPoisson(float nEst1, float n1, float nEst2=0, float n2=0, float nEst3=0, float n3=0, float nEst4=0, float n4=0, float nEst5=0, float n5=0, float nEst6=0, float n6=0);

TString kinematicCut(int ibin);

void estimateWjets(int njets);

enum { ee=1, mm=0, em=2, me=3 };
enum { barrellowpt=0, barrelhighpt=1, endcaplowpt=2, endcaphighpt=3 };

void printLatex() {
  
  ofstream textfile;
  textfile.open("Wjets_yields.tex", ios_base::trunc);
  textfile << "\\documentclass{article}" << endl;
  textfile << "\\setlength\\textheight{9.8in}" << endl;
  textfile << "\\usepackage{fullpage}" << endl;
  textfile << "\\begin{document}" << endl << endl;
  std::cout << "now estimating 0 jet bin " << std::endl;
  estimateWjets(0);

  std::cout << "now estimating 1 jet bin " << std::endl;
  estimateWjets(1);

  ofstream textfile2;
  textfile2.open("Wjets_yields.tex", ios_base::app);
  textfile2 << "\\end{document}" << endl << endl;
}

void estimateWjets(int njets) {
  
  // new WW selection
  char wwLevelCut[1000];

  if(njets==0) sprintf(wwLevelCut,"(step[14] && (dymva1>0.6 || !sameflav) && njet==0)"); 
  if(njets==1) sprintf(wwLevelCut,"(step[14] && (dymva1>0.3 || !sameflav) && njet==1)");
  if(njets==2) sprintf(wwLevelCut,"(trigger && pfmet > 20 && mll > (12 + 8*sameflav) && zveto && mpmet > (20+(17+nvtx/2.)*sameflav) && njet>=2 && (dphilljetjet< pi/180.*165. || !sameflav) && bveto_mu && nextra == 0 && bveto_ip && nbjet==0 && (pt2 > 15 || !sameflav) && ptll > 45)"); // not updated

  float yield_WWSel[5][4];           // [bin][icha] bin = 5 => total
  float yield_WWSel_staterr[5][4];   // [bin][icha]
  
  // systematics 
  float percErr[4][3];   // channel, jet-bin
  percErr[ee][0]=0.36;   // 0jets, ee
  percErr[ee][1]=0.36;  
  percErr[ee][2]=0.36;  
  percErr[me][0]=0.36;  
  percErr[me][1]=0.36;  
  percErr[me][2]=0.36;  
  percErr[mm][0]=0.36;  
  percErr[mm][1]=0.36;  
  percErr[mm][2]=0.36;  
  percErr[em][0]=0.36;  
  percErr[em][1]=0.36;  
  percErr[em][2]=0.36;  

  // trees
  TFile *fileEE = TFile::Open("results_data/datasets_trees/dataset_looseloose.root");
  TFile *fileMM = TFile::Open("results_data/datasets_trees/dataset_looseloose.root");
  TFile *fileEM = TFile::Open("results_data/datasets_trees/dataset_looseloose.root");
  TFile *fileME = TFile::Open("results_data/datasets_trees/dataset_looseloose.root");

  TTree *treeEE = (TTree*)fileEE->Get("latino");
  TTree *treeMM = (TTree*)fileMM->Get("latino");
  TTree *treeEM = (TTree*)fileEM->Get("latino");
  TTree *treeME = (TTree*)fileME->Get("latino");

  treeEE->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");
  treeMM->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");
  treeEM->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");
  treeME->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");

  std::vector<TTree*> trees;
  trees.push_back(treeEE);
  trees.push_back(treeMM);
  trees.push_back(treeEM);
  trees.push_back(treeME);

  TH1F *histo = new TH1F("histo","",100,0.,180.);

  // yields at WW level
  for(int icha=0; icha<4; icha++) {
    for(int ibin=0; ibin<4; ibin++) {
      
      char channel[20];
      sprintf(channel,"(channel==%d)",icha);

      TString fpCut, fpCutStatErr;
      fpCut        = TString("(") + kinematicCut(ibin) + TString(" && ") + TString(channel) + TString(")") + TString("*fake2W*")        + TString(wwLevelCut);
      fpCutStatErr = TString("(") + kinematicCut(ibin) + TString(" && ") + TString(channel) + TString(")") + TString("*fake2W*fake2W*") + TString(wwLevelCut);
      cout << "stima @ WW level: " << endl;
      cout << "icha = " << icha << ", ibin = " << ibin << ", string = " << fpCut << endl;
      
      trees[icha]->Project("histo","dphill",fpCut);
      yield_WWSel[ibin][icha] = histo->Integral();
      
      trees[icha]->Project("histo","dphill",fpCutStatErr);
      yield_WWSel_staterr[ibin][icha] = sqrt(histo->Integral());
    }
  }  
  
  for(int icha=0; icha<4; icha++) {
    for(int ibin=0; ibin<4; ibin++) {
      if(ibin==0) { 
        yield_WWSel[4][icha] = yield_WWSel[ibin][icha];
        yield_WWSel_staterr[4][icha] = pow(yield_WWSel_staterr[ibin][icha],2);    
      }
      else {
        yield_WWSel[4][icha] += yield_WWSel[ibin][icha];
        yield_WWSel_staterr[4][icha] += pow(yield_WWSel_staterr[ibin][icha],2);}  
    }
  }

  float yield_WWSel_fullerr[4];    // [bin]
  for(int icha=0; icha<4; icha++) { 
    yield_WWSel_staterr[4][icha] = sqrt(yield_WWSel_staterr[4][icha]);            
    yield_WWSel_fullerr[icha]    = quadrSum(yield_WWSel_staterr[4][icha], percErr[icha][njets]*yield_WWSel[4][icha]);
    cout << "channel " << icha 
	 << ": val = "     << yield_WWSel[4][icha] 
	 << ", statErr = " << yield_WWSel_staterr[4][icha] 
	 << ", systErr = " << percErr[icha][njets]*yield_WWSel[4][icha]
	 << ", totErr = "  << yield_WWSel_fullerr[icha] << endl;
  }

  // total for all the channels
  float yield_WWSel_tot = 0.;
  float yield_WWSel_tot_err = 0.;
  for(int icha=0; icha<4; icha++) {
    yield_WWSel_tot += yield_WWSel[4][icha];
    yield_WWSel_tot_err += pow(yield_WWSel_fullerr[icha],2);  
  }
  yield_WWSel_tot_err = sqrt(yield_WWSel_tot_err);

  std::vector<TString> labels;
  labels.push_back(TString("Barrel, 10 $\\leq p_{T} < $ 20 GeV"));
  labels.push_back(TString("Barrel, $p_{T} \\geq $ 20 GeV"));
  labels.push_back(TString("Endcap, 10 $\\leq p_{T} < $ 20 GeV"));
  labels.push_back(TString("Endcap, $p_{T} \\geq $ 20 GeV"));
  labels.push_back(TString("Total"));

  ofstream textfile;
  textfile.open("Wjets_yields.tex", ios_base::app);
  textfile.setf(ios::fixed,ios::floatfield);
  textfile.precision(1);

  // ee and me
  textfile << "\\begin{table}[p]" << endl;
  textfile << "\\begin{small}" << endl;
  textfile << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  textfile << "Fake Lepton Bin \t & \t Electron + Fake Electron \t & \t Muon + Fake Electron \\\\" << std::endl;
  textfile << "\\hline" << endl;
  for(int ibin=0; ibin<5; ibin++) {
    if(ibin==4) textfile << "\\hline" << endl;
    textfile << labels[ibin] << " & "
             << yield_WWSel[ibin][ee] << " $ \\pm $ " << yield_WWSel_staterr[ibin][ee] << " (stat.) & "
             << yield_WWSel[ibin][me] << " $ \\pm $ " << yield_WWSel_staterr[ibin][me] << " (stat.) \\\\ "
             << std::endl;
  }
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\caption{W+jets yields in the ee and $\\mu$e channels at WW selection level in the " 
           << njets << " jet bin.}" << std::endl;
  textfile << "\\end{center}" << endl;
  textfile << "\\end{small}" << endl;
  textfile << "\\end{table}" << endl;


  // mm and em
  textfile << "\\begin{table}[p]" << endl;
  textfile << "\\begin{small}" << endl;
  textfile << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  textfile << "Fake Lepton Bin \t & \t Electron + Fake Muon \t & \t Muon + Fake Muon \\\\" << std::endl;
  textfile << "\\hline" << endl;
  for(int ibin=0; ibin<5; ibin++) {
    if(ibin==4) textfile << "\\hline" << endl;
    textfile << labels[ibin] << " & "
             << yield_WWSel[ibin][em] << " $ \\pm $ " << yield_WWSel_staterr[ibin][em] << " (stat.) & "
             << yield_WWSel[ibin][mm] << " $ \\pm $ " << yield_WWSel_staterr[ibin][mm] << " (stat.) \\\\ "
             << std::endl;
  }
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\caption{W+jets yields in the e$\\mu$ and $\\mu\\mu$ channels at WW selection level in the " 
           << njets << " jet bin.}" << std::endl;
  textfile << "\\end{center}" << endl;
  textfile << "\\end{small}" << endl;
  textfile << "\\end{table}" << endl;

  std::cout << "WW level DONE." << std::endl;

  char nameFileTable[100];
  sprintf(nameFileTable, "WJetsYieldsData_ForTable_%dj.txt",njets);
  ofstream tablefile;
  tablefile.open(nameFileTable, ios_base::trunc);
  tablefile.precision(2);

  char nameFileTable2[100];
  sprintf(nameFileTable2, "WJetsYieldsData_SF_ForTable_%dj.txt",njets);
  ofstream tablefile2;
  tablefile2.open(nameFileTable2, ios_base::trunc);
  tablefile2.precision(2);

  char nameFileTable3[100];
  sprintf(nameFileTable3, "WJetsYieldsData_OF_ForTable_%dj.txt",njets);
  ofstream tablefile3;
  tablefile3.open(nameFileTable3, ios_base::trunc);
  tablefile3.precision(2);

  int masses[27] = {110,115,120,125,130,135,140,145,150,155,160,170,180,190,200,250,300,350,400,450,500,550,600,700,800,900,1000};
  // -------------------------------------------------------------------
  // now considering all masses to estimate the number of events at the end of the HWW selection
  for (int i=0; i<27; i++) {

    int mass = masses[i];

    TString higgsMassDependentCut = higgsCuts(mass,true,njets);       // for cut based studied
    // TString higgsMassDependentCut = higgsCutsBDT(mass,true,njets);       // for BDT studies

    // specific vbf selection
    if (njets==2) {
      char vbfLevelCut[1000];
      if(i<=8) sprintf(vbfLevelCut,"((max(abs(jeteta1),abs(jeteta2))<4.5) && (njetvbf==0) && (mll<100) && (abs(eta1 - (jeteta1+jeteta2)/2)/detajj < 0.5 && abs(eta2 - (jeteta1+jeteta2)/2)/detajj < 0.5) && (detajj>3.5) && (mjj>450))"); 
      if(i>8) sprintf(vbfLevelCut,"((max(abs(jeteta1),abs(jeteta2))<4.5) && (njetvbf==0) && (abs(eta1 - (jeteta1+jeteta2)/2)/detajj < 0.5 && abs(eta2 - (jeteta1+jeteta2)/2)/detajj < 0.5) && (detajj>3.5) && (mjj>450))"); 
      higgsMassDependentCut = TString(vbfLevelCut);
      // cout << "caso VBF: " << endl;
      // cout << vbfLevelCut  << endl;
    }
    
    // yields at higgs level
    for(int icha=0; icha<4; icha++) {
      char channel[20];
      sprintf(channel,"channel==%d",icha);

      TString fpCut, fpCutStatErr;
      fpCut        = TString("(") + higgsMassDependentCut  + TString(" && ") + TString(channel) + TString(")") + TString("*fake2W*")        + TString(wwLevelCut);
      fpCutStatErr = TString("(") + higgsMassDependentCut  + TString(" && ") + TString(channel) + TString(")") + TString("*fake2W*fake2W*") + TString(wwLevelCut);
      
      // cout << "at higgs level, mass = " << i << endl;
      // std::cout << "taglio = " << fpCut.Data() << std::endl;
      
      trees[icha]->Project("histo","dphill",fpCut);
      yield_WWSel[4][icha] = histo->Integral();
      
      trees[icha]->Project("histo","dphill",fpCutStatErr);
      yield_WWSel_staterr[4][icha] = sqrt(histo->Integral());  

      yield_WWSel_fullerr[icha] = quadrSum(yield_WWSel_staterr[4][icha], percErr[icha][njets]*yield_WWSel[4][icha]);
    }
    
    // total (statistical only here) error for the four channels together
    float yield_tot = 0.;
    float yield_tot_staterr = 0.;
    float yield_tot_fullerr = 0.;
    for(int icha=0; icha<4; icha++) {
      yield_tot         += yield_WWSel[4][icha];
      yield_tot_staterr += pow(yield_WWSel_staterr[4][icha],2);  
      yield_tot_fullerr += pow(yield_WWSel_fullerr[icha],2);  
    }
    yield_tot_staterr = sqrt(yield_tot_staterr);     
    yield_tot_fullerr = sqrt(yield_tot_fullerr);     
    cout << "mass " << i << ": yield = " << yield_tot << ", stat = " << yield_tot_staterr << ", full = " << yield_tot_fullerr << endl; 

    // total splitted between SF and OF (for datacards)
    float yield_sf   = yield_WWSel[4][mm] + yield_WWSel[4][ee];
    float yield_of   = yield_WWSel[4][em] + yield_WWSel[4][me];
    //
    float fullerr_sf = sqrt( yield_WWSel_fullerr[mm]*yield_WWSel_fullerr[mm] + yield_WWSel_fullerr[ee]*yield_WWSel_fullerr[ee]);
    float fullerr_of = sqrt( yield_WWSel_fullerr[em]*yield_WWSel_fullerr[em] + yield_WWSel_fullerr[me]*yield_WWSel_fullerr[me]);
    // 
    float staterr_sf = sqrt( yield_WWSel_staterr[4][mm]*yield_WWSel_staterr[4][mm] + yield_WWSel_staterr[4][ee]*yield_WWSel_staterr[4][ee]);
    float staterr_of = sqrt( yield_WWSel_staterr[4][em]*yield_WWSel_staterr[4][em] + yield_WWSel_staterr[4][me]*yield_WWSel_staterr[4][me]);

    // summary table for limits: splitting in 4 channels
    if (i==0) { 
      tablefile << njets << " jets bin data" << endl;
      tablefile << "\t\t mumu \t\t mue \t\t emu \t\t ee \t\t ll" << endl;
    }
    tablefile << mass 
              << "\t\t" << yield_WWSel[4][mm] << " +/- " <<  yield_WWSel_staterr[4][mm] 
              << "\t\t" << yield_WWSel[4][me] << " +/- " <<  yield_WWSel_staterr[4][me] 
              << "\t\t" << yield_WWSel[4][em] << " +/- " <<  yield_WWSel_staterr[4][em] 
              << "\t\t" << yield_WWSel[4][ee] << " +/- " <<  yield_WWSel_staterr[4][ee] 
              << "\t\t" << yield_tot << " +/- " <<  yield_tot_staterr  
              << std::endl;

    // summary table for limits: splitting in 2 channels (SF, OF)
    if (i==0) { 
      // tablefile2 << njets << " jets bin data" << endl;
      // tablefile2 << "\t\t SF" << endl;
      // tablefile3 << njets << " jets bin data" << endl;
      // tablefile3 << "\t\t OF" << endl;
    }
    // if the yield is negative, put the uncertainty as central value or the limit will go crazy
    if(yield_sf<0) yield_sf = staterr_sf;
    if(yield_of<0) yield_of = staterr_of;
    tablefile2 << mass << "\t\t" << yield_sf << "   " << staterr_sf << std::endl; 
    tablefile3 << mass << "\t\t" << yield_of << "   " << staterr_of << std::endl; 
  }

  /*
  /// ===> compute !b-veto efficiency on W+jets 
  // this is useful for W+jets contamination in the top-tagged control sample
  // use a channel independent estimate
  // for now only ee is done
  TFile *fileEEAll = TFile::Open("results_data_fakes/2011A_scenario3.root");
  TFile *fileMMAll = TFile::Open("results_data_fakes/2011A_scenario3.root");
  TFile *fileEMAll = TFile::Open("results_data_fakes/2011A_scenario3.root");
  TFile *fileMEAll = TFile::Open("results_data_fakes/2011A_scenario3.root");

  TTree *treeEEAll = (TTree*)fileEEAll->Get("latino");
  TTree *treeMMAll = (TTree*)fileMMAll->Get("latino");
  TTree *treeEMAll = (TTree*)fileEMAll->Get("latino");
  TTree *treeMEAll = (TTree*)fileMEAll->Get("latino");

  trees.clear();
  trees.push_back(treeEEAll);
  trees.push_back(treeMMAll);
  trees.push_back(treeEMAll);
  trees.push_back(treeMEAll);
  */

}

float quadrSum(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8) {
  return sqrt(x1*x1 + x2*x2 + x3*x3 + x4*x4 + x5*x5 + x6*x6 + x7*x7 + x8*x8);
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

TString kinematicCut(int ibin) {
  if(ibin==barrellowpt)  return TString("(abs(eta2)<=1.479 && pt2<20)");
  if(ibin==barrelhighpt) return TString("(abs(eta2)<=1.479 && pt2>=20)");
  if(ibin==endcaplowpt)  return TString("(abs(eta2)>1.479 && pt2<20)");
  if(ibin==endcaphighpt) return TString("(abs(eta2)>1.479 && pt2>=20)");
  return TString("wrong bin chosen. Check method kinematicCut(int ibin)");
}
