#include <TFile.h>
#include "HiggsAnalysisTools/include/IDForBsMVA.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include <math.h>
#include <assert.h>

IDForBsMVA::IDForBsMVA(bool isEle) :
  fIsInitialized(kFALSE)
{
  if (isEle) {
    for(UInt_t ie=0; ie<6; ++ie) fTMVAReader[ie] = 0;
  } else { 
    for(UInt_t im=0; im<4; ++im) fTMVAReader[im] = 0;
  }
}

IDForBsMVA::~IDForBsMVA() {

  for(UInt_t ie=0; ie<6; ++ie) {
    if (fTMVAReader[ie]) delete fTMVAReader[ie];
  }
}

void IDForBsMVA::Initialize( std::string Subdet0PtLowWeights , 
			     std::string Subdet1PtLowWeights , 
			     std::string Subdet2PtLowWeights,
			     std::string Subdet0PtHighWeights,
			     std::string Subdet1PtHighWeights, 
			     std::string Subdet2PtHighWeights,
			     bool isEle)  {

  fIsInitialized = kTRUE;
    
  int theNum = 6;              // 6 classes of weights for electrons 
  if (!isEle) theNum = 4;      // 4 classes of weights for muons 

  for(UInt_t i=0; i<theNum; ++i) {
    if (fTMVAReader[i]) delete fTMVAReader[i];

    fTMVAReader[i] = new TMVA::Reader( "!Color:!Silent:Error" );  
    fTMVAReader[i]->SetVerbose(kTRUE);

    // common variables
    /*
    fTMVAReader[i]->AddVariable( "neuRelIso",      &fMVAVar_neuRelIso );
    fTMVAReader[i]->AddVariable( "chRelIso",       &fMVAVar_chRelIso );
    fTMVAReader[i]->AddVariable( "jetDR_in",       &fMVAVar_jetDR_in );
    fTMVAReader[i]->AddVariable( "jetPtRatio_in",  &fMVAVar_jetPtRatio_in );
    fTMVAReader[i]->AddVariable( "jetBTagCSV_in",  &fMVAVar_jetBTagCSV_in );
    */

    fTMVAReader[i]->AddVariable( "relIso-chargedIso/pt",      &fMVAVar_neuRelIso );
    fTMVAReader[i]->AddVariable( "chargedIso/pt"       ,      &fMVAVar_chRelIso );
    fTMVAReader[i]->AddVariable( "min(dr_in,0.5)"      ,      &fMVAVar_jetDR_in );
    fTMVAReader[i]->AddVariable( "min(ptf_in,1.5)"     ,      &fMVAVar_jetPtRatio_in );
    fTMVAReader[i]->AddVariable( "max(CSV_in,0)"       ,      &fMVAVar_jetBTagCSV_in );
    fTMVAReader[i]->AddVariable( "sip3d"               ,      &fMVAVar_sip3d );

 
    // variables only for electrons
    if (isEle) { 
      fTMVAReader[i]->AddVariable( "mvaId",      &fMVAVar_mvaId );
      fTMVAReader[i]->AddVariable( "innerHits",  &(fMVAVar_innerHits) );
    }

    // variables only for muons
    if (!isEle) { 
      /*
      fTMVAReader[i]->AddVariable( "logdxy",  &fMVAVar_logdxy );
      fTMVAReader[i]->AddVariable( "logdz",   &fMVAVar_logdz );
      */
      fTMVAReader[i]->AddVariable( "log(abs(dxy))",  &fMVAVar_logdxy );
      fTMVAReader[i]->AddVariable( "log(abs(dz))" ,  &fMVAVar_logdz );
    }

    if (isEle) {
      if (i==0) fTMVAReader[i]->BookMVA("BDTG method" , Subdet0PtLowWeights ); 
      if (i==1) fTMVAReader[i]->BookMVA("BDTG method" , Subdet1PtLowWeights );
      if (i==2) fTMVAReader[i]->BookMVA("BDTG method" , Subdet2PtLowWeights );
      if (i==3) fTMVAReader[i]->BookMVA("BDTG method" , Subdet0PtHighWeights );
      if (i==4) fTMVAReader[i]->BookMVA("BDTG method" , Subdet1PtHighWeights );
      if (i==5) fTMVAReader[i]->BookMVA("BDTG method" , Subdet2PtHighWeights );
    } else {
      if (i==0) fTMVAReader[i]->BookMVA("BDTG method" , Subdet0PtLowWeights );
      if (i==1) fTMVAReader[i]->BookMVA("BDTG method" , Subdet1PtLowWeights );
      if (i==2) fTMVAReader[i]->BookMVA("BDTG method" , Subdet0PtHighWeights );
      if (i==3) fTMVAReader[i]->BookMVA("BDTG method" , Subdet1PtHighWeights );
    }
  }

  if (isEle) {
    std::cout << "Electron ID MVA for Bs Initialization\n";
    std::cout << "Load weights file : " << Subdet0PtLowWeights  << std::endl;
    std::cout << "Load weights file : " << Subdet1PtLowWeights  << std::endl;
    std::cout << "Load weights file : " << Subdet2PtLowWeights  << std::endl;
    std::cout << "Load weights file : " << Subdet0PtHighWeights << std::endl;
    std::cout << "Load weights file : " << Subdet1PtHighWeights << std::endl;
    std::cout << "Load weights file : " << Subdet2PtHighWeights << std::endl;
  } else {
    std::cout << "Muon ID MVA for Bs Initialization\n";
    std::cout << "Load weights file : " << Subdet0PtLowWeights  << std::endl;
    std::cout << "Load weights file : " << Subdet1PtLowWeights  << std::endl;
    std::cout << "Load weights file : " << Subdet0PtHighWeights << std::endl;
    std::cout << "Load weights file : " << Subdet1PtHighWeights << std::endl;
  }
}

Double_t IDForBsMVA::MVAValue(Double_t lepEta,
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
			      Bool_t isEle
			      ) {
  
  if (!fIsInitialized) { 
    std::cout << "Error: IDForBsMVA not properly initialized.\n"; 
    return -9999;
  }
  
  Int_t subdet = 999;
  if (isEle) {
    if (fabs(lepEta) < 0.8) subdet = 0;
    else if (fabs(lepEta) < 1.479) subdet = 1;
    else subdet = 2;
  } else {
    if (fabs(lepEta) < 1.5) subdet = 0;
    else subdet = 1;
  }
  if (isEle && subdet>2)  cout << "serious problem! wrong eta range" << endl; 
  if (!isEle && subdet>1) cout << "serious problem! wrong eta range" << endl; 

  Int_t ptBin = 999;
  if (isEle) {
    if (lepPt<=10) ptBin = 0;
    else ptBin = 1;
  } else {
    if (lepPt<=15) ptBin = 0;
    else ptBin = 1;
  }
  if (ptBin>1)  cout << "serious problem! wrong pT range" << endl; 
  
  //set all input variables
  fMVAVar_neuRelIso      = neuRelIso;
  fMVAVar_chRelIso       = chRelIso;
  fMVAVar_jetDR_in       = jetDR_in;
  fMVAVar_jetPtRatio_in  = jetPtRatio_in;
  fMVAVar_jetBTagCSV_in  = jetBTagCSV_in;
  fMVAVar_sip3d          = sip3d;
  if (isEle) { 
    fMVAVar_mvaId     = mvaId;
    fMVAVar_innerHits = innerHits;
  } else { 
    fMVAVar_logdxy = logdxy;
    fMVAVar_logdz  = logdz;
  }

  Double_t mva = -9999;  
  TMVA::Reader *reader = 0;
  Int_t MVABin = -1;
  if (isEle) {
    if (subdet == 0 && ptBin == 0) MVABin = 0;
    if (subdet == 1 && ptBin == 0) MVABin = 1;
    if (subdet == 2 && ptBin == 0) MVABin = 2;
    if (subdet == 0 && ptBin == 1) MVABin = 3;
    if (subdet == 1 && ptBin == 1) MVABin = 4;
    if (subdet == 2 && ptBin == 1) MVABin = 5;
    assert(MVABin >= 0 && MVABin <= 5);
  } else {
    if (subdet == 0 && ptBin == 0) MVABin = 0;
    if (subdet == 1 && ptBin == 0) MVABin = 1;
    if (subdet == 0 && ptBin == 1) MVABin = 2;
    if (subdet == 1 && ptBin == 1) MVABin = 3;
    assert(MVABin >= 0 && MVABin <= 3);
  }

  reader = fTMVAReader[MVABin];
                                                
  mva = reader->EvaluateMVA("BDTG method");

  return mva;
}
