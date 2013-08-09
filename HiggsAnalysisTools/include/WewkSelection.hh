//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed W -> enu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef WewkSelection_h
#define WewkSelection_h

#include <vector>
#include "CommonTools/include/Monitor.hh"
#include "HiggsBase.h"
#include "RedEWKTree.h"

#include <TVector3.h>
#include <TLorentzVector.h>


class WewkSelection : public HiggsBase{
public:
  WewkSelection(TTree *tree=0);
  virtual ~WewkSelection();
  void Loop();
  void displayEfficiencies();
  
private:

  bool findMcTree(const char* processType);
  int getBestLepton();
  bool isGoldenID(int eleIndex);
  bool isRobustID(int eleIndex);
  void addVariables();

  // private members
  Counters _counter;
  Counters _eleCounter;
  Selection* _selection;
  std::vector<Selection*> _eleGoldenSelection;
  std::vector<Selection*> _eleRobustSelection;
  bool _verbose;

  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;

  // reduced tree
  RedEWKTree *myOutTree;

  // new variables
  float _eOverP[100];
  float _nEle[1];

  //! the list of required triggers
  vector<int> m_requiredTriggers;

};
#endif
