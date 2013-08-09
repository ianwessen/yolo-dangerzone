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
  float L_Likelihood;
  float H_Likelihood;
  int L_EleClass;     
  int H_EleClass;

  T->SetMakeClass(1);
  T->SetBranchStatus("*",0);
  T->SetBranchStatus("L_Likelihood",1);
  T->SetBranchStatus("H_Likelihood",1);
  T->SetBranchStatus("L_EleClass",1);
  T->SetBranchStatus("H_EleClass",1);
  T->SetBranchAddress("L_Likelihood",&L_Likelihood);
  T->SetBranchAddress("H_Likelihood",&H_Likelihood);
  T->SetBranchAddress("L_EleClass",  &L_EleClass);
  T->SetBranchAddress("H_EleClass",  &H_EleClass);

  // histos
  // 0 = not showering
  // 1 = showering
  // 2 = all classes
  char title[200];
  TH1F *HH_Likelihood[3];
  TH1F *HL_Likelihood[3];
  for (int ii=0; ii<3; ii++){
    sprintf(title,"HH_Likelihood[%d]",ii);
    HH_Likelihood[ii] = new TH1F(title, title, 50, 0.0, 1.0);
    sprintf(title,"HL_Likelihood[%d]",ii);
    HL_Likelihood[ii] = new TH1F(title, title, 50, 0.0, 1.0);
  }

 
  // reading the tree
  float nEnt = T->GetEntries();
  cout << endl;
  cout << "Total number of events in loop is " << nEnt << endl; 
  
  for (int entry=0; entry<nEnt; entry++) { 
    if (entry%1000==0) cout << "entry " << entry << endl;
    T -> GetEntry(entry);
    
    float theLClass = 0;                             // not showering
    float theHClass = 0;
    if (L_EleClass>=30  && L_EleClass<40)  theLClass=1;    // showering
    if (L_EleClass>=130 && L_EleClass<140) theLClass=1;
    if (H_EleClass>=30  && H_EleClass<40)  theHClass=1;
    if (H_EleClass>=130 && H_EleClass<140) theHClass=1;

    for(int ii=0; ii<2; ii++){
      if (ii==theLClass){ HL_Likelihood[ii] -> Fill(L_Likelihood); }
      if (ii==theHClass){ HH_Likelihood[ii] -> Fill(H_Likelihood); }
    }

    HL_Likelihood[2] -> Fill(L_Likelihood);
    HH_Likelihood[2] -> Fill(H_Likelihood);    
  }
  

  // writing
  TFile *outFile[2];
  for (int ii=0; ii<3; ii++){     
    if(ii==0) sprintf(title,"outHistos_notShowering.root");
    if(ii==1) sprintf(title,"outHistos_showering.root");
    if(ii==2) sprintf(title,"outHistos_allClasses.root");
    outFile[ii] = new TFile(title,"RECREATE");
    outFile[ii]->cd();
    HH_Likelihood[ii] -> Write("HH_Likelihood");
    HL_Likelihood[ii] -> Write("HL_Likelihood");
    outFile[ii]->Close();
  }
}
  
