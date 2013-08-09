//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed H->WW->2e2nu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef HiggsSelection_h
#define HiggsSelection_h

#include <vector>
#include "CommonTools/include/Monitor.hh"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"
#include "HiggsAnalysisTools/include/CommonHiggsPreselector.hh"
#include "HiggsAnalysisTools/include/CutBasedHiggsSelector.hh"
#include "HiggsAnalysisTools/include/HiggsBase.h"
#include "HiggsAnalysisTools/include/RedHiggsTree.h"
#include "HiggsAnalysisTools/include/RedTriggerTree.hh"

#include <TVector3.h>
#include <TLorentzVector.h>


class HiggsSelection : public HiggsBase{
public:
  
  //! constructor
  HiggsSelection(TTree *tree=0);
  //! destructor
  virtual ~HiggsSelection();
  //! loop over events
  void Loop();
  //! set the name for dataset in output
  void SetDatasetName(std::string filename) {_datasetName=filename;};
  //! display the efficiency table
  void displayEfficiencies(std::string filename);
  //! set the list of the required triggers
  void requireTrigger(vector<int> requiredTriggers) { m_requiredTriggers = requiredTriggers; }
  
private:

  bool findMcTree(const char* processType);

  //! get the two hardest electrons with opposite charge
  std::pair<int,int> getBestElectronPair();
  //! get the two hardest muons with opposite charge 
  std::pair<int,int> getBestMuonPair();
  //! set the 4 vectors, invariant mass, etc. after preselections
  void setKinematics();  
  //! set the 4 vectors, invariant mass, etc. before preselections
  void setPreselKinematics();  
  //! true if there is a real jet in the event
  bool goodJetFound();
  //! returns the output of the custom cut electron ID
  bool isEleID(int eleIndex);
  //! if the 2nd ele falls in deltaR from first, get its Pt in tracker
  float getSecondEleTkPt(int first, int second, float deltaR);
  //! if the 2nd ele falls in deltaR from first, get its Et in ECAL
  float getSecondEleEmEt(int first, int second, float deltaR);
  //! evaluate cluster shape Fisher discriminant
  float Fisher(int eleIndex);
  //! get the kFactor of the event
  float getkFactor(std::string process);
  //! reset the kinematic quantities at the beginning of event
  void resetKinematics();
  //! offline ambiguity resolution
  std::vector<int> resolvedElectrons();

  //! to evaluate eleID
  CutBasedEleIDSelector EgammaCutBasedID;
  //! to evaluate preselection efficiency
  Selection *_preselection;
  CommonHiggsPreselector CommonHiggsPreselection;
  //! to evaluate full selection efficiency
  Selection *_selectionEE, *_selectionMM, *_selectionEM;
  CutBasedHiggsSelector CutBasedHiggsSelectionEE;
  CutBasedHiggsSelector CutBasedHiggsSelectionMM;
  CutBasedHiggsSelector CutBasedHiggsSelectionEM;
  //! be verbose during runtime
  bool _verbose;
  //! the list of required triggers
  vector<int> m_requiredTriggers;

  //! process variables to initialize kFactors
  int _massVal;
  std::string _process;

  //! an integer defining the sub-channel
  enum { ee=0, mm=1, em=2 };

  //! array containing the possibility of having reconstructed a certain sub-channel
  bool m_channel[3];
  bool isOk[3];

  //! kinematics of the event
  int theElectron,  thePositron;
  int theMuonMinus, theMuonPlus;
  TLorentzVector *m_p4ElectronPlus, *m_p4ElectronMinus;
  TLorentzVector *m_p4MuonPlus, *m_p4MuonMinus;
  TLorentzVector *m_p4MET;
  float m_HoEElectronMinus, m_HoEElectronPlus;
  float m_CaloEneElectronMinus, m_CaloEneElectronPlus;
  float m_deltaPhi[3];
  float m_mll[3];
  float m_transvMass[3];
  //! used for ee final state
  float hardestElectronPt, hardestMuonPt;
  //! used for mm final state
  float slowestElectronPt, slowestMuonPt;    
  //! used for mixed final state
  float hardestLeptonPt, slowestLeptonPt;
  
  int _theGenEle, _theGenPos;
  int _theGenMuMinus, _theGenMuPlus;

  //! vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  std::vector<int> *_bestMuons;
  std::vector<int> *_bestJets;
  std::vector<int> *_bestGenJets;

  //! vector to store indices of candidate to exclude
  std::vector<int> *_excludedJets;

  //! reduced tree for event selection (on at least 2leptons events)
  RedHiggsTree *myOutTreeEE;
  RedHiggsTree *myOutTreeMM;
  RedHiggsTree *myOutTreeEM;

  //! reduced tree for trigger studies (on all events)
  RedTriggerTree *myTriggerTree;

  //! new variables
  float m_eOverP[100];

  float _highestPtGen[1], _lowestPtGen[1];
  float _genHiggsPt[1];
  float _nGenJet[1];
  float _alphaEle[50], _emFracEle[50], _hadFracEle[50];

  //! name of rootfile with dataset
  std::string _datasetName;
};
#endif
