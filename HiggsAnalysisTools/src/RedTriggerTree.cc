#include "HiggsAnalysisTools/include/RedTriggerTree.hh"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

RedTriggerTree::RedTriggerTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("T1","trigger tree");

  myTree->Branch("decayEE",         &myDecayEE,         "decayEE/B");
  myTree->Branch("decayMM",         &myDecayMM,         "decayMM/B");
  myTree->Branch("decayEM",         &myDecayEM,         "decayEM/B");
  myTree->Branch("promptDecayEE",   &myPromptDecayEE,   "promptDecayEE/B");
  myTree->Branch("promptDecayMM",   &myPromptDecayMM,   "promptDecayMM/B");
  myTree->Branch("promptDecayEM",   &myPromptDecayEM,   "promptDecayEM/B");
  myTree->Branch("HLTSingleElectron",         &myHLTSingleElectron,         "HLTSingleElectron/B");
  //  myTree->Branch("HLTSingleElectronRelaxed",  &myHLTSingleElectronRelaxed,  "HLTSingleElectronRelaxed/B");
  //  myTree->Branch("HLTSingleElectronOR",       &myHLTSingleElectronOR,  "HLTSingleElectronOR/B");
  myTree->Branch("HLTSingleMuon",             &myHLTSingleMuon,         "HLTSingleMuon/B");
  //  myTree->Branch("HLTSingleMuonRelaxed",      &myHLTSingleMuonRelaxed,  "HLTSingleMuonRelaxed/B");
  //  myTree->Branch("HLTSingleMuonOR",           &myHLTSingleMuonOR,  "HLTSingleMuonOR/B");

}

RedTriggerTree::~RedTriggerTree() {
  delete myFile;
}

void RedTriggerTree::fillMcTruth(bool decayEE, bool decayMM, bool decayEM, bool promptEE, bool promptMM, bool promptEM) {

  myDecayEE = decayEE;
  myDecayMM = decayMM;
  myDecayEM = decayEM;
  myPromptDecayEE = promptEE;
  myPromptDecayMM = promptMM;
  myPromptDecayEM = promptEM;

}

void RedTriggerTree::fillHLTElectrons(bool singleEle, bool singleEleRelaxed, bool singleEleOR) {

  myHLTSingleElectron = singleEle;
  //  myHLTSingleElectronRelaxed = singleEleRelaxed;
  //  myHLTSingleElectronOR = singleEleOR;

}

void RedTriggerTree::fillHLTMuons(bool singleMuon, bool singleMuonRelaxed, bool singleMuonOR) {

  myHLTSingleMuon = singleMuon;
  //  myHLTSingleMuonRelaxed = singleMuonRelaxed;
  //  myHLTSingleMuonOR = singleMuonOR;

}

void RedTriggerTree::store()
{
  myTree->Fill();
}


void RedTriggerTree::save() 
{
  myFile->cd();
  myTree->Write();
  myFile->Close();
}
