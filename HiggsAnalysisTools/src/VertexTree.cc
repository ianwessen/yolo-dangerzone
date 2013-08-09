#include "HiggsAnalysisTools/include/VertexTree.hh"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

VertexTree::VertexTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","higgs vertexing tree");

  myTree->Branch("deltaZ",  &myDeltaZ,   "deltaZ/F");  
  myTree->Branch("deltaXY", &myDeltaXY,  "deltaXY/F");  
  myTree->Branch("deltaXYZ", &myDeltaXYZ, "deltaXYZ/F");  
}

VertexTree::~VertexTree() { delete myFile; }

void VertexTree::store() { myTree->Fill(); }

void VertexTree::save() {

  myFile->cd();
  myTree->Write();
  myFile->Close();
}


void VertexTree::fillAll(float dz, float dxy, float dxyz) { 
  
  myDeltaZ   = dz;
  myDeltaXY  = dxy;
  myDeltaXYZ = dxyz;
}

