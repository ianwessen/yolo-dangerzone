//-------------------------------------------------------
// Description:
//    Class for eleID optimization for H->WW->2e2nu channel
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef HiggsIsolationOptimToyMC_h
#define HiggsIsolationOptimToyMC_h

#include <vector>

#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Counters.hh"
#include "RedIsolationOptimTree.hh"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"

#include "HiggsBase.h"
#include <TH1.h>
#include <TH1F.h>
#include "THnSparse.h"
#include <TVector3.h>
#include <TLorentzVector.h>

class HiggsIsolationOptimToyMC : public HiggsBase{
public:
  
  //! constructor
  HiggsIsolationOptimToyMC(TTree *tree=0);
  //! destructor
  virtual ~HiggsIsolationOptimToyMC();
  //! loop over events
  void Loop();
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
  //! jet veto
  bool goodJetFound();
  //! full electron id
  bool isEleID(int eleIndex);
  //! if the 2nd ele falls in deltaR from first, get its Pt in tracker
  float getSecondEleTkPt(int first, int second, float deltaR);
  //! if the 2nd ele falls in deltaR from first, get its Et in ECAL
  float getSecondEleEmEt(int first, int second, float deltaR);
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
  float _mll, _deltaPhi, hardestElectronPt, slowestElectronPt;
  float _HoEElectronMinus, _HoEElectronPlus;
  float _CaloEneElectronMinus, _CaloEneElectronPlus;
  int _theGenEle, _theGenPos;
  //! vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  //! to evaluate eleID
  CutBasedEleIDSelector EgammaCutBasedID;

  //! histos
  TFile *outRootFile;
  RedIsolationOptimTree *outRootTree;
  int   theBins[3];
  double theMin[3], theMax[3], theWidth[3];
  TH1F *HH_tracker,  *HH_hcal, *HH_ecal;
  TH1F *HL_tracker,  *HL_hcal, *HL_ecal;
  THnSparseF *HH_NVarDim, *HL_NVarDim; 
  int nVar;
  int theHmass;

  //! counters
  float allEvents, passedMc, triggered, commonPresel; 
  float passedReco, elePresel, looseId, looseIsol, fullId, fullKine;

};
#endif
