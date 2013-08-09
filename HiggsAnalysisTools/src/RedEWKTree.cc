#include "HiggsAnalysisTools/include/RedEWKTree.h"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedEWKTree::RedEWKTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","analysis tree");

  // counters
  myNumberOfEle=0;

  // GENERAL block
  myTree->Branch("nEle",    &myNEle,    "nEle/I");  
  myTree->Branch("weight",  &myWeight,  "weight/F");  
  myTree->Branch("met",     &myMet,     "met/F");  
  myTree->Branch("metx",    &myMetx,    "metx/F");  
  myTree->Branch("mety",    &myMety,    "mety/F");  

  // eleID block
  myTree->Branch("iso",        &myIso,        "iso[nEle]/F");
  myTree->Branch("tip",        &myTip,        "tip[nEle]/F");
  myTree->Branch("lip",        &myLip,        "lip[nEle]/F");
  myTree->Branch("sigmaEE",    &mySigmaEE,    "sigmaEE[nEle]/F");
  myTree->Branch("deltaEtaIn", &myDeltaEtaIn, "deltaEtaIn[nEle]/F");
  myTree->Branch("deltaPhiIn", &myDeltaPhiIn, "deltaPhiIn[nEle]/F");
  myTree->Branch("EoverPin",   &myEoverPin,   "EoverPin[nEle]/F");
  myTree->Branch("HoverE",     &myHoverE,     "HoverE[nEle]/F");
  myTree->Branch("charge",     &myCharge,     "charge/I");  
  myTree->Branch("pt",         &myPt,         "pt/F");  
  myTree->Branch("eta",        &myEta,        "eta/F");  
  myTree->Branch("phi",        &myPhi,        "phi/F");  
}

RedEWKTree::~RedEWKTree() {
  delete myFile;
}

void RedEWKTree::store() {
  myTree->Fill();
  myNumberOfEle=0;
}


void RedEWKTree::save() {
  myFile->cd();
  myTree->Write();
  myFile->Close();
}


void RedEWKTree::fillAll(int ne, float iwe, float mt, float mtx, float mty ) {
  myNEle   = ne;
  myWeight = iwe;
  myMet    = mt;
  myMetx   = mtx;
  myMety   = mty;
}

void RedEWKTree::fillEleId(float is, float tp, float lp, float see, float de, float dp, float eop, float hoe, int qe, float pt, float eta, float phi) {
  myIso[myNumberOfEle]        = is;
  myTip[myNumberOfEle]        = tp;
  myLip[myNumberOfEle]        = lp;
  mySigmaEE[myNumberOfEle]    = see;
  myDeltaEtaIn[myNumberOfEle] = de;
  myDeltaPhiIn[myNumberOfEle] = dp;
  myEoverPin[myNumberOfEle]   = eop;
  myHoverE[myNumberOfEle]     = hoe;
  myCharge[myNumberOfEle]     = qe;
  myPt[myNumberOfEle]         = pt;
  myEta[myNumberOfEle]        = eta;
  myPhi[myNumberOfEle]        = phi;
  myNumberOfEle++;
}
