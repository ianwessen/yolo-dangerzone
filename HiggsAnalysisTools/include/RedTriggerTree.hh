#ifndef RedTriggerTree_hh
#define RedTriggerTree_hh

#include "TFile.h"
#include "TTree.h"

class RedTriggerTree {
public:
  RedTriggerTree(const char * filename = "trigger.root");
  ~RedTriggerTree();

  //! fill the MC truth informations
  void fillMcTruth(bool decayEE, bool decayMM, bool decayEM, bool promptEE, bool promptMM, bool promptEM);
  //! fill the HLT electron triggers informations
  void fillHLTElectrons(bool singleEle, bool singleEleRelaxed=false, bool singleEleOR=false);
  //! fill the HLT muons triggers informations
  void fillHLTMuons(bool singleMuon, bool singleMuonRelaxed=false, bool singleMuonOR=false);

  //! effectively store the events in the tree
  void store();
  //! save in the ROOT file
  void save();

private:
  bool myDecayEE;
  bool myDecayMM;
  bool myDecayEM;
  bool myPromptDecayEE;
  bool myPromptDecayMM;
  bool myPromptDecayEM;
  bool myHLTSingleElectron;
  //  bool myHLTSingleElectronRelaxed;
  //  bool myHLTSingleElectronOR;
  bool myHLTSingleMuon;
  //  bool myHLTSingleMuonRelaxed;
  //  bool myHLTSingleMuonOR;

  TFile* myFile;
  TTree* myTree;
 
};

#endif // RedTriggerTree_hh
