//-------------------------------------------------------
// Description:
//    Class for estimation of electron ID efficiency
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef ElectronID_h
#define ElectronID_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1F.h>

class ElectronID : public HiggsBase{
public:
  ElectronID(TTree *tree=0);
  virtual ~ElectronID();
  void Loop();
  void displayEfficiencies();
  
private:

  bool findMcTree(const char* processType);

  void bookHistos();
  std::pair<int,int> getBestLeptonPair();
  void setKinematics(int,int);  
  void addVariables();
  bool jetVeto();
  bool isEleID(int eleIndex);
  float Fisher(int eleIndex);
  void fillEfficiencyNumeratorHistos(TH1F* etaWom, TH1F* etaWm, TH1F* etWm, std::pair<int,int> bestElectronPair, const char* cut="");
  bool isInCrack(float eta);

  // private members
  Counters _counter;
  Counters _eleCounter;
  Selection* _selection;
  std::vector<Selection*> _electronSelection;
  bool _verbose;

  TVector3 *_p3Ele, *_p3Pos, *_p3Met;
  TLorentzVector *_p4Ele, *_p4Pos;
  float _maxPt,_minPt;

  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  std::vector<int> *_bestJets;
  std::vector<int> *_bestGenJets;

  // monitoring tools
  Monitor *_monitorEventAfterSelection, *_monitorMet, *_monitorElectrons, *_monitorJets;
  Monitor *_monitorEventAfterReco;
  Monitor *_monitorGenerator, *_monitorGenJets;

  // simple histograms
  TH1F *RecoSingle_eta_wcm, *RecoSingle_eta_wocm, *RecoSingleTrg_eta_wcm, *RecoSingleTrg_eta_wocm, *RecoSingleIso_eta_wcm, *RecoSingleIso_eta_wocm, *RecoSingleEId_eta_wcm, *RecoSingleEId_eta_wocm, *Gene_eta;
  TH1F *RecoSingle_pt_wcm, *RecoSingleTrg_pt_wcm, *RecoSingleIso_pt_wcm, *RecoSingleEId_pt_wcm, *Gene_pt;

  // new variables
  float _eOverP[100];
  float _drEleFromHighestPt[101];
  float _drPosFromHighestPt[101];
  float _deltaPhi[1];
  float _mll[1];
  float _transvMass[1];
  float _highestPt[1], _lowestPt[1];
  float _nEle[1], _nJet[1];

  float _highestPtGen[1], _lowestPtGen[1];
  float _nGenJet[1];
};
#endif
