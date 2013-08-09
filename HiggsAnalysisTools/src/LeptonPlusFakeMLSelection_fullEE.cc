#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/LeptonPlusFakeMLSelection_fullEE.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "CommonTools/include/LeptonIdBits.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <TTree.h>

using namespace bits;

LeptonPlusFakeMLSelection_fullEE::LeptonPlusFakeMLSelection_fullEE(TTree *tree) 
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
  std::string fileCutsEE     = higgsConfigDirMass + "2e2nuCuts.txt";
  std::string fileSwitchesEE = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionEE.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE FP");

  // taking the selection
  _selectionEE = CutBasedHiggsSelectionEE.GetSelection();  

  //  extra selection efficiencies  - to be put here not to pass the full list of leptons to the preselection class
  _selectionEE->addCut("etaElectronAcc");    
  _selectionEE->addCut("ptElectronAcc");
  _selectionEE->addCut("etaMuonAcc");
  _selectionEE->addCut("ptMuonAcc");
  _selectionEE->addCut("etUncorrJetAcc");  
  _selectionEE->addSwitch("apply_kFactor");   
  _selectionEE->addSwitch("isData");
  _selectionEE->addSwitch("goodRunLS");
  _selectionEE->addSwitch("asymmetricID");
  _selectionEE->addSwitch("applyBDTEleID");
  _selectionEE->addStringParameter("electronIDType");
  _selectionEE->addStringParameter("electronIDTypeLow");  

  // cut based electron id or likelihood 
  TString selectionString(_selectionEE->getStringParameter("electronIDType"));
  if (!_selectionEE->getSwitch("asymmetricID")) 
    cout << "=== CONFIGURING " << selectionString << " CUT BASED SYMMETRIC ELECTRON ID ===" << endl;
  EgammaCutBasedID.ConfigureNoClass("config/higgs/electronId/"+selectionString);
  EgammaCutBasedID.ConfigureEcalCleaner("config/higgs/electronId/");
  
  if (_selectionEE->getSwitch("asymmetricID")) {
    TString selectionStringLow (_selectionEE->getStringParameter("electronIDTypeLow"));
    cout << "=== CONFIGURING "  << selectionStringLow << " and " 
	 << selectionString << " for CUT BASED ASYMMETRIC ELECTRON ID ===" << endl;
    EgammaCutBasedIDLow.ConfigureNoClass("config/higgs/electronId/"+selectionStringLow);
    EgammaCutBasedIDLow.ConfigureEcalCleaner("config/higgs/electronId/");
  }
  
  // configuring electron likelihood
  TFile *fileLH = TFile::Open("pdfs_MC.root");
  TDirectory *EB0lt15dir = fileLH->GetDirectory("/");
  TDirectory *EB1lt15dir = fileLH->GetDirectory("/");
  TDirectory *EElt15dir = fileLH->GetDirectory("/");
  TDirectory *EB0gt15dir = fileLH->GetDirectory("/");
  TDirectory *EB1gt15dir = fileLH->GetDirectory("/");
  TDirectory *EEgt15dir = fileLH->GetDirectory("/");
  LikelihoodSwitches defaultSwitches;
  defaultSwitches.m_useFBrem = true;
  defaultSwitches.m_useEoverP = false;
  defaultSwitches.m_useSigmaPhiPhi = true;
  defaultSwitches.m_useHoverE = false;        
  defaultSwitches.m_useOneOverEMinusOneOverP = true;
  LH = new ElectronLikelihood(&(*EB0lt15dir), &(*EB1lt15dir), &(*EElt15dir), &(*EB0gt15dir), &(*EB1gt15dir), &(*EEgt15dir), defaultSwitches, std::string("class"),std::string("class"),true,true);
  
  // configuring electron ID BDT
  if(_selectionEE->getSwitch("applyBDTEleID")) m_useBDTEleID = true;
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
  std::cout << "[GoodRunLS]::goodRunLS is " << _selectionEE->getSwitch("goodRunLS") << " isData is " <<  _selectionEE->getSwitch("isData") << std::endl;

  // To read good run list!   // chiara
  if (_selectionEE->getSwitch("goodRunLS") && _selectionEE->getSwitch("isData")) {
    // std::string goodRunJsonFile = "config/json/2011a.json"; 
    // std::string goodRunJsonFile = "config/json/2011b.json"; 
    std::string goodRunJsonFile = "config/json/hww.Full2011.json";
    setJsonGoodRunList(goodRunJsonFile);
    fillRunLSMap();
  }

  // kinematics
  m_p3PFMET = new TVector3(0.,0.,0.);
  for(int theChannel=0; theChannel<1; theChannel++) {
    m_p4LeptonPlus[theChannel]  = new TLorentzVector(0.,0.,0.,0.);
    m_p4LeptonMinus[theChannel] = new TLorentzVector(0.,0.,0.,0.);
    m_jetsSum[theChannel]       = new TLorentzVector(0.,0.,0.,0.);
    m_uncorrJetsSum[theChannel] = new TLorentzVector(0.,0.,0.,0.);
  }    

  // b-veto event variables
  m_maxDxyEvt = 0.0;
  m_maxDszEvt = 0.0;
}

LeptonPlusFakeMLSelection_fullEE::~LeptonPlusFakeMLSelection_fullEE(){

  for(int theChannel=0; theChannel<1; theChannel++) {  
    delete m_p4LeptonPlus[theChannel];
    delete m_p4LeptonMinus[theChannel];
  }

  delete _selectionEE;

  myOutTreeEE -> save();
}

void LeptonPlusFakeMLSelection_fullEE::initialiseFakeBinning() {

  m_minFakePt[0] = 10.;   m_maxFakePt[0] = 15.;
  m_minFakePt[1] = 15.;   m_maxFakePt[1] = 20.;
  m_minFakePt[2] = 20.;   m_maxFakePt[2] = 25.;
  m_minFakePt[3] = 25.;   m_maxFakePt[3] = 30.;
  m_minFakePt[4] = 30.;   m_maxFakePt[4] = 10000.;
}

void LeptonPlusFakeMLSelection_fullEE::initialiseFakeEtaBinning() {

  m_minFakeEta[0] = 0.;    m_maxFakeEta[0] = 1.;
  m_minFakeEta[1] = 1.;    m_maxFakeEta[1] = 1.479;
  m_minFakeEta[2] = 1.479; m_maxFakeEta[2] = 2.;
  m_minFakeEta[3] = 2.;    m_maxFakeEta[3] = 2.5;
}

// fake for eleID Bdt, ET>15 - 2011A+2011B average analysis
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRate15() {

  m15_fakeRateEB1[0] = 0.109;
  m15_fakeRateEB1[1] = 0.103;
  m15_fakeRateEB1[2] = 0.123;
  m15_fakeRateEB1[3] = 0.099;
  m15_fakeRateEB1[4] = 0.102;

  m15_fakeRateEB1_err[0] = 0.007;
  m15_fakeRateEB1_err[1] = 0.008;
  m15_fakeRateEB1_err[2] = 0.010;
  m15_fakeRateEB1_err[3] = 0.013;
  m15_fakeRateEB1_err[4] = 0.018;

  m15_fakeRateEB2[0] = 0.056;
  m15_fakeRateEB2[1] = 0.074;
  m15_fakeRateEB2[2] = 0.093;
  m15_fakeRateEB2[3] = 0.070;
  m15_fakeRateEB2[4] = 0.058;

  m15_fakeRateEB2_err[0] = 0.006;
  m15_fakeRateEB2_err[1] = 0.009;
  m15_fakeRateEB2_err[2] = 0.012;
  m15_fakeRateEB2_err[3] = 0.015;
  m15_fakeRateEB2_err[4] = 0.019;

  m15_fakeRateEE1[0] = 0.028;
  m15_fakeRateEE1[1] = 0.028;
  m15_fakeRateEE1[2] = 0.085;
  m15_fakeRateEE1[3] = 0.059;
  m15_fakeRateEE1[4] = 0.071;

  m15_fakeRateEE1_err[0] = 0.005;
  m15_fakeRateEE1_err[1] = 0.007;
  m15_fakeRateEE1_err[2] = 0.011;
  m15_fakeRateEE1_err[3] = 0.013;
  m15_fakeRateEE1_err[4] = 0.019;

  m15_fakeRateEE2[0] = 0.035;
  m15_fakeRateEE2[1] = 0.048;
  m15_fakeRateEE2[2] = 0.069;
  m15_fakeRateEE2[3] = 0.080;
  m15_fakeRateEE2[4] = 0.073;

  m15_fakeRateEE2_err[0] = 0.006;
  m15_fakeRateEE2_err[1] = 0.009;
  m15_fakeRateEE2_err[2] = 0.010;
  m15_fakeRateEE2_err[3] = 0.014;
  m15_fakeRateEE2_err[4] = 0.019;
}

// fake for eleID Bdt, ET>35 - 2011A+2011B average analysis
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRate35() {

  m35_fakeRateEB1[0] = 0.066;
  m35_fakeRateEB1[1] = 0.057;
  m35_fakeRateEB1[2] = 0.085;
  m35_fakeRateEB1[3] = 0.085;
  m35_fakeRateEB1[4] = 0.085;

  m35_fakeRateEB1_err[0] = 0.015;
  m35_fakeRateEB1_err[1] = 0.009;
  m35_fakeRateEB1_err[2] = 0.009;
  m35_fakeRateEB1_err[3] = 0.012;
  m35_fakeRateEB1_err[4] = 0.017;

  m35_fakeRateEB2[0] = 0.040;
  m35_fakeRateEB2[1] = 0.054;
  m35_fakeRateEB2[2] = 0.064;
  m35_fakeRateEB2[3] = 0.067;
  m35_fakeRateEB2[4] = 0.059;

  m35_fakeRateEB2_err[0] = 0.012;
  m35_fakeRateEB2_err[1] = 0.010;
  m35_fakeRateEB2_err[2] = 0.011;
  m35_fakeRateEB2_err[3] = 0.015;
  m35_fakeRateEB2_err[4] = 0.019;

  m35_fakeRateEE1[0] = 0.016;
  m35_fakeRateEE1[1] = 0.018;
  m35_fakeRateEE1[2] = 0.057;
  m35_fakeRateEE1[3] = 0.052;
  m35_fakeRateEE1[4] = 0.072;

  m35_fakeRateEE1_err[0] = 0.009;
  m35_fakeRateEE1_err[1] = 0.007;
  m35_fakeRateEE1_err[2] = 0.010;
  m35_fakeRateEE1_err[3] = 0.012;
  m35_fakeRateEE1_err[4] = 0.019;

  m35_fakeRateEE2[0] = 0.023;
  m35_fakeRateEE2[1] = 0.041;
  m35_fakeRateEE2[2] = 0.046;
  m35_fakeRateEE2[3] = 0.070;
  m35_fakeRateEE2[4] = 0.068;

  m35_fakeRateEE2_err[0] = 0.013;
  m35_fakeRateEE2_err[1] = 0.013;
  m35_fakeRateEE2_err[2] = 0.009;
  m35_fakeRateEE2_err[3] = 0.015;
  m35_fakeRateEE2_err[4] = 0.019;
}

/*
// fake for eleID Bdt, ET>35 - 2011A analysis
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRate35() {

  m35_fakeRateEB1[0] = 0.083;
  m35_fakeRateEB1[1] = 0.053;
  m35_fakeRateEB1[2] = 0.071;
  m35_fakeRateEB1[3] = 0.062;
  m35_fakeRateEB1[4] = 0.048;

  m35_fakeRateEB1_err[0] = 0.021;
  m35_fakeRateEB1_err[1] = 0.010;
  m35_fakeRateEB1_err[2] = 0.010;
  m35_fakeRateEB1_err[3] = 0.012;
  m35_fakeRateEB1_err[4] = 0.015;

  m35_fakeRateEB2[0] = 0.047;
  m35_fakeRateEB2[1] = 0.055;
  m35_fakeRateEB2[2] = 0.056;
  m35_fakeRateEB2[3] = 0.062;
  m35_fakeRateEB2[4] = 0.020;

  m35_fakeRateEB2_err[0] = 0.016;
  m35_fakeRateEB2_err[1] = 0.012;
  m35_fakeRateEB2_err[2] = 0.012;
  m35_fakeRateEB2_err[3] = 0.017;
  m35_fakeRateEB2_err[4] = 0.014;

  m35_fakeRateEE1[0] = 0.008;
  m35_fakeRateEE1[1] = 0.006;
  m35_fakeRateEE1[2] = 0.059;
  m35_fakeRateEE1[3] = 0.041;
  m35_fakeRateEE1[4] = 0.075;

  m35_fakeRateEE1_err[0] = 0.008;
  m35_fakeRateEE1_err[1] = 0.005;
  m35_fakeRateEE1_err[2] = 0.012;
  m35_fakeRateEE1_err[3] = 0.013;
  m35_fakeRateEE1_err[4] = 0.024;

  m35_fakeRateEE2[0] = 0.032;
  m35_fakeRateEE2[1] = 0.038;
  m35_fakeRateEE2[2] = 0.045;
  m35_fakeRateEE2[3] = 0.061;
  m35_fakeRateEE2[4] = 0.067;

  m35_fakeRateEE2_err[0] = 0.018;
  m35_fakeRateEE2_err[1] = 0.013;
  m35_fakeRateEE2_err[2] = 0.011;
  m35_fakeRateEE2_err[3] = 0.016;
  m35_fakeRateEE2_err[4] = 0.022;
}
*/

/*
// fake for eleID Bdt, ET>35 - 2011B analysis
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRate35() {

  m35_fakeRateEB1[0] = 0.031;
  m35_fakeRateEB1[1] = 0.045;
  m35_fakeRateEB1[2] = 0.097;
  m35_fakeRateEB1[3] = 0.090;
  m35_fakeRateEB1[4] = 0.051;

  m35_fakeRateEB1_err[0] = 0.018;
  m35_fakeRateEB1_err[1] = 0.015;
  m35_fakeRateEB1_err[2] = 0.020;
  m35_fakeRateEB1_err[3] = 0.026;
  m35_fakeRateEB1_err[4] = 0.028;

  m35_fakeRateEB2[0] = 0.024;
  m35_fakeRateEB2[1] = 0.042;
  m35_fakeRateEB2[2] = 0.070;
  m35_fakeRateEB2[3] = 0.041;
  m35_fakeRateEB2[4] = 0.074;

  m35_fakeRateEB2_err[0] = 0.017;
  m35_fakeRateEB2_err[1] = 0.018;
  m35_fakeRateEB2_err[2] = 0.023;
  m35_fakeRateEB2_err[3] = 0.025;
  m35_fakeRateEB2_err[4] = 0.039;

  m35_fakeRateEE1[0] = 0.029;
  m35_fakeRateEE1[1] = 0.051;
  m35_fakeRateEE1[2] = 0.036;
  m35_fakeRateEE1[3] = 0.058;
  m35_fakeRateEE1[4] = 0.031;

  m35_fakeRateEE1_err[0] = 0.021;
  m35_fakeRateEE1_err[1] = 0.026;
  m35_fakeRateEE1_err[2] = 0.017;
  m35_fakeRateEE1_err[3] = 0.025;
  m35_fakeRateEE1_err[4] = 0.022;

  m35_fakeRateEE2[0] = 0.052;        // not enough stat: take the second bin
  m35_fakeRateEE2[1] = 0.052;  
  m35_fakeRateEE2[2] = 0.019;
  m35_fakeRateEE2[3] = 0.080;
  m35_fakeRateEE2[4] = 0.033;

  m35_fakeRateEE2_err[0] = 0.038;    // not enough stat: take the second bin
  m35_fakeRateEE2_err[1] = 0.038;
  m35_fakeRateEE2_err[2] = 0.015;
  m35_fakeRateEE2_err[3] = 0.031;
  m35_fakeRateEE2_err[4] = 0.029;
}
*/

// fake for eleID Bdt, ET>50 - 2011A+2011B average analysis
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRate50() {

  m50_fakeRateEB1[0] = 0.063;
  m50_fakeRateEB1[1] = 0.026;
  m50_fakeRateEB1[2] = 0.061;
  m50_fakeRateEB1[3] = 0.052;
  m50_fakeRateEB1[4] = 0.031;

  m50_fakeRateEB1_err[0] = 0.036;
  m50_fakeRateEB1_err[1] = 0.012;
  m50_fakeRateEB1_err[2] = 0.015;
  m50_fakeRateEB1_err[3] = 0.014;
  m50_fakeRateEB1_err[4] = 0.012;

  m50_fakeRateEB2[0] = 0.042;
  m50_fakeRateEB2[1] = 0.034;
  m50_fakeRateEB2[2] = 0.060;
  m50_fakeRateEB2[3] = 0.024;
  m50_fakeRateEB2[4] = 0.025;

  m50_fakeRateEB2_err[0] = 0.029;
  m50_fakeRateEB2_err[1] = 0.016;
  m50_fakeRateEB2_err[2] = 0.018;
  m50_fakeRateEB2_err[3] = 0.012;
  m50_fakeRateEB2_err[4] = 0.015;

  m50_fakeRateEE1[0] = 0.008;
  m50_fakeRateEE1[1] = 0.008; 
  m50_fakeRateEE1[2] = 0.054;
  m50_fakeRateEE1[3] = 0.033;
  m50_fakeRateEE1[4] = 0.044;
  
  m50_fakeRateEE1_err[0] = 0.010;
  m50_fakeRateEE1_err[1] = 0.010;
  m50_fakeRateEE1_err[2] = 0.018;
  m50_fakeRateEE1_err[3] = 0.015;
  m50_fakeRateEE1_err[4] = 0.017;

  m50_fakeRateEE2[0] = 0.034;
  m50_fakeRateEE2[1] = 0.041;
  m50_fakeRateEE2[2] = 0.051;
  m50_fakeRateEE2[3] = 0.044;
  m50_fakeRateEE2[4] = 0.039;

  m50_fakeRateEE2_err[0] = 0.034;
  m50_fakeRateEE2_err[1] = 0.024;
  m50_fakeRateEE2_err[2] = 0.018;
  m50_fakeRateEE2_err[3] = 0.016;
  m50_fakeRateEE2_err[4] = 0.017;
}

// fake for eleID Bdt, QCD for closure test
void LeptonPlusFakeMLSelection_fullEE::initialiseFakeRateQCD() {

  mQCD_fakeRateEB1[0] = 0.0082;
  mQCD_fakeRateEB1[1] = 0.0040;
  mQCD_fakeRateEB1[2] = 0.0049;
  mQCD_fakeRateEB1[3] = 0.0048;
  mQCD_fakeRateEB1[4] = 0.0051;

  mQCD_fakeRateEB1_err[0] = 0.0002;
  mQCD_fakeRateEB1_err[1] = 0.0002;
  mQCD_fakeRateEB1_err[2] = 0.0002;
  mQCD_fakeRateEB1_err[3] = 0.0003;
  mQCD_fakeRateEB1_err[4] = 0.0004;

  mQCD_fakeRateEB2[0] = 0.0061; 
  mQCD_fakeRateEB2[1] = 0.0037; 
  mQCD_fakeRateEB2[2] = 0.0065; 
  mQCD_fakeRateEB2[3] = 0.0061; 
  mQCD_fakeRateEB2[4] = 0.0064; 

  mQCD_fakeRateEB2_err[0] = 0.0002;
  mQCD_fakeRateEB2_err[1] = 0.0002;
  mQCD_fakeRateEB2_err[2] = 0.0004;
  mQCD_fakeRateEB2_err[3] = 0.0005;
  mQCD_fakeRateEB2_err[4] = 0.0008;

  mQCD_fakeRateEE1[0] = 0.0030;
  mQCD_fakeRateEE1[1] = 0.0017;
  mQCD_fakeRateEE1[2] = 0.0032;
  mQCD_fakeRateEE1[3] = 0.0032; 
  mQCD_fakeRateEE1[4] = 0.0031;
  
  mQCD_fakeRateEE1_err[0] = 0.0001;
  mQCD_fakeRateEE1_err[1] = 0.0001;
  mQCD_fakeRateEE1_err[2] = 0.0002;
  mQCD_fakeRateEE1_err[3] = 0.0003;
  mQCD_fakeRateEE1_err[4] = 0.0005;

  mQCD_fakeRateEE2[0] = 0.0018; 
  mQCD_fakeRateEE2[1] = 0.0009;
  mQCD_fakeRateEE2[2] = 0.0022;
  mQCD_fakeRateEE2[3] = 0.0023;
  mQCD_fakeRateEE2[4] = 0.0026;
  
  mQCD_fakeRateEE2_err[0] = 0.0001;
  mQCD_fakeRateEE2_err[1] = 0.0001;
  mQCD_fakeRateEE2_err[2] = 0.0002;
  mQCD_fakeRateEE2_err[3] = 0.0002;
  mQCD_fakeRateEE2_err[4] = 0.0004;
}

// prompt rate from eleID Bdt - for 2011A+B average analysis, split in #vtx (Clara, Nov 8)
void LeptonPlusFakeMLSelection_fullEE::initialisePromptRate(int numVtx) { 
  
  // binning                                      
  m_minPromptPt[0] = 10.;   m_maxPromptPt[0] = 15.;
  m_minPromptPt[1] = 15.;   m_maxPromptPt[1] = 20.;
  m_minPromptPt[2] = 20.;   m_maxPromptPt[2] = 25.;
  m_minPromptPt[3] = 25.;   m_maxPromptPt[3] = 50.;
  m_minPromptPt[4] = 50.;   m_maxPromptPt[4] = 10000.;

  // prompt in the barrel
  if (numVtx>=1 && numVtx<10) {
    m_promptRateEB[0] = 0.589;
    m_promptRateEB[1] = 0.673;
    m_promptRateEB[2] = 0.778;
    m_promptRateEB[3] = 0.874;
    m_promptRateEB[4] = 0.906;
    
    m_promptRateEB_err[0] = 0.0099;
    m_promptRateEB_err[1] = 0.006;
    m_promptRateEB_err[2] = 0.003;
    m_promptRateEB_err[3] = 0.063;
    m_promptRateEB_err[4] = 0.001;
  } else {
    m_promptRateEB[0] = 0.528;
    m_promptRateEB[1] = 0.616;
    m_promptRateEB[2] = 0.739;
    m_promptRateEB[3] = 0.862;
    m_promptRateEB[4] = 0.901;
    
    m_promptRateEB_err[0] = 0.046;
    m_promptRateEB_err[1] = 0.014;
    m_promptRateEB_err[2] = 0.008;
    m_promptRateEB_err[3] = 0.001;
    m_promptRateEB_err[4] = 0.003;
  }

  // prompt in the crack
  if (numVtx>=1 && numVtx<10) {
    m_promptRateCr[0] = 0.465;
    m_promptRateCr[1] = 0.540;
    m_promptRateCr[2] = 0.712;
    m_promptRateCr[3] = 0.823;
    m_promptRateCr[4] = 0.867;
    
    m_promptRateCr_err[0] = 0.043;
    m_promptRateCr_err[1] = 0.043;
    m_promptRateCr_err[2] = 0.017;
    m_promptRateCr_err[3] = 0.090;
    m_promptRateCr_err[4] = 0.070;
  } else {
    m_promptRateCr[0] = 0.271;
    m_promptRateCr[1] = 0.479;
    m_promptRateCr[2] = 0.658;
    m_promptRateCr[3] = 0.790;
    m_promptRateCr[4] = 0.863;
    
    m_promptRateCr_err[0] = 0.081;
    m_promptRateCr_err[1] = 0.088;
    m_promptRateCr_err[2] = 0.045;
    m_promptRateCr_err[3] = 0.001;
    m_promptRateCr_err[4] = 0.018;
  }

  // prompt in the endcap
  if (numVtx>=1 && numVtx<10) {
    m_promptRateEE[0] = 0.371;
    m_promptRateEE[1] = 0.449;
    m_promptRateEE[2] = 0.675;
    m_promptRateEE[3] = 0.779;
    m_promptRateEE[4] = 0.838;
    
    m_promptRateEE_err[0] = 0.192;
    m_promptRateEE_err[1] = 0.009;
    m_promptRateEE_err[2] = 0.006;
    m_promptRateEE_err[3] = 0.002;
    m_promptRateEE_err[4] = 0.002;
  } else {
    m_promptRateEE[0] = 0.305;
    m_promptRateEE[1] = 0.350;
    m_promptRateEE[2] = 0.588;
    m_promptRateEE[3] = 0.722;
    m_promptRateEE[4] = 0.793;
    
    m_promptRateEE_err[0] = 0.042;
    m_promptRateEE_err[1] = 0.022;
    m_promptRateEE_err[2] = 0.015;
    m_promptRateEE_err[3] = 0.002;
    m_promptRateEE_err[4] = 0.009;
  }
}

/*
// prompt rate from eleID Bdt - for 2011A+B average analysis, not split in #vtx (Clara, Nov 8)
void LeptonPlusFakeMLSelection_fullEE::initialisePromptRate(int numVtx) { 
  
  // binning                                      
  m_minPromptPt[0] = 10.;   m_maxPromptPt[0] = 15.;
  m_minPromptPt[1] = 15.;   m_maxPromptPt[1] = 20.;
  m_minPromptPt[2] = 20.;   m_maxPromptPt[2] = 25.;
  m_minPromptPt[3] = 25.;   m_maxPromptPt[3] = 50.;
  m_minPromptPt[4] = 50.;   m_maxPromptPt[4] = 10000.;

  // prompt in the barrel
  m_promptRateEB[0] = 0.584;
  m_promptRateEB[1] = 0.667;
  m_promptRateEB[2] = 0.773;
  m_promptRateEB[3] = 0.872;
  m_promptRateEB[4] = 0.905;
  
  m_promptRateEB_err[0] = 0.009;
  m_promptRateEB_err[1] = 0.005;
  m_promptRateEB_err[2] = 0.003;
  m_promptRateEB_err[3] = 0.001;
  m_promptRateEB_err[4] = 0.001;

  // prompt in the crack
  m_promptRateCr[0] = 0.456;
  m_promptRateCr[1] = 0.527;
  m_promptRateCr[2] = 0.703;
  m_promptRateCr[3] = 0.819;
  m_promptRateCr[4] = 0.868;
  
  m_promptRateCr_err[0] = 0.049;
  m_promptRateCr_err[1] = 0.035;
  m_promptRateCr_err[2] = 0.017;
  m_promptRateCr_err[3] = 0.092;
  m_promptRateCr_err[4] = 0.007;

  // prompt in the endcap
  m_promptRateEE[0] = 0.367;
  m_promptRateEE[1] = 0.441;
  m_promptRateEE[2] = 0.666;
  m_promptRateEE[3] = 0.771;
  m_promptRateEE[4] = 0.832;
  
  m_promptRateEE_err[0] = 0.020;
  m_promptRateEE_err[1] = 0.008;
  m_promptRateEE_err[2] = 0.006;
  m_promptRateEE_err[3] = 0.001;
  m_promptRateEE_err[4] = 0.002;
}
*/

/*
// prompt rate from eleID Bdt - for 2011A analysis (Clara, for 1st Nov. reload) 
void LeptonPlusFakeMLSelection_fullEE::initialisePromptRate(int numVtx) { 
  
  // binning                                      
  m_minPromptPt[0] = 10.;   m_maxPromptPt[0] = 15.;
  m_minPromptPt[1] = 15.;   m_maxPromptPt[1] = 20.;
  m_minPromptPt[2] = 20.;   m_maxPromptPt[2] = 25.;
  m_minPromptPt[3] = 25.;   m_maxPromptPt[3] = 50.;
  m_minPromptPt[4] = 50.;   m_maxPromptPt[4] = 10000.;

  // prompt in the barrel
  m_promptRateEB[0] = 0.609;
  m_promptRateEB[1] = 0.683;
  m_promptRateEB[2] = 0.785;
  m_promptRateEB[3] = 0.876;
  m_promptRateEB[4] = 0.906;
  
  m_promptRateEB_err[0] = 0.011;
  m_promptRateEB_err[1] = 0.006;
  m_promptRateEB_err[2] = 0.004;
  m_promptRateEB_err[3] = 0.001;
  m_promptRateEB_err[4] = 0.001;

  // prompt in the crack
  m_promptRateCr[0] = 0.437;
  m_promptRateCr[1] = 0.520;
  m_promptRateCr[2] = 0.724;
  m_promptRateCr[3] = 0.824;
  m_promptRateCr[4] = 0.879;
  
  m_promptRateCr_err[0] = 0.126;
  m_promptRateCr_err[1] = 0.036;
  m_promptRateCr_err[2] = 0.019;
  m_promptRateCr_err[3] = 0.003;
  m_promptRateCr_err[4] = 0.010;

  // prompt in the endcap
  m_promptRateEE[0] = 0.378;
  m_promptRateEE[1] = 0.458;
  m_promptRateEE[2] = 0.682;
  m_promptRateEE[3] = 0.787;
  m_promptRateEE[4] = 0.843;
  
  m_promptRateEE_err[0] = 0.014;
  m_promptRateEE_err[1] = 0.009;
  m_promptRateEE_err[2] = 0.009;
  m_promptRateEE_err[3] = 0.002;
  m_promptRateEE_err[4] = 0.003;
}
*/

/*
// prompt rate from eleID Bdt - partial 2011B analysis (Clara, for 1st Nov. reload) 
void LeptonPlusFakeMLSelection_fullEE::initialisePromptRate(int numVtx) { 
  
  // binning                                      
  m_minPromptPt[0] = 10.;   m_maxPromptPt[0] = 15.;
  m_minPromptPt[1] = 15.;   m_maxPromptPt[1] = 20.;
  m_minPromptPt[2] = 20.;   m_maxPromptPt[2] = 25.;
  m_minPromptPt[3] = 25.;   m_maxPromptPt[3] = 50.;
  m_minPromptPt[4] = 50.;   m_maxPromptPt[4] = 10000.;

  // prompt in the barrel
  m_promptRateEB[0] = 0.543;
  m_promptRateEB[1] = 0.640;
  m_promptRateEB[2] = 0.752;
  m_promptRateEB[3] = 0.867;
  m_promptRateEB[4] = 0.901;
  
  m_promptRateEB_err[0] = 0.016;
  m_promptRateEB_err[1] = 0.009;
  m_promptRateEB_err[2] = 0.005;
  m_promptRateEB_err[3] = 0.001;
  m_promptRateEB_err[4] = 0.002;

  // prompt in the crack
  m_promptRateCr[0] = 0.415;
  m_promptRateCr[1] = 0.583;
  m_promptRateCr[2] = 0.693;
  m_promptRateCr[3] = 0.813;
  m_promptRateCr[4] = 0.851;
  
  m_promptRateCr_err[0] = 0.063;
  m_promptRateCr_err[1] = 0.057;
  m_promptRateCr_err[2] = 0.030;
  m_promptRateCr_err[3] = 0.005;
  m_promptRateCr_err[4] = 0.013;

  // prompt in the endcap
  m_promptRateEE[0] = 0.309;
  m_promptRateEE[1] = 0.376;
  m_promptRateEE[2] = 0.619;
  m_promptRateEE[3] = 0.745;
  m_promptRateEE[4] = 0.817;
  
  m_promptRateEE_err[0] = 0.025;
  m_promptRateEE_err[1] = 0.014;
  m_promptRateEE_err[2] = 0.010;
  m_promptRateEE_err[3] = 0.001;
  m_promptRateEE_err[4] = 0.004;
}
*/

void LeptonPlusFakeMLSelection_fullEE::Loop() {

  _verbose=false;
  if(fChain == 0) return;

  // initializations
  initialiseFakeBinning();
  initialiseFakeEtaBinning();
  initialiseFakeRate15();
  initialiseFakeRate35();
  initialiseFakeRate50();
  initialiseFakeRateQCD();
  
  // kinematics reduced tree
  std::string reducedTreeNameEE = _datasetName+"-datasetEE.root";    
  myOutTreeEE = new RedHiggsTree(reducedTreeNameEE.c_str());

  if ( _selectionEE->getSwitch("isData")) myOutTreeEE->addRunInfos();
  myOutTreeEE->addMLVars();
  myOutTreeEE->addLatinos();
  myOutTreeEE->addKinematics();
  myOutTreeEE->addFake();
  myOutTreeEE->addRazor();

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

    // this depend on the number of PV...
    initialisePromptRate(nPV);

    // weight
    float tmpWeight = 1.;

    // pu weight
    float nputosave[3];
    if( !_selectionEE->getSwitch("isData") ) {
      for(int i=0; i<3; i++) nputosave[i] = float(nPU[i]);
    } else {
      for(int i=0; i<3; i++) nputosave[i] = -1.;
    }

    // Good Run selection
    if (_selectionEE->getSwitch("isData") && _selectionEE->getSwitch("goodRunLS") && !isGoodRunLS()) {
      if ( lastRun!= runNumber || lastLumi != lumiBlock) {
        lastRun = runNumber;
        lastLumi = lumiBlock;
        std::cout << "[GoodRunLS]::Run " << lastRun << " LS " << lastLumi << " is rejected" << std::endl;
      }
      continue;
    }
    if (_selectionEE->getSwitch("isData") && _selectionEE->getSwitch("goodRunLS") && ( lastRun!= runNumber || lastLumi != lumiBlock) ) {
      lastRun = runNumber;
      lastLumi = lumiBlock;
      std::cout << "[GoodRunLS]::Run " << lastRun << " LS " << lastLumi << " is OK" << std::endl;
    }
    
    
    // IMPORTANT: FOR DATA RELOAD THE TRIGGER MASK PER FILE WHICH IS SUPPOSED TO CONTAIN UNIFORM CONDITIONS X FILE
    reloadTriggerMask(runNumber);
    bool passedHLT[1];
    passedHLT[ee] = hasPassedHLT();


    // -------------------------------------------------------------
    // vertex selection - we only consider the first vertex of the list ( = highest sumPT^2)
    bool isGoodVertex = true;
    if (nPV<1) isGoodVertex = false;
    float rhoVtx = sqrt(PVxPV[0]*PVxPV[0] + PVyPV[0]*PVyPV[0]);
    if ( isFakePV[0] )       isGoodVertex = false;
    if ( ndofPV[0]<4 )       isGoodVertex = false;
    if ( fabs(PVzPV[0])>24.) isGoodVertex = false;
    if ( rhoVtx>2 )          isGoodVertex = false; 
    

    
    // ----------------------------------------------------------------------------
    // get the best electrons passing the denominator definition
    // ==> tu be used to select ALL the possible channels at the beginning only
    std::pair<int,int> thePreElectrons = getBestElectronPair_denominator();
    thePreElectron = thePreElectrons.second;
    thePrePositron = thePreElectrons.first;

    // reconstructed channel
    m_channel[ee] = false;     

    // at this level the SELECTED channel should have pT > 10 and > 20. So far, at least 2 leptons with pT >20 and 10 in the event
    if (thePreElectron > -1 && thePrePositron > -1) {
      float thisMaxPt = TMath::Max(GetPt(pxEle[thePreElectron],pyEle[thePreElectron]),GetPt(pxEle[thePrePositron],pyEle[thePrePositron]));
      float thisMinPt = TMath::Min(GetPt(pxEle[thePreElectron],pyEle[thePreElectron]),GetPt(pxEle[thePrePositron],pyEle[thePrePositron]));
      if (isGoodVertex && thisMaxPt>20 && thisMinPt>10) m_channel[ee] = true;    // fixme: hardcoded
    }

    if (_verbose) {
      std::cout << "nEle = "   << nEle << "\tnMuon = " << nMuon << std::endl;
      std::cout << "indices: " << thePreElectron << " " << thePrePositron << std::endl;
      std::cout << "chargeEle = " << chargeEle[thePreElectron] << "\tchargePos = " << chargeEle[thePrePositron] << std::endl;
      std::cout << "ee = " << m_channel[ee] << std::endl;
    }
    


    // -------------------------------------------------------------
    // EE candidates: preparing vectors of candidates and selecting the two highest pT ele- and ele+ after each step - to check the 20 GeV cut after 

    // eleID, for electrons passing the denominator
    std::pair<int,int> theBestIdEle = getBestElectronPair_id(_denomEleAll);   
    
    // isolation, for identified electrons
    std::pair<int,int> theBestIsolEle = getBestElectronPair_isol(_idEleAll); 

    // conversion rejection, for isolated electrons
    std::pair<int,int> theBestConvEle = getBestElectronPair_conv(_isolEleAll);     

    // transverse impact parameter, for electrons passing conversion rejection
    std::pair<int,int> theBestIpEle = getBestElectronPair_ip(_convEleAll);     

    // the two highest pT electrons at this point are those I use for my analysis since they passed the full lepton selection
    int thePositron = theBestIpEle.first;    
    int theElectron = theBestIpEle.second;

    // 0 - 1 - 2 tight candidates
    bool is0tight = false;
    bool is1tight = false;
    bool is2tight = false;

    // here I have two candidates with opposite sign passing the tight selection: is N2T
    if (thePositron>-1 && theElectron>-1) {  
      float ptPositron = GetPt(pxEle[thePositron],pyEle[thePositron]);
      float ptElectron = GetPt(pxEle[theElectron],pyEle[theElectron]);
      
      if (ptPositron>ptElectron) { 
	theReal = thePositron;    
	theFake = theElectron;
      }
      else { 
	theReal = theElectron;
	theFake = thePositron;
      }
      is2tight = true;
    } 

    // here I have only 1 candidate passing the tight selection: is N1T
    if (thePositron>-1 && theElectron<0) { 

      theReal = thePositron;
      theFake = getBestDenominator(theReal);      
      is1tight = true;
    }
    if (theElectron>-1 && thePositron<0) { 

      theReal = theElectron;
      theFake = getBestDenominator(theReal);      
      is1tight = true;
    }

    // here I have zero candidates passing the tight selection: is N0T
    if (thePositron<0 && theElectron<0) { 

      int theDenomPlus  = thePrePositron;
      int theDenomMinus = thePreElectron;
      float ptPlus  = GetPt(pxEle[theDenomPlus], pyEle[theDenomPlus]);
      float ptMinus = GetPt(pxEle[theDenomMinus],pyEle[theDenomMinus]);
      if (ptPlus>ptMinus) { 
	theReal = theDenomPlus;
	theFake = theDenomMinus;
      }
      else { 
	theReal = theDenomMinus;
	theFake = theDenomPlus;
      }
      is0tight = true;
    }   
    
    // to be sure: I assumed above that tight => loose. If not true, I discard the event....
    bool denomId, denomIso;
    if ( (thePositron>-1 && !isEleDenomFake(thePositron,&denomId,&denomIso)) || (theElectron>-1 && !isEleDenomFake(theElectron,&denomId,&denomIso)) ) {
      theReal = -1;
      theFake = -1;
    } 

    // sanity check
    if ( (is0tight && is1tight) || (is0tight && is2tight) || (is1tight && is2tight) ) cout << "questo non puo' succedere mai" << endl;

    // set of kinematics: : now I've all the final leptons 
    resetKinematics();

    // MET is an event variable. Independent o the channel        
    m_p3PFMET->SetXYZ(pxPFMet[0],pyPFMet[0],pzPFMet[0]);             // the one associated to the 0th vertex      
    m_theMET = m_p3PFMET->Pt();

    setKinematicsEE(theReal, theFake);
        
    // weight with the Fake / Prompt -> L2 probability	
    float theFakePt = GetPt(pxEle[theFake],pyEle[theFake]);
    float theRealPt = GetPt(pxEle[theReal],pyEle[theReal]);
    bool  isFakeBarrel = false;
    bool  isRealBarrel = false;
    if ( fabs(etaEle[theFake])<1.476 ) isFakeBarrel = true;
    if ( fabs(etaEle[theReal])<1.476 ) isRealBarrel = true;

    // do both F-P and F-F and P-P analysis
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
      
      float promptrate1    = getPromptRate( theFakePt, etaEle[theFake] );
      float promptrateErr1 = getPromptRateError( theFakePt, etaEle[theFake] );

      float fakerate1_15      = getFakeRate15( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerate1_35      = getFakeRate35( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerate1_50      = getFakeRate50( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerate1_QCD     = getFakeRateQCD( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerateErr1_15   = getFakeRateError15( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerateErr1_35   = getFakeRateError35( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerateErr1_50   = getFakeRateError50( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      float fakerateErr1_QCD  = getFakeRateErrorQCD( theFakePt, isFakeBarrel, fabs(etaEle[theFake]) );
      
      float promptrate2    = getPromptRate( theRealPt, etaEle[theReal] );
      float promptrateErr2 = getPromptRateError( theRealPt, etaEle[theReal] );

      float fakerate2_15      = getFakeRate15( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerate2_35      = getFakeRate35( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerate2_50      = getFakeRate50( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerate2_QCD     = getFakeRateQCD( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerateErr2_15   = getFakeRateError15( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerateErr2_35   = getFakeRateError35( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerateErr2_50   = getFakeRateError50( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );
      float fakerateErr2_QCD  = getFakeRateErrorQCD( theRealPt, isRealBarrel, fabs(etaEle[theReal]) );

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

	// for P-P15
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
	thisPartWeightFP_QCD = -( (promptrate2*fakerate1_QCD*(1-promptrate1)*(1-fakerate2_QCD)) + (promptrate1*fakerate2_QCD*(1-promptrate2)*(1-fakerate1_QCD)) )*myFactor_QCD; 
	
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

      weightFF_15  = tmpWeight;
      weightFF_35  = tmpWeight;
      weightFF_50  = tmpWeight;
      weightFF_QCD = tmpWeight;

      weightPP_15  = tmpWeight;
      weightPP_35  = tmpWeight;
      weightPP_50  = tmpWeight;
      weightPP_QCD = tmpWeight;
    }

    // nominal: ET cut = 35           
    float weightFP = weightFP_35;

    // -------------------------------------------------------------    
    int njets[1], nuncorrjets[1];
    float dphiLLJ[1], btag[1];
    int nsoftmu[1], nsoftmunojets[1], nextraleptons[1];

    // initialize the btags for the leading and subleading jets to unphysical value                     
    for(int ichan=0; ichan<1; ichan++) {
      leadJetBtag[ichan]        = -2000.;
      subleadJetBtag[ichan]     = -2000.;
      subLeadJetsMaxBtag[ichan] = -2000.;
    }

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


    // -------------------------------------------------------
    // filling counter for FP ( = W+jets)
    CutBasedHiggsSelectionEE.SetWeight(weightFP);                  
    CutBasedHiggsSelectionEE.SetMcTruth(true);  
    CutBasedHiggsSelectionEE.SetHLT(passedHLT[ee]);               
    CutBasedHiggsSelectionEE.SetIsChannel(m_channel[ee]);     
    CutBasedHiggsSelectionEE.SetElectronId(theReal);
    CutBasedHiggsSelectionEE.SetPositronId(theFake);
    CutBasedHiggsSelectionEE.SetElectronIsolation(theReal);
    CutBasedHiggsSelectionEE.SetPositronIsolation(theFake);
    CutBasedHiggsSelectionEE.SetElectronConvRejection(theReal);
    CutBasedHiggsSelectionEE.SetPositronConvRejection(theFake);
    CutBasedHiggsSelectionEE.SetElectronIp(theReal);
    CutBasedHiggsSelectionEE.SetPositronIp(theFake);
    // checking if the highest pT electron has pT>20
    float thisMaxPtIpEE = TMath::Max(GetPt(pxEle[theReal],pyEle[theReal]),GetPt(pxEle[theFake],pyEle[theFake]));
    if (thisMaxPtIpEE<20)   { 
      CutBasedHiggsSelectionEE.SetElectronIp(-1);
      CutBasedHiggsSelectionEE.SetPositronIp(-1);
    }

    CutBasedHiggsSelectionEE.SetHighElePt(hardestLeptonPt[ee]); 
    CutBasedHiggsSelectionEE.SetLowElePt(slowestLeptonPt[ee]);  
    CutBasedHiggsSelectionEE.SetExtraSlowLeptonPTCut(10.0); // enforce the min pT cut only on electrons 

    CutBasedHiggsSelectionEE.SetNJets(njets[ee]);
    CutBasedHiggsSelectionEE.SetNUncorrJets(nuncorrjets[ee]);
    CutBasedHiggsSelectionEE.SetBTagJets(btag[ee]);
    CutBasedHiggsSelectionEE.SetNSoftMuons(nsoftmu[ee]);
    CutBasedHiggsSelectionEE.SetNExtraLeptons(nextraleptons[ee]);
    CutBasedHiggsSelectionEE.SetMet(m_theMET);
    CutBasedHiggsSelectionEE.SetProjectedMet(m_projectedMet[ee]);
    CutBasedHiggsSelectionEE.SetNvtx(nPV);
    CutBasedHiggsSelectionEE.SetMetOverPtLL(m_metOptll[ee]);
    CutBasedHiggsSelectionEE.SetDeltaPhiLLJet(dphiLLJ[ee]);   
    CutBasedHiggsSelectionEE.SetDeltaPhi(m_deltaPhi[ee]);
    CutBasedHiggsSelectionEE.SetInvMass(m_mll[ee]);
    CutBasedHiggsSelectionEE.SetDetaLeptons(m_deltaEtaLeptons[ee]);
    CutBasedHiggsSelectionEE.SetWWInvMass(m_transvMass[ee]);

    bool isSelectedEE           = CutBasedHiggsSelectionEE.output();    
    bool selUpToFinalLeptonsEE  = CutBasedHiggsSelectionEE.outputUpToFinalLeptons();
    bool selUpToJetVetoEE       = CutBasedHiggsSelectionEE.outputUpToJetVeto();
    bool selUpToUncorrJetVetoEE = CutBasedHiggsSelectionEE.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiEE       = CutBasedHiggsSelectionEE.outputPreDeltaPhi();

    bool outputStep0  = CutBasedHiggsSelectionEE.outputStep0();
    bool outputStep1  = CutBasedHiggsSelectionEE.outputStep1();
    bool outputStep2  = CutBasedHiggsSelectionEE.outputStep2();
    bool outputStep3  = CutBasedHiggsSelectionEE.outputStep3();
    bool outputStep4  = CutBasedHiggsSelectionEE.outputStep4();
    bool outputStep5  = CutBasedHiggsSelectionEE.outputStep5();
    bool outputStep6  = CutBasedHiggsSelectionEE.outputStep6();
    bool outputStep7  = CutBasedHiggsSelectionEE.outputStep7();
    bool outputStep8  = CutBasedHiggsSelectionEE.outputStep8();
    bool outputStep9  = CutBasedHiggsSelectionEE.outputStep9();
    bool outputStep10 = CutBasedHiggsSelectionEE.outputStep10();
    bool outputStep11 = CutBasedHiggsSelectionEE.outputStep11();
    bool outputStep12 = CutBasedHiggsSelectionEE.outputStep12();
    bool outputStep13 = CutBasedHiggsSelectionEE.outputStep13();
    bool outputStep14 = CutBasedHiggsSelectionEE.outputStep14();
    bool outputStep15 = CutBasedHiggsSelectionEE.outputStep15();
    bool outputStep16 = CutBasedHiggsSelectionEE.outputStep16();
    bool outputStep17 = CutBasedHiggsSelectionEE.outputStep17();
    bool outputStep18 = CutBasedHiggsSelectionEE.outputStep18();
    bool outputStep19 = CutBasedHiggsSelectionEE.outputStep19();
    bool outputStep20 = CutBasedHiggsSelectionEE.outputStep20();
    bool outputStep21 = CutBasedHiggsSelectionEE.outputStep21();
    bool outputStep22 = CutBasedHiggsSelectionEE.outputStep22();
    bool outputStep23 = CutBasedHiggsSelectionEE.outputStep23();
    bool outputStep24 = CutBasedHiggsSelectionEE.outputStep24();
    bool outputStep25 = CutBasedHiggsSelectionEE.outputStep25();
    bool outputStep26 = CutBasedHiggsSelectionEE.outputStep26();
    bool outputStep27 = CutBasedHiggsSelectionEE.outputStep27();
    bool outputStep28 = CutBasedHiggsSelectionEE.outputStep28();

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

    int theLJ  = theLeadingJet[ee];
    int theSJ  = theSecondJet[ee];
    float ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeEE -> fillKFactor(1., 0., ptLJ);

    myOutTreeEE -> fillFake(myNT, weightFP, forStatErrFP_tree, 
			    weightFP_15, forStatErrFP_15_tree, weightFF_15, forStatErrFF_15_tree, weightPP_15, forStatErrPP_15_tree, 
			    weightFP_35, forStatErrFP_35_tree, weightFF_35, forStatErrFF_35_tree, weightPP_35, forStatErrPP_35_tree, 
			    weightFP_50, forStatErrFP_50_tree, weightFF_50, forStatErrFF_50_tree, weightPP_50, forStatErrPP_50_tree, 
			    weightFP_QCD, forStatErrFP_QCD_tree, weightFF_QCD, forStatErrFF_QCD_tree, weightPP_QCD, forStatErrPP_QCD_tree);

    myOutTreeEE -> fillRunInfos(runNumber, lumiBlock, eventNumber, weightFP, passedHLT[ee], nputosave);

    myOutTreeEE -> fillRazor(m_MTR[ee], m_MR[ee], m_GammaMR[ee]);

    myOutTreeEE -> fillAll(m_chMet[ee], GetPt(pxPFMet[0],pyPFMet[0]), GetPt(pxMet[0],pyMet[0]),
                           m_projectedMet[ee], m_deltaPhi[ee], m_deltaErre[ee], m_transvMass[ee], m_mll[ee],
                           hardestLeptonPt[ee], slowestLeptonPt[ee], hardestLeptonEta[ee], slowestLeptonEta[ee], 
			   m_deltaEtaLeptons[ee], nPV,
                           selUpToFinalLeptonsEE, selUpToJetVetoEE, selUpToUncorrJetVetoEE, selPreDeltaPhiEE, isSelectedEE);

    myOutTreeEE -> fillMLVars(njets[ee], nuncorrjets[ee], m_maxDxyEvt, m_maxDszEvt, btag[ee], m_maxImpactParameterMVABJetTags, m_maxCombinedSecondaryVertexMVABJetTags, nsoftmu[ee], leadJetBtag[ee], subleadJetBtag[ee], subLeadJetsMaxBtag[ee], nextraleptons[ee], nsoftmunojets[ee]);

    myOutTreeEE -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26, outputStep27, outputStep28); 

    float pxLJEE[3] = { -999., -999., -999. };
    float pyLJEE[3] = { -999., -999., -999. };
    float pzLJEE[3] = { -999., -999., -999. }; 
    float pxSJEE[3] = { -999., -999., -999. }; 
    float pySJEE[3] = { -999., -999., -999. }; 
    float pzSJEE[3] = { -999., -999., -999. }; 

    if ( GetPt(m_p4LeptonPlus[ee]->Px(),m_p4LeptonPlus[ee]->Py()) > GetPt(m_p4LeptonMinus[ee]->Px(),m_p4LeptonMinus[ee]->Py()) ) {

      myOutTreeEE -> fillKinematics( m_p3TKMET[ee].Px(), m_p3TKMET[ee].Py(), m_p3TKMET[ee].Pz(),
                                     pxLJEE, pyLJEE, pzLJEE, pxSJEE, pySJEE, pzSJEE,
                                     m_p4LeptonPlus[ee]->Px(),  m_p4LeptonPlus[ee]->Py(),  m_p4LeptonPlus[ee]->Pz(),
                                     m_p4LeptonMinus[ee]->Px(), m_p4LeptonMinus[ee]->Py(), m_p4LeptonMinus[ee]->Pz(),
                                     m_chEE, m_lhEE, m_isoEE, m_chmajEE, m_bdtEE, 
                                     m_jetsSum[ee], m_uncorrJetsSum[ee], m_p3PFMET);
    } else {
      
      myOutTreeEE -> fillKinematics( m_p3TKMET[ee].Px(), m_p3TKMET[ee].Py(), m_p3TKMET[ee].Pz(),
                                     pxLJEE, pyLJEE, pzLJEE, pxSJEE, pySJEE, pzSJEE,
                                     m_p4LeptonMinus[ee]->Px(), m_p4LeptonMinus[ee]->Py(), m_p4LeptonMinus[ee]->Pz(),
                                     m_p4LeptonPlus[ee]->Px(),  m_p4LeptonPlus[ee]->Py(),  m_p4LeptonPlus[ee]->Pz(),
                                     m_chEE, m_lhEE, m_isoEE, m_chmajEE, m_bdtEE, 
				     m_jetsSum[ee], m_uncorrJetsSum[ee], m_p3PFMET);
    }

    // dumping final tree, only if there are 2 leptons in the acceptance
    if(outputStep1) myOutTreeEE -> store();
  }
}

void LeptonPlusFakeMLSelection_fullEE::displayEfficiencies(std::string datasetName) {

  std::string::size_type loc = datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    datasetName.erase(loc);
  }
  
  std::cout << "--------------------------------" << std::endl;
  std::cout << "=== RATE ESTIMATED FROM FAKE RATE FOR PF SELECTION ===: " << std::endl;
  CutBasedHiggsSelectionEE.displayEfficiencies(datasetName);

  // simple cuts based or like based ele id                                                                                         
  if (!_selectionEE->getSwitch("asymmetricID")) {
    std::cout << "cut based symmetric ID: " << std::endl;
    EgammaCutBasedID.displayEfficiencies();
  } else {
    std::cout << "cut based asymmetric ID: Low pT" << std::endl;
    EgammaCutBasedIDLow.displayEfficiencies();
    std::cout << "cut based asymmetric ID: High pT" << std::endl;
    EgammaCutBasedID.displayEfficiencies();
  }
}

std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_acceptance() {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _acceptEleAll.clear();

  for(int i=0;i<nEle;i++) {

    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();

    if(_selectionEE->getSwitch("etaElectronAcc") && !_selectionEE->passCut("etaElectronAcc",etaEle[i]) ) continue;

    if(_selectionEE->getSwitch("ptElectronAcc") && !_selectionEE->passCut("ptElectronAcc",thisPt) ) continue;
    
    float thisCharge = chargeEle[i];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same charge test  // chiara
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

std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_id( std::vector<int> acceptEle ) {

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
    if (!_selectionEE->getSwitch("asymmetricID")) isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if ( _selectionEE->getSwitch("asymmetricID")) {
      if (thisPt>=20) isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if (thisPt<20)  isEleIDAndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }
    
    if (!theElectronID) continue;  

    /*
    // further requests if we apply the smurf ID and pT<20
    TString stringIdLow (_selectionEE->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      if ( thisPt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[thisEle], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[thisEle];   scEta = etaSC[sc];   }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[thisEle]; scEta = etaPFSC[sc]; }
	if ( fbremEle[thisEle]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[thisEle]>0.95) ) continue; // hardcoded
      }
    }
    */

    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    // only for same charge test  // chiara
    /*
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

std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_isol( std::vector<int> idEle ) {

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
    // only for same charge test  // chiara
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

std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_conv( std::vector<int> isolEle ) {

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
    // only for same charge test  // chiara
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


std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_ip( std::vector<int> convEle ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _ipEleAll.clear();

  for (int iEle=0; iEle<convEle.size(); iEle++) {
    int thisEle = convEle[iEle];

    int gsfTrack = gsfTrackIndexEle[thisEle]; 
    float dxyEle = transvImpactParGsfTrack[gsfTrack];
    float dzEle  = eleDzPV(thisEle,0);
    if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",dxyEle)) ) continue;
    if (_selectionEE->getSwitch("electronDz") && (!_selectionEE->passCut("electronDz",dzEle)) )  continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    // chiara
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same charge test  // chiara
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

std::pair<int,int> LeptonPlusFakeMLSelection_fullEE::getBestElectronPair_denominator() {
  
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
    // only for same charge test  // chiara
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

int LeptonPlusFakeMLSelection_fullEE::getBestDenominator(int realEle) {
  
  int theFake=-1;
  float maxPtFake=-1000.;
  
  for(int iele=0; iele<nEle; iele++) {
    
    if (iele==realEle) continue;
    
    // chiara, for SS test
    if (chargeEle[iele]*chargeEle[realEle]>0) continue;
    // if (chargeEle[iele]*chargeEle[realEle]<0) continue;
    // if (chargeEle[iele]<0) continue;
    
    bool denomId, denomIso;
    bool isGoodDenom = isEleDenomFake(iele,&denomId,&denomIso);
    if (!isGoodDenom) continue;

    float thisElePt = GetPt(pxEle[iele],pyEle[iele]);

    // removing candidates passing the tight selection
    bool isTight = true;
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    if (!_selectionEE->getSwitch("asymmetricID")) isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if ( _selectionEE->getSwitch("asymmetricID")) {
      if (thisElePt>=20) isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if (thisElePt<20)  isEleIDAndDenom(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }
    if (!theElectronID) isTight = false;

    /*
    TString stringIdLow (_selectionEE->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      if ( thisElePt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[iele], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[iele];   scEta = etaSC[sc];   }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[iele]; scEta = etaPFSC[sc]; }
	if ( fbremEle[iele]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[iele]>0.95) ) isTight = false;  // hardcoded
      }
    }
    */

    if (!theElectronIsol)    isTight = false; 
    if (!theElectronConvRej) isTight = false;
    int gsfTrack = gsfTrackIndexEle[iele]; 
    float dxyEle = transvImpactParGsfTrack[gsfTrack];
    float dzEle  = eleDzPV(iele,0);
    if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",dxyEle)) ) isTight = false;
    if (_selectionEE->getSwitch("electronDz") && (!_selectionEE->passCut("electronDz",dzEle)) )  isTight = false;
    if (isTight) continue;
    
    if( thisElePt > maxPtFake ) { maxPtFake = thisElePt; theFake=iele; }
  }

  return theFake;
}
						    
float LeptonPlusFakeMLSelection_fullEE::getFakeRate15( float fakePt, bool isFakeBarrel, float fakeEta ) {
  
  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m15_fakeRateEB1[theBin];
	  if (theEtaBin==1) return m15_fakeRateEB2[theBin];
	  if (theEtaBin==2) return m15_fakeRateEE1[theBin];
	  if (theEtaBin==3) return m15_fakeRateEE2[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRate35( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m35_fakeRateEB1[theBin];
	  if (theEtaBin==1) return m35_fakeRateEB2[theBin];
	  if (theEtaBin==2) return m35_fakeRateEE1[theBin];
	  if (theEtaBin==3) return m35_fakeRateEE2[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRate50( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m50_fakeRateEB1[theBin];
	  if (theEtaBin==1) return m50_fakeRateEB2[theBin];
	  if (theEtaBin==2) return m50_fakeRateEE1[theBin];
	  if (theEtaBin==3) return m50_fakeRateEE2[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRateQCD( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return mQCD_fakeRateEB1[theBin];
	  if (theEtaBin==1) return mQCD_fakeRateEB2[theBin];
	  if (theEtaBin==2) return mQCD_fakeRateEE1[theBin];
	  if (theEtaBin==3) return mQCD_fakeRateEE2[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRateError15( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m15_fakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m15_fakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m15_fakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m15_fakeRateEE2_err[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRateError35( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m35_fakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m35_fakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m35_fakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m35_fakeRateEE2_err[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRateError50( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return m50_fakeRateEB1_err[theBin];
	  if (theEtaBin==1) return m50_fakeRateEB2_err[theBin];
	  if (theEtaBin==2) return m50_fakeRateEE1_err[theBin];
	  if (theEtaBin==3) return m50_fakeRateEE2_err[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getFakeRateErrorQCD( float fakePt, bool isFakeBarrel, float fakeEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {

      for (int theEtaBin = 0; theEtaBin<4; theEtaBin++) {
	if( fabs(fakeEta) >= m_minFakeEta[theEtaBin] && fabs(fakeEta) < m_maxFakeEta[theEtaBin] ) {

	  if (theEtaBin==0) return mQCD_fakeRateEB1_err[theBin];
	  if (theEtaBin==1) return mQCD_fakeRateEB2_err[theBin];
	  if (theEtaBin==2) return mQCD_fakeRateEE1_err[theBin];
	  if (theEtaBin==3) return mQCD_fakeRateEE2_err[theBin];
	}
      }
    }
  }

  std::cout << "BIG ERROR: fakePt = " << fakePt << ", fakeEta = " << fakeEta << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getPromptRate( float promptPt, float promptEta ) {

  for (int theBin = 0; theBin<5; theBin++) {
    if( promptPt >= m_minPromptPt[theBin] && promptPt < m_maxPromptPt[theBin] ) {

      if (fabs(promptEta)<1.4442)  return m_promptRateEB[theBin];
      if (fabs(promptEta)>1.566)   return m_promptRateEE[theBin];
      if (fabs(promptEta)<1.566 && fabs(promptEta)>1.4442)   return m_promptRateCr[theBin];
    }
  }

  std::cout << "BIG ERROR: promptPt = " << promptPt << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_fullEE::getPromptRateError( float promptPt, float promptEta ) {

  for (int theBin = 0; theBin < 5; theBin++) {
    if( promptPt >= m_minPromptPt[theBin] && promptPt < m_maxPromptPt[theBin] ) {

      if (fabs(promptEta)<1.4442)  return m_promptRateEB_err[theBin];
      if (fabs(promptEta)>1.566)   return m_promptRateEE_err[theBin];
      if (fabs(promptEta)<1.566 && fabs(promptEta)>1.4442)   return m_promptRateCr_err[theBin];
    }
  }

  return -1.;
}

void LeptonPlusFakeMLSelection_fullEE::setKinematicsEE(int myReal, int myFake) {

  if (myFake > -1 && myReal > -1) {

    eleCands[ee].push_back(myReal);
    eleCands[ee].push_back(myFake);
    hardestLeptonPt[ee] = TMath::Max(GetPt(pxEle[myReal],pyEle[myReal]),GetPt(pxEle[myFake],pyEle[myFake]));
    slowestLeptonPt[ee] = TMath::Min(GetPt(pxEle[myReal],pyEle[myReal]),GetPt(pxEle[myFake],pyEle[myFake]));
    m_p4LeptonMinus[ee] -> SetXYZT(pxEle[myReal], pyEle[myReal], pzEle[myReal], energyEle[myReal]);
    m_p4LeptonPlus[ee]  -> SetXYZT(pxEle[myFake], pyEle[myFake], pzEle[myFake], energyEle[myFake]);
    m_mll[ee]       = (*(m_p4LeptonMinus[ee]) + *(m_p4LeptonPlus[ee])).M();
    m_deltaPhi[ee]  = fabs(180./TMath::Pi() * m_p4LeptonMinus[ee]->Vect().DeltaPhi(m_p4LeptonPlus[ee]->Vect()));
    m_deltaErre[ee] = m_p4LeptonMinus[ee]->Vect().DeltaR(m_p4LeptonPlus[ee]->Vect());
    m_deltaEtaLeptons[ee] = etaEle[myReal]-etaEle[myFake];
    m_dilepPt[ee].SetXYZ(m_p4LeptonMinus[ee]->Vect().X()+m_p4LeptonPlus[ee]->Vect().X(),m_p4LeptonMinus[ee]->Vect().Y()+m_p4LeptonPlus[ee]->Vect().Y(),0.0);
    m_transvMass[ee] = sqrt( 2.*(m_dilepPt[ee].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[ee]))) );
    m_GammaMR[ee] = CalcGammaMRstar(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee]);
    m_MR[ee]  = CalcMRstar(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee]);
    m_MTR[ee] = CalcMTR(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee],*m_p3PFMET);
    m_metOptll[ee] = m_theMET / m_dilepPt[ee].Pt();
    m_mT2[ee] = 0.;
    m_projectedMet[ee] = GetProjectedMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_p3TKMET[ee] = pfChargedMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_chMet[ee] = pfChargedMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect()).Pt();

    int lead(-1), sublead(-1);
    if(m_p4LeptonMinus[ee]->Pt() >= m_p4LeptonPlus[ee]->Pt()) {
      lead = myReal; 
      sublead = myFake;
      hardestLeptonEta[ee] = etaEle[myReal];
      slowestLeptonEta[ee] = etaEle[myFake];
    } else {
      lead = myFake; 
      sublead = myReal;
      hardestLeptonEta[ee] = etaEle[myFake];
      slowestLeptonEta[ee] = etaEle[myReal];
    }

    m_chEE[0] = chargeEle[lead];
    m_chEE[1] = chargeEle[sublead];
    m_isoEE[0] = pfCombinedIsoEle[lead] / hardestLeptonPt[ee];
    m_isoEE[1] = pfCombinedIsoEle[sublead] / slowestLeptonPt[ee];
    m_lhEE[0] = eleIdLikelihoodEle[lead];
    m_lhEE[1] = eleIdLikelihoodEle[sublead];
    m_bdtEE[0] = eleBDT(fMVA,lead);                 
    m_bdtEE[1] = eleBDT(fMVA,sublead);              
    m_chmajEE[0] = eleChargeMajority(lead);
    m_chmajEE[1] = eleChargeMajority(sublead);      
  }
}

void LeptonPlusFakeMLSelection_fullEE::resetKinematicsStart() {

  theReal         = -1;
  theFake         = -1;
  thePreElectron  = -1;
  thePrePositron  = -1;
}

void LeptonPlusFakeMLSelection_fullEE::resetKinematics() {

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
    m_projectedMet[theChannel]    = 0.;
    m_chMet[theChannel]           = 0.;
    m_GammaMR[theChannel]         = 0.;
    m_MR[theChannel]              = 0.;
    m_MTR[theChannel]             = 0.;
  }
}

int LeptonPlusFakeMLSelection_fullEE::numJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel) {

  int num=0;
  m_goodJets.clear();
  float ETMax=0.;
  float ETMax2=0.;
  
  theLeadingJet[theChannel]=-1;
  theSecondJet[theChannel]=-1;
  m_jetsSum[theChannel]->SetXYZT(0.,0.,0.,0);
  
  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));
  
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
        // taking from ee config file, but jets veto is the same for all the channels                                  
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    // check if the muons falls into the jet                                                                           
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        // taking from ee config file, but jets veto is the same for all the channels                                  
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;
    
    if(pt>5.0) (*m_jetsSum[theChannel]) += p4Jet;
    
    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;
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
    
    if(_selectionEE->getSwitch("etJetAcc") && !_selectionEE->passCut("etJetAcc", pt)) continue;
    
    m_goodJets.push_back(j);
    num++;
  }

  return num;
}

int LeptonPlusFakeMLSelection_fullEE::numUncorrJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  int num=0;

  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));

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
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }

    // check if the muons falls into the jet
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;
    
    if(uncorrEt>5.0) (*m_uncorrJetsSum[theChannel]) += p4Jet;

    if(_selectionEE->getSwitch("etaJetAcc")      && !_selectionEE->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;
    if(_selectionEE->getSwitch("etUncorrJetAcc") && !_selectionEE->passCut("etUncorrJetAcc", uncorrEt))   continue;

    num++;
  }
  
  return num;
}

float LeptonPlusFakeMLSelection_fullEE::bVetoJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));

  float output=-999;
  float outputSubLeadJets = -999;
  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    // no threshold is applied here on pt. Not affected by JES uncertainties
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);

    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) pt = (GetJESCorrected(p4Jet,JESUncertainty.Data())).Pt();

    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;

    // hardcoded
    float rawpt = uncorrEnergyAK5PFPUcorrJet[j] * fabs(sin(thetaAK5PFPUcorrJet[j]));
    if(rawpt < 10.0) continue;   

    // chiara, to be commented on MC until we do not run on Fall11
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
    //              chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    bool foundMatch=false;
    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }

    // check if the muons falls into the jet
    for(int i=0; i<(int)muonToRemove.size(); i++) {
      int mu = muonToRemove[i];
      if ( mu > -1 ) {
        TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Muon ) );
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    float tmp = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];
    if(tmp > output) output = tmp;
    if(j != theLeadingJet[theChannel] && tmp > outputSubLeadJets) outputSubLeadJets = tmp;
  }

  subLeadJetsMaxBtag[theChannel] = outputSubLeadJets;
  return output;
}

float LeptonPlusFakeMLSelection_fullEE::deltaPhiLLJet(int ichan) {   
  
  int myLeadingJet = theLeadingJet[ichan];
  
  if(myLeadingJet > -1) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);    
    if(leadingJetP3.Pt()>15.0) 
      return fabs(180./TMath::Pi() * leadingJetP3.DeltaPhi(m_dilepPt[ichan]));  // hardcoded
    else 
      return 0.0;
  } else return -999.;
}

float LeptonPlusFakeMLSelection_fullEE::deltaPhiLLJet15(int ichan) {   
  
  int myLeadingJet = theLeadingJet[ichan];

  if(myLeadingJet > -1) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);    
    float leadJetPT = leadingJetP3.Pt();
    if ( leadJetPT > 15. ) {
      return fabs(180./TMath::Pi() * leadingJetP3.DeltaPhi(m_dilepPt[ichan]));                           
    }
  } else return -999.;
}

 int LeptonPlusFakeMLSelection_fullEE::numSoftMuons(std::vector<int> muonToRemove, std::vector<int> jetsToRemove) {

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

int LeptonPlusFakeMLSelection_fullEE::numExtraLeptons( std::vector<int> eleToRemove, std::vector<int> muonToRemove  ) {

  int numEle = 0;
  for(int i=0; i<nEle; ++i) {
    
    bool isSelEle=false;
    for(int eleSel=0; eleSel<(int)eleToRemove.size(); eleSel++) {
      if(i==eleToRemove[eleSel]) isSelEle=true;
    }
    if(isSelEle) continue;

    if(_selectionEE->getSwitch("etaElectronAcc") && !_selectionEE->passCut("etaElectronAcc",etaEle[i]) ) continue;
    if(_selectionEE->getSwitch("ptElectronAcc")  && !_selectionEE->passCut("ptElectronAcc",GetPt(pxEle[i],pyEle[i])) ) continue;

    bool theId, theIso, theConvRej;
    theId = theIso = theConvRej = true;
    if (!_selectionEE->getSwitch("asymmetricID")) 
      isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID,m_useBDTEleID);
    if (_selectionEE->getSwitch("asymmetricID")) {
      float pt = GetPt(pxEle[i],pyEle[i]);	
      if(pt>=20) isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID,m_useBDTEleID);
      if(pt<20)  isEleIDAndDenom(i,&theId,&theIso,&theConvRej,&EgammaCutBasedIDLow,m_useBDTEleID);
    }
    if(!theId || !theIso || !theConvRej) continue;
    
    /*
    // further requests if we apply the smurf ID and pT<15
    TString stringIdLow (_selectionEE->getStringParameter("electronIDTypeLow"));
    if( stringIdLow.Contains("Smurf") ) {
      float pt = GetPt(pxEle[i],pyEle[i]);
      if ( pt<20  ) {
	Utils anaUtils;
	int sc;
	bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[i], bits::isEcalDriven);
	float scEta = -1.;
	if ( ecalDriven) { sc = superClusterIndexEle[i];   scEta = etaSC[sc];   }
	if (!ecalDriven) { sc = PFsuperClusterIndexEle[i]; scEta = etaPFSC[sc]; }
	if ( fbremEle[i]<0.15 && !(fabs(scEta)<1.0 && eSuperClusterOverPEle[i]>0.95) ) continue; // hardcoded
      }
    }
    */

    int track = gsfTrackIndexEle[i];
    float dxyEle = transvImpactParGsfTrack[track];
    float dzEle  = eleDzPV(i,0);
    if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",dxyEle)) ) continue;
    if (_selectionEE->getSwitch("electronDz") && (!_selectionEE->passCut("electronDz",dzEle)) ) continue;

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
    if(_selectionEE->getSwitch("etaMuonAcc") && !_selectionEE->passCut("etaMuonAcc",etaMuon[i]) ) continue;
    if(_selectionEE->getSwitch("ptMuonAcc") && !_selectionEE->passCut("ptMuonAcc",ptMu) ) continue;

    bool theId = true;
    isMuonID(i,&theId);
    if(!theId) continue;
    if( ! isPFIsolatedMuon(i) ) continue; 

    int track = trackIndexMuon[i];
    float dxy = transvImpactParTrack[track];
    float dz  = muonDzPV(i,0);
    if (ptMu>20)    // hardcoded
      if (_selectionEE->getSwitch("muonIPhighPT") && (!_selectionEE->passCut("muonIPhighPT",dxy)) ) continue;   
    
    if (ptMu<20)    // hardcoded
      if (_selectionEE->getSwitch("muonIPlowPT")  && (!_selectionEE->passCut("muonIPlowPT",dxy)) ) continue;   
    
    if (_selectionEE->getSwitch("muonDz") && (!_selectionEE->passCut("muonDz",dz)) )  continue;   

    numMu++;
  }
  
  return numEle + numMu;
}

bool LeptonPlusFakeMLSelection_fullEE::isGoodTrack(int iTrack, float ptMin, float ptMax, float chi2, float etaMax, float nHits) {
  TVector3 p3Track(pxTrack[iTrack],pyTrack[iTrack],pzTrack[iTrack]);
  double pt = p3Track.Pt();
  if(pt < ptMin) return false;
  if(pt > ptMax) return false;
  if(trackNormalizedChi2Track[iTrack] > chi2) return false; 
  if(fabs(p3Track.Eta()) > etaMax) return false;
  if(trackValidHitsTrack[iTrack] < nHits) return false;
  return true;
}

float LeptonPlusFakeMLSelection_fullEE::GetProjectedMet(TVector3 p1, TVector3 p2) {

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
bool LeptonPlusFakeMLSelection_fullEE::reloadTriggerMask(int runN) {

  std::vector<int> triggerMask;

  // load the triggers required for EE
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersEE.begin();fIter!=requiredTriggersEE.end();++fIter) {
    std::string pathName = getHLTPathForRun(runN,*fIter);
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(pathName) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_requiredTriggersEE = triggerMask;

  // load the triggers NOT required for EE 
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=notRequiredTriggersEE.begin();fIter!=notRequiredTriggersEE.end();++fIter) {
    std::string pathName = getHLTPathForRun(runN,*fIter);
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(pathName) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_notRequiredTriggersEE = triggerMask;
}

bool LeptonPlusFakeMLSelection_fullEE::hasPassedHLT(){

  Utils anaUtils;
  bool required    = anaUtils.getTriggersOR(m_requiredTriggersEE, firedTrg);
  bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersEE, firedTrg);
  return (required && !notRequired);
}

void LeptonPlusFakeMLSelection_fullEE::setRequiredTriggers(const std::vector<std::string>& reqTriggers) {

  requiredTriggersEE=reqTriggers;
}

void LeptonPlusFakeMLSelection_fullEE::setNotRequiredTriggers(const std::vector<std::string>& reqTriggers){

  notRequiredTriggersEE=reqTriggers;
}

bool LeptonPlusFakeMLSelection_fullEE::isPFIsolatedMuon(int muonIndex) {
  float eta = etaMuon[muonIndex];
  float pt = GetPt(pxMuon[muonIndex],pyMuon[muonIndex]);
  float iso = pfCombinedIsoMuon[muonIndex]/pt;
  if( pt>=20. && fabs(eta)<1.479 ) return (iso < 0.13);
  if( pt>=20. && fabs(eta)>=1.479 ) return (iso < 0.09);
  if( pt<20. && fabs(eta)<1.479 ) return (iso < 0.06);
  if( pt<20. && fabs(eta)>=1.479 ) return (iso < 0.05);
  return true;
}

