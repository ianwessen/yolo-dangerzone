#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/HiggsMLSelection.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "CommonTools/include/LeptonIdBits.h"
#include "CommonTools/include/PUWeight.h"
// #include "CommonTools/include/LumiReWeighting.h"  // for OOT PU

//#include "Mt2/SUSYPhys_Mt2_222_Calculator.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <TTree.h>

using namespace bits;

HiggsMLSelection::HiggsMLSelection(TTree *tree) 
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
  
  // selection efficiencies
  std::string fileCutsEE     = higgsConfigDirMass + "2e2nuCuts.txt";
  std::string fileSwitchesEE = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionEE.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 
  _selectionEE = CutBasedHiggsSelectionEE.GetSelection();  

  std::string fileCutsMM     = higgsConfigDirMass + "2mu2nuCuts.txt";
  std::string fileSwitchesMM = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionMM.Configure(fileCutsMM.c_str(),fileSwitchesMM.c_str(),"FULL SELECTION EVENT COUNTER MM"); 
  _selectionMM = CutBasedHiggsSelectionMM.GetSelection();

  std::string fileCutsEM     = higgsConfigDirMass + "emu2nuCuts.txt";
  std::string fileSwitchesEM = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionEM.Configure(fileCutsEM.c_str(),fileSwitchesEM.c_str(),"FULL SELECTION EVENT COUNTER EM"); 
  _selectionEM = CutBasedHiggsSelectionEM.GetSelection();

  std::string fileCutsME     = higgsConfigDirMass + "emu2nuCuts.txt";
  std::string fileSwitchesME = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionME.Configure(fileCutsME.c_str(),fileSwitchesME.c_str(),"FULL SELECTION EVENT COUNTER ME"); 
  _selectionME = CutBasedHiggsSelectionME.GetSelection();


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
  _selectionEE->addStringParameter("JESUncertainty");

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

  if(_selectionEE->getSwitch("applyBDTEleID")) m_useBDTEleID = true;
  else m_useBDTEleID = false;

  // configuring the electron BDT
  fMVA = new ElectronIDMVA();
  fMVA->Initialize("BDTG method",
                   "elebdtweights/Subdet0LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet1LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet2LowPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet0HighPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet1HighPt_WithIPInfo_BDTG.weights.xml",
                   "elebdtweights/Subdet2HighPt_WithIPInfo_BDTG.weights.xml" ,                
                   ElectronIDMVA::kWithIPInfo);

  // configuring the DYMVA
  m_dymvaAlgo = new GetDYMVA(1);

  // Reading GoodRUN LS
  std::cout << "[GoodRunLS]::goodRunLS is " << _selectionEE->getSwitch("goodRunLS") << " isData is " <<  _selectionEE->getSwitch("isData") << std::endl;

  // To read good run list!
  if (_selectionEE->getSwitch("goodRunLS") && _selectionEE->getSwitch("isData")) {
    std::string goodRunJsonFile = "config/json/goodCollisions2012.json";
    setJsonGoodRunList(goodRunJsonFile);
    fillRunLSMap();
  }

  // kinematics
  m_p3PFMET = new TVector3(0.,0.,0.);
  m_metFromJets = new TVector3(0.,0.,0.);
  m_pfMetJESUp = new TVector3(0.,0.,0.);
  m_pfMetJESDown = new TVector3(0.,0.,0.);
  for(int theChannel=0; theChannel<4; theChannel++) {
    m_p4LeptonPlus[theChannel]  = new TLorentzVector(0.,0.,0.,0.);
    m_p4LeptonMinus[theChannel] = new TLorentzVector(0.,0.,0.,0.);
    m_jetsSum[theChannel] = new TLorentzVector(0.,0.,0.,0.);
    m_uncorrJetsSum[theChannel] = new TLorentzVector(0.,0.,0.,0.);
  }

  // b-veto event variables
  m_maxDxyEvt = 0.0;
  m_maxDszEvt = 0.0;

  // histo to study jet/electron match
  H_deltaRuncorr = new TH1F("H_deltaRuncorr","uncorrected jets",100, 0.,2*TMath::Pi());
  H_deltaRcorr   = new TH1F("H_deltaRcorr",  "corrected jets",  100, 0.,2*TMath::Pi());

  // as defaults switch off jet ID studies 
  wantJetIdStuff = true;
}

HiggsMLSelection::~HiggsMLSelection(){

  for(int theChannel=0; theChannel<4; theChannel++) {  
    delete m_p4LeptonPlus[theChannel];
    delete m_p4LeptonMinus[theChannel];
    delete m_jetsSum[theChannel];
    delete m_uncorrJetsSum[theChannel];
  }
  delete m_p3PFMET;  
  delete m_metFromJets;
  delete m_pfMetJESUp;
  delete m_pfMetJESDown;

  delete _selectionEE;
  delete _selectionMM;
  delete _selectionEM;
  delete _selectionME;
  
  myOutTreeEE -> save();
  myOutTreeMM -> save();
  myOutTreeEM -> save();
  myOutTreeME -> save();

  myEleIdTree -> save();
}

bool HiggsMLSelection::findMcTree(const char* processType) {

  _process = "UNDEFINED";
  _theGenEle = -1;
  _theGenPos = -1;
  
  // now we look for ee || mumu || emu
  // in the acceptance and with a loose pT threshold
  float etaEleAcc_  = 2.5;
  float ptEleAcc_   = 5.0; // GeV
  float etaMuonAcc_ = 2.4;
  float ptMuonAcc_  = 0.0; // GeV
  
  // signal: 2e2nu
  if(strcmp(processType,"HtoWWto2e2nu")==0) {
    int indeminus=999, indeplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc] == -11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc] ==  11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeminus<25 && indeplus<25 ) {
      _theGenPos = indeplus;
      _theGenEle = indeminus;
    }
    return ( indeplus < 25 && indeminus < 25 );
  }

  // signal: 2m2nu
  if(strcmp(processType,"HtoWWto2m2nu")==0) {
    int indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc] == -13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc] ==  13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
    }
    if( indmuminus<25 && indmuplus<25 ) {
      _theGenMuPlus  = indmuplus;
      _theGenMuMinus = indmuminus;
    }
    return ( indmuplus < 25 && indmuminus < 25 );
  }

  // signal: em2nu
  if(strcmp(processType,"HtoWWtoem2nu")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;

    bool isEM = false;

    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && fabs(etaMc[imc]) < etaEleAcc_  && ptMc > ptEleAcc_ )  indeplus = imc;
      if( idMc[imc]  ==  13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  ==  11 && fabs(etaMc[imc]) < etaEleAcc_  && ptMc > ptEleAcc_ )  indeminus = imc;
    }

    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos     = indeplus;
      _theGenMuMinus = indmuminus;
      float ptMcPos     = pMc[indeplus]*fabs(sin(thetaMc[indeplus]));      
      float ptMcMuMinus = pMc[indmuminus]*fabs(sin(thetaMc[indmuminus]));      
      if ( ptMcPos>ptMcMuMinus) isEM = true;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
      float ptMcEle    = pMc[indeminus]*fabs(sin(thetaMc[indeminus]));      
      float ptMcMuPlus = pMc[indmuplus]*fabs(sin(thetaMc[indmuplus]));      
      if ( ptMcEle>ptMcMuPlus) isEM = true;
    }
    
    return ( (indeplus<25 && indmuminus<25 && isEM) || (indeminus<25 && indmuplus<25 && isEM) );
  }

  // signal: me2nu
  if(strcmp(processType,"HtoWWtome2nu")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;
    
    bool isME = false;

    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && fabs(etaMc[imc]) < etaEleAcc_  && ptMc > ptEleAcc_ )  indeplus = imc;
      if( idMc[imc]  ==  13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  ==  11 && fabs(etaMc[imc]) < etaEleAcc_  && ptMc > ptEleAcc_ )  indeminus = imc;
    }

    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos     = indeplus;
      _theGenMuMinus = indmuminus;
      float ptMcPos     = pMc[indeplus]*fabs(sin(thetaMc[indeplus]));      
      float ptMcMuMinus = pMc[indmuminus]*fabs(sin(thetaMc[indmuminus]));      
      if ( ptMcPos<ptMcMuMinus) isME = true;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
      float ptMcEle    = pMc[indeminus]*fabs(sin(thetaMc[indeminus]));      
      float ptMcMuPlus = pMc[indmuplus]*fabs(sin(thetaMc[indmuplus]));      
      if ( ptMcEle<ptMcMuPlus) isME = true;
    }
    
    return ( (indeplus<25 && indmuminus<25 && isME) || (indeminus<25 && indmuplus<25 && isME) );
  }

  // signal ee excluding taus
  if(strcmp(processType,"HtoWWto2e2nu_prompt")==0) {
    int indeminus=999, indeplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 11 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeminus<25 && indeplus<25 ) {
      _theGenPos = indeplus;
      _theGenEle = indeminus;
    }
    return ( indeplus < 25 && indeminus < 25 );
  }

  // signal mm excluding taus
  if(strcmp(processType,"HtoWWto2m2nu_prompt")==0) {
    int indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -13 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 13 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = 11;
    }
    if( indmuminus<25 && indmuplus<25 ) {
      _theGenMuPlus = indmuplus;
      _theGenMuMinus = indmuminus;
    }
    return ( indmuplus < 25 && indmuminus < 25 );
  }

  // signal em excluding taus
  if(strcmp(processType,"HtoWWtoem2nu_prompt")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;

    bool isEM = false;

    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 13 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 11 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }

    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos     = indeplus;
      _theGenMuMinus = indmuminus;
      float ptMcPos     = pMc[indeplus]*fabs(sin(thetaMc[indeplus]));      
      float ptMcMuMinus = pMc[indmuminus]*fabs(sin(thetaMc[indmuminus]));      
      if ( ptMcPos>ptMcMuMinus) isEM = true;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
      float ptMcEle    = pMc[indeminus]*fabs(sin(thetaMc[indeminus]));      
      float ptMcMuPlus = pMc[indmuplus]*fabs(sin(thetaMc[indmuplus]));      
      if ( ptMcEle>ptMcMuPlus) isEM = true;
    }
    
    return ( (indeplus<25 && indmuminus<25 && isEM) || (indeminus<25 && indmuplus<25 && isEM) );
  }

  // signal me excluding taus
  if(strcmp(processType,"HtoWWtome2nu_prompt")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;

    bool isME = false;

    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 13 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 11 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    
    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos     = indeplus;
      _theGenMuMinus = indmuminus;
      float ptMcPos     = pMc[indeplus]*fabs(sin(thetaMc[indeplus]));      
      float ptMcMuMinus = pMc[indmuminus]*fabs(sin(thetaMc[indmuminus]));      
      if ( ptMcPos<ptMcMuMinus) isME = true;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
      float ptMcEle    = pMc[indeminus]*fabs(sin(thetaMc[indeminus]));      
      float ptMcMuPlus = pMc[indmuplus]*fabs(sin(thetaMc[indmuplus]));      
      if ( ptMcEle<ptMcMuPlus) isME = true;
    }
    
    return ( (indeplus<25 && indmuminus<25 && isME) || (indeminus<25 && indmuplus<25 && isME) );
  }
  
  // signal: 2l2nu
  if(strcmp(processType,"HtoWWto2l2nu")==0) {
    int indlminus=999, indlplus=999;
    for(int imc=6;imc<25;imc++) {
      if(idMc[imc]>10 && idMc[imc]<19 && idMc[mothMc[imc]]==-24)  indlminus=imc;
      if(idMc[imc]<-10 && idMc[imc]>-19 && idMc[mothMc[imc]]==24) indlplus=imc;
    }
    if(indlminus<25 && indlplus<25) {
      if( (idMc[indlminus]==-11) || (idMc[indlminus]==-13) || (idMc[indlminus]==-15))  
	_theGenEle = indlminus;
      if( (idMc[indlplus]==11) || (idMc[indlplus]==13) || (idMc[indlplus]==15) )
	_theGenPos = indlplus;
    }
    return (indlminus<25 && indlplus<25);
  }
  

  // WW: e / mu / tau
  else if(strcmp(processType,"WW")==0) {
    _process = "WW";
    TVector3 WminusP, WplusP;
    WminusP.SetMagThetaPhi(pMc[6],thetaMc[6],phiMc[6]);
    WplusP.SetMagThetaPhi(pMc[7],thetaMc[7],phiMc[7]);
    float pT = (WminusP+WplusP).Pt();
    _theGenEle = 6;
    _theGenPos = 7;
    _genHiggsPt[0] = pT;
    return (
	    (abs(idMc[6])==24) && (abs(idMc[7])==24) &&
	    (abs(idMc[8])>10 && abs(idMc[8])<19 && abs(idMc[mothMc[8]])==24) &&
	    (abs(idMc[10])>10 && abs(idMc[10])<19 && abs(idMc[mothMc[10]])==24)
	    );
  }
  // w+jets: e / mu / tau
  else if(strcmp(processType,"Wjets")==0) {
    _process = "Wjets";
    return ( ((abs(idMc[8])==11) && abs(idMc[9])==12) || ((abs(idMc[8])==13) && abs(idMc[9])==14) || ((abs(idMc[8])==15) && abs(idMc[9])==16));
  }
  // ttbar: e / mu / tau
  else if(strcmp(processType,"ttbar")==0) {
    _process = "ttbar";
    _theGenEle = 13;
    _theGenPos = 15;
    return ( 
	    abs(idMc[9])==24 && abs(idMc[15])>10 && abs(idMc[15])<19 &&
	    abs(idMc[11])==24 && abs(idMc[13])>10 && abs(idMc[13])<19 &&
	    (idMc[13]*idMc[15]<0)
	    );
  }
  else if(strcmp(processType,"ZZleptonic")==0) {
    _process = "ZZleptonic";
    // 8,9; 10,11 are the daughters of the Z;Z
    return (fabs(idMc[8])>10 && fabs(idMc[8])<19 &&
	    fabs(idMc[9])>10 && fabs(idMc[9])<19 &&
	    fabs(idMc[10])>10 && fabs(idMc[10])<19 &&
	    fabs(idMc[11])>10 && fabs(idMc[11])<19);
  }
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}

float HiggsMLSelection::getkFactor(std::string process) {

  float weight = 1.;
  if((process.compare("Higgs")==0)) {
    // if computed in AOD
    //    weight = evtKfactor;

    // if computed offline
    for(int imc=2;imc<10;imc++) {
      if(idMc[imc]==25 && statusMc[imc]==3) {
        float ptHiggs = pMc[imc]*fabs(sin(thetaMc[imc]));
        return calculator_->evaluate(ptHiggs);
      }
    }
  }
  else if(process.compare("WW")==0) weight = 1.0; // we used MC @ NLO weight in 16X   

  return 1.0;
}

void HiggsMLSelection::Loop() {

  _verbose=false;
  if(fChain == 0) return;
  
  // kinematics reduced tree
  std::string reducedTreeNameEE = _datasetName+"-datasetEE.root";
  std::string reducedTreeNameMM = _datasetName+"-datasetMM.root";
  std::string reducedTreeNameEM = _datasetName+"-datasetEM.root";
  std::string reducedTreeNameME = _datasetName+"-datasetME.root";
  myOutTreeEE = new RedHiggsTree(reducedTreeNameEE.c_str());
  myOutTreeMM = new RedHiggsTree(reducedTreeNameMM.c_str());
  myOutTreeEM = new RedHiggsTree(reducedTreeNameEM.c_str());
  myOutTreeME = new RedHiggsTree(reducedTreeNameME.c_str());

  if ( !_selectionEE->getSwitch("isData") && _selectionEE->getSwitch("apply_kFactor") ) {
    myOutTreeEE->addKFactor();
    myOutTreeMM->addKFactor();
    myOutTreeEM->addKFactor();
    myOutTreeME->addKFactor();
  }

  if(!_selectionEE->getSwitch("isData")) {
    myOutTreeEE->addMcTruthInfos();
    myOutTreeMM->addMcTruthInfos();
    myOutTreeEM->addMcTruthInfos();
    myOutTreeME->addMcTruthInfos();

//     myOutTreeEE->addPDFs();
//     myOutTreeMM->addPDFs();
//     myOutTreeEM->addPDFs();
//     myOutTreeME->addPDFs();
  }

  myOutTreeEE->addRunInfos();
  myOutTreeMM->addRunInfos();
  myOutTreeEM->addRunInfos();
  myOutTreeME->addRunInfos();

  myOutTreeEE->addMLVars();
  myOutTreeMM->addMLVars();
  myOutTreeEM->addMLVars();
  myOutTreeME->addMLVars();

  if (wantJetIdStuff) {
    myOutTreeEE->addJetsVars();
    myOutTreeMM->addJetsVars();
    myOutTreeEM->addJetsVars();
    myOutTreeME->addJetsVars();
  }

  myOutTreeEE->addLatinos();
  myOutTreeMM->addLatinos();
  myOutTreeEM->addLatinos();
  myOutTreeME->addLatinos();

  myOutTreeEE->addKinematics();
  myOutTreeMM->addKinematics();
  myOutTreeEM->addKinematics();
  myOutTreeME->addKinematics();

  myOutTreeEE->addLeptonInfos();
  myOutTreeMM->addLeptonInfos();
  myOutTreeEM->addLeptonInfos();
  myOutTreeME->addLeptonInfos();

  myOutTreeEE->addSystematics();
  myOutTreeMM->addSystematics();
  myOutTreeEM->addSystematics();
  myOutTreeME->addSystematics();

  myOutTreeEE->addRazor();
  myOutTreeMM->addRazor();
  myOutTreeEM->addRazor();
  myOutTreeME->addRazor();

  myOutTreeEE->addMetStudies();
  myOutTreeMM->addMetStudies();
  myOutTreeEM->addMetStudies();
  myOutTreeME->addMetStudies();

  // eleId reduced tree
  std::string reducedEleIdTreeName = _datasetName+"-eleId.root";
  myEleIdTree = new RedEleIDTree(reducedEleIdTreeName.c_str());


  unsigned int lastLumi=0;
  unsigned int lastRun=0;

  /// kfactors
  std::ifstream file("config/higgs/higgsMass.txt");
  std::string var;
  int mh;
  if(!file.good()) {
    std::cout << "Error! Unable to open the mass file. Using kFactor = 1 always!" << std::endl;   
  } else {
    while(!file.eof()) {
      file >> var >> mh;
    }
  }
  
  cout << "higgs mass = " << mh << endl;

  calculator_ = new kFactorEvaluator(mh);

  PUWeight* fPUWeight = new PUWeight();    // Isidro's function
  // LumiReWeighting *fPUWeight;           // For OOT PU reweighting
  // fPUWeight = new LumiReWeighting("/afs/cern.ch/user/c/crovelli/public/puReweighting/recommended_summer11S4_alsoOOT/Recommended_PU.root","/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/Pileup_2011_to_173692_LPLumiScale_68mb.root","pileupMC","pileup");

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;
    
    resetKinematicsStart();

    // get the kFactor of the event (for signal)
    float weight = 1;
    float evtKfactor = 1.0;
    
    // weight for the PU observed in 2011 data
    // float avePU = ((float)(nPU[0]+nPU[1]+nPU[2]))/3.;    // for OOT PU reweighting
    // if ( !_selectionEE->getSwitch("isData") ) weight *= fPUWeight->ITweight3BX(avePU);     
    // cout << fPUWeight->ITweight3BX(avePU) << endl;
    // if ( !_selectionEE->getSwitch("isData") ) weight *= fPUWeight->GetWeight(nPU[1]);    // Isidro's function

    float nputosave[3];
    if( !_selectionEE->getSwitch("isData") ) {
      for(int i=0; i<3; i++) nputosave[i] = float(nPU[i]);
    } else {
      for(int i=0; i<3; i++) nputosave[i] = -1.;
    }

    if (!_selectionEE->getSwitch("isData") && _selectionEE->getSwitch("apply_kFactor")) evtKfactor = getkFactor("Higgs");

    // look to the MC truth decay tree 
    // bool decayEE = findMcTree("HtoWWto2e2nu");
    // bool decayMM = findMcTree("HtoWWto2m2nu");
    // bool decayEM = findMcTree("HtoWWtoem2nu");

    bool promptEE, promptMM, promptEM, promptME;
    promptEE = promptMM = promptEM = promptME = false;
    _genmll = _genptll = _genyll = -1.;
    if( !_selectionEE->getSwitch("isData") ) {
      promptEE = findMcTree("HtoWWto2e2nu_prompt");
      promptMM = findMcTree("HtoWWto2m2nu_prompt");
      promptEM = findMcTree("HtoWWtoem2nu_prompt");  
      promptME = findMcTree("HtoWWtome2nu_prompt");  
      if(TString(_datasetName.c_str()).Contains("DYToEE")) getDYGeneratorKinematics(11);
      if(TString(_datasetName.c_str()).Contains("DYToMuMu")) getDYGeneratorKinematics(13);
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
    //    bool passedHLT = (_selectionEE->getSwitch("isData")) ? hasPassedHLT() : true;
    bool passedHLT[4];
    passedHLT[ee] = hasPassedHLT(ee);
    passedHLT[mm] = hasPassedHLT(mm);
    passedHLT[em] = hasPassedHLT(em);
    passedHLT[me] = hasPassedHLT(em); // same triggers for em and me

    // -------------------------------------------------------------
    // vertex selection - we only consider the first vertex of the list ( = highest sumPT^2)
    bool isGoodVertex = goodPV(0);
    
    // count the number of good PVs (necessary for the MET cut)
    int nGoodPV=0;
    for(int v=0;v<nPV;++v) {
      if(goodPV(v)) nGoodPV++;
    }
    m_goodvertices = nGoodPV;

    // -------------------------------------------------------------
    
    // get the best electrons and best muons ==> tu be used to select ALL the possible channels at the beginning only
    std::pair<int,int> thePreElectrons = getBestElectronPair_acceptance();
    std::pair<int,int> thePreMuons     = getBestMuonPair_acceptance();
    // filling vectors with ele-muons passing the acceptance cuts   
    std::pair<int,int> theBestAcceptEleMuon = getBestEleMuonPair(_acceptEleAll,_acceptMuonsAll);
    // filling vectors with ele-muons passing the acceptance cuts
    std::pair<int,int> theBestAcceptMuonEle = getBestMuonElePair(_acceptEleAll,_acceptMuonsAll);

    thePreElectron  = thePreElectrons.second;
    thePrePositron  = thePreElectrons.first;
    thePreMuonPlus  = thePreMuons.first;
    thePreMuonMinus = thePreMuons.second;
    thePreElectronEM = theBestAcceptEleMuon.first;
    thePreMuonEM = theBestAcceptEleMuon.second;
    thePreElectronME = theBestAcceptMuonEle.second;
    thePreMuonME = theBestAcceptMuonEle.first;

    // reconstructed channel
    m_channel[ee] = false;     
    m_channel[mm] = false;
    m_channel[em] = false;
    m_channel[me] = false;

    // at this level the SELECTED channel should have pT > 10 and > 20. So far, at least 2 leptons with pT >20 and 10 in the event
    if (thePreElectron > -1 && thePrePositron > -1) {
      float thisMaxPt = TMath::Max(GetPt(pxEle[thePreElectron],pyEle[thePreElectron]),GetPt(pxEle[thePrePositron],pyEle[thePrePositron]));
      float thisMinPt = TMath::Min(GetPt(pxEle[thePreElectron],pyEle[thePreElectron]),GetPt(pxEle[thePrePositron],pyEle[thePrePositron]));
      if (isGoodVertex && thisMaxPt>20 && thisMinPt>10) m_channel[ee] = true;    // fixme: hardcoded
    }

    if (thePreMuonPlus > -1 && thePreMuonMinus > -1) {
      float thisMaxPt = TMath::Max(GetPt(pxMuon[thePreMuonMinus],pyMuon[thePreMuonMinus]),GetPt(pxMuon[thePreMuonPlus],pyMuon[thePreMuonPlus]));
      if (isGoodVertex && thisMaxPt>20) m_channel[mm] = true;    // fixme: hardcoded
    }

    if ( thePreElectronEM > -1 && thePreMuonEM > -1 ) {
      float thisMaxPt = GetPt(pxEle[thePreElectronEM],pyEle[thePreElectronEM]);
      if (isGoodVertex && thisMaxPt>20) m_channel[em] = true;    // fixme: hardcoded
    }

    if ( thePreElectronME > -1 && thePreMuonME > -1 ) {
      float thisMaxPt  = GetPt(pxMuon[thePreMuonME],pyMuon[thePreMuonME]);
      float thisMinPt  = GetPt(pxEle[thePreElectronME],pyEle[thePreElectronME]);
      if (isGoodVertex && thisMaxPt>20 && thisMinPt>10) m_channel[me] = true;    // fixme: hardcoded
    }
    
    if (_verbose) {
      std::cout << "nEle = "   << nEle << "\tnMuon = " << nMuon << std::endl;
      std::cout << "indices: " << thePreElectron << " " << thePrePositron << " " << thePreMuonMinus << " " << thePreMuonPlus << std::endl;
      std::cout << "chargeEle = " << chargeEle[thePreElectron] << "\tchargePos = " << chargeEle[thePrePositron] 
		<< "\tchargeMuonMinus = " << chargeMuon[thePreMuonMinus] << "\tchargeMuonPlus = " << chargeMuon[thePreMuonPlus] << std::endl;
      std::cout << "ee = " << m_channel[ee] << "\tmm = " << m_channel[mm] << "\temu = " << m_channel[em] << "\tmue = " << m_channel[me] << std::endl; 
    }



    // -------------------------------------------------------------
    // EE candidates: preparing vectors of candidates and selecting the two highest pT ele- and ele+ after each step - to check the 20 GeV cut after 

    // eleID, for electrons in acceptance
    std::pair<int,int> theBestIdEle = getBestElectronPair_id(_acceptEleAll);   

    // isolation, for identified electrons
    std::pair<int,int> theBestIsolEle = getBestElectronPair_isol(_idEleAll); 

    // conversion rejection, for isolated electrons
    std::pair<int,int> theBestConvEle = getBestElectronPair_conv(_isolEleAll);     

    // transverse impact parameter, for electrons passing conversion rejection
    std::pair<int,int> theBestIpEle = getBestElectronPair_ip(_convEleAll);     

    // the two highest pT electrons at this point are those I use for my analysis since the passed the full lepton selection
    thePositron = theBestIpEle.first;    
    theElectron = theBestIpEle.second;
    
    // to be used on the following
    int theIdElectron(theBestIdEle.second);
    int theIdPositron(theBestIdEle.first);    
    int theIsolElectron(theBestIsolEle.second);
    int theIsolPositron(theBestIsolEle.first);    
    int theConvElectron(theBestConvEle.second);
    int theConvPositron(theBestConvEle.first);    
    int theIpElectron(theBestIpEle.second);
    int theIpPositron(theBestIpEle.first);    
    
    // filling the three to compare the distribution before the cut
    int scElectron = superClusterIndexEle[theElectron];
    int scPositron = superClusterIndexEle[thePositron];
    if (theElectron > -1) myEleIdTree -> fillAll(classificationEle[theElectron], hOverEEle[theElectron], eSuperClusterOverPEle[theElectron], eSeedOverPoutEle[theElectron], deltaEtaAtVtxEle[theElectron], deltaPhiAtVtxEle[theElectron], sqrt(covIEtaIEtaSC[scElectron]));
    if (thePositron > -1) myEleIdTree -> fillAll(classificationEle[thePositron], hOverEEle[thePositron], eSuperClusterOverPEle[thePositron], eSeedOverPoutEle[thePositron], deltaEtaAtVtxEle[thePositron], deltaPhiAtVtxEle[thePositron], sqrt(covIEtaIEtaSC[scPositron]));
    myEleIdTree->store();


    // -------------------------------------------------------------
    // MM candidates: preparing vectors of candidates and selecting the two highest pT mu- and mu+ after each step - to check the 20 GeV cut after 

    // muID, for muons in acceptance
    std::pair<int,int> theBestIdMuon = getBestMuonPair_id(_acceptMuonsAll); 

    // isolation, for identified muons
    std::pair<int,int> theBestIsolMuon = getBestMuonPair_isol(_idMuonsAll); 

    // transverse impact parameter, for isolated muons
    std::pair<int,int> theBestIpMuon = getBestMuonPair_ip(_isolMuonsAll);     

    // the two highest pT muons at this point are those I use for my analysis since the passed the full lepton selection
    theMuonPlus  = theBestIpMuon.first;
    theMuonMinus = theBestIpMuon.second;    

    // to be used in the following
    int theIdMuonMinus(theBestIdMuon.second);
    int theIdMuonPlus(theBestIdMuon.first);
    int theIsolMuonMinus(theBestIsolMuon.second);
    int theIsolMuonPlus(theBestIsolMuon.first);
    int theIpMuonMinus(theBestIpMuon.second);
    int theIpMuonPlus(theBestIpMuon.first);

    // -------------------------------------------------------------
    // EM candidates: preparing vectors of candidates and selecting the two highest pT ele+- and muon-+ after each step - to check the 20 GeV cut after
    
    // leptonID, for leptons in acceptance
    std::pair<int,int> theBestIdEleMuon = getBestEleMuonPair(_idEleAll,_idMuonsAll);
    
    // isolation, for identified leptons
    std::pair<int,int> theBestIsolEleMuon = getBestEleMuonPair(_isolEleAll,_isolMuonsAll);

    // conversion rejection, for isolated leptons
    std::pair<int,int> theBestConvEleMuon = getBestEleMuonPair(_convEleAll,_isolMuonsAll);

    // transverse impact parameter, for leptons passing conversion rejection
    std::pair<int,int> theBestIpEleMuon = getBestEleMuonPair(_ipEleAll,_ipMuonsAll);


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
    // set of kinematics: : now I've all the final leptons 
    resetKinematics();
    
    // MET is an event variable. Independent o the channel
    TVector3 pfmet(pxPFMet[0],pyPFMet[0],pzPFMet[0]);
    int sample=kMC2012;
    if(_selectionEE->getSwitch("isData")) sample=kDATA2012;
    TVector3 corrpfmet = XYCorrectedMet(pfmet,sample);
    m_p3PFMET->SetXYZ(corrpfmet.x(),corrpfmet.y(),corrpfmet.z());
    m_theMET = m_p3PFMET->Pt();

    setKinematicsEE(theElectron, thePositron);
    setKinematicsMM(theMuonMinus, theMuonPlus);
    setKinematicsEMME(theElectron, thePositron, theMuonPlus, theMuonMinus);

    // -------------------------------------------------------------    
    // look for PV in the event (there is always at least 1 PV)
    // m_closestPV = getPV();    // fixme: si chiama closest ma e' quello a piu' alto pT. 
    
    float dphiLLJ[4];
    float btag[4];
    int nsoftmu[4], nsoftmunojets[4], nextraleptons[4];
    float jesMtUp[4], jesMtDown[4];

    // initialize the btags for the leading and subleading jets to unphysical value
    for(int ichan=0; ichan<4; ichan++) {
      leadJetBtag[ichan] = -2000.;
      subleadJetBtag[ichan] = -2000.;
      subLeadJetsMaxBtag[ichan] = -2000.;
      leadJetBtagBProb[ichan] = -2000.;
      subleadJetBtagBProb[ichan] = -2000.;
      subLeadJetsMaxBtagBProb[ichan] = -2000.;
      nsoftjets[ichan] = 0;
    }

    // initialize variables for jetId studies
    if (wantJetIdStuff) {
      for(int ichan=0; ichan<4; ichan++) {
	leadJetPt[ichan]        = -2000.;
	leadJetEta[ichan]       = -2000.;
	leadJetLoosePFId[ichan] = -2000;
	leadJetMatchGen[ichan]  = -2000;
	leadJetMvaJetId[ichan]  = -2000.;
	leadJetLooseId[ichan]   = -2000;
	//
	subleadJetPt[ichan]        = -2000.;
	subleadJetEta[ichan]       = -2000.;
	subleadJetLoosePFId[ichan] = -2000;
	subleadJetMatchGen[ichan]  = -2000;
	subleadJetMvaJetId[ichan]  = -2000.;
	subleadJetLooseId[ichan]   = -2000;
      }    
    }
    
    for(int ichan=0; ichan<4; ichan++) {

      // jet counter
      njets[ichan] = numJets(eleCands[ichan],muCands[ichan],ichan);
      nuncorrjets[ichan] = numUncorrJets(eleCands[ichan],muCands[ichan],ichan);

      // if 1-jet bin, use deltaphi(ll-jet)
      dphiLLJ[ichan] = deltaPhiLLJet(ichan);   

      // b veto
      btag[ichan] = bVetoJets(eleCands[ichan],muCands[ichan],ichan);
      
      // soft muon counter
      // w/o jet cleaning (for the selection)
      std::vector<int> emptyJets;
      emptyJets.clear();
      nsoftmu[ichan] = numSoftMuons(muCands[ichan],emptyJets);

      // w jet cleaning (for the top estimation)
      nsoftmunojets[ichan] = numSoftMuons(muCands[ichan],m_goodJets);

      // extra lepton counter
      nextraleptons[ichan] = numExtraLeptons(eleCands[ichan],muCands[ichan]);
      
      // DY MVA
      m_dymva[ichan] = getDYMVA(ichan);

      // calculate the smeared MET/MT // activate only when doing systematics, otherwise very slow
//       JESPfMet(eleCands[ichan],muCands[ichan]);
//       jesMtUp[ichan] = (transvMassJES(ichan)).first;
//       jesMtDown[ichan] = (transvMassJES(ichan)).second;
    }



    float genPtHiggs = -1.;
    if ( !_selectionEE->getSwitch("isData") ) {
      for(int imc=2;imc<10;imc++) {
        if(idMc[imc]==25 && statusMc[imc]==3) genPtHiggs = pMc[imc]*fabs(sin(thetaMc[imc]));
      }}
    float kfW = ( !_selectionEE->getSwitch("isData") && _selectionEE->getSwitch("apply_kFactor") ) ? evtKfactor : 1.0;

    // ---------------------------------------
    // filling counters for the different final states

    // EE
    CutBasedHiggsSelectionEE.SetWeight(weight);               
    CutBasedHiggsSelectionEE.SetMcTruth(promptEE);  
    CutBasedHiggsSelectionEE.SetHLT(passedHLT[ee]);               
    CutBasedHiggsSelectionEE.SetIsChannel(m_channel[ee]);     
    
    CutBasedHiggsSelectionEE.SetElectronId(theIdElectron);                 
    CutBasedHiggsSelectionEE.SetPositronId(theIdPositron);                 
    CutBasedHiggsSelectionEE.SetElectronIsolation(theIsolElectron);        
    CutBasedHiggsSelectionEE.SetPositronIsolation(theIsolPositron);        
    CutBasedHiggsSelectionEE.SetElectronConvRejection(theConvElectron);    
    CutBasedHiggsSelectionEE.SetPositronConvRejection(theConvPositron);    
    CutBasedHiggsSelectionEE.SetElectronIp(theIpElectron);                 
    CutBasedHiggsSelectionEE.SetPositronIp(theIpPositron);                 
    // checking if the highest pT electron at each step has pT>20
    float thisMaxPtIdEE   = TMath::Max(GetPt(pxEle[theIdElectron],pyEle[theIdElectron]),GetPt(pxEle[theIdPositron],pyEle[theIdPositron]));
    float thisMaxPtIsolEE = TMath::Max(GetPt(pxEle[theIsolElectron],pyEle[theIsolElectron]),GetPt(pxEle[theIsolPositron],pyEle[theIsolPositron]));
    float thisMaxPtConvEE = TMath::Max(GetPt(pxEle[theConvElectron],pyEle[theConvElectron]),GetPt(pxEle[theConvPositron],pyEle[theConvPositron]));
    float thisMaxPtIpEE   = TMath::Max(GetPt(pxEle[theIpElectron],pyEle[theIpElectron]),GetPt(pxEle[theIpPositron],pyEle[theIpPositron]));
    if (thisMaxPtIdEE<20)   { 
      CutBasedHiggsSelectionEE.SetElectronId(-1);
      CutBasedHiggsSelectionEE.SetPositronId(-1);
    }
    if (thisMaxPtIsolEE<20) { 
      CutBasedHiggsSelectionEE.SetElectronIsolation(-1);
      CutBasedHiggsSelectionEE.SetPositronIsolation(-1);
    }
    if (thisMaxPtConvEE<20) { 
      CutBasedHiggsSelectionEE.SetElectronConvRejection(-1);
      CutBasedHiggsSelectionEE.SetPositronConvRejection(-1);
    }
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
    CutBasedHiggsSelectionEE.SetNvtx(nGoodPV);
    CutBasedHiggsSelectionEE.SetMetOverPtLL(m_metOptll[ee]);
    CutBasedHiggsSelectionEE.SetDYMVA(m_dymva[ee]);
    CutBasedHiggsSelectionEE.SetDeltaPhiLLJet(dphiLLJ[ee]);   
    CutBasedHiggsSelectionEE.SetDeltaPhi(m_deltaPhi[ee]);
    CutBasedHiggsSelectionEE.SetInvMass(m_mll[ee]);
    CutBasedHiggsSelectionEE.SetDetaLeptons(m_deltaEtaLeptons[ee]);
    CutBasedHiggsSelectionEE.SetPtll(m_dilepPt[ee].Pt());
    // CutBasedHiggsSelectionEE.SetWWInvMass(2.*m_transvMass[ee]/_massVal);
    CutBasedHiggsSelectionEE.SetWWInvMass(m_transvMass[ee]);

    bool isSelectedEE           = CutBasedHiggsSelectionEE.output();    
    bool selUpToFinalLeptonsEE  = CutBasedHiggsSelectionEE.outputUpToFinalLeptons();
    bool selUpToJetVetoEE       = CutBasedHiggsSelectionEE.outputUpToJetVeto();
    bool selUpToUncorrJetVetoEE = CutBasedHiggsSelectionEE.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiEE       = CutBasedHiggsSelectionEE.outputPreDeltaPhi();

    // latinos
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

    if(!_selectionEE->getSwitch("isData")) {
      myOutTreeEE -> fillMcTruth(promptEE,_genmll,_genptll,_genyll);
      //      myOutTreeEE -> fillPDFs(wCTEQ66,wMRST2006NNLO,wNNPDF10100);
    }

    myOutTreeEE->fillRunInfos(runNumber, lumiBlock, eventNumber, weight, passedHLT[ee], nputosave);

    int theLJ  = theLeadingJet[ee];
    int theSJ  = theSecondJet[ee];

    float ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeEE->fillKFactor(kfW, genPtHiggs, ptLJ);

    myOutTreeEE -> fillAll(m_chMet[ee], GetPt(m_p3PFMET->x(),m_p3PFMET->y()), GetPt(pxMet[0],pyMet[0]), 
			   m_projectedMet[ee], m_deltaPhi[ee], m_deltaErre[ee], m_transvMass[ee], m_mll[ee], 
			   hardestLeptonPt[ee], slowestLeptonPt[ee], m_deltaEtaLeptons[ee], nGoodPV,
			   selUpToFinalLeptonsEE, selUpToJetVetoEE, selUpToUncorrJetVetoEE, selPreDeltaPhiEE, isSelectedEE);

    myOutTreeEE -> fillMLVars(njets[ee], nuncorrjets[ee], m_maxDxyEvt, m_maxDszEvt, m_softbdisc[ee], m_hardbdisc[ee], m_maxCombinedSecondaryVertexMVABJetTags, 
                              nsoftmu[ee], leadJetBtag[ee], subleadJetBtag[ee], subLeadJetsMaxBtag[ee], 
                              leadJetBtagBProb[ee], subleadJetBtagBProb[ee], subLeadJetsMaxBtagBProb[ee],
                              nextraleptons[ee], nsoftmunojets[ee], m_numbtagjets[ee], nsoftjets[ee]);

    if (wantJetIdStuff) 
      myOutTreeEE -> fillJetsVars(leadJetPt[ee], leadJetEta[ee], leadJetLoosePFId[ee], leadJetMatchGen[ee], leadJetMvaJetId[ee], leadJetLooseId[ee], subleadJetPt[ee], subleadJetEta[ee], subleadJetLoosePFId[ee], subleadJetMatchGen[ee], subleadJetMvaJetId[ee], subleadJetLooseId[ee]);


    myOutTreeEE -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, 
                                outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, 
                                outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26 ); 

    myOutTreeEE -> fillRazor(m_MTR[ee], m_MR[ee], m_GammaMR[ee]);
    
    if(eleCands[ee].size()==2) setLepIdVariables((eleCands[ee])[0],(eleCands[ee])[1],11,11);
    else setLepIdVariables(-1,-1,11,11);
    myOutTreeEE -> fillLeptons(myPt, myEta, myPhi, myLepId, myLepIso, myConv);

    myOutTreeEE -> fillMetStudies( m_projectedPFMet[ee], m_projectedTkMet[ee], significancePFMet[0], significancePFChMet[0], m_MTRcharged[ee], m_dymva[ee], rhoFastjet, rhoJetsFastJet );

    std::vector<TLorentzVector> jesLJ = GetJetJesPcomponent(theLJ);
    std::vector<TLorentzVector> jesSJ = GetJetJesPcomponent(theSJ);
    float pxLJEE[3] = { jesLJ[0].Px(), jesLJ[1].Px(), jesLJ[2].Px() };   float pyLJEE[3] = { jesLJ[0].Py(), jesLJ[1].Py(), jesLJ[2].Py() };   float pzLJEE[3] = { jesLJ[0].Pz(), jesLJ[1].Pz(), jesLJ[2].Pz() };
    float pxSJEE[3] = { jesSJ[0].Px(), jesSJ[1].Px(), jesSJ[2].Px() };   float pySJEE[3] = { jesSJ[0].Py(), jesSJ[1].Py(), jesSJ[2].Py() };   float pzSJEE[3] = { jesSJ[0].Pz(), jesSJ[1].Pz(), jesSJ[2].Pz() };

    if ( GetPt(m_p4LeptonPlus[ee]->Px(),m_p4LeptonPlus[ee]->Py()) > GetPt(m_p4LeptonMinus[ee]->Px(),m_p4LeptonMinus[ee]->Py()) ) {
      myOutTreeEE -> fillKinematics( m_p3TKMET[ee].Px(), m_p3TKMET[ee].Py(), m_p3TKMET[ee].Pz(),
                                     pxLJEE, pyLJEE, pzLJEE, pxSJEE, pySJEE, pzSJEE,
				     m_p4LeptonPlus[ee]->Px(), m_p4LeptonPlus[ee]->Py(), m_p4LeptonPlus[ee]->Pz(),
				     m_p4LeptonMinus[ee]->Px(), m_p4LeptonMinus[ee]->Py(), m_p4LeptonMinus[ee]->Pz(),
                                     m_chEE,
                                     m_jetsSum[ee], m_uncorrJetsSum[ee], m_p3PFMET); 
      myOutTreeEE -> fillSystematics( mySCEnergy, myR9, m_p4LeptonPlusEnergy[ee], m_p4LeptonMinusEnergy[ee], m_p4PlusType[ee], m_p4MinusType[ee],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[ee], jesMtDown[ee]);
    } else { 
      myOutTreeEE -> fillKinematics( m_p3TKMET[ee].Px(), m_p3TKMET[ee].Py(), m_p3TKMET[ee].Pz(),
                                     pxLJEE, pyLJEE, pzLJEE, pxSJEE, pySJEE, pzSJEE,
				     m_p4LeptonMinus[ee]->Px(), m_p4LeptonMinus[ee]->Py(), m_p4LeptonMinus[ee]->Pz(),
				     m_p4LeptonPlus[ee]->Px(), m_p4LeptonPlus[ee]->Py(), m_p4LeptonPlus[ee]->Pz(),
                                     m_chEE,
                                     m_jetsSum[ee], m_uncorrJetsSum[ee], m_p3PFMET);
      myOutTreeEE -> fillSystematics( mySCEnergy, myR9, m_p4LeptonMinusEnergy[ee], m_p4LeptonPlusEnergy[ee], m_p4MinusType[ee], m_p4PlusType[ee],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[ee], jesMtDown[ee]);
    }
    
    // dumping final tree, only if there are 2 final leptons
    if(selUpToFinalLeptonsEE) myOutTreeEE -> store();


    
    // ---------------------------------------
    // MM
    CutBasedHiggsSelectionMM.SetWeight(weight);               
    CutBasedHiggsSelectionMM.SetMcTruth(promptMM); 
    CutBasedHiggsSelectionMM.SetHLT(passedHLT[mm]);               
    CutBasedHiggsSelectionMM.SetIsChannel(m_channel[mm]);     
    
    CutBasedHiggsSelectionMM.SetElectronId(theIdMuonMinus);                 
    CutBasedHiggsSelectionMM.SetPositronId(theIdMuonPlus);                 
    CutBasedHiggsSelectionMM.SetElectronIsolation(theIsolMuonMinus);        
    CutBasedHiggsSelectionMM.SetPositronIsolation(theIsolMuonPlus);        
    CutBasedHiggsSelectionMM.SetElectronConvRejection(true);    
    CutBasedHiggsSelectionMM.SetPositronConvRejection(true);    
    CutBasedHiggsSelectionMM.SetElectronIp(theIpMuonMinus);                 
    CutBasedHiggsSelectionMM.SetPositronIp(theIpMuonPlus);                 
    // checking if the highest pT electron at each step has pT>20
    float thisMaxPtIdMM   = TMath::Max(GetPt(pxMuon[theIdMuonMinus],pyMuon[theIdMuonMinus]),GetPt(pxMuon[theIdMuonPlus],pyMuon[theIdMuonPlus]));
    float thisMaxPtIsolMM = TMath::Max(GetPt(pxMuon[theIsolMuonMinus],pyMuon[theIsolMuonMinus]),GetPt(pxMuon[theIsolMuonPlus],pyMuon[theIsolMuonPlus]));
    float thisMaxPtIpMM   = TMath::Max(GetPt(pxMuon[theIpMuonMinus],pyMuon[theIpMuonMinus]),GetPt(pxMuon[theIpMuonPlus],pyMuon[theIpMuonPlus]));
    if (thisMaxPtIdMM<20)   { 
      CutBasedHiggsSelectionMM.SetElectronId(-1);
      CutBasedHiggsSelectionMM.SetPositronId(-1);
    }
    if (thisMaxPtIsolMM<20) { 
      CutBasedHiggsSelectionMM.SetElectronIsolation(-1);
      CutBasedHiggsSelectionMM.SetPositronIsolation(-1);
    }
    if (thisMaxPtIpMM<20)   { 
      CutBasedHiggsSelectionMM.SetElectronIp(-1);
      CutBasedHiggsSelectionMM.SetPositronIp(-1);
    }

    CutBasedHiggsSelectionMM.SetHighElePt(hardestLeptonPt[mm]); 
    CutBasedHiggsSelectionMM.SetLowElePt(slowestLeptonPt[mm]);  

    CutBasedHiggsSelectionMM.SetNJets(njets[mm]);
    CutBasedHiggsSelectionMM.SetNUncorrJets(nuncorrjets[mm]);
    CutBasedHiggsSelectionMM.SetBTagJets(btag[mm]);
    CutBasedHiggsSelectionMM.SetNSoftMuons(nsoftmu[mm]);
    CutBasedHiggsSelectionMM.SetNExtraLeptons(nextraleptons[mm]);
    CutBasedHiggsSelectionMM.SetMet(m_theMET);					
    CutBasedHiggsSelectionMM.SetProjectedMet(m_projectedMet[mm]);
    CutBasedHiggsSelectionMM.SetNvtx(nGoodPV);
    CutBasedHiggsSelectionMM.SetMetOverPtLL(m_metOptll[mm]);
    CutBasedHiggsSelectionMM.SetDYMVA(m_dymva[mm]);
    CutBasedHiggsSelectionMM.SetDeltaPhiLLJet(dphiLLJ[mm]);   
    CutBasedHiggsSelectionMM.SetDeltaPhi(m_deltaPhi[mm]);
    CutBasedHiggsSelectionMM.SetInvMass(m_mll[mm]);
    CutBasedHiggsSelectionMM.SetDetaLeptons(m_deltaEtaLeptons[mm]);
    CutBasedHiggsSelectionMM.SetPtll(m_dilepPt[mm].Pt());
    // CutBasedHiggsSelectionMM.SetWWInvMass(2.*m_transvMass[mm]/_massVal);
    CutBasedHiggsSelectionMM.SetWWInvMass(m_transvMass[mm]);

    bool isSelectedMM           = CutBasedHiggsSelectionMM.output();    
    bool selUpToFinalLeptonsMM  = CutBasedHiggsSelectionMM.outputUpToFinalLeptons();
    bool selUpToJetVetoMM       = CutBasedHiggsSelectionMM.outputUpToJetVeto();
    bool selUpToUncorrJetVetoMM = CutBasedHiggsSelectionMM.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiMM       = CutBasedHiggsSelectionMM.outputPreDeltaPhi();

    // latinos
    outputStep0  = CutBasedHiggsSelectionMM.outputStep0();
    outputStep1  = CutBasedHiggsSelectionMM.outputStep1();
    outputStep2  = CutBasedHiggsSelectionMM.outputStep2();
    outputStep3  = CutBasedHiggsSelectionMM.outputStep3();
    outputStep4  = CutBasedHiggsSelectionMM.outputStep4();
    outputStep5  = CutBasedHiggsSelectionMM.outputStep5();
    outputStep6  = CutBasedHiggsSelectionMM.outputStep6();
    outputStep7  = CutBasedHiggsSelectionMM.outputStep7();
    outputStep8  = CutBasedHiggsSelectionMM.outputStep8();
    outputStep9  = CutBasedHiggsSelectionMM.outputStep9();
    outputStep10 = CutBasedHiggsSelectionMM.outputStep10();
    outputStep11 = CutBasedHiggsSelectionMM.outputStep11();
    outputStep12 = CutBasedHiggsSelectionMM.outputStep12();
    outputStep13 = CutBasedHiggsSelectionMM.outputStep13();
    outputStep14 = CutBasedHiggsSelectionMM.outputStep14();
    outputStep15 = CutBasedHiggsSelectionMM.outputStep15();
    outputStep16 = CutBasedHiggsSelectionMM.outputStep16();
    outputStep17 = CutBasedHiggsSelectionMM.outputStep17();
    outputStep18 = CutBasedHiggsSelectionMM.outputStep18();
    outputStep19 = CutBasedHiggsSelectionMM.outputStep19();
    outputStep20 = CutBasedHiggsSelectionMM.outputStep20();
    outputStep21 = CutBasedHiggsSelectionMM.outputStep21();
    outputStep22 = CutBasedHiggsSelectionMM.outputStep22();
    outputStep23 = CutBasedHiggsSelectionMM.outputStep23();
    outputStep24 = CutBasedHiggsSelectionMM.outputStep24();
    outputStep25 = CutBasedHiggsSelectionMM.outputStep25();
    outputStep26 = CutBasedHiggsSelectionMM.outputStep26();


    // filling the tree
    if(!_selectionMM->getSwitch("isData")) {
      myOutTreeMM -> fillMcTruth(promptMM,_genmll,_genptll,_genyll);
      //      myOutTreeMM -> fillPDFs(wCTEQ66,wMRST2006NNLO,wNNPDF10100);
    }
    myOutTreeMM->fillRunInfos(runNumber, lumiBlock, eventNumber, weight, passedHLT[mm], nputosave);
    
    theLJ  = theLeadingJet[mm];
    theSJ  = theSecondJet[mm];
    ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeMM->fillKFactor(kfW, genPtHiggs, ptLJ);

    myOutTreeMM -> fillAll(m_chMet[mm], GetPt(m_p3PFMET->x(),m_p3PFMET->y()), GetPt(pxMet[0],pyMet[0]), 
			   m_projectedMet[mm], m_deltaPhi[mm], m_deltaErre[mm], m_transvMass[mm], m_mll[mm], 
			   hardestLeptonPt[mm], slowestLeptonPt[mm], m_deltaEtaLeptons[mm], nGoodPV,
			   selUpToFinalLeptonsMM, selUpToJetVetoMM, selUpToUncorrJetVetoMM, selPreDeltaPhiMM, isSelectedMM);

    if(muCands[mm].size()==2) setLepIdVariables((muCands[mm])[0],(muCands[mm])[1],13,13);
    else setLepIdVariables(-1,-1,13,13);
    myOutTreeMM -> fillLeptons(myPt, myEta, myPhi, myLepId, myLepIso, myConv);

    myOutTreeMM -> fillMLVars(njets[mm], nuncorrjets[mm], m_maxDxyEvt, m_maxDszEvt, m_softbdisc[mm], m_hardbdisc[mm], m_maxCombinedSecondaryVertexMVABJetTags, 
                              nsoftmu[mm], leadJetBtag[mm], subleadJetBtag[mm], subLeadJetsMaxBtag[mm], 
                              leadJetBtagBProb[mm], subleadJetBtagBProb[mm], subLeadJetsMaxBtagBProb[mm],
                              nextraleptons[mm], nsoftmunojets[mm], m_numbtagjets[mm], nsoftjets[mm]);

    if (wantJetIdStuff)     
      myOutTreeMM -> fillJetsVars(leadJetPt[mm], leadJetEta[mm], leadJetLoosePFId[mm], leadJetMatchGen[mm], leadJetMvaJetId[mm], leadJetLooseId[mm], subleadJetPt[mm], subleadJetEta[mm], subleadJetLoosePFId[mm], subleadJetMatchGen[mm], subleadJetMvaJetId[mm], subleadJetLooseId[mm]);

    myOutTreeMM -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, 
                                outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, 
                                outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26 ); 

    myOutTreeMM -> fillRazor(m_MTR[mm], m_MR[mm], m_GammaMR[mm]);

    myOutTreeMM -> fillMetStudies( m_projectedPFMet[mm], m_projectedTkMet[mm], significancePFMet[0], significancePFChMet[0], m_MTRcharged[mm], m_dymva[mm], rhoFastjet, rhoJetsFastJet);   

    jesLJ = GetJetJesPcomponent(theLJ);
    jesSJ = GetJetJesPcomponent(theSJ);
    float pxLJMM[3] = { jesLJ[0].Px(), jesLJ[1].Px(), jesLJ[2].Px() };   float pyLJMM[3] = { jesLJ[0].Py(), jesLJ[1].Py(), jesLJ[2].Py() };   float pzLJMM[3] = { jesLJ[0].Pz(), jesLJ[1].Pz(), jesLJ[2].Pz() };
    float pxSJMM[3] = { jesSJ[0].Px(), jesSJ[1].Px(), jesSJ[2].Px() };   float pySJMM[3] = { jesSJ[0].Py(), jesSJ[1].Py(), jesSJ[2].Py() };   float pzSJMM[3] = { jesSJ[0].Pz(), jesSJ[1].Pz(), jesSJ[2].Pz() };

    if ( GetPt(m_p4LeptonPlus[mm]->Px(),m_p4LeptonPlus[mm]->Py()) > GetPt(m_p4LeptonMinus[mm]->Px(),m_p4LeptonMinus[mm]->Py()) ) {
      
      float dummyV[2];
      for (int ii=0; ii<2; ii++) dummyV[ii] = -999.;

      myOutTreeMM -> fillKinematics( m_p3TKMET[mm].Px(), m_p3TKMET[mm].Py(), m_p3TKMET[mm].Pz(),
                                     pxLJMM, pyLJMM, pzLJMM, pxSJMM, pySJMM, pzSJMM,
				     m_p4LeptonPlus[mm]->Px(), m_p4LeptonPlus[mm]->Py(), m_p4LeptonPlus[mm]->Pz(),
				     m_p4LeptonMinus[mm]->Px(), m_p4LeptonMinus[mm]->Py(), m_p4LeptonMinus[mm]->Pz(),
                                     m_chMM,
                                     m_jetsSum[mm], m_uncorrJetsSum[mm], m_p3PFMET); 
      myOutTreeMM -> fillSystematics( dummyV, dummyV, m_p4LeptonPlusEnergy[mm], m_p4LeptonMinusEnergy[mm], m_p4PlusType[mm], m_p4MinusType[mm],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[mm], jesMtDown[mm]);
    } else { 

      float dummyV[2];
      for (int ii=0; ii<2; ii++) dummyV[ii] = -999.;

      myOutTreeMM -> fillKinematics( m_p3TKMET[mm].Px(), m_p3TKMET[mm].Py(), m_p3TKMET[mm].Pz(),
                                     pxLJMM, pyLJMM, pzLJMM, pxSJMM, pySJMM, pzSJMM,
				     m_p4LeptonMinus[mm]->Px(), m_p4LeptonMinus[mm]->Py(), m_p4LeptonMinus[mm]->Pz(),
				     m_p4LeptonPlus[mm]->Px(), m_p4LeptonPlus[mm]->Py(), m_p4LeptonPlus[mm]->Pz(),
                                     m_chMM,
                                     m_jetsSum[mm], m_uncorrJetsSum[mm], m_p3PFMET);
      myOutTreeMM -> fillSystematics( dummyV, dummyV, m_p4LeptonMinusEnergy[mm], m_p4LeptonPlusEnergy[mm], m_p4MinusType[mm], m_p4PlusType[mm],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[mm], jesMtDown[mm] );
    }
    
    // dumping final tree, only if there are 2 final leptons
    if(selUpToFinalLeptonsMM) myOutTreeMM -> store();




    // ---------------------------------------
    // EM
    CutBasedHiggsSelectionEM.SetWeight(weight);               
    CutBasedHiggsSelectionEM.SetMcTruth(promptEM);  
    CutBasedHiggsSelectionEM.SetHLT(passedHLT[em]);               
    CutBasedHiggsSelectionEM.SetIsChannel(m_channel[em]);     

    CutBasedHiggsSelectionEM.SetElectronId(theBestIdEleMuon.first);                 
    CutBasedHiggsSelectionEM.SetPositronId(theBestIdEleMuon.second);                 
    
    CutBasedHiggsSelectionEM.SetElectronIsolation(theBestIsolEleMuon.first);        
    CutBasedHiggsSelectionEM.SetPositronIsolation(theBestIsolEleMuon.second);        
    CutBasedHiggsSelectionEM.SetElectronConvRejection(theBestConvEleMuon.first);    
    CutBasedHiggsSelectionEM.SetPositronConvRejection(theBestConvEleMuon.second);    
    CutBasedHiggsSelectionEM.SetElectronIp(theBestIpEleMuon.first);                 
    CutBasedHiggsSelectionEM.SetPositronIp(theBestIpEleMuon.second);                 
    // checking if the highest pT electron at each step has pT>20. E is the hardest in EM
    float thisMaxPtIdEM   = (theBestIdEleMuon.first > -1) ? GetPt(pxEle[theBestIdEleMuon.first],pyEle[theBestIdEleMuon.first]) : 0.;
    float thisMaxPtIsolEM = (theBestIsolEleMuon.first > -1) ? GetPt(pxEle[theBestIsolEleMuon.first],pyEle[theBestIsolEleMuon.first]) : 0.;
    float thisMaxPtIpEM   = (theBestIpEleMuon.first > -1) ? GetPt(pxEle[theBestIpEleMuon.first],pyEle[theBestIpEleMuon.first]) : 0.;
    if (thisMaxPtIdEM<20)   { 
      CutBasedHiggsSelectionEM.SetElectronId(-1);
      CutBasedHiggsSelectionEM.SetPositronId(-1);
    }
    if (thisMaxPtIsolEM<20) { 
      CutBasedHiggsSelectionEM.SetElectronIsolation(-1);
      CutBasedHiggsSelectionEM.SetPositronIsolation(-1);
    }
    if (thisMaxPtIpEM<20)   { 
      CutBasedHiggsSelectionEM.SetElectronIp(-1);
      CutBasedHiggsSelectionEM.SetPositronIp(-1);
    }

    CutBasedHiggsSelectionEM.SetHighElePt(hardestLeptonPt[em]); 
    CutBasedHiggsSelectionEM.SetLowElePt(slowestLeptonPt[em]);  

    CutBasedHiggsSelectionEM.SetNJets(njets[em]);
    CutBasedHiggsSelectionEM.SetNUncorrJets(nuncorrjets[em]);
    CutBasedHiggsSelectionEM.SetBTagJets(btag[em]);
    CutBasedHiggsSelectionEM.SetNSoftMuons(nsoftmu[em]);
    CutBasedHiggsSelectionEM.SetNExtraLeptons(nextraleptons[em]);
    CutBasedHiggsSelectionEM.SetMet(m_theMET);					
    CutBasedHiggsSelectionEM.SetProjectedMet(m_projectedMet[em]);
    CutBasedHiggsSelectionEM.SetNvtx(nGoodPV);
    CutBasedHiggsSelectionEM.SetMetOverPtLL(m_metOptll[em]);
    CutBasedHiggsSelectionEM.SetDYMVA(m_dymva[em]);
    CutBasedHiggsSelectionEM.SetDeltaPhiLLJet(dphiLLJ[em]);  
    CutBasedHiggsSelectionEM.SetDeltaPhi(m_deltaPhi[em]);
    CutBasedHiggsSelectionEM.SetInvMass(m_mll[em]);
    CutBasedHiggsSelectionEM.SetDetaLeptons(m_deltaEtaLeptons[em]);
    CutBasedHiggsSelectionEM.SetPtll(m_dilepPt[em].Pt());
    // CutBasedHiggsSelectionEM.SetWWInvMass(2.*m_transvMass[em]/_massVal);
    CutBasedHiggsSelectionEM.SetWWInvMass(m_transvMass[em]);

    bool isSelectedEM           = CutBasedHiggsSelectionEM.output();    
    bool selUpToFinalLeptonsEM  = CutBasedHiggsSelectionEM.outputUpToFinalLeptons();
    bool selUpToJetVetoEM       = CutBasedHiggsSelectionEM.outputUpToJetVeto();
    bool selUpToUncorrJetVetoEM = CutBasedHiggsSelectionEM.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiEM       = CutBasedHiggsSelectionEM.outputPreDeltaPhi();

    // latinos
    outputStep0  = CutBasedHiggsSelectionEM.outputStep0();
    outputStep1  = CutBasedHiggsSelectionEM.outputStep1();
    outputStep2  = CutBasedHiggsSelectionEM.outputStep2();
    outputStep3  = CutBasedHiggsSelectionEM.outputStep3();
    outputStep4  = CutBasedHiggsSelectionEM.outputStep4();
    outputStep5  = CutBasedHiggsSelectionEM.outputStep5();
    outputStep6  = CutBasedHiggsSelectionEM.outputStep6();
    outputStep7  = CutBasedHiggsSelectionEM.outputStep7();
    outputStep8  = CutBasedHiggsSelectionEM.outputStep8();
    outputStep9  = CutBasedHiggsSelectionEM.outputStep9();
    outputStep10 = CutBasedHiggsSelectionEM.outputStep10();
    outputStep11 = CutBasedHiggsSelectionEM.outputStep11();
    outputStep12 = CutBasedHiggsSelectionEM.outputStep12();
    outputStep13 = CutBasedHiggsSelectionEM.outputStep13();
    outputStep14 = CutBasedHiggsSelectionEM.outputStep14();
    outputStep15 = CutBasedHiggsSelectionEM.outputStep15();
    outputStep16 = CutBasedHiggsSelectionEM.outputStep16();
    outputStep17 = CutBasedHiggsSelectionEM.outputStep17();
    outputStep18 = CutBasedHiggsSelectionEM.outputStep18();
    outputStep19 = CutBasedHiggsSelectionEM.outputStep19();
    outputStep20 = CutBasedHiggsSelectionEM.outputStep20();
    outputStep21 = CutBasedHiggsSelectionEM.outputStep21();
    outputStep22 = CutBasedHiggsSelectionEM.outputStep22();
    outputStep23 = CutBasedHiggsSelectionEM.outputStep23();
    outputStep24 = CutBasedHiggsSelectionEM.outputStep24();
    outputStep25 = CutBasedHiggsSelectionEM.outputStep25();
    outputStep26 = CutBasedHiggsSelectionEM.outputStep26();

    // filling the tree
    if(!_selectionEM->getSwitch("isData")) {
      myOutTreeEM -> fillMcTruth(promptEM,_genmll,_genptll,_genyll);
      //      myOutTreeEM -> fillPDFs(wCTEQ66,wMRST2006NNLO,wNNPDF10100);
    }

    myOutTreeEM->fillRunInfos(runNumber, lumiBlock, eventNumber, weight, passedHLT[em], nputosave);

    theLJ  = theLeadingJet[em];
    theSJ  = theSecondJet[em];
    ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeEM->fillKFactor(kfW, genPtHiggs, ptLJ);

    myOutTreeEM -> fillAll(m_chMet[em], GetPt(m_p3PFMET->x(),m_p3PFMET->y()), GetPt(pxMet[0],pyMet[0]), 
			   m_projectedMet[em], m_deltaPhi[em], m_deltaErre[em], m_transvMass[em], m_mll[em], 
			   hardestLeptonPt[em], slowestLeptonPt[em], m_deltaEtaLeptons[em], nGoodPV,
			   selUpToFinalLeptonsEM, selUpToJetVetoEM, selUpToUncorrJetVetoEM, selPreDeltaPhiEM, isSelectedEM);


    if(eleCands[em].size()==1 && muCands[em].size()==1) setLepIdVariables((eleCands[em])[0],(muCands[em])[0],11,13);
    else setLepIdVariables(-1,-1,11,13);
    myOutTreeEM -> fillLeptons(myPt, myEta, myPhi, myLepId, myLepIso, myConv);
    
    myOutTreeEM -> fillMLVars(njets[em], nuncorrjets[em], m_maxDxyEvt, m_maxDszEvt, m_softbdisc[em], m_hardbdisc[em], m_maxCombinedSecondaryVertexMVABJetTags, 
                              nsoftmu[em], leadJetBtag[em], subleadJetBtag[em], subLeadJetsMaxBtag[em], 
                              leadJetBtagBProb[em], subleadJetBtagBProb[em], subLeadJetsMaxBtagBProb[em],
                              nextraleptons[em], nsoftmunojets[em], m_numbtagjets[em], nsoftjets[em]);

    if (wantJetIdStuff) 
      myOutTreeEM -> fillJetsVars(leadJetPt[em], leadJetEta[em], leadJetLoosePFId[em], leadJetMatchGen[em], leadJetMvaJetId[em], leadJetLooseId[em], subleadJetPt[em], subleadJetEta[em], subleadJetLoosePFId[em], subleadJetMatchGen[em], subleadJetMvaJetId[em], subleadJetLooseId[em]);
    
    myOutTreeEM -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, 
                                outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, 
                                outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26 ); 

    myOutTreeEM -> fillRazor(m_MTR[em], m_MR[em], m_GammaMR[em]);

    myOutTreeEM -> fillMetStudies( m_projectedPFMet[em], m_projectedTkMet[em], significancePFMet[0], significancePFChMet[0], m_MTRcharged[em], m_dymva[em], rhoFastjet, rhoJetsFastJet); 

    jesLJ = GetJetJesPcomponent(theLJ);
    jesSJ = GetJetJesPcomponent(theSJ);
    float pxLJEM[3] = { jesLJ[0].Px(), jesLJ[1].Px(), jesLJ[2].Px() };   float pyLJEM[3] = { jesLJ[0].Py(), jesLJ[1].Py(), jesLJ[2].Py() };   float pzLJEM[3] = { jesLJ[0].Pz(), jesLJ[1].Pz(), jesLJ[2].Pz() };
    float pxSJEM[3] = { jesSJ[0].Px(), jesSJ[1].Px(), jesSJ[2].Px() };   float pySJEM[3] = { jesSJ[0].Py(), jesSJ[1].Py(), jesSJ[2].Py() };   float pzSJEM[3] = { jesSJ[0].Pz(), jesSJ[1].Pz(), jesSJ[2].Pz() };

    if ( GetPt(m_p4LeptonPlus[em]->Px(),m_p4LeptonPlus[em]->Py()) > GetPt(m_p4LeptonMinus[em]->Px(),m_p4LeptonMinus[em]->Py()) ) {
      myOutTreeEM -> fillKinematics( m_p3TKMET[em].Px(), m_p3TKMET[em].Py(), m_p3TKMET[em].Pz(),
                                     pxLJEM, pyLJEM, pzLJEM, pxSJEM, pySJEM, pzSJEM,
				     m_p4LeptonPlus[em]->Px(), m_p4LeptonPlus[em]->Py(), m_p4LeptonPlus[em]->Pz(),
				     m_p4LeptonMinus[em]->Px(), m_p4LeptonMinus[em]->Py(), m_p4LeptonMinus[em]->Pz(),
                                     m_chEM,
                                     m_jetsSum[em], m_uncorrJetsSum[em], m_p3PFMET); 
      myOutTreeEM -> fillSystematics( mySCEnergy, myR9, m_p4LeptonPlusEnergy[em], m_p4LeptonMinusEnergy[em], m_p4PlusType[em], m_p4MinusType[em],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[em], jesMtDown[em]);
    } else { 
      myOutTreeEM -> fillKinematics( m_p3TKMET[em].Px(), m_p3TKMET[em].Py(), m_p3TKMET[em].Pz(),
                                     pxLJEM, pyLJEM, pzLJEM, pxSJEM, pySJEM, pzSJEM,
				     m_p4LeptonMinus[em]->Px(), m_p4LeptonMinus[em]->Py(), m_p4LeptonMinus[em]->Pz(),
				     m_p4LeptonPlus[em]->Px(), m_p4LeptonPlus[em]->Py(), m_p4LeptonPlus[em]->Pz(),
                                     m_chEM,
                                     m_jetsSum[em], m_uncorrJetsSum[em], m_p3PFMET);
      myOutTreeEM -> fillSystematics( mySCEnergy, myR9, m_p4LeptonMinusEnergy[em], m_p4LeptonPlusEnergy[em], m_p4MinusType[em], m_p4PlusType[em],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[em], jesMtDown[em]);
    }
    
    // dumping final tree, only if there are 2 final leptons
    if(selUpToFinalLeptonsEM) myOutTreeEM -> store();


  

    // ---------------------------------------
    // ME
    CutBasedHiggsSelectionME.SetWeight(weight);               
    CutBasedHiggsSelectionME.SetMcTruth(promptME);    
    CutBasedHiggsSelectionME.SetHLT(passedHLT[me]);               
    CutBasedHiggsSelectionME.SetIsChannel(m_channel[me]);     
    
    CutBasedHiggsSelectionME.SetElectronId(theBestIdMuonEle.first);                 
    CutBasedHiggsSelectionME.SetPositronId(theBestIdMuonEle.second);                 
    CutBasedHiggsSelectionME.SetElectronIsolation(theBestIsolMuonEle.first);        
    CutBasedHiggsSelectionME.SetPositronIsolation(theBestIsolMuonEle.second);        
    CutBasedHiggsSelectionME.SetElectronConvRejection(theBestConvMuonEle.first);    
    CutBasedHiggsSelectionME.SetPositronConvRejection(theBestConvMuonEle.second);    
    CutBasedHiggsSelectionME.SetElectronIp(theBestIpMuonEle.first);                 
    CutBasedHiggsSelectionME.SetPositronIp(theBestIpMuonEle.second);                 
    // checking if the highest pT electron at each step has pT>20. MU is the hardest in ME
    float thisMaxPtIdME = (theBestIdMuonEle.first > -1) ? GetPt(pxMuon[theBestIdMuonEle.first],pyMuon[theBestIdMuonEle.first]) : 0.;
    float thisMaxPtIsolME = (theBestIsolMuonEle.first > -1) ? GetPt(pxMuon[theBestIsolMuonEle.first],pyMuon[theBestIsolMuonEle.first]) : 0.;
    float thisMaxPtIpME = (theBestIpMuonEle.first > -1) ? GetPt(pxMuon[theBestIpMuonEle.first],pyMuon[theBestIpMuonEle.first]) : 0.;
    if (thisMaxPtIdME<20)   { 
      CutBasedHiggsSelectionME.SetElectronId(-1);
      CutBasedHiggsSelectionME.SetPositronId(-1);
    }
    if (thisMaxPtIsolME<20) { 
      CutBasedHiggsSelectionME.SetElectronIsolation(-1);
      CutBasedHiggsSelectionME.SetPositronIsolation(-1);
    }
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
    CutBasedHiggsSelectionME.SetProjectedMet(m_projectedMet[me]);
    CutBasedHiggsSelectionME.SetNvtx(nGoodPV);
    CutBasedHiggsSelectionME.SetMetOverPtLL(m_metOptll[me]);
    CutBasedHiggsSelectionME.SetDYMVA(m_dymva[me]);
    CutBasedHiggsSelectionME.SetDeltaPhiLLJet(dphiLLJ[me]);   
    CutBasedHiggsSelectionME.SetDeltaPhi(m_deltaPhi[me]);
    CutBasedHiggsSelectionME.SetInvMass(m_mll[me]);
    CutBasedHiggsSelectionME.SetDetaLeptons(m_deltaEtaLeptons[me]);
    CutBasedHiggsSelectionME.SetPtll(m_dilepPt[me].Pt());
    // CutBasedHiggsSelectionME.SetWWInvMass(2.*m_transvMass[me]/_massVal);
    CutBasedHiggsSelectionME.SetWWInvMass(m_transvMass[me]);

    bool isSelectedME           = CutBasedHiggsSelectionME.output();    
    bool selUpToFinalLeptonsME  = CutBasedHiggsSelectionME.outputUpToFinalLeptons();
    bool selUpToJetVetoME       = CutBasedHiggsSelectionME.outputUpToJetVeto();
    bool selUpToUncorrJetVetoME = CutBasedHiggsSelectionME.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiME       = CutBasedHiggsSelectionME.outputPreDeltaPhi();

    // latinos
    outputStep0  = CutBasedHiggsSelectionME.outputStep0();
    outputStep1  = CutBasedHiggsSelectionME.outputStep1();
    outputStep2  = CutBasedHiggsSelectionME.outputStep2();
    outputStep3  = CutBasedHiggsSelectionME.outputStep3();
    outputStep4  = CutBasedHiggsSelectionME.outputStep4();
    outputStep5  = CutBasedHiggsSelectionME.outputStep5();
    outputStep6  = CutBasedHiggsSelectionME.outputStep6();
    outputStep7  = CutBasedHiggsSelectionME.outputStep7();
    outputStep8  = CutBasedHiggsSelectionME.outputStep8();
    outputStep9  = CutBasedHiggsSelectionME.outputStep9();
    outputStep10 = CutBasedHiggsSelectionME.outputStep10();
    outputStep11 = CutBasedHiggsSelectionME.outputStep11();
    outputStep12 = CutBasedHiggsSelectionME.outputStep12();
    outputStep13 = CutBasedHiggsSelectionME.outputStep13();
    outputStep14 = CutBasedHiggsSelectionME.outputStep14();
    outputStep15 = CutBasedHiggsSelectionME.outputStep15();
    outputStep16 = CutBasedHiggsSelectionME.outputStep16();
    outputStep17 = CutBasedHiggsSelectionME.outputStep17();
    outputStep18 = CutBasedHiggsSelectionME.outputStep18();
    outputStep19 = CutBasedHiggsSelectionME.outputStep19();
    outputStep20 = CutBasedHiggsSelectionME.outputStep20();
    outputStep21 = CutBasedHiggsSelectionME.outputStep21();
    outputStep22 = CutBasedHiggsSelectionME.outputStep22();
    outputStep23 = CutBasedHiggsSelectionME.outputStep23();
    outputStep24 = CutBasedHiggsSelectionME.outputStep24();
    outputStep25 = CutBasedHiggsSelectionME.outputStep25();
    outputStep26 = CutBasedHiggsSelectionME.outputStep26();

    // filling the tree
    if(!_selectionME->getSwitch("isData")) {
      myOutTreeME -> fillMcTruth(promptME,_genmll,_genptll,_genyll);
      //      myOutTreeME -> fillPDFs(wCTEQ66,wMRST2006NNLO,wNNPDF10100);
    }

    myOutTreeME->fillRunInfos(runNumber, lumiBlock, eventNumber, weight, passedHLT[me], nputosave);

    theLJ  = theLeadingJet[me];
    theSJ  = theSecondJet[me];
    ptLJ = sqrt(pxAK5PFPUcorrJet[theLJ]*pxAK5PFPUcorrJet[theLJ] + pyAK5PFPUcorrJet[theLJ]*pyAK5PFPUcorrJet[theLJ]);
    myOutTreeME->fillKFactor(kfW, genPtHiggs, ptLJ);

    myOutTreeME -> fillAll(m_chMet[me], GetPt(m_p3PFMET->x(),m_p3PFMET->y()), GetPt(pxMet[0],pyMet[0]), 
			   m_projectedMet[me], m_deltaPhi[me], m_deltaErre[me], m_transvMass[me], m_mll[me], 
			   hardestLeptonPt[me], slowestLeptonPt[me], m_deltaEtaLeptons[me], nGoodPV,
			   selUpToFinalLeptonsME, selUpToJetVetoME, selUpToUncorrJetVetoME, selPreDeltaPhiME, isSelectedME);

    if(muCands[me].size()==1 && eleCands[me].size()==1) setLepIdVariables((muCands[me])[0],(eleCands[me])[0],13,11);
    else setLepIdVariables(-1,-1,13,11);
    myOutTreeME -> fillLeptons(myPt, myEta, myPhi, myLepId, myLepIso, myConv);
    
    myOutTreeME -> fillMLVars(njets[me], nuncorrjets[me], m_maxDxyEvt, m_maxDszEvt, m_softbdisc[me], m_hardbdisc[me], m_maxCombinedSecondaryVertexMVABJetTags, 
                              nsoftmu[me], leadJetBtag[me], subleadJetBtag[me], subLeadJetsMaxBtag[me], 
                              leadJetBtagBProb[me], subleadJetBtagBProb[me], subLeadJetsMaxBtagBProb[me],
                              nextraleptons[me], nsoftmunojets[me], m_numbtagjets[me], nsoftjets[me]);

    if (wantJetIdStuff) 
      myOutTreeME -> fillJetsVars(leadJetPt[me], leadJetEta[me], leadJetLoosePFId[me], leadJetMatchGen[me], leadJetMvaJetId[me], leadJetLooseId[me], subleadJetPt[me], subleadJetEta[me], subleadJetLoosePFId[me], subleadJetMatchGen[me], subleadJetMvaJetId[me], subleadJetLooseId[me]);
    
    myOutTreeME -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, 
                                outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, 
                                outputStep21, outputStep22, outputStep23, outputStep24, outputStep25, outputStep26 ); 

    myOutTreeME -> fillRazor(m_MTR[me], m_MR[me], m_GammaMR[me]);

    myOutTreeME -> fillMetStudies( m_projectedPFMet[me], m_projectedTkMet[me], significancePFMet[0], significancePFChMet[0], m_MTRcharged[me], m_dymva[me], rhoFastjet, rhoJetsFastJet);  

    jesLJ = GetJetJesPcomponent(theLJ);
    jesSJ = GetJetJesPcomponent(theSJ);
    float pxLJME[3] = { jesLJ[0].Px(), jesLJ[1].Px(), jesLJ[2].Px() };   float pyLJME[3] = { jesLJ[0].Py(), jesLJ[1].Py(), jesLJ[2].Py() };   float pzLJME[3] = { jesLJ[0].Pz(), jesLJ[1].Pz(), jesLJ[2].Pz() };
    float pxSJME[3] = { jesSJ[0].Px(), jesSJ[1].Px(), jesSJ[2].Px() };   float pySJME[3] = { jesSJ[0].Py(), jesSJ[1].Py(), jesSJ[2].Py() };   float pzSJME[3] = { jesSJ[0].Pz(), jesSJ[1].Pz(), jesSJ[2].Pz() };

    if ( GetPt(m_p4LeptonPlus[me]->Px(),m_p4LeptonPlus[me]->Py()) > GetPt(m_p4LeptonMinus[me]->Px(),m_p4LeptonMinus[me]->Py()) ) {
      myOutTreeME -> fillKinematics( m_p3TKMET[me].Px(), m_p3TKMET[me].Py(), m_p3TKMET[me].Pz(),
                                     pxLJME, pyLJME, pzLJME, pxSJME, pySJME, pzSJME,
				     m_p4LeptonPlus[me]->Px(), m_p4LeptonPlus[me]->Py(), m_p4LeptonPlus[me]->Pz(),
				     m_p4LeptonMinus[me]->Px(), m_p4LeptonMinus[me]->Py(), m_p4LeptonMinus[me]->Pz(),
                                     m_chME,
                                     m_jetsSum[me], m_uncorrJetsSum[me], m_p3PFMET); 
      myOutTreeME -> fillSystematics( mySCEnergy, myR9, m_p4LeptonPlusEnergy[me], m_p4LeptonMinusEnergy[me], m_p4PlusType[me], m_p4MinusType[me],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[me], jesMtDown[me]);
    } else { 
      myOutTreeME -> fillKinematics( m_p3TKMET[me].Px(), m_p3TKMET[me].Py(), m_p3TKMET[me].Pz(),
                                     pxLJME, pyLJME, pzLJME, pxSJME, pySJME, pzSJME,
				     m_p4LeptonMinus[me]->Px(), m_p4LeptonMinus[me]->Py(), m_p4LeptonMinus[me]->Pz(),
				     m_p4LeptonPlus[me]->Px(), m_p4LeptonPlus[me]->Py(), m_p4LeptonPlus[me]->Pz(),
                                     m_chME,
                                     m_jetsSum[me], m_uncorrJetsSum[me], m_p3PFMET);
      myOutTreeME -> fillSystematics( mySCEnergy, myR9, m_p4LeptonMinusEnergy[me], m_p4LeptonPlusEnergy[me], m_p4MinusType[me], m_p4PlusType[me],
                                      m_metFromJets, m_pfMetJESUp, m_pfMetJESDown, jesMtUp[me], jesMtDown[me]);
    }


    // dumping final tree, only if there are 2 final leptons
    if(selUpToFinalLeptonsME) myOutTreeME -> store();

  }

  fMatch = new TFile("matching.root","RECREATE");
  fMatch->cd();
  H_deltaRuncorr->Write();
  H_deltaRcorr->Write();
  fMatch->Close();
}

void HiggsMLSelection::displayEfficiencies(std::string datasetName) {

  std::string::size_type loc = datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    datasetName.erase(loc);
  }
  
  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full EE selections: " << std::endl;
  CutBasedHiggsSelectionEE.displayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full MM selections: " << std::endl;
  CutBasedHiggsSelectionMM.displayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full EM selections: " << std::endl;
  CutBasedHiggsSelectionEM.displayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full ME selections: " << std::endl;
  CutBasedHiggsSelectionME.displayEfficiencies(datasetName);


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

std::pair<int,int> HiggsMLSelection::getBestElectronPair_acceptance() {
  
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
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = i;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = i;
    }
    */

    _acceptEleAll.push_back(i);   
  }
  _acceptEleAll = sortElectronsByPt(_acceptEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestElectronPair_id( std::vector<int> acceptEle ) {

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
    if (!_selectionEE->getSwitch("asymmetricID")) isEleID2012AndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej);
    if ( _selectionEE->getSwitch("asymmetricID")) {
      if (thisPt>=20) isEleID2012AndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej);
      if (thisPt<20)  isEleID2012AndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej);
    }

    if (!theElectronID) continue;
    
    // further requests if we apply the smurf ID and pT<15
//     TString stringIdLow (_selectionEE->getStringParameter("electronIDTypeLow"));
//     if( stringIdLow.Contains("Smurf") && !m_useBDTEleID ) {
//       if ( thisPt<20  ) {
// 	if ( fbremEle[thisEle]<0.15 && !(fabs(etaEle[thisEle])<1.0 && eSuperClusterOverPEle[thisEle]>0.95) ) continue;
//       }
//     }

    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same charge test  // chiara
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisEle;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisEle;
    }
    */

    _idEleAll.push_back(thisEle);  
  }
  _idEleAll = sortElectronsByPt(_idEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestElectronPair_isol( std::vector<int> idEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _isolEleAll.clear();

  for (int iEle=0; iEle<idEle.size(); iEle++) {
    int thisEle = idEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleID2012AndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej);

    if (!theElectronIsol) continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisEle;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisEle;
    }                                                                                             
    */

    _isolEleAll.push_back(thisEle);  
  }
  _isolEleAll = sortElectronsByPt(_isolEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestElectronPair_conv( std::vector<int> isolEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _convEleAll.clear();

  for (int iEle=0; iEle<isolEle.size(); iEle++) {
    int thisEle = isolEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleID2012AndDenom(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej);
    
    if (!theElectronConvRej) continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisEle;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisEle;
      }                                                                                                                                                            */
                                  
    _convEleAll.push_back(thisEle);      
  }
  _convEleAll = sortElectronsByPt(_convEleAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> HiggsMLSelection::getBestElectronPair_ip( std::vector<int> convEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _ipEleAll.clear();

  for (int iEle=0; iEle<convEle.size(); iEle++) {
    int thisEle = convEle[iEle];

    int gsfTrack = gsfTrackIndexEle[thisEle]; 
    // float d3dEle = impactPar3DGsfTrack[gsfTrack];
    // if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",d3dEle)) ) continue;   
    float dxyEle = transvImpactParGsfTrack[gsfTrack];
    float dzEle  = eleDzPV(thisEle,0);
    if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",dxyEle)) ) continue;
    if (_selectionEE->getSwitch("electronDz") && (!_selectionEE->passCut("electronDz",dzEle)) ) continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisEle;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisEle;
      } 
    */                                                                                                                                                                                             
    _ipEleAll.push_back(thisEle);  
  }
  _ipEleAll = sortElectronsByPt(_ipEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestMuonPair_acceptance() {
  
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

    if(_selectionEE->getSwitch("etaMuonAcc") && !_selectionEE->passCut("etaMuonAcc",etaMuon[i]) ) continue;

    float thisPt = GetPt(pxMuon[i],pyMuon[i]);
    if(_selectionEE->getSwitch("ptMuonAcc") && !_selectionEE->passCut("ptMuonAcc",thisPt) ) continue;

    float thisCharge = chargeMuon[i];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = i;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = i;
      }
    */

    _acceptMuonsAll.push_back(i);  
  }
  _acceptMuonsAll = sortMuonsByPt(_acceptMuonsAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> HiggsMLSelection::getBestMuonPair_id( std::vector<int> acceptMu ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _idMuonsAll.clear();

  for(int iMu=0; iMu<acceptMu.size(); iMu++) {

    int thisMu = acceptMu[iMu];

    bool theMuonID = true;
    isMuonID2012(thisMu, &theMuonID);
    if (!theMuonID) continue;

    float thisPt     = GetPt(pxMuon[thisMu],pyMuon[thisMu]);
    float thisCharge = chargeMuon[thisMu];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisMu;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisMu;
    }                                                                                                            
    */
                                                                                       
    _idMuonsAll.push_back(thisMu);   
  }
  _idMuonsAll = sortMuonsByPt(_idMuonsAll);
  
  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestMuonPair_isol( std::vector<int> idMu ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
   
  _isolMuonsAll.clear();

  for(int iMu=0; iMu<idMu.size(); iMu++) {

    int thisMu   = idMu[iMu];
    float thisPt = GetPt(pxMuon[thisMu],pyMuon[thisMu]);

    // fixme: diverso da prima: rimuovevo il secondo leptone....
//     float muonTrackerForGlobal = sumPt03Muon[thisMu];
//     float muonEcalForGlobal    = emEt03Muon[thisMu];
//     float muonHcalForGlobal    = hadEt03Muon[thisMu]; 
//     float theMuonGlobalSum     = muonTrackerForGlobal + muonEcalForGlobal + muonHcalForGlobal - rhoFastjet*TMath::Pi()*0.3*0.3;
//     float theRelMuonIso        = theMuonGlobalSum/thisPt; 
//    float theRelMuonIso = pfCombinedIsoMuon[thisMu]/thisPt;
    //    if(_selectionEE->getSwitch("muGlobalIso") && !_selectionEE->passCut("muGlobalIso",theRelMuonIso)) continue;  
    if( ! isPFIsolatedMuon2012(thisMu) ) continue;

    float thisCharge = chargeMuon[thisMu];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisMu;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisMu;
      }*/                                                                                                                                                                                                      
    _isolMuonsAll.push_back(thisMu);   
  }
  _isolMuonsAll = sortMuonsByPt(_isolMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestMuonPair_ip( std::vector<int> isoMu ) {

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
    
    if (thisPt>20) {   // hardcoded
      if (_selectionEE->getSwitch("muonIPhighPT") && (!_selectionEE->passCut("muonIPhighPT",dxyMuon)) ) continue;   
    } 
    if (thisPt<20) {   // hardcoded
      if (_selectionEE->getSwitch("muonIPlowPT")  && (!_selectionEE->passCut("muonIPlowPT",dxyMuon)) ) continue;   
    }
    if (_selectionEE->getSwitch("muonDz") && (!_selectionEE->passCut("muonDz",dzMuon)) )  continue;   

    float thisCharge = chargeMuon[thisMu];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    /*
    // only for same charge test  // chiara  
    if (thisCharge < 0 && thisPt > maxPtLep1 && thisPt > maxPtLep2){
      maxPtLep2 = maxPtLep1;
      maxPtLep1 = thisPt;
      theLep2   = theLep1;
      theLep1   = thisMu;
    } else if ( thisCharge < 0 && thisPt > maxPtLep2 && thisPt < maxPtLep1 ){
      maxPtLep2 = thisPt;
      theLep2   = thisMu;
      } */                                                                                                                                                                                                     
    _ipMuonsAll.push_back(thisMu);   
  }
  _ipMuonsAll = sortMuonsByPt(_ipMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsMLSelection::getBestEleMuonPair(std::vector<int> electrons, std::vector<int> muons) {

  int theEle=-1;
  int theMuon=-1;

  std::vector<int>::const_iterator eleiter;
  for(eleiter=electrons.begin(); eleiter<electrons.end();++eleiter) {
    int eleCharge = chargeEle[*eleiter];
    float elePt = GetPt(pxEle[*eleiter],pyEle[*eleiter]);
    theEle = *eleiter;
    std::vector<int>::const_iterator muiter;
    for(muiter=muons.begin(); muiter<muons.end();++muiter) {
      int muCharge = chargeMuon[*muiter];
      float muPt = GetPt(pxMuon[*muiter],pyMuon[*muiter]);
      // chiara
      if(eleCharge*muCharge<0 && elePt > muPt) return std::make_pair(*eleiter,*muiter); 
      // if (muCharge>0) continue;
      // if (eleCharge*muCharge>0 && elePt > muPt) return std::make_pair(*eleiter,*muiter); 
    }
  }
  
  return std::make_pair(theEle,theMuon);
}

std::pair<int,int> HiggsMLSelection::getBestMuonElePair(std::vector<int> electrons, std::vector<int> muons) {

  int theEle=-1;
  int theMuon=-1;

  std::vector<int>::const_iterator muiter;
  for(muiter=muons.begin(); muiter<muons.end();++muiter) {
    int muCharge = chargeMuon[*muiter];
    float muPt = GetPt(pxMuon[*muiter],pyMuon[*muiter]);
    theMuon = *muiter;
    std::vector<int>::const_iterator eleiter;
    for(eleiter=electrons.begin(); eleiter<electrons.end();++eleiter) {
      int eleCharge = chargeEle[*eleiter];
      float elePt = GetPt(pxEle[*eleiter],pyEle[*eleiter]);
      // chiara
      if(eleCharge*muCharge<0 && muPt > elePt) return std::make_pair(*muiter,*eleiter); 
      // if (eleCharge>0) continue;
      // if (eleCharge*muCharge>0 && elePt > muPt) return std::make_pair(*eleiter,*muiter); 
    }
  }
  return std::make_pair(theMuon,theEle); 
}

void HiggsMLSelection::setKinematicsEE(int myEle, int myPosi) {

  if (myPosi > -1 && myEle > -1) {

    if(GetPt(pxEle[myEle],pyEle[myEle])>GetPt(pxEle[myPosi],pyEle[myPosi])) {
      eleCands[ee].push_back(myEle);
      eleCands[ee].push_back(myPosi); 
    } else {
      eleCands[ee].push_back(myPosi);
      eleCands[ee].push_back(myEle);
    }
    hardestLeptonPt[ee] = TMath::Max(GetPt(pxEle[myEle],pyEle[myEle]),GetPt(pxEle[myPosi],pyEle[myPosi]));
    slowestLeptonPt[ee] = TMath::Min(GetPt(pxEle[myEle],pyEle[myEle]),GetPt(pxEle[myPosi],pyEle[myPosi]));
    m_p4LeptonMinus[ee] -> SetXYZT(pxEle[myEle], pyEle[myEle], pzEle[myEle], energyEle[myEle]);
    m_p4LeptonPlus[ee]  -> SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
    m_p4LeptonMinusEnergy[ee] = energyEle[myEle];
    m_p4LeptonPlusEnergy[ee]  = energyEle[myPosi];
    m_p4MinusType[ee] = 0;
    m_p4PlusType[ee]  = 0;
    m_mll[ee]       = (*(m_p4LeptonMinus[ee]) + *(m_p4LeptonPlus[ee])).M();
    m_deltaPhi[ee]  = fabs(180./TMath::Pi() * m_p4LeptonMinus[ee]->Vect().DeltaPhi(m_p4LeptonPlus[ee]->Vect()));
    m_deltaErre[ee] = m_p4LeptonMinus[ee]->Vect().DeltaR(m_p4LeptonPlus[ee]->Vect());
    m_deltaEtaLeptons[ee] = etaEle[myEle]-etaEle[myPosi];
    m_dilepPt[ee].SetXYZ( m_p4LeptonMinus[ee]->Vect().X()+m_p4LeptonPlus[ee]->Vect().X(),m_p4LeptonMinus[ee]->Vect().Y()+m_p4LeptonPlus[ee]->Vect().Y(),0.0 );
    // def. 3 of http://indico.cern.ch/getFile.py/access?contribId=4&resId=0&materialId=slides&confId=104213                           
    // m_transvMass[ee]=mT3(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee],m_p3MET);
    // usual definition
    m_transvMass[ee] = sqrt( 2.*(m_dilepPt[ee].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[ee]))) );
    // chris' variable
    m_GammaMR[ee]=CalcGammaMRstar(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee]);
    m_MR[ee]=CalcMRNEW(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee],*m_p3PFMET);
    m_MTR[ee]=CalcMTR(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee],*m_p3PFMET);
    m_metOptll[ee] = m_theMET / m_dilepPt[ee].Pt();
    m_mT2[ee] = 0.;
    m_p3TKMET[ee] = pfChargedMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_chMet[ee] = (m_p3TKMET[ee]).Pt();
    m_projectedMet[ee] = GetProjectedMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_projectedPFMet[ee] = GetProjectedPFMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_projectedTkMet[ee] = GetProjectedTkMet(m_p4LeptonMinus[ee]->Vect(),m_p4LeptonPlus[ee]->Vect());
    m_MTRcharged[ee]     = CalcMTR(*m_p4LeptonMinus[ee],*m_p4LeptonPlus[ee],m_p3TKMET[ee]);

    int lead(-1), sublead(-1);
    if(m_p4LeptonMinus[ee]->Pt() >= m_p4LeptonPlus[ee]->Pt()) {
      lead = myEle; sublead = myPosi;
    } else {
      lead = myPosi; sublead = myEle;
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

void HiggsMLSelection::setKinematicsMM(int myMuMinus, int myMuPlus) {
  
  if (myMuPlus > -1 && myMuMinus > -1){

    if(GetPt(pxMuon[myMuMinus],pyMuon[myMuMinus])>GetPt(pxMuon[myMuPlus],pyMuon[myMuPlus])) {
      muCands[mm].push_back(myMuMinus);
      muCands[mm].push_back(myMuPlus);
    } else {
      muCands[mm].push_back(myMuPlus);
      muCands[mm].push_back(myMuMinus);
    }
    hardestLeptonPt[mm] = TMath::Max(GetPt(pxMuon[myMuPlus],pyMuon[myMuPlus]),GetPt(pxMuon[myMuMinus],pyMuon[myMuMinus]));
    slowestLeptonPt[mm] = TMath::Min(GetPt(pxMuon[myMuPlus],pyMuon[myMuPlus]),GetPt(pxMuon[myMuMinus],pyMuon[myMuMinus]));
    m_p4LeptonMinus[mm] -> SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
    m_p4LeptonPlus[mm]  -> SetXYZT(pxMuon[myMuPlus], pyMuon[myMuPlus], pzMuon[myMuPlus], energyMuon[myMuPlus]);
    m_p4LeptonMinusEnergy[mm] = energyMuon[myMuMinus];
    m_p4LeptonPlusEnergy[mm]  = energyMuon[myMuPlus];
    m_p4MinusType[mm] = 1;
    m_p4PlusType[mm]  = 1;
    m_mll[mm]             = (*(m_p4LeptonMinus[mm]) + *(m_p4LeptonPlus[mm])).M();
    m_deltaPhi[mm]        = fabs(180./TMath::Pi() * m_p4LeptonMinus[mm]->Vect().DeltaPhi(m_p4LeptonPlus[mm]->Vect()));
    m_deltaErre[mm]       = m_p4LeptonMinus[mm]->Vect().DeltaR(m_p4LeptonPlus[mm]->Vect());
    m_deltaEtaLeptons[mm] = etaEle[myMuMinus]-etaEle[myMuPlus];
    m_dilepPt[mm].SetXYZ( m_p4LeptonMinus[mm]->Vect().X()+m_p4LeptonPlus[mm]->Vect().X(),m_p4LeptonMinus[mm]->Vect().Y()+m_p4LeptonPlus[mm]->Vect().Y(),0.0 );
    // chris' variable
    m_GammaMR[mm] = CalcGammaMRstar(*m_p4LeptonMinus[mm],*m_p4LeptonPlus[mm]);
    m_MR[mm] = CalcMRNEW(*m_p4LeptonMinus[mm],*m_p4LeptonPlus[mm],*m_p3PFMET);
    m_MTR[mm] = CalcMTR(*m_p4LeptonMinus[mm],*m_p4LeptonPlus[mm],*m_p3PFMET);
    // usual definition
    m_transvMass[mm] = sqrt( 2.*(m_dilepPt[mm].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[mm]))) );
    m_metOptll[mm]        = m_theMET / m_dilepPt[mm].Pt();
    m_mT2[mm]             = 0.;
    m_projectedMet[mm]    = GetProjectedMet(m_p4LeptonMinus[mm]->Vect(),m_p4LeptonPlus[mm]->Vect());
    m_projectedPFMet[mm] = GetProjectedPFMet(m_p4LeptonMinus[mm]->Vect(),m_p4LeptonPlus[mm]->Vect());
    m_projectedTkMet[mm] = GetProjectedTkMet(m_p4LeptonMinus[mm]->Vect(),m_p4LeptonPlus[mm]->Vect());
    m_MTRcharged[mm]     = CalcMTR(*m_p4LeptonMinus[mm],*m_p4LeptonPlus[mm],m_p3TKMET[mm]);
    m_p3TKMET[mm] = pfChargedMet(m_p4LeptonMinus[mm]->Vect(),m_p4LeptonPlus[mm]->Vect());
    m_chMet[mm] = (m_p3TKMET[mm]).Pt();

    int lead(-1), sublead(-1);
    if(m_p4LeptonMinus[mm]->Pt() >= m_p4LeptonPlus[mm]->Pt()) {
      lead = myMuMinus; sublead = myMuPlus;
    } else {
      lead = myMuPlus; sublead = myMuMinus;
    }
    m_chMM[0] = chargeMuon[lead];
    m_chMM[1] = chargeMuon[sublead];
    m_isoMM[0] = pfCombinedIsoMuon[lead] / hardestLeptonPt[mm];
    m_isoMM[1] = pfCombinedIsoMuon[sublead] / slowestLeptonPt[mm];
    m_lhMM[0] = -999.;
    m_lhMM[1] = -999.;
    m_bdtMM[0] = -999.;
    m_bdtMM[1] = -999.;
    m_chmajMM[0] = -999.;
    m_chmajMM[1] = -999.;
  }

}

void HiggsMLSelection::setKinematicsEMME(int myEle, int myPosi, int myMuPlus, int myMuMinus) {

  if ( myEle > -1 && myMuPlus > -1 && ( myPosi<0 || myMuMinus<0 ) ) {  // only 1 pair reconstructed                                    

    float ptE = GetPt(pxEle[myEle],pyEle[myEle]);
    float ptM = GetPt(pxMuon[myMuPlus],pyMuon[myMuPlus]);
    
    if (ptE > ptM) {
      eleCands[em].push_back(myEle);
      muCands[em].push_back(myMuPlus);
      m_p4LeptonMinus[em] -> SetXYZT(pxEle[myEle],pyEle[myEle],pzEle[myEle],energyEle[myEle]);
      m_p4LeptonPlus[em]  -> SetXYZT(pxMuon[myMuPlus],pyMuon[myMuPlus],pzMuon[myMuPlus],energyMuon[myMuPlus]);
      m_p4LeptonMinusEnergy[em] = energyEle[myEle];
      m_p4LeptonPlusEnergy[em]  = energyMuon[myMuPlus];
      m_p4MinusType[em] = 0;
      m_p4PlusType[em]  = 1;
      hardestLeptonPt[em] = ptE;
      slowestLeptonPt[em] = ptM;
      m_mll[em]           = (*(m_p4LeptonMinus[em]) + *(m_p4LeptonPlus[em])).M();
      m_deltaPhi[em]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[em]->Vect().DeltaPhi(m_p4LeptonPlus[em]->Vect()));
      m_deltaErre[em]     = m_p4LeptonMinus[em]->Vect().DeltaR(m_p4LeptonPlus[em]->Vect());
      m_deltaEtaLeptons[em] = etaEle[myEle]-etaEle[myMuPlus];
      m_dilepPt[em].SetXYZ( m_p4LeptonMinus[em]->Vect().X()+m_p4LeptonPlus[em]->Vect().X(),m_p4LeptonMinus[em]->Vect().Y()+m_p4LeptonPlus[em]->Vect().Y(),0.0 );
      // chris' variable
      m_GammaMR[em] = CalcGammaMRstar(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em]);
      m_MR[em] = CalcMRNEW(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
      m_MTR[em] = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
      // usual definition
      m_transvMass[em] = sqrt( 2.*(m_dilepPt[em].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[em]))) );      
      m_metOptll[em]      = m_theMET / m_dilepPt[em].Pt();
      m_mT2[em]           = 0.;
      m_projectedMet[em]  = GetProjectedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_projectedPFMet[em] = GetProjectedPFMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_projectedTkMet[em] = GetProjectedTkMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_MTRcharged[em]     = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],m_p3TKMET[em]);
      m_p3TKMET[em] = pfChargedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_chMet[em] = (m_p3TKMET[em]).Pt();

      int lead = myEle;
      int sublead = myMuPlus;
      m_chEM[0] = chargeEle[lead];
      m_chEM[1] = chargeMuon[sublead];
      m_isoEM[0] = pfCombinedIsoEle[lead] / hardestLeptonPt[em];
      m_isoEM[1] = pfCombinedIsoMuon[sublead] / slowestLeptonPt[em];
      m_lhEM[0] = eleIdLikelihoodEle[lead];
      m_lhEM[1] = -999.;
      m_bdtEM[0] = eleBDT(fMVA,lead);
      m_bdtEM[1] = -999.;
      m_chmajEM[0] = eleChargeMajority(lead);
      m_chmajEM[1] = -999.;

    } else {
      eleCands[me].push_back(myEle);
      muCands[me].push_back(myMuPlus);
      m_p4LeptonMinus[me] -> SetXYZT(pxEle[myEle],pyEle[myEle],pzEle[myEle],energyEle[myEle]);
      m_p4LeptonPlus[me]  -> SetXYZT(pxMuon[myMuPlus],pyMuon[myMuPlus],pzMuon[myMuPlus],energyMuon[myMuPlus]);
      m_p4LeptonMinusEnergy[me] = energyEle[myEle];
      m_p4LeptonPlusEnergy[me]  = energyMuon[myMuPlus];
      m_p4MinusType[me] = 0;
      m_p4PlusType[me]  = 1;
      hardestLeptonPt[me] = ptM;
      slowestLeptonPt[me] = ptE;
      m_mll[me]           = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
      m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
      m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
      m_deltaEtaLeptons[me] = etaEle[myEle]-etaEle[myMuPlus];
      m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
      // chris' variable
      m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MR[me] = CalcMRNEW(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      m_MTR[me] = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      // usual variable
      m_transvMass[me] = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );      
      m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
      m_mT2[me]           = 0.;
      m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_projectedPFMet[me] = GetProjectedPFMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_projectedTkMet[me] = GetProjectedTkMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_MTRcharged[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],m_p3TKMET[me]);
      m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_chMet[me] = (m_p3TKMET[me]).Pt();

      int lead = myMuPlus;
      int sublead = myEle;
      m_chME[0] = chargeMuon[lead];
      m_chME[1] = chargeEle[sublead];
      m_isoME[0] = pfCombinedIsoMuon[lead] / hardestLeptonPt[me];
      m_isoME[1] = pfCombinedIsoEle[sublead] / slowestLeptonPt[me];
      m_lhME[0] = -999.;
      m_lhME[1] = eleIdLikelihoodEle[sublead];
      m_bdtEM[0] = -999.;
      m_bdtEM[1] = eleBDT(fMVA,sublead);
      m_chmajME[0] = -999.;
      m_chmajME[1] = eleChargeMajority(sublead);

    }
  }
  
  if ( myPosi > -1 && myMuMinus > -1 && (myEle<0 || myMuPlus<0 )) {   // only 1 pair reconstructed                                     

    float ptE = GetPt(pxEle[myPosi],pyEle[myPosi]);
    float ptM = GetPt(pxMuon[myMuMinus],pyMuon[myMuMinus]);

    if (ptE > ptM) {
      eleCands[em].push_back(myPosi);
      muCands[em].push_back(myMuMinus);
      m_p4LeptonMinus[em] -> SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
      m_p4LeptonPlus[em]  -> SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
      m_p4LeptonMinusEnergy[em] = energyMuon[myMuMinus];
      m_p4LeptonPlusEnergy[em]  = energyEle[myPosi];
      m_p4MinusType[em] = 1;
      m_p4PlusType[em]  = 0;
      hardestLeptonPt[em] = ptE;
      slowestLeptonPt[em] = ptM;
      m_mll[em]           = (*(m_p4LeptonMinus[em]) + *(m_p4LeptonPlus[em])).M();
      m_deltaPhi[em]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[em]->Vect().DeltaPhi(m_p4LeptonPlus[em]->Vect()));
      m_deltaErre[em]     = m_p4LeptonMinus[em]->Vect().DeltaR(m_p4LeptonPlus[em]->Vect());
      m_deltaEtaLeptons[em] = etaEle[myMuMinus]-etaEle[myPosi];
      m_dilepPt[em].SetXYZ( m_p4LeptonMinus[em]->Vect().X()+m_p4LeptonPlus[em]->Vect().X(),m_p4LeptonMinus[em]->Vect().Y()+m_p4LeptonPlus[em]->Vect().Y(),0.0 );
      // usual variable
      m_transvMass[em] = sqrt( 2.*(m_dilepPt[em].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[em]))) );      
      // chris' variable
      m_GammaMR[em] = CalcGammaMRstar(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em]);
      m_MR[em] = CalcMRNEW(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
      m_MTR[em] = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
      m_metOptll[em]      = m_theMET / m_dilepPt[em].Pt();
      m_mT2[em]           = 0.;
      m_projectedMet[em]  = GetProjectedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_projectedPFMet[em] = GetProjectedPFMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_projectedTkMet[em] = GetProjectedTkMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_MTRcharged[em]     = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],m_p3TKMET[em]);
      m_p3TKMET[em] = pfChargedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
      m_chMet[em] = (m_p3TKMET[em]).Pt();

      int lead = myPosi;
      int sublead = myMuMinus;
      m_chEM[0] = chargeEle[lead];
      m_chEM[1] = chargeMuon[sublead];
      m_isoEM[0] = pfCombinedIsoEle[lead] / hardestLeptonPt[em];
      m_isoEM[1] = pfCombinedIsoMuon[sublead] / slowestLeptonPt[em];
      m_lhEM[0] = eleIdLikelihoodEle[lead];
      m_lhEM[1] = -999.;
      m_bdtEM[0] = eleBDT(fMVA,lead);
      m_bdtEM[1] = -999.;
      m_chmajEM[0] = eleChargeMajority(lead);
      m_chmajEM[1] = -999.;

    } else {
      eleCands[me].push_back(myPosi);
      muCands[me].push_back(myMuMinus);
      m_p4LeptonMinus[me] -> SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
      m_p4LeptonPlus[me]  -> SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
      m_p4LeptonMinusEnergy[me] = energyMuon[myMuMinus];
      m_p4LeptonPlusEnergy[me]  = energyEle[myPosi];
      m_p4MinusType[me] = 1;
      m_p4PlusType[me]  = 0;
      hardestLeptonPt[me] = ptM;
      slowestLeptonPt[me] = ptE;
      m_mll[me]           = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
      m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
      m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
      m_deltaEtaLeptons[me] = etaEle[myMuMinus]-etaEle[myPosi];
      m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
      // usual variabl
      m_transvMass[me]    = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );      
      // chris' variable
      m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
      m_MR[me] = CalcMRNEW(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      m_MTR[me] = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
      m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
      m_mT2[me]           = 0.;
      m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_projectedPFMet[me] = GetProjectedPFMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_projectedTkMet[me] = GetProjectedTkMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_MTRcharged[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],m_p3TKMET[me]);
      m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
      m_chMet[me] = (m_p3TKMET[me]).Pt();

      int lead = myMuMinus;
      int sublead = myPosi;
      m_chME[0] = chargeMuon[lead];
      m_chME[1] = chargeEle[sublead];
      m_isoME[0] = pfCombinedIsoMuon[lead] / hardestLeptonPt[me];
      m_isoME[1] = pfCombinedIsoEle[sublead] / slowestLeptonPt[me];
      m_lhME[0] = -999.;
      m_lhME[1] = eleIdLikelihoodEle[sublead];
      m_bdtME[0] = -999.;
      m_bdtME[1] = eleBDT(fMVA,sublead);
      m_chmajME[0] = -999.;
      m_chmajME[1] = eleChargeMajority(sublead);
    }
  }
  
  // if two pairs are built we choose the one with highest di-lepton pt                                                                
  if ( myPosi>-1 && myEle>-1 &&  myMuPlus>-1 && myMuMinus>-1) {   // 2 pairs reconstructed                                             
    
    TLorentzVector m_posi, m_ele, m_mum, m_mup;
    m_posi.SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
    m_ele.SetXYZT(pxEle[myEle], pyEle[myEle], pzEle[myEle], energyEle[myEle]);
    m_mum.SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
    m_mup.SetXYZT(pxMuon[myMuPlus], pyMuon[myMuPlus], pzMuon[myMuPlus], energyMuon[myMuPlus]);
    TVector3 m_ep_mm( m_posi.Vect().X()+m_mum.Vect().X(), m_posi.Vect().Y()+m_mum.Vect().Y(), 0.0 );
    TVector3 m_em_mp( m_ele.Vect().X()+m_mup.Vect().X(),  m_ele.Vect().Y()+m_mup.Vect().Y(),  0.0 );
    float mod_ep_mm = m_ep_mm.Mag();
    float mod_em_mp = m_em_mp.Mag();
    
    if (mod_ep_mm>mod_em_mp) {

      float ptE = GetPt(pxEle[myPosi],pyEle[myPosi]);
      float ptM = GetPt(pxMuon[myMuMinus],pyMuon[myMuMinus]);
      
      if (ptE > ptM) {
        eleCands[em].push_back(myPosi);
        muCands[em].push_back(myMuMinus);
        m_p4LeptonMinus[em] -> SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
        m_p4LeptonPlus[em]  -> SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
	m_p4LeptonMinusEnergy[em] = energyMuon[myMuMinus];
	m_p4LeptonPlusEnergy[em]  = energyEle[myPosi];
	m_p4MinusType[em] = 1;
	m_p4PlusType[em]  = 0;
        hardestLeptonPt[em] = ptE;
        slowestLeptonPt[em] = ptM;
        m_mll[em]           = (*(m_p4LeptonMinus[em]) + *(m_p4LeptonPlus[em])).M();
        m_deltaPhi[em]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[em]->Vect().DeltaPhi(m_p4LeptonPlus[em]->Vect()));
        m_deltaErre[em]     = m_p4LeptonMinus[em]->Vect().DeltaR(m_p4LeptonPlus[em]->Vect());
	m_deltaEtaLeptons[em] = etaEle[myMuMinus]-etaEle[myPosi];
        m_dilepPt[em].SetXYZ( m_p4LeptonMinus[em]->Vect().X()+m_p4LeptonPlus[em]->Vect().X(),m_p4LeptonMinus[em]->Vect().Y()+m_p4LeptonPlus[em]->Vect().Y(),0.0 );
	// usual variable
	m_transvMass[em]    = sqrt( 2.*(m_dilepPt[em].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[em]))) );      
	// chris' variable
        m_GammaMR[em] = CalcGammaMRstar(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em]);
        m_MR[em] = CalcMRNEW(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
        m_MTR[em] = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
        m_metOptll[em]      = m_theMET / m_dilepPt[em].Pt();
        m_mT2[em]           = 0.;
        m_projectedMet[em]  = GetProjectedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_projectedPFMet[em] = GetProjectedPFMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_projectedTkMet[em] = GetProjectedTkMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_MTRcharged[em]     = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],m_p3TKMET[em]);
        m_p3TKMET[em] = pfChargedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
        m_chMet[em] = (m_p3TKMET[em]).Pt();

        int lead = myPosi;
        int sublead = myMuMinus;
        m_chEM[0] = chargeEle[lead];
        m_chEM[1] = chargeMuon[sublead];
        m_isoEM[0] = pfCombinedIsoEle[lead] / hardestLeptonPt[em];
        m_isoEM[1] = pfCombinedIsoMuon[sublead] / slowestLeptonPt[em];
        m_lhEM[0] = eleIdLikelihoodEle[lead];
        m_lhEM[1] = -999.;
        m_bdtEM[0] = eleBDT(fMVA,lead);
        m_bdtEM[1] = -999.;
        m_chmajEM[0] = eleChargeMajority(lead);
        m_chmajEM[1] = -999.;

      } else {
        eleCands[me].push_back(myPosi);
        muCands[me].push_back(myMuMinus);
	m_p4LeptonMinus[me] -> SetXYZT(pxMuon[myMuMinus],pyMuon[myMuMinus],pzMuon[myMuMinus],energyMuon[myMuMinus]);
        m_p4LeptonPlus[me]  -> SetXYZT(pxEle[myPosi],pyEle[myPosi],pzEle[myPosi],energyEle[myPosi]);
	m_p4LeptonMinusEnergy[me] = energyMuon[myMuMinus];
	m_p4LeptonPlusEnergy[me]  = energyEle[myPosi];
	m_p4MinusType[me] = 1;
	m_p4PlusType[me]  = 0;
        hardestLeptonPt[me] = ptM;
        slowestLeptonPt[me] = ptE;
        m_mll[me]           = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
        m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
        m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
	m_deltaEtaLeptons[me] = etaEle[myMuMinus]-etaEle[myPosi];
        m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
	// usuale variable
	m_transvMass[me]    = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );      
	// chris' variable
        m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
        m_MR[me] = CalcMRNEW(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
        m_MTR[me] = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
        m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
        m_mT2[me]           = 0.;
        m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_projectedPFMet[me] = GetProjectedPFMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_projectedTkMet[me] = GetProjectedTkMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_MTRcharged[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],m_p3TKMET[me]);
        m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
        m_chMet[me] = (m_p3TKMET[me]).Pt();

        int lead = myMuMinus;
        int sublead = myPosi;
        m_chME[0] = chargeMuon[lead];
        m_chME[1] = chargeEle[sublead];
        m_isoME[0] = pfCombinedIsoMuon[lead] / hardestLeptonPt[me];
        m_isoME[1] = pfCombinedIsoEle[sublead] / slowestLeptonPt[me];
        m_lhME[0] = -999.;
        m_lhME[1] = eleIdLikelihoodEle[sublead];
        m_bdtME[0] = -999.;
        m_bdtME[1] = eleBDT(fMVA,sublead);
        m_chmajME[0] = -999.;
        m_chmajME[1] = eleChargeMajority(sublead);
      }

    } else {
      float ptE = GetPt(pxEle[myEle],pyEle[myEle]);
      float ptM = GetPt(pxMuon[myMuPlus],pyMuon[myMuPlus]);
      
      if (ptE > ptM) {
        eleCands[em].push_back(myEle);
        muCands[em].push_back(myMuPlus);
        m_p4LeptonMinus[em] -> SetXYZT(pxEle[myEle],pyEle[myEle],pzEle[myEle],energyEle[myEle]);
        m_p4LeptonPlus[em]  -> SetXYZT(pxMuon[myMuPlus],pyMuon[myMuPlus],pzMuon[myMuPlus],energyMuon[myMuPlus]);
	m_p4LeptonMinusEnergy[em] = energyEle[myEle];
	m_p4LeptonPlusEnergy[em]  = energyMuon[myMuPlus];
	m_p4MinusType[em] = 0;
	m_p4PlusType[em]  = 1;
        hardestLeptonPt[em] = ptE;
        slowestLeptonPt[em] = ptM;
        m_mll[em]           = (*(m_p4LeptonMinus[em]) + *(m_p4LeptonPlus[em])).M();
        m_deltaPhi[em]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[em]->Vect().DeltaPhi(m_p4LeptonPlus[em]->Vect()));
        m_deltaErre[em]     = m_p4LeptonMinus[em]->Vect().DeltaR(m_p4LeptonPlus[em]->Vect());
	m_deltaEtaLeptons[em] = etaEle[myEle]-etaEle[myMuPlus];
        m_dilepPt[em].SetXYZ( m_p4LeptonMinus[em]->Vect().X()+m_p4LeptonPlus[em]->Vect().X(),m_p4LeptonMinus[em]->Vect().Y()+m_p4LeptonPlus[em]->Vect().Y(),0.0 );
	// usual variable
	m_transvMass[em]    = sqrt( 2.*(m_dilepPt[em].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[em]))) );      
	// chris' variable
        m_GammaMR[em] = CalcGammaMRstar(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em]);
        m_MR[em] = CalcMRNEW(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
        m_MTR[em] = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],*m_p3PFMET);
        m_metOptll[em]      = m_theMET / m_dilepPt[em].Pt();
        m_mT2[em]           = 0.;
        m_projectedMet[em]  = GetProjectedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_projectedPFMet[em] = GetProjectedPFMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_projectedTkMet[em] = GetProjectedTkMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
	m_MTRcharged[em]     = CalcMTR(*m_p4LeptonMinus[em],*m_p4LeptonPlus[em],m_p3TKMET[em]);
        m_p3TKMET[em] = pfChargedMet(m_p4LeptonMinus[em]->Vect(),m_p4LeptonPlus[em]->Vect());
        m_chMet[em] = (m_p3TKMET[em]).Pt();

        int lead = myEle;
        int sublead = myMuPlus;
        m_chEM[0] = chargeEle[lead];
        m_chEM[1] = chargeMuon[sublead];
        m_isoEM[0] = pfCombinedIsoEle[lead] / hardestLeptonPt[em];
        m_isoEM[1] = pfCombinedIsoMuon[sublead] / slowestLeptonPt[em];
        m_lhEM[0] = eleIdLikelihoodEle[lead];
        m_lhEM[1] = -999.;
        m_bdtEM[0] = eleBDT(fMVA,lead);
        m_bdtEM[1] = -999.;
        m_chmajEM[0] = eleChargeMajority(lead);
        m_chmajEM[1] = -999.;
      } else {
        eleCands[me].push_back(myEle);
        muCands[me].push_back(myMuPlus);
        m_p4LeptonMinus[me] -> SetXYZT(pxEle[myEle],pyEle[myEle],pzEle[myEle],energyEle[myEle]);
        m_p4LeptonPlus[me]  -> SetXYZT(pxMuon[myMuPlus],pyMuon[myMuPlus],pzMuon[myMuPlus],energyMuon[myMuPlus]);
	m_p4LeptonMinusEnergy[me] = energyEle[myEle];
	m_p4LeptonPlusEnergy[me]  = energyMuon[myMuPlus];
	m_p4MinusType[me] = 0;
	m_p4PlusType[me]  = 1;
        hardestLeptonPt[me] = ptM;
        slowestLeptonPt[me] = ptE;
        m_mll[me]           = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
        m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
        m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
	m_deltaEtaLeptons[me] = etaEle[myEle]-etaEle[myMuPlus];
        m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
	// usul variable
	m_transvMass[me]    = sqrt( 2.*(m_dilepPt[me].Pt())*(m_p3PFMET->Pt())*(1- cos(m_p3PFMET->DeltaPhi(m_dilepPt[me]))) );      
	// chris' variable
        m_GammaMR[me] = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
        m_MR[me] = CalcMRNEW(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
        m_MTR[me] = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],*m_p3PFMET);
        m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
        m_mT2[me]           = 0.;
        m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_projectedPFMet[me] = GetProjectedPFMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_projectedTkMet[me] = GetProjectedTkMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
	m_MTRcharged[me]     = CalcMTR(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me],m_p3TKMET[me]);
        m_p3TKMET[me] = pfChargedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
        m_chMet[me] = (m_p3TKMET[me]).Pt();

        int lead = myMuPlus;
        int sublead = myEle;
        m_chME[0] = chargeMuon[lead];
        m_chME[1] = chargeEle[sublead];
        m_isoME[0] = pfCombinedIsoMuon[lead] / hardestLeptonPt[me];
        m_isoME[1] = pfCombinedIsoEle[sublead] / slowestLeptonPt[me];
        m_lhME[0] = -999.;
        m_lhME[1] = eleIdLikelihoodEle[sublead];
        m_bdtME[0] = -999.;
        m_bdtME[1] = eleBDT(fMVA,sublead);
        m_chmajME[0] = -999.;
        m_chmajME[1] = eleChargeMajority(sublead);
      }
    }
  }
  
}

void HiggsMLSelection::resetKinematicsStart() {

  theElectron  = -1;
  thePositron  = -1;
  theMuonMinus = -1;
  theMuonPlus  = -1;

  thePreElectron  = -1;
  thePrePositron  = -1;
  thePreMuonMinus = -1;
  thePreMuonPlus  = -1;
}

void HiggsMLSelection::resetKinematics() {

  m_p3PFMET                   -> SetXYZ(0,0,0);
  for(int theChannel=0; theChannel<4; theChannel++) {
    eleCands[theChannel].clear();
    muCands[theChannel].clear();
    m_p4LeptonMinus[theChannel] -> SetXYZT(0,0,0,0);                                                        
    m_p4LeptonPlus[theChannel]  -> SetXYZT(0,0,0,0);
    hardestLeptonPt[theChannel]   = 0.;
    slowestLeptonPt[theChannel]   = 0.;
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
  }
}




int HiggsMLSelection::numJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel) {

  int num=0;
  m_goodJets.clear();
  float ETMax=0.;
  float ETMax2=0.;
  nsoftjets[theChannel]=0;

  theLeadingJet[theChannel]=-1;   
  theSecondJet[theChannel]=-1;   
  m_jetsSum[theChannel]->SetXYZT(0.,0.,0.,0);

  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);

    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    float rawpt = GetPt(uncorrpxAK5PFPUcorrJet[j],uncorrpyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) pt = (GetJESCorrected(p4Jet,JESUncertainty.Data())).Pt();

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    
//     if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
//                   chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    // Loose MvaId
    if (!isLooseJetMva(pt,etaAK5PFPUcorrJet[j],jetIdMvaPhilV1AK5PFPUcorrJet[j])) continue;

    bool foundMatch = false;

    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        H_deltaRcorr -> Fill(deltaR);
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
        H_deltaRcorr -> Fill(deltaR);
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
      ETMax2 = ETMax;
      
      theLeadingJet[theChannel] = j;
      leadJetBtag[theChannel] = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];
      leadJetBtagBProb[theChannel] = jetBProbabilityBJetTagsAK5PFPUcorrJet[j];
      ETMax = pt;

    } else if ( pt>ETMax2 && pt<ETMax ) {
      theSecondJet[theChannel] = j;
      subleadJetBtag[theChannel] = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];
      subleadJetBtagBProb[theChannel] = jetBProbabilityBJetTagsAK5PFPUcorrJet[j];

      ETMax2 = pt;
    }
  
    if(pt>10.) nsoftjets[theChannel]++;
    if(_selectionEE->getSwitch("etJetAcc") && !_selectionEE->passCut("etJetAcc", pt)) continue;
    
    m_goodJets.push_back(j);
    num++;
    
  }

  // extra info for jet Id studies
  if( wantJetIdStuff ) {
    int theFirst  = theLeadingJet[theChannel];
    int theSecond = theSecondJet[theChannel];
    
    // leading jet
    if (theFirst>-1) {
      float firstNeutralHadFrac    = neutralHadronEnergyAK5PFPUcorrJet[theFirst]/uncorrenergyAK5PFPUcorrJet[theFirst];
      float firstNeutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[theFirst]/uncorrenergyAK5PFPUcorrJet[theFirst];
      int firstNConstituents       = chargedHadronMultiplicityAK5PFPUcorrJet[theFirst] + neutralHadronMultiplicityAK5PFPUcorrJet[theFirst] + photonMultiplicityAK5PFPUcorrJet[theFirst] + electronMultiplicityAK5PFPUcorrJet[theFirst] + muonMultiplicityAK5PFPUcorrJet[theFirst] + HFHadronMultiplicityAK5PFPUcorrJet[theFirst] + HFEMMultiplicityAK5PFPUcorrJet[theFirst];
      float firstChargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[theFirst]/uncorrenergyAK5PFPUcorrJet[theFirst];
      int firstChargedMultiplicity  = chargedHadronMultiplicityAK5PFPUcorrJet[theFirst] + electronMultiplicityAK5PFPUcorrJet[theFirst] + muonMultiplicityAK5PFPUcorrJet[theFirst];
      float firstChargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[theFirst]/uncorrenergyAK5PFPUcorrJet[theFirst];
      float firstPt = sqrt( pxAK5PFPUcorrJet[theFirst]*pxAK5PFPUcorrJet[theFirst] + pyAK5PFPUcorrJet[theFirst]*pyAK5PFPUcorrJet[theFirst] );
      
      leadJetPt[theChannel]        = firstPt;
      leadJetEta[theChannel]       = etaAK5PFPUcorrJet[theFirst];
      leadJetLoosePFId[theChannel] = isPFJetID(fabs(etaAK5PFPUcorrJet[theFirst]),firstNeutralHadFrac,firstNeutralEmFraction,firstNConstituents,firstChargedHadFraction,firstChargedMultiplicity,firstChargedEmFraction, Higgs::loose);
      leadJetMvaJetId[theChannel]  = jetIdMvaPhilV1AK5PFPUcorrJet[theFirst];
      leadJetLooseId[theChannel]   = isLooseJetMva(leadJetPt[theChannel],etaAK5PFPUcorrJet[theFirst],jetIdMvaPhilV1AK5PFPUcorrJet[theFirst]);
    } else {
      leadJetPt[theChannel]        = -999.;
      leadJetEta[theChannel]       = -999.;
      leadJetLoosePFId[theChannel] = -999;
      leadJetMvaJetId[theChannel]  = -999.;
      leadJetLooseId[theChannel]   = -999;
    }
    
    // subleading jet
    if (theSecond>-1) {
      float secondNeutralHadFrac    = neutralHadronEnergyAK5PFPUcorrJet[theSecond]/uncorrenergyAK5PFPUcorrJet[theSecond];
      float secondNeutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[theSecond]/uncorrenergyAK5PFPUcorrJet[theSecond];
      int secondNConstituents       = chargedHadronMultiplicityAK5PFPUcorrJet[theSecond] + neutralHadronMultiplicityAK5PFPUcorrJet[theSecond] + photonMultiplicityAK5PFPUcorrJet[theSecond] + electronMultiplicityAK5PFPUcorrJet[theSecond] + muonMultiplicityAK5PFPUcorrJet[theSecond] + HFHadronMultiplicityAK5PFPUcorrJet[theSecond] + HFEMMultiplicityAK5PFPUcorrJet[theSecond];
      float secondChargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[theSecond]/uncorrenergyAK5PFPUcorrJet[theSecond];
      int secondChargedMultiplicity  = chargedHadronMultiplicityAK5PFPUcorrJet[theSecond] + electronMultiplicityAK5PFPUcorrJet[theSecond] + muonMultiplicityAK5PFPUcorrJet[theSecond];
      float secondChargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[theSecond]/uncorrenergyAK5PFPUcorrJet[theSecond];
      float secondPt = sqrt( pxAK5PFPUcorrJet[theSecond]*pxAK5PFPUcorrJet[theSecond] + pyAK5PFPUcorrJet[theSecond]*pyAK5PFPUcorrJet[theSecond] );
      subleadJetPt[theChannel]        = secondPt;
      subleadJetEta[theChannel]       = etaAK5PFPUcorrJet[theSecond];
      subleadJetLoosePFId[theChannel] = isPFJetID(fabs(etaAK5PFPUcorrJet[theSecond]),secondNeutralHadFrac,secondNeutralEmFraction,secondNConstituents,secondChargedHadFraction,secondChargedMultiplicity,secondChargedEmFraction, Higgs::loose);
      subleadJetMvaJetId[theChannel]  = jetIdMvaPhilV1AK5PFPUcorrJet[theSecond];
      subleadJetLooseId[theChannel]   = isLooseJetMva(subleadJetPt[theChannel],etaAK5PFPUcorrJet[theSecond],jetIdMvaPhilV1AK5PFPUcorrJet[theSecond]);
    } else {
      subleadJetPt[theChannel]        = -999.;
      subleadJetEta[theChannel]       = -999.;
      subleadJetLoosePFId[theChannel] = -999;
      subleadJetMvaJetId[theChannel]  = -999.;
      subleadJetLooseId[theChannel]   = -999;
    }
    
    // match with gen jets
    int firstAss  = -999;
    int secondAss = -999;
    float firstDRmin  = 999.;
    float secondDRmin = 999.;
    if (theFirst>-1) {
      for (int iGen=0; iGen<nAK5GenJet; iGen++) {
	TVector3 t3GenJet(pxAK5GenJet[iGen],pyAK5GenJet[iGen],pzAK5GenJet[iGen]);
	TVector3 t3FirstRecoJet(pxAK5PFPUcorrJet[theFirst],pyAK5PFPUcorrJet[theFirst],pzAK5PFPUcorrJet[theFirst]);
	float genPt   = t3GenJet.Perp();
	float firstPt = sqrt( pxAK5PFPUcorrJet[theFirst]*pxAK5PFPUcorrJet[theFirst] + pyAK5PFPUcorrJet[theFirst]*pyAK5PFPUcorrJet[theFirst] );
	float firstDR = t3FirstRecoJet.DeltaR(t3GenJet);
	double firstExpres  = ErrEt(firstPt,t3FirstRecoJet.Eta());
	if ( (firstDR<firstDRmin) && ((fabs(firstPt-genPt))/genPt)<0.5 ) {
	  firstAss = iGen;
	  firstDRmin = firstDR;
	}
      }
      if (firstAss>-999) {
	float firstGenAssPt = sqrt(pxAK5GenJet[firstAss]*pxAK5GenJet[firstAss] + pyAK5GenJet[firstAss]*pyAK5GenJet[firstAss]);
	if (firstDRmin > 0.1 + 0.3 * exp(-0.05*(firstGenAssPt-10))) firstAss = -999;
      }
      if (firstAss>-1) leadJetMatchGen[theChannel] = 1;  
      if (firstAss<0)  leadJetMatchGen[theChannel] = 0;  
    } else {
      leadJetMatchGen[theChannel] = -999;
    }
    
    if (theSecond>-1) {
      for (int iGen=0; iGen<nAK5GenJet; iGen++) {
	TVector3 t3GenJet(pxAK5GenJet[iGen],pyAK5GenJet[iGen],pzAK5GenJet[iGen]);
	TVector3 t3SecondRecoJet(pxAK5PFPUcorrJet[theSecond],pyAK5PFPUcorrJet[theSecond],pzAK5PFPUcorrJet[theSecond]);
	float genPt = t3GenJet.Perp();
	float secondPt = sqrt( pxAK5PFPUcorrJet[theSecond]*pxAK5PFPUcorrJet[theSecond] + pyAK5PFPUcorrJet[theSecond]*pyAK5PFPUcorrJet[theSecond] );
	float secondDR = t3SecondRecoJet.DeltaR(t3GenJet);
	double secondExpres = ErrEt(secondPt,t3SecondRecoJet.Eta());
	if ( (secondDR<secondDRmin) && ((fabs(secondPt-genPt))/genPt)<0.5 ) {
	  secondAss = iGen;
	  secondDRmin = secondDR;
	}
      }
      if (secondAss>-999) {
	float secondGenAssPt = sqrt(pxAK5GenJet[secondAss]*pxAK5GenJet[secondAss] + pyAK5GenJet[secondAss]*pyAK5GenJet[secondAss]);
	if (secondDRmin > 0.1 + 0.3 * exp(-0.05*(secondGenAssPt-10))) secondAss = -999;
      }
      if (secondAss>-1) subleadJetMatchGen[theChannel] = 1;  
      if (secondAss<0)  subleadJetMatchGen[theChannel] = 0;  
    } else {
      subleadJetMatchGen[theChannel] =  -999;
    }
  }

  return num;
}


int HiggsMLSelection::numUncorrJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  int num=0;

  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));

  m_uncorrJetsSum[theChannel]->SetXYZT(0.,0.,0.,0.);

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    float uncorrEt = uncorrenergyAK5PFPUcorrJet[j]*fabs(sin(thetaAK5PFPUcorrJet[j]));
    TLorentzVector p4Jet;
    p4Jet.SetPtEtaPhiE(uncorrEt,etaAK5PFPUcorrJet[j],phiAK5PFPUcorrJet[j],uncorrenergyAK5PFPUcorrJet[j]);
    TVector3 p3Jet = p4Jet.Vect();

    TLorentzVector p4JESJet(p3Jet, uncorrenergyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) uncorrEt = (GetJESCorrected(p4JESJet,JESUncertainty.Data())).Pt();

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    
//     if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
//                   chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    bool foundMatch=false;
    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        H_deltaRcorr -> Fill(deltaR);
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
        H_deltaRcorr -> Fill(deltaR);
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

float HiggsMLSelection::bVetoJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel ) {

  TString JESUncertainty(_selectionEE->getStringParameter("JESUncertainty"));

  float maxTCHE=-999;
  float maxJetBProb=-999;
  float outputSubLeadJets = -999;
  float outputSubLeadJetsBProb = -999;
  m_numbtagjets[theChannel]=0;
  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    // no threshold is applied here on pt. Not affected by JES uncertainties
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);

    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    if(JESUncertainty == TString("Up") || JESUncertainty == TString("Down")) pt = (GetJESCorrected(p4Jet,JESUncertainty.Data())).Pt();

    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;
    
    // if(theSecondJet[theChannel]>-1 && fabs(etaAK5PFPUcorrJet[j])>4.5) continue;

    // hardcoded
    float rawpt = GetPt(uncorrpxAK5PFPUcorrJet[j],uncorrpyAK5PFPUcorrJet[j]);
    if(rawpt < 10.0) continue;

    //    if(weightedDz1AK5PFPUcorrJet[j] >= 2) continue;

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    
//     if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
//                   chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    // Loose MvaId 
    if (!isLooseJetMva(pt,etaAK5PFPUcorrJet[j],jetIdMvaPhilV1AK5PFPUcorrJet[j])) continue;

    bool foundMatch=false;
    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        H_deltaRcorr -> Fill(deltaR);
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
        H_deltaRcorr -> Fill(deltaR);
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    float tmpTCHE = trackCountingHighEffBJetTagsAK5PFPUcorrJet[j];     
    float tmpJBP = jetBProbabilityBJetTagsAK5PFPUcorrJet[j];
    if(tmpTCHE > maxTCHE) maxTCHE = tmpTCHE; 
    //     if(pt<=30 && tmpTCHE > maxTCHE) maxTCHE = tmpTCHE;
    //     if(pt>30 && tmpJBP > maxJetBProb) maxJetBProb = tmpJBP;

    if(j != theLeadingJet[theChannel] && tmpTCHE > outputSubLeadJets) outputSubLeadJets = tmpTCHE;
    if(j != theLeadingJet[theChannel] && tmpJBP > outputSubLeadJetsBProb) outputSubLeadJetsBProb = tmpJBP;
    if(tmpTCHE>1.6) m_numbtagjets[theChannel]++;

  }

  subLeadJetsMaxBtag[theChannel] = outputSubLeadJets;
  subLeadJetsMaxBtagBProb[theChannel] = outputSubLeadJetsBProb;

  int lj = theLeadingJet[theChannel];
  float ptleadjet = GetPt(pxAK5PFPUcorrJet[lj],pyAK5PFPUcorrJet[lj]);

  // hardcode the cuts
  float bitval = 1;
  if(maxTCHE>=2.1) bitval=0.;
  // give data-MC discrepancies, revert to 2011 TCHE only
  // if(maxJetBProb>=1.05) bitval=0.;
  
  m_softbdisc[theChannel]=maxTCHE;
  m_hardbdisc[theChannel]=maxJetBProb;

  return bitval;

}

float HiggsMLSelection::deltaPhiLLJet(int ichan) {   
  
  int myLeadingJet = theLeadingJet[ichan];
  int mySecondJet = theSecondJet[ichan];

  // if the event has >=2 jet, the cut is on dphi(ll-jj). But the acceptance on jets has to be restricted from 5.0 to 4.5
  if( mySecondJet > -1 && myLeadingJet > -1 && m_dilepPt[ichan].Pt()>0 
      && fabs(etaAK5PFPUcorrJet[myLeadingJet])<4.5 && fabs(etaAK5PFPUcorrJet[mySecondJet])<4.5 ) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);
    TVector3 subleadingJetP3(pxAK5PFPUcorrJet[mySecondJet],pyAK5PFPUcorrJet[mySecondJet],pzAK5PFPUcorrJet[mySecondJet]);
    TVector3 jjP3 = leadingJetP3+subleadingJetP3;
    return fabs(180./TMath::Pi() * jjP3.DeltaPhi(m_dilepPt[ichan]));
  }


  // if the event has <=1 jet, the cut is on dphi(ll-j)
  if(myLeadingJet > -1 && m_dilepPt[ichan].Pt()>0) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);    
    if(leadingJetP3.Pt()>15.0) return fabs(180./TMath::Pi() * leadingJetP3.DeltaPhi(m_dilepPt[ichan]));                           
    else return 0.1;
  } else return 0.1;
}

int HiggsMLSelection::numSoftMuons(std::vector<int> muonToRemove, std::vector<int> jetsToRemove) {

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
    float dzMuon = fabs(muonDzPV(i,0));
    if(dxyMuon > 0.200) continue;     // hardcoded  
    if(dzMuon  > 0.200) continue;     // hardcoded  

    float isoSumRel = (sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i]) / pt;
    //    float isoSumRel = pfCombinedIsoMuon[i] / pt;
    if(pt>20 && isoSumRel<0.1) continue;  
    
    num++;
  }
  return num;
}

int HiggsMLSelection::numExtraLeptons( std::vector<int> eleToRemove, std::vector<int> muonToRemove  ) {

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
      isEleID2012AndDenom(i,&theId,&theIso,&theConvRej);
    if (_selectionEE->getSwitch("asymmetricID")) {
      float pt = GetPt(pxEle[i],pyEle[i]);	
      if(pt>=20) isEleID2012AndDenom(i,&theId,&theIso,&theConvRej);
      if(pt<20)  isEleID2012AndDenom(i,&theId,&theIso,&theConvRej);
    }
    if(!theId || !theIso || !theConvRej) continue;
    
    // further requests if we apply the smurf ID and pT<15
//     TString stringIdLow (_selectionEE->getStringParameter("electronIDTypeLow"));
//     if( stringIdLow.Contains("Smurf") && !m_useBDTEleID ) {
//       float pt = GetPt(pxEle[i],pyEle[i]);
//       if ( pt<20  ) {
// 	if ( fbremEle[i]<0.15 && !(fabs(etaEle[i])<1.0 && eSuperClusterOverPEle[i]>0.95) ) continue;
//       }
//     }

    int track = gsfTrackIndexEle[i];
    // float d3dEle = impactPar3DGsfTrack[track];
    // if (_selectionEE->getSwitch("electronIP") && (!_selectionEE->passCut("electronIP",d3dEle)) ) continue;    
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
    isMuonID2012(i,&theId);
    if(!theId) continue;
//     float isoSumAbs = sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i] - rhoFastjet*TMath::Pi()*0.3*0.3;
//     float isoSumRel = isoSumAbs / ptMu;
//    float isoSumRel = pfCombinedIsoMuon[i] / ptMu;
//    if(_selectionMM->getSwitch("muGlobalIso") && !_selectionMM->passCut("muGlobalIso",isoSumRel)) continue;
    if( ! isPFIsolatedMuon2012(i) ) continue; 

    int track = trackIndexMuon[i];
    float dxy = transvImpactParTrack[track];
    float dz  = muonDzPV(i,0);  

    if (ptMu>20) {   // hardcoded
      if (_selectionEE->getSwitch("muonIPhighPT") && (!_selectionEE->passCut("muonIPhighPT",dxy)) ) continue;   
    } 
    if (ptMu<20) {   // hardcoded
      if (_selectionEE->getSwitch("muonIPlowPT")  && (!_selectionEE->passCut("muonIPlowPT",dxy)) ) continue;   
    }
    if (_selectionEE->getSwitch("muonDz") && (!_selectionEE->passCut("muonDz",dz)) )  continue;   

    numMu++;
  }
  
  return numEle + numMu;
}

void HiggsMLSelection::setLepIdVariables(int hard, int slow, int hardpdgid, int slowpdgid) {

  Utils anaUtils;

  int selectedLeptons[2];
  selectedLeptons[0] = hard;
  selectedLeptons[1] = slow; 
  int pdgids[2];
  pdgids[0] = hardpdgid;
  pdgids[1] = slowpdgid;

  for(int i = 0; i < 2; i++) {
    if(selectedLeptons[i] > -1) {
      int lepIndex = selectedLeptons[i];
      if(pdgids[i]==11) {
        myPt[i] = GetPt(pxEle[lepIndex],pyEle[lepIndex]);
        myEta[i] = etaEle[lepIndex];
        myPhi[i] = phiEle[lepIndex];
        myLepId[i] = mvaidtrigEle[lepIndex];
        myLepIso[i] = corrEleIso2012(lepIndex);
        int gsf = gsfTrackIndexEle[lepIndex];  
        myConv[i] = (!hasMatchedConversionEle[lepIndex] && expInnerLayersGsfTrack[gsf]==0);
      } else if(pdgids[i]==13) {
        myPt[i] = GetPt(pxMuon[lepIndex],pyMuon[lepIndex]);
        myEta[i] = etaMuon[lepIndex];
        myPhi[i] = phiMuon[lepIndex];
        bool muid;
        isMuonID2012(lepIndex,&muid);
        myLepId[i] = (muid) ? 1. : 0.;
        myLepIso[i] = mvaisoMuon[lepIndex];      
        myConv[i] = 1.;
      } else {
        cout << "MISTAKE! WRONG PDG TYPE!" << endl;
      }
    } else {
      myPt[i] = -999.;
      myEta[i] = -999.;
      myPhi[i] = -999.;
      myLepId[i] = -999.;
      myLepIso[i] = -999.;
      myConv[i] = -999.;
    }
  }
}

int HiggsMLSelection::getPV() {
  int hardestPV = -1;
  float sumPtMax = 0.0;
  for(int v=0; v<nPV; v++) {
    if(SumPtPV[v] > sumPtMax) {
      sumPtMax = SumPtPV[v];
      hardestPV = v;
    }
  }
  return hardestPV;
}

bool HiggsMLSelection::isGoodTrack(int iTrack, float ptMin, float ptMax, float chi2, float etaMax, float nHits) {
  TVector3 p3Track(pxTrack[iTrack],pyTrack[iTrack],pzTrack[iTrack]);
  double pt = p3Track.Pt();
  if(pt < ptMin) return false;
  if(pt > ptMax) return false;
  if(trackNormalizedChi2Track[iTrack] > chi2) return false; 
  if(fabs(p3Track.Eta()) > etaMax) return false;
  if(trackValidHitsTrack[iTrack] < nHits) return false;
  return true;
}

double HiggsMLSelection::mT(TVector3 plep, TVector3 pneu) {

  TVector3 pTlep(plep.X(),plep.Y(),0.0);
  TVector3 pTneu(pneu.X(),pneu.Y(),0.0);

  return sqrt(2 * (pTlep.Mag()*pTneu.Mag() - pTlep*pTneu));

}

double HiggsMLSelection::mT2(TVector3 plep1, TVector3 plep2, TVector3 ptmiss) {

  // need an external dependency: removed right now
//   Mt2::TwoVector pTlep1(plep1.X(),plep1.Y());
//   Mt2::TwoVector pTlep2(plep2.X(),plep2.Y());
//   Mt2::TwoVector pTmiss(ptmiss.X(),ptmiss.Y());
//   double invis_mass = 0.0;

//   Mt2::SUSYPhys_Mt2_222_Calculator mt2Calculator;

//   // Could tell the MT2 calculating object to be verbose, and print out
//   // debug messages while it is thinking ... but we won't:

//   mt2Calculator.setDebug(false);

//   // Now we can actually calculate MT2:
//   double mt2 = mt2Calculator.mt2_222( pTlep1, pTlep2, pTmiss, invis_mass);

  double mt2 = 0.0;
  return mt2;

}

float HiggsMLSelection::GetProjectedMet(TVector3 p1, TVector3 p2) {

  // calculate with PF met
  float projMET_pf = 0.0;
  float deltaPhi1_pf = fabs(p1.DeltaPhi(*m_p3PFMET));
  float deltaPhi2_pf = fabs(p2.DeltaPhi(*m_p3PFMET));
  float deltaphi_pf = TMath::Min(deltaPhi1_pf,deltaPhi2_pf);
  if(deltaphi_pf<TMath::Pi()/2.) projMET_pf = m_p3PFMET->Mag() * fabs(sin(deltaphi_pf));
  else projMET_pf = m_p3PFMET->Mag();

  // calculate with TKMET
  TVector3 p3tkMet = pfChargedMet(p1,p2);
  float projMET_tk = 0.0;
  float deltaPhi1_tk = fabs(p1.DeltaPhi(p3tkMet));
  float deltaPhi2_tk = fabs(p2.DeltaPhi(p3tkMet));
  float deltaphi_tk  = TMath::Min(deltaPhi1_tk,deltaPhi2_tk);
  if(deltaphi_tk<TMath::Pi()/2.) projMET_tk = p3tkMet.Mag() * fabs(sin(deltaphi_tk));
  else projMET_tk = p3tkMet.Mag();

  return TMath::Min(projMET_pf,projMET_tk);
}

// calculate with PF met
float HiggsMLSelection::GetProjectedPFMet(TVector3 p1, TVector3 p2) {  

  float projMET_pf = 0.0;
  float deltaPhi1_pf = fabs(p1.DeltaPhi(*m_p3PFMET));
  float deltaPhi2_pf = fabs(p2.DeltaPhi(*m_p3PFMET));
  float deltaphi_pf = TMath::Min(deltaPhi1_pf,deltaPhi2_pf);
  if(deltaphi_pf<TMath::Pi()/2.) projMET_pf = m_p3PFMET->Mag() * fabs(sin(deltaphi_pf));
  else projMET_pf = m_p3PFMET->Mag();

  return projMET_pf;
}

// calculate with TK met  
float HiggsMLSelection::GetProjectedTkMet(TVector3 p1, TVector3 p2) {  

  TVector3 p3tkMet = pfChargedMet(p1,p2);
  float projMET_tk = 0.0;
  float deltaPhi1_tk = fabs(p1.DeltaPhi(p3tkMet));
  float deltaPhi2_tk = fabs(p2.DeltaPhi(p3tkMet));
  float deltaphi_tk  = TMath::Min(deltaPhi1_tk,deltaPhi2_tk);
  if(deltaphi_tk<TMath::Pi()/2.) projMET_tk = p3tkMet.Mag() * fabs(sin(deltaphi_tk));
  else projMET_tk = p3tkMet.Mag();

  return projMET_tk;
}

/// specific for HWW that has multiple channels with different HLT requirements
bool HiggsMLSelection::reloadTriggerMask(int runN)
{
  std::vector<int> triggerMask;

  // load the triggers required for EE
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersEE.begin();fIter!=requiredTriggersEE.end();++fIter)
    {   
      std::string pathName = getHLTPathForRun(runN,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          //if( !strcmp ((*fIter).c_str(), nameHLT->at(i).c_str() ) )
          // nameHLT[i] has ..._vXXX
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
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

  // load the triggers required for MM
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersMM.begin();fIter!=requiredTriggersMM.end();++fIter)
    {   
      //      std::cout << "For MM required: " << *fIter << std::endl;
      std::string pathName = getHLTPathForRun(runN,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
              triggerMask.push_back( indexHLT[i] ) ;
              break;
            }
        }
    }
  m_requiredTriggersMM = triggerMask;

  // load the triggers NOT required for MM
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=notRequiredTriggersMM.begin();fIter!=notRequiredTriggersMM.end();++fIter)
    {   
      //      std::cout << "For MM not required: " << *fIter << std::endl;
      std::string pathName = getHLTPathForRun(runN,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
              triggerMask.push_back( indexHLT[i] ) ;
              break;
            }
        }
    }
  m_notRequiredTriggersMM = triggerMask;

  // load the triggers required for EM
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersEM.begin();fIter!=requiredTriggersEM.end();++fIter)
    {   
      //      std::cout << "For EM required: " << *fIter << std::endl;
      std::string pathName = getHLTPathForRun(runN,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
              triggerMask.push_back( indexHLT[i] ) ;
              break;
            }
        }
    }
  m_requiredTriggersEM = triggerMask;

  // load the triggers NOT required for EM
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=notRequiredTriggersEM.begin();fIter!=notRequiredTriggersEM.end();++fIter)
    {   
      //      std::cout << "For EM not required: " << *fIter << std::endl;
      std::string pathName = getHLTPathForRun(runN,*fIter);
      for(unsigned int i=0; i<nameHLT->size(); i++)
        {
          if(nameHLT->at(i).find(pathName) != string::npos)
            {
              triggerMask.push_back( indexHLT[i] ) ;
              break;
            }
        }
    }
  m_notRequiredTriggersEM = triggerMask;

}

bool HiggsMLSelection::hasPassedHLT(int channel) {
  Utils anaUtils;
  if(channel==ee) { 
    bool required = anaUtils.getTriggersOR(m_requiredTriggersEE, firedTrg);
    bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersEE, firedTrg);
    return (required && !notRequired);
  } else if(channel==mm) {
    bool required = anaUtils.getTriggersOR(m_requiredTriggersMM, firedTrg);
    bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersMM, firedTrg);
    return (required && !notRequired);
  } else if(channel==em) {
    bool required = anaUtils.getTriggersOR(m_requiredTriggersEM, firedTrg);
    bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersEM, firedTrg);
    return (required && !notRequired);
  }
  return true;
}

void HiggsMLSelection::setRequiredTriggers(const std::vector<std::string>& reqTriggers, int channel) {
  if(channel==ee) requiredTriggersEE=reqTriggers;
  else if(channel==mm) requiredTriggersMM=reqTriggers;
  else if(channel==em) requiredTriggersEM=reqTriggers;
  else std::cout << "WARNING: triggers are set for an unknown channel!" << std::endl;
}

void HiggsMLSelection::setNotRequiredTriggers(const std::vector<std::string>& reqTriggers, int channel) {
  if(channel==ee) notRequiredTriggersEE=reqTriggers;
  else if(channel==mm) notRequiredTriggersMM=reqTriggers;
  else if(channel==em) notRequiredTriggersEM=reqTriggers;
  else std::cout << "WARNING: triggers are set for an unknown channel!" << std::endl;
}


void HiggsMLSelection::JESPfMet( std::vector<int> eleToRemove, std::vector<int> muonToRemove) {

  TLorentzVector jetSumNom, jetSumUp, jetSumDown;
  jetSumUp.SetXYZT(0.,0.,0.,0);
  jetSumDown.SetXYZT(0.,0.,0.,0);

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);
    TLorentzVector p4Jet(p3Jet, energyAK5PFPUcorrJet[j]);
    float pt = p4Jet.Pt();

    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;


    TLorentzVector p4JetUp = GetJESCorrected(p4Jet,"Up");
    float ptUp = p4JetUp.Pt();
    float energyUp = p4JetUp.E();

    TLorentzVector p4JetDown = GetJESCorrected(p4Jet,"Down");
    float ptDown = p4JetDown.Pt();
    float energyDown = p4JetDown.E();

    TLorentzVector p4JetJesUp, p4JetJesDown;
    p4JetJesUp.SetPtEtaPhiE(ptUp,p4Jet.Eta(),p4Jet.Phi(),energyUp);
    p4JetJesDown.SetPtEtaPhiE(ptDown,p4Jet.Eta(),p4Jet.Phi(),energyDown);

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/uncorrenergyAK5PFPUcorrJet[j];
    
//     if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
//                   chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;
    
    bool foundMatch = false;

    // check if the electrons falls into the jet
    for(int i=0; i<(int)eleToRemove.size(); i++) {
      int ele = eleToRemove[i];
      if ( ele > -1 ) {
        TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
        float deltaR =  fabs( p3Jet.DeltaR( p3Ele ) );
        H_deltaRcorr -> Fill(deltaR);
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
        H_deltaRcorr -> Fill(deltaR);
        // taking from ee config file, but jets veto is the same for all the channels
        if(_selectionEE->getSwitch("jetConeWidth") && _selectionEE->passCut("jetConeWidth",deltaR)) foundMatch=true;
      }
    }
    if(foundMatch) continue;

    if(pt>5.0) {
      jetSumNom += p4Jet;
      jetSumUp += p4JetJesUp;
      jetSumDown += p4JetJesDown;
    }

  }

  // add back the electron and muon candidates, with their calibrations
  TVector3 electronSum, muonSum;
  electronSum.SetXYZ(0,0,0);
  muonSum.SetXYZ(0,0,0);
  for(int i=0; i<(int)eleToRemove.size(); i++) {
    int ele = eleToRemove[i];
    if ( ele > -1 ) {
      TVector3 p3Ele(pxEle[ele],pyEle[ele],pzEle[ele]);
      electronSum += p3Ele;
    }
  }

  // check if the muons falls into the jet
  for(int i=0; i<(int)muonToRemove.size(); i++) {
    int mu = muonToRemove[i];
    if ( mu > -1 ) {
      TVector3 p3Muon(pxMuon[mu],pyMuon[mu],pzMuon[mu]);
      muonSum += p3Muon;
    }
  }

  TVector3 metFromJetsNom = jetSumNom.Vect() + electronSum + muonSum; 
  TVector3 metFromJetsUp = jetSumUp.Vect() + electronSum + muonSum; 
  TVector3 metFromJetsDown = jetSumDown.Vect() + electronSum + muonSum; 

  TVector3 diffUp = metFromJetsUp - metFromJetsNom;
  TVector3 diffDown = metFromJetsDown - metFromJetsNom;

  *m_metFromJets = metFromJetsNom;
  *m_pfMetJESUp = *m_p3PFMET + diffUp;
  *m_pfMetJESDown = *m_p3PFMET + diffDown;

}

std::pair<float,float> HiggsMLSelection::transvMassJES(int theChannel) {

  float mTUp = sqrt( 2.*(m_dilepPt[theChannel].Pt())*(m_pfMetJESUp->Pt())*(1- cos(m_pfMetJESUp->DeltaPhi(m_dilepPt[theChannel]))) );
  float mTDown = sqrt( 2.*(m_dilepPt[theChannel].Pt())*(m_pfMetJESDown->Pt())*(1- cos(m_pfMetJESDown->DeltaPhi(m_dilepPt[theChannel]))) );
  return std::make_pair(mTUp,mTDown);

}

std::vector<TLorentzVector> HiggsMLSelection::GetJetJesPcomponent(int jet) {

  // [nom/+1s/-1s]

  if(jet<0) {
    TLorentzVector up(0,0,0,0);
    std::vector<TLorentzVector> zero;
    zero.push_back(up);
    zero.push_back(up);
    zero.push_back(up);    
    return zero;
  }

  TLorentzVector JP4(pxAK5PFPUcorrJet[jet],pyAK5PFPUcorrJet[jet],pzAK5PFPUcorrJet[jet],energyAK5PFPUcorrJet[jet]);
  TLorentzVector LJP4JesUp = GetJESCorrected(JP4,"Up");
  TLorentzVector LJP4JesDown = GetJESCorrected(JP4,"Down");

  std::vector<TLorentzVector> jes;
  jes.push_back(JP4);
  jes.push_back(LJP4JesUp);
  jes.push_back(LJP4JesDown);

  return jes;

}

void HiggsMLSelection::getDYGeneratorKinematics(int lepType) {

  TLorentzVector p1, p2;

  for(int imc=2;imc<40;imc++) {
    if(fabs(idMc[mothMc[imc]])==23) {
      float pt = pMc[imc]*fabs(sin(thetaMc[imc]));
      if(idMc[imc]==lepType) p1.SetPtEtaPhiE(pt,etaMc[imc],phiMc[imc],energyMc[imc]);
      if(idMc[imc]==-lepType) p2.SetPtEtaPhiE(pt,etaMc[imc],phiMc[imc],energyMc[imc]);
    }
  }
  
  if(p1.Pt()>0 && p2.Pt()>0) {
    _genmll = (p1+p2).M();
    _genptll = (p1+p2).Pt();
    _genyll = (p1+p2).Rapidity();
  } 

}

TVector3 HiggsMLSelection::getLeadingJet(int index, float ptThr) {
  TVector3 p3(0,0,0);
  if(index>-1) {
    p3.SetXYZ(pxAK5PFPUcorrJet[index],pyAK5PFPUcorrJet[index],pzAK5PFPUcorrJet[index]);
    if(p3.Pt()>ptThr) return p3;
  }
  return p3;
}

double HiggsMLSelection::ErrEt( double Et, double Eta) {
  
  double InvPerr2;
  
  double N, S, C, m;
  if(fabs(Eta) < 0.5 ) {
    N = 3.96859;
    S = 0.18348;
    C = 0.;
    m = 0.62627;
  } else if( fabs(Eta) < 1. ) {
    N = 3.55226;
    S = 0.24026;
    C = 0.;
    m = 0.52571;
  } else if( fabs(Eta) < 1.5 ) {
    N = 4.54826;
    S = 0.22652;
    C = 0.;
    m = 0.58963;
  } else if( fabs(Eta) < 2. ) {
    N = 4.62622;
    S = 0.23664;
    C = 0.;
    m = 0.48738;
  } else if( fabs(Eta) < 3. ) {
    N = 2.53324;
    S = 0.34306;
    C = 0.;
    m = 0.28662;
  } else if( fabs(Eta) < 5. ) {
    N = 2.95397;
    S = 0.11619;
    C = 0.;
    m = 0.96086;
  }
  
  // this is the absolute resolution (squared), not sigma(pt)/pt	
  // so have to multiply by pt^2, thats why m+1 instead of m-1	
  InvPerr2 =  (N * fabs(N) ) + (S * S) * pow(Et, m+1) + (C * C) * Et * Et ;
  
  return sqrt(InvPerr2)/Et;
}

bool HiggsMLSelection::isLooseJetMva(float pt, float eta, float id) {

  bool isOk = true;

  if (pt<10) {
    if (fabs(eta)<=2.5 && id<0.0)                   isOk = false;
    if (fabs(eta)>2.5 && fabs(eta)<=2.75 && id<0.0) isOk = false;
    if (fabs(eta)>2.75 && fabs(eta)<=3.0 && id<0.0) isOk = false;
    if (fabs(eta)>3.0 && fabs(eta)<=5.0 && id<0.2)  isOk = false;
  }

  if (pt<20 && pt>=10) {
    if (fabs(eta)<=2.5 && id<-0.4)                   isOk = false;
    if (fabs(eta)>2.5 && fabs(eta)<=2.75 && id<-0.4) isOk = false;
    if (fabs(eta)>2.75 && fabs(eta)<=3.0 && id<-0.4) isOk = false;
    if (fabs(eta)>3.0 && fabs(eta)<=5.0 && id<0.4)   isOk = false;
  }

  if (pt<30 && pt>=20) {
    if (fabs(eta)<=2.5 && id<0.0)                   isOk = false;
    if (fabs(eta)>2.5 && fabs(eta)<=2.75 && id<0.0) isOk = false;
    if (fabs(eta)>2.75 && fabs(eta)<=3.0 && id<0.2) isOk = false;
    if (fabs(eta)>3.0 && fabs(eta)<=5.0 && id<0.6)  isOk = false;
  }

  if (pt<50 && pt>=30) {
    if (fabs(eta)<=2.5 && id<0.0)                   isOk = false;
    if (fabs(eta)>2.5 && fabs(eta)<=2.75 && id<0.0) isOk = false;
    if (fabs(eta)>2.75 && fabs(eta)<=3.0 && id<0.6) isOk = false;
    if (fabs(eta)>3.0 && fabs(eta)<=5.0 && id<0.2)  isOk = false;
  }

  return isOk;
}

double HiggsMLSelection::getDYMVA(int channel) {
  TVector3 ll = m_dilepPt[channel];

  int nj = njets[channel];
  double pmet = m_projectedPFMet[channel];
  double pTrackMet = m_projectedTkMet[channel];
  int nvtx = m_goodvertices;
  double dilpt = m_dilepPt[channel].Pt();
  
  int theLJ  = theLeadingJet[channel];
  TVector3 p3LJet(pxAK5PFPUcorrJet[theLJ],pyAK5PFPUcorrJet[theLJ],pzAK5PFPUcorrJet[theLJ]);
  double ptjet1 = std::max(15.,p3LJet.Pt());

  double metSig = mEtSigPFMet[0];
  double dPhiDiLepJet1 = (p3LJet.Pt()<15.) ? -0.1 : fabs(ll.DeltaPhi(p3LJet));
  double dPhiJet1MET =   (p3LJet.Pt()<15.) ? -0.1 : fabs(m_p3PFMET->DeltaPhi(p3LJet));
  double dPhiDiLepMET =  fabs(ll.DeltaPhi(*m_p3PFMET));
  double recoil = (ll+(*m_p3PFMET)).Pt();
  double mt = m_transvMass[channel];

  return m_dymvaAlgo->getValue(nj, pmet, pTrackMet, nvtx, dilpt, ptjet1, metSig,
                               dPhiDiLepJet1, dPhiDiLepMET, dPhiJet1MET, recoil, mt);
}
