#include <string>
#include <iostream>

#include <TTree.h>

#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/HiggsIsolationStudiesInput.hh"

HiggsIsolationStudiesInput::HiggsIsolationStudiesInput(TTree *tree) 
  : HiggsBase(tree) {

  // to be changed : signal or background - later on
  
  // for full class dependent eleID
  char egamma[600];
  sprintf(egamma,"../EgammaAnalysisTools/config/newOptimEleId_looseOthers_m160/");
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

  // 1 dimension histos for 2nd pt electron
  char title[200];
  for (int etRange=0; etRange<5; etRange++){ 
    sprintf(title, "H_etEle[%d]",etRange); 
    H_etEle[etRange] = new TH1F(title, title, 50, 0., 100. );
    // 
    sprintf(title, "Hass_eleSumPt03[%d]",etRange); 
    Hass_eleSumPt03[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleSumPt04[%d]",etRange); 
    Hass_eleSumPt04[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleSumPt05[%d]",etRange); 
    Hass_eleSumPt05[etRange] = new TH1F(title, title, 500, 0., 30.);
    sprintf(title, "Hass_eleSumHadEt04[%d]",etRange); 
    Hass_eleSumHadEt04[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleSumHadEt05[%d]",etRange); 
    Hass_eleSumHadEt05[etRange] = new TH1F(title, title, 500, 0., 30.);
    sprintf(title, "Hass_eleSumEmEt04[%d]",etRange); 
    Hass_eleSumEmEt04[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleSumEmEt05[%d]",etRange); 
    Hass_eleSumEmEt05[etRange] = new TH1F(title, title, 500, 0., 30.);
    sprintf(title, "Hass_eleScBasedEcalSum04[%d]",etRange); 
    Hass_eleScBasedEcalSum04[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleScBasedEcalSum05[%d]",etRange); 
    Hass_eleScBasedEcalSum05[etRange] = new TH1F(title, title, 500, 0., 30.);
    sprintf(title, "Hass_eleScHaloBasedEcalSum04[%d]",etRange); 
    Hass_eleScHaloBasedEcalSum04[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleScHaloBasedEcalSum05[%d]",etRange); 
    Hass_eleScHaloBasedEcalSum05[etRange] = new TH1F(title, title, 500, 0., 30.);
    sprintf(title, "Hass_eleIsoFromDepsTk[%d]",etRange); 
    Hass_eleIsoFromDepsTk[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleIsoFromDepsEcal[%d]",etRange); 
    Hass_eleIsoFromDepsEcal[etRange] = new TH1F(title, title, 500, 0., 20.);
    sprintf(title, "Hass_eleIsoFromDepsHcal[%d]",etRange); 
    Hass_eleIsoFromDepsHcal[etRange] = new TH1F(title, title, 500, 0., 20.);
    //
    sprintf(title, "Hrel_eleSumPt03[%d]",etRange); 
    Hrel_eleSumPt03[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleSumPt04[%d]",etRange); 
    Hrel_eleSumPt04[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleSumPt05[%d]",etRange); 
    Hrel_eleSumPt05[etRange] = new TH1F(title, title, 500, 0., 10.);
    sprintf(title, "Hrel_eleSumHadEt04[%d]",etRange); 
    Hrel_eleSumHadEt04[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleSumHadEt05[%d]",etRange); 
    Hrel_eleSumHadEt05[etRange] = new TH1F(title, title, 500, 0., 10.);
    sprintf(title, "Hrel_eleSumEmEt04[%d]",etRange); 
    Hrel_eleSumEmEt04[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleSumEmEt05[%d]",etRange); 
    Hrel_eleSumEmEt05[etRange] = new TH1F(title, title, 500, 0., 10.);
    sprintf(title, "Hrel_eleScBasedEcalSum04[%d]",etRange); 
    Hrel_eleScBasedEcalSum04[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleScBasedEcalSum05[%d]",etRange); 
    Hrel_eleScBasedEcalSum05[etRange] = new TH1F(title, title, 500, 0., 10.);
    sprintf(title, "Hrel_eleScHaloBasedEcalSum04[%d]",etRange); 
    Hrel_eleScHaloBasedEcalSum04[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleScHaloBasedEcalSum05[%d]",etRange); 
    Hrel_eleScHaloBasedEcalSum05[etRange] = new TH1F(title, title, 500, 0., 10.);
    sprintf(title, "Hrel_eleIsoFromDepsTk[%d]",etRange); 
    Hrel_eleIsoFromDepsTk[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleIsoFromDepsEcal[%d]",etRange); 
    Hrel_eleIsoFromDepsEcal[etRange] = new TH1F(title, title, 500, 0., 5.);
    sprintf(title, "Hrel_eleIsoFromDepsHcal[%d]",etRange); 
    Hrel_eleIsoFromDepsHcal[etRange] = new TH1F(title, title, 500, 0., 5.);
  }
}

HiggsIsolationStudiesInput::~HiggsIsolationStudiesInput(){
  
  // output txt file with efficiencies:
  *outTxtFile << "all events:    "      << allEvents    << endl;
  *outTxtFile << "passedMC:      "      << passedMc     << endl;
  *outTxtFile << "triggered:     "      << triggered    << endl;
  *outTxtFile << "common presel: "      << commonPresel << endl;
  *outTxtFile << "reco:          "      << passedReco   << endl;
  *outTxtFile << "presel x ele:  "      << elePresel    << endl;
  *outTxtFile << "loose eleID:   "      << looseId      << endl;
  *outTxtFile << "loose isolation: "    << looseIsol    << endl;
  *outTxtFile << "full eleID:   "       << fullId       << endl;
  
  // saving histos file
  outRootFile->Close();

  // deleting
  delete _p4ElectronPlus;
  delete _p4ElectronMinus;
  delete _bestElectrons;
}

bool HiggsIsolationStudiesInput::findMcTree(const char* processType) {
  
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
    return 1;
  }
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}


void HiggsIsolationStudiesInput::Loop(const char *filename) {

  _verbose=false;
  if(fChain == 0) return;
  
  // output root files
  char outfilename[200];
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
    bool foundMcTree = findMcTree("Wjets");
    if ( !foundMcTree ) continue;
    passedMc=passedMc+theWeight;    

    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);
    if (!passedHLT) continue; 
    triggered=triggered+theWeight;   

    // offline ambiguity resolution
    _resolvedElectrons = resolvedElectrons();

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
    // if (hardestElectronPt < 20) continue;     
    // if (slowestElectronPt < 10) continue; 
    // if (etMet[0] < 30)          continue; 
    // if (_mll < 12)              continue; 
    elePresel=elePresel+theWeight; 

    // did we pass the loose electronId?
    // if (!eleIdCutBasedEle[theElectron] || !eleIdCutBasedEle[thePositron]) continue;
    looseId=looseId+theWeight;   
    
    // did we pass loose electron tracker based isolation?
    // if (eleSumPt04Ele[theElectron]>0.1 || eleSumPt04Ele[thePositron]>0.1) continue;
    looseIsol=looseIsol+theWeight;

    // did we pass the full class dependent electronId?  
    // bool theElectronID = isEleID(theElectron);
    // bool thePositronID = isEleID(thePositron);
    // if (!theElectronID || !thePositronID) continue;
    fullId=fullId+theWeight;   

    // ordering the two electrons in pt
    int theLowestPt = -1;
    if(etEle[theElectron] >= etEle[thePositron]) theLowestPt = thePositron;
    if(etEle[theElectron] < etEle[thePositron])  theLowestPt = theElectron;


    // filling histos for the lowest Et electron

    // computing absolute quantities when we have relative in the ntuple
    float ass_eleSumPt03Ele    = eleSumPt03Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumPt04Ele    = eleSumPt04Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumPt05Ele    = eleSumPt05Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumHadEt04Ele = eleSumHadEt04Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumHadEt05Ele = eleSumHadEt05Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumEmEt04Ele  = eleSumEmEt04Ele[theLowestPt]*etEle[theLowestPt];
    float ass_eleSumEmEt05Ele  = eleSumEmEt05Ele[theLowestPt]*etEle[theLowestPt];

    // computing relative quantities when we have absolute in the ntuple    
    float rel_eleScBasedEcalSum04Ele     = eleScBasedEcalSum04Ele[theLowestPt]/etEle[theLowestPt];
    float rel_eleScBasedEcalSum05Ele     = eleScBasedEcalSum05Ele[theLowestPt]/etEle[theLowestPt];
    float rel_eleScHaloBasedEcalSum04Ele = eleScHaloBasedEcalSum04Ele[theLowestPt]/etEle[theLowestPt];
    float rel_eleScHaloBasedEcalSum05Ele = eleScHaloBasedEcalSum05Ele[theLowestPt]/etEle[theLowestPt];
    float rel_eleIsoFromDepsTkEle        = eleIsoFromDepsTkEle[theLowestPt]/etEle[theLowestPt];
    float rel_eleIsoFromDepsEcalEle      = eleIsoFromDepsEcalEle[theLowestPt]/etEle[theLowestPt];
    float rel_eleIsoFromDepsHcalEle      = eleIsoFromDepsHcalEle[theLowestPt]/etEle[theLowestPt];

    for (int etRange=0; etRange<5; etRange++){
      
      bool continuo = true;                                                // 0 = all; 
      /*
      if (etRange==1 && etEle[theLowestPt]>25)  continuo = false;          // 1 = pT < 25; 2 = pT > 25
      if (etRange==2 && etEle[theLowestPt]<=25) continuo = false;
      if (etRange==3 && fabs(etaEle[theLowestPt])>1.5)  continuo = false;  // 3 = barrel, 4 = endcap
      if (etRange==4 && fabs(etaEle[theLowestPt])<=1.5) continuo = false; */
      if (etRange==0 && etEle[theLowestPt]<10) continuo = false;          
      if (etRange==1 && etEle[theLowestPt]<15) continuo = false;          
      if (etRange==2 && etEle[theLowestPt]<20) continuo = false;          
      if (etRange==3 && etEle[theLowestPt]<25) continuo = false;          
      if (etRange==4 && etEle[theLowestPt]<30) continuo = false;          
      if (!continuo) continue;
      
      H_etEle[etRange] -> Fill(etEle[theLowestPt]);

      Hrel_eleSumPt03[etRange]    -> Fill(eleSumPt03Ele[theLowestPt]);
      Hrel_eleSumPt04[etRange]    -> Fill(eleSumPt04Ele[theLowestPt]);
      Hrel_eleSumPt05[etRange]    -> Fill(eleSumPt05Ele[theLowestPt]);
      Hrel_eleSumHadEt04[etRange] -> Fill(eleSumHadEt04Ele[theLowestPt]);
      Hrel_eleSumHadEt05[etRange] -> Fill(eleSumHadEt05Ele[theLowestPt]);
      Hrel_eleSumEmEt04[etRange]  -> Fill(eleSumEmEt04Ele[theLowestPt]);
      Hrel_eleSumEmEt05[etRange]  -> Fill(eleSumEmEt05Ele[theLowestPt]);
      
      Hass_eleSumPt03[etRange]    -> Fill(ass_eleSumPt03Ele);
      Hass_eleSumPt04[etRange]    -> Fill(ass_eleSumPt04Ele);
      Hass_eleSumPt05[etRange]    -> Fill(ass_eleSumPt05Ele);
      Hass_eleSumHadEt04[etRange] -> Fill(ass_eleSumHadEt04Ele);
      Hass_eleSumHadEt05[etRange] -> Fill(ass_eleSumHadEt05Ele);
      Hass_eleSumEmEt04[etRange]  -> Fill(ass_eleSumEmEt04Ele);
      Hass_eleSumEmEt05[etRange]  -> Fill(ass_eleSumEmEt05Ele);
      
      Hass_eleScBasedEcalSum04[etRange]     -> Fill(eleScBasedEcalSum04Ele[theLowestPt]);
      Hass_eleScBasedEcalSum05[etRange]     -> Fill(eleScBasedEcalSum05Ele[theLowestPt]);
      Hass_eleScHaloBasedEcalSum04[etRange] -> Fill(eleScHaloBasedEcalSum04Ele[theLowestPt]);
      Hass_eleScHaloBasedEcalSum05[etRange] -> Fill(eleScHaloBasedEcalSum05Ele[theLowestPt]);
      Hass_eleIsoFromDepsTk[etRange]        -> Fill(eleIsoFromDepsTkEle[theLowestPt]);
      Hass_eleIsoFromDepsHcal[etRange]      -> Fill(eleIsoFromDepsHcalEle[theLowestPt]);
      Hass_eleIsoFromDepsEcal[etRange]      -> Fill(eleIsoFromDepsEcalEle[theLowestPt]);	
      
      Hrel_eleScBasedEcalSum04[etRange]     -> Fill(rel_eleScBasedEcalSum04Ele);
      Hrel_eleScBasedEcalSum05[etRange]     -> Fill(rel_eleScBasedEcalSum05Ele);
      Hrel_eleScHaloBasedEcalSum04[etRange] -> Fill(rel_eleScHaloBasedEcalSum04Ele);
      Hrel_eleScHaloBasedEcalSum05[etRange] -> Fill(rel_eleScHaloBasedEcalSum05Ele);
      Hrel_eleIsoFromDepsTk[etRange]        -> Fill(rel_eleIsoFromDepsTkEle);
      Hrel_eleIsoFromDepsHcal[etRange]      -> Fill(rel_eleIsoFromDepsHcalEle);
      Hrel_eleIsoFromDepsEcal[etRange]      -> Fill(rel_eleIsoFromDepsEcalEle);	
    }
    
  } // end loop over entries

  // saving the histos
  outRootFile->cd();
  for (int etRange=0; etRange<5; etRange++){
    H_etEle[etRange]                      -> Write();
    Hass_eleSumPt03[etRange]              -> Write();	
    Hass_eleSumPt04[etRange]              -> Write();	
    Hass_eleSumPt05[etRange]              -> Write(); 
    Hass_eleSumHadEt04[etRange]           -> Write(); 
    Hass_eleSumHadEt05[etRange]           -> Write(); 
    Hass_eleSumEmEt04[etRange]            -> Write();	 
    Hass_eleSumEmEt05[etRange]            -> Write();	 
    Hass_eleScBasedEcalSum04[etRange]     -> Write();  
    Hass_eleScBasedEcalSum05[etRange]     -> Write();  
    Hass_eleScHaloBasedEcalSum04[etRange] -> Write();  
    Hass_eleScHaloBasedEcalSum05[etRange] -> Write();  
    Hass_eleIsoFromDepsTk[etRange]        -> Write();  
    Hass_eleIsoFromDepsHcal[etRange]      -> Write(); 
    Hass_eleIsoFromDepsEcal[etRange]      -> Write();    
    Hrel_eleSumPt03[etRange]              -> Write();	
    Hrel_eleSumPt04[etRange]              -> Write();	
    Hrel_eleSumPt05[etRange]              -> Write(); 
    Hrel_eleSumHadEt04[etRange]           -> Write(); 
    Hrel_eleSumHadEt05[etRange]           -> Write(); 
    Hrel_eleSumEmEt04[etRange]            -> Write();	 
    Hrel_eleSumEmEt05[etRange]            -> Write();	 
    Hrel_eleScBasedEcalSum04[etRange]     -> Write();  
    Hrel_eleScBasedEcalSum05[etRange]     -> Write();  
    Hrel_eleScHaloBasedEcalSum04[etRange] -> Write();  
    Hrel_eleScHaloBasedEcalSum05[etRange] -> Write();  
    Hrel_eleIsoFromDepsTk[etRange]        -> Write();  
    Hrel_eleIsoFromDepsHcal[etRange]      -> Write(); 
    Hrel_eleIsoFromDepsEcal[etRange]      -> Write();
  }
}

std::pair<int,int> HiggsIsolationStudiesInput::getBestElectronPair() {

  int theLep1=-1;          
  int theLep2=-1;
  float maxPtLep1=-1000.;  
  float maxPtLep2=-1000.;
  
  int resEleSize = _resolvedElectrons.size();

  std::vector<int> goodRecoLeptons;

  for(int i=0;i<resEleSize;i++) {
    int theEle = _resolvedElectrons[i];
    TVector3 pLepton(pxEle[theEle],pyEle[theEle],pzEle[theEle]);
    float thisPt=pLepton.Pt();
    if(fabs(etaEle[theEle])>2.5) continue;
    if(thisPt<10)               continue;
    float thisCharge = chargeEle[theEle];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = theEle; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = theEle; }
  }
  
  _bestElectrons->clear();
  _bestElectrons->push_back(theLep1);  
  _bestElectrons->push_back(theLep2); 
  return make_pair(theLep1,theLep2);
}

void HiggsIsolationStudiesInput::setKinematics() {
  hardestElectronPt = TMath::Max(etEle[theElectron],etEle[thePositron]);
  slowestElectronPt = TMath::Min(etEle[theElectron],etEle[thePositron]);
  _p4ElectronMinus -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
  _p4ElectronPlus  -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);  
  _mll = (*_p4ElectronMinus + *_p4ElectronPlus).M();
}

void HiggsIsolationStudiesInput::resetKinematics() {
  theElectron = -1;
  thePositron = -1;
  _p4ElectronMinus->SetXYZT(0,0,0,0);
  _p4ElectronPlus->SetXYZT(0,0,0,0);
  _mll = 0;
  hardestElectronPt = 0;
  slowestElectronPt = 0;
}

bool HiggsIsolationStudiesInput::isEleID(int eleIndex) {

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

vector<int> HiggsIsolationStudiesInput::resolvedElectrons() {

  float drmin=0.1;
  vector<int> amb_index;
  vector<int> resolvedEles;
  vector<std::pair<int, int> > ambEle;
  
  ambEle.clear();
  
  if(nEle==1) {
    resolvedEles.push_back(0);
    return resolvedEles;
  }
  
  // first electron
  for(int i=0;i<nEle;i++) {
    TVector3 pEle1(pxEle[i],pyEle[i],pzEle[i]);
    int no_ambiguity=0;
    // second electron
    for(int j=i+1;j<nEle;j++) {
      TVector3 pEle2(pxEle[j],pyEle[j],pzEle[j]);
      float dr = pEle1.DeltaR(pEle2);
      if(dr<drmin) {
        amb_index.push_back(j);
        ambEle.push_back(std::make_pair(i,j));
      } else no_ambiguity++;
    }
    bool test=true;
    for (int t=0; t<amb_index.size(); t++){
      if (amb_index[t] == i ) test=false;
    }
    
    if(no_ambiguity == (nEle-1-i) && test == true) {
      resolvedEles.push_back(i);
    }
  }
  
  // resolve ambiguities
  std::vector< pair<int,int> >::const_iterator it;
  for(it=ambEle.begin(); it<ambEle.end(); it++) {
    int bestEleId=(int)it->first;
    int multiAmbEleId=(int)it->first;
    while((int)it->first==multiAmbEleId && it<ambEle.end()) {
      int bestEle = bestEleId;
      int compEle = it->second;
      if(fabs(eSuperClusterOverPEle[compEle]-1) <= fabs(eSuperClusterOverPEle[bestEle]-1)) bestEleId=it->second;
      it++;
    }
    resolvedEles.push_back(bestEleId);
  }
  
  return resolvedEles;
}

