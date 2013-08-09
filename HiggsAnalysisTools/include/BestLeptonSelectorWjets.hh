//-------------------------------------------------------
// Description:
//    Class for estimating the best criteria to select the true electron in W+jets
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
//-------------------------------------------------------

#ifndef BestLeptonSelectorWjets_h
#define BestLeptonSelectorWjets_h

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

class BestLeptonSelectorWjets : public HiggsBase{
public:
  
  //! constructor
  BestLeptonSelectorWjets(TTree *tree=0);
  //! destructor
  virtual ~BestLeptonSelectorWjets();
  //! loop over events
  void Loop();
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  //! compute mc-matching efficiency
  void computeMatchEfficiency();
  
private:

  //! fake rate estimation sources
  enum { QCD=0, Wjets=1 };

  //! get the index of MC truth electron
  int indexMcTruthElectronWToENu();
  //! get the dR between a reco ele and the mc truth one
  float deltaR_MCmatch(int iMc, int iEle);
  //! get the best electron with different criteria
  void getBestElectronFunny(std::vector<int> goodElectrons);
  //! offline resolve electrons
  std::vector<int> resolvedElectrons();

  int _bestByPt, _bestBySCenergy, _bestByTrackerIsolation, _bestByHcalIsolation, _bestByElectronIdLH;
  int _secondbestByPt, _secondbestBySCenergy, _secondbestByTrackerIsolation, _secondbestByHcalIsolation, _secondbestByElectronIdLH;

  //! the list of required triggers
  vector<int> m_requiredTriggers;

  //! the list of electrons in the acceptance
  vector<int> _acceptanceElectrons;

};
#endif
