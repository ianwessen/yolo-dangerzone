//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed Z -> e+e-
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef ZTandProbe_h
#define ZTandProbe_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>
#include <TH1F.h>

class ZTandProbe : public HiggsBase{
public:
  ZTandProbe(TTree *tree=0);
  virtual ~ZTandProbe();
  void Loop();
  void displayEfficiencies();
  void writeHistos();

private:

  bool findMcTree(const char* processType);
  bool isInCrack(float eta);
  bool isATag(int theTag);
  bool isAProbe(int theProbe, int theTag, float mll);
  bool isProbeEleID(int eleIndex);
  bool isTagEleID(int eleIndex);
  void setKinematics(int,int);  
  float Fisher(int eleIndex);
  void bookHistos();

  // private members
  Counters _counter;
  Counters _eleCounter;
  Selection* _selection;
  std::vector<Selection*> _electronSelection;
  bool _verbose;

  TVector3 *_p3Tag, *_p3Probe;
  TLorentzVector *_p4Tag, *_p4Probe;

  // histos
  TH1F *Gene_eta, *H_tagEta, *H_probeAllEta, *H_probeOkEta;
  TH1F *Htrigger_eta, *Hmc_eta, *Hiso_eta, *HeleID_eta, *Hpt_eta;
  TH1F *H_probeEoPoutOkEta, *H_probeHoEOkEta, *H_probedPhiOkEta, *H_probedEtaOkEta, *H_probeCovEtaEtaOkEta, *H_probeS9S25OkEta;
  TH1F *H_probeGoldenEta, *H_probeBigBremEta, *H_probeNarrowEta, *H_probeShoweringEta, *H_probeCracksEta;
  
  TH1F *Gene_pt, *H_tagPt, *H_probeAllPt, *H_probeOkPt;
  TH1F *Htrigger_pt, *Hmc_pt, *Hiso_pt, *HeleID_pt, *Hpt_pt;
  TH1F *H_probeEoPoutOkPt, *H_probeHoEOkPt, *H_probedPhiOkPt, *H_probedEtaOkPt, *H_probeCovEtaEtaOkPt, *H_probeS9S25OkPt;
  TH1F *H_probeGoldenPt, *H_probeBigBremPt, *H_probeNarrowPt, *H_probeShoweringPt, *H_probeCracksPt;

  TH1F *dPhiVtx0[2];
  TH1F *dEtaCalo0[2];
  TH1F *dEtaVtx0[2];
  TH1F *EoPout0[2];
  TH1F *HoE0[2];
  TH1F *shapeFisher0[2];
  
  TH1F *dPhiVtx1[2];
  TH1F *dEtaCalo1[2];
  TH1F *dEtaVtx1[2];
  TH1F *EoPout1[2];
  TH1F *HoE1[2];
  TH1F *shapeFisher1[2];

  TH1F *dPhiVtx2[2];
  TH1F *dEtaCalo2[2];
  TH1F *dEtaVtx2[2];
  TH1F *EoPout2[2];
  TH1F *HoE2[2];
  TH1F *shapeFisher2[2];

  TH1F *dPhiVtx3[2];
  TH1F *dEtaCalo3[2];
  TH1F *dEtaVtx3[2];
  TH1F *EoPout3[2];
  TH1F *HoE3[2];
  TH1F *shapeFisher3[2];

  // new variables
  float _mll[1];
};
#endif
