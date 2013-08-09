/// The Higgs class is an auxiliary class which contains basic
/// functionality useful for any analysis of Vecbos+jets events.
/// It derives from HiggsBase.

#ifndef Higgs_h
#define Higgs_h

#include "EgammaAnalysisTools/include/ElectronLikelihood.h"
#include "HiggsAnalysisTools/include/HiggsBase.h"
#include "HiggsAnalysisTools/include/JetCorrectionUncertainty.h"
#include "EgammaAnalysisTools/include/CutBasedEleIDSelector.hh"
#include "HiggsAnalysisTools/include/ElectronIDMVA.h"
// ROOT includes
#include <TLorentzVector.h>
#include <TVector3.h>
// std includes
#include <string>
#include <vector>
#include <map>

class Higgs : public HiggsBase{

public:
  typedef std::pair<unsigned int,unsigned int> aLSSegment;
  typedef std::vector< std::pair<unsigned int,unsigned int> > LSSegments;
  typedef unsigned int aRun;
  typedef std::map< aRun, LSSegments > runsLSSegmentsMap;
  typedef std::pair < aRun, LSSegments > aRunsLSSegmentsMapElement;

  enum samples { kDATA2011=0,
                 kMC2011, 
                 kDATA2012,
                 kMC2012 };

  /// Class Constructor
  Higgs(TTree *tree=0);
  /// Class Destructor
  virtual ~Higgs();

  /// Fill RunLSMap according to json file
  void fillRunLSMap();
  /// Set Good Run LS
  void setJsonGoodRunList(const std::string& jsonFilePath);
  /// check if Run/LS is a good one
  bool isGoodRunLS();
  /// reload TriggerMask if necessary (data file is changed). Should be called for each event inside the event loop
  bool reloadTriggerMask(bool newVersion=false);
  /// set the list of required trigger to produce the bitmask
  void setRequiredTriggers(const std::vector<std::string>& reqTriggers);
  /// check if the event passed HLT. To be called per event
  bool hasPassedHLT();
  /// get the required HLT path given the current run
  std::string getHLTPathForRun(int runN, std::string fullname);
  //get the value of the requested bits
  std::vector<int> getHLTOutput();

  /// Get pt given x/y coordinates
  float GetPt(float px, float py) { return TMath::Sqrt(px*px + py*py); }

  // useful electron functions
  /// sigma ieta ieta of the seed cluster (ecal-driven/tracker-driven)
  float SigmaiEiE(int electron);
  /// sigma iphi iphi of the seed cluster (ecal-driven/tracker-driven)
  float SigmaiPiP(int electron);
  // get the likelihood electron ID
  float likelihoodRatio(int eleIndex, ElectronLikelihood &lh);
  // get the PFjet ID
  bool isPFJetID(float eta, float nHFrac, float nEmFrac, int nConst, float chHFrac, float chMult, float chEmFrac, int WP);
  //! Chris kinematic variables
  double CalcMTR(TLorentzVector ja, TLorentzVector jb, TVector3 met);
  double CalcMRstar(TLorentzVector ja, TLorentzVector jb);
  double CalcGammaMRstar(TLorentzVector ja, TLorentzVector jb);
  double CalcMRNEW(TLorentzVector P, TLorentzVector Q, TVector3 MET);
  TVector3 XYCorrectedMet(TVector3 MET, int sample);
  std::vector<int> sortElectronsByPt(std::vector<int> electrnons);
  std::vector<int> sortMuonsByPt(std::vector<int> muons);
  TLorentzVector GetJESCorrected(TLorentzVector p4jet, const char *ScaleDirection);
  //! comput the PF MET with charged PFcandidates = -ptL1 -ptL2 - sum_i(ptChPFcand_i)
  TVector3 pfChargedMet(TVector3 lep1, TVector3 lep2);
  // the lepton fakeable object definition
  bool isEleDenomFake(int theEle, bool *isDenomEleID, bool *isDenomEleIso);
  bool isMuonDenomFake(int theMuon, bool *isDenomMuonID, bool *isDenomMuonIso);
  //! returns the output of the custom muon ID
  void isMuonID(int muonIndex, bool *muonIdOutput);
  void isMuonID2012(int muonIndex, bool *muonIdOutput);
  //! hardcoded muon isolation
  bool isPFIsolatedMuon(int muonIndex);
  bool isPFIsolatedMuon2012(int muonIndex);
  //! returns the output of the custom cut electron ID with WPXX
  void isEleID(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput, CutBasedEleIDSelector *thisCutBasedID, bool applyBDTIdNotCutbased=false);
  //! returns the output of the custom cut electron ID with WPXX && deominator selection
  void isEleIDAndDenom(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput, CutBasedEleIDSelector *thisCutBasedID, bool applyBDTIdNotCutbased=false);
  //! returns the value of corrected PF isolation
  float corrEleIso2012(int eleIndex);
  //! returns the output of the custom cut electron ID with WPXX
  void isEleID2012(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput);
  //! returns the output of the custom cut electron ID with WPXX && deominator selection
  void isEleID2012AndDenom(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput);
  /// dxy, dz and dsz parameters with respect to PV for tracks
  double trackDxyPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom);
  double trackDzPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom);
  double trackDszPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom);
  /// dxy, dz and dsz parameters with respect to PV for electrons
  double eleDxyPV(int iele, int iPV);
  double eleDzPV(int iele, int iPV);
  double eleDszPV(int iele, int iPV);
  /// dxy, dz and dsz parameters with respect to PV for muons
  double muonDxyPV(int imu, int iPV);
  double muonDzPV(int imu, int iPV);
  double muonDszPV(int imu, int iPV);
  /// return 1/0 if the three methods to estimate the charge gives the same result / not
  int eleChargeMajority(int iele);
  /// return the value of electron BDT
  float eleBDT(ElectronIDMVA *mva, int iele);
  /// apply the BDT cut
  bool passEleBDT(float pt, float eta, float bdtoutput);
  /// filter the good PVs
  bool goodPV(int v);

  enum jetIdWP { none=0, loose=1, medium=2, tight=3 };

private:
  ///goodRUN/LS list
  runsLSSegmentsMap goodRunLS; 
  std::string jsonFile;

  std::string lastFile;
  std::vector<std::string> requiredTriggers;

  JetCorrectionUncertainty *jecUnc_calo;
  JetCorrectionUncertainty *jecUnc_PF;

protected:
  //! the list of required triggers
  std::vector<int> m_requiredTriggers;

  /// calculate transverse mass
  /// definitions in http://indico.cern.ch/getFile.py/access?contribId=4&resId=0&materialId=slides&confId=104213
  float mT3(TLorentzVector pl1, TLorentzVector pl2, TVector3 met);

  /// MVA for electron ID. To be created and initialized from the children classes
  ElectronIDMVA *fMVA;

};

#endif
