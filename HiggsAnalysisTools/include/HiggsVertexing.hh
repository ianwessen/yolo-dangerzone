#ifndef HiggsVertexing_h
#define HiggsVertexing_h

#include <vector>

#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Counters.hh"
#include "VertexTree.hh"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"

#include "HiggsBase.h"
#include <TH1.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TLorentzVector.h>

class HiggsVertexing : public HiggsBase{
public:
  
  //! constructor
  HiggsVertexing(TTree *tree=0);
  //! destructor
  virtual ~HiggsVertexing();
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
  //! if the 2nd ele falls in deltaR from first, get its Pt in tracker
  float getSecondEleTkPt(int first, int second, float deltaR);
  //! if the 2nd ele falls in deltaR from first, get its Et in ECAL
  float getSecondEleEmEt(int first, int second, float deltaR);
  //! be verbose during runtime
  bool _verbose;
  //! the list of required triggers
  vector<int> m_requiredTriggers;

  //! kinematics of the event
  int theElectron,  thePositron;
  TLorentzVector *_p4ElectronPlus, *_p4ElectronMinus;
  float _mll, _deltaPhi, hardestElectronPt, slowestElectronPt;
  int _theGenEle, _theGenPos;
  //! vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  //! to evaluate eleID
  CutBasedEleIDSelector EgammaCutBasedID;

  //! histos
  TFile *outRootFile;
  VertexTree *outRootTree;
  TH1F *H_deltaZ;
  TH1F *H_deltaXY;
  TH1F *H_deltaXYZ;
  TH1F *H_trackDz;
  TH1F *H_trackDxy;
  TH1F *H_trackDxyz;
  TH1F *H_compZ;
  TH1F *H_compXY;
  TH1F *H_compXYZ;

  ofstream *outTxtFile;

  //! counters
  float allEvents, passedMc, triggered, commonPresel; 
  float passedReco, elePresel, looseId, looseIsol;
  float fullId, trackerIsol, ecalIsol, hcalIsol;

};
#endif
