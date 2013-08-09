#include <string>
#include <iostream>

#include <TTree.h>
#include "TRandom.h"

#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/HiggsEleIdOptimToyMC.hh"

HiggsEleIdOptimToyMC::HiggsEleIdOptimToyMC(TTree *tree) 
  : HiggsBase(tree) {

  // to be changed (A only for likelihood studies):
  // A) signal or background
  // B) do you want toy MC?
  theToy = 0;

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
  passedIsol   = 0.;
  fullKine     = 0.;

  // 1 dimension histos - high pt electron
  HH_dEta   = new TH1F("HH_dEta",   "HH_dEta",   35,  0.0, 0.009);
  HH_dPhi   = new TH1F("HH_dPhi",   "HH_dPhi",   35,  0.0, 0.09);
  HH_HoE    = new TH1F("HH_HoE",    "HH_HoE",    35, -0.1, 0.12);
  HH_S9S25  = new TH1F("HH_S9s25",  "HH_S9S25",  35,  0.5, 1.0);
  HH_See    = new TH1F("HH_See",    "HH_See",    35,  0.0, 0.02);
  HH_EoPout = new TH1F("HH_EoPout", "HH_EoPout", 35,  0.5, 4.0);

  // 1 dimension histos - low pt electron
  HL_dEta   = new TH1F("HL_dEta",   "HL_dEta",   35,  0.0, 0.009);
  HL_dPhi   = new TH1F("HL_dPhi",   "HL_dPhi",   35,  0.0, 0.09);
  HL_HoE    = new TH1F("HL_HoE",    "HL_HoE",    35, -0.1, 0.12);
  HL_S9S25  = new TH1F("HL_S9s25",  "HL_S9S25",  35,  0.5, 1.0);
  HL_See    = new TH1F("HL_See",    "HL_See",    35,  0.0, 0.02);
  HL_EoPout = new TH1F("HL_EoPout", "HL_EoPout", 35,  0.5, 4.0);

  // likelihood studies - not divided by class
  HH_like   = new TH1F("HH_like",   "HH_like",   100, 0.0, 1.0);
  HL_like   = new TH1F("HL_like",   "HL_like",   100, 0.0, 1.0);

  // N dimensions histo
  theBins[0] = 35;
  theBins[1] = 35;
  theBins[2] = 35;
  theBins[3] = 35;
  theBins[4] = 35;
  theBins[5] = 35;
  // 
  theMin[0] =  0.0;
  theMin[1] =  0.0; 
  theMin[2] = -0.1; 
  theMin[3] =  0.5;
  theMin[4] =  0.0;
  theMin[5] =  0.5;
  //
  theMax[0] = 0.009;
  theMax[1] = 0.09; 
  theMax[2] = 0.12; 
  theMax[3] = 1.0;
  theMax[4] = 0.02;
  theMax[5] = 4.0;
  // 
  HH_NVarDim = new THnSparseF("HH_NVarDim", "HH_NVarDim", 6, theBins, theMin, theMax);
  HL_NVarDim = new THnSparseF("HL_NVarDim", "HL_NVarDim", 6, theBins, theMin, theMax);
}

HiggsEleIdOptimToyMC::~HiggsEleIdOptimToyMC(){
  
  // output txt file with efficiencies:
  *outTxtFile << "all events:    "      << allEvents    << endl;
  *outTxtFile << "passedMC:      "      << passedMc     << endl;
  *outTxtFile << "triggered:     "      << triggered    << endl;
  *outTxtFile << "common presel: "      << commonPresel << endl;
  *outTxtFile << "reco:          "      << passedReco   << endl;
  *outTxtFile << "presel x ele:  "      << elePresel    << endl;
  *outTxtFile << "loose eleID:   "      << looseId      << endl;
  *outTxtFile << "isolation:     "      << passedIsol   << endl;
  *outTxtFile << "full kinematics ok: " << fullKine     << endl; 
  
  // saving histos and tree
  outRootFile->Close();
  outRootTree->save();
  outLikeTree->save();

  // deleting
  delete _p4ElectronPlus;
  delete _p4ElectronMinus;
  delete _bestElectrons;
}

bool HiggsEleIdOptimToyMC::findMcTree(const char* processType) {

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
  else if(strcmp(processType,"Background")==0) {
    _process = "Background";
    return 1;
  }
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}


void HiggsEleIdOptimToyMC::Loop(const char *filename) {

  _verbose=false;
  if(fChain == 0) return;
  
  // output trees and root file and efficiencies
  char outfilename[200];
  sprintf(outfilename,"%s_MyOutTree.root",filename);
  outRootTree = new RedEleIDOptimTree(outfilename);
  sprintf(outfilename,"%s_MyLikeTree.root",filename);
  outLikeTree = new RedLikeOptimTree(outfilename);
  sprintf(outfilename,"%s_OutHistos.root",filename);
  outRootFile = new TFile(outfilename,"RECREATE");
  sprintf(outfilename,"%s_outputFile.txt",filename);
  outTxtFile = new ofstream(outfilename,ios::app);

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
    bool foundMcTree = findMcTree("Background");
    if ( !foundMcTree ) continue;
    passedMc=passedMc+theWeight;    
    
    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);
    if (!passedHLT) continue;
    triggered=triggered+theWeight;   

    // did we pass preselections?
    // bool is missing in summer08 ntuples, 
    // but preselections are anyway applied later on for electrons only... 
    // we can skip this part
    // if( !evtPresel ) continue;  
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
    if (hardestElectronPt<20) continue; 
    if (slowestElectronPt<10) continue; 
    if (etMet[0]<30)          continue; 
    if (_mll<12)              continue; 

    elePresel=elePresel+theWeight; 

    // did we pass the loose electronId?
    // we skip this cut: preselections (in the ntuple) already include a loose eleID
    // if (!eleIdCutBasedEle[theElectron] || !eleIdCutBasedEle[thePositron]) continue;
    looseId=looseId+theWeight;   
    
    // did we pass loose electron tracker based isolation? // dovrebbe essere sempre cosi' per nuove ntuple
    // we skip this cut. preselections (in the ntuple) already include a loose isolation
    // if (eleSumPt04Ele[theElectron]>0.1 || eleSumPt04Ele[thePositron]>0.1) continue;
    passedIsol=passedIsol+theWeight;

    // is this electron belonging to the good class? // old code, now always true since we separate in the optimization
    bool isEle = true;
    bool isPos = true;

    // ordering in pt
    bool isHigherEle = true; 
    if(etEle[theElectron] >= etEle[thePositron]) isHigherEle = true; 
    if(etEle[theElectron] < etEle[thePositron])  isHigherEle = false; 
    
    // filling histos: 1 dim and N dim histos
    if(isEle){
      double toFill[6];
      toFill[0] = fabs(deltaEtaAtVtxEle[theElectron]);
      toFill[1] = fabs(deltaPhiAtVtxEle[theElectron]);
      toFill[2] = hOverEEle[theElectron];
      toFill[3] = s9s25Ele[theElectron];
      toFill[4] = sqrt(covEtaEtaEle[theElectron]);
      toFill[5] = eSeedOverPoutEle[theElectron];

      // separating high and low pt
      if(isHigherEle){  	// high pt lepton
	HH_dEta    -> Fill(toFill[0],theWeight);
	HH_dPhi    -> Fill(toFill[1],theWeight);
	HH_HoE     -> Fill(toFill[2],theWeight);
	HH_S9S25   -> Fill(toFill[3],theWeight);
	HH_See     -> Fill(toFill[4],theWeight);
	HH_EoPout  -> Fill(toFill[5],theWeight);
	HH_NVarDim -> Fill(toFill,theWeight);
      }
      if(!isHigherEle){   	// low pt lepton
	HL_dEta    -> Fill(toFill[0],theWeight);
	HL_dPhi    -> Fill(toFill[1],theWeight);
	HL_HoE     -> Fill(toFill[2],theWeight);
	HL_S9S25   -> Fill(toFill[3],theWeight);
	HL_See     -> Fill(toFill[4],theWeight);
	HL_EoPout  -> Fill(toFill[5],theWeight);
	HL_NVarDim -> Fill(toFill,theWeight);
      }
    }
    if(isPos){
      double toFill[6];
      toFill[0] = fabs(deltaEtaAtVtxEle[thePositron]);
      toFill[1] = fabs(deltaPhiAtVtxEle[thePositron]);
      toFill[2] = hOverEEle[thePositron];
      toFill[3] = s9s25Ele[thePositron];
      toFill[4] = sqrt(covEtaEtaEle[thePositron]);
      toFill[5] = eSeedOverPoutEle[thePositron];

      // separating high and low pt
      if(!isHigherEle){       // high pt lepton
	HH_dEta    -> Fill(toFill[0],theWeight);
	HH_dPhi    -> Fill(toFill[1],theWeight);
	HH_HoE     -> Fill(toFill[2],theWeight);
	HH_S9S25   -> Fill(toFill[3],theWeight);
	HH_See     -> Fill(toFill[4],theWeight);
	HH_EoPout  -> Fill(toFill[5],theWeight);
	HH_NVarDim -> Fill(toFill,theWeight);
      }
      if(isHigherEle){	// low pt lepton
	HL_dEta    -> Fill(toFill[0],theWeight);
	HL_dPhi    -> Fill(toFill[1],theWeight);
	HL_HoE     -> Fill(toFill[2],theWeight);
	HL_S9S25   -> Fill(toFill[3],theWeight);
	HL_See     -> Fill(toFill[4],theWeight);
	HL_EoPout  -> Fill(toFill[5],theWeight);
	HL_NVarDim -> Fill(toFill,theWeight);
      }
    }

    // likelihood studies: filling the histos according the Pt
    if(isHigherEle){
      HH_like -> Fill(eleIdLikelihoodEle[theElectron],theWeight);
      HL_like -> Fill(eleIdLikelihoodEle[thePositron],theWeight);
      outLikeTree->fillAll(eleIdLikelihoodEle[theElectron], eleIdLikelihoodEle[thePositron], classificationEle[theElectron], classificationEle[thePositron]); 
    }
    if(!isHigherEle){
      HH_like -> Fill(eleIdLikelihoodEle[thePositron],theWeight);
      HL_like -> Fill(eleIdLikelihoodEle[theElectron],theWeight);
      outLikeTree->fillAll(eleIdLikelihoodEle[thePositron], eleIdLikelihoodEle[theElectron], classificationEle[thePositron], classificationEle[theElectron]); 
    }
    outLikeTree -> store();
    
    
    // in case toy generation is not needed
    if (!theToy) {
      if(isHigherEle){
	outRootTree->fillAll(fabs(deltaEtaAtVtxEle[theElectron]), fabs(deltaPhiAtVtxEle[theElectron]), hOverEEle[theElectron], s9s25Ele[theElectron], sqrt(covEtaEtaEle[theElectron]), eSeedOverPoutEle[theElectron], classificationEle[theElectron], fabs(deltaEtaAtVtxEle[thePositron]), fabs(deltaPhiAtVtxEle[thePositron]), hOverEEle[thePositron], s9s25Ele[thePositron], sqrt(covEtaEtaEle[thePositron]), eSeedOverPoutEle[thePositron], classificationEle[thePositron]);
      }
      if(!isHigherEle){
	outRootTree->fillAll(fabs(deltaEtaAtVtxEle[thePositron]), fabs(deltaPhiAtVtxEle[thePositron]), hOverEEle[thePositron], s9s25Ele[thePositron], sqrt(covEtaEtaEle[thePositron]), eSeedOverPoutEle[thePositron], classificationEle[thePositron], fabs(deltaEtaAtVtxEle[theElectron]), fabs(deltaPhiAtVtxEle[theElectron]), hOverEEle[theElectron], s9s25Ele[theElectron], sqrt(covEtaEtaEle[theElectron]), eSeedOverPoutEle[theElectron], classificationEle[theElectron]);
      }
      outRootTree -> store();
    }

    
    // full isolation still to be optimized at this level
    // here we use Xi's suggestion
    float eleAbsTkIso   = eleSumPt04Ele[theElectron]*etEle[theElectron];
    float posAbsTkIso   = eleSumPt04Ele[thePositron]*etEle[thePositron];
    float eleAbsEcalIso = eleIsoFromDepsEcalEle[theElectron];
    float posAbsEcalIso = eleIsoFromDepsEcalEle[thePositron];
    float eleGlobalIso  = eleAbsTkIso + eleAbsEcalIso;
    float posGlobalIso  = posAbsTkIso + posAbsEcalIso;

    if (etEle[theElectron]>=25){ 
      if (eleGlobalIso>5) continue; 
    }

    if (etEle[thePositron]>=25){ 
      if (posGlobalIso>5) continue; 
    }

    if (etEle[theElectron]<25){ 
      if ( eleGlobalIso > ((etEle[theElectron]-10.)/3.) ) continue; 
    }

    if (etEle[thePositron]<25){ 
      if ( posGlobalIso > ((etEle[thePositron]-10.)/3.) ) continue; 
    }

    
    // full kine analysis

    // in the analysis we'll consider different cases, but everything should help in removing ttbar.
    // we consider the best case, here: apply full jet veto
    if (goodJetFound())           continue;  
    
    // other kinematic selections depending on the mass 
    // these are the same cuts applied for csa07 note at 160 GeV; to be reoptimized, but should not be that different)
    if (etMet[0] < 40)          continue;
    if (etMet[0] > 85)          continue;
    if (_deltaPhi > 80)         continue;
    if (hardestElectronPt < 26) continue;
    if (hardestElectronPt > 66) continue;
    if (slowestElectronPt < 25) continue;
    if (_mll > 65)              continue;

    fullKine=fullKine+theWeight;

  } // end loop over entries
  



  // generating a random distribution according to the sampled one
  if (theToy) {
    
    TDatime *now = new TDatime();  
    for(int scan =0; scan<100000; scan++){
      if(scan%1000==0) cout << "random: " << scan << endl;
      
      // randomly generated electron and positron 
      int today = now->GetDate();  
      int clock = now->GetTime();  
      int seed  = today+clock+scan;
      gRandom->SetSeed(seed);
      double theRndHigh[6], theRndLow[6];
      HH_NVarDim->GetRandom(theRndHigh);
      HL_NVarDim->GetRandom(theRndLow);
      
      // filling the reduced tree with toy MC electrons
      outRootTree->fillAll(theRndHigh[0],theRndHigh[1],theRndHigh[2],theRndHigh[3],theRndHigh[4],theRndHigh[5],8000, theRndLow[0],theRndLow[1],theRndLow[2],theRndLow[3],theRndLow[4],theRndLow[5], 8000);
      outRootTree -> store();
      
    } // mc generation
    delete now;
  }



  // saving the histos
  outRootFile->cd();
  HH_dEta         -> Write();
  HH_dPhi         -> Write();
  HH_HoE          -> Write();
  HH_S9S25        -> Write();
  HH_See          -> Write();
  HH_EoPout       -> Write();
  HH_like         -> Write();
  HH_NVarDim      -> Write();
  HL_dEta         -> Write();
  HL_dPhi         -> Write();
  HL_HoE          -> Write();
  HL_S9S25        -> Write();
  HL_See          -> Write();
  HL_EoPout       -> Write();
  HL_like         -> Write();
  HL_NVarDim      -> Write();
}

std::pair<int,int> HiggsEleIdOptimToyMC::getBestElectronPair() {
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

void HiggsEleIdOptimToyMC::setKinematics() {
  hardestElectronPt = TMath::Max(etEle[theElectron],etEle[thePositron]);
  slowestElectronPt = TMath::Min(etEle[theElectron],etEle[thePositron]);
  _p4ElectronMinus -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
  _p4ElectronPlus  -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);      
  _mll = (*_p4ElectronMinus + *_p4ElectronPlus).M();
  _deltaPhi = fabs(180./TMath::Pi() * _p4ElectronMinus->Vect().DeltaPhi(_p4ElectronPlus->Vect()));
}

void HiggsEleIdOptimToyMC::resetKinematics() {
  theElectron = -1;
  thePositron = -1;
  _p4ElectronMinus->SetXYZT(0,0,0,0);
  _p4ElectronPlus->SetXYZT(0,0,0,0);
  _mll = 0;
  _deltaPhi = 0;
  hardestElectronPt = 0;
  slowestElectronPt = 0;
}


bool HiggsEleIdOptimToyMC::goodJetFound() {
  
  bool foundJet=false;
  
  for(int jj=0;jj<nSisConeCorrJet;jj++) {
    
    TVector3 p3Jet(pxSisConeCorrJet[jj],pySisConeCorrJet[jj],pzSisConeCorrJet[jj]);
    
    // check if the electron falls into the jet
    if (_p4ElectronMinus->Vect().Mag() != 0) {
      float deltaR = p3Jet.DeltaR(_p4ElectronMinus->Vect());
      if( deltaR<0.3 ) continue; 
    }

    // check if the positron falls into the jet
    if (_p4ElectronPlus->Vect().Mag() != 0) {
      float deltaR =  p3Jet.DeltaR(_p4ElectronPlus->Vect());
      if( deltaR<0.3 ) continue;
    }
    
    // checking the jet kinematics
    if( fabs(etaSisConeCorrJet[jj])>3.0 ) continue;  
    if( etSisConeCorrJet[jj]<35)          continue;

    // this is a good jet
    foundJet=true;
    break;
  }
  return foundJet;
}
