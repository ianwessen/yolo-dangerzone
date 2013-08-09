#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/kFactorEvaluator.hh"
#include "HiggsAnalysisTools/include/LeptonPlusFakeSelection.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"

#include <iostream>
#include <fstream>
#include <string>

#include <TTree.h>

using namespace bits;

LeptonPlusFakeSelection::LeptonPlusFakeSelection(TTree *tree) 
  : HiggsBase(tree) {

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
  LFakeFromQCDSelector.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 
  _selectionEE = LFakeFromQCDSelector.GetSelection();  

  LFakeErrorFromQCDSelector.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 

  LFakeFromWjetsSelector.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 
  LFakeErrorFromWjetsSelector.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 

  // single electron efficiency
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/looseEleId/"); 
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/tightEleId/"); 
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/hwwAnEleId/");
  EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/newOptimEleId_looseOthers_m160/");
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/newOptimEleId_tightOthers_m160/");
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/newOptimEleId_looseOthers_m190/");
  // EgammaCutBasedID.Configure("../EgammaAnalysisTools/config/newOptimEleId_tightOthers_m190/");

  // create histograms
  m_histoLL_mll = new TH1F("m_histoLL_mll","m_histoLL_mll",15,0,300);
  m_histoLL_ptmax = new TH1F("m_histoLL_ptmax","m_histoLL_ptmax",20,0,200);
  m_histoLL_ptmin = new TH1F("m_histoLL_ptmin","m_histoLL_ptmin",20,0,200);
  m_histoLL_met = new TH1F("m_histoLL_met","m_histoLL_met",15,0,200);
  m_histoLL_deltaphi = new TH1F("m_histoLL_deltaphi","m_histoLL_deltaphi",10,0,180);

  m_histoLD_mll = new TH1F("m_histoLD_mll","m_histoLD_mll",15,0,300);
  m_histoLD_ptmax = new TH1F("m_histoLD_ptmax","m_histoLD_ptmax",20,0,200);
  m_histoLD_ptmin = new TH1F("m_histoLD_ptmin","m_histoLD_ptmin",20,0,200);
  m_histoLD_met = new TH1F("m_histoLD_met","m_histoLD_met",15,0,200);
  m_histoLD_deltaphi = new TH1F("m_histoLD_deltaphi","m_histoLD_deltaphi",10,0,180);

}

LeptonPlusFakeSelection::~LeptonPlusFakeSelection(){

  delete _selectionEE;

}

void LeptonPlusFakeSelection::initialiseFakeRate() {

  // binning
  m_minFakePt[0] = 10.;  m_maxFakePt[0] = 15.;
  m_minFakePt[1] = 15.;  m_maxFakePt[1] = 20.;
  m_minFakePt[2] = 20.;  m_maxFakePt[2] = 25.;
  m_minFakePt[3] = 25.;  m_maxFakePt[3] = 30.;
  m_minFakePt[4] = 30.;  m_maxFakePt[4] = 35.;
  m_minFakePt[5] = 35.;  m_maxFakePt[5] = 40.;
  m_minFakePt[6] = 40.;  m_maxFakePt[6] = 14000.;

  m_fakeRateFromQCD[0] = 0.000738841;  m_fakeRateFromQCD_err[0] = 0.00051084;
  m_fakeRateFromQCD[1] = 0.00362373;   m_fakeRateFromQCD_err[1] = 0.0020247;
  m_fakeRateFromQCD[2] = 0.0011834;    m_fakeRateFromQCD_err[2] = 0.000642422;
  m_fakeRateFromQCD[3] = 0.00373419;   m_fakeRateFromQCD_err[3] = 0.00196991;
  m_fakeRateFromQCD[4] = 0.0044899;    m_fakeRateFromQCD_err[4] = 0.00333266;
  m_fakeRateFromQCD[5] = 0.000877784;  m_fakeRateFromQCD_err[5] = 0.000515083;
  m_fakeRateFromQCD[6] = 0.00131569;   m_fakeRateFromQCD_err[6] = 0.00100121;

  m_fakeRateFromWjets[0] = 0.000221169; m_fakeRateFromWjets_err[0] = 5.57981e-06;
  m_fakeRateFromWjets[1] = 0.000668425; m_fakeRateFromWjets_err[1] = 1.49002e-05;
  m_fakeRateFromWjets[2] = 0.00153968;  m_fakeRateFromWjets_err[2] = 3.58406e-05;
  m_fakeRateFromWjets[3] = 0.00188213;  m_fakeRateFromWjets_err[3] = 3.71708e-05;
  m_fakeRateFromWjets[4] = 0.00198295;  m_fakeRateFromWjets_err[4] = 3.23153e-05;
  m_fakeRateFromWjets[5] = 0.00116383;  m_fakeRateFromWjets_err[5] = 1.19151e-05;
  m_fakeRateFromWjets[6] = 0.00296834;  m_fakeRateFromWjets_err[6] = 8.59184e-05;

}

void LeptonPlusFakeSelection::Loop() {

  if(fChain == 0) return;
  
  initialiseFakeRate();

  TFile fileOut("LDvsLL.root","recreate");

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

    resetKinematics();

    float weight = 1;
    
    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);

    // discard the events with L1 + L2 (signal on data)
    std::pair<int,int> Leptons = getBestElectronPair();
    if ( Leptons.first > -1 && Leptons.second > -1 ) {
      int L1 = Leptons.first;
      int L2 = Leptons.second;
      bool eleID1 = isEleID(L1);
      bool eleID2 = isEleID(L2);
      float TrackerPtSum1 = dr04TkSumPtEle[L1] - getSecondEleTkPt(L1,L2,0.2);
      float TrackerPtSum2 = dr04TkSumPtEle[L2] - getSecondEleTkPt(L2,L1,0.2);
      float HcalPtSum1 = dr04HcalTowerSumEtEle[L1];
      float HcalPtSum2 = dr04HcalTowerSumEtEle[L2];
      float EcalPtSum1 = dr04EcalRecHitSumEtEle[L1] - getSecondEleEmEt(L1,L2,0.4);
      float EcalPtSum2 = dr04EcalRecHitSumEtEle[L2] - getSecondEleEmEt(L2,L1,0.4);
      if ( eleID1 && eleID2 && 
	   TrackerPtSum1 < 0.065 && TrackerPtSum2 < 0.065 &&
	   HcalPtSum1 < 0.1 && HcalPtSum2 < 0.1 &&
	   EcalPtSum1 < 0.2 && EcalPtSum2 < 0.2 ) {

	TLorentzVector L14Momentum(pxEle[L1],pyEle[L1],pzEle[L1],energyEle[L1]);
	TLorentzVector L24Momentum(pxEle[L2],pyEle[L2],pzEle[L2],energyEle[L2]);
	
	float L1L2mll = (L14Momentum + L24Momentum).M();

	float L1L2deltaphi = fabs( 180./TMath::Pi() * L14Momentum.Vect().DeltaPhi(L24Momentum.Vect()) );
	
	m_histoLL_mll->Fill( L1L2mll, weight );
	m_histoLL_ptmax->Fill( TMath::Max(L14Momentum.Vect().Mag(),L24Momentum.Vect().Mag()), weight );
	m_histoLL_ptmin->Fill( TMath::Min(L14Momentum.Vect().Mag(),L24Momentum.Vect().Mag()), weight );
	m_histoLL_met->Fill( etMet[0], weight );
	m_histoLL_deltaphi->Fill ( L1L2deltaphi, weight );

	continue;
      }
    }

    // get the best L1 + Fake combination
    getBestLplusFakePair();

    if (m_theL1 == -1 || m_theFake == -1) continue;

    // set the kinematic variables for the rest of the selection
    setKinematics();

    m_histoLD_mll->Fill( m_mll, weight );
    m_histoLD_ptmax->Fill( TMath::Max(m_theL14Momentum.Vect().Mag(),m_theFake4Momentum.Vect().Mag()), weight );
    m_histoLD_ptmin->Fill( TMath::Min(m_theL14Momentum.Vect().Mag(),m_theFake4Momentum.Vect().Mag()), weight );
    m_histoLD_met->Fill( etMet[0], weight );
    m_histoLD_deltaphi->Fill ( m_deltaPhi, weight );

    // weight with the Fake -> L2 probability
    float fakerateQCD = getFakeRate( m_theFake4Momentum.Pt(), QCD );
    float weightFromQCD = weight * fakerateQCD;
    float fakerateErrQCD = getFakeRateError( m_theFake4Momentum.Pt(), QCD );
    float weightErrorFromQCD = weight * fakerateErrQCD;

    float fakerateWjets = getFakeRate( m_theFake4Momentum.Pt(), Wjets );
    float weightFromWjets = weight * fakerateWjets;
    float fakerateErrWjets = getFakeRateError( m_theFake4Momentum.Pt(), Wjets );
    float weightErrorFromWjets = weight * fakerateErrWjets;
    
    // ----------------------- selection ----------------------------
    // electron ID (true by default - studied only if ee or emu channel)
    bool theElectronID = true;
    // custom electron ID
    if ( m_theL1 > -1 ) theElectronID = isEleID(m_theL1);

    // tracker isolation for electrons
    float theEleTrackerPtSum = 0.;
    if ( m_theL1 > -1 ) theEleTrackerPtSum = dr04TkSumPtEle[m_theL1];

    // hcal isolation for electrons
    float theEleHcalPtSum = 0.;
    if ( m_theL1 > -1 ) theEleHcalPtSum = dr04HcalTowerSumEtEle[m_theL1];

    // ecal isolation for electrons
    float theEleEcalPtSum = 0.;
    if ( m_theL1 > -1 ) theEleEcalPtSum = dr04EcalRecHitSumEtEle[m_theL1];

    // jet veto: method gives true if good jet is found (L1 and Fake are excluded)
    bool passedJetVeto = !goodJetFound();

    // selections: QCD counters
    LFakeFromQCDSelector.SetWeight( weightFromQCD );
    LFakeFromQCDSelector.SetHighElePt( m_theL14Momentum.Pt() );
    LFakeFromQCDSelector.SetLowElePt( m_theFake4Momentum.Pt() );
    LFakeFromQCDSelector.SetElectronId(theElectronID);
    LFakeFromQCDSelector.SetPositronId( true );
    LFakeFromQCDSelector.SetEleHardTrackerPtSum( theEleTrackerPtSum );
    LFakeFromQCDSelector.SetEleSlowTrackerPtSum( 0. );
    LFakeFromQCDSelector.SetEleHardHcalPtSum( theEleHcalPtSum );
    LFakeFromQCDSelector.SetEleSlowHcalPtSum( 0. );
    LFakeFromQCDSelector.SetEleHardEcalPtSum( theEleEcalPtSum );
    LFakeFromQCDSelector.SetEleSlowEcalPtSum( 0. );
    LFakeFromQCDSelector.SetJetVeto(passedJetVeto);
    LFakeFromQCDSelector.SetMet( etMet[0] );					
    LFakeFromQCDSelector.SetDeltaPhi( m_deltaPhi );
    LFakeFromQCDSelector.SetInvMass( m_mll );
    LFakeFromQCDSelector.SetDetaLeptons( 0. );
    bool selOutput = LFakeFromQCDSelector.output();

    LFakeErrorFromQCDSelector.SetWeight( weightErrorFromQCD );
    LFakeErrorFromQCDSelector.SetHighElePt( m_theL14Momentum.Pt() );
    LFakeErrorFromQCDSelector.SetLowElePt( m_theFake4Momentum.Pt() );
    LFakeErrorFromQCDSelector.SetElectronId(theElectronID);
    LFakeErrorFromQCDSelector.SetPositronId( true );
    LFakeErrorFromQCDSelector.SetEleHardTrackerPtSum( theEleTrackerPtSum );
    LFakeErrorFromQCDSelector.SetEleSlowTrackerPtSum( 0. );
    LFakeErrorFromQCDSelector.SetEleHardHcalPtSum( theEleHcalPtSum );
    LFakeErrorFromQCDSelector.SetEleSlowHcalPtSum( 0. );
    LFakeErrorFromQCDSelector.SetEleHardEcalPtSum( theEleEcalPtSum );
    LFakeErrorFromQCDSelector.SetEleSlowEcalPtSum( 0. );
    LFakeErrorFromQCDSelector.SetJetVeto(passedJetVeto);
    LFakeErrorFromQCDSelector.SetMet( etMet[0] );
    LFakeErrorFromQCDSelector.SetDeltaPhi( m_deltaPhi );
    LFakeErrorFromQCDSelector.SetInvMass( m_mll );
    LFakeErrorFromQCDSelector.SetDetaLeptons( 0. );
    selOutput = LFakeErrorFromQCDSelector.output();

    // selections: Wjets counters
    LFakeFromWjetsSelector.SetWeight( weightFromWjets );
    LFakeFromWjetsSelector.SetHighElePt( m_theL14Momentum.Pt() );
    LFakeFromWjetsSelector.SetLowElePt( m_theFake4Momentum.Pt() );
    LFakeFromWjetsSelector.SetElectronId(theElectronID);
    LFakeFromWjetsSelector.SetPositronId( true );
    LFakeFromWjetsSelector.SetEleHardTrackerPtSum( theEleTrackerPtSum );
    LFakeFromWjetsSelector.SetEleSlowTrackerPtSum( 0. );
    LFakeFromWjetsSelector.SetEleHardHcalPtSum( theEleHcalPtSum );
    LFakeFromWjetsSelector.SetEleSlowHcalPtSum( 0. );
    LFakeFromWjetsSelector.SetEleHardEcalPtSum( theEleEcalPtSum );
    LFakeFromWjetsSelector.SetEleSlowEcalPtSum( 0. );
    LFakeFromWjetsSelector.SetJetVeto(passedJetVeto);
    LFakeFromWjetsSelector.SetMet( etMet[0] );					
    LFakeFromWjetsSelector.SetDeltaPhi( m_deltaPhi );
    LFakeFromWjetsSelector.SetInvMass( m_mll );
    LFakeFromWjetsSelector.SetDetaLeptons( 0. );
    selOutput = LFakeFromWjetsSelector.output();

    LFakeErrorFromWjetsSelector.SetWeight( weightErrorFromWjets );
    LFakeErrorFromWjetsSelector.SetHighElePt( m_theL14Momentum.Pt() );
    LFakeErrorFromWjetsSelector.SetLowElePt( m_theFake4Momentum.Pt() );
    LFakeErrorFromWjetsSelector.SetElectronId(theElectronID);
    LFakeErrorFromWjetsSelector.SetPositronId( true );
    LFakeErrorFromWjetsSelector.SetEleHardTrackerPtSum( theEleTrackerPtSum );
    LFakeErrorFromWjetsSelector.SetEleSlowTrackerPtSum( 0. );
    LFakeErrorFromWjetsSelector.SetEleHardHcalPtSum( theEleHcalPtSum );
    LFakeErrorFromWjetsSelector.SetEleSlowHcalPtSum( 0. );
    LFakeErrorFromWjetsSelector.SetEleHardEcalPtSum( theEleEcalPtSum );
    LFakeErrorFromWjetsSelector.SetEleSlowEcalPtSum( 0. );
    LFakeErrorFromWjetsSelector.SetJetVeto(passedJetVeto);
    LFakeErrorFromWjetsSelector.SetMet( etMet[0] );
    LFakeErrorFromWjetsSelector.SetDeltaPhi( m_deltaPhi );
    LFakeErrorFromWjetsSelector.SetInvMass( m_mll );
    LFakeErrorFromWjetsSelector.SetDetaLeptons( 0. );
    selOutput = LFakeErrorFromWjetsSelector.output();
      
  }

  std::cout << "=== RATE ESTIMATED FROM QCD FAKE RATE ===" << std::endl; 
  LFakeFromQCDSelector.diplayEfficiencies("pippo");

  std::cout << "=== RATE UNCERTAINTY ESTIMATED FROM QCD FAKE RATE ===" << std::endl;
  LFakeErrorFromQCDSelector.diplayEfficiencies("pippo");
  
  std::cout << "=== RATE ESTIMATED FROM WJETS FAKE RATE ===" << std::endl; 
  LFakeFromWjetsSelector.diplayEfficiencies("pippo");

  std::cout << "=== RATE UNCERTAINTY ESTIMATED FROM WJETS FAKE RATE ===" << std::endl;
  LFakeErrorFromWjetsSelector.diplayEfficiencies("pippo");

  fileOut.cd();
  m_histoLL_mll->Write();
  m_histoLL_ptmax->Write();
  m_histoLL_ptmin->Write();
  m_histoLL_met->Write();
  m_histoLL_deltaphi->Write();
  m_histoLD_mll->Write();
  m_histoLD_ptmax->Write();
  m_histoLD_ptmin->Write();
  m_histoLD_met->Write();
  m_histoLD_deltaphi->Write();
  fileOut.Close();
}

std::pair<int,int> LeptonPlusFakeSelection::getBestElectronPair() {
  int theLep1=-1;
  int theLep2=-1;
  float maxPtLep1=-1000.;
  float maxPtLep2=-1000.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nEle;i++) {
    if( fabs(etaEle[i]) > 2.5 ) continue;
    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();
    if( thisPt < 20 ) continue;
    float thisCharge = chargeEle[i];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
  }
  return make_pair(theLep1,theLep2);
}

void LeptonPlusFakeSelection::getBestLplusFakePair() {
  int theLep1=-1;
  int theFake=-1;
  float maxPtLep1=-1000.;

  for(int i=0;i<nEle;i++) {
    if( fabs(etaEle[i]) > 2.5 ) continue;
    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();
    if( thisPt < 20 ) continue;
    if (thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
  }
  m_theL1 = theLep1;
  if ( theLep1 > -1 ) 
    m_theL14Momentum.SetXYZT(pxEle[theLep1],pyEle[theLep1],pzEle[theLep1],energyEle[theLep1]);
  else 
    m_theL14Momentum.SetXYZT(0,0,0,0);
  
  float maxPtJet=-1000.;
  for(int j=0;j<nAK5Jet;j++) {

    // check if the electron or the positron falls into the jet
    // common cleaning class
    TVector3 p3Jet(pxAK5Jet[j],pyAK5Jet[j],pzAK5Jet[j]);
    if ( m_theL14Momentum.Vect().Mag() != 0 ) {
      float deltaR =  p3Jet.DeltaR( m_theL14Momentum.Vect() );
      // taking from ee config file, but jets veto is the same for all the channels
      if(_selectionEE->getSwitch("jetConeWidth") && 
	 _selectionEE->passCut("jetConeWidth",deltaR)
	// && (m_HoEElectronMinus < 0.2)  
	// && (m_CaloEneElectronMinus/energyJet[j] > 0.9)
	 ) continue;
    }

    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc",etaAK5Jet[j])) continue;
    if(_selectionEE->getSwitch("etJetLowAcc") && !_selectionEE->passCut("etJetLowAcc",etAK5Jet[j]) ) continue;

    if(etAK5Jet[j]>maxPtJet) { maxPtJet=etAK5Jet[j]; theFake=j; }

    break;
  }

  m_theFake = theFake;
  if ( theFake > -1 )
    m_theFake4Momentum.SetXYZT(pxAK5Jet[theFake],pyAK5Jet[theFake],pzAK5Jet[theFake],energyAK5Jet[theFake]);
  else
    m_theFake4Momentum.SetXYZT(0,0,0,0);
  
}


bool LeptonPlusFakeSelection::isEleID(int eleIndex) {

  Utils anaUtils;

  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  EgammaCutBasedID.SetHOverE( hOverEEle[eleIndex] );
  EgammaCutBasedID.SetS9S25( s9s25Ele[eleIndex] );
  EgammaCutBasedID.SetDEta( deltaEtaAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiIn( deltaPhiAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiOut( deltaPhiAtCaloEle[eleIndex] );
  EgammaCutBasedID.SetInvEminusInvP( 1./ecalEle[eleIndex]-1./momentumEle[eleIndex] );
  EgammaCutBasedID.SetBremFraction( fabs(momentumEle[eleIndex]-pTrkAtOuter.Mag())/momentumEle[eleIndex] );
  EgammaCutBasedID.SetSigmaEtaEta( sqrt(covEtaEtaEle[eleIndex]) );
  EgammaCutBasedID.SetSigmaPhiPhi( sqrt(covPhiPhiEle[eleIndex]) );
  EgammaCutBasedID.SetEOverPout( eSeedOverPoutEle[eleIndex] );
  EgammaCutBasedID.SetEOverPin( eSuperClusterOverPEle[eleIndex] );
  EgammaCutBasedID.SetElectronClass ( classificationEle[eleIndex] );
  EgammaCutBasedID.SetEgammaCutBasedID ( anaUtils.electronIdVal(eleIdCutsEle[eleIndex],eleIdLoose) );
  EgammaCutBasedID.SetLikelihood( eleIdLikelihoodEle[eleIndex] );

  bool isIdentified = EgammaCutBasedID.output();

  return isIdentified;
}



void LeptonPlusFakeSelection::setKinematics() {

  m_mll = (m_theL14Momentum + m_theFake4Momentum).M();

  // MET
  m_MET = etMet[0]; 

  // compute delta Phi in degrees
  TVector3 dilepPt;
  m_deltaPhi = fabs( 180./TMath::Pi() * m_theL14Momentum.Vect().DeltaPhi(m_theFake4Momentum.Vect()) );

}


bool LeptonPlusFakeSelection::goodJetFound() {

  // first check that kinematics has been set
  bool foundJet=false;
  for(int j=0;j<nAK5Jet;j++) {

    if ( j == m_theFake ) continue;

    // check if the electron or the positron falls into the jet
    // common cleaning class
    TVector3 p3Jet(pxAK5Jet[j],pyAK5Jet[j],pzAK5Jet[j]);
    if ( m_theL14Momentum.Vect().Mag() != 0 ) {
      float deltaR =  p3Jet.DeltaR( m_theL14Momentum.Vect() );
      // taking from ee config file, but jets veto is the same for all the channels
      if(_selectionEE->getSwitch("jetConeWidth") && 
	 _selectionEE->passCut("jetConeWidth",deltaR)
	// && (m_HoEElectronMinus < 0.2)  
	// && (m_CaloEneElectronMinus/energyJet[j] > 0.9)
	 ) continue;
    }

    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc",etaAK5Jet[j])) continue;
    if(_selectionEE->getSwitch("etJetLowAcc") && !_selectionEE->passCut("etJetLowAcc",etAK5Jet[j]) ) continue;

    if( (_selectionEE->getSwitch("etJetHighAcc") && _selectionEE->passCut("etJetHighAcc",etAK5Jet[j])) &&
 	(_selectionEE->getSwitch("alphaJet") && !_selectionEE->passCut("alphaJet",alphaAK5Jet[j])) 
	) continue;
    foundJet=true;
    break;
  }

  return foundJet;

}

void LeptonPlusFakeSelection::resetKinematics() {

  m_theL1 = -1;
  m_theFake = -1;
  m_theL14Momentum.SetXYZT(0,0,0,0);
  m_theFake4Momentum.SetXYZT(0,0,0,0);
  m_deltaPhi = 0;
  m_mll = 0;

}

float LeptonPlusFakeSelection::getSecondEleTkPt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleTrackPt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr < deltaR ) { 
    secondEleTrackPt = momentumEle[second] * fabs( sin(thetaEle[second]) );
  }

  return secondEleTrackPt;

}

float LeptonPlusFakeSelection::getSecondEleEmEt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleEmEt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr < deltaR ) { 
    secondEleEmEt = energyEle[second] * fabs( sin(thetaEle[second]) );
  }
  
  return secondEleEmEt;

}


float LeptonPlusFakeSelection::getFakeRate(float fakePt, int source) {
  for (int i=0; i<7; i++) {
    if( fakePt >= m_minFakePt[i] && fakePt < m_maxFakePt[i] ) {
      if( source == QCD ) {
	return m_fakeRateFromQCD[i];
      } else if( source == Wjets ) {
	return m_fakeRateFromWjets[i];
      }
    }
  }
  std::cout << "BIG ERROR: source = " << source << " fakePt = " << fakePt << endl; 
  return -1.;
}

float LeptonPlusFakeSelection::getFakeRateError(float fakePt, int source) {
  for (int i=0; i<7; i++) {
    if( fakePt >= m_minFakePt[i] && fakePt < m_maxFakePt[i] ) {
      if( source == QCD ) {
	return m_fakeRateFromQCD_err[i];
      } else if ( source == Wjets ) {
	return m_fakeRateFromWjets_err[i];
      }
    }
  }
  return -1.;
}

