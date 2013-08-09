#include "HiggsAnalysisTools/include/RedLikeOptimTree.hh"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedLikeOptimTree::RedLikeOptimTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","eleID tree");

  myTree->Branch("H_Likelihood", &myH_Likelihood,  "H_Likelihood/F");  
  myTree->Branch("L_Likelihood", &myL_Likelihood,  "L_Likelihood/F");  
  myTree->Branch("H_EleClass",   &myH_EleClass,    "H_EleClass/I");  
  myTree->Branch("L_EleClass",   &myL_EleClass,    "L_EleClass/I");  
}

RedLikeOptimTree::~RedLikeOptimTree()  {

  delete myFile;
}

void RedLikeOptimTree::store() {

  myTree->Fill();
}


void RedLikeOptimTree::save() {

  myFile->cd();
  myTree->Write();
  myFile->Close();
}

void RedLikeOptimTree::fillAll(float fhlike, float fllike, int hclass, int lclass) {

  myH_Likelihood = fhlike;
  myL_Likelihood = fllike;
  myH_EleClass   = hclass;
  myL_EleClass   = lclass;
}

