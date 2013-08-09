#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/LeptonPlusFakeMLSelection_fullME.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "CommonTools/include/LeptonIdBits.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <TTree.h>

using namespace bits;

LeptonPlusFakeMLSelection_fullME::LeptonPlusFakeMLSelection_fullME(TTree *tree) 
  : Higgs(tree) {
  
  // choose the Higgs Mass
  std::string higgsConfigDir;
  std::string higgsConfigDirMass;
  std::ifstream setfile("config/higgs/higgsMass.txt");
  higgsConfigDir="config/higgs/";
  if(!setfile.good()) {
    std::cout << "Cannot read the higgsMass file to choose the selection: config/higgs/higgsMass.txt" << std::endl
	      << "using default (160 GeV)" << std::endl;
    higgsConfigDirMass="config/higgs/h160/";
  }
  else {
    std::string var, massVal; 
    bool found=false;
    while (1) {
      setfile >> var >> massVal;
      _massVal = atoi(massVal.c_str());
      if(!setfile.good()) break;
      if(var.compare("HiggsMass")==0) { 
	found=true;
	higgsConfigDirMass="config/higgs/h" + massVal + "/";
	std::cout << "Reading configuration for Higgs mass = " << massVal << " GeV/c^2" << std::endl;
	break;
      }
    }
  }
  
  // selection efficiencies: counters
  std::string fileCutsME     = higgsConfigDirMass + "emu2nuCuts.txt";
  std::string fileSwitchesME = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionME.Configure(fileCutsME.c_str(),fileSwitchesME.c_str(),"FULL SELECTION EVENT COUNTER ME FP"); 
  _selectionME = CutBasedHiggsSelectionME.GetSelection();

  //  extra selection efficiencies  - to be put here not to pass the full list of leptons to the preselection class
  _selectionME->addCut("etaElectronAcc");    
  _selectionME->addCut("ptElectronAcc");
  _selectionME->addCut("etaMuonAcc");
  _selectionME->addCut("ptMuonAcc");
  _selectionME->addCut("etUncorrJetAcc");  
  _selectionME->addSwitch("apply_kFactor");   
  _selectionME->addSwitch("isData");
  _selectionME->addSwitch("goodRunLS");
  _selectionME->addSwitch("asymmetricID");
  _selectionME->addSwitch("applyBDTEleID");
  _selectionME->addStringParameter("electronIDType");
  _selectionME->addStringParameter("electronIDTypeLow");  

  // cut based electron id or likelihood 
  TString selectionString(_selectionME->getStringParameter("electronIDType"));
  if (!_selectionME->getSwitch("asymmetricID")) 
    cout << "=== CONFIGURING " << selectionString << " CUT BASED SYMMETRIC ELECTRON ID ===" << endl;
  EgammaCutBasedID.ConfigureNoClass("config/higgs/electronId/"+selectionString);
  EgammaCutBasedID.ConfigureEcalCleaner("config/higgs/electronId/");
  
  if (_selectionME->getSwitch("asymmetricID")) {
    TString selectionStringLow (_selectionME->getStringParameter("electronIDTypeLow"));
    cout << "=== CONFIGURING "  << selectionStringLow << " and " 
	 << selectionString << " for CUT BASED ASYMMETRIC ELECTRON ID ===" << endl;
    EgammaCutBasedIDLow.ConfigureNoClass("config/higgs/electronId/"+selectionStringLow);
    EgammaCutBasedIDLow.ConfigureEcalCleaner("config/higgs/electronId/");
  }
  
  // configuring electron likelihood
  TFile *fileLH = TFile::Open("pdfs_MC.root");
  TDirectory *EB0lt15dir = fileLH->GetDirectory("/");
  TDirectory *EB1lt15dir = fileLH->GetDirectory("/");
  TDirectory *EElt15dir  = fileLH->GetDirectory("/");
  TDirectory *EB0gt15dir = fileLH->GetDirectory("/");
  TDirectory *EB1gt15dir = fileLH->GetDirectory("/");
  TDirectory *EEgt15dir  = fileLH->GetDirectory("/");
  LikelihoodSwitches defaultSwitches;
  defaultSwitches.m_useFBrem = true;
  defaultSwitches.m_useEoverP = false;
  defaultSwitches.m_useSigmaPhiPhi = true;
  defaultSwitches.m_useHoverE = false;        
  defaultSwitches.m_useOneOverEMinusOneOverP = true;
  LH = new ElectronLikelihood(&(*EB0lt15dir), &(*EB1lt15dir), &(*EElt15dir), &(*EB0gt15dir), &(*EB1gt15dir), &(*EEgt15dir), defaultSwitches, std::string("class"),std::string("class"),true,true);
  
  // configuring electron ID BDT 
  if(_selectionME->getSwitch("applyBDTEleID")) m_useBDTEleID = true;
  else m_useBDTEleID = false;

  fMVA = new ElectronIDMVA();
  fMVA->Initialize("BDTG method",
                   "elebdtweights/Subdet0LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet1LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet2LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet0HighPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet1HighPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet2HighPt_WithIPInfo_BDTG.weights.xml" ,
                   ElectronIDMVA::kWithIPInfo);

  // Reading GoodRUN LS
  std::cout << "[GoodRunLS]::goodRunLS is " << _selectionME->getSwitch("goodRunLS") << " isData is " <<  _selectionME->getSwitch("isData") << std::endl;

  // To read good run list!    // chiara
  if (_selectionME->getSwitch("goodRunLS") && _selectionME->getSwitch("isData")) {
    // std::string goodRunJsonFile = "config/json/2011a.json";
    // std::string goodRunJsonFile = "config/json/2011b.json";
    std::string goodRunJsonFile = "config/json/hww.Full2011.json";
    setJsonGoodRunList(goodRunJsonFile);
    fillRunLSMap();
  }

  // kinematics
  for(int theChannel=0; theChannel<1; theChannel++) {
    m_p4LeptonPlus[theChannel]  = new TLorentzVector(0.,0.,0.,0.);
    m_p4LeptonMinus[theChannel] = new TLorentzVector(0.,0.,0.,0.);
    m_jetsSum[theChannel]       = new TLorentzVector(0.,0.,0.,0.);
    m_uncorrJetsSum[theChannel] = new TLorentzVector(0.,0.,0.,0.);
  }

  // met
  m_p3PFMET = new TVector3(0.,0.,0.);

  // b-veto event variables
  m_maxDxyEvt = 0.0;
  m_maxDszEvt = 0.0;
}

LeptonPlusFakeMLSelection_fullME::~LeptonPlusFakeMLSelection_fullME(){

  for(int theChannel=0; theChannel<1; theChannel++) {  
    delete m_p4LeptonPlus[theChannel];
    delete m_p4LeptonMinus[theChannel];
  }
  
  delete _selectionME;

  myOutTreeME -> save();
}

void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeBinning() {

  m_eleMinFakePt[0] = 10.;   m_eleMaxFakePt[0] = 15.;
  m_eleMinFakePt[1] = 15.;   m_eleMaxFakePt[1] = 20.;
  m_eleMinFakePt[2] = 20.;   m_eleMaxFakePt[2] = 25.;
  m_eleMinFakePt[3] = 25.;   m_eleMaxFakePt[3] = 30.;
  m_eleMinFakePt[4] = 30.;   m_eleMaxFakePt[4] = 10000.;
}

void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeEtaBinning() {

  m_eleMinFakeEta[0] = 0.;    m_eleMaxFakeEta[0] = 1.;
  m_eleMinFakeEta[1] = 1.;    m_eleMaxFakeEta[1] = 1.479;
  m_eleMinFakeEta[2] = 1.479; m_eleMaxFakeEta[2] = 2.;
  m_eleMinFakeEta[3] = 2.;    m_eleMaxFakeEta[3] = 2.5;
}

// fake for eleID Bdt, ET>15 - 2011A+2011B average analysis                               
void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeRate15() {                                                                                                                                                                       
  m15_eleFakeRateEB1[0] = 0.109; 
  m15_eleFakeRateEB1[1] = 0.103; 
  m15_eleFakeRateEB1[2] = 0.123; 
  m15_eleFakeRateEB1[3] = 0.099;
  m15_eleFakeRateEB1[4] = 0.102;                                                                                                                                         
  m15_eleFakeRateEB1_err[0] = 0.007; 
  m15_eleFakeRateEB1_err[1] = 0.008;
  m15_eleFakeRateEB1_err[2] = 0.010; 
  m15_eleFakeRateEB1_err[3] = 0.013;
  m15_eleFakeRateEB1_err[4] = 0.018;                                                                                                                                     
  m15_eleFakeRateEB2[0] = 0.056; 
  m15_eleFakeRateEB2[1] = 0.074; 
  m15_eleFakeRateEB2[2] = 0.093; 
  m15_eleFakeRateEB2[3] = 0.070; 
  m15_eleFakeRateEB2[4] = 0.058;                                                                                                                                         
  m15_eleFakeRateEB2_err[0] = 0.006; 
  m15_eleFakeRateEB2_err[1] = 0.009; 
  m15_eleFakeRateEB2_err[2] = 0.012; 
  m15_eleFakeRateEB2_err[3] = 0.015; 
  m15_eleFakeRateEB2_err[4] = 0.019;     

  m15_eleFakeRateEE1[0] = 0.028; 
  m15_eleFakeRateEE1[1] = 0.028; 
  m15_eleFakeRateEE1[2] = 0.085; 
  m15_eleFakeRateEE1[3] = 0.059; 
  m15_eleFakeRateEE1[4] = 0.071;                                                                                                                                         
  m15_eleFakeRateEE1_err[0] = 0.005; 
  m15_eleFakeRateEE1_err[1] = 0.007; 
  m15_eleFakeRateEE1_err[2] = 0.011; 
  m15_eleFakeRateEE1_err[3] = 0.013; 
  m15_eleFakeRateEE1_err[4] = 0.019;                                                                                                                                     
  m15_eleFakeRateEE2[0] = 0.035; 
  m15_eleFakeRateEE2[1] = 0.048; 
  m15_eleFakeRateEE2[2] = 0.069; 
  m15_eleFakeRateEE2[3] = 0.080; 
  m15_eleFakeRateEE2[4] = 0.073;                

  m15_eleFakeRateEE2_err[0] = 0.006; 
  m15_eleFakeRateEE2_err[1] = 0.009; 
  m15_eleFakeRateEE2_err[2] = 0.010; 
  m15_eleFakeRateEE2_err[3] = 0.014; 
  m15_eleFakeRateEE2_err[4] = 0.019; 
}                                                                                                                                                                     
// fake for eleID Bdt, ET>35 - 2011A+2011B average analysis 
void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeRate35() {                                                                                                       
  m35_eleFakeRateEB1[0] = 0.066; 
  m35_eleFakeRateEB1[1] = 0.057; 
  m35_eleFakeRateEB1[2] = 0.085; 
  m35_eleFakeRateEB1[3] = 0.085; 
  m35_eleFakeRateEB1[4] = 0.085;                                                                                                                                         
  m35_eleFakeRateEB1_err[0] = 0.015; 
  m35_eleFakeRateEB1_err[1] = 0.009; 
  m35_eleFakeRateEB1_err[2] = 0.009; 
  m35_eleFakeRateEB1_err[3] = 0.012; 
  m35_eleFakeRateEB1_err[4] = 0.017;

  m35_eleFakeRateEB2[0] = 0.040; 
  m35_eleFakeRateEB2[1] = 0.054; 
  m35_eleFakeRateEB2[2] = 0.064; 
  m35_eleFakeRateEB2[3] = 0.067; 
  m35_eleFakeRateEB2[4] = 0.059;                                                                                                                                         
  m35_eleFakeRateEB2_err[0] = 0.012; 
  m35_eleFakeRateEB2_err[1] = 0.010; 
  m35_eleFakeRateEB2_err[2] = 0.011; 
  m35_eleFakeRateEB2_err[3] = 0.015; 
  m35_eleFakeRateEB2_err[4] = 0.019;

  m35_eleFakeRateEE1[0] = 0.016;     
  m35_eleFakeRateEE1[1] = 0.018; 
  m35_eleFakeRateEE1[2] = 0.057; 
  m35_eleFakeRateEE1[3] = 0.052; 
  m35_eleFakeRateEE1[4] = 0.072;                                                                                                                                         
  m35_eleFakeRateEE1_err[0] = 0.009; 
  m35_eleFakeRateEE1_err[1] = 0.007; 
  m35_eleFakeRateEE1_err[2] = 0.010; 
  m35_eleFakeRateEE1_err[3] = 0.012; 
  m35_eleFakeRateEE1_err[4] = 0.019;

  m35_eleFakeRateEE2[0] = 0.023;    
  m35_eleFakeRateEE2[1] = 0.041; 
  m35_eleFakeRateEE2[2] = 0.046; 
  m35_eleFakeRateEE2[3] = 0.070; 
  m35_eleFakeRateEE2[4] = 0.068;                                                                                                                                         
  m35_eleFakeRateEE2_err[0] = 0.013; 
  m35_eleFakeRateEE2_err[1] = 0.013; 
  m35_eleFakeRateEE2_err[2] = 0.009; 
  m35_eleFakeRateEE2_err[3] = 0.015; 
  m35_eleFakeRateEE2_err[4] = 0.019; 
}          

// fake for eleID Bdt, ET>50 - 2011A+2011B average analysis 
void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeRate50() {                                                                                                       
  m50_eleFakeRateEB1[0] = 0.063; 
  m50_eleFakeRateEB1[1] = 0.026; 
  m50_eleFakeRateEB1[2] = 0.061; 
  m50_eleFakeRateEB1[3] = 0.052; 
  m50_eleFakeRateEB1[4] = 0.031;                                                                                                                                         
  m50_eleFakeRateEB1_err[0] = 0.036; 
  m50_eleFakeRateEB1_err[1] = 0.012; 
  m50_eleFakeRateEB1_err[2] = 0.015; 
  m50_eleFakeRateEB1_err[3] = 0.014; 
  m50_eleFakeRateEB1_err[4] = 0.012;                                                                                                                                     
  m50_eleFakeRateEB2[0] = 0.042; 
  m50_eleFakeRateEB2[1] = 0.034; 
  m50_eleFakeRateEB2[2] = 0.060; 
  m50_eleFakeRateEB2[3] = 0.024; 
  m50_eleFakeRateEB2[4] = 0.025;  

  m50_eleFakeRateEB2_err[0] = 0.029; 
  m50_eleFakeRateEB2_err[1] = 0.016; 
  m50_eleFakeRateEB2_err[2] = 0.018; 
  m50_eleFakeRateEB2_err[3] = 0.012; 
  m50_eleFakeRateEB2_err[4] = 0.015;                                                                                                                                     
  m50_eleFakeRateEE1[0] = 0.008; 
  m50_eleFakeRateEE1[1] = 0.008; 
  m50_eleFakeRateEE1[2] = 0.054; 
  m50_eleFakeRateEE1[3] = 0.033; 
  m50_eleFakeRateEE1[4] = 0.044;                                                                                                                                         
  m50_eleFakeRateEE1_err[0] = 0.010; 
  m50_eleFakeRateEE1_err[1] = 0.010; 
  m50_eleFakeRateEE1_err[2] = 0.018; 
  m50_eleFakeRateEE1_err[3] = 0.015; 
  m50_eleFakeRateEE1_err[4] = 0.017;     

  m50_eleFakeRateEE2[0] = 0.034; 
  m50_eleFakeRateEE2[1] = 0.041; 
  m50_eleFakeRateEE2[2] = 0.051; 
  m50_eleFakeRateEE2[3] = 0.044; 
  m50_eleFakeRateEE2[4] = 0.039;                                                                                                                                         
  m50_eleFakeRateEE2_err[0] = 0.034; 
  m50_eleFakeRateEE2_err[1] = 0.024; 
  m50_eleFakeRateEE2_err[2] = 0.018; 
  m50_eleFakeRateEE2_err[3] = 0.016; 
  m50_eleFakeRateEE2_err[4] = 0.017; 
}

// fake for eleID Bdt, QCD for closure test
void LeptonPlusFakeMLSelection_fullME::initialiseElectronFakeRateQCD() {

  mQCD_eleFakeRateEB1[0] = 0.0082;
  mQCD_eleFakeRateEB1[1] = 0.0040;
  mQCD_eleFakeRateEB1[2] = 0.0049;
  mQCD_eleFakeRateEB1[3] = 0.0048;
  mQCD_eleFakeRateEB1[4] = 0.0051;

  mQCD_eleFakeRateEB1_err[0] = 0.0002;
  mQCD_eleFakeRateEB1_err[1] = 0.0002;
  mQCD_eleFakeRateEB1_err[2] = 0.0002;
  mQCD_eleFakeRateEB1_err[3] = 0.0003;
  mQCD_eleFakeRateEB1_err[4] = 0.0004;

  mQCD_eleFakeRateEB2[0] = 0.0061; 
  mQCD_eleFakeRateEB2[1] = 0.0037; 
  mQCD_eleFakeRateEB2[2] = 0.0065; 
  mQCD_eleFakeRateEB2[3] = 0.0061; 
  mQCD_eleFakeRateEB2[4] = 0.0064; 

  mQCD_eleFakeRateEB2_err[0] = 0.0002;
  mQCD_eleFakeRateEB2_err[1] = 0.0002;
  mQCD_eleFakeRateEB2_err[2] = 0.0004;
  mQCD_eleFakeRateEB2_err[3] = 0.0005;
  mQCD_eleFakeRateEB2_err[4] = 0.0008;

  mQCD_eleFakeRateEE1[0] = 0.0030;
  mQCD_eleFakeRateEE1[1] = 0.0017;
  mQCD_eleFakeRateEE1[2] = 0.0032;
  mQCD_eleFakeRateEE1[3] = 0.0032; 
  mQCD_eleFakeRateEE1[4] = 0.0031;
  
  mQCD_eleFakeRateEE1_err[0] = 0.0001;
  mQCD_eleFakeRateEE1_err[1] = 0.0001;
  mQCD_eleFakeRateEE1_err[2] = 0.0002;
  mQCD_eleFakeRateEE1_err[3] = 0.0003;
  mQCD_eleFakeRateEE1_err[4] = 0.0005;

  mQCD_eleFakeRateEE2[0] = 0.0018; 
  mQCD_eleFakeRateEE2[1] = 0.0009;
  mQCD_eleFakeRateEE2[2] = 0.0022;
  mQCD_eleFakeRateEE2[3] = 0.0023;
  mQCD_eleFakeRateEE2[4] = 0.0026;
  
  mQCD_eleFakeRateEE2_err[0] = 0.0001;
  mQCD_eleFakeRateEE2_err[1] = 0.0001;
  mQCD_eleFakeRateEE2_err[2] = 0.0002;
  mQCD_eleFakeRateEE2_err[3] = 0.0002;
  mQCD_eleFakeRateEE2_err[4] = 0.0004;
}

// prompt rate from eleID Bdt - for 2011A+B average analysis, split in #vtx (Clara, Nov 8)
void LeptonPlusFakeMLSelection_fullME::initialiseElectronPromptRate(int numVtx) {
  
  // binning                                      
  m_eleMinPromptPt[0] = 10.;   m_eleMaxPromptPt[0] = 15.;
  m_eleMinPromptPt[1] = 15.;   m_eleMaxPromptPt[1] = 20.;
  m_eleMinPromptPt[2] = 20.;   m_eleMaxPromptPt[2] = 25.;
  m_eleMinPromptPt[3] = 25.;   m_eleMaxPromptPt[3] = 50.;
  m_eleMinPromptPt[4] = 50.;   m_eleMaxPromptPt[4] = 10000.;

  // prompt in the barrel
  if (numVtx>=1 && numVtx<10) { 

    m_elePromptRateEB[0] = 0.589; 
    m_elePromptRateEB[1] = 0.673; 
    m_elePromptRateEB[2] = 0.778; 
    m_elePromptRateEB[3] = 0.874; 
    m_elePromptRateEB[4] = 0.906;                                                                                                                                        
    m_elePromptRateEB_err[0] = 0.0099; 
    m_elePromptRateEB_err[1] = 0.006; 
    m_elePromptRateEB_err[2] = 0.003; 
    m_elePromptRateEB_err[3] = 0.063; 
    m_elePromptRateEB_err[4] = 0.001; 
  } else {                                                                                                                                                            
    m_elePromptRateEB[0] = 0.528; 
    m_elePromptRateEB[1] = 0.616; 
    m_elePromptRateEB[2] = 0.739; 
    m_elePromptRateEB[3] = 0.862; 
    m_elePromptRateEB[4] = 0.901;                                                                                                                                        
    m_elePromptRateEB_err[0] = 0.046; 
    m_elePromptRateEB_err[1] = 0.014; 
    m_elePromptRateEB_err[2] = 0.008; 
    m_elePromptRateEB_err[3] = 0.001; 
    m_elePromptRateEB_err[4] = 0.003; 
  }                

  // prompt in the crack
  if (numVtx>=1 && numVtx<10) {                                                                                                                                       
    m_elePromptRateCr[0] = 0.465; 
    m_elePromptRateCr[1] = 0.540; 
    m_elePromptRateCr[2] = 0.712; 
    m_elePromptRateCr[3] = 0.823; 
    m_elePromptRateCr[4] = 0.867; 
    
    m_elePromptRateCr_err[0] = 0.043; 
    m_elePromptRateCr_err[1] = 0.043; 
    m_elePromptRateCr_err[2] = 0.017; 
    m_elePromptRateCr_err[3] = 0.090; 
    m_elePromptRateCr_err[4] = 0.070;                                                                                                                                    
  } else {                                                                                                                                                            
    m_elePromptRateCr[0] = 0.271; 
    m_elePromptRateCr[1] = 0.479; 
    m_elePromptRateCr[2] = 0.658; 
    m_elePromptRateCr[3] = 0.790; 
    m_elePromptRateCr[4] = 0.863;                                                                                                                                        
    m_elePromptRateCr_err[0] = 0.081; 
    m_elePromptRateCr_err[1] = 0.088; 
    m_elePromptRateCr_err[2] = 0.045; 
    m_elePromptRateCr_err[3] = 0.001; 
    m_elePromptRateCr_err[4] = 0.018; 
  }             

  // prompt in the endcap                 
  if (numVtx>=1 && numVtx<10) {                                                                                                                                       
    m_elePromptRateEE[0] = 0.371;
    m_elePromptRateEE[1] = 0.449; 
    m_elePromptRateEE[2] = 0.675; 
    m_elePromptRateEE[3] = 0.779; 
    m_elePromptRateEE[4] = 0.838;                                                                                                                                        
    m_elePromptRateEE_err[0] = 0.192; 
    m_elePromptRateEE_err[1] = 0.009; 
    m_elePromptRateEE_err[2] = 0.006; 
    m_elePromptRateEE_err[3] = 0.002; 
    m_elePromptRateEE_err[4] = 0.002;                                                                                                                                    
  } else {                                                                                                                                                            
    m_elePromptRateEE[0] = 0.305; 
    m_elePromptRateEE[1] = 0.350; 
    m_elePromptRateEE[2] = 0.588; 
    m_elePromptRateEE[3] = 0.722; 
    m_elePromptRateEE[4] = 0.793;                                                                                                                                        
    m_elePromptRateEE_err[0] = 0.042; 
    m_elePromptRateEE_err[1] = 0.022; 
    m_elePromptRateEE_err[2] = 0.015; 
    m_elePromptRateEE_err[3] = 0.002; 
    m_elePromptRateEE_err[4] = 0.009; 
  }                                       
}

// fake rate from smurfs denominator - for 2011A+B average analysis  (alicia, Nov 8th)
void LeptonPlusFakeMLSelection_fullME::initialiseMuonFakeRate() {

  // pT binning                                      
  m_muonMinFakePt[0] = 10.;   m_muonMaxFakePt[0] = 15.;                
  m_muonMinFakePt[1] = 15.;   m_muonMaxFakePt[1] = 20.;
  m_muonMinFakePt[2] = 20.;   m_muonMaxFakePt[2] = 25.;
  m_muonMinFakePt[3] = 25.;   m_muonMaxFakePt[3] = 30.;
  m_muonMinFakePt[4] = 30.;   m_muonMaxFakePt[4] = 10000.;

  // eta binning
  m_muonMinFakeEta[0] = 0.;    m_muonMaxFakeEta[0] = 1.;
  m_muonMinFakeEta[1] = 1.;    m_muonMaxFakeEta[1] = 1.479;
  m_muonMinFakeEta[2] = 1.479; m_muonMaxFakeEta[2] = 2.;
  m_muonMinFakeEta[3] = 2.;    m_muonMaxFakeEta[3] = 2.5;

  // fake rate barrel
  m_muonFakeRateEB1[0] = 0.196995;        
  m_muonFakeRateEB1[1] = 0.128234;        
  m_muonFakeRateEB1[2] = 0.238875;        
  m_muonFakeRateEB1[3] = 0.196995;        
  m_muonFakeRateEB1[4] = 0.17903; 
  
  m_muonFakeRateEB1_err[0] = 0.00288387;
  m_muonFakeRateEB1_err[1] = 0.00637965;
  m_muonFakeRateEB1_err[2] = 0.0033742;
  m_muonFakeRateEB1_err[3] = 0.00651601;
  m_muonFakeRateEB1_err[4] = 0.00529552;

  m_muonFakeRateEB2[0] = 0.22329; 
  m_muonFakeRateEB2[1] = 0.156986;        
  m_muonFakeRateEB2[2] = 0.283973;        
  m_muonFakeRateEB2[3] = 0.260501;        
  m_muonFakeRateEB2[4] = 0.241332;       
  
  m_muonFakeRateEB2_err[0] = 0.00462381;
  m_muonFakeRateEB2_err[1] = 0.0104939;
  m_muonFakeRateEB2_err[2] = 0.00520542;
  m_muonFakeRateEB2_err[3] = 0.0100281;
  m_muonFakeRateEB2_err[4] = 0.00743089;
  
  // fake rate endcap
  m_muonFakeRateEE1[0] = 0.251618;        
  m_muonFakeRateEE1[1] = 0.164606;        
  m_muonFakeRateEE1[2] = 0.249847;        
  m_muonFakeRateEE1[3] = 0.25045; 
  m_muonFakeRateEE1[4] = 0.257156;        
  
  m_muonFakeRateEE1_err[0] = 0.005566;
  m_muonFakeRateEE1_err[1] = 0.0121525;
  m_muonFakeRateEE1_err[2] = 0.00578382;
  m_muonFakeRateEE1_err[3] = 0.0109878;
  m_muonFakeRateEE1_err[4] = 0.00807854;

  m_muonFakeRateEE2[0] = 0.290008;        
  m_muonFakeRateEE2[1] = 0.20282; 
  m_muonFakeRateEE2[2] = 0.281293;        
  m_muonFakeRateEE2[3] = 0.283557;        
  m_muonFakeRateEE2[4] = 0.328231;        
  
  m_muonFakeRateEE2_err[0] = 0.0088987;
  m_muonFakeRateEE2_err[1] = 0.020893;
  m_muonFakeRateEE2_err[2] = 0.010023;
  m_muonFakeRateEE2_err[3] = 0.0190325;
  m_muonFakeRateEE2_err[4] = 0.0158864;
}

// prompt rate from smurfs denominator - for 2011A+B average analysis  (Clara, Nov 9th)
void LeptonPlusFakeMLSelection_fullME::initialiseMuonPromptRate(int numVtx) {

  // binning                                      
  m_muonMinPromptPt[0] = 10.;   m_muonMaxPromptPt[0] = 15.;               
  m_muonMinPromptPt[1] = 15.;   m_muonMaxPromptPt[1] = 20.;
  m_muonMinPromptPt[2] = 20.;   m_muonMaxPromptPt[2] = 25.;
  m_muonMinPromptPt[3] = 25.;   m_muonMaxPromptPt[3] = 50.;
  m_muonMinPromptPt[4] = 50.;   m_muonMaxPromptPt[4] = 10000.;

  // prompt rate barrel
  if (numVtx>=1 && numVtx<10) { 
    m_muonPromptRateEB[0] = 0.723;
    m_muonPromptRateEB[1] = 0.777;
    m_muonPromptRateEB[2] = 0.948;
    m_muonPromptRateEB[3] = 0.984;
    m_muonPromptRateEB[4] = 0.995;
    
    m_muonPromptRateEB_err[0] = 0.006;
    m_muonPromptRateEB_err[1] = 0.003;
    m_muonPromptRateEB_err[2] = 0.002;
    m_muonPromptRateEB_err[3] = 0.001;
    m_muonPromptRateEB_err[4] = 0.001;
  } else { 
    m_muonPromptRateEB[0] = 0.667;
    m_muonPromptRateEB[1] = 0.715;
    m_muonPromptRateEB[2] = 0.923;
    m_muonPromptRateEB[3] = 0.976;
    m_muonPromptRateEB[4] = 0.992;
    
    m_muonPromptRateEB_err[0] = 0.011;
    m_muonPromptRateEB_err[1] = 0.007;
    m_muonPromptRateEB_err[2] = 0.003;
    m_muonPromptRateEB_err[3] = 0.001;
    m_muonPromptRateEB_err[4] = 0.001;
  }

  // prompt rate endcap
  if (numVtx>=1 && numVtx<10) { 
    m_muonPromptRateEE[0] = 0.741;
    m_muonPromptRateEE[1] = 0.770;
    m_muonPromptRateEE[2] = 0.913;
    m_muonPromptRateEE[3] = 0.965;
    m_muonPromptRateEE[4] = 0.987;
    
    m_muonPromptRateEE_err[0] = 0.005;
    m_muonPromptRateEE_err[1] = 0.004;
    m_muonPromptRateEE_err[2] = 0.002;
    m_muonPromptRateEE_err[3] = 0.001;
    m_muonPromptRateEE_err[4] = 0.001;
  } else {
    m_muonPromptRateEE[0] = 0.654;
    m_muonPromptRateEE[1] = 0.682;
    m_muonPromptRateEE[2] = 0.863;
    m_muonPromptRateEE[3] = 0.938;
    m_muonPromptRateEE[4] = 0.981;
    
    m_muonPromptRateEE_err[0] = 0.012;
    m_muonPromptRateEE_err[1] = 0.009;
    m_muonPromptRateEE_err[2] = 0.004;
    m_muonPromptRateEE_err[3] = 0.001;
    m_muonPromptRateEE_err[4] = 0.002;
  }
}

void LeptonPlusFakeMLSelection_fullME::Loop() {

  _verbose=false;
  if(fChain == 0) return;
  
  initialiseElectronFakeBinning();
  initialiseElectronFakeEtaBinning();
  initialiseElectronFakeRate15();
  initialiseElectronFakeRate35();
  initialiseElectronFakeRate50();
  initialiseElectronFakeRateQCD();
  initialiseMuonFakeRate();    

  // kinematics reduced tree
  std::string reducedTreeNameME = _datasetName+"-datasetME.root";
  myOutTreeME = new RedHiggsTree(reducedTreeNameME.c_str());
  myOutTreeME->addMLVars();
  myOutTreeME->addLatinos();
  myOutTreeME->addFake();
  myOutTreeME->addRazor();
  myOutTreeME->addKinematics();

  unsigned int lastLumi=0;
  unsigned int lastRun=0;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;
    
    resetKinematicsStart();

    // these depend on the number of PV...
    initialiseElectronPromptRate(nPV);
    initialiseMuonPromptRate(nPV);  

    // weight
    float tmpWeight = 1.;

    // pu weight 
    float nputosave[3];
    if( !_selectionME->getSwitch("isData") ) {
      for(int i=0; i<3; i++) nputosave[i] = float(nPU[i]);
    } else {
      for(int i=0; i<3; i++) nputosave[i] = -1.;
    }

    // Good Run selection
    if (_selectionME->getSwitch("isData") && _selectionME->getSwitch("goodRunLS") && !isGoodRunLS()) {
      if ( lastRun!= runNumber || lastLumi != lumiBlock) {
        lastRun = runNumber;
        lastLumi = lumiBlock;
        std::cout << "[GoodRunLS]::Run " << lastRun << " LS " << lastLumi << " is rejected" << std::endl;
      }
      continue;
    }
    if (_selectionME->getSwitch("isData") && _selectionME->getSwitch("goodRunLS") && ( lastRun!= runNumber || lastLumi != lumiBlock) ) {
      lastRun = runNumber;
      lastLumi = lumiBlock;
      std::cout << "[GoodRunLS]::Run " << lastRun << " LS " << lastLumi << " is OK" << std::endl;
    }
    
    
    // IMPORTANT: FOR DATA RELOAD THE TRIGGER MASK PER FILE WHICH IS SUPPOSED TO CONTAIN UNIFORM CONDITIONS X FILE
    reloadTriggerMask(runNumber);
    bool passedHLT[1];
    passedHLT[me] = hasPassedHLT(); 


    // -------------------------------------------------------------
    // vertex selection - we only consider the first vertex of the list ( = highest sumPT^2)
    bool isGoodVertex = true;
    if (nPV<1) isGoodVertex = false;
    float rhoVtx = sqrt(PVxPV[0]*PVxPV[0] + PVyPV[0]*PVyPV[0]);
    if ( isFakePV[0] )       isGoodVertex = false;
    if ( ndofPV[0]<4 )       isGoodVertex = false;
    if ( fabs(PVzPV[0])>24.) isGoodVertex = false;
    if ( rhoVtx>2 )          isGoodVertex = false; 


    // -------------------------------------------------------------
    // get the best electrons and best muons passing the loose selection ==> tu be used to select ALL the possible channels at the beginning only
    std::pair<int,int> thePreElectrons = getBestElectronPair_denominator();
    std::pair<int,int> thePreMuons     = getBestMuonPair_denominator();
    std::pair<int,int> theBestDenomMuonEle = getBestMuonElePair(_denomEleAll,_denomMuonAll);
    thePreElectronME = theBestDenomMuonEle.second;
    thePreMuonME     = theBestDenomMuonEle.first;

    // reconstructed channel
    m_channel[me] = false;
    
    // at this level the SELECTED channel should have pT > 10 and > 20. So far, at least 2 leptons with pT >20 and 10 in the event
    if ( thePreElectronME > -1 && thePreMuonME > -1 ) {
      float thisMaxPt  = GetPt(pxMuon[thePreMuonME],pyMuon[thePreMuonME]);
      float thisMinPt  = GetPt(pxEle[thePreElectronME],pyEle[thePreElectronME]);
      if (isGoodVertex && thisMaxPt>20 && thisMinPt>10) m_channel[me] = true;    // fixme: hardcoded
    }
    
    if (_verbose) {
      std::cout << "nEle = "   << nEle << "\tnMuon = " << nMuon << std::endl;
      std::cout << "indices: " << thePreElectronME << " " << thePreMuonME << std::endl;
      std::cout << "chargeEle = " << chargeEle[thePreElectronME] << "\tchargeMuon = " << chargeMuon[thePreMuonME] << std::endl;
      std::cout << "mue = "    << m_channel[me] << std::endl; 
    }


    // -------------------------------------------------------------
    // EE candidates: preparing vectors of candidates and selecting the two highest pT ele- and ele+ after each step - to check the 20 GeV cut after 

    // eleID, for electrons in acceptance
    std::pair<int,int> theBestIdEle = getBestElectronPair_id(_denomEleAll);   

    // isolation, for identified electrons
    std::pair<int,int> theBestIsolEle = getBestElectronPair_isol(_idEleAll); 

    // conversion rejection, for isolated electrons
    std::pair<int,int> theBestConvEle = getBestElectronPair_conv(_isolEleAll);     

    // transverse impact parameter, for electrons passing conversion rejection
    std::pair<int,int> theBestIpEle = getBestElectronPair_ip(_convEleAll);     

    // -------------------------------------------------------------
    // MM candidates: preparing vectors of candidates and selecting the two highest pT mu- and mu+ after each step - to check the 20 GeV cut after 

    // muID, for muons in acceptance
    std::pair<int,int> theBestIdMuon = getBestMuonPair_id(_denomMuonAll); 

    // isolation, for identified muons
    std::pair<int,int> theBestIsolMuon = getBestMuonPair_isol(_idMuonsAll); 

    // transverse impact parameter, for isolated muons
    std::pair<int,int> theBestIpMuon = getBestMuonPair_ip(_isolMuonsAll);     

    // -------------------------------------------------------------
    // ME candidates: preparing vectors of candidates and selecting the two highest pT ele+- and muon-+ after each step - to check the 20 GeV cut after

    // leptonID, for leptons in acceptance
    std::pair<int,int> theBestIdMuonEle = getBestMuonElePair(_idEleAll,_idMuonsAll);

    // isolation, for identified leptons
    std::pair<int,int> theBestIsolMuonEle = getBestMuonElePair(_isolEleAll,_isolMuonsAll);

    // conversion rejection, for isolated leptons
    std::pair<int,int> theBestConvMuonEle = getBestMuonElePair(_convEleAll,_isolMuonsAll);

    // transverse impact parameter, for isolated leptons
    std::pair<int,int> theBestIpMuonEle = getBestMuonElePair(_ipEleAll,_ipMuonsAll);

    // -------------------------------------------------------------
    // the two highest pT electron and muon at this point are those I use for my analysis since they passed the full lepton selection  
    int theMuon     = theBestIpMuonEle.first;
    int theElectron = theBestIpMuonEle.second;

    // 0 - 1 - 2 tight candidates                                 
    bool is0tight = false;
    bool is1tight = false;
    bool is2tight = false;

    bool isRealMu = false;
    
    // here I have two candidates with opposite sign passing the tight selection: is N2T 
    if (theMuon>-1 && theElectron>-1) {
      theReal  = theMuon;
      theFake  = theElectron;
      is2tight = true;
      isRealMu = true;
    }

    // here I have only 1 candidate passing the tight selection: is N1T              
    if (theMuon>-1 && theElectron<0) {
      theReal  = theMuon;
      theFake  = getBestEleDenominator(theReal);    
      is1tight = true;
      isRealMu = true;
    }
    if (theElectron>-1 && theMuon<0) {
      theReal  = theElectron;
      theFake  = getBestMuDenominator(theReal);   
      is1tight = true;
      isRealMu = false;
    }

    // here I have zero candidates passing the tight selection: is N0T              
    if (theMuon<0 && theElectron<0) {
      theReal  = theBestDenomMuonEle.first;        
      theFake  = theBestDenomMuonEle.second;     
      is0tight = true;
      isRealMu = true;
    }

    // to be sure: I assumed above that tight => loose. If not true, I discard the event....
    bool denomEleId, denomEleIso, denomMuonId, denomMuonIso;
    if ( (theElectron>-1 && !isEleDenomFake(theElectron,&denomEleId,&denomEleIso)) || (theMuon>-1 && !isMuonDenomFake(theMuon,&denomMuonId,&denomMuonIso)) ) {
      cout << "discard at sanity check" << endl;
      theReal = -1;
      theFake = -1;
    } 
    
    // sanity check
    if ( (is0tight && is1tight) || (is0tight && is2tight) || (is1tight && is2tight) ) cout << "questo non puo' succedere mai" << endl;
    
    // set of kinematics: : now I've all the final leptons 
    resetKinematics();

    // MET is an event variable. Independent o the channel
    m_p3PFMET->SetXYZ(pxPFMet[0],pyPFMet[0],pzPFMet[0]);
    m_theMET = m_p3PFMET->Pt();

    setKinematicsME(theReal, theFake, isRealMu);

    // weight with the Fake / Prompt -> L2 probability                
    float theFakePt, theRealPt;
    bool  isFakeBarrel = false;
    bool  isRealBarrel = false;

    if (isRealMu) { 
      theFakePt = GetPt(pxEle[theFake],pyEle[theFake]);
      theRealPt = GetPt(pxMuon[theReal],pyMuon[theReal]);
      if ( fabs(etaEle[theFake])<1.476 ) isFakeBarrel = true;  
      if ( fabs(etaMuon[theReal])<1.5 )  isRealBarrel = true;
    }

    if (!isRealMu) {
      theFakePt = GetPt(pxMuon[theFake],pyMuon[theFake]);
      theRealPt = GetPt(pxEle[theReal],pyEle[theReal]);
      if ( fabs(etaMuon[theFake])<1.5 )  isFakeBarrel = true;  
      if ( fabs(etaEle[theReal])<1.476 ) isRealBarrel = true;
    }

    // do both F-P and F-F analysis
    float weightFP_15 = 1.;
    float weightFF_15 = 1.;
    float weightPP_15 = 1.;
    // 
    float weightFP_35 = 1.;
    float weightFF_35 = 1.;
    float weightPP_35 = 1.;
    // 
    float weightFP_50 = 1.;
    float weightFF_50 = 1.;
    float weightPP_50 = 1.;    
    // 
    float weightFP_QCD = 1.;
    float weightFF_QCD = 1.;
    float weightPP_QCD = 1.;

    if ( theFake>-1 && theReal>-1) { 
      
      float fakerate1_15,      fakerate1_35,      fakerate1_50,      fakerate1_QCD;
      float fakerateErr1_15,   fakerateErr1_35,   fakerateErr1_50,   fakerateErr1_QCD;
      float promptrate1;
      float promptrateErr1;
      // 
      float fakerate2_15,      fakerate2_35,      fakerate2_50,      fakerate2_QCD;
      float fakerateErr2_15,   fakerateErr2_35,   fakerateErr2_50,   fakerateErr2_QCD;
      float promptrate2;
      float promptrateErr2;


      if (isRealMu) {

	promptrate1    = getElectronPromptRate( theFakePt, etaEle[theFake] );
	promptrateErr1 = getElectronPromptRateError( theFakePt, etaEle[theFake] );

	promptrate2    = getMuonPromptRate( theRealPt, isRealBarrel );
	promptrateErr2 = getMuonPromptRateError( theRealPt, isRealBarrel );

	fakerate1_15  = getElectronFakeRate15(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerate1_35  = getElectronFakeRate35(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerate1_50  = getElectronFakeRate50(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerate1_QCD = getElectronFakeRateQCD( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	
	fakerateErr1_15  = getElectronFakeRateError15(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerateErr1_35  = getElectronFakeRateError35(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerateErr1_50  = getElectronFakeRateError50(  theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	fakerateErr1_QCD = getElectronFakeRateErrorQCD( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
	
	fakerate2_15     = getMuonFakeRate( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerate2_35     = getMuonFakeRate( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerate2_50     = getMuonFakeRate( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerate2_QCD    = getMuonFakeRate( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );

	fakerateErr2_15  = getMuonFakeRateError( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerateErr2_35  = getMuonFakeRateError( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerateErr2_50  = getMuonFakeRateError( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
	fakerateErr2_QCD = getMuonFakeRateError( theRealPt, isRealBarrel, fabs(etaMuon[theReal]) );
      }

      if (!isRealMu) {

	promptrate1    = getMuonPromptRate( theFakePt, isFakeBarrel );  
	promptrateErr1 = getMuonPromptRateError( theFakePt, isFakeBarrel ); 

	promptrate2    = getElectronPromptRate( theRealPt, etaEle[theReal] );
	promptrateErr2 = getElectronPromptRateError( theRealPt, etaEle[theReal] );

	fakerate1_15   = getMuonFakeRate( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerate1_35   = getMuonFakeRate( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerate1_50   = getMuonFakeRate( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerate1_QCD  = getMuonFakeRate( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );

	fakerateErr1_15  = getMuonFakeRateError( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerateErr1_35  = getMuonFakeRateError( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerateErr1_50  = getMuonFakeRateError( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	fakerateErr1_QCD = getMuonFakeRateError( theFakePt, isFakeBarrel, fabs(etaMuon[theFake]) );
	
	fakerate2_15  = getElectronFakeRate15( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerate2_35  = getElectronFakeRate35( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerate2_50  = getElectronFakeRate50( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerate2_QCD = getElectronFakeRateQCD( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );

	fakerateErr2_15  = getElectronFakeRateError15( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerateErr2_35  = getElectronFakeRateError35( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerateErr2_50  = getElectronFakeRateError50( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
	fakerateErr2_QCD = getElectronFakeRateErrorQCD( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      }

      float thisPartWeightFP_15 = 1.;
      float thisPartWeightFF_15 = 1.;
      float thisPartWeightPP_15 = 1.;
      //
      float thisPartWeightFP_35 = 1.;
      float thisPartWeightFF_35 = 1.;
      float thisPartWeightPP_35 = 1.;
      //
      float thisPartWeightFP_50 = 1.;
      float thisPartWeightFF_50 = 1.;
      float thisPartWeightPP_50 = 1.;
      //
      float thisPartWeightFP_QCD = 1.;
      float thisPartWeightFF_QCD = 1.;
      float thisPartWeightPP_QCD = 1.;
      
      if (is0tight) {

	float myFactor_15  = 1/((promptrate1-fakerate1_15)*(promptrate2-fakerate2_15));
	float myFactor_35  = 1/((promptrate1-fakerate1_35)*(promptrate2-fakerate2_35));
	float myFactor_50  = 1/((promptrate1-fakerate1_50)*(promptrate2-fakerate2_50));
	float myFactor_QCD = 1/((promptrate1-fakerate1_QCD)*(promptrate2-fakerate2_QCD));

        // for F-P 
        thisPartWeightFP_15  = -( fakerate1_15*promptrate2*promptrate1*fakerate2_15 + fakerate2_15*promptrate1*promptrate2*fakerate1_15 )*myFactor_15;
        thisPartWeightFP_35  = -( fakerate1_35*promptrate2*promptrate1*fakerate2_35 + fakerate2_35*promptrate1*promptrate2*fakerate1_35 )*myFactor_35;
        thisPartWeightFP_50  = -( fakerate1_50*promptrate2*promptrate1*fakerate2_50 + fakerate2_50*promptrate1*promptrate2*fakerate1_50 )*myFactor_50;
        thisPartWeightFP_QCD = -( fakerate1_QCD*promptrate2*promptrate1*fakerate2_QCD + fakerate2_QCD*promptrate1*promptrate2*fakerate1_QCD )*myFactor_QCD;

        // for F-F
        thisPartWeightFF_15  = ( promptrate1*promptrate2*fakerate1_15*fakerate2_15 )*myFactor_15;
        thisPartWeightFF_35  = ( promptrate1*promptrate2*fakerate1_35*fakerate2_35 )*myFactor_35;
        thisPartWeightFF_50  = ( promptrate1*promptrate2*fakerate1_50*fakerate2_50 )*myFactor_50;
        thisPartWeightFF_QCD = ( promptrate1*promptrate2*fakerate1_QCD*fakerate2_QCD )*myFactor_QCD;

	// for P-P
	thisPartWeightPP_15  = ( promptrate1*promptrate2*fakerate1_15*fakerate2_15 )*myFactor_15;    
	thisPartWeightPP_35  = ( promptrate1*promptrate2*fakerate1_35*fakerate2_35 )*myFactor_35;    
	thisPartWeightPP_50  = ( promptrate1*promptrate2*fakerate1_50*fakerate2_50 )*myFactor_50;    
	thisPartWeightPP_QCD = ( promptrate1*promptrate2*fakerate1_QCD*fakerate2_QCD )*myFactor_QCD;    
      }
      
      if (is1tight) {
        double myFactor_15  = 1/((promptrate1-fakerate1_15)*(promptrate2-fakerate2_15));
	double myFactor_35  = 1/((promptrate1-fakerate1_35)*(promptrate2-fakerate2_35));
        double myFactor_50  = 1/((promptrate1-fakerate1_50)*(promptrate2-fakerate2_50));
	double myFactor_QCD = 1/((promptrate1-fakerate1_QCD)*(promptrate2-fakerate2_QCD));

	// for F-P
	thisPartWeightFP_15  = ( fakerate1_15*(1-promptrate2)*promptrate1*fakerate2_15 + promptrate1*(1-fakerate2_15)*fakerate1_15*promptrate2 )*myFactor_15;
	thisPartWeightFP_35  = ( fakerate1_35*(1-promptrate2)*promptrate1*fakerate2_35 + promptrate1*(1-fakerate2_35)*fakerate1_35*promptrate2 )*myFactor_35;
        thisPartWeightFP_50  = ( fakerate1_50*(1-promptrate2)*promptrate1*fakerate2_50 + promptrate1*(1-fakerate2_50)*fakerate1_50*promptrate2 )*myFactor_50;
	thisPartWeightFP_QCD = ( fakerate1_QCD*(1-promptrate2)*promptrate1*fakerate2_QCD + promptrate1*(1-fakerate2_QCD)*fakerate1_QCD*promptrate2 )*myFactor_QCD;

        // for F-F
	thisPartWeightFF_15  = -( promptrate1*(1-promptrate2)*fakerate1_15*fakerate2_15 )*myFactor_15;
	thisPartWeightFF_35  = -( promptrate1*(1-promptrate2)*fakerate1_35*fakerate2_35 )*myFactor_35;
	thisPartWeightFF_50  = -( promptrate1*(1-promptrate2)*fakerate1_50*fakerate2_50 )*myFactor_50;
	thisPartWeightFF_QCD = -( promptrate1*(1-promptrate2)*fakerate1_QCD*fakerate2_QCD )*myFactor_QCD;

	// for P-P 
	thisPartWeightPP_15  = -( fakerate2_15*(1-fakerate1_15)*promptrate1*promptrate2 )*myFactor_15;  
	thisPartWeightPP_35  = -( fakerate2_35*(1-fakerate1_35)*promptrate1*promptrate2 )*myFactor_35;  
	thisPartWeightPP_50  = -( fakerate2_50*(1-fakerate1_50)*promptrate1*promptrate2 )*myFactor_50;  
	thisPartWeightPP_QCD = -( fakerate2_QCD*(1-fakerate1_QCD)*promptrate1*promptrate2 )*myFactor_QCD;  
      }
      
      if (is2tight) {

	double myFactor_15  = 1/((promptrate1-fakerate1_15)*(promptrate2-fakerate2_15));
	double myFactor_35  = 1/((promptrate1-fakerate1_35)*(promptrate2-fakerate2_35));
	double myFactor_50  = 1/((promptrate1-fakerate1_50)*(promptrate2-fakerate2_50));
	double myFactor_QCD = 1/((promptrate1-fakerate1_QCD)*(promptrate2-fakerate2_QCD));

        // for F-P 
        thisPartWeightFP_15  = -( (promptrate2*fakerate1_15*(1-promptrate1)*(1-fakerate2_15)) + (promptrate1*fakerate2_15*(1-promptrate2)*(1-fakerate1_15)) )*myFactor_15;
        thisPartWeightFP_35  = -( (promptrate2*fakerate1_35*(1-promptrate1)*(1-fakerate2_35)) + (promptrate1*fakerate2_35*(1-promptrate2)*(1-fakerate1_35)) )*myFactor_35;
        thisPartWeightFP_50  = -( (promptrate2*fakerate1_50*(1-promptrate1)*(1-fakerate2_50)) + (promptrate1*fakerate2_50*(1-promptrate2)*(1-fakerate1_50)) )*myFactor_50;
        thisPartWeightFP_QCD = -( (promptrate2*fakerate1_QCD*(1-promptrate1)*(1-fakerate2_QCD)) + (promptrate1*fakerate2_QCD*(1-promptrate2)*(1-fakerate1_QCD)))*myFactor_QCD;

        // for F-F
        thisPartWeightFF_15  = ( fakerate1_15*fakerate2_15*(1-promptrate1)*(1-promptrate2) )*myFactor_15;
        thisPartWeightFF_35  = ( fakerate1_35*fakerate2_35*(1-promptrate1)*(1-promptrate2) )*myFactor_35;
        thisPartWeightFF_50  = ( fakerate1_50*fakerate2_50*(1-promptrate1)*(1-promptrate2) )*myFactor_50;
        thisPartWeightFF_QCD = ( fakerate1_QCD*fakerate2_QCD*(1-promptrate1)*(1-promptrate2) )*myFactor_QCD;

	// for P-P
	thisPartWeightPP_15  = ( promptrate1*promptrate2*(1-fakerate1_15)*(1-fakerate2_15) )*myFactor_15;
	thisPartWeightPP_35  = ( promptrate1*promptrate2*(1-fakerate1_35)*(1-fakerate2_35) )*myFactor_35;
	thisPartWeightPP_50  = ( promptrate1*promptrate2*(1-fakerate1_50)*(1-fakerate2_50) )*myFactor_50;
	thisPartWeightPP_QCD = ( promptrate1*promptrate2*(1-fakerate1_QCD)*(1-fakerate2_QCD) )*myFactor_QCD;
      }
      
      // for F-P
      weightFP_15  = tmpWeight * thisPartWeightFP_15;
      weightFP_35  = tmpWeight * thisPartWeightFP_35;
      weightFP_50  = tmpWeight * thisPartWeightFP_50;
      weightFP_QCD = tmpWeight * thisPartWeightFP_QCD;

      // for F-F
      weightFF_15  = tmpWeight * thisPartWeightFF_15;
      weightFF_35  = tmpWeight * thisPartWeightFF_35;
      weightFF_50  = tmpWeight * thisPartWeightFF_50;
      weightFF_QCD = tmpWeight * thisPartWeightFF_QCD;

      // for P-P
      weightPP_15  = tmpWeight * thisPartWeightPP_15;
      weightPP_35  = tmpWeight * thisPartWeightPP_35;
      weightPP_50  = tmpWeight * thisPartWeightPP_50;
      weightPP_QCD = tmpWeight * thisPartWeightPP_QCD;
      
    } else {
      
      weightFP_15  = tmpWeight;
      weightFP_35  = tmpWeight;
      weightFP_50  = tmpWeight;
      weightFP_QCD = tmpWeight;
      // 
      weightFF_15  = tmpWeight;
      weightFF_35  = tmpWeight;
      weightFF_50  = tmpWeight;
      weightFF_QCD = tmpWeight;
      //
      weightPP_15  = tmpWeight;
      weightPP_35  = tmpWeight;
      weightPP_50  = tmpWeight;
      weightPP_QCD = tmpWeight;
    }

    // nominal: ET cut = 35 
    float weightFP = weightFP_35;

    // -------------------------------------------------------------    
    // initialize the btags for the leading and subleading jets to unphysical value                                                                         
    for(int ichan=0; ichan<1; ichan++) {
      leadJetBtag[ichan] = -2000.;
      subleadJetBtag[ichan] = -2000.;
      subLeadJetsMaxBtag[ichan] = -2000.;
    }

    int njets[1], nuncorrjets[1];
    float dphiLLJ[1], btag[1];
    int nsoftmu[1], nsoftmunojets[1], nextraleptons[1];
    for(int ichan=0; ichan<1; ichan++) {

      // jet counter
      njets[ichan] = numJets(eleCands[ichan],muCands[ichan],ichan);
      nuncorrjets[ichan] = numUncorrJets(eleCands[ichan],muCands[ichan],ichan);

      // if 1-jet bin, use deltaphi(ll-jet)
      dphiLLJ[ichan] = deltaPhiLLJet(ichan);   
      
      // b veto
      btag[ichan] = bVetoJets(eleCands[ichan],muCands[ichan],ichan);

      // soft muon counter
      std::vector<int> emptyJets;
      emptyJets.clear();
      nsoftmu[ichan] = numSoftMuons(muCands[ichan],emptyJets);
      nsoftmunojets[ichan] = -1000;

      // extra lepton counter
      nextraleptons[ichan] = numExtraLeptons(eleCands[ichan],muCands[ichan]);
    }





    // ---------------------------------------
    // filling counter for FP ( = W+jets)      
    CutBasedHiggsSelectionME.SetWeight(weightFP);             
    CutBasedHiggsSelectionME.SetMcTruth(1);
    CutBasedHiggsSelectionME.SetHLT(passedHLT[me]);               
    CutBasedHiggsSelectionME.SetIsChannel(m_channel[me]);     
    
    CutBasedHiggsSelectionME.SetElectronId(theReal);
    CutBasedHiggsSelectionME.SetPositronId(theFake);
    CutBasedHiggsSelectionME.SetElectronIsolation(theReal);
    CutBasedHiggsSelectionME.SetPositronIsolation(theFake);
    CutBasedHiggsSelectionME.SetElectronConvRejection(theReal);
    CutBasedHiggsSelectionME.SetPositronConvRejection(theFake);
    CutBasedHiggsSelectionME.SetElectronIp(theReal);
    CutBasedHiggsSelectionME.SetPositronIp(theFake);

    // checking if the highest pT lepton at the end has pT>20. MU is the hardest in ME, but I do not know if it is real or fake
    float thisMaxPtIpME = -1.;
    if ( isRealMu) thisMaxPtIpME = GetPt(pxMuon[theReal],pyMuon[theReal]);
    if (!isRealMu) thisMaxPtIpME = GetPt(pxMuon[theFake],pyMuon[theFake]);
    if (thisMaxPtIpME<20)   {
      CutBasedHiggsSelectionME.SetElectronIp(-1);
      CutBasedHiggsSelectionME.SetPositronIp(-1);
    }

    CutBasedHiggsSelectionME.SetHighElePt(hardestLeptonPt[me]); 
    CutBasedHiggsSelectionME.SetLowElePt(slowestLeptonPt[me]);  
    CutBasedHiggsSelectionME.SetExtraSlowLeptonPTCut(10.0); // enforce the min pT only on electrons

    CutBasedHiggsSelectionME.SetNJets(njets[me]);
    CutBasedHiggsSelectionME.SetNUncorrJets(nuncorrjets[me]);
    CutBasedHiggsSelectionME.SetBTagJets(btag[me]);
    CutBasedHiggsSelectionME.SetNSoftMuons(nsoftmu[me]);
    CutBasedHiggsSelectionME.SetNExtraLeptons(nextraleptons[me]);
    CutBasedHiggsSelectionME.SetMet(m_theMET);					
    CutBasedHiggsSelectionME.SetNvtx(nPV);
    CutBasedHiggsSelectionME.SetProjectedMet(m_projectedMet[me]);
    CutBasedHiggsSelectionME.SetMetOverPtLL(m_metOptll[me]);
    CutBasedHiggsSelectionME.SetDeltaPhiLLJet(dphiLLJ[me]);   
    CutBasedHiggsSelectionME.SetDeltaPhi(m_deltaPhi[me]);
    CutBasedHiggsSelectionME.SetInvMass(m_mll[me]);
    CutBasedHiggsSelectionME.SetDetaLeptons(m_deltaEtaLeptons[me]);
    CutBasedHiggsSelectionME.SetWWInvMass(m_transvMass[me]);

    bool isSelectedME           = CutBasedHiggsSelectionME.output();    
    bool selUpToFinalLeptonsME  = CutBasedHiggsSelectionME.outputUpToFinalLeptons();
    bool selUpToJetVetoME       = CutBasedHiggsSelectionME.outputUpToJetVeto();
    bool selUpToUncorrJetVetoME = CutBasedHiggsSelectionME.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiME       = CutBasedHiggsSelectionME.outputPreDeltaPhi();

    // latinos
    bool outputStep0  = CutBasedHiggsSelectionME.outputStep0();
    bool outputStep1  = CutBasedHiggsSelectionME.outputStep1();
    bool outputStep2  = CutBasedHiggsSelectionME.outputStep2();
    bool outputStep3  = CutBasedHiggsSelectionME.outputStep3();
    bool outputStep4  = CutBasedHiggsSelectionME.outputStep4();
    bool outputStep5  = CutBasedHiggsSelectionME.outputStep5();
    bool outputStep6  = CutBasedHiggsSelectionME.outputStep6();
    bool outputStep7  = CutBasedHiggsSelectionME.outputStep7();
    bool outputStep8  = CutBasedHiggsSelectionME.outputStep8();
    bool outputStep9  = CutBasedHiggsSelectionME.outputStep9();
    bool outputStep10 = CutBasedHiggsSelectionME.outputStep10();
    bool outputStep11 = CutBasedHiggsSelectionME.outputStep11();
    bool outputStep12 = CutBasedHiggsSelectionME.outputStep12();
    bool outputStep13 = CutBasedHiggsSelectionME.outputStep13();
    bool outputStep14 = CutBasedHiggsSelectionME.outputStep14();
    bool outputStep15 = CutBasedHiggsSelectionME.outputStep15();
    bool outputStep16 = CutBasedHiggsSelectionME.outputStep16();
    bool outputStep17 = CutBasedHiggsSelectionME.outputStep17();
    bool outputStep18 = CutBasedHiggsSelectionME.outputStep18();
    bool outputStep19 = CutBasedHiggsSelectionME.outputStep19();
    bool outputStep20 = CutBasedHiggsSelectionME.outputStep20();
    bool outputStep21 = CutBasedHiggsSelectionME.outputStep21();
    bool outputStep22 = CutBasedHiggsSelectionME.outputStep22();
    bool outputStep23 = CutBasedHiggsSelectionME.outputStep23();
    bool outputStep24 = CutBasedHiggsSelectionME.outputStep24();
    bool outputStep25 = CutBasedHiggsSelectionME.outputStep25();
    bool outputStep26 = CutBasedHiggsSelectionME.outputStep26();
    bool outputStep27 = CutBasedHiggsSelectionME.outputStep27();
    bool outputStep28 = CutBasedHiggsSelectionME.outputStep28();

    // for statistical errors                                                                                                                               
    float forStatErrFP_15_tree = weightFP_15*weightFP_15;
    float forStatErrFF_15_tree = weightFF_15*weightFF_15;
    float forStatErrPP_15_tree = weightPP_15*weightPP_15;
    //                                                                                                                                                      
    float forStatErrFP_35_tree = weightFP_35*weightFP_35;
    float forStatErrFF_35_tree = weightFF_35*weightFF_35;
    float forStatErrPP_35_tree = weightPP_35*weightPP_35;
    //                                                                                                                                                      
    float forStatErrFP_50_tree = weightFP_50*weightFP_50;
    float forStatErrFF_50_tree = weightFF_50*weightFF_50;
    float forStatErrPP_50_tree = weightPP_50*weightPP_50;
    //                                                                                                                                                      
    float forStatErrFP_QCD_tree = weightFP_QCD*weightFP_QCD;
    float forStatErrFF_QCD_tree = weightFF_QCD*weightFF_QCD;
    float forStatErrPP_QCD_tree = weightPP_QCD*weightPP_QCD;

    // nominal: ET cut = 35 
    float forStatErrFP_tree = weightFP_35*weightFP_35;

    // filling the tree                                     
    int myNT = -1;
    if (is0tight) myNT=0;
    if (is1tight) myNT=1;
    if (is2tight) myNT=2;
    
    int theLJ  = theLeadingJet[me];
    int theSJ  = theSecondJet[me];
    float ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeME -> fillKFactor(1., 0., ptLJ);

    myOutTreeME -> fillFake(myNT, weightFP, forStatErrFP_tree, 
			    weightFP_15, forStatErrFP_15_tree, weightFF_15, forStatErrFF_15_tree, weightPP_15, forStatErrPP_15_tree, 
			    weightFP_35, forStatErrFP_35_tree, weightFF_35, forStatErrFF_35_tree, weightPP_35, forStatErrPP_35_tree, 
			    weightFP_50, forStatErrFP_50_tree, weightFF_50, forStatErrFF_50_tree, weightPP_50, forStatErrPP_50_tree, 
			    weightFP_QCD, forStatErrFP_QCD_tree, weightFF_QCD, forStatErrFF_QCD_tree, weightPP_QCD, forStatErrPP_QCD_tree);

    myOutTreeME -> fillRunInfos(runNumber, lumiBlock, eventNumber, weightFP, passedHLT[me], nputosave);
    
    myOutTreeME -> fillRazor(m_MTR[me], m_MR[me], m_GammaMR[me]);
    
    myOutTreeME -> fillAll(m_chMet[me], GetPt(pxPFMet[0],pyPFMet[0]), GetPt(pxMet[0],pyMet[0]),
                           m_projectedMet[me], m_deltaPhi[me], m_deltaErre[me], m_transvMass[me], m_mll[me],
                           hardestLeptonPt[me], slowestLeptonPt[me], hardestLeptonEta[me], slowestLeptonEta[me], 
			   m_deltaEtaLeptons[me], nPV,
                           selUpToFinalLeptonsME, selUpToJetVetoME, selUpToUncorrJetVetoME, selPreDeltaPhiME, isSelectedME);
    
    myOutTreeME -> fillMLVars(njets[me], nuncorrjets[me], m_maxDxyEvt, m_maxDszEvt, btag[me], m_maxImpactParameterMVABJetTags, m_maxCombinedSecondaryVertexMVABJetTags, nsoftmu[me], leadJetBtag[me], subleadJetBtag[me], subLeadJetsMaxBtag[me], nextraleptons[me], nsoftmunojets[me]);

    myOutTreeME -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26, outputStep27, outputStep28 );
    
    float pxLJME[3] = { -999., -999., -999. };
    float pyLJME[3] = { -999., -999., -999. };
    float pzLJME[3] = { -999., -999., -999. };
    float pxSJME[3] = { -999., -999., -999. };
    float pySJME[3] = { -999., -999., -999. };
    float pzSJME[3] = { -999., -999., -999. };
    
    if ( GetPt(m_p4LeptonPlus[me]->Px(),m_p4LeptonPlus[me]->Py()) > GetPt(m_p4LeptonMinus[me]->Px(),m_p4LeptonMinus[me]->Py()) ) {
      
      myOutTreeME -> fillKinematics( m_p3TKMET[me].Px(), m_p3TKMET[me].Py(), m_p3TKMET[me].Pz(),
                                     pxLJME, pyLJME, pzLJME, pxSJME, pySJME, pzSJME,
                                     m_p4LeptonPlus[me]->Px(),  m_p4LeptonPlus[me]->Py(),  m_p4LeptonPlus[me]->Pz(),
                                     m_p4LeptonMinus[me]->Px(), m_p4LeptonMinus[me]->Py(), m_p4LeptonMinus[me]->Pz(),
                                     m_chME, m_lhME, m_isoME, m_chmajME, m_bdtME,
                                     m_jetsSum[me], m_uncorrJetsSum[me], m_p3PFMET);
    } else {
      
      myOutTreeME -> fillKinematics( m_p3TKMET[me].Px(), m_p3TKMET[me].Py(), m_p3TKMET[me].Pz(),
                                     pxLJME, pyLJME, pzLJME, pxSJME, pySJME, pzSJME,
                                     m_p4LeptonMinus[me]->Px(), m_p4LeptonMinus[me]->Py(), m_p4LeptonMinus[me]->Pz(),
                                     m_p4LeptonPlus[me]->Px(),  m_p4LeptonPlus[me]->Py(),  m_p4LeptonPlus[me]->Pz(),
                                     m_chME, m_lhME, m_isoME, m_chmajME, m_bdtME,
                                     m_jetsSum[me], m_uncorrJetsSum[me], m_p3PFMET);
    }
    
    // dumping final tree, only if there are 2 leptons in the acceptance
    if(outputStep1) myOutTreeME -> store();
  }
}

void LeptonPlusFakeMLSelection_fullME::displayEfficiencies(std::string datasetName) {

  std::string::size_type loc = datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    datasetName.erase(loc);
  }
  
  std::cout << "--------------------------------" << std::endl;
  std::cout << "=== RATE ESTIMATED FROM FAKE RATE FOR PF SELECTION ===: " << std::endl;
  CutBasedHiggsSelectionME.displayEfficiencies(datasetName);

  // simple cuts based or like based ele id
  if (!_selectionME->getSwitch("asymmetricID")) {
    std::cout << "cut based symmetric ID: " << std::endl;
    EgammaCutBasedID.displayEfficiencies();
  } else {
    std::cout << "cut based asymmetric ID: Low pT" << std::endl;
    EgammaCutBasedIDLow.displayEfficiencies();
    std::cout << "cut based asymmetric ID: High pT" << std::endl;
    EgammaCutBasedID.displayEfficiencies();
  }
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_acceptance() {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _acceptEleAll.clear();

  for(int i=0;i<nEle;i++) {

    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();

    if(_selectionME->getSwitch("etaElectronAcc") && !_selectionME->passCut("etaElectronAcc",etaEle[i]) ) continue;

    if(_selectionME->getSwitch("ptElectronAcc") && !_selectionME->passCut("ptElectronAcc",thisPt) ) continue;
    
    float thisCharge = chargeEle[i];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = i; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = i;       
    }
    */

    _acceptEleAll.push_back(i);   
  }
  _acceptEleAll = sortElectronsByPt(_acceptEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_id( std::vector<int> acceptEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _idEleAll.clear();

  for (int iEle=0; iEle<acceptEle.size(); iEle++) {
    int thisEle = acceptEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    float thisPt = GetPt(pxEle[thisEle],pyEle[thisEle]);
    if (!_selectionME->getSwitch("asymmetricID")) isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if ( _selectionME->getSwitch("asymmetricID")) {
      if (thisPt>=20) isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if (thisPt<20)  isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }

    if (!theElectronID) continue;

    // further requests if we apply the smurf ID and pT<20
    TString stringIdLow (_selectionME->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      if ( thisPt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[thisEle], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[thisEle];   scEta = etaSC[sc]; }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[thisEle]; scEta = etaPFSC[sc]; }
	if ( fbremEle[thisEle]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[thisEle]>0.95) ) continue; // hardcoded
      }
    }

    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisEle; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisEle;       
    }
    */

    _idEleAll.push_back(thisEle);  
  }
  _idEleAll = sortElectronsByPt(_idEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_isol( std::vector<int> idEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _isolEleAll.clear();

  for (int iEle=0; iEle<idEle.size(); iEle++) {
    int thisEle = idEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);

    if (!theElectronIsol) continue;
    
    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisEle; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisEle;       
    }
    */

    _isolEleAll.push_back(thisEle);  
  }
  _isolEleAll = sortElectronsByPt(_isolEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_conv( std::vector<int> isolEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _convEleAll.clear();

  for (int iEle=0; iEle<isolEle.size(); iEle++) {
    int thisEle = isolEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
    
    if (!theElectronConvRej) continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisEle; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisEle;       
    }
    */

    _convEleAll.push_back(thisEle);      
  }
  _convEleAll = sortElectronsByPt(_convEleAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_ip( std::vector<int> convEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _ipEleAll.clear();

  for (int iEle=0; iEle<convEle.size(); iEle++) {
    int thisEle = convEle[iEle];

    int gsfTrack = gsfTrackIndexEle[thisEle]; 
    // float d3dEle = impactPar3DGsfTrack[gsfTrack];
    // if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) continue;   
    float dxyEle = transvImpactParGsfTrack[gsfTrack];
    float dzEle  = eleDzPV(thisEle,0);
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",dxyEle)) ) continue;
    if (_selectionME->getSwitch("electronDz") && (!_selectionME->passCut("electronDz",dzEle)) )  continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisEle; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisEle;       
    }
    */
    _ipEleAll.push_back(thisEle);
  }
  _ipEleAll = sortElectronsByPt(_ipEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestElectronPair_denominator() {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _denomEleAll.clear();   

  for(int i=0;i<nEle;i++) {

    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();

    bool denomId, denomIso;
    bool isGoodDenom = isEleDenomFake(i,&denomId,&denomIso);
    if (!isGoodDenom) continue;

    float thisCharge = chargeEle[i];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = i; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = i;       
    }
    */
    _denomEleAll.push_back(i);
  }
  _denomEleAll = sortElectronsByPt(_denomEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonPair_acceptance() {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _acceptMuonsAll.clear();

  for(int i=0;i<nMuon;i++) {

    // only not standalone muons latinos
    Utils anaUtils;
    bool thisMuFlag1 = anaUtils.muonIdVal(muonIdMuon[i],AllGlobalMuons);
    bool thisMuFlag2 = anaUtils.muonIdVal(muonIdMuon[i],AllTrackerMuons);
    if (!thisMuFlag1 && !thisMuFlag2) continue;
    if(typeMuon[i]==8) continue;  // to be used when new trees are available
    // only not standalone muons latinos

    if(_selectionME->getSwitch("etaMuonAcc") && !_selectionME->passCut("etaMuonAcc",etaMuon[i]) ) continue;

    float thisPt = GetPt(pxMuon[i],pyMuon[i]);
    if(_selectionME->getSwitch("ptMuonAcc") && !_selectionME->passCut("ptMuonAcc",thisPt) ) continue;

    float thisCharge = chargeMuon[i];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = i; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = i;       
    }
    */

    _acceptMuonsAll.push_back(i);  
  }
  _acceptMuonsAll = sortMuonsByPt(_acceptMuonsAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonPair_id( std::vector<int> acceptMu ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _idMuonsAll.clear();

  for(int iMu=0; iMu<acceptMu.size(); iMu++) {

    int thisMu = acceptMu[iMu];

    bool theMuonID = true;
    isMuonID(thisMu, &theMuonID);
    if (!theMuonID) continue;

    float thisPt     = GetPt(pxMuon[thisMu],pyMuon[thisMu]);
    float thisCharge = chargeMuon[thisMu];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisMu; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisMu;       
    }
    */

    _idMuonsAll.push_back(thisMu);   
  }
  _idMuonsAll = sortMuonsByPt(_idMuonsAll);
  
  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonPair_isol( std::vector<int> idMu ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
   
  _isolMuonsAll.clear();
  
  for(int iMu=0; iMu<idMu.size(); iMu++) {
    
    int thisMu   = idMu[iMu];
    float thisPt = GetPt(pxMuon[thisMu],pyMuon[thisMu]);

    // fixme: diverso da prima: rimuovevo il secondo leptone....
    // float muonTrackerForGlobal = sumPt03Muon[thisMu];
    // float muonEcalForGlobal    = emEt03Muon[thisMu];
    // float muonHcalForGlobal    = hadEt03Muon[thisMu]; 
    // float theMuonGlobalSum     = muonTrackerForGlobal + muonEcalForGlobal + muonHcalForGlobal - rhoFastjet*TMath::Pi()*0.3*0.3;
    // float theRelMuonIso        = theMuonGlobalSum/thisPt; 
    // if(_selectionME->getSwitch("muGlobalIso") && !_selectionME->passCut("muGlobalIso",theRelMuonIso)) continue;  
    if( !isPFIsolatedMuon(thisMu) ) continue;

    float thisCharge = chargeMuon[thisMu];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisMu; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisMu;       
    }
    */

    _isolMuonsAll.push_back(thisMu);   
  }
  _isolMuonsAll = sortMuonsByPt(_isolMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonPair_ip( std::vector<int> isoMu ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _ipMuonsAll.clear();  

  for(int iMu=0; iMu<isoMu.size(); iMu++) {

    int thisMu = isoMu[iMu];

    float thisPt = GetPt(pxMuon[thisMu],pyMuon[thisMu]);
    
    int ctfMuon   = trackIndexMuon[thisMu]; 
    float dxyMuon = transvImpactParTrack[ctfMuon];
    float dzMuon  = muonDzPV(thisMu,0);
    // if (_selectionME->getSwitch("muonIP") && (!_selectionME->passCut("muonIP",dxyMuon)) ) continue;   
    // if (_selectionME->getSwitch("muonDz") && (!_selectionME->passCut("muonDz",dzMuon)) )  continue;   
    if (thisPt>20)    // hardcoded
      if (_selectionME->getSwitch("muonIPhighPT") && (!_selectionME->passCut("muonIPhighPT",dxyMuon)) ) continue;   
    
    if (thisPt<20)    // hardcoded
      if (_selectionME->getSwitch("muonIPlowPT")  && (!_selectionME->passCut("muonIPlowPT",dxyMuon)) ) continue;   
    
    if (_selectionME->getSwitch("muonDz") && (!_selectionME->passCut("muonDz",dzMuon)) )  continue;   

    float thisCharge = chargeMuon[thisMu];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = thisMu; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = thisMu;       
    }
    */
    _ipMuonsAll.push_back(thisMu);   
  }
  _ipMuonsAll = sortMuonsByPt(_ipMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonPair_denominator() {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _denomMuonAll.clear();   

  for(int i=0;i<nMuon;i++) {

    TVector3 pLepton(pxMuon[i],pyMuon[i],pzMuon[i]);
    float thisPt=pLepton.Pt();

    bool denomId, denomIso;
    bool isGoodDenom = isMuonDenomFake(i,&denomId,&denomIso);
    if (!isGoodDenom) continue;

    float thisCharge = chargeMuon[i];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same sign test
    if (thisCharge > 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){ 
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt; 
      theLep2   = theLep1; 
      theLep1   = i; 
    } else if ( thisCharge > 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt; 
      theLep2   = i;       
    }
    */

    _denomMuonAll.push_back(i);
  }

  _denomMuonAll = sortMuonsByPt(_denomMuonAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullME::getBestMuonElePair(std::vector<int> electrons, std::vector<int> muons) {

  int theEle =-1;
  int theMuon=-1;

  if (muons.size()==0 && electrons.size()==0) { return std::make_pair(theMuon,theEle); }

  if (muons.size()>0 && electrons.size()==0) { 
    std::vector<int>::const_iterator muiter;
    muiter=muons.begin();
    return std::make_pair(*muiter,theEle); 
  }

  if (muons.size()==0 && electrons.size()>0) { 
    std::vector<int>::const_iterator eleiter;
    eleiter=electrons.begin();
    return std::make_pair(theMuon,*eleiter); 
  }

  if (muons.size()>0 && electrons.size()>0) {

    std::vector<int>::const_iterator muiter;
    for(muiter=muons.begin(); muiter<muons.end();++muiter) {
      int muCharge = chargeMuon[*muiter];
      float muPt = GetPt(pxMuon[*muiter],pyMuon[*muiter]);

      std::vector<int>::const_iterator eleiter;
      for(eleiter=electrons.begin(); eleiter<electrons.end();++eleiter) {
	int eleCharge = chargeEle[*eleiter];
	float elePt = GetPt(pxEle[*eleiter],pyEle[*eleiter]);

	// chiara: for SS test
	if(eleCharge*muCharge<0 && muPt>elePt) return std::make_pair(*muiter,*eleiter); 
	// if(eleCharge*muCharge>0 && muPt > elePt && eleCharge>0) return std::make_pair(*muiter,*eleiter); 
      }
    }
    return std::make_pair(*muons.begin(),theEle);   // chiara: questo andra' fissato...  
  } 
}

int LeptonPlusFakeMLSelection_fullME::getBestEleDenominator(int realMuon) {  // this requires not only denom ok, but also NOT tight!
  
  int theFake=-1;
  
  float maxPtFake=-1000.;
  
  float theRealPt = GetPt(pxMuon[realMuon],pyMuon[realMuon]);    

  for(int iele=0; iele<nEle; iele++) {
    
    float thisElePt = GetPt(pxEle[iele],pyEle[iele]);    

    // chiara: for same sign test
    if (chargeEle[iele]*chargeMuon[realMuon]>0) continue;
    // if (chargeEle[iele]*chargeMuon[realMuon]<0) continue;
    // if (chargeEle[iele]<0) continue;

    if (thisElePt>theRealPt) continue;

    bool denomId, denomIso;
    bool isGoodDenom = isEleDenomFake(iele,&denomId,&denomIso);   
    if (!isGoodDenom) continue;
    
    // removing candidates passing the tight selection
    bool isTight = true;
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    if (!_selectionME->getSwitch("asymmetricID")) isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if ( _selectionME->getSwitch("asymmetricID")) {
      if (thisElePt>=20) isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if (thisElePt<20)  isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }
    if (!theElectronID)      isTight = false;
    TString stringIdLow (_selectionME->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      if ( thisElePt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[iele], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[iele];   scEta = etaSC[sc]; }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[iele]; scEta = etaPFSC[sc]; }
	if ( fbremEle[iele]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[iele]>0.95) ) isTight = false;  // hardcoded
      }
    }
    if (!theElectronIsol)    isTight = false; 
    if (!theElectronConvRej) isTight = false;
    int gsfTrack = gsfTrackIndexEle[iele]; 
    // float d3dEle = impactPar3DGsfTrack[gsfTrack];
    // if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) isTight = false;
    float dxyEle = transvImpactParGsfTrack[gsfTrack];
    float dzEle  = eleDzPV(iele,0);
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",dxyEle)) ) isTight = false;
    if (_selectionME->getSwitch("electronDz") && (!_selectionME->passCut("electronDz",dzEle)) )  isTight = false;
    if (isTight) continue;

    if( thisElePt > maxPtFake ) { maxPtFake = thisElePt; theFake=iele; }
  }
  
  return theFake;
}


int LeptonPlusFakeMLSelection_fullME::getBestMuDenominator(int realEle) {  // this requires not only denom ok, but also NOT tight!

  int theFake=-1;
  
  float maxPtFake=-1000.;
  
  float theRealPt = GetPt(pxEle[realEle],pyEle[realEle]);    

  for(int imu=0; imu<nMuon; imu++) {
    
    float thisMuPt = GetPt(pxMuon[imu],pyMuon[imu]);    
   
    // chiara: for SS test
    if (chargeEle[realEle]*chargeMuon[imu]>0) continue;
    // if (chargeEle[realEle]*chargeMuon[imu]<0) continue;
    // if (chargeEle[realEle]<0) continue;

    if (theRealPt>thisMuPt) continue;

    bool denomId, denomIso;
    bool isGoodDenom = isMuonDenomFake(imu,&denomId,&denomIso);   
    if (!isGoodDenom) continue;

    // removing candidates passing the tight selection
    bool isTight = true;
    isMuonID(imu, &isTight);
    if (!isPFIsolatedMuon(imu)) isTight = false;
    int ctfMuon   = trackIndexMuon[imu]; 
    float dxyMuon = transvImpactParTrack[ctfMuon];
    float dzMuon  = muonDzPV(imu,0);
    if (thisMuPt>20)    // hardcoded
      if (_selectionME->getSwitch("muonIPhighPT") && (!_selectionME->passCut("muonIPhighPT",dxyMuon)) ) isTight = false;
    if (thisMuPt<20)    // hardcoded
      if (_selectionME->getSwitch("muonIPlowPT")  && (!_selectionME->passCut("muonIPlowPT",dxyMuon)) ) isTight = false;
    if (_selectionME->getSwitch("muonDz") && (!_selectionME->passCut("muonDz",dzMuon)) )  isTight = false;
    if (isTight) continue;

    if( thisMuPt > maxPtFake ) { maxPtFake = thisMuPt; theFake=imu; }
  }
  
  return theFake;
}


float LeptonPlusFakeMLSelection_fullME::getElectronFakeRate15( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m15_eleFakeRateEB1[theBin];
	  if (theEtaBin==1) return m15_eleFakeRateEB2[theBin];
	  if (theEtaBin==2) return m15_eleFakeRateEE1[theBin];
	  if (theEtaBin==3) return m15_eleFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fake eta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRate35( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m35_eleFakeRateEB1[theBin];
	  if (theEtaBin==1) return m35_eleFakeRateEB2[theBin];
	  if (theEtaBin==2) return m35_eleFakeRateEE1[theBin];
	  if (theEtaBin==3) return m35_eleFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fake eta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRate50( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m50_eleFakeRateEB1[theBin];
	  if (theEtaBin==1) return m50_eleFakeRateEB2[theBin];
	  if (theEtaBin==2) return m50_eleFakeRateEE1[theBin];
	  if (theEtaBin==3) return m50_eleFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fake eta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRateQCD( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return mQCD_eleFakeRateEB1[theBin];
	  if (theEtaBin==1) return mQCD_eleFakeRateEB2[theBin];
	  if (theEtaBin==2) return mQCD_eleFakeRateEE1[theBin];
	  if (theEtaBin==3) return mQCD_eleFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fake eta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRateError15( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m15_eleFakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m15_eleFakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m15_eleFakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m15_eleFakeRateEE2_err[theBin];
	}
      }
    }
  }
  
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRateError35( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m35_eleFakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m35_eleFakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m35_eleFakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m35_eleFakeRateEE2_err[theBin];
	}
      }
    }
  }
  
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRateError50( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m50_eleFakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m50_eleFakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m50_eleFakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m50_eleFakeRateEE2_err[theBin];
	}
      }
    }
  }
  
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronFakeRateErrorQCD( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_eleMinFakePt[theBin] && fakePt < m_eleMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_eleMinFakeEta[theEtaBin] && fabs(fakeEta) < m_eleMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return mQCD_eleFakeRateEB1_err[theBin];
	  if (theEtaBin==1) return mQCD_eleFakeRateEB2_err[theBin];
	  if (theEtaBin==2) return mQCD_eleFakeRateEE1_err[theBin];
	  if (theEtaBin==3) return mQCD_eleFakeRateEE2_err[theBin];
	}
      }
    }
  }
  
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronPromptRate( float promptPt, float promptEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( promptPt >= m_eleMinPromptPt[theBin] && promptPt < m_eleMaxPromptPt[theBin] ) {

      if (fabs(promptEta)<1.4442)  return m_elePromptRateEB[theBin];
      if (fabs(promptEta)>1.566)   return m_elePromptRateEE[theBin];
      if (fabs(promptEta)<1.566 && fabs(promptEta)>1.4442)   return m_elePromptRateCr[theBin];
    }
  }

  std::cout << "BIG ERROR: promptPt = " << promptPt << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getElectronPromptRateError( float promptPt, float promptEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( promptPt >= m_eleMinPromptPt[theBin] && promptPt < m_eleMaxPromptPt[theBin] ) {

      if (fabs(promptEta)<1.4442)  return m_elePromptRateEB_err[theBin];
      if (fabs(promptEta)>1.566)   return m_elePromptRateEE_err[theBin];
      if (fabs(promptEta)<1.566 && fabs(promptEta)>1.4442)   return m_elePromptRateCr_err[theBin];
    }
  }

  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getMuonFakeRate( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_muonMinFakePt[theBin] && fakePt < m_muonMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_muonMinFakeEta[theEtaBin] && fabs(fakeEta) < m_muonMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m_muonFakeRateEB1[theBin];
	  if (theEtaBin==1) return m_muonFakeRateEB2[theBin];
	  if (theEtaBin==2) return m_muonFakeRateEE1[theBin];
	  if (theEtaBin==3) return m_muonFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getMuonFakeRateError( float fakePt, bool isFakeBarrel, float fakeEta ) {
  
  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_muonMinFakePt[theBin] && fakePt < m_muonMaxFakePt[theBin] ) {
      
      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_muonMinFakeEta[theEtaBin] && fabs(fakeEta) < m_muonMaxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m_muonFakeRateEB1[theBin];
	  if (theEtaBin==1) return m_muonFakeRateEB2[theBin];
	  if (theEtaBin==2) return m_muonFakeRateEE1[theBin];
	  if (theEtaBin==3) return m_muonFakeRateEE2[theBin];
	}
      }
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getMuonPromptRate( float promptPt, bool isPromptBarrel ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( promptPt >= m_muonMinPromptPt[theBin] && promptPt < m_muonMaxPromptPt[theBin] ) {
      if (isPromptBarrel)  return m_muonPromptRateEB[theBin];
      if (!isPromptBarrel) return m_muonPromptRateEE[theBin];
    }
  }
  
  std::cout << "BIG ERROR: promptPt = " << promptPt << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullME::getMuonPromptRateError( float promptPt, bool isPromptBarrel ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( promptPt >= m_muonMinPromptPt[theBin] && promptPt < m_muonMaxPromptPt[theBin] ) {
      if (isPromptBarrel)  return m_muonPromptRateEB_err[theBin];
      if (!isPromptBarrel) return m_muonPromptRateEE_err[theBin];
    }
  }

  return -1.;
}

void LeptonPlusFakeMLSelection_fullME::setKinematicsME(int myReal, int myFake, bool realMu) {

  if (myFake > -1 && myReal > -1) {

    if (realMu) { 
      eleCands[me].push_back(myFake);
      muCands[me] .push_back(myReal);
      m_p4LeptonMinus[me] -> SetXYZT(pxEle[myFake],pyEle[myFake],pzEle[myFake],energyEle[myFake]);
      m_p4LeptonPlus[me]  -> SetXYZT(pxMuon[myReal],pyMuon[myReal],pzMuon[myReal],energyMuon[myReal]);
      hardestLeptonPt[me] = GetPt(pxMuon[myReal],pyMuon[myReal]);
      slowestLeptonPt[me] = GetPt(pxEle[myFake], pyEle[myFake]);
      
      if ( hardestLeptonPt[me] != GetPt(pxMuon[myReal],pyMuon[myReal]) ) {
	cout << "questo non puo succedere mai" << endl;
	cout << "myReal = " << myReal << ", myFake = " << myFake << endl;
	cout << "hardest PT = " << hardestLeptonPt[me] << endl;
	cout << "slowest PT = " << slowestLeptonPt[me] << endl;
	cout << "real PT = "    << GetPt(pxMuon[myReal],pyMuon[myReal]) << endl;
	cout << "fake PT = "    << GetPt(pxEle[myFake],pyEle[myFake]) << endl;
      }

      hardestLeptonEta[me] = etaMuon[myReal];
      slowestLeptonEta[me] = etaEle[myFake];
      m_mll[me] = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
      m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
      m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
      m_deltaEtaLeptons[me] = etaEle[myFake]-etaMuon[myReal];
      m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
      m_transvMass[me] = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );
      m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MR[me]      = CalcMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MTR[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
      m_mT2[me]           = 0.;
      m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_chMet[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect()).Pt();
      
      m_chEE[0]  = chargeMuon[myReal];
      m_chEE[1]  = chargeEle[myFake];
      m_isoEE[0] = pfCombinedIsoMuon[myReal] / hardestLeptonPt[me];
      m_isoEE[1] = pfCombinedIsoEle[myFake] / slowestLeptonPt[me];
      m_lhEE[0]  = -999;
      m_lhEE[1]  = eleIdLikelihoodEle[myFake];
      m_bdtEE[0] = -999;
      m_bdtEE[1] = eleBDT(fMVA,myFake);
      m_chmajEE[0] = -999;
      m_chmajEE[1] = eleChargeMajority(myFake);
    } 


    if (!realMu) { 
      eleCands[me].push_back(myReal);
      muCands[me] .push_back(myFake);
      m_p4LeptonMinus[me] -> SetXYZT(pxEle[myReal],pyEle[myReal],pzEle[myReal],energyEle[myReal]);
      m_p4LeptonPlus[me]  -> SetXYZT(pxMuon[myFake],pyMuon[myFake],pzMuon[myFake],energyMuon[myFake]);
      hardestLeptonPt[me] = GetPt(pxMuon[myFake],pyMuon[myFake]);
      slowestLeptonPt[me] = GetPt(pxEle[myReal], pyEle[myReal]);
      
      if ( hardestLeptonPt[me] != GetPt(pxMuon[myFake],pyMuon[myFake]) ) {
	cout << "questo non puo succedere mai" << endl;
	cout << "myReal = " << myReal << ", myFake = " << myFake << endl;
	cout << "hardest PT = " << hardestLeptonPt[me] << endl;
	cout << "slowest PT = " << slowestLeptonPt[me] << endl;
	cout << "real PT = "    << GetPt(pxMuon[myFake],pyMuon[myFake]) << endl;
	cout << "fake PT = "    << GetPt(pxEle[myReal],pyEle[myReal]) << endl;
      }

      hardestLeptonEta[me] = etaMuon[myFake];
      slowestLeptonEta[me] = etaEle[myReal];
      m_mll[me] = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
      m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
      m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
      m_deltaEtaLeptons[me] = etaEle[myReal]-etaMuon[myFake];
      m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
      m_transvMass[me] = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );
      m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MR[me]      = CalcMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MTR[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
      m_mT2[me]           = 0.;
      m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_chMet[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect()).Pt();
      
      m_chEE[0]  = chargeMuon[myFake];
      m_chEE[1]  = chargeEle[myReal];
      m_isoEE[0] = pfCombinedIsoMuon[myFake] / hardestLeptonPt[me];
      m_isoEE[1] = pfCombinedIsoEle[myReal] / slowestLeptonPt[me];
      m_lhEE[0]  = -999;
      m_lhEE[1]  = eleIdLikelihoodEle[myReal];
      m_bdtEE[0] = -999;
      m_bdtEE[1] = eleBDT(fMVA,myReal);
      m_chmajEE[0] = -999;
      m_chmajEE[1] = eleChargeMajority(myReal);
    } 
  }  
}

void LeptonPlusFakeMLSelection_fullME::resetKinematicsStart() {

  thePreElectronME = -1;
  thePreMuonME     = -1;
  theReal          = -1;
  theFake          = -1;
}

void LeptonPlusFakeMLSelection_fullME::resetKinematics() {

  for(int theChannel=0; theChannel<1; theChannel++) {
    eleCands[theChannel].clear();
    muCands[theChannel].clear();
    m_p4LeptonMinus[theChannel] -> SetXYZT(0,0,0,0);                                                        
    m_p4LeptonPlus[theChannel]  -> SetXYZT(0,0,0,0);
    m_p3PFMET                   -> SetXYZ(0,0,0);
    hardestLeptonPt[theChannel]   = 0.;
    slowestLeptonPt[theChannel]   = 0.;
    hardestLeptonEta[theChannel]  = 0.;
    slowestLeptonEta[theChannel]  = 0.;
    m_mll[theChannel]             = 0.;
    m_deltaPhi[theChannel]        = 0.;
    m_deltaErre[theChannel]       = 0.;
    m_deltaEtaLeptons[theChannel] = 0.; 
    m_dilepPt[theChannel]         = 0.;
    m_transvMass[theChannel]      = 0.;
    m_metOptll[theChannel]        = 0.;
    m_mT2[theChannel]             = 0.;
    m_GammaMR[theChannel]         = 0.;
    m_MR[theChannel]              = 0.;
    m_MTR[theChannel]             = 0.; 
    m_projectedMet[theChannel]    = 0.;
    m_chMet[theChannel]           = 0.;
  }
}

int LeptonPlusFakeMLSelection_fullME::numJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel) {

  int num=0;
  m_goodJets.clear();
  float ETMax=0.;
  float ETMax2=0.;
  
  theLeadingJet[theChannel]=-1;
  theSecondJet[theChannel]=-1;
  m_jetsSum[theChannel]->SetXYZT(0.,0.,0.,0);
  
  TString JESUncertainty(_selectionME->getStringParameter("JESUncertainty"));
  
  for(int j=0;j<nAK5PFPUcorrJet;j++) {
    
    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);
    
    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) pt = (GetJESCorrected(p4Jet,JESUncertainty.Data())).Pt();
    
    // PF jet ID variables                                                                                                                                    
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    
    //     if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,                                                               
    //                   chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;                                                       
    
    bool foundMatch = false;
    // check if the electrons falls into the jet                                                                                                              
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
	float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    // check if the muons falls into the jet                                                                                                                  
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;
    
    if(pt>5.0) (*m_jetsSum[theChannel]) += p4Jet;
    
    if(_selectionME->getSwitch("etaJetAcc") && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;
    if(fabs(etaAK5PFPUcorrJet[j]==5)) continue; // this is to remove the place with wrong JECs (https://hypernews.cern.ch/HyperNews/CMS/get/JetMET/1259/1.html)                                                                                                                                                            
    if ( pt>ETMax2 && pt>ETMax ) {
      
      theSecondJet[theChannel] = theLeadingJet[theChannel];
      subleadJetBtag[theChannel] = trackCountingHighEffBJetTagsAK5PFPUcorrJet[theSecondJet[theChannel]];
      ETMax2 = ETMax;

      theLeadingJet[theChannel] = j;
      leadJetBtag[theChannel] = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];
      ETMax = pt;
      
    } else if ( pt>ETMax2 && pt<ETMax ) {
      theSecondJet[theChannel] = j;
      subleadJetBtag[theChannel] = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];
      ETMax2 = pt;
    }
    if(_selectionME->getSwitch("etJetAcc") && !_selectionME->passCut("etJetAcc", pt)) continue;
    
    m_goodJets.push_back(j);
    num++;
  }
  
  return num;
}

int LeptonPlusFakeMLSelection_fullME::numUncorrJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  int num=0;

  TString JESUncertainty(_selectionME->getStringParameter("JESUncertainty"));

  m_uncorrJetsSum[theChannel]->SetXYZT(0.,0.,0.,0.);

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    float uncorrEt = uncorrEnergyAK5PFPUcorrJet[j]*fabs(sin(thetaAK5PFPUcorrJet[j]));
    TLorentzVector p4Jet;
    p4Jet.SetPtEtaPhiE(uncorrEt,etaAK5PFPUcorrJet[j],phiAK5PFPUcorrJet[j],uncorrEnergyAK5PFPUcorrJet[j]);
    TVector3 p3Jet = p4Jet.Vect();

    TLorentzVector p4JESJet(p3Jet, uncorrEnergyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) uncorrEt = (GetJESCorrected(p4JESJet,JESUncertainty.Data())).Pt();
    
    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    
    // if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
    //           chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    bool foundMatch=false;
    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }

    // check if the muons falls into the jet
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;
    
    if(uncorrEt>5.0) (*m_uncorrJetsSum[theChannel]) += p4Jet;
    if(_selectionME->getSwitch("etaJetAcc")      && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;
    if(_selectionME->getSwitch("etUncorrJetAcc") && !_selectionME->passCut("etUncorrJetAcc", uncorrEt))   continue;

    num++;
  }
  
  return num;
}

float LeptonPlusFakeMLSelection_fullME::bVetoJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  TString JESUncertainty(_selectionME->getStringParameter("JESUncertainty"));

  float output=-999;
  float outputSubLeadJets = -999;

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);

    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) pt = (GetJESCorrected(p4Jet,JESUncertainty.Data())).Pt();
    if(_selectionME->getSwitch("etaJetAcc") && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;

    // hardcoded
    float rawpt = uncorrEnergyAK5PFPUcorrJet[j] * fabs(sin(thetaAK5PFPUcorrJet[j]));
    if(rawpt < 10.0) continue;    

    if(weightedDz1AK5PFPUcorrJet[j] >= 2) continue;

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrEnergyAK5PFPUcorrJet[j];
    
    // if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
    //           chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    bool foundMatch=false;
    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }

    // check if the muons falls into the jet
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionME->getSwitch("jetConeWidth") && _selectionME->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    float tmp = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];     
    if(tmp > output) output = tmp;
    if(j != theLeadingJet[theChannel] && tmp > outputSubLeadJets) outputSubLeadJets = tmp;
  }

  return output;

}

float LeptonPlusFakeMLSelection_fullME::deltaPhiLLJet(int ichan) {   
  
  int myLeadingJet = theLeadingJet[ichan];

  if(myLeadingJet > -1) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);    
    if(leadingJetP3.Pt()>15.0) 
      return fabs(180./TMath::Pi() * leadingJetP3.DeltaPhi(m_dilepPt[ichan]));                           
    else 
      return 0.0;
  } else return -999.;
}

int LeptonPlusFakeMLSelection_fullME::numSoftMuons(std::vector<int> muonToRemove, std::vector<int> jetsToRemove) {
  
  int num = 0;
  for(int i=0; i<nMuon; ++i) {
    
    bool isSelMuon=false;
    for(int muSel=0; muSel<(int)muonToRemove.size(); muSel++) {
      if(i==muonToRemove[muSel]) isSelMuon=true;
    }
    if(isSelMuon) continue;
    
    bool isAroundGoodJet=false;
    for(int jet=0; jet<(int)jetsToRemove.size(); jet++) {
      int j = jetsToRemove[jet];
      TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
      TVector3 p3Muon(pxMuon[i],pyMuon[i],pzMuon[i]);
      if(p3Jet.DeltaR(p3Muon)<0.5) isAroundGoodJet=true;
    }
    if(isAroundGoodJet) continue;
    float pt = GetPt(pxMuon[i],pyMuon[i]);
    if(pt < 3.0) continue;
    
    Utils anaUtils;
    if(!anaUtils.muonIdVal(muonIdMuon[i],AllTrackerMuons) || !anaUtils.muonIdVal(muonIdMuon[i],TMLastStationTight)) continue;
    
    int track = trackIndexMuon[i];
    if(trackValidHitsTrack[track]<=10) continue;
    
    float dxyMuon= transvImpactParTrack[track];
    float dzMuon = muonDzPV(i,0);
    if(dxyMuon > 0.200) continue;     // hardcoded	
    if(dzMuon  > 0.200) continue;     // hardcoded

    float isoSumRel = (sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i]) / pt;
    //    float isoSumRel = pfCombinedIsoMuon[i] / pt;
    if(pt>20 && isoSumRel<0.1) continue;  
    
    num++;
  }
  return num;
}

int LeptonPlusFakeMLSelection_fullME::numExtraLeptons( std::vector<int> eleToRemove, std::vector<int> muonToRemove  ) {

  int numEle = 0;
  for(int i=0; i<nEle; ++i) {
    
    bool isSelEle=false;
    for(int eleSel=0; eleSel<(int)eleToRemove.size(); eleSel++) {
      if(i==eleToRemove[eleSel]) isSelEle=true;
    }
    if(isSelEle) continue;

    if(_selectionME->getSwitch("etaElectronAcc") && !_selectionME->passCut("etaElectronAcc",etaEle[i]) ) continue;
    if(_selectionME->getSwitch("ptElectronAcc")  && !_selectionME->passCut("ptElectronAcc",GetPt(pxEle[i],pyEle[i])) ) continue;

    bool theId, theIso, theConvRej;
    theId = theIso = theConvRej = true;
    if (!_selectionME->getSwitch("asymmetricID")) 
      isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if (_selectionME->getSwitch("asymmetricID")) {
      float pt = GetPt(pxEle[i],pyEle[i]);	
      if(pt>=20) isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if(pt<20)  isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }
    if(!theId || !theIso || !theConvRej) continue;

    // further requests if we apply the smurf ID and pT<15
    TString stringIdLow (_selectionME->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      float pt = GetPt(pxEle[i],pyEle[i]);
      if ( pt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[i], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[i];   scEta = etaSC[sc]; }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[i]; scEta = etaPFSC[sc]; }
	if ( fbremEle[i]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[i]>0.95) ) continue; // hardcoded
      }
    }

    int track = gsfTrackIndexEle[i];
    // float d3dEle = impactPar3DGsfTrack[track];
    // if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) continue;    
    float dxyEle = transvImpactParGsfTrack[track];
    float dzEle  = eleDzPV(i,0);
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",dxyEle)) ) continue;
    if (_selectionME->getSwitch("electronDz") && (!_selectionME->passCut("electronDz",dzEle)) ) continue;

    numEle++;
  }

  int numMu = 0;
  for(int i=0; i<nMuon; ++i) {
    
    bool isSelMuon=false;
    for(int muSel=0; muSel<(int)muonToRemove.size(); muSel++) {
      if(i==muonToRemove[muSel]) isSelMuon=true;
    }
    if(isSelMuon) continue;
    
    float ptMu = GetPt(pxMuon[i],pyMuon[i]);
    if(_selectionME->getSwitch("etaMuonAcc") && !_selectionME->passCut("etaMuonAcc",etaMuon[i]) ) continue;
    if(_selectionME->getSwitch("ptMuonAcc") && !_selectionME->passCut("ptMuonAcc",ptMu) ) continue;

    bool theId = true;
    isMuonID(i,&theId);
    if(!theId) continue;
    //float isoSumAbs = sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i] - rhoFastjet*TMath::Pi()*0.3*0.3;
    //float isoSumRel = isoSumAbs / ptMu;
    //if(_selectionME->getSwitch("muGlobalIso") && !_selectionME->passCut("muGlobalIso",isoSumRel)) continue;
    if( ! isPFIsolatedMuon(i) ) continue; 

    int track = trackIndexMuon[i];
    float dxy = transvImpactParTrack[track];
    float dz  = muonDzPV(i,0);
    //if(_selectionME->getSwitch("muonIP") && !_selectionME->passCut("muonIP",dxy)) continue;
    //if(_selectionME->getSwitch("muonDz") && !_selectionME->passCut("muonDz",dz))  continue;  
    if (ptMu>20)   // hardcoded
      if (_selectionME->getSwitch("muonIPhighPT") && (!_selectionME->passCut("muonIPhighPT",dxy)) ) continue;   
    if (ptMu<20)    // hardcoded
      if (_selectionME->getSwitch("muonIPlowPT")  && (!_selectionME->passCut("muonIPlowPT",dxy)) ) continue;   
    if (_selectionME->getSwitch("muonDz") && (!_selectionME->passCut("muonDz",dz)) )  continue;   

    numMu++;
  }
  
  return numEle + numMu;
}

float LeptonPlusFakeMLSelection_fullME::GetProjectedMet(TVector3 p1, TVector3 p2) {

  // calculate with PF met
  float projMET_pf = 0.0;
  float deltaPhi1_pf = fabs(p1.DeltaPhi(*m_p3PFMET));
  float deltaPhi2_pf = fabs(p2.DeltaPhi(*m_p3PFMET));
  float deltaphi_pf = TMath::Min(deltaPhi1_pf,deltaPhi2_pf);
  if(deltaphi_pf<TMath::Pi()/2.) projMET_pf = m_p3PFMET->Mag() * sin(deltaphi_pf);
  else projMET_pf = m_p3PFMET->Mag();

  // calculate with TKMET
  TVector3 p3tkMet = pfChargedMet(p1,p2);

  float projMET_tk = 0.0;
  float deltaPhi1_tk = fabs(p1.DeltaPhi(p3tkMet));
  float deltaPhi2_tk = fabs(p2.DeltaPhi(p3tkMet));
  float deltaphi_tk = TMath::Min(deltaPhi1_tk,deltaPhi2_tk);
  if(deltaphi_tk<TMath::Pi()/2.) projMET_tk = p3tkMet.Mag() * sin(deltaphi_tk);
  else projMET_tk = p3tkMet.Mag();

  return TMath::Min(projMET_pf,projMET_tk);

}


/// specific for HWW that has multiple channels with different HLT requirements
bool LeptonPlusFakeMLSelection_fullME::reloadTriggerMask(int runN)
{
  std::vector<int> triggerMask;

  // load the triggers required for ME
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersME.begin();fIter!=requiredTriggersME.end();++fIter) {
    std::string pathName = getHLTPathForRun(runN,*fIter);
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(pathName) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_requiredTriggersME = triggerMask;

  // load the triggers NOT required for ME
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=notRequiredTriggersME.begin();fIter!=notRequiredTriggersME.end();++fIter) {
    std::string pathName = getHLTPathForRun(runN,*fIter);
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(pathName) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_notRequiredTriggersME = triggerMask;
}

bool LeptonPlusFakeMLSelection_fullME::hasPassedHLT() {

  Utils anaUtils;
  bool required    = anaUtils.getTriggersOR(m_requiredTriggersME, firedTrg);
  bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersME, firedTrg);
  return (required && !notRequired);
}

void LeptonPlusFakeMLSelection_fullME::setRequiredTriggers(const std::vector<std::string>& reqTriggers) {
  requiredTriggersME=reqTriggers;
}

void LeptonPlusFakeMLSelection_fullME::setNotRequiredTriggers(const std::vector<std::string>& reqTriggers) {
  notRequiredTriggersME=reqTriggers;
}

bool LeptonPlusFakeMLSelection_fullME::isPFIsolatedMuon(int muonIndex) {
  float eta = etaMuon[muonIndex];
  float pt = GetPt(pxMuon[muonIndex],pyMuon[muonIndex]);
  float iso = pfCombinedIsoMuon[muonIndex]/pt;

  if( pt>=20. && fabs(eta)<1.479 ) return (iso < 0.13);
  if( pt>=20. && fabs(eta)>=1.479 ) return (iso < 0.09);
  if( pt<20. && fabs(eta)<1.479 ) return (iso < 0.06);
  if( pt<20. && fabs(eta)>=1.479 ) return (iso < 0.05);
  return true;
}

