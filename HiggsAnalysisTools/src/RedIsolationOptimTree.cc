#include "HiggsAnalysisTools/include/RedIsolationOptimTree.hh"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedIsolationOptimTree::RedIsolationOptimTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","eleID tree");

  myTree->Branch("H_trackerIsol", &myH_trackerIsol, "H_trackerIsol/F");  
  myTree->Branch("H_hcalIsol",    &myH_hcalIsol,    "H_hcalIsol/F");  
  myTree->Branch("H_ecalIsol",    &myH_ecalIsol,    "H_ecalIsol/F");  
  myTree->Branch("L_trackerIsol", &myL_trackerIsol, "L_trackerIsol/F");  
  myTree->Branch("L_hcalIsol",    &myL_hcalIsol,    "L_hcalIsol/F");  
  myTree->Branch("L_ecalIsol",    &myL_ecalIsol,    "L_ecalIsol/F");  
}

RedIsolationOptimTree::~RedIsolationOptimTree() 
{
  delete myFile;
}

void RedIsolationOptimTree::store()
{
  myTree->Fill();
}


void RedIsolationOptimTree::save() 
{
  myFile->cd();
  myTree->Write();
  myFile->Close();
}

void RedIsolationOptimTree::fillAll(float fhtracker, float fhhcal, float fhecal, float fltracker, float flhcal, float flecal)
{
  myH_trackerIsol = fhtracker;
  myH_hcalIsol    = fhhcal;
  myH_ecalIsol    = fhecal;
  myL_trackerIsol = fltracker;
  myL_hcalIsol    = flhcal;
  myL_ecalIsol    = flecal;
}

