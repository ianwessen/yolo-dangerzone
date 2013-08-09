//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed W -> enu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef ZewkSelection_h
#define ZewkSelection_h

#include <vector>
#include "CommonTools/include/Monitor.hh"
#include "HiggsAnalysisTools/include/HiggsBase.h"
#include <TVector3.h>
#include <TLorentzVector.h>

class ZewkSelection : public HiggsBase{
public:
  ZewkSelection(TTree *tree=0);
  virtual ~ZewkSelection();
  void Loop();
  void displayEfficiencies();
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  
private:

  bool findMcTree(const char* processType);
  std::pair<int,int> getBestLeptonPair();
  void setKinematics(int,int); 
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

  TLorentzVector *_p4Ele, *_p4Pos;

  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;

  // new variables
  float _eOverP[100];
  float _nEle[1];
  //! the list of required triggers
  vector<int> m_requiredTriggers;

};
#endif
