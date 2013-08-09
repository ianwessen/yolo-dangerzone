#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/LeptonPlusFakeMLSelection_ME.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "CommonTools/include/LeptonIdBits.h"
#include "CommonTools/include/PUWeight.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <TTree.h>

using namespace bits;

LeptonPlusFakeMLSelection_ME::LeptonPlusFakeMLSelection_ME(TTree *tree) 
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
  std::string fileCutsME     = higgsConfigDirMass + "emu2nuCuts.txt";
  std::string fileSwitchesME = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionME.Configure(fileCutsME.c_str(),fileSwitchesME.c_str(),"FULL SELECTION EVENT COUNTER ME"); 
  CutBasedHiggsErrorsSelectionME.Configure(fileCutsME.c_str(),fileSwitchesME.c_str(),"FULL SELECTION ERRORS ME");
  _selectionME = CutBasedHiggsSelectionME.GetSelection();
  _selectionErrME = CutBasedHiggsErrorsSelectionME.GetSelection();

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
  
  // Reading GoodRUN LS
  std::cout << "[GoodRunLS]::goodRunLS is " << _selectionME->getSwitch("goodRunLS") << " isData is " <<  _selectionME->getSwitch("isData") << std::endl;

  // To read good run list!
  if (_selectionME->getSwitch("goodRunLS") && _selectionME->getSwitch("isData")) {
    std::string goodRunJsonFile = "config/json/goodCollisions2011.json";
    setJsonGoodRunList(goodRunJsonFile);
    fillRunLSMap();
  }

  // kinematics
  for(int theChannel=0; theChannel<1; theChannel++) {
    m_p4LeptonPlus[theChannel]  = new TLorentzVector(0.,0.,0.,0.);
    m_p4LeptonMinus[theChannel] = new TLorentzVector(0.,0.,0.,0.);
  }

  // met
  m_p3PFMET = new TVector3(0.,0.,0.);
  m_p3TKMET = new TVector3(0.,0.,0.);

  // b-veto event variables
  m_maxDxyEvt = 0.0;
  m_maxDszEvt = 0.0;
}

LeptonPlusFakeMLSelection_ME::~LeptonPlusFakeMLSelection_ME(){

  for(int theChannel=0; theChannel<1; theChannel++) {  
    delete m_p4LeptonPlus[theChannel];
    delete m_p4LeptonMinus[theChannel];
  }
  delete m_p3PFMET;  
  delete m_p3TKMET;  
  
  delete _selectionME;
  
  myOutTreeME -> save();
}

void LeptonPlusFakeMLSelection_ME::initialiseFakeRate() {

  // binning                                      
  m_minFakePt[0] = 10.;   m_maxFakePt[0] = 15.;
  m_minFakePt[1] = 15.;   m_maxFakePt[1] = 20.;
  m_minFakePt[2] = 20.;   m_maxFakePt[2] = 25.;
  m_minFakePt[3] = 25.;   m_maxFakePt[3] = 50.;
  m_minFakePt[4] = 50.;   m_maxFakePt[4] = 10000.;

  /*
  // fake in the barrel from QCD MC                                  
  m_fakeRateEB[0] = 0.31852;
  m_fakeRateEB[1] = 0.0815463;
  m_fakeRateEB[2] = 0.0707214;
  m_fakeRateEB[3] = 0.0588124;
  m_fakeRateEB[4] = 0.0458273;

  m_fakeRateEB_err[0] = 0.00598496;
  m_fakeRateEB_err[1] = 0.00467256;
  m_fakeRateEB_err[2] = 0.00381354;
  m_fakeRateEB_err[3] = 0.00335443;
  m_fakeRateEB_err[4] = 0.0120015;

  // fake in the endcap from QCD MC                                                                     
  m_fakeRateEE[0] = 0.208365;
  m_fakeRateEE[1] = 0.0621581;
  m_fakeRateEE[2] = 0.0546968; 
  m_fakeRateEE[3] = 0.0522816; 
  m_fakeRateEE[4] = 0.0515749;

  m_fakeRateEE_err[0] = 0.00714442;
  m_fakeRateEE_err[1] = 0.00411465;
  m_fakeRateEE_err[2] = 0.00314477;
  m_fakeRateEE_err[3] = 0.00299393;
  m_fakeRateEE_err[4] = 0.0142672;
  */

  /*    // chiara
  // fake in the barrel from data (jet:30)) - no EWK subtraction
  m_fakeRateEB[0] = 0.170909;
  m_fakeRateEB[1] = 0.127837; 
  m_fakeRateEB[2] = 0.0843061;
  m_fakeRateEB[3] = 0.0857687;
  m_fakeRateEB[4] = 0.116071; 

  m_fakeRateEB_err[0] = 0.00926705;
  m_fakeRateEB_err[1] = 0.00918355;
  m_fakeRateEB_err[2] = 0.00707559;
  m_fakeRateEB_err[3] = 0.00705814;
  m_fakeRateEB_err[4] = 0.0302665;

  // fake in the endcap from data (jet:30) - no EWK subtraction
  m_fakeRateEE[0] = 0.136997;
  m_fakeRateEE[1] = 0.0632603;
  m_fakeRateEE[2] = 0.0563645;
  m_fakeRateEE[3] = 0.045511;
  m_fakeRateEE[4] = 0.0300752;

  m_fakeRateEE_err[0] = 0.009566;
  m_fakeRateEE_err[1] = 0.00693256;
  m_fakeRateEE_err[2] = 0.00499824;
  m_fakeRateEE_err[3] = 0.00423941;
  m_fakeRateEE_err[4] = 0.0148097;
  */

  // fake in the barrel from data (jet:30)) - with EWK subtraction (apart from bin0, where no EWK removal)
  m_fakeRateEB[0] = 0.170909;
  m_fakeRateEB[1] = 0.126939;
  m_fakeRateEB[2] = 0.0829306;
  m_fakeRateEB[3] = 0.0804465;
  m_fakeRateEB[4] = 0.0993018;

  m_fakeRateEB_err[0] = 0.00926705;
  m_fakeRateEB_err[1] = 0.00916318;
  m_fakeRateEB_err[2] = 0.00703008;
  m_fakeRateEB_err[3] = 0.00687927;
  m_fakeRateEB_err[4] = 0.0285547;

  // fake in the endcap from data (jet:30) - with EWK subtraction (apart from bin0, where no EWK removal)
  m_fakeRateEE[0] = 0.136997;
  m_fakeRateEE[1] = 0.0628376;
  m_fakeRateEE[2] = 0.0559572;
  m_fakeRateEE[3] = 0.0441833;
  m_fakeRateEE[4] = 0.0255683;

  m_fakeRateEE_err[0] = 0.009566;
  m_fakeRateEE_err[1] = 0.00691408;
  m_fakeRateEE_err[2] = 0.00498304;
  m_fakeRateEE_err[3] = 0.00418404;
  m_fakeRateEE_err[4] = 0.01372;

  /*
  // fake in the barrel from data (jet:15)) - no EWK subtraction
  m_fakeRateEB[0] = 0.176334; 
  m_fakeRateEB[1] = 0.13883;
  m_fakeRateEB[2] = 0.105588;
  m_fakeRateEB[3] = 0.0916634;
  m_fakeRateEB[4] = 0.114754;
  
  m_fakeRateEB_err[0] = 0.00382773;
  m_fakeRateEB_err[1] = 0.00532204;
  m_fakeRateEB_err[2] = 0.00503719;
  m_fakeRateEB_err[3] = 0.00573555;
  m_fakeRateEB_err[4] = 0.028856;

  // fake in the endcap from data (jet:15)
  m_fakeRateEE[0] = 0.138302;
  m_fakeRateEE[1] = 0.0695556;
  m_fakeRateEE[2] = 0.0601607;
  m_fakeRateEE[3] = 0.0486044;
  m_fakeRateEE[4] = 0.0357143;

  m_fakeRateEE_err[0] = 0.00379404;
  m_fakeRateEE_err[1] = 0.00379232;
  m_fakeRateEE_err[2] = 0.00310862;
  m_fakeRateEE_err[3] = 0.00333565;
  m_fakeRateEE_err[4] = 0.0156841;
  */

  /*
  // fake in the barrel from data (jet:50))
  m_fakeRateEB[0] = 0.130682;
  m_fakeRateEB[1] = 0.101523;
  m_fakeRateEB[2] = 0.0920502;
  m_fakeRateEB[3] = 0.0910973;
  m_fakeRateEB[4] = 0.0804598;
  
  m_fakeRateEB_err[0] = 0.0254063;
  m_fakeRateEB_err[1] = 0.021518;
  m_fakeRateEB_err[2] = 0.0187001;
  m_fakeRateEB_err[3] = 0.013093;
  m_fakeRateEB_err[4] = 0.0291619;

  // fake in the endcap from data (jet:50)
  m_fakeRateEE[0] = 0.104575;
  m_fakeRateEE[1] = 0.0753425;
  m_fakeRateEE[2] = 0.0536913;
  m_fakeRateEE[3] = 0.0351438;
  m_fakeRateEE[4] = 0.0222222;

  m_fakeRateEE_err[0] = 0.0247391;
  m_fakeRateEE_err[1] = 0.0218441;
  m_fakeRateEE_err[2] = 0.0130575;
  m_fakeRateEE_err[3] = 0.00735984;
  m_fakeRateEE_err[4] = 0.0155379;
  */
}

void LeptonPlusFakeMLSelection_ME::Loop() {

  _verbose=false;
  if(fChain == 0) return;
  
  initialiseFakeRate();

  // kinematics reduced tree
  std::string reducedTreeNameME = _datasetName+"-datasetME.root";
  myOutTreeME = new RedHiggsTree(reducedTreeNameME.c_str());
  myOutTreeME->addMLVars();
  myOutTreeME->addLatinos();

  unsigned int lastLumi=0;
  unsigned int lastRun=0;

  PUWeight* fPUWeight = new PUWeight();

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;
    
    resetKinematicsStart();

    // weight for the PU observed in 2011 data
    float tmpWeight = 1.;
    if ( !_selectionME->getSwitch("isData") ) tmpWeight *= fPUWeight->GetWeight(nPU);  

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
    reloadTriggerMask();
    bool passedHLT[1];
    passedHLT[me] = hasPassedHLT(); 


    // -------------------------------------------------------------
    
    // get the best electrons and best muons ==> tu be used to select ALL the possible channels at the beginning only
    std::pair<int,int> thePreElectrons      = getBestElectronPair_acceptance();
    std::pair<int,int> thePreMuons          = getBestMuonPair_acceptance();
    std::pair<int,int> theBestAcceptMuonEle = getBestMuonElePair(_acceptEleAll,_acceptMuonsAll);
    thePreElectronME = theBestAcceptMuonEle.second;
    thePreMuonME     = theBestAcceptMuonEle.first;

    // reconstructed channel
    m_channel[me] = false;
    
    // at this level the SELECTED channel should have pT > 10 and > 20. So far, at least 2 leptons with pT >20 and 10 in the event
    if ( thePreElectronME > -1 && thePreMuonME > -1 ) {
      float thisMaxPt  = GetPt(pxMuon[thePreMuonME],pyMuon[thePreMuonME]);
      float thisMinPt  = GetPt(pxEle[thePreElectronME],pyEle[thePreElectronME]);
      if (thisMaxPt>20 && thisMinPt>15) m_channel[me] = true;    // fixme: hardcoded
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
    std::pair<int,int> theBestIdEle = getBestElectronPair_id(_acceptEleAll);   

    // isolation, for identified electrons
    std::pair<int,int> theBestIsolEle = getBestElectronPair_isol(_idEleAll); 

    // conversion rejection, for isolated electrons
    std::pair<int,int> theBestConvEle = getBestElectronPair_conv(_isolEleAll);     

    // transverse impact parameter, for electrons passing conversion rejection
    std::pair<int,int> theBestIpEle = getBestElectronPair_ip(_convEleAll);     

    // -------------------------------------------------------------
    // MM candidates: preparing vectors of candidates and selecting the two highest pT mu- and mu+ after each step - to check the 20 GeV cut after 

    // muID, for muons in acceptance
    std::pair<int,int> theBestIdMuon = getBestMuonPair_id(_acceptMuonsAll); 

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

    // muon is the real lepton - I check it exists
    theReal = theBestIpMuonEle.first;

    // then I consider all possible denominators as a fake 
    theFake = getBestDenominator(theReal);   


    
    // -------------------------------------------------------------
    // set of kinematics: : now I've all the final leptons 
    resetKinematics();
    
    // MET is an event variable. Independent o the channel
    m_p3PFMET->SetXYZ(pxPFMet[0],pyPFMet[0],pzPFMet[0]);
    m_p3TKMET->SetXYZ(pxChMetPV[0],pyChMetPV[0],pzChMetPV[0]); // the one associated to the 0th vertex
    m_theMET = m_p3PFMET->Pt();

    setKinematicsME(theReal, theFake);

    // weight with the Fake -> L2 probability                                                                                       
    float weight       = 1.;      
    float weightError  = 1.;
    float theFakePt    = GetPt(pxEle[theFake],pyEle[theFake]);
    bool  isFakeBarrel = false;
    if ( fabs(etaEle[theFake])<1.476 ) isFakeBarrel = true;
    if ( theFake>-1 ) {
      float fakerate    = getFakeRate( theFakePt, isFakeBarrel );
      float fakerateErr = getFakeRateError( theFakePt, isFakeBarrel );
      weight      = tmpWeight * fakerate / (1. - fakerate);
      weightError = tmpWeight * fakerateErr / ( (1. - fakerate)*(1. - fakerate) );
      // weight      = tmpWeight;   // chiara: only to study real leptons contamination
      // weightError = tmpWeight;   // chiara: only to study real leptons contamination
    } else {
      weight      = tmpWeight;
      weightError = tmpWeight;
    }

    // -------------------------------------------------------------    
    // look for PV in the event (there is always at least 1 PV)
    m_closestPV = getPV();    // fixme: si chiama closest ma e' quello a piu' alto pT. 
    
    int njets[1], nuncorrjets[1];
    float dphiLLJ[1], btag[1];
    int nsoftmu[1],nextraleptons[1];
    for(int ichan=0; ichan<1; ichan++) {

      // jet counter
      njets[ichan] = numJets(eleCands[ichan],muCands[ichan],ichan);
      nuncorrjets[ichan] = numUncorrJets(eleCands[ichan],muCands[ichan]);
      
      // if 1-jet bin, use deltaphi(ll-jet)
      dphiLLJ[ichan] = deltaPhiLLJet(ichan);   
      
      // b veto
      btag[ichan] = bVetoJets(eleCands[ichan],muCands[ichan]);
      
      // soft muon counter
      nsoftmu[ichan] = numSoftMuons(muCands[ichan]);
      
      // extra lepton counter
      nextraleptons[ichan] = numExtraLeptons(eleCands[ichan],muCands[ichan]);
    }


    // ---------------------------------------
    // ME
    CutBasedHiggsSelectionME.SetWeight(weight);               
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

    // checking if the highest pT lepton at the end has pT>20. MU is the hardest in ME
    float thisMaxPtIpME = GetPt(pxMuon[theReal],pyMuon[theReal]);
    if (thisMaxPtIpME<20)   { 
      CutBasedHiggsSelectionME.SetElectronIp(-1);
      CutBasedHiggsSelectionME.SetPositronIp(-1);
    }

    CutBasedHiggsSelectionME.SetHighElePt(hardestLeptonPt[me]); 
    CutBasedHiggsSelectionME.SetLowElePt(slowestLeptonPt[me]);  
    CutBasedHiggsSelectionME.SetExtraSlowLeptonPTCut(15.0); // enforce the min pT only on electrons

    CutBasedHiggsSelectionME.SetNJets(njets[me]);
    CutBasedHiggsSelectionME.SetNUncorrJets(nuncorrjets[me]);
    CutBasedHiggsSelectionME.SetBTagJets(btag[me]);
    CutBasedHiggsSelectionME.SetNSoftMuons(nsoftmu[me]);
    CutBasedHiggsSelectionME.SetNExtraLeptons(nextraleptons[me]);
    CutBasedHiggsSelectionME.SetMet(m_theMET);					
    CutBasedHiggsSelectionME.SetProjectedMet(m_projectedMet[me]);
    CutBasedHiggsSelectionME.SetMetOverPtLL(m_metOptll[me]);
    CutBasedHiggsSelectionME.SetDeltaPhiLLJet(dphiLLJ[me]);   
    CutBasedHiggsSelectionME.SetDeltaPhi(m_deltaPhi[me]);
    CutBasedHiggsSelectionME.SetInvMass(m_mll[me]);
    CutBasedHiggsSelectionME.SetDetaLeptons(m_deltaEtaLeptons[me]);
    CutBasedHiggsSelectionME.SetWWInvMass(2.*m_transvMass[me]/_massVal);

    bool isSelectedME           = CutBasedHiggsSelectionME.output();    
    bool selUpToFinalLeptonsME  = CutBasedHiggsSelectionME.outputUpToFinalLeptons();
    bool selUpToJetVetoME       = CutBasedHiggsSelectionME.outputUpToJetVeto();
    bool selUpToUncorrJetVetoME = CutBasedHiggsSelectionME.outputUpToUncorrJetVeto();
    bool selPreDeltaPhiME       = CutBasedHiggsSelectionME.outputPreDeltaPhi();

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


    // filling the tree
    myOutTreeME->fillRunInfos(runNumber, lumiBlock, eventNumber, weight);

    myOutTreeME -> fillAll(GetPt(pxTCMet[0],pyTCMet[0]), GetPt(pxPFMet[0],pyPFMet[0]), GetPt(pxMet[0],pyMet[0]), 
			   m_projectedMet[me], m_deltaPhi[me], m_deltaErre[me], m_transvMass[me], m_mll[me], 
			   hardestLeptonPt[me], slowestLeptonPt[me], hardestLeptonEta[me], slowestLeptonEta[me],
			   m_deltaEtaLeptons[me], nPV,
			   selUpToFinalLeptonsME, selUpToJetVetoME, selUpToUncorrJetVetoME, selPreDeltaPhiME, isSelectedME);

    myOutTreeME -> fillMLVars(njets[me], nuncorrjets[me], m_maxDxyEvt, m_maxDszEvt, m_maxTrackCountingHighEffBJetTags, m_maxImpactParameterMVABJetTags, m_maxCombinedSecondaryVertexMVABJetTags);
    
    myOutTreeME -> fillLatinos( outputStep0, outputStep1, outputStep2, outputStep3, outputStep4, outputStep5, outputStep6, outputStep7, outputStep8, outputStep9, outputStep10, outputStep11, outputStep12, outputStep13, outputStep14, outputStep15, outputStep16, outputStep17, outputStep18, outputStep19, outputStep20, outputStep21, outputStep22, outputStep23, outputStep24 );

    // dumping final tree, only if there are 2 leptons in the acceptance
    if(outputStep1) myOutTreeME -> store();
  
     // for errors                                                                                                                   
    CutBasedHiggsErrorsSelectionME.SetWeight(weightError);               
    CutBasedHiggsErrorsSelectionME.SetMcTruth(1);
    CutBasedHiggsErrorsSelectionME.SetHLT(passedHLT[me]);               
    CutBasedHiggsErrorsSelectionME.SetIsChannel(m_channel[me]);     
    
    CutBasedHiggsErrorsSelectionME.SetElectronId(theReal);
    CutBasedHiggsErrorsSelectionME.SetPositronId(theFake);
    CutBasedHiggsErrorsSelectionME.SetElectronIsolation(theReal);
    CutBasedHiggsErrorsSelectionME.SetPositronIsolation(theFake);
    CutBasedHiggsErrorsSelectionME.SetElectronConvRejection(theReal);
    CutBasedHiggsErrorsSelectionME.SetPositronConvRejection(theFake);
    CutBasedHiggsErrorsSelectionME.SetElectronIp(theReal);
    CutBasedHiggsErrorsSelectionME.SetPositronIp(theFake);

    // checking if the highest pT lepton at the end has pT>20. MU is the hardest in ME
    thisMaxPtIpME = GetPt(pxMuon[theReal],pyMuon[theReal]);
    if (thisMaxPtIpME<20)   { 
      CutBasedHiggsErrorsSelectionME.SetElectronIp(-1);
      CutBasedHiggsErrorsSelectionME.SetPositronIp(-1);
    }

    CutBasedHiggsErrorsSelectionME.SetHighElePt(hardestLeptonPt[me]); 
    CutBasedHiggsErrorsSelectionME.SetLowElePt(slowestLeptonPt[me]);  
    CutBasedHiggsErrorsSelectionME.SetExtraSlowLeptonPTCut(15.0); // enforce the min pT only on electrons

    CutBasedHiggsErrorsSelectionME.SetNJets(njets[me]);
    CutBasedHiggsErrorsSelectionME.SetNUncorrJets(nuncorrjets[me]);
    CutBasedHiggsErrorsSelectionME.SetBTagJets(btag[me]);
    CutBasedHiggsErrorsSelectionME.SetNSoftMuons(nsoftmu[me]);
    CutBasedHiggsErrorsSelectionME.SetNExtraLeptons(nextraleptons[me]);
    CutBasedHiggsErrorsSelectionME.SetMet(m_theMET);					
    CutBasedHiggsErrorsSelectionME.SetProjectedMet(m_projectedMet[me]);
    CutBasedHiggsErrorsSelectionME.SetMetOverPtLL(m_metOptll[me]);
    CutBasedHiggsErrorsSelectionME.SetDeltaPhiLLJet(dphiLLJ[me]);   
    CutBasedHiggsErrorsSelectionME.SetDeltaPhi(m_deltaPhi[me]);
    CutBasedHiggsErrorsSelectionME.SetInvMass(m_mll[me]);
    CutBasedHiggsErrorsSelectionME.SetDetaLeptons(m_deltaEtaLeptons[me]);
    CutBasedHiggsErrorsSelectionME.SetWWInvMass(2.*m_transvMass[me]/_massVal);
    bool isSelectedErrorME = CutBasedHiggsErrorsSelectionME.output();
  }
}

void LeptonPlusFakeMLSelection_ME::displayEfficiencies(std::string datasetName) {

  std::string::size_type loc = datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    datasetName.erase(loc);
  }
  
  std::cout << "--------------------------------" << std::endl;
  std::cout << "=== RATE ESTIMATED FROM FAKE RATE FOR ME SELECTION ===: " << std::endl;
  CutBasedHiggsSelectionME.displayEfficiencies(datasetName);

  std::cout << "=== RATE UNCERTAINTY ESTIMATED FROM FAKE RATE FOR ME SELECTION ===" << std::endl;
  CutBasedHiggsErrorsSelectionME.displayEfficiencies(datasetName);

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

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestElectronPair_acceptance() {
  
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
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }

    _acceptEleAll.push_back(i);   
  }
  _acceptEleAll = sortElectronsByPt(_acceptEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestElectronPair_id( std::vector<int> acceptEle ) {

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
    if (!_selectionME->getSwitch("asymmetricID")) isEleID(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);
    if ( _selectionME->getSwitch("asymmetricID")) {
      if (thisPt>=20) isEleID(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);
      if (thisPt<20)  isEleID(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow);
    }

    if (!theElectronID) continue;

    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }

    _idEleAll.push_back(thisEle);  
  }
  _idEleAll = sortElectronsByPt(_idEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestElectronPair_isol( std::vector<int> idEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _isolEleAll.clear();

  for (int iEle=0; iEle<idEle.size(); iEle++) {
    int thisEle = idEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleID(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);

    if (!theElectronIsol) continue;
    
    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }

    _isolEleAll.push_back(thisEle);  
  }
  _isolEleAll = sortElectronsByPt(_isolEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestElectronPair_conv( std::vector<int> isolEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
  
  _convEleAll.clear();

  for (int iEle=0; iEle<isolEle.size(); iEle++) {
    int thisEle = isolEle[iEle];
    
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    
    isEleID(thisEle,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);
    
    if (!theElectronConvRej) continue;

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }

    _convEleAll.push_back(thisEle);      
  }
  _convEleAll = sortElectronsByPt(_convEleAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestElectronPair_ip( std::vector<int> convEle ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _ipEleAll.clear();

  for (int iEle=0; iEle<convEle.size(); iEle++) {
    int thisEle = convEle[iEle];

    int gsfTrack = gsfTrackIndexEle[thisEle]; 
    float d3dEle = impactPar3DGsfTrack[gsfTrack];
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) continue;   

    float thisPt     = GetPt(pxEle[thisEle],pyEle[thisEle]);
    float thisCharge = chargeEle[thisEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisEle; }

    _ipEleAll.push_back(thisEle);  
  }
  _ipEleAll = sortElectronsByPt(_ipEleAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestMuonPair_acceptance() {
  
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
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
    
    _acceptMuonsAll.push_back(i);  
  }
  _acceptMuonsAll = sortMuonsByPt(_acceptMuonsAll);

  return make_pair(theLep1,theLep2);
}


std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestMuonPair_id( std::vector<int> acceptMu ) {
  
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
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }
    
    _idMuonsAll.push_back(thisMu);   
  }
  _idMuonsAll = sortMuonsByPt(_idMuonsAll);
  
  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestMuonPair_isol( std::vector<int> idMu ) {
  
  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;
   
  _isolMuonsAll.clear();

  for(int iMu=0; iMu<idMu.size(); iMu++) {

    int thisMu   = idMu[iMu];
    float thisPt = GetPt(pxMuon[thisMu],pyMuon[thisMu]);

    // fixme: diverso da prima: rimuovevo il secondo leptone....
    float muonTrackerForGlobal = sumPt03Muon[thisMu];
    float muonEcalForGlobal    = emEt03Muon[thisMu];
    float muonHcalForGlobal    = hadEt03Muon[thisMu]; 
    float theMuonGlobalSum     = muonTrackerForGlobal + muonEcalForGlobal + muonHcalForGlobal - rhoFastjet*TMath::Pi()*0.3*0.3;
    float theRelMuonIso        = theMuonGlobalSum/thisPt; 
    if(_selectionME->getSwitch("muGlobalIso") && !_selectionME->passCut("muGlobalIso",theRelMuonIso)) continue;  

    float thisCharge = chargeMuon[thisMu];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }

    _isolMuonsAll.push_back(thisMu);   
  }
  _isolMuonsAll = sortMuonsByPt(_isolMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestMuonPair_ip( std::vector<int> isoMu ) {

  int theLep1 = -1;
  int theLep2 = -1;
  float maxPtLep1 = -1000.;
  float maxPtLep2 = -1000.;

  _ipMuonsAll.clear();  

  for(int iMu=0; iMu<isoMu.size(); iMu++) {

    int thisMu = isoMu[iMu];
    
    int ctfMuon   = trackIndexMuon[thisMu]; 
    float dxyMuon = transvImpactParTrack[ctfMuon];
    float dzMuon  = PVzPV[m_closestPV] - trackVzTrack[ctfMuon];   
    if (_selectionME->getSwitch("muonIP") && (!_selectionME->passCut("muonIP",dxyMuon)) ) continue;   
    if (_selectionME->getSwitch("muonDz") && (!_selectionME->passCut("muonDz",dzMuon)) )  continue;   

    float thisPt     = GetPt(pxMuon[thisMu],pyMuon[thisMu]);
    float thisCharge = chargeMuon[thisMu];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = thisMu; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = thisMu; }

    _ipMuonsAll.push_back(thisMu);   
  }
  _ipMuonsAll = sortMuonsByPt(_ipMuonsAll);

  return make_pair(theLep1,theLep2);
}

std::pair<int,int> LeptonPlusFakeMLSelection_ME::getBestMuonElePair(std::vector<int> electrons, std::vector<int> muons) {

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
      if(eleCharge*muCharge<0 && muPt > elePt) return std::make_pair(*muiter,*eleiter); 
    }
  }
  return std::make_pair(theMuon,theEle); 
}

int LeptonPlusFakeMLSelection_ME::getBestDenominator(int realMuon) {
  
  int theFake=-1;

  float maxPtFake=-1000.;

  float theRealPt = GetPt(pxMuon[realMuon],pyMuon[realMuon]);    

  for(int iele=0; iele<nEle; iele++) {
    
    float thisElePt = GetPt(pxEle[iele],pyEle[iele]);    

    if (chargeEle[iele]*chargeMuon[realMuon]>0) continue;

    if (thisElePt>theRealPt) continue;

    bool isGoodDenom = isDenomFake(iele);
    if (!isGoodDenom) continue;
    
    // removing candidates passing the tight selection
    bool isTight = true;
    bool theElectronID, theElectronIsol, theElectronConvRej;
    theElectronID = theElectronIsol = theElectronConvRej = true;
    if (!_selectionME->getSwitch("asymmetricID")) isEleID(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);
    if ( _selectionME->getSwitch("asymmetricID")) {
      if (thisElePt>=20) isEleID(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedID);
      if (thisElePt<20)  isEleID(iele,&theElectronID,&theElectronIsol,&theElectronConvRej,&EgammaCutBasedIDLow);
    }
    if (!theElectronID)      isTight = false;
    if (!theElectronIsol)    isTight = false; 
    if (!theElectronConvRej) isTight = false;
    int gsfTrack = gsfTrackIndexEle[iele]; 
    float d3dEle = impactPar3DGsfTrack[gsfTrack];
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) isTight = false;
    if (isTight) continue;

    if( thisElePt > maxPtFake ) { maxPtFake = thisElePt; theFake=iele; }
  }
  
  return theFake;
}

bool LeptonPlusFakeMLSelection_ME::isDenomFake(int theEle) {

  Utils anaUtils;
  bool isGoodDenom = true;
  TVector3 p3Ele(pxEle[theEle], pyEle[theEle], pzEle[theEle]);
  
  // acceptance	   
  if( fabs(p3Ele.Eta()) > 2.5 ) isGoodDenom = false;
  if( p3Ele.Pt() < 10. )        isGoodDenom = false;

  // only ecal driven                                                                                                               
  bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[theEle], bits::isEcalDriven);
  if(!ecalDriven) isGoodDenom = false;
  
  // barrel or endcap                                                                                                               
  bool isEleEB = anaUtils.fiducialFlagECAL(fiducialFlagsEle[theEle], isEB);
  
  // isolation                                                                                                                      
  float combinedIso;
  if (isEleEB)  combinedIso = dr03TkSumPtEle[theEle] + TMath::Max(0.0,dr03EcalRecHitSumEtEle[theEle]-1.0) + dr03HcalTowerSumEtFullConeEle[theEle];
  if (!isEleEB) combinedIso = dr03TkSumPtEle[theEle] + dr03EcalRecHitSumEtEle[theEle] + dr03HcalTowerSumEtFullConeEle[theEle];
  float corrCombinedIso = (combinedIso - rhoFastjet*TMath::Pi()*0.3*0.3) / p3Ele.Pt();
  if ( corrCombinedIso>0.15 ) isGoodDenom = false;
  
  // H/E                                                                                                                                        
  bool isBarrelEle;
  if ( fabs(etaEle[theEle]) <  1.479 ) isBarrelEle = true;
  if ( fabs(etaEle[theEle]) >= 1.479 ) isBarrelEle = false;
  if ( isBarrelEle && hOverEEle[theEle]>0.15) isGoodDenom = false;
  if (!isBarrelEle && hOverEEle[theEle]>0.10) isGoodDenom = false;
  
  // sigmaIetaIeta                                                                                                                
  bool isBarrelSc;
  int sc = superClusterIndexEle[theEle];
  if ( sc < 0 ) isGoodDenom = false;
  if ( fabs(etaSC[sc]) <  1.479 ) isBarrelSc = true;
  if ( fabs(etaSC[sc]) >= 1.479 ) isBarrelSc = false;
  if ( isBarrelSc && sqrt(covIEtaIEtaSC[sc])>0.014 ) isGoodDenom = false;
  if (!isBarrelSc && sqrt(covIEtaIEtaSC[sc])>0.035 ) isGoodDenom = false;
  
  // spikes                                                                                                                     
  float theE1 = eMaxSC[sc];
  float theE4SwissCross = e4SwissCrossSC[sc];
  float theSpikeSC = 1.0 - (theE4SwissCross/theE1);
  if (theSpikeSC>0.95) isGoodDenom = false;
  
  return isGoodDenom;
}

float LeptonPlusFakeMLSelection_ME::getFakeRate( float fakePt, bool isFakeBarrel ) {

  for (int theBin = 0; theBin<7; theBin++) {
    
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {
      if (isFakeBarrel)  return m_fakeRateEB[theBin];
      if (!isFakeBarrel) return m_fakeRateEE[theBin];
    }
  }
  
  std::cout << "BIG ERROR: fakePt = " << fakePt << endl;
  return -1.;
}

float LeptonPlusFakeMLSelection_ME::getFakeRateError( float fakePt, bool isFakeBarrel ) {
  
  for (int theBin = 0; theBin < 7; theBin++) {
    if( fakePt >= m_minFakePt[theBin] && fakePt < m_maxFakePt[theBin] ) {
      if (isFakeBarrel)  return m_fakeRateEB_err[theBin];
      if (!isFakeBarrel) return m_fakeRateEE_err[theBin];
    }
  }
  
  return -1.;
}

void LeptonPlusFakeMLSelection_ME::setKinematicsME(int myReal, int myFake) {

  if (myFake > -1 && myReal > -1) {

    eleCands[me].push_back(myFake);
    muCands[me] .push_back(myReal);
    m_p4LeptonMinus[me] -> SetXYZT(pxEle[myFake],pyEle[myFake],pzEle[myFake],energyEle[myFake]);
    m_p4LeptonPlus[me]  -> SetXYZT(pxMuon[myReal],pyMuon[myReal],pzMuon[myReal],energyMuon[myReal]);
    hardestLeptonPt[me] = GetPt(pxMuon[myReal],pyMuon[myReal]);
    slowestLeptonPt[me] = GetPt(pxEle[myFake], pyEle[myFake]);

    if ( hardestLeptonPt[me] != GetPt(pxMuon[myReal],pyMuon[myReal]) ) {
      cout << "this should not happen!!" << endl;
      cout << "myReal = " << myReal << ", myFake = " << myFake << endl;
      cout << "hardest PT = " << hardestLeptonPt[me] << endl;
      cout << "slowest PT = " << slowestLeptonPt[me] << endl;
      cout << "real PT = "    << GetPt(pxMuon[myReal],pyMuon[myReal]) << endl;
      cout << "fake PT = "    << GetPt(pxEle[myFake],pyEle[myFake]) << endl;
    }

    hardestLeptonEta[me] = etaMuon[myReal];
    slowestLeptonEta[me] = etaEle[myFake];
    m_mll[me]           = (*(m_p4LeptonMinus[me]) + *(m_p4LeptonPlus[me])).M();
    m_deltaPhi[me]      = fabs(180./TMath::Pi() * m_p4LeptonMinus[me]->Vect().DeltaPhi(m_p4LeptonPlus[me]->Vect()));
    m_deltaErre[me]     = m_p4LeptonMinus[me]->Vect().DeltaR(m_p4LeptonPlus[me]->Vect());
    m_deltaEtaLeptons[me] = etaEle[myFake]-etaEle[myReal];
    m_dilepPt[me].SetXYZ( m_p4LeptonMinus[me]->Vect().X()+m_p4LeptonPlus[me]->Vect().X(),m_p4LeptonMinus[me]->Vect().Y()+m_p4LeptonPlus[me]->Vect().Y(),0.0 );
    m_transvMass[me]    = CalcGammaMRstar(*m_p4LeptonMinus[me],*m_p4LeptonPlus[me]);
    m_metOptll[me]      = m_theMET / m_dilepPt[me].Pt();
    m_mT2[me]           = 0.;
    m_projectedMet[me]  = GetProjectedMet(m_p4LeptonMinus[me]->Vect(),m_p4LeptonPlus[me]->Vect());
  }  
}

void LeptonPlusFakeMLSelection_ME::resetKinematicsStart() {

  thePreElectronME = -1;
  thePreMuonME     = -1;
  theReal          = -1;
  theFake          = -1;
}

void LeptonPlusFakeMLSelection_ME::resetKinematics() {

  for(int theChannel=0; theChannel<1; theChannel++) {
    eleCands[theChannel].clear();
    muCands[theChannel].clear();
    m_p4LeptonMinus[theChannel] -> SetXYZT(0,0,0,0);                                                        
    m_p4LeptonPlus[theChannel]  -> SetXYZT(0,0,0,0);
    m_p3PFMET                   -> SetXYZ(0,0,0);
    m_p3TKMET                   -> SetXYZ(0,0,0);
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
  }
}

void LeptonPlusFakeMLSelection_ME::isEleID(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput, CutBasedEleIDSelector *thisCutBasedID) {
  
  *eleIdOutput = *isolOutput = *convRejOutput = false;

  Utils anaUtils;
  int gsf = gsfTrackIndexEle[eleIndex];
  float pt = GetPt(pxEle[eleIndex],pyEle[eleIndex]);

  // if is ECAL driven, take the electron ID variables from the standard electron
  // above all, take the ECAL supercluster instead of PF super cluster
  float HoE, s9s25, deta, dphiin, dphiout, fbrem, see, spp, eopout, eop;
  float e1, e4SwissCross, fidFlagSC, seedRecHitFlag, seedTime, seedChi2;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[eleIndex], isEcalDriven);
  HoE = hOverEEle[eleIndex];
  deta = deltaEtaAtVtxEle[eleIndex];
  dphiin = deltaPhiAtVtxEle[eleIndex];
  dphiout = deltaPhiAtCaloEle[eleIndex];
  fbrem = fbremEle[eleIndex];
  eopout = eSeedOverPoutEle[eleIndex];
  eop = eSuperClusterOverPEle[eleIndex];
  if(ecaldriven) {
    int sc = superClusterIndexEle[eleIndex];
    s9s25 = e3x3SC[sc]/e5x5SC[sc];
    see = sqrt(covIEtaIEtaSC[sc]);
    spp = sqrt(covIPhiIPhiSC[sc]);
    e1 = eMaxSC[sc];
    e4SwissCross = e4SwissCrossSC[sc];
    fidFlagSC = fiducialFlagsEle[eleIndex];
    seedRecHitFlag = recoFlagSC[sc];
    seedTime = timeSC[sc];
    seedChi2 = chi2SC[sc];
  } else {
    int sc = PFsuperClusterIndexEle[eleIndex];
    if(sc>-1) {
      s9s25 = e3x3PFSC[sc]/e5x5PFSC[sc];
      see = sqrt(covIEtaIEtaPFSC[sc]);
      spp = sqrt(covIPhiIPhiPFSC[sc]);
      e1 = eMaxSC[sc];
      e4SwissCross = e4SwissCrossSC[sc];
      fidFlagSC = fiducialFlagsEle[eleIndex];
      seedRecHitFlag = recoFlagSC[sc];
      seedTime = timeSC[sc];
      seedChi2 = chi2SC[sc];
    } else {
      s9s25 = 999.;
      see = 999.;
      spp = 999.;
    }
  }


  thisCutBasedID->SetEcalFiducialRegion( fiducialFlagsEle[eleIndex] );
  thisCutBasedID->SetRecoFlag(recoFlagsEle[eleIndex]);
  thisCutBasedID->applyElectronIDOnPFlowElectrons(true);
  thisCutBasedID->SetHOverE( HoE );
  thisCutBasedID->SetS9S25( s9s25 );
  thisCutBasedID->SetDEta( deta );
  thisCutBasedID->SetDPhiIn( dphiin );
  thisCutBasedID->SetDPhiOut( dphiout );
  thisCutBasedID->SetBremFraction( fbrem );
  thisCutBasedID->SetSigmaEtaEta( see );
  thisCutBasedID->SetSigmaPhiPhi( spp );
  thisCutBasedID->SetEOverPout( eopout );
  thisCutBasedID->SetEOverPin( eop );
  thisCutBasedID->SetElectronClass ( classificationEle[eleIndex] );
  thisCutBasedID->SetEgammaCutBasedID ( anaUtils.electronIdVal(eleIdCutsEle[eleIndex],eleIdLoose) );
  //  thisCutBasedID->SetLikelihood( likelihoodRatio(eleIndex,*LH) );
  thisCutBasedID->SetLikelihood( eleIdLikelihoodEle[eleIndex] );
  thisCutBasedID->SetNBrem( nbremsEle[eleIndex] );
  thisCutBasedID->SetEcalIsolation( (dr03EcalRecHitSumEtEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );                
  thisCutBasedID->SetTrkIsolation ( (dr03TkSumPtEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );                        
  thisCutBasedID->SetHcalIsolation( (dr03HcalTowerSumEtFullConeEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );         
  float iso = 0.0;
  if ( anaUtils.fiducialFlagECAL(fiducialFlagsEle[eleIndex],isEB) ) iso = dr03TkSumPtEle[eleIndex] + max(0.0,dr03EcalRecHitSumEtEle[eleIndex]-1.0) + dr03HcalTowerSumEtFullConeEle[eleIndex];
  else iso = dr03TkSumPtEle[eleIndex] + dr03EcalRecHitSumEtEle[eleIndex] + dr03HcalTowerSumEtFullConeEle[eleIndex];
  thisCutBasedID->SetCombinedIsolation( (iso - rhoFastjet*TMath::Pi()*0.3*0.3) / pt );
  thisCutBasedID->SetMissingHits( expInnerLayersGsfTrack[gsf] );
  thisCutBasedID->SetConvDist( fabs(convDistEle[eleIndex]) );
  thisCutBasedID->SetConvDcot( fabs(convDcotEle[eleIndex]) );

  // ECAL cleaning variables
  thisCutBasedID->m_cleaner->SetE1(e1);
  thisCutBasedID->m_cleaner->SetE4SwissCross(e4SwissCross);
  thisCutBasedID->m_cleaner->SetFiducialFlag(fidFlagSC);
  thisCutBasedID->m_cleaner->SetSeedFlag(seedRecHitFlag);
  thisCutBasedID->m_cleaner->SetSeedTime(seedTime);
  thisCutBasedID->m_cleaner->SetSeedChi2(seedChi2);

  //  return egammaCutBasedID.output(); // class dependent result
  *eleIdOutput = thisCutBasedID->outputNoClassEleId();
  *isolOutput = thisCutBasedID->outputNoClassIso();
  *convRejOutput = thisCutBasedID->outputNoClassConv();
}

void LeptonPlusFakeMLSelection_ME::isMuonID(int muonIndex, bool *muonIdOutput) {

  *muonIdOutput = true;

  Utils anaUtils; 
  bool flagGlobalMu = false;
  if(anaUtils.muonIdVal(muonIdMuon[muonIndex],AllGlobalMuons)) {
    int globalMuonTrack = combinedTrackIndexMuon[muonIndex];
    if(trackNormalizedChi2GlobalMuonTrack[globalMuonTrack] < 10 && 
       trackValidHitsGlobalMuonTrack[globalMuonTrack] > 0 &&
       numberOfMatchesMuon[muonIndex] > 1 ) flagGlobalMu = true; // to be used when new trees are available
  }

  bool flagTrackerMu = false;
  if( (anaUtils.muonIdVal(muonIdMuon[muonIndex],AllTrackerMuons) &&
       anaUtils.muonIdVal(muonIdMuon[muonIndex],TMLastStationTight)) ) flagTrackerMu  = true;

  if(!(flagGlobalMu || flagTrackerMu)) {
    *muonIdOutput = false;
    return;
  }
    
  int track = trackIndexMuon[muonIndex];

  if(trackValidHitsTrack[track]<=10) *muonIdOutput = false;

  if( (numberOfValidPixelBarrelHitsTrack[track]+numberOfValidPixelEndcapHitsTrack[track])<1 ) *muonIdOutput = false; 

  float ptTrack = sqrt( pxTrack[track]*pxTrack[track] + pyTrack[track]*pyTrack[track] );
  float sign = fabs(ptErrorTrack[track]/ptTrack);
  if (sign>=0.1) *muonIdOutput = false;
}


int LeptonPlusFakeMLSelection_ME::numJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove, int theChannel) {

  int num=0;
  m_goodJets.clear();
  float ETMax=0.;

  theLeadingJet[theChannel]=-1;   

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);

    if(_selectionME->getSwitch("etaJetAcc") && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;

    float pt = GetPt(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j]);
    if(_selectionME->getSwitch("etJetAcc") && !_selectionME->passCut("etJetAcc", pt)) continue;

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    
    if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
                  chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

    bool foundMatch = false;

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
    if(foundMatch) continue;

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

    m_goodJets.push_back(j);
    num++;
    
    if(pt>ETMax) {
      theLeadingJet[theChannel] = j;
      ETMax = pt;
    }

  }

  return num;
}


int LeptonPlusFakeMLSelection_ME::numUncorrJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove ) {

  int num=0;

  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    float uncorrEt = uncorrEnergyAK5PFPUcorrJet[j]*fabs(sin(thetaAK5PFPUcorrJet[j]));
    TLorentzVector p4Jet;
    p4Jet.SetPtEtaPhiE(uncorrEt,etaAK5PFPUcorrJet[j],phiAK5PFPUcorrJet[j],uncorrEnergyAK5PFPUcorrJet[j]);
    TVector3 p3Jet = p4Jet.Vect();

    if(_selectionME->getSwitch("etaJetAcc")      && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;    
    if(_selectionME->getSwitch("etUncorrJetAcc") && !_selectionME->passCut("etUncorrJetAcc", uncorrEt))   continue;

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    
    if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
                  chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

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
    
    num++;
  }
  
  return num;
}

float LeptonPlusFakeMLSelection_ME::bVetoJets( std::vector<int> eleToRemove, std::vector<int> muonToRemove ) {

  float output=-999;
  for(int j=0;j<nAK5PFPUcorrJet;j++) {

    TVector3 p3Jet(pxAK5PFPUcorrJet[j],pyAK5PFPUcorrJet[j],pzAK5PFPUcorrJet[j]);

    if(_selectionME->getSwitch("etaJetAcc") && !_selectionME->passCut("etaJetAcc", fabs(etaAK5PFPUcorrJet[j]))) continue;

    // PF jet ID variables
    float neutralHadFrac = neutralHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    float neutralEmFraction = neutralEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int nConstituents = chargedHadronMultiplicityAK5PFPUcorrJet[j] + neutralHadronMultiplicityAK5PFPUcorrJet[j] +
      photonMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j] +
      HFHadronMultiplicityAK5PFPUcorrJet[j] + HFEMMultiplicityAK5PFPUcorrJet[j];
    float chargedHadFraction = chargedHadronEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    int chargedMultiplicity = chargedHadronMultiplicityAK5PFPUcorrJet[j] + electronMultiplicityAK5PFPUcorrJet[j] + muonMultiplicityAK5PFPUcorrJet[j];
    float chargedEmFraction = chargedEmEnergyAK5PFPUcorrJet[j]/energyAK5PFPUcorrJet[j];
    
    if(!isPFJetID(fabs(etaAK5PFPUcorrJet[j]),neutralHadFrac,neutralEmFraction,nConstituents,
                  chargedHadFraction,chargedMultiplicity,chargedEmFraction, Higgs::loose)) continue;

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
    
  }

  return output;

}

float LeptonPlusFakeMLSelection_ME::deltaPhiLLJet(int ichan) {   
  
  int myLeadingJet = theLeadingJet[ichan];

  if(myLeadingJet > -1) {
    TVector3 leadingJetP3(pxAK5PFPUcorrJet[myLeadingJet],pyAK5PFPUcorrJet[myLeadingJet],pzAK5PFPUcorrJet[myLeadingJet]);    
    return fabs(180./TMath::Pi() * leadingJetP3.DeltaPhi(m_dilepPt[ichan]));                           
  } else return -999.;
}

int LeptonPlusFakeMLSelection_ME::numSoftMuons(std::vector<int> muonToRemove) {

  int num = 0;
  for(int i=0; i<nMuon; ++i) {

    bool isSelMuon=false;
    for(int muSel=0; muSel<(int)muonToRemove.size(); muSel++) { 
      if(i==muonToRemove[muSel]) isSelMuon=true;
    }
    if(isSelMuon) continue;

    float pt = GetPt(pxMuon[i],pyMuon[i]);
    if(pt < 3.0) continue;

    Utils anaUtils;
    if(!anaUtils.muonIdVal(muonIdMuon[i],AllTrackerMuons) || !anaUtils.muonIdVal(muonIdMuon[i],TMLastStationTight)) continue;
       
    int track = trackIndexMuon[i];
    if(trackValidHitsTrack[track]<=10) continue;

    float dxy = transvImpactParTrack[track];
    if(dxy > 0.100) continue;   

    float isoSumAbs = sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i] - rhoFastjet*TMath::Pi()*0.3*0.3;
    float isoSumRel = isoSumAbs / pt;
    if(pt>20 || isoSumRel<0.1) continue;
    
    num++;
  }
  return num;
}

int LeptonPlusFakeMLSelection_ME::numExtraLeptons( std::vector<int> eleToRemove, std::vector<int> muonToRemove  ) {

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
      isEleID(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID);
    if (_selectionME->getSwitch("asymmetricID")) {
      float pt = GetPt(pxEle[i],pyEle[i]);	
      if(pt>=20) isEleID(i,&theId,&theIso,&theConvRej,&EgammaCutBasedID);
      if(pt<20)  isEleID(i,&theId,&theIso,&theConvRej,&EgammaCutBasedIDLow);
    }
    if(!theId || !theIso || !theConvRej) continue;

    int track = gsfTrackIndexEle[i];
    float d3dEle = impactPar3DGsfTrack[track];
    if (_selectionME->getSwitch("electronIP") && (!_selectionME->passCut("electronIP",d3dEle)) ) continue;    

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
    float isoSumAbs = sumPt03Muon[i] + emEt03Muon[i] + hadEt03Muon[i] - rhoFastjet*TMath::Pi()*0.3*0.3;
    float isoSumRel = isoSumAbs / ptMu;
    if(_selectionME->getSwitch("muGlobalIso") && !_selectionME->passCut("muGlobalIso",isoSumRel)) continue;

    int track = trackIndexMuon[i];
    float dxy = transvImpactParTrack[track];
    float dz  = PVzPV[m_closestPV] - trackVzTrack[track];  
    if(_selectionME->getSwitch("muonIP") && !_selectionME->passCut("muonIP",dxy)) continue;
    if(_selectionME->getSwitch("muonDz") && !_selectionME->passCut("muonDz",dz))  continue;  

    numMu++;
  }
  
  return numEle + numMu;
}

int LeptonPlusFakeMLSelection_ME::getPV() {
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

float LeptonPlusFakeMLSelection_ME::GetProjectedMet(TVector3 p1, TVector3 p2) {

  // calculate with PF met
  float projMET_pf = 0.0;
  float deltaPhi1_pf = fabs(p1.DeltaPhi(*m_p3PFMET));
  float deltaPhi2_pf = fabs(p2.DeltaPhi(*m_p3PFMET));
  float deltaphi_pf = TMath::Min(deltaPhi1_pf,deltaPhi2_pf);
  if(deltaphi_pf<TMath::Pi()/2.) projMET_pf = m_p3PFMET->Mag() * sin(deltaphi_pf);
  else projMET_pf = m_p3PFMET->Mag();

  // calculate with TKMET
  float projMET_tk = 0.0;
  float deltaPhi1_tk = fabs(p1.DeltaPhi(*m_p3TKMET));
  float deltaPhi2_tk = fabs(p2.DeltaPhi(*m_p3TKMET));
  float deltaphi_tk = TMath::Min(deltaPhi1_tk,deltaPhi2_tk);
  if(deltaphi_tk<TMath::Pi()/2.) projMET_tk = m_p3TKMET->Mag() * sin(deltaphi_tk);
  else projMET_tk = m_p3TKMET->Mag();

  return TMath::Min(projMET_pf,projMET_tk);

}


/// specific for HWW that has multiple channels with different HLT requirements
bool LeptonPlusFakeMLSelection_ME::reloadTriggerMask()
{
  std::vector<int> triggerMask;

  // load the triggers required for ME
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=requiredTriggersME.begin();fIter!=requiredTriggersME.end();++fIter) {
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(*fIter) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_requiredTriggersME = triggerMask;

  // load the triggers NOT required for ME
  triggerMask.clear();
  for (std::vector< std::string >::const_iterator fIter=notRequiredTriggersME.begin();fIter!=notRequiredTriggersME.end();++fIter) {
    for(unsigned int i=0; i<nameHLT->size(); i++) {
      if(nameHLT->at(i).find(*fIter) != string::npos) {
	triggerMask.push_back( indexHLT[i] ) ;
	break;
      }
    }
  }
  m_notRequiredTriggersME = triggerMask;
}

bool LeptonPlusFakeMLSelection_ME::hasPassedHLT() {

  Utils anaUtils;
  bool required    = anaUtils.getTriggersOR(m_requiredTriggersME, firedTrg);
  bool notRequired = anaUtils.getTriggersOR(m_notRequiredTriggersME, firedTrg);
  return (required && !notRequired);
}

void LeptonPlusFakeMLSelection_ME::setRequiredTriggers(const std::vector<std::string>& reqTriggers) {
  requiredTriggersME=reqTriggers;
}

void LeptonPlusFakeMLSelection_ME::setNotRequiredTriggers(const std::vector<std::string>& reqTriggers) {
  notRequiredTriggersME=reqTriggers;
}

