#ifndef CutBasedHiggsSelector_h
#define CutBasedHiggsSelector_h

#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Counters.hh"

class CutBasedHiggsSelector {

public:

  //! constructor
  CutBasedHiggsSelector();

  //! copy constructor
  CutBasedHiggsSelector( const CutBasedHiggsSelector& selector );

  //! destructor
  virtual ~CutBasedHiggsSelector();   

  //! configure from files
  void Configure(const char *fileCuts, const char* fileSwitches, const char *theTitle);

  //! configure pre-selection (it is not necessarily applied)
  void AppendPreselection(Selection *preselection) { _selection->append(preselection); }

  //! get the applied selection
  Selection* GetSelection() { return _selection; }

  //! set event by event observables
  void SetMcTruth(bool foundMcTree)   { m_foundMcTree = foundMcTree; }
  void SetHLT(bool passedHLT)         { m_passedHLT   = passedHLT;   }
  void SetIsChannel(bool channelsel)  { m_isThisChannel = channelsel; }
  void SetProcessID(int processID)       { m_processID     = processID; }
  void SetWeight(float weight)           { m_weight        = weight;    }
  void SetHighElePt(float highPt)        { m_highPt        = highPt;    }
  void SetLowElePt(float lowPt)          { m_lowPt         = lowPt;     }
  void SetInvMass(float mll)             { m_invMass       = mll;       }
  void SetElectronId(int isEleId)       { m_isElectronId  = isEleId; }
  void SetPositronId(int isPosId)       { m_isPositronId  = isPosId; }
  void SetElectronIsolation(int isEleIsol)   { m_isElectronIsol  = isEleIsol; }
  void SetPositronIsolation(int isPosIsol)   { m_isPositronIsol  = isPosIsol; }
  void SetElectronConvRejection(int isEleConvRej)   { m_isElectronConvRej  = isEleConvRej; }
  void SetPositronConvRejection(int isPosConvRej)   { m_isPositronConvRej  = isPosConvRej; }
  void SetElectronIp(int isEleIp)       { m_isElectronIp  = isEleIp; }
  void SetPositronIp(int isPosIp)       { m_isPositronIp  = isPosIp; }
  void SetJetVeto(bool passedCJV)        { m_passedJetVeto = passedCJV; }
  void SetUncorrJetVeto(bool passedUncorrCJV) { m_passedUncorrJetVeto = passedUncorrCJV; }
  void SetNJets(int njets)               { m_nJets         = njets; }
  void SetNUncorrJets(int nuncorrjets)   { m_nUncorrJets   = nuncorrjets; }
  void SetBTagJets(float btag)           { m_btagJets      = btag; }
  void SetNSoftMuons(int nsoftmu)        { m_nSoftMuons    = nsoftmu; }
  void SetNExtraLeptons(int nextralep)   { m_nExtraLeptons = nextralep; }
  void SetMet(float met)                 { m_met           = met;}
  void SetProjectedMet(float projmet)    { m_projectedMet  = projmet;}
  void SetMetOverPtLL(float metopt)      { m_metOverPtLL   = metopt;}
  void SetDYMVA(float dymva)             { m_dymva         = dymva; }
  void SetDeltaPhiLLJet(float deltaphiLLJet) { m_deltaPhiLLJet = deltaphiLLJet; }
  void SetDeltaPhi(float deltaPhi)       { m_deltaPhi      = deltaPhi;}
  void SetDetaLeptons(float deltaEta)    { m_detaLeptons   = deltaEta;}
  void SetPtll(float ptll) { m_ptll = ptll; }
  void SetExtraSlowLeptonPTCut(float ptMin) { m_extraSlowLeptonPTMin = ptMin; }
  void SetWWInvMass(float wwmass) { m_WWInvMass = wwmass; }
  void SetNvtx(int nvtx) { m_nVtx = nvtx; }

  //! get output of the selector
  bool output();
  //! the methods outputUpToFinalLeptons and outputUpToJetVeto need 
  //! output() run before using them
  //! get output of the selector until lepton ID + isolation
  bool outputUpToFinalLeptons() { return m_finalLeptons; }
  //! get output of the selector until jet veto
  bool outputUpToJetVeto() {return m_jetVeto; }
  bool outputUpToUncorrJetVeto() {return m_uncorrJetVeto; }
  //! get output of the selector previous to deltaPhi cut
  bool outputPreDeltaPhi() { return m_preDeltaPhi; }

  //! latinos 
  bool outputStep0() { return m_step0; }
  bool outputStep1() { return m_step1; }
  bool outputStep2() { return m_step2; }
  bool outputStep3() { return m_step3; }
  bool outputStep4() { return m_step4; }
  bool outputStep5() { return m_step5; }
  bool outputStep6() { return m_step6; }
  bool outputStep7() { return m_step7; }
  bool outputStep8() { return m_step8; }
  bool outputStep9() { return m_step9; }
  bool outputStep10() { return m_step10; }
  bool outputStep11() { return m_step11; }
  bool outputStep12() { return m_step12; }
  bool outputStep13() { return m_step13; }
  bool outputStep14() { return m_step14; }
  bool outputStep15() { return m_step15; }
  bool outputStep16() { return m_step16; }
  bool outputStep17() { return m_step17; }
  bool outputStep18() { return m_step18; }
  bool outputStep19() { return m_step19; }
  bool outputStep20() { return m_step20; }
  bool outputStep21() { return m_step21; }
  bool outputStep22() { return m_step22; }
  bool outputStep23() { return m_step23; }
  bool outputStep24() { return m_step24; }
  bool outputStep25() { return m_step25; }
  bool outputStep26() { return m_step26; }

  //! display the electron efficiency
  void displayEfficiencies(std::string datasetName);

private:
  
  float m_weight;
  bool m_foundMcTree;
  bool m_passedHLT;
  bool m_isThisChannel;
  float m_highPt, m_lowPt;
  int m_isElectronId, m_isPositronId;
  int m_isElectronIsol, m_isPositronIsol;
  int m_isElectronConvRej, m_isPositronConvRej;
  int m_isElectronIp, m_isPositronIp;
  float m_invMass;
  bool m_passedJetVeto;
  bool m_passedUncorrJetVeto;
  int m_nJets, m_nUncorrJets, m_nSoftMuons, m_nExtraLeptons;
  float m_btagJets;
  float m_met, m_projectedMet, m_deltaPhi, m_detaLeptons, m_metOverPtLL, m_deltaPhiLLJet, m_dymva;
  float m_maxPtElectron, m_minPtElectron, m_WWInvMass, m_ptll;
  float m_extraSlowLeptonPTMin;
  int m_nVtx;
  int m_processID;

  //! contains the preselection cuts
  Selection* _selection;

  //! counters for the efficiencies display, based on electron candidates
  Counters* globalCounter;

  //! true if the selection arrived to lepton ID and isolation
  bool m_finalLeptons;
  //! true if the selection arrived to jet veto
  bool m_jetVeto;
  //! true if the selection arrived to jet veto with uncorrected jets
  bool m_uncorrJetVeto;
  //! true if passed all the selections previous to deltaPhi
  bool m_preDeltaPhi;

  //! latinos
  bool m_step0, m_step1, m_step2, m_step3, m_step4, m_step5, m_step6, m_step7, m_step8, m_step9;  
  bool m_step10, m_step11, m_step12, m_step13, m_step14, m_step15, m_step16, m_step17;
  bool m_step18, m_step19, m_step20, m_step21, m_step22, m_step23, m_step24;
  bool m_step25, m_step26;

  //! this is to do an efficiency for each process in the sample 
  //! (if more than one is present)
  //! to turn on it, use SetProcessID(int processID) with processID=!-1
  std::map<int, Counters*> multiProcessCounter;

};

#endif
