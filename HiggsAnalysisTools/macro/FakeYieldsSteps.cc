#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

string fullSelCuts[27];

int UseCuts[27];

float Wj_fullSel[27];
float WjF_fullSel[27];
float data1_fullSel[27];
float data2_fullSel[27];
float data3_fullSel[27];

float Wj_eff_fullSel[27];
float WjF_eff_fullSel[27];

float Wj_finaleff_fullSel;
float WjF_finaleff_fullSel;

// x-sections
float Wjets_xsec = 31314.;  // madgraph 

string sampleNames[5];

void computeYields(float lumi) {

  TChain *chains_fullSel[5];  
  for(int isample=0; isample<5; isample++) {
    char fullsel_treename[500];
    sprintf(fullsel_treename,"FULL_SELECTION_EVENT_COUNTER_EE_FP");
    // sprintf(fullsel_treename,"FULL_SELECTION_STAT_ERRORS_EE_FP");
    chains_fullSel[isample] = new TChain(fullsel_treename);
  }
  
  // samples name
  sampleNames[0] = "Wjets - nominal";
  sampleNames[1] = "Wjets - from fake";
  sampleNames[2] = "data, pTjet = 30";
  sampleNames[3] = "data, pTjet = 15";
  sampleNames[4] = "data, pTjet = 50";

  // mc
  chains_fullSel[0]->Add("results_nominalWjets/*Counters.root");

  // closure ee
  chains_fullSel[1]->Add("results_closure_EE/*Counters.root");
  // chains_fullSel[1]->Add("results_closure_EE/*Counters.root");
  
  // data ee
  chains_fullSel[2]->Add("results_data_30/*Counters.root");
  chains_fullSel[3]->Add("results_data_15/*Counters.root");
  chains_fullSel[4]->Add("results_data_50/*Counters.root");

  // data me
  // chains_fullSel[2]->Add("results_data_ME_MueG_30/*Counters.root");
  // chains_fullSel[2]->Add("results_data_ME_SingleMu_30/*Counters.root");
  // chains_fullSel[3]->Add("results_data_ME_MueG_15/*Counters.root");
  // chains_fullSel[3]->Add("results_data_ME_SingleMu_15/*Counters.root");
  // chains_fullSel[4]->Add("results_data_ME_MueG_50/*Counters.root");
  // chains_fullSel[4]->Add("results_data_ME_SingleMu_50/*Counters.root");
  
  // initialization
  float nFullSelTot[27][5];
  for(int isample=0; isample<5; isample++) {
    for(int icut=0; icut<27; icut++) { nFullSelTot[icut][isample] = 0.0; }
  }
  
  // full selection
  int nCutsAnaFull = 27;
  for(int isample=0; isample<5; isample++) {

    // List of branches    
    Int_t     nCutsFull;
    Float_t   nSelFull[27];   //[nCuts]
    TBranch   *b_nCutsFull;   //!
    TBranch   *b_nSelFull;    //!
    chains_fullSel[isample]->SetBranchAddress("nCuts", &nCutsFull, &b_nCutsFull);
    chains_fullSel[isample]->SetBranchAddress("nSel",  nSelFull,   &b_nSelFull);
    
    Long64_t nentriesFull = chains_fullSel[isample]->GetEntries();
    for (Long64_t jentry=0; jentry<nentriesFull;jentry++) {
      Long64_t nb2;
      nb2 = chains_fullSel[isample]->GetEntry(jentry);   
      for(int icut=0; icut<nCutsFull; icut++) nFullSelTot[icut][isample] += nSelFull[icut];
    }
  }
  
  // eff at full selection level
  for(int icut=0; icut<nCutsAnaFull; icut++) {

    // numbers
    if(nFullSelTot[0][0]>0) Wj_fullSel[icut]  = lumi * Wjets_xsec * nFullSelTot[icut][0]/nFullSelTot[0][0];
    if(nFullSelTot[0][1]>0) WjF_fullSel[icut] = lumi * Wjets_xsec * nFullSelTot[icut][1]/nFullSelTot[0][1];
    data1_fullSel[icut] = nFullSelTot[icut][2];
    data2_fullSel[icut] = nFullSelTot[icut][3];
    data3_fullSel[icut] = nFullSelTot[icut][4];

    // efficiencies
    if(icut>0 && nFullSelTot[icut-1][0]>0) Wj_eff_fullSel[icut]  = nFullSelTot[icut][0]/nFullSelTot[icut-1][0];
    else Wj_eff_fullSel[icut] = 0.0;
    
    if(icut>0 && nFullSelTot[icut-1][1]>0) WjF_eff_fullSel[icut] = nFullSelTot[icut][1]/nFullSelTot[icut-1][1];
    else WjF_eff_fullSel[icut] = 0.0;

    if(icut==0) { 
      Wj_eff_fullSel[icut]  = nFullSelTot[icut][0]/nFullSelTot[0][0];
      WjF_eff_fullSel[icut] = nFullSelTot[icut][1]/nFullSelTot[0][1];
    }
  }
  
  // final efficiency after full selections (-4 = 3 x jets + 1=final)
  if(nFullSelTot[0][0]>0) Wj_finaleff_fullSel = nFullSelTot[nCutsAnaFull-4][0]/nFullSelTot[0][0];
  else Wj_finaleff_fullSel = 0.0;
  
  if(nFullSelTot[0][1]>0) WjF_finaleff_fullSel = nFullSelTot[nCutsAnaFull-4][1]/nFullSelTot[0][1];
  else WjF_finaleff_fullSel = 0.0;

  cout << "\n\nPROCESSED EVENTS:" << endl;
  for(int i=0; i<5; i++) cout << sampleNames[i] << "\t" << nFullSelTot[0][i] << endl;  
}

void setupCuts() {
  
  for(int i=0; i<27; i++) UseCuts[i] = 1;
  
  fullSelCuts[0]="event";
  fullSelCuts[1]="MCtruth";
  fullSelCuts[2]="trigger";
  fullSelCuts[3]="channel preSel.";
  fullSelCuts[4]="e/$\\mu$ ID";
  fullSelCuts[5]="e/$\\mu$ isolation";
  fullSelCuts[6]="conv. rej.";
  fullSelCuts[7]="e/$\\mu$ d0";
  fullSelCuts[8]="$MET>20$ GeV";
  fullSelCuts[9]="$m_{ll}$";
  fullSelCuts[10]="$|m_{ll}-m_Z|>15$ GeV";
  fullSelCuts[11]="proj. MET";
  fullSelCuts[12]="MET/$p_T^{ll}$";
  fullSelCuts[13]="jet veto";
  fullSelCuts[14]="$\\mu^{soft}$ veto";
  fullSelCuts[15]="extra lepton veto";
  fullSelCuts[16]="anti b-tag";
  fullSelCuts[17]="$m_{ll}2$";
  fullSelCuts[18]="sel $p_T^{max}$";
  fullSelCuts[19]="sel $p_T^{min}$";
  fullSelCuts[20]="$\\Delta \\phi$";
  fullSelCuts[21]="$M_T$";
  fullSelCuts[22]="$M_T$ (1jet)";
  fullSelCuts[23]="final";
  fullSelCuts[24]="1 jets";
  fullSelCuts[25]="$>1$ jets";
  fullSelCuts[26]="WW, 1 jet bin";
}


void printLatex(float lumi) {

  setupCuts();

  cout << " === NOW COMPUTING YIELDS ===" << endl;
  
  computeYields(lumi);
  
  
  /// ==============  print detailed breakdown  ================== ///
  char namefile[200];
  sprintf(namefile,"yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(2);

  textfile << "\\documentclass{article}" << endl;
  textfile << "\\setlength\\textheight{9.8in}" << endl;
  textfile << "\\usepackage{rotating}" << endl;
  textfile << "\\begin{document}" << endl;
  textfile << "\\begin{sidewaystable}[p]" << endl
           << "\\begin{tiny}" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  textfile << "selection & data 30 & data 15 & data 50 & nominal W$(l\\nu)$+jets & fake W$(l\\nu)$+jets \\\\" << endl;
  textfile << "\\hline" << endl; 
  textfile << "\\hline" << endl;
  textfile << "\\hline" << endl;
  
  for(int icut=0; icut<27; icut++) {

    if(!UseCuts[icut]) continue;
    
    textfile << fullSelCuts[icut] << "\t&\t";
    
    textfile << fixed
             << data1_fullSel[icut]  << "\t&\t"
             << data2_fullSel[icut]  << "\t&\t"
             << data3_fullSel[icut]  << "\t&\t"
             << Wj_fullSel[icut]    << " (" << 100. * Wj_eff_fullSel[icut]  << "\\%)" << "\t&\t"
             << WjF_fullSel[icut]   << " (" << 100. * WjF_eff_fullSel[icut] << "\\%)" << "\t";
    textfile << "\t\\\\" << endl;
  }
    
  textfile << "\\hline" << endl;
  
  textfile << "total fullselection " << "\t&\t"
           << data1_fullSel[21]    << "\t&\t"
           << data2_fullSel[21]    << "\t&\t"
           << data3_fullSel[21]    << "\t&\t"
           << Wj_fullSel[21]       << " (" << 100. * Wj_finaleff_fullSel     << "\\%)" << "\t&\t"
           << WjF_fullSel[21]      << " (" << 100. * WjF_finaleff_fullSel    << "\\%)" << "\t";
  textfile << "\t\\\\"  << endl;
  textfile << "\\hline" << endl;
  textfile << "\t\\\\"  << endl;

  textfile << "WW level, 1 jet bin " << "\t&\t"
           << data1_fullSel[26]      << "\t&\t"
           << data2_fullSel[26]      << "\t&\t"
           << data3_fullSel[26]      << "\t&\t"
           << Wj_fullSel[26]         << " (" << 100. * Wj_finaleff_fullSel     << "\\%)" << "\t&\t"
           << WjF_fullSel[26]        << " (" << 100. * WjF_finaleff_fullSel    << "\\%)" << "\t";
  textfile << "\t\\\\"  << endl;
  textfile << "\\hline" << endl;
  textfile << "\t\\\\"  << endl;
    
  textfile << "\\hline"        << endl
           << "\\end{tabular}" << endl
           << "\\end{center}"  << endl
           << "\\end{tiny}"    << endl
           << "\\end{sidewaystable}" << endl;
  textfile << "\\end{document}" << endl;
  textfile.close();
}

