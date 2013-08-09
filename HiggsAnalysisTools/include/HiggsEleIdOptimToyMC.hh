//-------------------------------------------------------
// Description:
//    Class for eleID optimization for H->WW->2e2nu channel
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef HiggsEleIdOptimToyMC_h
#define HiggsEleIdOptimToyMC_h

#include <vector>

#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Counters.hh"
#include "RedEleIDOptimTree.hh"
#include "RedLikeOptimTree.hh"

#include "HiggsBase.h"
#include <TH1.h>
#include <TH1F.h>
#include "THnSparse.h"
#include <TVector3.h>
#include <TLorentzVector.h>

class HiggsEleIdOptimToyMC : public HiggsBase{
public:
  
  //! constructor
  HiggsEleIdOptimToyMC(TTree *tree=0);
  //! destructor
  virtual ~HiggsEleIdOptimToyMC();
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
  //! jet veto
  bool goodJetFound();
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

  //! histos
  TFile *outRootFile;
  RedEleIDOptimTree *outRootTree;
  RedLikeOptimTree  *outLikeTree;
  int   theBins[6];
  double theMin[6], theMax[6], theWidth[6];
  TH1F *HH_dEta,  *HH_dPhi, *HH_HoE;
  TH1F *HH_S9S25, *HH_See,  *HH_EoPout;
  THnSparseF *HH_NVarDim;
  TH1F *HL_dEta,  *HL_dPhi, *HL_HoE;
  TH1F *HL_S9S25, *HL_See,  *HL_EoPout;
  THnSparseF *HL_NVarDim;
  TH1F *HH_like,  *HL_like;
  int nVar;
  int theClass;
  int theHmass;
  int theToy;

  ofstream *outTxtFile;

  //! counters
  float allEvents, passedMc, triggered, commonPresel; 
  float passedReco, elePresel, looseId, passedIsol, twoGoodClass, fullKine;

};
#endif
