#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

enum { ee=0, mm=1, em=2 };

string preSelCuts[10];
string fullSelCuts[23];

int UsePreSelCuts[3][10];
int UseCuts[3][23];

int H_mass[8];

float H_preSel[11][8];
float H_fullSel[23][8];

float H_eff_preSel[11][8];
float H_eff_fullSel[23][8];

float H_finaleff_preSel[8];    
float H_finaleff_fullSel[8];    

float H_finaleff[8];    

float H_final[3][8];

// xsections
float H_xsec[8];

string sampleNames[8];

void computeYields(float lumi, const char* finalstate) {

  // Higgs * BR(WW) cross sections
  H_xsec[0] = 0.247143; // 120 GeV
  H_xsec[1] = 0.452859; // 130 GeV
  H_xsec[2] = 0.649260; // 140 GeV
  H_xsec[3] = 0.787871; // 150 GeV
  H_xsec[4] = 0.897043; // 160 GeV
  H_xsec[5] = 0.808914; // 170 GeV
  H_xsec[6] = 0.685617; // 180 GeV
  H_xsec[7] = 0.503611; // 190 GeV

  TChain *chains_preSel[8];
  TChain *chains_fullSel[8];
  for(int isample=0; isample<8; isample++) {
    chains_preSel[isample]  = new TChain("PRESELECTION_EVENT_COUNTER");
    char fullsel_treename[200];
    sprintf(fullsel_treename,"FULL_SELECTION_EVENT_COUNTER_%s",finalstate);
    chains_fullSel[isample] = new TChain(fullsel_treename);
  }

  // signal
  sampleNames[0] = "Higgs 120 GeV";
  sampleNames[1] = "Higgs 130 GeV";
  sampleNames[2] = "Higgs 140 GeV";
  sampleNames[3] = "Higgs 150 GeV";
  sampleNames[4] = "Higgs 160 GeV";
  sampleNames[5] = "Higgs 170 GeV";
  sampleNames[6] = "Higgs 180 GeV";
  sampleNames[7] = "Higgs 190 GeV";

  // signal
  chains_preSel[0]->Add("results/HiggsWW/H120_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[1]->Add("results/HiggsWW/H130_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[2]->Add("results/HiggsWW/H140_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[3]->Add("results/HiggsWW/H150_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[4]->Add("results/HiggsWW/H160_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[5]->Add("results/HiggsWW/H170_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[6]->Add("results/HiggsWW/H180_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_preSel[7]->Add("results/HiggsWW/H190_2W_2lnu_gluonfusion_7TeV/*Counters.root");       

  chains_fullSel[0]->Add("results/HiggsWW/H120_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[1]->Add("results/HiggsWW/H130_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[2]->Add("results/HiggsWW/H140_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[3]->Add("results/HiggsWW/H150_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[4]->Add("results/HiggsWW/H160_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[5]->Add("results/HiggsWW/H170_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[6]->Add("results/HiggsWW/H180_2W_2lnu_gluonfusion_7TeV/*Counters.root");       
  chains_fullSel[7]->Add("results/HiggsWW/H190_2W_2lnu_gluonfusion_7TeV/*Counters.root");       


  float nPreSelTot[10][8];
  float nFullSelTot[23][8];

  for(int isample=0; isample<8; isample++) {
    for(int icut=0; icut<10; icut++) { nPreSelTot[icut][isample]  = 0.0; }
    for(int icut=0; icut<28; icut++) { nFullSelTot[icut][isample] = 0.0; }
  }

  // preselections
  int nCutsAnaPre  = 10;
  int nCutsAnaFull = 23;
  for(int isample=0; isample<8; isample++) {

    cout << "Processing sample # " << isample << endl;
    
    // List of branches    
    Int_t           nCutsPre;
    Float_t         nSelPre[11];   //[nCuts]
    TBranch        *b_nCutsPre;   //!
    TBranch        *b_nSelPre;    //!
    chains_preSel[isample]->SetBranchAddress("nCuts", &nCutsPre, &b_nCutsPre);
    chains_preSel[isample]->SetBranchAddress("nSel",  nSelPre,   &b_nSelPre);
    
    Int_t           nCutsFull;
    Float_t         nSelFull[19];   //[nCuts]
    TBranch        *b_nCutsFull;   //!
    TBranch        *b_nSelFull;    //!
    chains_fullSel[isample]->SetBranchAddress("nCuts", &nCutsFull, &b_nCutsFull);
    chains_fullSel[isample]->SetBranchAddress("nSel",  nSelFull,   &b_nSelFull);
    
    Long64_t nentriesPre  = chains_preSel[isample]->GetEntries();
    Long64_t nentriesFull = chains_fullSel[isample]->GetEntries();
    
    // pre-selection
    for (Long64_t jentry=0; jentry<nentriesPre;jentry++) {
      Long64_t nb = chains_preSel[isample]->GetEntry(jentry);   
      // nCutsAnaPre = nCutsPre;      
      for(int icut=0; icut<nCutsPre; icut++) nPreSelTot[icut][isample] += nSelPre[icut];	
    }
    
    // full selection
    for (Long64_t jentry=0; jentry<nentriesFull;jentry++) {
      Long64_t nb2 = chains_fullSel[isample]->GetEntry(jentry);   
      // nCutsAnaFull = nCutsFull;
      for(int icut=0; icut<nCutsFull; icut++) nFullSelTot[icut][isample] += nSelFull[icut];
    }
  }

  // eff at preselection level
  for(int imass=0; imass<8; imass++) {

    for(int icut=0; icut<nCutsAnaPre; icut++) {
      // N = L * x-sec * eff (eff = N_fin / N_ini)
      if(nPreSelTot[0][imass]>0) H_preSel[imass][icut] = lumi * H_xsec[imass]  * nPreSelTot[icut][imass]/nPreSelTot[0][imass];
      
      // efficiencies
      if(icut>0 && nPreSelTot[icut-1][imass]>0) H_eff_preSel[icut][imass] = nPreSelTot[icut][imass]/nPreSelTot[icut-1][imass];
      else H_eff_preSel[icut][imass] = 0.0;
    }

    // final efficiency at preselection level
    if(nPreSelTot[0][imass]>0) H_finaleff_preSel[imass] = nPreSelTot[nCutsAnaPre-2][imass]/nPreSelTot[0][imass];
    else H_finaleff_preSel[imass] = 0.0;

    // eff at full selection level
    for(int icut=0; icut<nCutsAnaFull; icut++) {

      // numbers
      if(nFullSelTot[0][imass]>0) H_fullSel[icut][imass] = lumi * H_xsec[imass] * nFullSelTot[icut][imass]/nPreSelTot[0][imass];

      // efficiencies
      if(icut>0 && nFullSelTot[icut-1][imass]>0) H_eff_fullSel[icut][imass] = nFullSelTot[icut][imass]/nFullSelTot[icut-1][imass];
      else H_eff_fullSel[icut][imass] = 0.0;

      if(icut==0) H_eff_fullSel[icut][imass] = nFullSelTot[icut][imass]/nPreSelTot[nCutsAnaPre-2][imass];
    }

    // final efficiency after full selections (-4 = 3 x jets + 1=final)
    if(nFullSelTot[0][imass]>0) H_finaleff_fullSel[imass] = nFullSelTot[nCutsAnaFull-4][imass]/nFullSelTot[0][imass];
    else H_finaleff_fullSel[imass] = 0.0;

    // final efficiency combining pre and full selections
    if(nPreSelTot[0][imass]>0) H_finaleff[imass] = nFullSelTot[nCutsAnaFull-4][imass]/nPreSelTot[0][imass];
    else H_finaleff[imass] = 0.0;

  }

  cout << "\n\nPROCESSED EVENTS:" << endl;
  for(int imass=0; imass<8; imass++) {
    cout << sampleNames[imass] << "\t" << nPreSelTot[0][imass] << endl;
  }

}

void setupCuts() {
  
  for(int ichan=0; ichan<3; ichan++) {
    for(int i=0; i<10; i++) {
      UsePreSelCuts[ichan][i] = 1;
    }
  }

  for(int ichan=0; ichan<3; ichan++) {
    for(int i=0; i<23; i++) {
      UseCuts[ichan][i] = 1;
    }
  }

  // unusued ee cuts
  UsePreSelCuts[ee][1] = UsePreSelCuts[ee][7] = UsePreSelCuts[ee][8] = 0; 
  UseCuts[ee][5] = UseCuts[ee][6] = UseCuts[ee][7] = UseCuts[ee][8] = UseCuts[ee][20] = 0; // muon iso

  // unusued mm cuts
  UsePreSelCuts[mm][1] = UsePreSelCuts[mm][7] = UsePreSelCuts[mm][8] = 0; 
  UseCuts[mm][4] = UseCuts[mm][10] = UseCuts[mm][5] = UseCuts[mm][6] = UseCuts[mm][7] = UseCuts[mm][20] = 0; // ele ID and conv. rej. and separate mu iso

  // unusued em cuts
  UsePreSelCuts[em][1] = UsePreSelCuts[em][7] = UsePreSelCuts[em][8] = UseCuts[em][13] = UseCuts[em][20] = 0;
  UseCuts[em][5] = UseCuts[em][6] = UseCuts[em][7] = 0; // separate mu iso 
  
  preSelCuts[0]="event";
  preSelCuts[1]="MCtruth";
  preSelCuts[2]="trigger";
  preSelCuts[3]="$\\geq 2$ leptons";
  preSelCuts[4]="acceptance";
  preSelCuts[5]="$p_T^{max}>20$ GeV";
  preSelCuts[6]="$p_T^{min}>20$ GeV";
  preSelCuts[7]="MET preselection";
  preSelCuts[8]="$m_{ll}>12$ GeV";
  preSelCuts[9]="final presel.";

  fullSelCuts[0]="channel presel.";
  fullSelCuts[1]="$p_T^{max}>20$ GeV";
  fullSelCuts[2]="$p_T^{min}>20$ GeV";
  fullSelCuts[3]="e/$\\mu$ d0";
  fullSelCuts[4]="e isolation";
  fullSelCuts[5]="$\\mu$ tracker Iso";
  fullSelCuts[6]="$\\mu$ HCAL Iso";
  fullSelCuts[7]="$\\mu$ ECAL Iso";
  fullSelCuts[8]="$\\mu$ isolation";
  fullSelCuts[9]="e/$\\mu$ ID";
  fullSelCuts[10]="conv. rej.";
  fullSelCuts[11]="$MET>20$ GeV";
  fullSelCuts[12]="$m_{ll}>12$ GeV";
  fullSelCuts[13]="$|m_{ll}-m_Z|>15$ GeV";
  fullSelCuts[14]="tight (p)MET";
  fullSelCuts[15]="jet veto";
  fullSelCuts[16]="anti b-tag";
  fullSelCuts[17]="$\\mu^{soft}$ veto";
  fullSelCuts[18]="extra lepton veto";
  fullSelCuts[19]="$\\Delta \\phi$";
  fullSelCuts[20]="final";
  fullSelCuts[21]="1 jets";
  fullSelCuts[22]="$>1$ jets";

}


void printLatex(float lumi, const char* finalstate) {

  setupCuts();

  if(strcmp(finalstate,"EE") && strcmp(finalstate,"EM") && strcmp(finalstate,"MM")) {
    cout << "ERROR! finalstate must be one among EE/EM/MM. Exiting..." << endl;
    return;
  } else {
    cout << " === NOW COMPUTING YIELDS FOR FINAL STATE: " << finalstate << " ===" << endl;
  }

  computeYields(lumi,finalstate);


  /// ==============  print detailed breakdown  ================== ///
  char namefile[200];
  sprintf(namefile,"signal_yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(1);

  textfile << "\\begin{sidewaystable}[p]" << endl
           << "\\begin{tiny}" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  textfile << "selection & H(120) & H(130) & H(140) & H(150) & H(160) & H(170) & H(180) & H(190) \t\\\\" << endl;
  textfile << "\\hline" << endl; 
    
  textfile << "\\hline"        << endl;
  textfile << "\\hline"        << endl;
    
  for(int icut=0; icut<9; icut++) {

    if(!strcmp(finalstate,"EE")) {
      if(!UsePreSelCuts[ee][icut]) continue;
    }
    if(!strcmp(finalstate,"MM")) {
      if(!UsePreSelCuts[mm][icut]) continue;
    }
    if(!strcmp(finalstate,"EM")) {
      if(!UsePreSelCuts[em][icut]) continue;
    }

    textfile << preSelCuts[icut] << "\t&\t";
      
    textfile << fixed;
    for(int imass=0; imass<8; ++imass) {
      textfile << H_preSel[icut][imass] << " (" << 100. * H_eff_preSel[icut][imass] << "\\%)";
      if(imass<7) textfile << "\t&\t";
      else textfile << "\t\\\\" << endl;
    }
  }

  textfile << "\\hline" << endl;
  
  textfile << "total preselection " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_preSel[8][imass] << " (" << 100. * H_finaleff_preSel[imass]  << "\\%)";
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }

  textfile << "\\hline" << endl;
    
  for(int icut=0; icut<21; icut++) {

    if(!strcmp(finalstate,"EE")) {
      if(!UseCuts[ee][icut]) continue;
    }
    if(!strcmp(finalstate,"MM")) {
      if(!UseCuts[mm][icut]) continue;
    }
    if(!strcmp(finalstate,"EM")) {
      if(!UseCuts[em][icut]) continue;
    }

    textfile << fullSelCuts[icut] << "\t&\t";
      
    textfile << fixed;
    for(int imass=0; imass<8; ++imass) {
      textfile << H_fullSel[icut][imass] << " (" << 100. * H_eff_fullSel[icut][imass] << "\\%)";
      if(imass<7) textfile << "\t&\t";
      else textfile << "\t\\\\" << endl;
    }
  }
    
  textfile << "\\hline" << endl;

  textfile << "total fullselection " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_fullSel[20][imass] << " (" << 100. * H_finaleff_fullSel[imass]  << "\\%)";
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }

  textfile << "\\hline" << endl;
    
  textfile << "total " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_fullSel[20][imass] << " (" << 100. * H_finaleff[imass]   << "\\%)";
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }
    
  textfile << "0 jets bin " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_fullSel[20][imass];
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }
    
  textfile << "1 jets bin " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_fullSel[21][imass];
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }

  textfile << "$>1$ jets bin " << "\t&\t";
  for(int imass=0; imass<8; ++imass) {
    textfile << H_fullSel[22][imass];
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }
    
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\end{tiny}" << endl
           << "\\caption{Breakdown of signal and backgrounds events in "
           << lumi << " $pb^{-1}$ for " << finalstate << " final state.} " << endl 
           << "\\end{sidewaystable}" << endl;

  // assign the final yields
  int channel=-1;
  if(!strcmp(finalstate,"MM")) channel = mm;
  if(!strcmp(finalstate,"EM")) channel = em;
  if(!strcmp(finalstate,"EE")) channel = ee;

  for(int imass=0; imass<8; ++imass) H_final[channel][imass] = H_fullSel[20][imass];

}

void printShortBkgSummary(float lumi) {

  /// ==============  print short summary  ================== ///

  char namefile[200];
  sprintf(namefile,"signal_yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(1);

  std::string channelName[3];
  channelName[mm] = "$\\mu\\mu$";
  channelName[ee] = "$ee$";
  channelName[em] = "$e\\mu$";

  textfile << "\\begin{sidewaystable}[p]" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;

  textfile << "final state & H(120) & H(130) & H(140) & H(150) & H(160) & H(170) & H(180) & H(190) \t\\\\" << endl;
  textfile << "\\hline" << endl; 
  for(int ichan=0; ichan<3; ++ichan) {
    textfile << channelName[ichan] << "\t&\t";
    textfile << fixed;
    for(int imass=0; imass<8; ++imass) {
      textfile << H_final[ichan][imass];
      if(imass<7) textfile << "\t&\t";
      else textfile << "\t\\\\" << endl;
    }
  }
  textfile << "ll" << "\t&\t";
  textfile << fixed;
  for(int imass=0; imass<8; ++imass) {
    textfile << H_final[mm][imass] + H_final[ee][imass] + H_final[em][imass];
    if(imass<7) textfile << "\t&\t";
    else textfile << "\t\\\\" << endl;
  }
  
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\caption{Expected signal events in "
           << lumi << " $pb^{-1}$ for different Higgs masses.} " << endl 
           << "\\end{sidewaystable}" << endl;

}

void printLatex(float lumi) {
  
  char namefile[200];
  sprintf(namefile,"signal_yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::trunc);
  textfile.precision(1);

  textfile << "\\documentclass{article}" << endl;
  textfile << "\\setlength\\textheight{9.8in}" << endl;
  textfile << "\\usepackage{rotating}" << endl;
  textfile << "\\begin{document}" << endl;

  textfile.close();

  printLatex(lumi, "EE");
  printLatex(lumi, "MM");
  printLatex(lumi, "EM");

  printShortBkgSummary(lumi);

  textfile.open(namefile, ios_base::app);
  textfile << "\\end{document}" << endl;
  textfile.close();

}

