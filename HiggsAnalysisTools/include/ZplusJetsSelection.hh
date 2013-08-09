//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed W -> enu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef ZplusJetsSelection_h
#define ZplusJetsSelection_h

#include <vector>
#include "CommonTools/include/Monitor.hh"
#include "HiggsAnalysisTools/include/HiggsBase.h"
#include <TVector3.h>
#include <TLorentzVector.h>

class ZplusJetsSelection : public HiggsBase{
public:
  ZplusJetsSelection(TTree *tree=0);
  virtual ~ZplusJetsSelection();
  void Loop();
  void displayEfficiencies();
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  void writeHistos();
  
private:

  bool ZplusJetsSelection::isZplusJets();
  std::pair<int,int> getBestLeptonPair();
  void setKinematics(int,int); 
  bool isGoldenID(int eleIndex);
  bool isRobustID(int eleIndex);
  void addVariables();
  std::vector<int> getGoodJets(int theEle, int thePos);

  // private members
  Counters _counter;
  Counters _eleCounter;
  Counters _counterExclusive;
  Selection* _selection;
  std::vector<Selection*> _eleGoldenSelection;
  std::vector<Selection*> _eleRobustSelection;
  bool _verbose;

  TLorentzVector *_p4Ele, *_p4Pos;
  float m_HoEElectronMinus, m_HoEElectronPlus;
  float m_CaloEneElectronMinus, m_CaloEneElectronPlus;

  TH1F *H_mee0jets, *H_mee1jets, *H_mee2jets, *H_mee3jets, *H_mee4jets, *H_mee5jets; 
   
  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;

  // new variables
  float _eOverP[100];
  float _nEle[1];
  //! the list of required triggers
  vector<int> m_requiredTriggers;

};
#endif
