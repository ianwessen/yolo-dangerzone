#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TString.h>
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include "massDependentCuts.cc"

enum { ee=0, mm=1, em=2, me=3 };

void scanEvents(const char* file, int njets, TString addCut);

void estimateOverlap(const char* file, int njets) {

  //  int mH[18] = {115,120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600};
  int mH[9] = {120,130,140,150,160,170,180,190,200};
  for(int i=0; i<9;i++) {
    std::cout << "mH = " << mH[i] << std::endl;
    TString addCut = higgsCuts(mH[i],true);
    scanEvents(file, njets,addCut);
  }
  
}


void scanEvents(const char* file, int njets, TString addCut) {

  char njcut[30];
  sprintf(njcut, "njets==%d", njets);
  char wwselcut[30];
  if(njets==0) sprintf(wwselcut,"WWSel");
  else if(njets==1) sprintf(wwselcut,"WWSel1j");
  else {
    std::cout << "Jet bin must be 0/1" << std::endl;
    return;
  }

  TFile *fileData  = TFile::Open(file);

  TTree *treeData  = (TTree*)fileData->Get("T1");

  treeData->SetScanField(0);
  treeData->Scan("run:lumi:event",(TString(wwselcut)+TString(" && ")+TString(njcut)+TString(" && ")+addCut).Data());

  return;

}

