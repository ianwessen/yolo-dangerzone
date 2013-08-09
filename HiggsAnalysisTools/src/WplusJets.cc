#include <iostream>
#include <string>
#include <cmath>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "HiggsAnalysisTools/include/WplusJets.hh"

WplusJets::WplusJets(TTree *tree) 
  : HiggsBase(tree) {

  // EleID: class dependent or not
  bool classDepID = true;

  // Initialize selection
  std::string fileCuts("config/wplusjets/wplusjetsCuts.txt");
  std::string fileSwitches("config/wplusjets/wplusjetsSwitches.txt");
  _selection = new Selection(fileCuts,fileSwitches);

  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");

  _selection->addCut("nRecoEle");
  _selection->addCut("etaEleAcc");
  _selection->addCut("ptEleAcc");
  _selection->addCut("trackerPtSum");
  _selection->addCut("MET");
  _selection->addCut("nRecoJets");
  _selection->addCut("jetConeWidth");
  _selection->addCut("etaJetAcc");
  _selection->addCut("ptJetAcc");
  _selection->addCut("alphaJet");
  _selection->addCut("emFractionJet");
  _selection->addCut("wTrMass");
  _selection->addCut("deltaPhi");

  Selection *_goldenSelectionEB, *_bigbremSelectionEB, *_narrowSelectionEB, *_showeringSelectionEB; 
  Selection *_goldenSelectionEE, *_bigbremSelectionEE, *_narrowSelectionEE, *_showeringSelectionEE; 
  
  if (classDepID){
    _goldenSelectionEB    = new Selection("config/wplusjets/electronIDGoldenCutsEB.txt",     "config/wplusjets/electronIDSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/wplusjets/electronIDBigBremCutsEB.txt",    "config/wplusjets/electronIDSwitches.txt");
    _narrowSelectionEB    = new Selection("config/wplusjets/electronIDNarrowCutsEB.txt",     "config/wplusjets/electronIDSwitches.txt");
    _showeringSelectionEB = new Selection("config/wplusjets/electronIDShoweringCutsEB.txt",  "config/wplusjets/electronIDSwitches.txt");
    _goldenSelectionEE    = new Selection("config/wplusjets/electronIDGoldenCutsEE.txt",   "config/wplusjets/electronIDSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/wplusjets/electronIDBigBremCutsEE.txt",  "config/wplusjets/electronIDSwitches.txt");
    _narrowSelectionEE    = new Selection("config/wplusjets/electronIDNarrowCutsEE.txt",   "config/wplusjets/electronIDSwitches.txt");
    _showeringSelectionEE = new Selection("config/wplusjets/electronIDShoweringCutsEE.txt","config/wplusjets/electronIDSwitches.txt");
  }
  if (!classDepID){
    _goldenSelectionEB    = new Selection("config/wplusjets/electronIDnoClassCutsEB.txt",  "config/wplusjets/electronIDSwitches.txt");
    _bigbremSelectionEB   = new Selection("config/wplusjets/electronIDnoClassCutsEB.txt",  "config/wplusjets/electronIDSwitches.txt");
    _narrowSelectionEB    = new Selection("config/wplusjets/electronIDnoClassCutsEB.txt",  "config/wplusjets/electronIDSwitches.txt");
    _showeringSelectionEB = new Selection("config/wplusjets/electronIDnoClassCutsEB.txt",  "config/wplusjets/electronIDSwitches.txt");
    _goldenSelectionEE    = new Selection("config/wplusjets/electronIDnoClassCutsEE.txt",  "config/wplusjets/electronIDSwitches.txt");
    _bigbremSelectionEE   = new Selection("config/wplusjets/electronIDnoClassCutsEE.txt",  "config/wplusjets/electronIDSwitches.txt");
    _narrowSelectionEE    = new Selection("config/wplusjets/electronIDnoClassCutsEE.txt",  "config/wplusjets/electronIDSwitches.txt");
    _showeringSelectionEE = new Selection("config/wplusjets/electronIDnoClassCutsEE.txt",  "config/wplusjets/electronIDSwitches.txt");
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
  _counter.AddVar("MET");
  _counter.AddVar("nRecoJets");
  _counter.AddVar("wTrMass");
  _counter.AddVar("deltaPhi");
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

  _jetCounter.SetTitle("SINGLE JET COUNTER");
  _jetCounter.AddVar("jets");
  _jetCounter.AddVar("etaJetAcc");
  _jetCounter.AddVar("ptJetAcc");
  _jetCounter.AddVar("alphaJet");
  _jetCounter.AddVar("emFracJet");
  _jetCounter.AddVar("finalJet");


  // the Monitoring Histograms
   _monitorEventAfterSelection = new Monitor(0);
   _monitorEventAfterReco      = new Monitor(0);

   _bestProbe = -1;
}

WplusJets::~WplusJets(){
  delete _monitorEventAfterSelection;
  delete _monitorEventAfterReco;
  delete _selection;
}


bool WplusJets::findMcTree(const char* processType) {

  bool found=false;
  if(strcmp(processType,"Wtoenu")==0) { 
    for(int imc=0; imc<15; imc++) {
      if(abs(idMc[imc])==11 && abs(idMc[mothMc[imc]])==24)
	found=true;
    }
  }
  else found=false;
  return found;
}

void WplusJets::Loop() {
  _verbose=true;
  if(fChain == 0) return;
  
  bookHistos();
  TFile *file = new TFile("histograms.root","RECREATE");
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

      if(_selection->getSwitch("trigger") && !(singleElePassedTrg) && !(singleEleRelaxPassedTrg) ) continue;    
      _counter.IncrVar("trigger"); 

      if(_selection->getSwitch("nRecoEle") && !_selection->passCut("nRecoEle",nEle)) continue;   
      _counter.IncrVar("nRecoEle");

      int theEle = getBestLepton();
      if(theEle<0) continue; 
      _counter.IncrVar("oneGoodRec");

      int theJet = getBestJet();
      setKinematics(theEle, theJet);
//       cout << theJet << endl;
//       std::cout << "etaEle[theEle] = " << etaEle[theEle] << "   etaJet[theJet] = " << etaJet[theJet] << std::endl;


      fakeGenEta->Fill(etaMc[7]); // 7 = primary parton of jet
      fakeGenPt->Fill(pMc[7]*fabs(sin(thetaMc[7])));

      std::vector<int> theProbes = findProbe(0.5);
      //      std::cout << "theProbes.size() = " << theProbes.size() << std::endl;
//       if(_bestProbe>-1 && _bestProbe!=theEle && theJet>-1) fakeReco->Fill(etaEle[_bestProbe]);
//       if(_bestProbe>-1 && _bestProbe!=theEle && theJet>-1 && isEleID(_bestProbe)) fakeIdentified->Fill(etaEle[_bestProbe]);  
      if(_bestProbe>-1 && _bestProbe!=theEle && theJet>-1) {
	fakeRecoEta->Fill(etaMc[7]);
	fakeRecoPt->Fill(pMc[7]*fabs(sin(thetaMc[7])));
      }
      if(_bestProbe>-1 && _bestProbe!=theEle && theJet>-1 && isEleID(_bestProbe)) {
	fakeIdentifiedEta->Fill(etaMc[7]);  
	fakeIdentifiedPt->Fill(pMc[7]*fabs(sin(thetaMc[7])));
      }


      if(!isEleID(theEle)) continue; 
      _counter.IncrVar("eleID");
      _monitorEventAfterReco->Fill();

      std::vector<int>::iterator probeIter;
      for(probeIter=theProbes.begin(); probeIter!=theProbes.end(); probeIter++) {
	_EoverPoutPDF->Fill(eSeedOverPoutEle[*probeIter]);
      }

      if(_selection->getSwitch("trackerPtSum") && (!_selection->passCut("trackerPtSum",eleSumPt04Ele[theEle])) ) continue;
      _counter.IncrVar("trackerIsol");

      if(_selection->getSwitch("MET") && !_selection->passCut("MET",etMet[0])) continue; 
      _counter.IncrVar("MET");

      if(_selection->getSwitch("nRecoJets") && !_selection->passCut("nRecoEle",nEle)) continue;
      _counter.IncrVar("nRecoJets");


      if(_selection->getSwitch("wTrMass") && !_selection->passCut("wTrMass",_transvMass[0])) continue;
      _counter.IncrVar("wTrMass");

      if(_selection->getSwitch("deltaPhi") && !_selection->passCut("deltaPhi",_deltaPhi[0])) continue;
      _counter.IncrVar("deltaPhi");

      _counter.IncrVar("final");

      
      _monitorEventAfterSelection->Fill();

  }

  _monitorEventAfterReco->WritePs("eventAfterReco.ps");
  _monitorEventAfterReco->WriteRoot(file);
  _monitorEventAfterSelection->WritePs("eventAfterSelection.ps");
  _monitorEventAfterSelection->WriteRoot(file);
  file->Close();
  
  // Write the PDF histograms to ROOT file
  TFile filePDF("bkgPdfWplusJets.root","RECREATE");
  filePDF.cd();
  _EoverPoutPDF->Write();
  filePDF.Close();

}

void WplusJets::displayEfficiencies() {
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

  _jetCounter.Draw();
  _jetCounter.Draw("etaJetAcc","jets");
  _jetCounter.Draw("ptJetAcc","etaJetAcc");
  _jetCounter.Draw("emFracJet","ptJetAcc");
  _jetCounter.Draw("finalJet","jets");

  _counter.Draw();
  _counter.Draw("MCtruth","event");
  _counter.Draw("trigger","MCtruth");
  _counter.Draw("nRecoEle","trigger");
  _counter.Draw("oneGoodRec","nRecoEle");
  _counter.Draw("eleID","oneGoodRec");
  _counter.Draw("trackerIsol","eleID");
  _counter.Draw("MET","trackerIsol");
  _counter.Draw("nRecoJets","MET");
  _counter.Draw("wTrMass","nRecoJets");
  _counter.Draw("deltaPhi","wTrMass");
  _counter.Draw("final","MCtruth");

  // electron mis-id vs eta
  EfficiencyEvaluator eleMisIdEta("eleMisidEta.root");
  eleMisIdEta.AddNumerator(fakeRecoEta);
  eleMisIdEta.AddNumerator(fakeIdentifiedEta);
  eleMisIdEta.SetDenominator(fakeGenEta);
  eleMisIdEta.ComputeEfficiencies();
  eleMisIdEta.Write();

  // electron mmis-id vs pt
  EfficiencyEvaluator eleMisIdPt("eleMisidPt.root");
  eleMisIdPt.AddNumerator(fakeRecoPt);
  eleMisIdPt.AddNumerator(fakeIdentifiedPt);
  eleMisIdPt.SetDenominator(fakeGenPt);
  eleMisIdPt.ComputeEfficiencies();
  eleMisIdPt.Write();

}


int WplusJets::getBestLepton() {
  int theEle=-1;
  float ptEle=-1000.;
  TVector3 pEle;
  for(int i=0;i<nEle;i++) {
    if(_selection->getSwitch("etaEleAcc") && !_selection->passCut("etaEleAcc",etaEle[i]) ) continue;
    pEle = TVector3(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pEle.Pt();
    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",thisPt) ) continue;
    if (thisPt> ptEle){ ptEle = thisPt;  _p3Ele=pEle; theEle = i; }
  }
  return theEle;
}

int WplusJets::getBestJet() {
  int theJet=-1;
  float ptJet=-1000.;
  TVector3 pJet;
  for(int i=0;i<nJet;i++) {

    // check if the electron or the positron falls into the jet
    // (the electron IS a jet)
    pJet = TVector3(pxJet[i],pyJet[i],pzJet[i]);
    float deltaR = pJet.DeltaR(_p3Ele);
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaR)) continue;
    _jetCounter.IncrVar("jets");

    if(_selection->getSwitch("etaJetAcc") && !_selection->passCut("etaJetAcc",etaJet[i]) ) continue;
    _jetCounter.IncrVar("etaJetAcc");


    float thisPt=pJet.Pt();
    if(_selection->getSwitch("ptJetAcc") && !_selection->passCut("ptJetAcc",thisPt) ) continue;
    _jetCounter.IncrVar("ptJetAcc");

    if(_selection->getSwitch("alphaJet") && !_selection->passCut("alphaJet",alphaJet[i]) ) continue;
    _jetCounter.IncrVar("alphaJet");

    if(_selection->getSwitch("emFracJet") && !_selection->passCut("emFracJet",emFracJet[i]) ) continue;
    _jetCounter.IncrVar("emFracJet");
    _jetCounter.IncrVar("finalJet");

    if (thisPt> ptJet){ ptJet = thisPt; _p3Jet = pJet; theJet = i; }
  }
  return theJet;
  
}

bool WplusJets::isEleID(int eleIndex) {

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

void WplusJets::setKinematics(int theEle, int theJet) {

  // three-vectors
  _p3Met.SetXYZ(pxMet[0],pyMet[0],0.0);
  TVector3 ptEle(pxEle[theEle],pyEle[theEle],0.0);
  TVector3 ptJet(pxJet[theJet],pyJet[theJet],0.0);

  TVector3 ptSystem = _p3Met+ptEle;

  _transvMass[0] = sqrt(2*_p3Ele.Pt()*etMet[0] * fabs(1-cos(ptEle.Angle(_p3Met))) );
  _deltaPhi[0] = fabs(ptSystem.Angle(ptJet));

}


bool WplusJets::jetVeto() {

  // first check that kinematics has been set
  assert(_p3Ele.Mag());

  bool foundJet=false;
  float drEle, eta, et;
  for(int j=0;j<nJet;j++){
    TVector3 p3Jet(pxJet[j],pyJet[j],pzJet[j]);

    // check if the electron falls into the jet (the electron IS a jet)
    std::vector<float> deltaR;
    deltaR.push_back(p3Jet.DeltaR(_p3Ele));
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaR[0])) continue;
    //    std::cout << "possible candidate of jet: deltaR from Ele = " << deltaR[0] << "   and from pos = " << deltaR[1] << std::endl;
    //    std::cout << "energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    // jet veto
    if(_selection->getSwitch("etaJetAcc") && !_selection->passCut("etaJetAcc",etaJet[j])) continue;
    if(_selection->getSwitch("ptJetAcc") && !_selection->passCut("ptJetAcc",etJet[j])) continue;
    drEle = deltaR[0];
    et =etJet[j];
    eta=etaJet[j];
    //    std::cout << "passed: energy = " << etJet[j] << "eta = " << etaJet[j] << std::endl;
    foundJet=true;
  }
  return foundJet;
}

float WplusJets::Fisher(int eleIndex) {
  float fisher;
  if(classificationEle[eleIndex]<100)
    fisher = 42.0238-3.38943*s9s25Ele[eleIndex]-794.092*sqrt(covEtaEtaEle[eleIndex])-15.3449*latEle[eleIndex]-31.1032*a20Ele[eleIndex];
  else
    fisher = 27.2967+2.97453*s9s25Ele[eleIndex]-169.219*sqrt(covEtaEtaEle[eleIndex])-17.0445*latEle[eleIndex]-24.8542*a20Ele[eleIndex];
  return fisher;
}

void WplusJets::bookHistos() {

  _monitorEventAfterReco->book1D("deltaPhiReco","#Delta #phi of e^{+}e^{-}",_deltaPhi,50,0.,TMath::Pi());
  _monitorEventAfterReco->book1D("WWtrMassReco","W^{+}W^{-} transverse mass (GeV/c^{2})",_transvMass,50,0,250);

  _monitorEventAfterSelection->book1D("deltaPhiSel","#Delta #phi of e^{+}e^{-}",_deltaPhi,50,0.,TMath::Pi());
  _monitorEventAfterSelection->book1D("WWtrMassSel","W^{+}W^{-} transverse mass (GeV/c^{2})",_transvMass,50,0,250);

  // PDF histograms
  _EoverPoutPDF = new TH1F("EoverPoutPDF","EoverPoutPDF",100,0.0,4.0);
  _deltaPhiPDF  = new TH1F("deltaPhiPDF", "deltaPhiPDF",100,-0.1,0.1);
  _deltaEtaPDF  = new TH1F("deltaEtaPDF", "deltaEtaPDF",100,-0.02,0.02);
  _HoverEPDF  = new TH1F("HoverEPDF", "HoverEPDF",50,-0.1,0.1);
  _shapeFisherPDF  = new TH1F("shapeFisherPDF", "shapeFisherPDF",100,-15.0,15.0);

  // Efficiencies
  fakeRecoEta = new TH1F("fakeRecoEta","fake reco", 25, -2.5, 2.5);
  fakeIdentifiedEta = new TH1F("fakeIdentifiedEta","fake electron mis-identified", 25, -2.5, 2.5);
  fakeGenEta = new TH1F("fakeGenEta","fake generated", 25, -2.5, 2.5);

  fakeRecoPt = new TH1F("fakeRecoPt","fake reco", 25, 0., 250);
  fakeIdentifiedPt = new TH1F("fakeIdentifiedPt","fake electron mis-identified", 25, 0., 250);
  fakeGenPt = new TH1F("fakeGenPt","fake generated", 25, 0., 250);

}

std::vector<int> WplusJets::findProbe(float deltaR) {
  float lowestDr=999.;
  std::vector<int> probeList;
  for(int iele=0; iele<nEle; iele++) {
    TVector3 pCandProbe(pxEle[iele],pyEle[iele],pzEle[iele]);
    // exclude the real jet reco as electron
    float deltaRjetVeto = pCandProbe.Angle(_p3Jet);
    if(_selection->getSwitch("jetConeWidth") && _selection->passCut("jetConeWidth",deltaRjetVeto)) continue;
    // check that electron is inside the conic corona
    float deltaRjetele = pCandProbe.Angle(_p3Jet);
    if(deltaRjetele<deltaR) probeList.push_back(iele);
    if(deltaRjetele<lowestDr) {
      _bestProbe = iele;
      lowestDr = deltaRjetele;
    }
  }
  return probeList;
}
