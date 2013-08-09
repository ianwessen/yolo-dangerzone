#include <iostream>
#include <string>
#include <cmath>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "HiggsAnalysisTools/include/WSelection.hh"

WSelection::WSelection(TTree *tree) 
  : HiggsBase(tree) {

  // EleID: class dependent or not
  bool classDepID = false;

  // Initialize selection
  std::string fileCuts("config/wenu/wenuCuts.txt");
  std::string fileSwitches("config/wenu/wenuSwitches.txt");
  _selection = new Selection(fileCuts,fileSwitches);

  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addSwitch("jetVeto");

  _selection->addCut("nRecoEle");
  _selection->addCut("etaEleAcc");
  _selection->addCut("ptEleAcc");
  _selection->addCut("trackerPtSum");
  _selection->addCut("ptEle");
  _selection->addCut("MET");
  _selection->addCut("ptSystem");
  _selection->addCut("jetConeWidth");
  _selection->addCut("etaJetAcc");
  _selection->addCut("etJetAcc");

  Selection *_goldenSelectionEB, *_bigbremSelectionEB, *_narrowSelectionEB, *_showeringSelectionEB; 
  Selection *_goldenSelectionEE, *_bigbremSelectionEE, *_narrowSelectionEE, *_showeringSelectionEE; 
  
  if (classDepID){
    _goldenSelectionEB    = new Selection("config/wenu/electronIDGoldenCuts.txt",     "config/wenu/electronIDSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/wenu/electronIDBigBremCuts.txt",    "config/wenu/electronIDSwitches.txt");
    _narrowSelectionEB    = new Selection("config/wenu/electronIDNarrowCuts.txt",     "config/wenu/electronIDSwitches.txt");
    _showeringSelectionEB = new Selection("config/wenu/electronIDShoweringCuts.txt",  "config/wenu/electronIDSwitches.txt");
    _goldenSelectionEE    = new Selection("config/wenu/electronIDGoldenCutsEE.txt",   "config/wenu/electronIDSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/wenu/electronIDBigBremCutsEE.txt",  "config/wenu/electronIDSwitches.txt");
    _narrowSelectionEE    = new Selection("config/wenu/electronIDNarrowCutsEE.txt",   "config/wenu/electronIDSwitches.txt");
    _showeringSelectionEE = new Selection("config/wenu/electronIDShoweringCutsEE.txt","config/wenu/electronIDSwitches.txt");
  }
  if (!classDepID){
    _goldenSelectionEB    = new Selection("config/wenu/electronIDnoClassCutsEB.txt",  "config/wenu/electronIDSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/wenu/electronIDnoClassCutsEB.txt",  "config/wenu/electronIDSwitches.txt");
    _narrowSelectionEB    = new Selection("config/wenu/electronIDnoClassCutsEB.txt",  "config/wenu/electronIDSwitches.txt");
    _showeringSelectionEB = new Selection("config/wenu/electronIDnoClassCutsEB.txt",  "config/wenu/electronIDSwitches.txt");
    _goldenSelectionEE    = new Selection("config/wenu/electronIDnoClassCutsEE.txt",  "config/wenu/electronIDSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/wenu/electronIDnoClassCutsEE.txt",  "config/wenu/electronIDSwitches.txt");
    _narrowSelectionEE    = new Selection("config/wenu/electronIDnoClassCutsEE.txt",  "config/wenu/electronIDSwitches.txt");
    _showeringSelectionEE = new Selection("config/wenu/electronIDnoClassCutsEE.txt",  "config/wenu/electronIDSwitches.txt");
  }

  _electronSelection.push_back(_goldenSelectionEB);
  _electronSelection.push_back(_bigbremSelectionEB);
  _electronSelection.push_back(_narrowSelectionEB);
  _electronSelection.push_back(_showeringSelectionEB);
  _electronSelection.push_back(_goldenSelectionEE);
  _electronSelection.push_back(_bigbremSelectionEE);
  _electronSelection.push_back(_narrowSelectionEE);
  _electronSelection.push_back(_showeringSelectionEE);

  std::vector<Selection*>::iterator selectionItr;
  for(selectionItr=_electronSelection.begin();selectionItr!=_electronSelection.end();selectionItr++) {
    Selection *eleSelection = *selectionItr;
    eleSelection->addCut("hOverE");
    eleSelection->addCut("s9s25");
    eleSelection->addCut("deta");
    eleSelection->addCut("dphiIn");
    eleSelection->addCut("dphiOut");
    eleSelection->addCut("invEMinusInvP");
    eleSelection->addCut("covEtaEta");
    eleSelection->addCut("eOverPout");
    eleSelection->addCut("eOverPin");
    eleSelection->addCut("Fisher");
    eleSelection->addCut("Likelihood");
    eleSelection->summary();
  }

  std::map<std::string,std::pair<float,float> > selectionMap = _selection->getSelection();
  _selection->summary();

  // Initialize additional counters...
  _counter.SetTitle("EVENT COUNTER");
  _counter.AddVar("event");
  _counter.AddVar("MCtruth");
  _counter.AddVar("trigger");
  _counter.AddVar("nRecoEle");
  _counter.AddVar("oneGoodRec");
  _counter.AddVar("eleID");
  _counter.AddVar("trackerIsol");
  _counter.AddVar("ptEle");
  _counter.AddVar("MET");
  _counter.AddVar("ptSystem");
  _counter.AddVar("jetVeto");
  _counter.AddVar("final");
  
  _eleCounter.SetTitle("SINGLE ELECTRON COUNTER");
  _eleCounter.AddVar("electrons");
  _eleCounter.AddVar("hOverE");
  _eleCounter.AddVar("s9s25");
  _eleCounter.AddVar("deta");
  _eleCounter.AddVar("invEMinusInvP");
  _eleCounter.AddVar("dphiIn");
  _eleCounter.AddVar("dphiOut");
  _eleCounter.AddVar("covEtaEta");
  _eleCounter.AddVar("eOverPout");
  _eleCounter.AddVar("eOverPin");
  _eleCounter.AddVar("Fisher");
  _eleCounter.AddVar("Likelihood");
  _eleCounter.AddVar("finalEleID");


  // the kinematic vectors
  _p3Ele    = new TVector3(0.,0.,0.);
  _p3Met    = new TVector3(0.,0.,0.);
  _p3System = new TVector3(0.,0.,0.);
  _p4Ele    = new TLorentzVector(0.,0.,0.,0.);


  // the Monitoring Histograms
   _monitorGenerator           = new Monitor(0);
   _monitorEventAfterSelection = new Monitor(0);
   _monitorEventAfterReco      = new Monitor(0);
   _monitorMet                 = new Monitor(&nMet);
   _bestElectrons              = new std::vector<int>;
   _monitorElectrons           = new Monitor(&nEle,_bestElectrons);
   _bestJets                   = new std::vector<int>;
   _monitorJets                = new Monitor(&nJet,_bestJets);
   _bestGenJets                = new std::vector<int>;
   _monitorGenJets             = new Monitor(&nGenJet,_bestGenJets);

}

WSelection::~WSelection(){
  delete _p3Ele;
  delete _p3Met;
  delete _p3System;
  delete _p4Ele;
  delete _monitorGenerator;
  delete _monitorEventAfterSelection;
  delete _monitorEventAfterReco;
  delete _monitorMet;
  delete _bestElectrons;
  delete _monitorElectrons;
  delete _bestJets;
  delete _bestGenJets;
  delete _monitorGenJets;
  delete _monitorJets;
  delete _selection;
}


bool WSelection::findMcTree(const char* processType) {

  // basic trigger studies preselections - see HLT note
  //if( (pMc[7]*fabs(sin(thetaMc[7])) < 5) || (fabs(etaMc[7])>2.5) ) { return false; }

  // all - barrel - endcap
  if(strcmp(processType,"Wtoenu")==0)       { return ( (abs(idMc[6])==24) && (abs(idMc[7])==11) ); }
  if(strcmp(processType,"WtoenuBarrel")==0) { return ( (abs(idMc[6])==24) && (abs(idMc[7])==11) && fabs(etaMc[7])<1.476 ); }
  if(strcmp(processType,"WtoenuEndcap")==0) { return ( (abs(idMc[6])==24) && (abs(idMc[7])==11) && fabs(etaMc[7])>1.476 ); }
  if( (strcmp(processType,"Wtoenu") !=0) && (strcmp(processType,"WtoenuBarrel") !=0) && (strcmp(processType,"WtoenuEndcap") !=0) ){
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}

void WSelection::Loop() {
  _verbose=true;
  if(fChain == 0) return;
  
  bookHistos();
  TFile *file = new TFile("histograms.root","RECREATE");
  _monitorGenerator->setPath("Generator");
  _monitorMet->setPath("MET");
  _monitorElectrons->setPath("Electrons");
  _monitorJets->setPath("Jets");
  _monitorGenJets->setPath("GenJets");
  _monitorEventAfterReco->setPath("EventAfterReco");
  _monitorEventAfterSelection->setPath("EventAfterSelection");

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  if(_verbose) std::cout << "Number of entries = " << nentries << std::endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      _counter.IncrVar("event");
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (_verbose && jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

      if(_selection->getSwitch("MCtruth") && !findMcTree("Wtoenu") ) continue; 
      _counter.IncrVar("MCtruth");
      _monitorGenerator->Fill();

      if(_selection->getSwitch("trigger") && !(singleElePassedTrg) && !(singleEleRelaxPassedTrg) ) continue;    
      _counter.IncrVar("trigger"); 

      if(_selection->getSwitch("nRecoEle") && !_selection->passCut("nRecoEle",nEle)) continue;   
      _counter.IncrVar("nRecoEle");

      int theEle = getBestLepton();
      if(theEle<0) continue; 
      _counter.IncrVar("oneGoodRec");

      setKinematics(theEle);

      addVariables();

      if(!isEleID(theEle)) continue; 
      _counter.IncrVar("eleID");
      _monitorEventAfterReco->Fill();

      if(_selection->getSwitch("trackerPtSum") && (!_selection->passCut("trackerPtSum",eleSumPt04Ele[theEle])) ) continue;
      _counter.IncrVar("trackerIsol");

      if(_selection->getSwitch("ptEle") && !_selection->passCut("ptEle",_elePt)) continue; 
      _counter.IncrVar("ptEle");

      if(_selection->getSwitch("MET") && !_selection->passCut("MET",etMet[0])) continue; 
      _counter.IncrVar("MET");

      if(_selection->getSwitch("ptSystem") && !_selection->passCut("ptSystem",_systemPt)) continue; 
      _counter.IncrVar("ptSystem");

      if(_selection->getSwitch("jetVeto") && jetVeto()) continue; 
      _counter.IncrVar("jetVeto");

      _counter.IncrVar("final");

      _monitorEventAfterSelection->Fill();
      _monitorMet->Fill();
      _monitorElectrons->Fill();
      _monitorJets->Fill();
      _monitorGenJets->Fill();
  }

  _monitorGenerator->WritePs("eventGenerator.ps");
  _monitorGenerator->WriteRoot(file);
  _monitorEventAfterReco->WritePs("eventAfterReco.ps");
  _monitorEventAfterReco->WriteRoot(file);
  _monitorEventAfterSelection->WritePs("eventAfterSelection.ps");
  _monitorEventAfterSelection->WriteRoot(file);
  _monitorMet->WritePs("met.ps");
  _monitorMet->WriteRoot(file);
  _monitorElectrons->WritePs("electrons.ps");
  _monitorElectrons->WriteRoot(file);
  _monitorJets->WritePs("jets.ps");
  _monitorJets->WriteRoot(file);
  _monitorGenJets->WritePs("jets.ps");
  _monitorGenJets->WriteRoot(file);
  file->Close();
}

void WSelection::displayEfficiencies() {
  _eleCounter.Draw();
  _eleCounter.Draw("hOverE","electrons");
  _eleCounter.Draw("s9s25","hOverE");
  _eleCounter.Draw("deta","s9s25");
  _eleCounter.Draw("dphiIn","deta");
  _eleCounter.Draw("dphiOut","dphiIn");
  _eleCounter.Draw("invEMinusInvP","dphiOut");
  _eleCounter.Draw("covEtaEta","invEMinusInvP");
  _eleCounter.Draw("eOverPout","covEtaEta");
  _eleCounter.Draw("eOverPin", "eOverPout");
  _eleCounter.Draw("Fisher","eOverPin");
  _eleCounter.Draw("Likelihood","Fisher");
  _eleCounter.Draw("finalEleID","electrons");

  _counter.Draw();
  _counter.Draw("MCtruth","event");
  _counter.Draw("trigger","MCtruth");
  _counter.Draw("nRecoEle","trigger");
  _counter.Draw("oneGoodRec","nRecoEle");
  _counter.Draw("eleID","oneGoodRec");
  _counter.Draw("trackerIsol","eleID");
  _counter.Draw("ptEle","trackerIsol");
  _counter.Draw("MET","ptEle");
  _counter.Draw("ptSystem","MET");
  _counter.Draw("jetVeto","ptSystem");
  _counter.Draw("final","MCtruth");
}


int WSelection::getBestLepton() {
  int theEle=-1;
  float ptEle=-1000.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nEle;i++) {
    if(_selection->getSwitch("etaEleAcc") && !_selection->passCut("etaEleAcc",etaEle[i]) ) continue;
    TVector3 pEle(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pEle.Pt();
    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",thisPt) ) continue;
    if (thisPt> ptEle){ ptEle = thisPt; theEle = i; }
  }
  return theEle;
}


bool WSelection::isEleID(int eleIndex) {

  int GsfClass = classificationEle[eleIndex];
  int offset=0;
  if(GsfClass>=100) {
    GsfClass-=100;
    offset=4;
  }

  Selection *selection;
  if(GsfClass==0) 
    selection=_electronSelection[offset];
  else if(GsfClass==10)
    selection=_electronSelection[offset+1];
  else if(GsfClass==20)
    selection=_electronSelection[offset+2];
  else if(GsfClass>=30 && GsfClass<=40) {
    selection=_electronSelection[offset+3];
  }

  TVector3 pTrkAtInner(pxAtInnerEle[eleIndex],pyAtInnerEle[eleIndex],pzAtInnerEle[eleIndex]);
  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  _eleCounter.IncrVar("electrons");
  if(selection->getSwitch("hOverE")     && !selection->passCut("hOverE",hOverEEle[eleIndex]))             return false; _eleCounter.IncrVar("hOverE");
  if(selection->getSwitch("s9s25")      && !selection->passCut("s9s25",s9s25Ele[eleIndex]))               return false; _eleCounter.IncrVar("s9s25");
  if(selection->getSwitch("deta")       && !selection->passCut("deta",deltaEtaAtVtxEle[eleIndex]))     return false; _eleCounter.IncrVar("deta");
  if(selection->getSwitch("dphiIn")     && !selection->passCut("dphiIn",deltaPhiAtVtxEle[eleIndex]))   return false; _eleCounter.IncrVar("dphiIn");
  if(selection->getSwitch("dphiOut")    && !selection->passCut("dphiOut",deltaPhiAtCaloEle[eleIndex])) return false; _eleCounter.IncrVar("dphiOut");
  if(selection->getSwitch("invEMinusInvP") && !selection->passCut("invEMinusInvP",1./ecalEle[eleIndex]-1./pTrkAtInner.Mag())) return false; _eleCounter.IncrVar("invEMinusInvP");
  if(selection->getSwitch("covEtaEta")  && !selection->passCut("covEtaEta",covEtaEtaEle[eleIndex]))       return false; _eleCounter.IncrVar("covEtaEta");
  if(selection->getSwitch("eOverPout")  && !selection->passCut("eOverPout",eSeedOverPoutEle[eleIndex]))   return false; _eleCounter.IncrVar("eOverPout");
  if(selection->getSwitch("eOverPin")   && !selection->passCut("eOverPin", eSuperClusterOverPEle[eleIndex]))      return false; _eleCounter.IncrVar("eOverPin");
  if(selection->getSwitch("Fisher")     && !selection->passCut("Fisher",Fisher(eleIndex)))                return false; _eleCounter.IncrVar("Fisher");
  if(selection->getSwitch("Likelihood") && !selection->passCut("Likelihood",eleIdLikelihoodEle[eleIndex]))  return false; _eleCounter.IncrVar("Likelihood");
  _eleCounter.IncrVar("finalEleID");

  return true;
}

void WSelection::addVariables() {

  for(int i=0;i<nEle;i++) {
    _eOverP[i]=eSuperClusterOverPEle[i];   
  }

  int theHighestPtEle=0;
  float maxElePt     =-100;
  for(int i=0;i<nEle;i++) {
    TVector3 p3Ele(pxEle[i],pyEle[i],pzEle[i]);
    float thisElePt=p3Ele.Pt();
    if(thisElePt>maxElePt) {
      maxElePt=thisElePt;
      theHighestPtEle=i;
    }
  }
  _histopt[0]=maxElePt;
  _nEle[0]=(float)nEle;


  int theHighestPtJet=0;
  float maxJetPt     =-100;
  for(int i=0;i<nJet;i++) {
    TVector3 p3Jet(pxJet[i],pyJet[i],pzJet[i]);
    _drEleFromHighestPt[i]=_p3Ele->Angle(p3Jet);
    if(etJet[i]>maxJetPt) {
      maxJetPt=etJet[i];
      theHighestPtJet=i;
    }
  }
  
  _bestJets->clear();
  _bestJets->push_back(theHighestPtJet);
  _nJet[0]=(float)nJet;

  
  // generator level  
  _elePtGen[0]=pMc[11]*fabs(sin(thetaMc[11]));

  theHighestPtJet=0;
  maxJetPt=-100;
  for(int i=0;i<nJet;i++) {
    TVector3 p3GenJet(pxGenJet[i],pyGenJet[i],pzGenJet[i]);
    if(etGenJet[i]>maxJetPt) {
      maxJetPt=etGenJet[i];
      theHighestPtJet=i;
    }
  }
  
  _bestGenJets->clear();
  _bestGenJets->push_back(theHighestPtJet);
  _nGenJet[0]=(float)nGenJet;
  
}

void WSelection::setKinematics(int theEle) {

  // three-vectors
  _p3Ele->SetXYZ(pxEle[theEle],pyEle[theEle],pzEle[theEle]);
  _p3Met->SetXYZ(pxMet[0],pyMet[0],pzMet[0]);
  _p3System->SetXYZ(pxEle[theEle]+pxMet[0],pyEle[theEle]+pyMet[0],pzEle[theEle]+pzMet[0]);

  // four-vectors
  _p4Ele->SetXYZT(pxEle[theEle],pyEle[theEle],pzEle[theEle],energyEle[theEle]);

  // pt
  _elePt=_p3Ele->Pt();
  _systemPt= _p3System->Pt();
}


bool WSelection::jetVeto() {

  // first check that kinematics has been set
  assert(_p3Ele->Mag());

  bool foundJet=false;
  float drEle, eta, et;
  for(int j=0;j<nJet;j++){
    TVector3 p3Jet(pxJet[j],pyJet[j],pzJet[j]);

    // check if the electron falls into the jet (the electron IS a jet)
    std::vector<float> deltaR;
    deltaR.push_back(p3Jet.DeltaR(*_p3Ele));
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaR[0])) continue;
    //    std::cout << "possible candidate of jet: deltaR from Ele = " << deltaR[0] << "   and from pos = " << deltaR[1] << std::endl;
    //    std::cout << "energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    // jet veto
    if(_selection->getSwitch("etaJetAcc") && !_selection->passCut("etaJetAcc",etaJet[j])) continue;
    if(_selection->getSwitch("etJetAcc") && !_selection->passCut("etJetAcc",etJet[j])) continue;
    drEle = deltaR[0];
    et =etJet[j];
    eta=etaJet[j];
    //    std::cout << "passed: energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    foundJet=true;
  }
  return foundJet;
}

float WSelection::Fisher(int eleIndex) {
  float fisher;
  if(classificationEle[eleIndex]<100)
    fisher = 42.0238-3.38943*s9s25Ele[eleIndex]-794.092*sqrt(covEtaEtaEle[eleIndex])-15.3449*latEle[eleIndex]-31.1032*a20Ele[eleIndex];
  else
    fisher = 27.2967+2.97453*s9s25Ele[eleIndex]-169.219*sqrt(covEtaEtaEle[eleIndex])-17.0445*latEle[eleIndex]-24.8542*a20Ele[eleIndex];
  return fisher;
}

void WSelection::bookHistos() {
  // Generator 
  _monitorGenerator->book1D("ptGenEle","p_{T} generated electron (GeV/c)",_ptGen,50,0,165);
  _monitorGenerator->book1D("nGenJets","number of generated Jets",_nGenJet,100,0,100);
  _monitorGenJets  ->book1D("et","generated jet tranverse energy (GeV)",etGenJet,50,0,300,"All+Fake+Best");

  // Event quantities
  _monitorEventAfterReco->book1D("nEle", "number of reconstructed electrons",_nEle,10,0,10);
  _monitorEventAfterReco->book1D("nJets","number of reconstructed jets",_nJet,100,0,100);
  _monitorEventAfterReco->book1D("ptEle","p_{T} electron (GeV/c)",_histopt,50,0,165);

  _monitorEventAfterSelection->book1D("nEle", "number of reconstructed electrons",_nEle,10,0,10);
  _monitorEventAfterSelection->book1D("nJets","number of reconstructed jets",_nJet,100,0,100);
  _monitorEventAfterSelection->book1D("ptEle","p_{T} electron (GeV/c)",_histopt,100,0,165);

  // Met quantities
  _monitorMet->book1D("metEt","Missing trensverse energy (GeV)",etMet,50,0,150,"All");
  _monitorMet->book1D("metPhi","Missing trensverse momentum #phi",phiMet,50,-TMath::Pi(),TMath::Pi(),"All");

  // Jet quantities
  _monitorJets->book1D("et","jet tranverse energy (GeV)",etJet,50,0,300,"All+Fake+Best");
  _monitorJets->book1D("drEleFromHighestPt","#Delta R of highest p_{T} jet from electron",_drEleFromHighestPt,50,0,TMath::Pi(),"All+Fake+Best");

  // Electron quantities
  _monitorElectrons->book1D("energy","electron energy (GeV)",energyEle,50,0,150,"All+Fake+Best");
  _monitorElectrons->book1D("et","electron tranverse energy (GeV)",etEle,50,0,150,"All+Fake+Best");
  _monitorElectrons->book1D("eta","electron #eta",etaEle,50,-6.,6.,"All+Fake+Best");
  _monitorElectrons->book1D("phi","electron #phi",phiEle,50,-TMath::Pi(),TMath::Pi(),"All+Fake+Best");
  _monitorElectrons->book1D("s1s9","electron #sum 1/#sum 9",s1s9Ele,50,0.,1.,"All+Fake+Best");
  _monitorElectrons->book1D("s9s25","electron #sum 9/#sum 25",s9s25Ele,50,0.,1.,"All+Fake+Best");
  _monitorElectrons->book1D("eOverP","electron E/P",_eOverP,50,0.75,1.1,"All+Fake+Best");  
  _monitorElectrons->book1D("deltaEta","#Delta #eta",deltaEtaAtVtxEle,150,-0.03,0.03,"All+Fake+Best");  
  _monitorElectrons->book1D("deltaPhi","#Delta #phi",deltaPhiAtVtxEle,150,-0.11,0.11,"All+Fake+Best");  
}
