#include <TH1.h>
#include <TFile.h>
#include <TChain.h>
#include <TRandom1.h>
#include <TDatime.h>
#include <iostream>

using namespace std;

void makePUSmearing() {

  // taking the data histo before any rescaling
  TFile *file = new TFile("/afs/cern.ch/user/c/crovelli/public/4PU/all2011B.pileup_v2.root");
  TH1F *dataH = (TH1F*)file->Get("pileup");
  
  // new histo
  TH1D *pu2 = new TH1D("pu2","pu2",36,-0.5,35.5);

  // initialise random numbers
  TRandom1 *r1 = new TRandom1();

  // applying smearing to account for the wrong mb x-sec
  int nPoints = dataH->GetEntries();
  cout << "in total " << nPoints << " entries" << endl;
  for (int mytry=0; mytry<nPoints; mytry++) {
    if( mytry%100000==0) cout << mytry << endl;

    float y = dataH->GetRandom();
    float shiftedY = y+0.5; 
    float myY = (73.5/68.)*shiftedY;
    float backY = myY - 0.5;
    if (shiftedY<0 || shiftedY>36) cout << "bad shiftedY: " << shiftedY << endl;
    if (backY<-0.5 || backY>35.5)  cout << "bad backY: "    << backY    << endl;
    pu2->Fill(backY);
  }

  // saving the tree
  TFile fileN("fileNew.root","RECREATE");
  pu2->Write();
}
