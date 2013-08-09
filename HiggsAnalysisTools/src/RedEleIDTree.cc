#include "../include/RedEleIDTree.h"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedEleIDTree::RedEleIDTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","eleID tree");

  // GENERAL block
  myTree->Branch("class",          &myClass,          "class/I");  
  myTree->Branch("hOverE",         &myHOverE,         "hOverE/F");  
  myTree->Branch("eOverP",         &myEOverP,         "eOverP/F");  
  myTree->Branch("eOverPout",      &myEOverPout,      "eOverPout/F");  
  myTree->Branch("deltaEtaAtVtx",  &myDeltaEtaAtVtx,  "deltaEtaAtVtx/F");  
  myTree->Branch("deltaPhiAtVtx",  &myDeltaPhiAtVtx,  "deltaPhiAtVtx/F");  
  myTree->Branch("sigmaIEtaIEta",  &mySigmaIEtaIEta,  "sigmaIEtaIEta/F");  
}


RedEleIDTree::~RedEleIDTree() 
{
  delete myFile;
}

void RedEleIDTree::store()
{
  myTree->Fill();
}


void RedEleIDTree::save() 
{
  myFile->cd();
  myTree->Write();
  myFile->Close();
}


void RedEleIDTree::fillAll(int cl, float hoe, float eop, float eopo, float deta, float dphi, float see)
{
  myClass         = cl;
  myHOverE        = hoe;
  myEOverP        = eop;
  myEOverPout     = eopo;
  myDeltaEtaAtVtx = deta;
  myDeltaPhiAtVtx = dphi;
  mySigmaIEtaIEta = see;
}
