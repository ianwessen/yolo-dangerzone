#include "HiggsAnalysisTools/include/RedEleIDOptimTree.hh"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedEleIDOptimTree::RedEleIDOptimTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","eleID tree");

  myTree->Branch("H_deltaEta", &myH_DeltaEta,  "H_deltaEta/F");  
  myTree->Branch("H_deltaPhi", &myH_DeltaPhi,  "H_deltaPhi/F");  
  myTree->Branch("H_hoe",      &myH_Hoe,       "H_hoe/F");  
  myTree->Branch("H_s9s25",    &myH_S9s25,     "H_s9s25/F");  
  myTree->Branch("H_see",      &myH_See,       "H_see/F");  
  myTree->Branch("H_eopOut",   &myH_EopOut,    "H_eopOut/F");  
  myTree->Branch("H_class",    &myH_Class,     "H_class/I");  
  myTree->Branch("L_deltaEta", &myL_DeltaEta,  "L_deltaEta/F");  
  myTree->Branch("L_deltaPhi", &myL_DeltaPhi,  "L_deltaPhi/F");  
  myTree->Branch("L_hoe",      &myL_Hoe,       "L_hoe/F");  
  myTree->Branch("L_s9s25",    &myL_S9s25,     "L_s9s25/F");  
  myTree->Branch("L_see",      &myL_See,       "L_see/F");  
  myTree->Branch("L_eopOut",   &myL_EopOut,    "L_eopOut/F");  
  myTree->Branch("L_class",    &myL_Class,     "L_class/I");  
}

RedEleIDOptimTree::~RedEleIDOptimTree() 
{
  delete myFile;
}

void RedEleIDOptimTree::store()
{
  myTree->Fill();
}


void RedEleIDOptimTree::save() 
{
  myFile->cd();
  myTree->Write();
  myFile->Close();
}


void RedEleIDOptimTree::fillAll(float fhdeta, float fhdphi, float fhhoe, float fhs9s25, float fhsee, float fheopout, int fhclass, float fldeta, float fldphi, float flhoe, float fls9s25, float flsee, float fleopout, int flclass)
{
  myH_DeltaEta = fhdeta;
  myH_DeltaPhi = fhdphi;
  myH_Hoe      = fhhoe;
  myH_S9s25    = fhs9s25;
  myH_See      = fhsee;
  myH_EopOut   = fheopout;
  myH_Class    = fhclass;
  myL_DeltaEta = fldeta;
  myL_DeltaPhi = fldphi;
  myL_Hoe      = flhoe;
  myL_S9s25    = fls9s25;
  myL_See      = flsee;
  myL_EopOut   = fleopout;
  myL_Class    = flclass;
}

