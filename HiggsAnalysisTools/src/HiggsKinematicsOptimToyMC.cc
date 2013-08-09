#include <string>
#include <iostream>

#include <TTree.h>
#include "TRandom.h"

#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/HiggsKinematicsOptimToyMC.hh"

HiggsKinematicsOptimToyMC::HiggsKinematicsOptimToyMC(TTree *tree) 
  : HiggsBase(tree) {

  // to be changed 
  // A) signal or background - later on
  // B) higgs mass
  theHmass = 160;   
  // C) number of variables
  nVar = 5;         

  // for full class dependent eleID
  char egamma[500];
  sprintf(egamma,"../EgammaAnalysisTools/config/newOptimEleId_looseOthers_m%d/",theHmass);
  cout << egamma << endl;
  EgammaCutBasedID.Configure(egamma);

  // kinematics
  _p4ElectronPlus  = new TLorentzVector(0.,0.,0.,0.);
  _p4ElectronMinus = new TLorentzVector(0.,0.,0.,0.);
  _bestElectrons   = new std::vector<int>;
  
  // event weight
  theWeight = 1;

  // counters
  allEvents    = 0.;
  passedMc     = 0.;
  triggered    = 0.; 
  commonPresel = 0.; 
  passedReco   = 0.;
  elePresel    = 0.; 
  looseId      = 0.;
  looseIsol    = 0.;
  fullId       = 0.; 
  fullIsol     = 0.;
  jetVeto      = 0.;

  // 1 dimension histos
  H_mee    = new TH1F("H_mee",    "H_mee",    40,  0.0, 110.0);
  H_met    = new TH1F("H_met",    "H_met",    40, 30.0, 150.0);
  H_dphi   = new TH1F("H_dphi",   "H_dphi",   40,  0.0, 180.0);
  H_ptHigh = new TH1F("H_ptHigh", "H_ptHigh", 40, 20.0, 110.0);
  H_ptLow  = new TH1F("H_ptLow",  "H_ptLow",  40, 10.0,  80.0);

  // N dimensions histo
  theBins[0] = 50;
  theBins[1] = 50;
  theBins[2] = 50;
  theBins[3] = 50;
  theBins[4] = 50;
  // 
  theMin[0] =  0.0;
  theMin[1] = 30.0; 
  theMin[2] =  0.0; 
  theMin[3] = 20.0; 
  theMin[4] = 10.0; 
  //
  theMax[0] = 110.0;
  theMax[1] = 150.0;
  theMax[2] = 180.0;
  theMax[3] = 110.0;
  theMax[4] =  80.0;
  // 
  H_NVarDim = new THnSparseF("H_NVarDim", "H_NVarDim", nVar, theBins, theMin, theMax);

  // output trees
  outRootFileWithTree = new TFile("myOutTree.root", "RECREATE");
  outRootTree =  new TTree("T1","kinematics tree");
  outRootTree->Branch("mee",    &myMll,    "mee/F");
  outRootTree->Branch("met",    &myMet,    "met/F");
  outRootTree->Branch("dphi",   &myDphi,   "dphi/F");
  outRootTree->Branch("ptHigh", &myPtHigh, "ptHigh/F");
  outRootTree->Branch("ptLow",  &myPtLow,  "ptLow/F");

  // output root file
  outRootFile = new TFile("outHistos.root","RECREATE");
}

HiggsKinematicsOptimToyMC::~HiggsKinematicsOptimToyMC(){
  
  // output txt file with efficiencies:
  ofstream *outTxtFile = new ofstream("outputFile.txt",ios::app);
  *outTxtFile << "all events:    "      << allEvents    << endl;
  *outTxtFile << "passedMC:      "      << passedMc     << endl;
  *outTxtFile << "triggered:     "      << triggered    << endl;
  *outTxtFile << "common presel: "      << commonPresel << endl;
  *outTxtFile << "reco:          "      << passedReco   << endl;
  *outTxtFile << "presel x ele:  "      << elePresel    << endl;
  *outTxtFile << "loose eleID:   "      << looseId      << endl;
  *outTxtFile << "loose isolation: "    << looseIsol    << endl;
  *outTxtFile << "full eleID:   "       << fullId       << endl;
  *outTxtFile << "full isol:    "       << fullIsol     << endl;
  *outTxtFile << "jetVeto:      "       << jetVeto      << endl;

  // saving histos
  outRootFile->Close();

  // saving tree
  outRootFileWithTree->cd();
  outRootTree->Write();
  outRootFileWithTree->Close();

  // deleting
  delete _p4ElectronPlus;
  delete _p4ElectronMinus;
  delete _bestElectrons;
}

bool HiggsKinematicsOptimToyMC::findMcTree(const char* processType) {

  _process = "UNDEFINED";
  _theGenEle = -1;
  _theGenPos = -1;
  
  // signal: 2e2nu
  if(strcmp(processType,"HtoWWto2e2nu")==0) {
    if( idMc[9]  == -11) _theGenEle = 9;
    if( idMc[11] ==  11) _theGenPos = 11;
    theWeight=1; 
    return (_theGenEle > -1 && _theGenPos > -1 );
  }
  // w+jets: e / mu / tau
  else if(strcmp(processType,"Wjets")==0) {
    _process = "Wjets";
    theWeight=genWeight; 
    return ( genAlpgenID>=1000 && genAlpgenID<2000);
  }
  // z+jets: e / mu / tau
  else if(strcmp(processType,"Zjets")==0) {
    _process = "Zjets";
    theWeight=genWeight;
    return ( genAlpgenID>=2000 && genAlpgenID<3000 );
  }
  // ttbar (inclusive)
  else if(strcmp(processType,"ttbar")==0) {
    _process = "ttbar";
    theWeight=genWeight;
    return ( genAlpgenID>=3000 && genAlpgenID<4000 );
  }
  else if(strcmp(processType,"inclusive")==0) {
    theWeight=1; 
    return true;
  }
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}


void HiggsKinematicsOptimToyMC::Loop() {

  _verbose=false;
  if(fChain == 0) return;
  
  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

    resetKinematics();

    // before any cut
    allEvents=allEvents+theWeight; 

    // look to the MC truth decay tree
    // bool foundMcTree = findMcTree("HtoWWto2e2nu");
    // bool foundMcTree = findMcTree("ttbar");
    bool foundMcTree = findMcTree("inclusive");
    if ( !foundMcTree ) continue;
    passedMc=passedMc+theWeight;    

    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);
    if (!passedHLT) continue; 
    triggered=triggered+theWeight;   

    // did we pass preselections?
    if( !evtPresel ) continue;
    commonPresel=commonPresel+theWeight;

    // did we reconstruct two electrons?
    std::pair<int,int> theElectrons = getBestElectronPair();
    int tbElectron(theElectrons.second), tbPositron(theElectrons.first);    
    theElectron = tbElectron;
    thePositron = tbPositron;
    if (theElectron<0 || thePositron<0)  continue;  
    passedReco=passedReco+theWeight;  

    // did we pass preselections specific for electrons?
    setKinematics();
    if (hardestElectronPt < 20) continue; 
    if (slowestElectronPt < 10) continue; 
    if (etMet[0] < 30)          continue; 
    if (_mll < 12)              continue; 
    elePresel=elePresel+theWeight; 

    // did we pass the loose electronId?
    if (!eleIdCutBasedEle[theElectron] || !eleIdCutBasedEle[thePositron]) continue;
    looseId=looseId+theWeight;   

    // did we pass loose electron tracker based isolation?
    if (eleSumPtPreselectionEle[theElectron]>0.1 || eleSumPtPreselectionEle[thePositron]>0.1) continue;
    looseIsol=looseIsol+theWeight;

    // did we pass the full class dependent electronId?  
    bool theElectronID = isEleID(theElectron);
    bool thePositronID = isEleID(thePositron);
    if (!theElectronID || !thePositronID) continue;
    fullId=fullId+theWeight;   

    // did we pass the full electron isolation?
    float theEleTrackerPtSum = eleSumPtPreselectionEle[theElectron] - getSecondEleTkPt(theElectron,thePositron,0.2);
    float thePosTrackerPtSum = eleSumPtPreselectionEle[thePositron] - getSecondEleTkPt(thePositron,theElectron,0.2);
    if ( theEleTrackerPtSum>0.065 || thePosTrackerPtSum>0.065 ) continue;

    if ( eleSumHadEt04Ele[theElectron]>0.1 || eleSumHadEt04Ele[thePositron]>0.1 ) continue;

    float theEleEcalPtSum = eleSumEmEt04Ele[theElectron] - getSecondEleEmEt(theElectron,thePositron,0.4);
    float thePosEcalPtSum = eleSumEmEt04Ele[thePositron] - getSecondEleEmEt(thePositron,theElectron,0.4);
    if ( theEleEcalPtSum>0.2 || thePosEcalPtSum>0.2 ) continue;
    fullIsol=fullIsol+theWeight;

    // full kine analysis
    if (goodJetFound())           continue;
    jetVeto=jetVeto+theWeight;
    
    // ----------------------------------------------------------
    // filling histos to drive the toy MC generation:    
    double toFill[nVar];
    toFill[0] = _mll;
    toFill[1] = etMet[0];
    toFill[2] = _deltaPhi;
    toFill[3] = hardestElectronPt;
    toFill[4] = slowestElectronPt;

    H_mee     -> Fill(toFill[0],theWeight);
    H_met     -> Fill(toFill[1],theWeight);
    H_dphi    -> Fill(toFill[2],theWeight);
    H_ptHigh  -> Fill(toFill[3],theWeight);
    H_ptLow   -> Fill(toFill[4],theWeight);
    H_NVarDim -> Fill(toFill,theWeight);
    // ----------------------------------------------------------
    
  } // end loop over entries

  // generating a random distribution according to the sampled one
  TDatime *now = new TDatime();  
  for(int scan =0; scan<100000; scan++){
    if(scan%1000==0) cout << "random: " << scan << endl;

    // randomly generated electron and positron 
    int today = now->GetDate();  
    int clock = now->GetTime();  
    int seed  = today+clock+scan;
    gRandom->SetSeed(seed);
    double theRnd[nVar];
    H_NVarDim->GetRandom(theRnd);

    // filling the reduced tree with toy MC electrons
    myMll    = theRnd[0];
    myMet    = theRnd[1];
    myDphi   = theRnd[2];
    myPtHigh = theRnd[3];
    myPtLow  = theRnd[4];
    
    outRootTree->Fill();
    
  } // mc generation
  delete now;

  // saving the histos
  outRootFile->cd();
  H_mee    ->Write();
  H_met    ->Write();
  H_dphi   -> Write();
  H_ptHigh -> Write();
  H_ptLow  ->Write();
}

std::pair<int,int> HiggsKinematicsOptimToyMC::getBestElectronPair() {
  int theLep1=-1;          
  int theLep2=-1;
  float maxPtLep1=-1000.;  
  float maxPtLep2=-1000.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nEle;i++) {
    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();
    if(fabs(etaEle[i])>2.5) continue;
    if(thisPt<10)           continue;
    float thisCharge = chargeEle[i];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
  }
  _bestElectrons->clear();
  _bestElectrons->push_back(theLep1);  
  _bestElectrons->push_back(theLep2); 
  return make_pair(theLep1,theLep2);
}

void HiggsKinematicsOptimToyMC::setKinematics() {
  hardestElectronPt = TMath::Max(etEle[theElectron],etEle[thePositron]);
  slowestElectronPt = TMath::Min(etEle[theElectron],etEle[thePositron]);
  _p4ElectronMinus -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
  _p4ElectronPlus  -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);      
  _mll = (*_p4ElectronMinus + *_p4ElectronPlus).M();
  _deltaPhi = fabs(180./TMath::Pi() * _p4ElectronMinus->Vect().DeltaPhi(_p4ElectronPlus->Vect()));
  _HoEElectronMinus     = hOverEEle[theElectron];
  _HoEElectronPlus      = hOverEEle[thePositron];
  _CaloEneElectronMinus = ecalEle[theElectron];
  _CaloEneElectronPlus  = ecalEle[thePositron];
}

void HiggsKinematicsOptimToyMC::resetKinematics() {
  theElectron = -1;
  thePositron = -1;
  _p4ElectronMinus->SetXYZT(0,0,0,0);
  _p4ElectronPlus->SetXYZT(0,0,0,0);
  _mll = 0;
  _deltaPhi = 0;
  hardestElectronPt = 0;
  slowestElectronPt = 0;
  _HoEElectronMinus = 0;
  _HoEElectronPlus  = 0;
  _CaloEneElectronMinus = 0;
  _CaloEneElectronPlus  = 0;
}

bool HiggsKinematicsOptimToyMC::goodJetFound() {

  bool foundJet=false;
  for(int jj=0;jj<nJet;jj++) {
    // check if the electron falls into the jet
    TVector3 p3Jet(pxJet[jj],pyJet[jj],pzJet[jj]);
    if (_p4ElectronMinus->Vect().Mag() != 0) {
      float deltaR = p3Jet.DeltaR(_p4ElectronMinus->Vect());
      if( (deltaR<0.2) && (_HoEElectronMinus < 0.2) && (_CaloEneElectronMinus/energyJet[jj] > 0.9) ) continue;
    }
    // check if the positron falls into the jet
    if (_p4ElectronPlus->Vect().Mag() != 0) {
      float deltaR =  p3Jet.DeltaR(_p4ElectronPlus->Vect());
      if( (deltaR<0.2) && (_HoEElectronPlus < 0.2) && (_CaloEneElectronPlus/energyJet[jj] > 0.9) ) continue;
    }
    // checking the jet kinematics
    if(fabs(etaJet[jj])>2.5) continue;
    if(etJet[jj]<15)         continue;
    if(etJet[jj]>=15 && etJet[jj]<=20 && alphaJet[jj]<0.2) continue; 

    // this is a good jet
    foundJet=true;
    break;
  }
  return foundJet;
}

bool HiggsKinematicsOptimToyMC::isEleID(int eleIndex) {

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
  EgammaCutBasedID.SetEgammaCutBasedID ( eleIdCutBasedEle[eleIndex] );
  EgammaCutBasedID.SetLikelihood( eleIdLikelihoodEle[eleIndex] );

  bool isIdentified = EgammaCutBasedID.output();

  return isIdentified;
}

float HiggsKinematicsOptimToyMC::getSecondEleTkPt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleTrackPt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr < deltaR ) { 
    secondEleTrackPt = momentumEle[second] * fabs( sin(thetaEle[second]) );
  }

  return secondEleTrackPt;

}

float HiggsKinematicsOptimToyMC::getSecondEleEmEt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleEmEt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr < deltaR ) { 
    secondEleEmEt = energyEle[second] * fabs( sin(thetaEle[second]) );
  }
  
  return secondEleEmEt;

}
