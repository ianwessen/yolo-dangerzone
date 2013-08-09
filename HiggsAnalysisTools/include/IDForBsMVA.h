//--------------------------------------------------------------------------------------------------
// $Id $
//
// IDForBsMVA
//
// Helper Class for applying MVA leptonselection
//
//--------------------------------------------------------------------------------------------------

#ifndef HIGGSANALYSIS_IDForBsMVA_H
#define HIGGSANALYSIS_IDForBsMVA_H

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace std;

class IDForBsMVA {
 public:
  IDForBsMVA(bool isEle);
  ~IDForBsMVA();
  
  void Initialize( std::string Subdet0PtLowWeights ,
		   std::string Subdet1PtLowWeights ,
		   std::string Subdet2PtLowWeights,
		   std::string Subdet0PtHighWeights,
		   std::string Subdet1PtHighWeights,
		   std::string Subdet2PtHighWeights,
		   bool isEle);

  Bool_t IsInitialized() const { return fIsInitialized; }
  
  double MVAValue(Double_t lepEta,
		  Double_t lepPt,
		  Double_t neuRelIso,
		  Double_t chRelIso,
		  Double_t jetDR_in,
		  Double_t jetPtRatio_in,
		  Double_t jetBTagCSV_in,
		  Double_t sip3d,
		  Double_t mvaId,
		  Int_t innerHits,
		  Double_t logdxy,
		  Double_t logdz,
		  Bool_t isEle );

  protected:
  TMVA::Reader *fTMVAReader[6];
    
  Bool_t fIsInitialized;

  Float_t fMVAVar_neuRelIso;
  Float_t fMVAVar_chRelIso;
  Float_t fMVAVar_jetDR_in;
  Float_t fMVAVar_jetPtRatio_in;
  Float_t fMVAVar_jetBTagCSV_in;
  Float_t fMVAVar_sip3d;
  Float_t fMVAVar_mvaId;
  //Int_t fMVAVar_innerHits;
  Float_t fMVAVar_innerHits;
  Float_t fMVAVar_logdxy;
  Float_t fMVAVar_logdz; 
};

#endif
