#include <string>
#include <iostream>

#include <TTree.h>
#include "TRandom.h"

#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/HiggsVertexing.hh"

HiggsVertexing::HiggsVertexing(TTree *tree) 
  : HiggsBase(tree) {
  
  // to be changed: signal or background - later on
  

  
  // for full class dependent eleID
  char egamma[500];
  sprintf(egamma,"../EgammaAnalysisTools/config/newOptimEleId_looseOthers_m160/");
  cout << egamma << endl;
  EgammaCutBasedID.Configure(egamma);
  
  // kinematics
  _p4ElectronPlus  = new TLorentzVector(0.,0.,0.,0.);
  _p4ElectronMinus = new TLorentzVector(0.,0.,0.,0.);
  _bestElectrons   = new std::vector<int>;
  
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
  trackerIsol  = 0.; 
  ecalIsol     = 0.; 
  hcalIsol     = 0.; 

  // 1 dimension histos 
  H_deltaZ    = new TH1F("H_deltaZ",    "H_deltaZ" ,   80, -0.5, 0.5);  
  H_deltaXY   = new TH1F("H_deltaXY",   "H_deltaXY",   80,  0.0, 0.5);  
  H_deltaXYZ  = new TH1F("H_deltaXYZ",  "H_deltaXYZ",  80,  0.0, 0.5);  
  H_trackDz   = new TH1F("H_trackDz",   "H_trackDz" ,  80, -0.5, 0.5);  
  H_trackDxy  = new TH1F("H_trackDxy",  "H_trackDxy" , 80, -0.5, 0.5);  
  H_trackDxyz = new TH1F("H_trackDxyz", "H_trackDxyz", 80, -0.5, 0.5);  
  H_compZ     = new TH1F("H_compZ",     "H_compZ",    100,  0.,100.);
  H_compXY    = new TH1F("H_compXY",    "H_compXY",   100,  0.,100.);
  H_compXYZ   = new TH1F("H_compXYZ",   "H_compXYZ",  100,  0.,100.);

}

HiggsVertexing::~HiggsVertexing(){
  
  // output txt file with efficiencies:
  ofstream *outTxtFile = new ofstream("outputFile.txt",ios::app);
  *outTxtFile << "all events:      "        << allEvents    << endl;
  *outTxtFile << "passedMC:        "        << passedMc     << endl;
  *outTxtFile << "triggered:       "        << triggered    << endl;
  *outTxtFile << "common presel:   "        << commonPresel << endl;
  *outTxtFile << "reco:            "        << passedReco   << endl;
  *outTxtFile << "presel x ele:    "        << elePresel    << endl;
  *outTxtFile << "loose eleID:     "        << looseId      << endl;
  *outTxtFile << "loose isolation: "        << looseIsol    << endl;
  *outTxtFile << "full eleID:      "        << fullId       << endl;
  *outTxtFile << "full tracker isolation: " << trackerIsol  << endl; 
  *outTxtFile << "full Ecal isolation:    " << ecalIsol     << endl; 
  *outTxtFile << "full Hcal isolation:    " << hcalIsol     << endl; 
  
  // saving histos and tree
  outRootFile->Close();
  outRootTree->save();
  
  // deleting
  delete _p4ElectronPlus;
  delete _p4ElectronMinus;
  delete _bestElectrons;
}

bool HiggsVertexing::findMcTree(const char* processType) {
  
  _theGenEle = -1;
  _theGenPos = -1;
  
  // signal: 2e2nu
  if(strcmp(processType,"HtoWWto2e2nu")==0) {
    if( idMc[9]  == -11) _theGenEle = 9;
    if( idMc[11] ==  11) _theGenPos = 11;
    return (_theGenEle > -1 && _theGenPos > -1 );
  }
  // backgrounds: everything
  else if(strcmp(processType,"Background")==0) return 1;
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}


void HiggsVertexing::Loop(const char *filename) {

  _verbose=false;
  if(fChain == 0) return;
  
  // output trees and root file  
  char outfilename[200];
  sprintf(outfilename,"%s_VertexTree.root",filename);
  outRootTree = new VertexTree(outfilename);
  sprintf(outfilename,"%s_VertexHistos.root",filename);
  outRootFile = new TFile(outfilename,"RECREATE");
  sprintf(outfilename,"%s_outputFile.txt",filename);
  outTxtFile = new ofstream(outfilename,ios::app);


  // loop
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
    allEvents++; 

    // look to the MC truth decay tree
    bool foundMcTree = findMcTree("HtoWWto2e2nu");
    // bool foundMcTree = findMcTree("Background");
    if ( !foundMcTree ) continue;
    passedMc++;    

    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);
    if (!passedHLT) continue; 
    triggered++;   

    // did we pass preselections?
    // bool is missing in summer08 ntuples,
    // but preselections are anyway applied later on for electrons only...
    // we can skip this part
    // if( !evtPresel ) continue;
    commonPresel++;

    // did we reconstruct two electrons?
    std::pair<int,int> theElectrons = getBestElectronPair();
    int tbElectron(theElectrons.second), tbPositron(theElectrons.first);    
    theElectron = tbElectron;
    thePositron = tbPositron;
    if (theElectron<0 || thePositron<0)  continue;  
    passedReco++;  

    // did we pass preselections specific for electrons?
    setKinematics();
    if (hardestElectronPt < 20) continue; 
    if (slowestElectronPt < 10) continue; 
    if (etMet[0] < 30)          continue; 
    if (_mll < 12)              continue; 
    elePresel++; 

    // did we pass the loose electronId?
    if (!eleIdCutBasedEle[theElectron] || !eleIdCutBasedEle[thePositron]) continue;
    looseId++;   

    // did we pass loose electron tracker based isolation?
    if (eleSumPtPreselectionEle[theElectron]>0.1 || eleSumPtPreselectionEle[thePositron]>0.1) continue;
    looseIsol++;
    
    // did we pass the full class dependent electronId?  
    bool theElectronID = isEleID(theElectron);
    bool thePositronID = isEleID(thePositron);
    if (!theElectronID || !thePositronID) continue;
    fullId++;   

    // did we pass the extra tracker isolation?
    float theEleTrackerPtSum = 0.;
    float thePosTrackerPtSum = 0.;
    if (theElectron > -1 && thePositron > -1) {
      float ptEle = sqrt(pxEle[theElectron]*pxEle[theElectron] + pyEle[theElectron]*pyEle[theElectron]);
      theEleTrackerPtSum = (eleSumPtPreselectionEle[theElectron]*ptEle - getSecondEleTkPt(theElectron,thePositron,0.2))/ptEle;
    }
    if (thePositron > -1 && theElectron > -1) {
      float ptPos = sqrt(pxEle[thePositron]*pxEle[thePositron] + pyEle[thePositron]*pyEle[thePositron]);
      thePosTrackerPtSum = (eleSumPtPreselectionEle[thePositron]*ptPos - getSecondEleTkPt(thePositron,theElectron,0.2))/ptPos;
    }
    if (theElectron > -1 && thePositron < 0)  theEleTrackerPtSum = eleSumPtPreselectionEle[theElectron];
    if (thePositron > -1 && theElectron < 0)  thePosTrackerPtSum = eleSumPtPreselectionEle[thePositron];
    if (theEleTrackerPtSum>0.065) continue;
    if (thePosTrackerPtSum>0.065) continue;
    trackerIsol++;

    // hcal isolation for electrons
    float theEleHcalPtSum = 0.;
    float thePosHcalPtSum = 0.;
    if (theElectron > -1) theEleHcalPtSum = eleSumHadEt04Ele[theElectron];
    if (thePositron > -1) thePosHcalPtSum = eleSumHadEt04Ele[thePositron];
    if (theEleHcalPtSum>0.10) continue;
    if (thePosHcalPtSum>0.10) continue;
    hcalIsol++;
    
    // ecal isolation for electrons
    float theEleEcalPtSum = 0.;
    float thePosEcalPtSum = 0.;
    if (theElectron > -1 && thePositron > -1) {
      float ptEle = sqrt(pxEle[theElectron]*pxEle[theElectron] + pyEle[theElectron]*pyEle[theElectron]);
      theEleEcalPtSum = (eleSumEmEt04Ele[theElectron]*ptEle - getSecondEleEmEt(theElectron,thePositron,0.4))/ptEle;
    }
    if (thePositron > -1 && theElectron > -1) {
      float ptPos = sqrt(pxEle[thePositron]*pxEle[thePositron] + pyEle[thePositron]*pyEle[thePositron]);
      thePosEcalPtSum = (eleSumEmEt04Ele[thePositron]*ptPos - getSecondEleEmEt(thePositron,theElectron,0.4))/ptPos;
    }
    if (theElectron > -1 && thePositron < 0 ) theEleEcalPtSum = eleSumEmEt04Ele[theElectron];
    if (thePositron > -1 && theElectron < 0 ) thePosEcalPtSum = eleSumEmEt04Ele[thePositron];
    if (theEleEcalPtSum>0.20) continue;
    if (thePosEcalPtSum>0.20) continue;
    ecalIsol++;

    // computing dxy and dz between the two electrons: 1 variable x event
    float dz   = eleTrackVzEle[theElectron] - eleTrackVzEle[thePositron];
    float dxy  = sqrt( (eleTrackVxEle[theElectron] - eleTrackVxEle[thePositron]) + (eleTrackVyEle[theElectron] - eleTrackVyEle[thePositron]) );
    float dxyz = sqrt( (eleTrackVxEle[theElectron] - eleTrackVxEle[thePositron]) + (eleTrackVyEle[theElectron] - eleTrackVyEle[thePositron]) + (eleTrackVzEle[theElectron] - eleTrackVzEle[thePositron]) );
    

    // computing the average track compatibility within a dR cone around the electron
    float eleTotTrack     = 0;
    float posTotTrack     = 0;
    float eleCompTrackXY  = 0;
    float posCompTrackXY  = 0;
    float eleCompTrackXYZ = 0;
    float posCompTrackXYZ = 0;
    float eleCompTrackZ   = 0;
    float posCompTrackZ   = 0;
    for(int theTrack = 0; theTrack<nTrack; theTrack++){ 
      
      TVector3 trackPAtVtx(pxTrack[theTrack], pyTrack[theTrack], pzTrack[theTrack]);
      TVector3 elePAtVtx(pxEle[theElectron], pyEle[theElectron], pzEle[theElectron]);
      
      // usually low pt tracks are fakes
      float this_pt  = trackPAtVtx.Perp();
      if ( this_pt < 1. ) continue;
      
      // does the track fall within the cone?
      double this_dr = elePAtVtx.DeltaR(trackPAtVtx);
      if ( fabs(this_dr) > 0.4 || fabs(this_dr) < 0.015 ) continue;
      
      // computing dz and dxy and dxyz
      float thisDz_ele   = eleTrackVzEle[theElectron] - vertexZTrack[theTrack];
      float thisDxy_ele  = sqrt((eleTrackVxEle[theElectron]-vertexXTrack[theTrack])+(eleTrackVyEle[theElectron]-vertexYTrack[theTrack]));
      float thisDxyz_ele = sqrt((eleTrackVxEle[theElectron]-vertexXTrack[theTrack])+(eleTrackVyEle[theElectron]-vertexYTrack[theTrack])+(eleTrackVzEle[theElectron]-vertexZTrack[theTrack]));
      
      H_trackDz   -> Fill(thisDz_ele);
      H_trackDxy  -> Fill(thisDxy_ele);
      H_trackDxyz -> Fill(thisDxyz_ele);

      // compatible track fraction
      eleTotTrack++;
      if (fabs(thisDz_ele) <0.6)  eleCompTrackZ++;
      if (fabs(thisDxy_ele)<0.35) eleCompTrackXY++;
      if (fabs(thisDxyz_ele)<0.6) eleCompTrackXYZ++;

    } //end loop over tracks	


    // computing the average track compatibility within a dR cone around the electron
    cout << "nTrack = " << nTrack << endl;
    for(int theTrack = 0; theTrack<nTrack; theTrack++){ 
      
      TVector3 trackPAtVtx(pxTrack[theTrack], pyTrack[theTrack], pzTrack[theTrack]);
      TVector3 posPAtVtx(pxEle[thePositron], pyEle[thePositron], pzEle[thePositron]);
      
      // usually low pt tracks are fakes
      float this_pt  = trackPAtVtx.Perp();
      if ( this_pt < 1. ) continue;
      cout << theTrack << ": ok pt" << endl;
      
      // does the track fall within the cone?
      double this_dr = posPAtVtx.DeltaR(trackPAtVtx);
      if ( fabs(this_dr) > 0.4 || fabs(this_dr) < 0.015 ) continue;
      cout << theTrack << ": ok dr" << endl;
      
      // computing dz and dxy
      float thisDz_pos   = eleTrackVzEle[thePositron] - vertexZTrack[theTrack];
      float thisDxy_pos  = sqrt((eleTrackVxEle[thePositron]-vertexXTrack[theTrack])+(eleTrackVyEle[thePositron]-vertexYTrack[theTrack]));
      float thisDxyz_pos = sqrt((eleTrackVxEle[thePositron]-vertexXTrack[theTrack])+(eleTrackVyEle[thePositron]-vertexYTrack[theTrack])+(eleTrackVzEle[thePositron]-vertexZTrack[theTrack]));
      
      H_trackDz   -> Fill(thisDz_pos);
      H_trackDxy  -> Fill(thisDxy_pos);
      H_trackDxyz -> Fill(thisDxyz_pos);

      // compatible track fraction
      posTotTrack++;
      if (fabs(thisDz_pos) <0.6)  posCompTrackZ++;
      if (fabs(thisDxy_pos)<0.35) posCompTrackXY++;
      if (fabs(thisDxyz_pos)<0.6) posCompTrackXYZ++;

    } //end loop over tracks	
    

    // filling histos / tree
    float compTrackZ   = posCompTrackZ   + eleCompTrackZ;
    float compTrackXY  = posCompTrackXY  + eleCompTrackXY;
    float compTrackXYZ = posCompTrackXYZ + eleCompTrackXYZ;
    float totTrack     = posTotTrack     + eleTotTrack;

    H_deltaZ    -> Fill(dz);
    H_deltaXY   -> Fill(dxy);
    H_deltaXYZ  -> Fill(dxyz);
    H_compZ     -> Fill(compTrackZ/totTrack);
    H_compXY    -> Fill(compTrackXY/totTrack);
    H_compXYZ   -> Fill(compTrackXYZ/totTrack);
    outRootTree -> fillAll(dz, dxy, dxyz);
    outRootTree -> store();
    
  } // end loop over entries


  // saving the histos
  outRootFile->cd();
  H_deltaZ    -> Write();   
  H_deltaXY   -> Write();
  H_deltaXYZ  -> Write();
  H_trackDz   -> Write();
  H_trackDxy  -> Write();
  H_trackDxyz -> Write();
  H_compZ     -> Write();
  H_compXY    -> Write();
  H_compXYZ   -> Write();
}


std::pair<int,int> HiggsVertexing::getBestElectronPair() {
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

void HiggsVertexing::setKinematics() {
  hardestElectronPt = TMath::Max(etEle[theElectron],etEle[thePositron]);
  slowestElectronPt = TMath::Min(etEle[theElectron],etEle[thePositron]);
  _p4ElectronMinus  -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
  _p4ElectronPlus   -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);      
  _mll = (*_p4ElectronMinus + *_p4ElectronPlus).M();
  _deltaPhi = fabs(180./TMath::Pi() * _p4ElectronMinus->Vect().DeltaPhi(_p4ElectronPlus->Vect()));
}

void HiggsVertexing::resetKinematics() {
  theElectron       = -1;
  thePositron       = -1;
  _mll              = 0;
  _deltaPhi         = 0;
  hardestElectronPt = 0;
  slowestElectronPt = 0;
  _p4ElectronMinus->SetXYZT(0,0,0,0);
  _p4ElectronPlus ->SetXYZT(0,0,0,0);
}

bool HiggsVertexing::isEleID(int eleIndex) {

  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  EgammaCutBasedID.SetHOverE( eleHoEEle[eleIndex] );
  EgammaCutBasedID.SetS9S25( s9s25Ele[eleIndex] );
  EgammaCutBasedID.SetDEta( eleDeltaEtaAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiIn( eleDeltaPhiAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiOut( eleDeltaPhiAtCaloEle[eleIndex] );
  EgammaCutBasedID.SetInvEminusInvP( 1./eleCaloCorrEEle[eleIndex]-1./eleTrackerPEle[eleIndex] );
  EgammaCutBasedID.SetBremFraction( fabs(eleTrackerPEle[eleIndex]-pTrkAtOuter.Mag())/eleTrackerPEle[eleIndex] );
  EgammaCutBasedID.SetSigmaEtaEta( sqrt(covEtaEtaEle[eleIndex]) );
  EgammaCutBasedID.SetSigmaPhiPhi( sqrt(covPhiPhiEle[eleIndex]) );
  EgammaCutBasedID.SetEOverPout( eleCorrEoPoutEle[eleIndex] );
  EgammaCutBasedID.SetEOverPin( eleCorrEoPEle[eleIndex] );
  EgammaCutBasedID.SetElectronClass ( eleClassEle[eleIndex] );
  EgammaCutBasedID.SetEgammaCutBasedID ( eleIdCutBasedEle[eleIndex] );
  EgammaCutBasedID.SetLikelihood( eleLikelihoodEle[eleIndex] );

  bool isIdentified = EgammaCutBasedID.output();
  
  return isIdentified;
}

float HiggsVertexing::getSecondEleTkPt(int first, int second, float deltaR) {
  
  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleTrackPt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr<deltaR ) secondEleTrackPt = eleTrackerPEle[second] * fabs( sin(thetaEle[second]) );

  return secondEleTrackPt;
}

float HiggsVertexing::getSecondEleEmEt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleEmEt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr<deltaR ) secondEleEmEt = eleFullCorrEEle[second] * fabs( sin(thetaEle[second]) );

  return secondEleEmEt;
}
