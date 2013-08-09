#include <iostream>
#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "HiggsAnalysisTools/include/ZSelection.hh"

ZSelection::ZSelection(TTree *tree) 
  : HiggsBase(tree) {

  // Initialize selection
  std::string fileCuts("config/zee/zeeCuts.txt");
  std::string fileSwitches("config/zee/zeeSwitches.txt");
  _selection = new Selection(fileCuts,fileSwitches);
  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addSwitch("jetVeto");
  _selection->addSwitch("classDepEleId");

  _selection->addCut("nRecoEle");
  _selection->addCut("absEtaEleAcc");
  _selection->addCut("ptEleAcc");
  _selection->addCut("trackerPtSum");
  _selection->addCut("hcalPtSum");
  _selection->addCut("jetConeWidth");
  _selection->addCut("etaJetAcc");
  _selection->addCut("etJetAcc");
  _selection->addCut("maxPtEle");
  _selection->addCut("minPtEle");
  _selection->addCut("detaLeptons");
  _selection->addCut("MET");
  _selection->addCut("deltaPhi");
  _selection->addCut("eleInvMass");

  Selection *_goldenSelectionEB, *_bigbremSelectionEB, *_narrowSelectionEB, *_showeringSelectionEB;
  Selection *_goldenSelectionEE, *_bigbremSelectionEE, *_narrowSelectionEE, *_showeringSelectionEE;

  if (_selection->getSwitch("classDepEleId") ) {
    _goldenSelectionEB    = new Selection("config/zee/electronIDGoldenCuts.txt",      "config/zee/electronIDSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/zee/electronIDBigBremCuts.txt",     "config/zee/electronIDSwitches.txt");
    _narrowSelectionEB    = new Selection("config/zee/electronIDNarrowCuts.txt",      "config/zee/electronIDSwitches.txt");
    _showeringSelectionEB = new Selection("config/zee/electronIDShoweringCuts.txt",   "config/zee/electronIDSwitches.txt");
    _goldenSelectionEE    = new Selection("config/zee/electronIDGoldenCutsEE.txt",    "config/zee/electronIDSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/zee/electronIDBigBremCutsEE.txt",   "config/zee/electronIDSwitches.txt");
    _narrowSelectionEE    = new Selection("config/zee/electronIDNarrowCutsEE.txt",    "config/zee/electronIDSwitches.txt");
    _showeringSelectionEE = new Selection("config/zee/electronIDShoweringCutsEE.txt", "config/zee/electronIDSwitches.txt");
  }
  else {
    _goldenSelectionEB    = new Selection("config/zee/electronIDnoClassCutsEB.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/zee/electronIDnoClassCutsEB.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _narrowSelectionEB    = new Selection("config/zee/electronIDnoClassCutsEB.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _showeringSelectionEB = new Selection("config/zee/electronIDnoClassCutsEB.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _goldenSelectionEE    = new Selection("config/zee/electronIDnoClassCutsEE.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/zee/electronIDnoClassCutsEE.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _narrowSelectionEE    = new Selection("config/zee/electronIDnoClassCutsEE.txt",  "config/zee/electronIDnoClassSwitches.txt");
    _showeringSelectionEE = new Selection("config/zee/electronIDnoClassCutsEE.txt",  "config/zee/electronIDnoClassSwitches.txt");
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
    eleSelection->addSwitch("removeCracks");
    eleSelection->addCut("hOverE");
    eleSelection->addCut("s9s25");
    eleSelection->addCut("deta");
    eleSelection->addCut("dphiIn");
    eleSelection->addCut("dphiOut");
    eleSelection->addCut("invEMinusInvP");
    eleSelection->addCut("bremFraction");
    eleSelection->addCut("covEtaEta");
    //    eleSelection->addCut("covPhiPhi");
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
  _counter.AddVar("twoGoodRec");
  _counter.AddVar("trackerIsol");
  _counter.AddVar("hcalIsol");
  _counter.AddVar("eleID");
  _counter.AddVar("MET");
  _counter.AddVar("deltaPhi");
  _counter.AddVar("eleInvMass");
  _counter.AddVar("jetVeto");
  _counter.AddVar("maxPtEle");
  _counter.AddVar("minPtEle");
  _counter.AddVar("detaLeptons");
  _counter.AddVar("final");
  
  _eleCounter.SetTitle("SINGLE ELECTRON COUNTER");
  _eleCounter.AddVar("electrons");
  _eleCounter.AddVar("outOfCracks");
  _eleCounter.AddVar("hOverE");
  _eleCounter.AddVar("s9s25");
  _eleCounter.AddVar("deta");
  _eleCounter.AddVar("dphiIn");
  _eleCounter.AddVar("dphiOut");
  _eleCounter.AddVar("invEMinusInvP");
  _eleCounter.AddVar("bremFraction");
  _eleCounter.AddVar("covEtaEta");
  //  _eleCounter.AddVar("covPhiPhi");
  _eleCounter.AddVar("eOverPout");
  _eleCounter.AddVar("eOverPin");
  _eleCounter.AddVar("Fisher");
  _eleCounter.AddVar("Likelihood");
  _eleCounter.AddVar("finalEleID");


  // the kinematic vectors
  _p3Ele = new TVector3(0.,0.,0.);
  _p3Pos = new TVector3(0.,0.,0.);
  _p3Met = new TVector3(0.,0.,0.);
  
  _p4Ele = new TLorentzVector(0.,0.,0.,0.);
  _p4Pos = new TLorentzVector(0.,0.,0.,0.);

  // the Monitoring Histograms
   _monitorGenerator = new Monitor(0);
   _monitorEventAfterSelection = new Monitor(0);
   _monitorEventAfterReco = new Monitor(0);
   _monitorMet = new Monitor(&nMet);
   _bestElectrons = new std::vector<int>;
   _monitorElectrons = new Monitor(&nEle,_bestElectrons);
   _bestJets = new std::vector<int>;
   _monitorJets = new Monitor(&nJet,_bestJets);
   _bestGenJets = new std::vector<int>;
   _monitorGenJets = new Monitor(&nGenJet,_bestGenJets);


}

ZSelection::~ZSelection(){
  delete _p3Ele;
  delete _p3Pos;
  delete _p3Met;
  delete _p4Ele;
  delete _p4Pos;
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

bool ZSelection::findMcTree(const char* processType) {
  if(strcmp(processType,"Ztoee")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0)
	    );
  if(strcmp(processType,"ZtoeeBarrel")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0 &&
	     fabs(etaMc[7])<1.47 && fabs(etaMc[8])<1.47)
	    );
  if(strcmp(processType,"ZtoeeEndcap")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0 &&
	     fabs(etaMc[7])>1.47 && fabs(etaMc[8])>1.47)
	    );
  if(strcmp(processType,"ZtoeeBarrelNoCracks")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    idMc[7]*idMc[8]<0 &&
	    fabs(etaMc[7])<1.47 && fabs(etaMc[8])<1.47 &&
	    (!isInCrack(fabs(etaMc[7]))) && (!isInCrack(fabs(etaMc[8])))
	    );
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}
  
bool ZSelection::isInCrack(float eta) {
    return (eta < 0.018 ||
	    (eta>0.423 && eta<0.461) ||
	    (eta>0.770 && eta<0.806) ||
	    (eta>1.127 && eta<1.163) ||
	    (eta>1.460 && eta<1.558));
}

void ZSelection::Loop() {
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

      if(_selection->getSwitch("MCtruth") && !findMcTree("Ztoee") ) continue;              _counter.IncrVar("MCtruth");
      _monitorGenerator->Fill();
      Gene_eta->Fill(etaMc[7]);
      Gene_eta->Fill(etaMc[8]);
      Gene_pt->Fill(pMc[7]*fabs( sin(thetaMc[7])) );
      Gene_pt->Fill(pMc[8]*fabs( sin(thetaMc[8])) );
      std::pair<int,int> bestElectronPair(-1,-1);
      fillEfficiencyNumeratorHistos(RecoSingle_eta_wocm,RecoSingle_eta_wcm,RecoSingle_pt_wcm,bestElectronPair);

      if(_selection->getSwitch("trigger") && !(singleElePassedTrg || doubleElePassedTrg) ) continue;    _counter.IncrVar("trigger"); 
      fillEfficiencyNumeratorHistos(RecoSingleTrg_eta_wocm,RecoSingleTrg_eta_wcm,RecoSingleTrg_pt_wcm,bestElectronPair);
      fillEfficiencyNumeratorHistos(RecoSingleIso_eta_wocm,RecoSingleIso_eta_wcm,RecoSingleIso_pt_wcm,bestElectronPair,"isolated");
      fillEfficiencyNumeratorHistos(RecoSingleEId_eta_wocm,RecoSingleEId_eta_wcm,RecoSingleEId_pt_wcm,bestElectronPair,"identified");

      if(_selection->getSwitch("nRecoEle") && !_selection->passCut("nRecoEle",nEle)) continue;   _counter.IncrVar("nRecoEle");
      bestElectronPair=getBestLeptonPair();
      int theEle(bestElectronPair.first), thePos(bestElectronPair.second);
      if(theEle<0 || thePos<0) continue; _counter.IncrVar("twoGoodRec");
      setKinematics(theEle,thePos);
      addVariables();
      if(_selection->getSwitch("trackerPtSum") && 
	 ( !_selection->passCut("trackerPtSum",eleSumPt04Ele[theEle]) || 
	   !_selection->passCut("trackerPtSum",eleSumPt04Ele[thePos]) ) ) continue; _counter.IncrVar("trackerIsol");
      if(_selection->getSwitch("hcalPtSum") &&
	 ( !_selection->passCut("hcalPtSum",eleSumPt04Ele[theEle]) || 
	   !_selection->passCut("hcalPtSum",eleSumPt04Ele[thePos]) ) ) continue; _counter.IncrVar("hcalIsol");
      if(!isEleID(theEle) || !isEleID(thePos)) continue; _counter.IncrVar("eleID");
      _monitorEventAfterReco->Fill();
      if(_selection->getSwitch("MET") && !_selection->passCut("MET",etMet[0])) continue; _counter.IncrVar("MET");
      if(_selection->getSwitch("deltaPhi") && !_selection->passCut("deltaPhi",fabs(180./TMath::Pi()* _p3Ele->DeltaPhi(*_p3Pos))) ) continue; _counter.IncrVar("deltaPhi");
      if(_selection->getSwitch("eleInvMass") && !_selection->passCut("eleInvMass",(*_p4Ele+*_p4Pos).M())) continue; _counter.IncrVar("eleInvMass");
      if(_selection->getSwitch("jetVeto") && jetVeto()) continue; _counter.IncrVar("jetVeto");
      if(_selection->getSwitch("maxPtEle") && !_selection->passCut("maxPtEle",_maxPt)) continue; _counter.IncrVar("maxPtEle");
      if(_selection->getSwitch("minPtEle") && !_selection->passCut("minPtEle",_minPt)) continue; _counter.IncrVar("minPtEle");
      if(_selection->getSwitch("detaLeptons") && !_selection->passCut("detaLeptons",etaEle[theEle]-etaEle[thePos]) ) continue; _counter.IncrVar("detaLeptons");
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

void ZSelection::fillEfficiencyNumeratorHistos(TH1F* etaWom, TH1F* etaWcm, TH1F* etWcm, std::pair<int,int> bestElectronPair, const char* cut) {

  int etaMatchedRecoEleWom=0;
  int etaMatchedRecoEleWcm=0;  
  double etaRecoBestEle=100.;
  double etaRecoBestPos=100.;
  double etBestEle=-999.;
  double etBestPos=-999.;
  double EoP_bestEle=999.;
  double EoP_bestPos=999.;

  for(int i=0;i<nEle;i++) {

    // after the best electron pair has been chosen, fill reco histograms with tose two
    if(bestElectronPair.first!=-1 && bestElectronPair.second!=-1)
      if(i!=bestElectronPair.first && i!=bestElectronPair.second) continue;

    float deltaRPos, deltaREle;
    float EoEtruePos, EoEtrueEle;
    int itrueele=0;

    for(int imc=7;imc<9;imc++) {
      TVector3 elePAtInner(pxAtInnerEle[i],pyAtInnerEle[i],pzAtInnerEle[i]);
      TVector3 trueEleP;
      trueEleP.SetPtThetaPhi(pMc[imc]*fabs(sin(thetaMc[imc])),thetaMc[imc],phiMc[imc]);
      if(idMc[imc]<0)  { 
	deltaRPos = trueEleP.DeltaR(elePAtInner);
	EoEtruePos = energyEle[i]/pMc[imc];	
      }
      else { 
	deltaREle = trueEleP.DeltaR(elePAtInner);
	EoEtrueEle = energyEle[i]/pMc[imc];	
      }
    }

    int ptBinRecoEle = Gene_pt->FindBin( etEle[i] );

    // no charge matching
    if( (deltaRPos<0.5 || deltaREle<0.5) && etaMatchedRecoEleWom<3) {
      if(strcmp(cut,"isolated")==0 && !_selection->passCut("trackerPtSum",eleSumPt04Ele[i])) continue;
      if(strcmp(cut,"identified")==0 && 
	 (!_selection->passCut("trackerPtSum",eleSumPt04Ele[i]) ||
	  !isEleID(i)) ) continue;
      etaWom->Fill(etaEle[i]);
      etaMatchedRecoEleWom++;
    }

    // charge matching
    if(chargeEle[i]<0) {
      if( deltaREle<0.5 && fabs(EoEtrueEle-1) < EoP_bestEle) {
	if(strcmp(cut,"isolated")==0 && !_selection->passCut("trackerPtSum",eleSumPt04Ele[i])) continue;
	if(strcmp(cut,"identified")==0 && 
	   (!_selection->passCut("trackerPtSum",eleSumPt04Ele[i]) ||
	    !isEleID(i)) ) continue;
	etaRecoBestEle = etaEle[i];
	etBestEle = pMc[8]*fabs(sin(thetaMc[8]));
	EoP_bestEle = fabs(EoEtrueEle-1); 
      }
    }
    else {
      if( deltaRPos<0.5 && fabs(EoEtruePos-1) < EoP_bestPos) {
	if(strcmp(cut,"isolated")==0 && !_selection->passCut("trackerPtSum",eleSumPt04Ele[i])) continue;
	if(strcmp(cut,"identified")==0 && 
	   (!_selection->passCut("trackerPtSum",eleSumPt04Ele[i]) ||
	    !isEleID(i)) ) continue;
	etaRecoBestPos = etaEle[i];
	etBestPos = pMc[7]*fabs(sin(thetaMc[7]));
	EoP_bestPos = fabs(EoEtruePos-1); 
      }
    }
  }

  etaWcm->Fill(etaRecoBestEle);
  etaWcm->Fill(etaRecoBestPos);
  etWcm->Fill(etBestEle);
  etWcm->Fill(etBestPos);

}

void ZSelection::displayEfficiencies() {
  _eleCounter.Draw();
  _eleCounter.Draw("outOfCracks","electrons");
  _eleCounter.Draw("hOverE","outOfCracks");
  _eleCounter.Draw("s9s25","hOverE");
  _eleCounter.Draw("deta","s9s25");
  _eleCounter.Draw("dphiIn","deta");
  _eleCounter.Draw("dphiOut","dphiIn");
  _eleCounter.Draw("invEMinusInvP","dphiOut");
  _eleCounter.Draw("bremFraction","invEMinusInvP");
  _eleCounter.Draw("covEtaEta","bremFraction");
  //  _eleCounter.Draw("covPhiPhi","covEtaEta");
  _eleCounter.Draw("eOverPout","covEtaEta");
  _eleCounter.Draw("eOverPin","eOverPout");
  _eleCounter.Draw("Fisher","eOverPin");
  _eleCounter.Draw("Likelihood","Fisher");
  _eleCounter.Draw("finalEleID","electrons");

  _counter.Draw();
  _counter.Draw("MCtruth","event");
  _counter.Draw("trigger","MCtruth");
  _counter.Draw("nRecoEle","trigger");
  _counter.Draw("twoGoodRec","nRecoEle");
  _counter.Draw("trackerIsol","twoGoodRec");
  _counter.Draw("hcalIsol","trackerIsol");
  _counter.Draw("eleID","hcalIsol");
  _counter.Draw("MET","eleID");
  _counter.Draw("deltaPhi","MET");
  _counter.Draw("eleInvMass","deltaPhi");
  _counter.Draw("jetVeto","eleInvMass");
  _counter.Draw("maxPtEle","jetVeto");
  _counter.Draw("minPtEle","maxPtEle");
  _counter.Draw("detaLeptons","minPtEle");
  _counter.Draw("final","MCtruth");

  // make the efficiency histograms
  TH1F *EffRecoSingle_eta_geoMatch = (TH1F*)RecoSingle_eta_wocm -> Clone("EffRecoSingle_eta_geoMatch");
  TH1F *EffRecoSingle_eta_chaMatch = (TH1F*)RecoSingle_eta_wcm  -> Clone("EffRecoSingle_eta_chaMatch");
  TH1F *EffRecoSingleTrg_eta_geoMatch = (TH1F*)RecoSingleTrg_eta_wocm -> Clone("EffRecoSingleTrg_eta_geoMatch");
  TH1F *EffRecoSingleTrg_eta_chaMatch = (TH1F*)RecoSingleTrg_eta_wcm  -> Clone("EffRecoSingleTrg_eta_chaMatch");
  TH1F *EffRecoSingleIso_eta_geoMatch = (TH1F*)RecoSingleIso_eta_wocm -> Clone("EffRecoSingleIso_eta_geoMatch");
  TH1F *EffRecoSingleIso_eta_chaMatch = (TH1F*)RecoSingleIso_eta_wcm  -> Clone("EffRecoSingleIso_eta_chaMatch");
  TH1F *EffRecoSingleEId_eta_geoMatch = (TH1F*)RecoSingleEId_eta_wocm -> Clone("EffRecoSingleEId_eta_geoMatch");
  TH1F *EffRecoSingleEId_eta_chaMatch = (TH1F*)RecoSingleEId_eta_wcm  -> Clone("EffRecoSingleEId_eta_chaMatch");
  EffRecoSingle_eta_geoMatch->Sumw2();
  EffRecoSingle_eta_chaMatch->Sumw2();
  EffRecoSingleTrg_eta_geoMatch->Sumw2();
  EffRecoSingleTrg_eta_chaMatch->Sumw2();       
  EffRecoSingleIso_eta_geoMatch->Sumw2(); 
  EffRecoSingleIso_eta_chaMatch->Sumw2(); 
  EffRecoSingleEId_eta_geoMatch->Sumw2(); 
  EffRecoSingleEId_eta_chaMatch->Sumw2();

  TH1F *EffRecoSingle_pt_chaMatch = (TH1F*)RecoSingle_pt_wcm  -> Clone("EffRecoSingle_pt_chaMatch");
  TH1F *EffRecoSingleTrg_pt_chaMatch = (TH1F*)RecoSingleTrg_pt_wcm  -> Clone("EffRecoSingleTrg_pt_chaMatch");
  TH1F *EffRecoSingleIso_pt_chaMatch = (TH1F*)RecoSingleIso_pt_wcm  -> Clone("EffRecoSingleIso_pt_chaMatch");
  TH1F *EffRecoSingleEId_pt_chaMatch = (TH1F*)RecoSingleEId_pt_wcm  -> Clone("EffRecoSingleEId_pt_chaMatch");
  EffRecoSingle_pt_chaMatch->Sumw2();   
  EffRecoSingleTrg_pt_chaMatch->Sumw2();
  EffRecoSingleIso_pt_chaMatch->Sumw2();
  EffRecoSingleEId_pt_chaMatch->Sumw2();

  EffRecoSingle_eta_geoMatch   -> Divide(RecoSingle_eta_wocm, Gene_eta, 1, 1);
  EffRecoSingle_eta_chaMatch   -> Divide(RecoSingle_eta_wcm,  Gene_eta, 1, 1);
  EffRecoSingleTrg_eta_geoMatch-> Divide(RecoSingleTrg_eta_wocm, Gene_eta, 1, 1);
  EffRecoSingleTrg_eta_chaMatch-> Divide(RecoSingleTrg_eta_wcm,  Gene_eta, 1, 1);
  EffRecoSingleIso_eta_geoMatch-> Divide(RecoSingleIso_eta_wocm, Gene_eta, 1, 1);
  EffRecoSingleIso_eta_chaMatch-> Divide(RecoSingleIso_eta_wcm,  Gene_eta, 1, 1);
  EffRecoSingleEId_eta_geoMatch-> Divide(RecoSingleEId_eta_wocm, Gene_eta, 1, 1);
  EffRecoSingleEId_eta_chaMatch-> Divide(RecoSingleEId_eta_wcm,  Gene_eta, 1, 1);

  EffRecoSingle_pt_chaMatch   -> Divide(RecoSingle_pt_wcm,  Gene_pt, 1, 1);
  EffRecoSingleTrg_pt_chaMatch-> Divide(RecoSingleTrg_pt_wcm,  Gene_pt, 1, 1);
  EffRecoSingleIso_pt_chaMatch-> Divide(RecoSingleIso_pt_wcm,  Gene_pt, 1, 1);
  EffRecoSingleEId_pt_chaMatch-> Divide(RecoSingleEId_pt_wcm,  Gene_pt, 1, 1);

  TFile effFile("efficiency.root","RECREATE");
  Gene_eta->Write();
  Gene_pt->Write();
  RecoSingle_eta_wocm->Write();
  RecoSingle_eta_wcm->Write();
  RecoSingleTrg_eta_wocm->Write();
  RecoSingleTrg_eta_wcm->Write();
  RecoSingleIso_eta_wocm->Write();
  RecoSingleIso_eta_wcm->Write();
  RecoSingleEId_eta_wocm->Write();
  RecoSingleEId_eta_wcm->Write();
  RecoSingle_pt_wcm->Write();
  RecoSingleTrg_pt_wcm->Write();
  RecoSingleIso_pt_wcm->Write();
  RecoSingleEId_pt_wcm->Write();

  EffRecoSingle_eta_geoMatch->Write();
  EffRecoSingle_eta_chaMatch->Write();
  EffRecoSingleTrg_eta_geoMatch->Write();
  EffRecoSingleTrg_eta_chaMatch->Write();
  EffRecoSingleIso_eta_geoMatch->Write();
  EffRecoSingleIso_eta_chaMatch->Write();
  EffRecoSingleEId_eta_geoMatch->Write();
  EffRecoSingleEId_eta_chaMatch->Write();
  EffRecoSingle_pt_chaMatch->Write();
  EffRecoSingleTrg_pt_chaMatch->Write();
  EffRecoSingleIso_pt_chaMatch->Write();
  EffRecoSingleEId_pt_chaMatch->Write();

  effFile.Close();

}

std::pair<int,int> ZSelection::getBestLeptonPair() {
  int theEle=-1;
  int thePos=-1;
  float minDeltaEoPEle = 1000.;
  float minDeltaEoPPos = 1000.;
  std::vector<int> goodRecoLeptons;

  for(int i=0;i<nEle;i++) {
    if(_selection->getSwitch("absEtaEleAcc") && !_selection->passCut("absEtaEleAcc",fabs(etaEle[i])) ) continue;
    TVector3 pEle(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pEle.Pt();
    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",thisPt) ) continue;
    float thisCharge = chargeEle[i];
    float thisDeltaEoP=fabs( eSeedOverPoutEle[i]-1 );
    if (thisCharge > 0 && thisDeltaEoP < minDeltaEoPPos){ minDeltaEoPPos = thisDeltaEoP; thePos = i; }
    if (thisCharge < 0 && thisDeltaEoP < minDeltaEoPEle){ minDeltaEoPEle = thisDeltaEoP; theEle = i; }
  }
  _bestElectrons->clear();
  _bestElectrons->push_back(theEle);  _bestElectrons->push_back(thePos); 
  return make_pair(theEle,thePos);
}

bool ZSelection::isEleID(int eleIndex) {

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
  if(selection->getSwitch("removeCracks") && isInCrack(fabs(etaEle[eleIndex])) ) return false; _eleCounter.IncrVar("outOfCracks");
  if(selection->getSwitch("hOverE") && !selection->passCut("hOverE",hOverEEle[eleIndex])) return false;  _eleCounter.IncrVar("hOverE");
  if(selection->getSwitch("s9s25") && !selection->passCut("s9s25",s9s25Ele[eleIndex])) return false; _eleCounter.IncrVar("s9s25");
  if(selection->getSwitch("deta") && !selection->passCut("deta",deltaEtaAtVtxEle[eleIndex])) return false; _eleCounter.IncrVar("deta");
  if(selection->getSwitch("dphiIn") && !selection->passCut("dphiIn",deltaPhiAtVtxEle[eleIndex])) return false; _eleCounter.IncrVar("dphiIn");
  if(selection->getSwitch("dphiOut") && !selection->passCut("dphiOut",deltaPhiAtCaloEle[eleIndex])) return false; _eleCounter.IncrVar("dphiOut");
  if(selection->getSwitch("invEMinusInvP") && !selection->passCut("invEMinusInvP",1./ecalEle[eleIndex]-1./pTrkAtInner.Mag())) return false; _eleCounter.IncrVar("invEMinusInvP");
  if(selection->getSwitch("bremFraction") && !selection->passCut("bremFraction",fabs(pTrkAtInner.Mag()-pTrkAtOuter.Mag())/pTrkAtInner.Mag())) return false; _eleCounter.IncrVar("bremFraction");
  if(selection->getSwitch("covEtaEta") && !selection->passCut("covEtaEta",covEtaEtaEle[eleIndex])) return false; _eleCounter.IncrVar("covEtaEta");
  //  if(selection->getSwitch("covPhiPhi") && !selection->passCut("covPhiPhi",covPhiPhiEle[eleIndex])) return false; _eleCounter.IncrVar("covPhiPhi");
  if(selection->getSwitch("eOverPout") && !selection->passCut("eOverPout",eSeedOverPoutEle[eleIndex])) return false; _eleCounter.IncrVar("eOverPout");
  if(selection->getSwitch("eOverPin") && !selection->passCut("eOverPin",eSuperClusterOverPEle[eleIndex])) return false; _eleCounter.IncrVar("eOverPin");
  if(selection->getSwitch("Fisher") && !selection->passCut("Fisher",Fisher(eleIndex))) return false; _eleCounter.IncrVar("Fisher");
  if(selection->getSwitch("Likelihood") && !selection->passCut("Likelihood",eleIdLikelihoodEle[eleIndex])) return false; _eleCounter.IncrVar("Likelihood");
  _eleCounter.IncrVar("finalEleID");

  return true;
}

void ZSelection::addVariables() {
  _deltaPhi[0]=fabs(180./TMath::Pi()* _p3Ele->DeltaPhi(*_p3Pos));
  _mll[0]=(*_p4Ele+*_p4Pos).M();
  for(int i=0;i<nEle;i++) {
    _eOverP[i]=ecalEle[i]/energyEle[i];
  }

  _highestPt[0]=_maxPt;
  _lowestPt[0]=_minPt;
  _nEle[0]=(float)nEle;

  int theHighestPtJet=0;
  float maxJetPt=-100;
  for(int i=0;i<nJet;i++) {
    TVector3 p3Jet(pxJet[i],pyJet[i],pzJet[i]);
    _drEleFromHighestPt[i]=_p3Ele->Angle(p3Jet);
    _drPosFromHighestPt[i]=_p3Pos->Angle(p3Jet);
    if(etJet[i]>maxJetPt) {
      maxJetPt=etJet[i];
      theHighestPtJet=i;
    }
  }

  _bestJets->clear();
  _bestJets->push_back(theHighestPtJet);
  _nJet[0]=(float)nJet;

  TVector3 dilepPt(_p3Ele->X()+_p3Pos->X(),_p3Ele->Y()+_p3Pos->Y());
  _transvMass[0]=sqrt(2*dilepPt.Mag()*_p3Met->Mag()*(1-cos(dilepPt.Angle(*_p3Met)) ) );

  // generator level
  if(pMc[11]>=pMc[13]) {
    _highestPtGen[0]=pMc[11]*fabs(sin(thetaMc[11]));
    _lowestPtGen[0]=pMc[13]*fabs(sin(thetaMc[13]));
  }
  else {
    _highestPtGen[0]=pMc[13]*fabs(sin(thetaMc[13]));
    _lowestPtGen[0]=pMc[11]*fabs(sin(thetaMc[11]));
  }


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

void ZSelection::setKinematics(int theEle, int thePos) {
  // three-vectors
  _p3Ele->SetXYZ(pxEle[theEle],pyEle[theEle],pzEle[theEle]);
  _p3Pos->SetXYZ(pxEle[thePos],pyEle[thePos],pzEle[thePos]);
  _p3Met->SetXYZ(pxMet[0],pyMet[0],pzMet[0]);

  // four-vectors
  _p4Ele->SetXYZT(pxEle[theEle],pyEle[theEle],pzEle[theEle],energyEle[theEle]);
  _p4Pos->SetXYZT(pxEle[thePos],pyEle[thePos],pzEle[thePos],energyEle[thePos]);

  _maxPt=TMath::Max(_p3Ele->Pt(),_p3Pos->Pt());
  _minPt=TMath::Min(_p3Ele->Pt(),_p3Pos->Pt());

}

bool ZSelection::jetVeto() {
  // first check that kinematics has been set
  assert(_p3Ele->Mag() && _p3Pos->Mag());
  //  std::cout << "INIZIO DEL LOOP SUI JETS" << std::endl;
  bool foundJet=false;
  float drEle, drPos, eta, et;
  for(int j=0;j<nJet;j++){
    TVector3 p3Jet(pxJet[j],pyJet[j],pzJet[j]);
    // check if the electron or the positron falls into the jet
    // (the electron IS a jet)
    std::vector<float> deltaR;
    deltaR.push_back(p3Jet.DeltaR(*_p3Ele));
    deltaR.push_back(p3Jet.DeltaR(*_p3Pos));
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaR[0])) continue;
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaR[1])) continue;
    //    std::cout << "possible candidate of jet: deltaR from Ele = " << deltaR[0] << "   and from pos = " << deltaR[1] << std::endl;
    //    std::cout << "energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    // jet veto
    if(_selection->getSwitch("etaJetAcc") && !_selection->passCut("etaJetAcc",etaJet[j])) continue;
    if(_selection->getSwitch("etJetAcc") && !_selection->passCut("etJetAcc",etJet[j])) continue;
    drEle = deltaR[0];
    drPos = deltaR[1];
    et=etJet[j];
    eta=etaJet[j];
    //    std::cout << "passed: energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    foundJet=true;
  }
  //  std::cout << "FINE DEL LOOP SUI JETS" << std::endl;
  //  if(foundJet) std::cout << "found a jet" << " drEle = " << drEle << "    drPos =" << drPos << "   et = " << et << "   eta = " << eta << std::endl;
  return foundJet;
}

float ZSelection::Fisher(int eleIndex) {
  float fisher;
  if(classificationEle[eleIndex]<100)
    fisher = 42.0238-3.38943*s9s25Ele[eleIndex]-794.092*sqrt(covEtaEtaEle[eleIndex])-15.3449*latEle[eleIndex]-31.1032*a20Ele[eleIndex];
  else
    fisher = 27.2967+2.97453*s9s25Ele[eleIndex]-169.219*sqrt(covEtaEtaEle[eleIndex])-17.0445*latEle[eleIndex]-24.8542*a20Ele[eleIndex];
  return fisher;
}

void ZSelection::bookHistos() {
  // Generator 
  _monitorGenerator->book1D("highestPtGenEle","Highest p_{T} generated electron (GeV/c)",_highestPtGen,50,0,165);
  _monitorGenerator->book1D("lowestPtGenEle","Lowest p_{T} generated electron (GeV/c)",_lowestPtGen,50,0,60);
  _monitorGenerator->book1D("nGenJets","number of generated Jets",_nGenJet,100,0,100);

  _monitorGenJets->book1D("et","generated jet tranverse energy (GeV)",etGenJet,50,0,300,"All+Fake+Best");

  // Event quantities
  _monitorEventAfterReco->book1D("nEle","number of reconstructed electrons",_nEle,10,0,10);
  _monitorEventAfterReco->book1D("nJets","number of reconstructed jets",_nJet,100,0,100);
  _monitorEventAfterReco->book1D("deltaPhi","#Delta #phi of e^{+}e^{-} (degrees)",_deltaPhi,50,0.,180);
  _monitorEventAfterReco->book1D("mll","e^{+}e^{-} invariant mass (GeV/c^{2})",_mll,50,0.,165.);
  _monitorEventAfterReco->book1D("WWtrMass","W^{+}W^{-} transverse mass (GeV/c^{2})",_transvMass,50,0,250);
  _monitorEventAfterReco->book1D("highestPtEle","Highest p_{T} electron (GeV/c)",_highestPt,50,0,165);
  _monitorEventAfterReco->book1D("lowestPtEle","Lowest p_{T} electron (GeV/c)",_lowestPt,50,0,60);

  _monitorEventAfterSelection->book1D("nEle","number of reconstructed electrons",_nEle,10,0,10);
  _monitorEventAfterSelection->book1D("nJets","number of reconstructed jets",_nJet,100,0,100);
  _monitorEventAfterSelection->book1D("deltaPhi","#Delta #phi of e^{+}e^{-} (degrees)",_deltaPhi,50,0.,180);
  _monitorEventAfterSelection->book1D("mll","e^{+}e^{-} invariant mass (GeV/c^{2})",_mll,100,70.,110.);
  _monitorEventAfterSelection->book1D("WWtrMass","W^{+}W^{-} transverse mass (GeV/c^{2})",_transvMass,100,0,250);
  _monitorEventAfterSelection->book1D("highestPtEle","Highest p_{T} electron (GeV/c)",_highestPt,100,0,165);
  _monitorEventAfterSelection->book1D("lowestPtEle","Lowest p_{T} electron (GeV/c)",_lowestPt,100,0,60);

  // Met quantities
  _monitorMet->book1D("metEt","Missing trensverse energy (GeV)",etMet,50,0,150,"All");
  _monitorMet->book1D("metPhi","Missing trensverse momentum #phi",phiMet,50,-TMath::Pi(),TMath::Pi(),"All");

  // Jet quantities
  _monitorJets->book1D("et","jet tranverse energy (GeV)",etJet,50,0,300,"All+Fake+Best");
  _monitorJets->book1D("drEleFromHighestPt","#Delta R of highest p_{T} jet from electron",_drEleFromHighestPt,50,0,TMath::Pi(),"All+Fake+Best");
  _monitorJets->book1D("drPosFromHighestPt","#Delta R of highest p_{T} jet from positron",_drPosFromHighestPt,50,0,TMath::Pi(),"All+Fake+Best");


  // Electron quantities
  _monitorElectrons->book1D("energy","electron energy (GeV)",energyEle,50,0,150,"All+Fake+Best");
  _monitorElectrons->book1D("et","electron tranverse energy (GeV)",etEle,50,0,150,"All+Fake+Best");
  _monitorElectrons->book1D("eta","electron #eta",etaEle,50,-6.,6.,"All+Fake+Best");
  _monitorElectrons->book1D("phi","electron #phi",phiEle,50,-TMath::Pi(),TMath::Pi(),"All+Fake+Best");
  _monitorElectrons->book1D("s1s9","electron #sum 1/#sum 9",s1s9Ele,50,0.,1.,"All+Fake+Best");
  _monitorElectrons->book1D("s9s25","electron #sum 9/#sum 25",s9s25Ele,50,0.,1.,"All+Fake+Best");
  _monitorElectrons->book1D("eOverP","electron E/P",_eOverP,50,0.75,1.1,"All+Fake+Best");


  // Efficiencies
  RecoSingle_eta_wcm  = new TH1F("RecoSingle_eta_wcm",  "geometrical and charge matching - After reconstruction",     25, -2.5,2.5);
  RecoSingle_eta_wocm = new TH1F("RecoSingle_eta_wocm", "geometrical matching - After reconstruction",  25, -2.5,2.5);
  RecoSingleTrg_eta_wcm  = new TH1F("RecoSingleTrg_eta_wcm",  "geometrical and charge matching - After HLT",     25, -2.5,2.5);
  RecoSingleTrg_eta_wocm = new TH1F("RecoSingleTrg_eta_wocm", "geometrical matching - After HLT",  25, -2.5,2.5);
  RecoSingleIso_eta_wcm  = new TH1F("RecoSingleIso_eta_wcm",  "geometrical and charge matching - After isolation",     25, -2.5,2.5);
  RecoSingleIso_eta_wocm = new TH1F("RecoSingleIso_eta_wocm", "geometrical matching - After isolation",  25, -2.5,2.5);
  RecoSingleEId_eta_wcm  = new TH1F("RecoSingleEId_eta_wcm",  "geometrical and charge matching - After e-Id",     25, -2.5,2.5);
  RecoSingleEId_eta_wocm = new TH1F("RecoSingleEId_eta_wocm", "geometrical matching - After e-Id",  25, -2.5,2.5);
  Gene_eta      = new TH1F("Gene_eta",      "generated #eta",                               25, -2.5,2.5);

  RecoSingle_eta_wcm->GetXaxis()->SetTitle("electron #eta");
  RecoSingle_eta_wocm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleTrg_eta_wcm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleTrg_eta_wocm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleIso_eta_wcm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleIso_eta_wocm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleEId_eta_wcm->GetXaxis()->SetTitle("electron #eta");
  RecoSingleEId_eta_wocm->GetXaxis()->SetTitle("electron #eta");
  RecoSingle_eta_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingle_eta_wocm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleTrg_eta_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleTrg_eta_wocm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleIso_eta_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleIso_eta_wocm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleEId_eta_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleEId_eta_wocm->GetYaxis()->SetTitle("single electron efficiency");

  RecoSingle_pt_wcm  = new TH1F("RecoSingle_pt_wcm",  "geometrical and charge matching - After reconstruction",     25, 0.,90);
  RecoSingleTrg_pt_wcm  = new TH1F("RecoSingleTrg_pt_wcm",  "geometrical and charge matching - After HLT",     25, 0.,90);
  RecoSingleIso_pt_wcm  = new TH1F("RecoSingleIso_pt_wcm",  "geometrical and charge matching - After isolation",     25, 0.,90);
  RecoSingleEId_pt_wcm  = new TH1F("RecoSingleEId_pt_wcm",  "geometrical and charge matching - After e-Id",     25, 0.,90);
  Gene_pt      = new TH1F("Gene_pt",      "generated p_{T}",                               25, 0, 90.);
 
  RecoSingle_pt_wcm->GetXaxis()->SetTitle("electron p_{T} (GeV)");
  RecoSingleTrg_pt_wcm->GetXaxis()->SetTitle("electron p_{T} (GeV)");
  RecoSingleIso_pt_wcm->GetXaxis()->SetTitle("electron p_{T} (GeV)");
  RecoSingleEId_pt_wcm->GetXaxis()->SetTitle("electron p_{T} (GeV)");
  RecoSingle_pt_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleTrg_pt_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleIso_pt_wcm->GetYaxis()->SetTitle("single electron efficiency");
  RecoSingleEId_pt_wcm->GetYaxis()->SetTitle("single electron efficiency");
 
}
