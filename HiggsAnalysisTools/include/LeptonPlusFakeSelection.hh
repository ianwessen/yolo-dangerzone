//-------------------------------------------------------
// Description:
//    Class for selection of Lepton+Fake to estimate W+jets
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
//-------------------------------------------------------

#ifndef LeptonPlusFakeSelection_h
#define LeptonPlusFakeSelection_h

#include <vector>
#include "CommonTools/include/Monitor.hh"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"
#include "HiggsAnalysisTools/include/CommonHiggsPreselector.hh"
#include "HiggsAnalysisTools/include/CutBasedHiggsSelector.hh"
#include "HiggsAnalysisTools/include/HiggsBase.h"
#include "RedHiggsTree.h"

#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1F.h>

class LeptonPlusFakeSelection : public HiggsBase{
public:
  
  //! constructor
  LeptonPlusFakeSelection(TTree *tree=0);
  //! destructor
  virtual ~LeptonPlusFakeSelection();
  //! loop over events
  void Loop();
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  
private:

  //! fake rate estimation sources
  enum { QCD=0, Wjets=1 };

  bool findMcTree(const char* processType);

  //! hardcode the fake rates
  void initialiseFakeRate();
  //! get the combination L1+L2 (opposite charge)
  std::pair<int,int> getBestElectronPair();
  //! get the combination L1+Fake
  void getBestLplusFakePair();
  //! set the 4 vectors, invariant mass, etc.
  void setKinematics();  
  //! true if there is a real jet in the event
  bool goodJetFound();
  //! returns the output of the custom cut electron ID
  bool isEleID(int eleIndex);
  //! reset the kinematic quantities at the beginning of event
  void resetKinematics();
  //! if the 2nd ele falls in deltaR from first, get its Pt in tracker
  float getSecondEleTkPt(int first, int second, float deltaR);
  //! if the 2nd ele falls in deltaR from first, get its Et in ECAL
  float getSecondEleEmEt(int first, int second, float deltaR);
  //! get the fake rate value
  float getFakeRate(float fakePt, int source=QCD);
  float getFakeRateError(float fakePt, int source=QCD);

  //! the L1 and the Fake
  int m_theL1, m_theFake;
  TLorentzVector m_theL14Momentum, m_theFake4Momentum;

  //! to evaluate eleID
  CutBasedEleIDSelector EgammaCutBasedID;
  //! to evaluate full selection efficiency
  Selection *_selectionEE;
  CutBasedHiggsSelector LFakeFromQCDSelector, LFakeErrorFromQCDSelector;
  CutBasedHiggsSelector LFakeFromWjetsSelector, LFakeErrorFromWjetsSelector;
  //! the list of required triggers
  vector<int> m_requiredTriggers;

  //! process variables to initialize kFactors
  int _massVal;

  //! kinematics of the event
  float m_deltaPhi, m_mll, m_MET;

  //! the fake rate numbers
  float m_fakeRateFromQCD[7], m_fakeRateFromQCD_err[7];
  float m_fakeRateFromWjets[7], m_fakeRateFromWjets_err[7];
  float m_minFakePt[7], m_maxFakePt[7];

  //! comparison of distributions
  TH1F *m_histoLL_mll, *m_histoLD_mll;
  TH1F *m_histoLL_ptmax, *m_histoLD_ptmax;
  TH1F *m_histoLL_ptmin, *m_histoLD_ptmin;
  TH1F *m_histoLL_met, *m_histoLD_met;
  TH1F *m_histoLL_deltaphi, *m_histoLD_deltaphi;

};
#endif
