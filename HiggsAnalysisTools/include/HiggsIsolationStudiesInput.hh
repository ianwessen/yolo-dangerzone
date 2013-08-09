//-------------------------------------------------------
// Description:
//    Class for eleID optimization for H->WW->2e2nu channel
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef HiggsIsolationStudiesInput_h
#define HiggsIsolationStudiesInput_h

#include <vector>

#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Counters.hh"
#include "RedIsolationOptimTree.hh"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"

#include "HiggsBase.h"
#include <TH1.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TLorentzVector.h>

class HiggsIsolationStudiesInput : public HiggsBase{
public:
  
  //! constructor
  HiggsIsolationStudiesInput(TTree *tree=0);
  //! destructor
  virtual ~HiggsIsolationStudiesInput();
  //! loop over events
  void Loop(const char *filename);
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  
private:

  //! select events using the MC info about the process
  bool findMcTree(const char* processType);
  //! get the two hardest electrons with opposite charge
  std::pair<int,int> getBestElectronPair();
  //! set the 4 vectors, invariant mass, etc. after preselections
  void setKinematics();  
  //! reset the kinematic quantities at the beginning of event
  void resetKinematics();
  //! full electron id
  bool isEleID(int eleIndex);
  //! offline ambiguity solving
  vector<int> resolvedElectrons();
  //! be verbose during runtime
  bool _verbose;
  //! the list of required triggers
  vector<int> m_requiredTriggers;
  //! process variables to initialize kFactors
  std::string _process;
  float theWeight;

  //! kinematics of the event
  int theElectron,  thePositron;
  TLorentzVector *_p4ElectronPlus, *_p4ElectronMinus;
  float _mll, hardestElectronPt, slowestElectronPt;
  int _theGenEle, _theGenPos;
  //! vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  //! to resolve offline the ambiguity among electrons
  std::vector<int> _resolvedElectrons;
  //! to evaluate eleID
  CutBasedEleIDSelector EgammaCutBasedID;

  

  //! histos
  ofstream *outTxtFile;
  TFile *outRootFile;
  TH1F *H_etEle[5];
  TH1F *Hass_eleSumPt03[5],    *Hass_eleSumPt04[5],    *Hass_eleSumPt05[5];
  TH1F *Hass_eleSumHadEt04[5], *Hass_eleSumHadEt05[5];
  TH1F *Hass_eleSumEmEt04[5],  *Hass_eleSumEmEt05[5];
  TH1F *Hass_eleScBasedEcalSum04[5], *Hass_eleScBasedEcalSum05[5];
  TH1F *Hass_eleScHaloBasedEcalSum04[5], *Hass_eleScHaloBasedEcalSum05[5];
  TH1F *Hass_eleIsoFromDepsTk[5], *Hass_eleIsoFromDepsEcal[5], *Hass_eleIsoFromDepsHcal[5];
  TH1F *Hrel_eleSumPt03[5],    *Hrel_eleSumPt04[5],    *Hrel_eleSumPt05[5];
  TH1F *Hrel_eleSumHadEt04[5], *Hrel_eleSumHadEt05[5];
  TH1F *Hrel_eleSumEmEt04[5],  *Hrel_eleSumEmEt05[5];
  TH1F *Hrel_eleScBasedEcalSum04[5], *Hrel_eleScBasedEcalSum05[5];
  TH1F *Hrel_eleScHaloBasedEcalSum04[5], *Hrel_eleScHaloBasedEcalSum05[5];
  TH1F *Hrel_eleIsoFromDepsTk[5], *Hrel_eleIsoFromDepsEcal[5], *Hrel_eleIsoFromDepsHcal[5];


  //! counters
  float allEvents, passedMc, triggered, commonPresel; 
  float passedReco, elePresel, looseId, looseIsol, fullId;

};
#endif
