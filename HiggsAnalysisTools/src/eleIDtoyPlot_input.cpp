// ! c++ includes
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream.h>
#include <math.h>

//! ROOT includes
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1F.h"
#include "TApplication.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"

using namespace std;

int main ( int argc, char **argv) {
  
  if (argc < 2){ cout << "Argument missing! Insert the root input file " << std::endl; return 0; }
  
  
  // reading the input tree
  TChain *T = new TChain("T1"); 
  T->Add(argv[1]);      
  float L_deltaEta, L_deltaPhi;
  float L_hoe,      L_s9s25;
  float L_see,      L_eopOut;  
  int L_class;
  float H_deltaEta, H_deltaPhi;
  float H_hoe,      H_s9s25;
  float H_see,      H_eopOut;  
  int H_class;

  T->SetMakeClass(1);
  T->SetBranchStatus("*",0);
  T->SetBranchStatus("L_deltaEta",1);
  T->SetBranchStatus("L_deltaPhi",1);
  T->SetBranchStatus("L_hoe",1);
  T->SetBranchStatus("L_s9s25",1);
  T->SetBranchStatus("L_see",1);
  T->SetBranchStatus("L_eopOut",1);
  T->SetBranchStatus("L_class",1);
  T->SetBranchStatus("H_deltaEta",1);
  T->SetBranchStatus("H_deltaPhi",1);
  T->SetBranchStatus("H_hoe",1);
  T->SetBranchStatus("H_s9s25",1);
  T->SetBranchStatus("H_see",1);
  T->SetBranchStatus("H_eopOut",1);
  T->SetBranchStatus("H_class",1);
  T->SetBranchAddress("L_deltaEta",&L_deltaEta);
  T->SetBranchAddress("L_deltaPhi",&L_deltaPhi);
  T->SetBranchAddress("L_hoe",&L_hoe);
  T->SetBranchAddress("L_s9s25",&L_s9s25);
  T->SetBranchAddress("L_see",&L_see);
  T->SetBranchAddress("L_eopOut",&L_eopOut);
  T->SetBranchAddress("L_class",&L_class);
  T->SetBranchAddress("H_deltaEta",&H_deltaEta);
  T->SetBranchAddress("H_deltaPhi",&H_deltaPhi);
  T->SetBranchAddress("H_hoe",&H_hoe);
  T->SetBranchAddress("H_s9s25",&H_s9s25);
  T->SetBranchAddress("H_see",&H_see);
  T->SetBranchAddress("H_eopOut",&H_eopOut);
  T->SetBranchAddress("H_class",&H_class);

  // histos
  char title[200];
  TH1F *HH_dEta[4],   *HL_dEta[4]; 
  TH1F *HH_dPhi[4],   *HL_dPhi[4]; 
  TH1F *HH_HoE[4],    *HL_HoE[4]; 
  TH1F *HH_S9S25[4],  *HL_S9S25[4]; 
  TH1F *HH_See[4],    *HL_See[4]; 
  TH1F *HH_EoPout[4], *HL_EoPout[4]; 
  for (int ii=0; ii<4; ii++){
    sprintf(title,"HH_dEta[%d]",ii);
    if (ii==0 || ii==1) HH_dEta[ii]   = new TH1F(title, title, 35,  0.0, 0.005);
    if (ii==2 || ii==3) HH_dEta[ii]   = new TH1F(title, title, 35,  0.0, 0.009);
    sprintf(title,"HH_dPhi[%d]",ii);
    if (ii==0 || ii==1) HH_dPhi[ii]   = new TH1F(title, title, 35,  0.0, 0.025);
    if (ii==2 || ii==3) HH_dPhi[ii]   = new TH1F(title, title, 35,  0.0, 0.090);
    sprintf(title,"HH_HoE[%d]",ii);
    if (ii==0 || ii==1) HH_HoE[ii]    = new TH1F(title, title, 35, -0.01, 0.06);
    if (ii==2 || ii==3) HH_HoE[ii]    = new TH1F(title, title, 35, -0.01, 0.10);
    sprintf(title,"HH_S9S25[%d]",ii);
    if (ii==0 || ii==1) HH_S9S25[ii]  = new TH1F(title, title, 35,  0.8, 1.0);
    if (ii==2 || ii==3) HH_S9S25[ii]  = new TH1F(title, title, 35,  0.5, 1.0);
    sprintf(title,"HH_See[%d]",ii);
    if (ii==0 || ii==2) HH_See[ii]    = new TH1F(title, title, 35,  0.0, 0.02);
    if (ii==1 || ii==3) HH_See[ii]    = new TH1F(title, title, 35,  0.006, 0.026);
    sprintf(title,"HH_EoPout[%d]",ii);
    if (ii==0 || ii==1) HH_EoPout[ii] = new TH1F(title, title, 35,  0.5, 2.5);
    if (ii==2 || ii==3) HH_EoPout[ii] = new TH1F(title, title, 35,  0.5, 4.0);
    //
    sprintf(title,"HL_dEta[%d]",ii);
    if (ii==0 || ii==1) HL_dEta[ii]   = new TH1F(title, title, 35,  0.0, 0.005);
    if (ii==2 || ii==3) HL_dEta[ii]   = new TH1F(title, title, 35,  0.0, 0.009);
    sprintf(title,"HL_dPhi[%d]",ii);
    if (ii==0 || ii==1) HL_dPhi[ii]   = new TH1F(title, title, 35,  0.0, 0.025);
    if (ii==2 || ii==3) HL_dPhi[ii]   = new TH1F(title, title, 35,  0.0, 0.090);
    sprintf(title,"HL_HoE[%d]",ii);
    if (ii==0 || ii==1) HL_HoE[ii]    = new TH1F(title, title, 35, -0.01, 0.06);
    if (ii==2 || ii==3) HL_HoE[ii]    = new TH1F(title, title, 35, -0.01, 0.10);
    sprintf(title,"HL_S9S25[%d]",ii);
    if (ii==0 || ii==1) HL_S9S25[ii]  = new TH1F(title, title, 35,  0.8, 1.0);
    if (ii==2 || ii==3) HL_S9S25[ii]  = new TH1F(title, title, 35,  0.5, 1.0);
    sprintf(title,"HL_See[%d]",ii);
    if (ii==0 || ii==2) HL_See[ii]    = new TH1F(title, title, 35,  0.0, 0.02);
    if (ii==1 || ii==3) HL_See[ii]    = new TH1F(title, title, 35,  0.006, 0.026);
    sprintf(title,"HL_EoPout[%d]",ii);
    if (ii==0 || ii==1) HL_EoPout[ii] = new TH1F(title, title, 35,  0.5, 2.5);
    if (ii==2 || ii==3) HL_EoPout[ii] = new TH1F(title, title, 35,  0.5, 4.0);
  }

  
  // reading the tree
  float nEnt = T->GetEntries();
  cout << endl;
  cout << "Total number of events in loop is " << nEnt << endl; 

  for (int entry=0; entry<nEnt; entry++) { 
    if (entry%1000==0) cout << "entry " << entry << endl;
    T -> GetEntry(entry);
      
    float theLClass = 4;
    float theHClass = 4;
    if (L_class==0)                  theLClass=0;
    if (L_class==100)                theLClass=1;
    if (L_class>=30  && L_class<40)  theLClass=2;
    if (L_class>=130 && L_class<140) theLClass=3;
    if (H_class==0)                  theHClass=0;
    if (H_class==100)                theHClass=1;
    if (H_class>=30  && H_class<40)  theHClass=2;
    if (H_class>=130 && H_class<140) theHClass=3;

    for(int ii=0; ii<4; ii++){
      if (ii==theLClass){
	HL_dEta[ii]   -> Fill(L_deltaEta);
	HL_dPhi[ii]   -> Fill(L_deltaPhi);
	HL_HoE[ii]    -> Fill(L_hoe);
	HL_S9S25[ii]  -> Fill(L_s9s25);
	HL_See[ii]    -> Fill(L_see);
	HL_EoPout[ii] -> Fill(L_eopOut);
      }
      if (ii==theHClass){
	HH_dEta[ii]   -> Fill(H_deltaEta);
	HH_dPhi[ii]   -> Fill(H_deltaPhi);
	HH_HoE[ii]    -> Fill(H_hoe);
	HH_S9S25[ii]  -> Fill(H_s9s25);
	HH_See[ii]    -> Fill(H_see);
	HH_EoPout[ii] -> Fill(H_eopOut);
      }
    }
  }

  // writing
  TFile *outFile[4];
  for (int ii=0; ii<4; ii++){     
    if(ii==0) sprintf(title,"outHistos_goldenEB.root");
    if(ii==1) sprintf(title,"outHistos_goldenEE.root");
    if(ii==2) sprintf(title,"outHistos_showeringEB.root");
    if(ii==3) sprintf(title,"outHistos_showeringEE.root");    
    outFile[ii] = new TFile(title,"RECREATE");
    outFile[ii]->cd();
    HL_dEta[ii]   -> Write();
    HL_dPhi[ii]   -> Write();
    HL_HoE[ii]    -> Write();
    HL_S9S25[ii]  -> Write();
    HL_See[ii]    -> Write();
    HL_EoPout[ii] -> Write();
    HH_dEta[ii]   -> Write();
    HH_dPhi[ii]   -> Write();
    HH_HoE[ii]    -> Write();
    HH_S9S25[ii]  -> Write();
    HH_See[ii]    -> Write();
    HH_EoPout[ii] -> Write();
    outFile[ii]->Close();
  }
}
  
