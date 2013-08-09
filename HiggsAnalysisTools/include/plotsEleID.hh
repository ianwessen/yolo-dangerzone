#ifndef plotsEleID_h
#define plotsEleID_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>

#include "TFile.h"
#include "TTree.h"

class plotsEleID : public HiggsBase {
 public:
  plotsEleID(TTree *tree=0);
  virtual ~plotsEleID();
  void Loop();
  void setWeight(char* rootfile, double theInfEB[100], double theSupEB[100], double theWeightEB[100], double theInfEE[100], double theSupEE[100], double theWeightEE[100], int eneBin);
  double getWeight(double theInfEB[100], double theSupEB[100], double theWeightEB[100], double theInfEE[100], double theSupEE[100], double theWeightEE[100], int eneBin, double eneele, int etabin);

private:

  // private members
  bool _verbose;

  // sample
  char* category;

  // hardcoded
  bool toWeight;

  // output files
  TFile *tfilePdfs, *tfileIso, *tfileEff, *tfileEne;

  // histos
  // ECALsubdet: 0 = EB; 1 = EE
  // ptBin: 0 = < 15GeV; 1 = > 15 GeV 
  // class: 0 = non-showering; 1 = showering
  // fullclass: 0 = golden; 1 = bigbrem; 2 = narrow; 3 = showering+cracks

  // Hadrons: not splitted
  TH1F *dPhiCaloHad[2][2];
  TH1F *dPhiVtxHad[2][2];
  TH1F *dEtaHad[2][2];
  TH1F *EoPoutHad[2][2];
  TH1F *HoEHad[2][2];  
  TH1F *shapeFisherHad[2][2];  
  TH1F *sigmaEtaEtaHad[2][2];
  TH1F *sigmaEtaPhiHad[2][2];
  TH1F *sigmaPhiPhiHad[2][2];
  TH1F *s1s9Had[2][2];
  TH1F *s9s25Had[2][2];
  TH1F *LATHad[2][2];
  TH1F *etaLATHad[2][2];
  TH1F *phiLATHad[2][2];
  TH1F *a20Had[2][2];
  TH1F *a42Had[2][2];

  // Electrons: not splitted
  // histo[ecalsubdet][ptbin]
  TH1F *dPhiCaloUnsplitEle[2][2];
  TH1F *dPhiVtxUnsplitEle[2][2];
  TH1F *dEtaUnsplitEle[2][2];
  TH1F *EoPoutUnsplitEle[2][2];
  TH1F *HoEUnsplitEle[2][2];  
  TH1F *shapeFisherUnsplitEle[2][2];  
  TH1F *sigmaEtaEtaUnsplitEle[2][2];
  TH1F *sigmaEtaPhiUnsplitEle[2][2];
  TH1F *sigmaPhiPhiUnsplitEle[2][2];
  TH1F *s1s9UnsplitEle[2][2];
  TH1F *s9s25UnsplitEle[2][2];
  TH1F *LATUnsplitEle[2][2];
  TH1F *etaLATUnsplitEle[2][2];
  TH1F *phiLATUnsplitEle[2][2];
  TH1F *a20UnsplitEle[2][2];
  TH1F *a42UnsplitEle[2][2];
  TH1F *eneUnsplitEle[2];  // [ecalsubdet]
  
  // Electrons class-splitted
  // histo[ecalsubdet][ptbin][class]
  TH1F *dPhiCaloClassEle[2][2][2];
  TH1F *dPhiVtxClassEle[2][2][2];
  TH1F *dEtaClassEle[2][2][2];
  TH1F *EoPoutClassEle[2][2][2];
  TH1F *HoEClassEle[2][2][2];
  TH1F *shapeFisherClassEle[2][2][2];
  TH1F *sigmaEtaEtaClassEle[2][2][2];
  TH1F *sigmaEtaPhiClassEle[2][2][2];
  TH1F *sigmaPhiPhiClassEle[2][2][2];
  TH1F *s1s9ClassEle[2][2][2];
  TH1F *s9s25ClassEle[2][2][2];
  TH1F *LATClassEle[2][2][2];
  TH1F *etaLATClassEle[2][2][2];
  TH1F *phiLATClassEle[2][2][2];
  TH1F *a20ClassEle[2][2][2];
  TH1F *a42ClassEle[2][2][2];

  // Electrons fullclass-splitted
  // histo[ecalsubdet][ptbin][fullclass]
  TH1F *dPhiCaloFullclassEle[2][2][4];
  TH1F *dPhiVtxFullclassEle[2][2][4];
  TH1F *dEtaFullclassEle[2][2][4];
  TH1F *EoPoutFullclassEle[2][2][4];
  TH1F *HoEFullclassEle[2][2][4];
  TH1F *shapeFisherFullclassEle[2][2][4];
  TH1F *sigmaEtaEtaFullclassEle[2][2][4];
  TH1F *sigmaEtaPhiFullclassEle[2][2][4];
  TH1F *sigmaPhiPhiFullclassEle[2][2][4];
  TH1F *s1s9FullclassEle[2][2][4];
  TH1F *s9s25FullclassEle[2][2][4];
  TH1F *LATFullclassEle[2][2][4];
  TH1F *etaLATFullclassEle[2][2][4];
  TH1F *phiLATFullclassEle[2][2][4];
  TH1F *a20FullclassEle[2][2][4];
  TH1F *a42FullclassEle[2][2][4];

  TH1F *H_dRmin_tracker_withVeto,      *E_dRmin_tracker_withVeto;
  TH1F *H_dRmin_tracker_withVeto_zoom, *E_dRmin_tracker_withVeto_zoom;
  TH1F *H_dRmin_tracker_noVeto,        *E_dRmin_tracker_noVeto;
  TH1F *H_dRmin_tracker_noVeto_zoom,   *E_dRmin_tracker_noVeto_zoom;
  TH1F *H_ptFrac_tracker,              *E_ptFrac_tracker;
  TH1F *H_ptFrac_tracker_zoom,         *E_ptFrac_tracker_zoom;
  TH1F *H_dRmin_calo_noVeto,           *E_dRmin_calo_noVeto;
  TH1F *H_ptFrac_calo,                 *E_ptFrac_calo;

  TH1F *H_Reco_eta_wcm,    *E_Reco_eta_wcm;
  TH1F *H_Reco_eta_wgm,    *E_Reco_eta_wgm;
  TH1F *H_Reco_eta_wogm,   *E_Reco_eta_wogm;
  TH1F *H_Gene_eta,        *E_Gene_eta;

  // pt reweighting
  int   eneBin;  
  float eneMin,  eneMax;
  double theEleWeightEB[75], theEleWeightEE[75];           //  75 = bins number 
  double theEleInfEB[75],    theEleSupEB[75]; 
  double theEleInfEE[75],    theEleSupEE[75]; 
  double theZeeWeightEB[75], theZeeWeightEE[75];           //  75 = bins number 
  double theZeeInfEB[75],    theZeeSupEB[75]; 
  double theZeeInfEE[75],    theZeeSupEE[75]; 
  
  // counters
  int ntotEve;
  int ntot, ntotEB, ntotEE;
  int nGsfClass0[2], nGsfClass1[2], nGsfClass2[2], nGsfClass3[2];
  int trackIso, caloIso;

  // for StatPatterRecognition
  int nStatPatternRecognitionVars;

};
#endif
