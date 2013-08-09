//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed W -> enu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef WSelection_h
#define WSelection_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>


class WSelection : public HiggsBase{
public:
  WSelection(TTree *tree=0);
  virtual ~WSelection();
  void Loop();
  void displayEfficiencies();
  
private:

  bool findMcTree(const char* processType);

  void bookHistos();
  int getBestLepton();
  void setKinematics(int);
  void addVariables();
  bool jetVeto();
  bool isEleID(int eleIndex);
  float Fisher(int eleIndex);

  // private members
  Counters _counter;
  Counters _eleCounter;
  Selection* _selection;
  std::vector<Selection*> _electronSelection;
  bool _verbose;

  TVector3 *_p3Ele, *_p3Met, *_p3System;
  TLorentzVector *_p4Ele;
  float _elePt;
  float _systemPt;

  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  std::vector<int> *_bestJets;
  std::vector<int> *_bestGenJets;

  // monitoring tools
  Monitor *_monitorEventAfterSelection, *_monitorMet, *_monitorElectrons, *_monitorJets;
  Monitor *_monitorEventAfterReco;
  Monitor *_monitorGenerator, *_monitorGenJets;

  // new variables
  float _eOverP[100];
  float _drEleFromHighestPt[101];
  float _transvMass[1];
  float _histopt[1];
  float _nEle[1], _nJet[1];

  float _elePtGen[1];
  float _ptGen[1];
  float _nGenJet[1];
};
#endif
