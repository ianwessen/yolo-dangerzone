
#include <iostream>
#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "HiggsAnalysisTools/include/kFactorEvaluator.hh"
#include "HiggsAnalysisTools/include/WewkSelection.hh"
#include "CommonTools/include/Utils.hh"

/* 
// taken from PhysicsTools/HepMCCandAlgos/plugins/CSA07EventWeightProducer.cc:

// electron enriched from bbbar 5 < pt_hat < 50   
if ( (filter_eff == 0.00019) && (processID == 11 || processID == 12 || processID == 13 || processID == 28 || processID == 68 || processID == 53 || processID == 95)) {   (*weight) = cross_section * filter_eff / 3.E+06 ; 
// number = effective cross section in 1 pb-1 div by 3M    }  

// electron enriched from bbbar 50 < pt_hat < 170   
if ( (filter_eff == 0.0068) && (processID == 11 || processID == 12 || processID == 13 || processID == 28 || processID == 68 || processID == 53 || processID == 95)) {   (*weight) = cross_section * filter_eff / 3.E+06 ; 
// number = effective cross section in 1 pb-1 div by 3M    }  

// electron enriched from bbbar 170 < pt_hat   
if ( (filter_eff == 0.0195) && (processID == 11 || processID == 12 || processID == 13 || processID == 28 || processID == 68 || processID == 53 || processID == 95)) {   (*weight) = cross_section * filter_eff / 2.6E+06 ; 
// number = effective cross section in 1 pb-1 div by 3M    }
*/

WewkSelection::WewkSelection(TTree *tree) 
  : HiggsBase(tree) {

  // Initialize kine selection
  std::string fileCuts     = "config/noteEWK/wenuCuts.txt";
  std::string fileSwitches = "config/noteEWK/wenuSwitches.txt";

  _selection = new Selection(fileCuts,fileSwitches);
  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addSwitch("apply_weight");
  _selection->addCut("nRecoEle");
  _selection->addCut("etaEleAcc");
  _selection->addCut("etaCrack1");
  _selection->addCut("etaCrack2");
  _selection->addCut("ptEleAcc");
  _selection->addCut("trackerPtSum");
  _selection->addSwitch("golden");
  _selection->addSwitch("goldenID");
  _selection->addSwitch("robustID");
  _selection->addCut("MET1");
  _selection->addCut("MET2");

  // EleID selection
  Selection *_goldenSelectionEB = new Selection("config/noteEWK/electronIDGoldenCutsEB.txt","config/noteEWK/goldenIDSwitches.txt");
  Selection *_robustSelectionEB = new Selection("config/noteEWK/electronIDRobustCutsEB.txt","config/noteEWK/robustIDSwitches.txt");
  Selection *_goldenSelectionEE = new Selection("config/noteEWK/electronIDGoldenCutsEE.txt","config/noteEWK/goldenIDSwitches.txt");
  Selection *_robustSelectionEE = new Selection("config/noteEWK/electronIDRobustCutsEE.txt","config/noteEWK/robustIDSwitches.txt");

  _eleGoldenSelection.push_back(_goldenSelectionEB);
  _eleGoldenSelection.push_back(_goldenSelectionEE);
  _eleRobustSelection.push_back(_robustSelectionEB);
  _eleRobustSelection.push_back(_robustSelectionEE);

  std::vector<Selection*>::iterator goldenSelItr;
  for(goldenSelItr=_eleGoldenSelection.begin();goldenSelItr!=_eleGoldenSelection.end();goldenSelItr++) {
    Selection *eleGSelection = *goldenSelItr;    
    eleGSelection->addCut("hOverE");
    eleGSelection->addCut("s9s25");
    eleGSelection->addCut("deta");
    eleGSelection->addCut("dphiIn");
    eleGSelection->addCut("dphiOut");
    eleGSelection->addCut("invEMinusInvP");
    eleGSelection->addCut("bremFraction");
    eleGSelection->addCut("covEtaEta");
    eleGSelection->addCut("covPhiPhi");
    eleGSelection->addCut("eOverPout");   
    eleGSelection->addCut("eOverPin");
    eleGSelection->summary();
  }

  std::vector<Selection*>::iterator robSelItr;
  for(robSelItr=_eleRobustSelection.begin();robSelItr!=_eleRobustSelection.end();robSelItr++) {
    Selection *eleRSelection = *robSelItr;
    eleRSelection->addCut("hOverE");
    eleRSelection->addCut("deta");
    eleRSelection->addCut("dphiIn");
    eleRSelection->addCut("covEtaEta");
    eleRSelection->summary();
  }

  std::map<std::string,std::pair<float,float> > selectionMap = _selection->getSelection();
  _selection->summary();

  // Initialize additional counters...
  _counter.SetTitle("EVENT COUNTER");
  _counter.AddVar("event");
  _counter.AddVar("MCtruth");
  _counter.AddVar("eveHLT");
  _counter.AddVar("eleReco");
  _counter.AddVar("eleEta");
  _counter.AddVar("eleCrack");
  _counter.AddVar("elePt");
  _counter.AddVar("eleIso");
  _counter.AddVar("eleGolden");
  _counter.AddVar("eleGoldenID");
  _counter.AddVar("eleRobustID");
  _counter.AddVar("met1");
  _counter.AddVar("met2");
  _counter.AddVar("final");
  
  _eleCounter.SetTitle("SINGLE ELECTRON COUNTER");
  _eleCounter.AddVar("electrons");
  _eleCounter.AddVar("hOverE");
  _eleCounter.AddVar("s9s25");
  _eleCounter.AddVar("deta");
  _eleCounter.AddVar("dphiIn");
  _eleCounter.AddVar("dphiOut");
  _eleCounter.AddVar("invEMinusInvP");
  _eleCounter.AddVar("bremFraction");
  _eleCounter.AddVar("covEtaEta");
  _eleCounter.AddVar("covPhiPhi");
  _eleCounter.AddVar("eOverPout");
  _eleCounter.AddVar("eOverPin");
  _eleCounter.AddVar("finalEleID");
}

WewkSelection::~WewkSelection(){
  myOutTree -> save();
}

bool WewkSelection::findMcTree(const char* processType) {

  bool processOk = true;
  
  /*
    bool processOk = false;  
    if(strcmp(processType,"bbar_550")==0) {
    if ( (genFilterEff == 0.00019) 
    && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
    && (genPtHat > 5 && genPtHat < 50)
    )	 
    { processOk = true; }
    }
    
  if(strcmp(processType,"bbar_50170")==0) {
  if ( (genFilterEff == 0.0068) 
  && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
  && (genPtHat > 50 && genPtHat < 170)
  ){ processOk = true; }
  }
  
  if (strcmp(processType,"bbar_170up")==0) {
  if ( (genFilterEff == 0.0195) 
  && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
  && (genPtHat > 170)
  ){ processOk = true; }
  }
  */
  
  return processOk;
}


void WewkSelection::Loop() {
  _verbose=true;
  if(fChain == 0) return;

  myOutTree = new RedEWKTree("outWewkTree.root");
  
  // tree
  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  if(_verbose) std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (_verbose && jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

    // get the weight of the event for the soup
    
    float weight = 1.0;
    //    if(_selection->getSwitch("apply_weight")) weight = genWeight;
    
    _counter.IncrVar("event",weight);

    // event type for the soup
    bool foundMcTree = findMcTree("bbar_170up");
    if(_selection->getSwitch("MCtruth") && 
       !foundMcTree ) continue;              
    _counter.IncrVar("MCtruth",weight);


    Utils anaUtils;
    if(_selection->getSwitch("trigger") && !( anaUtils.getTriggersOR(m_requiredTriggers, firedTrg) ) ) continue;
    _counter.IncrVar("eveHLT",weight);

    //    if(_selection->getSwitch("trigger") && !(singleElePassedTrg)) continue;
    //_counter.IncrVar("eveHLT",weight); 
    
    if(!_selection->passCut("nRecoEle",nEle)) continue;   	
    _counter.IncrVar("eleReco",weight);
    
    int theEle = getBestLepton();
    if(theEle<0) {
      std::cout << "mah, non dovrebbe succedere mai" << std::endl;
      continue; 
    }
        
    addVariables();

    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",etEle[theEle]) ) continue;
    _counter.IncrVar("elePt",weight);

    if(_selection->getSwitch("etaEleAcc") && !_selection->passCut("etaEleAcc",etaEle[theEle]) ) continue;
    _counter.IncrVar("eleEta",weight);

    float absEta = fabs(etaEle[theEle]);
    if(_selection->getSwitch("etaCrack1") && 
       _selection->getSwitch("etaCrack2") && 
       !_selection->passCut("etaCrack1",absEta) &&
       !_selection->passCut("etaCrack2",absEta) 
       ) continue;
    _counter.IncrVar("eleCrack",weight);

    float myIso = eleSumPt04Ele[theEle];
    if(_selection->getSwitch("trackerPtSum") && 
       ( !_selection->passCut("trackerPtSum",myIso) )) continue;
    _counter.IncrVar("eleIso",weight);

    bool isGolden = false;
    if((classificationEle[theEle]==0) || (classificationEle[theEle]==100)) isGolden = true; 
    if(_selection->getSwitch("golden") && !isGolden) continue;   
    _counter.IncrVar("eleGolden",weight);

    if(_selection->getSwitch("goldenID") && !isGoldenID(theEle)) continue;   
    _counter.IncrVar("eleGoldenID",weight);

    if(_selection->getSwitch("robustID") && !isRobustID(theEle)) continue;   
    _counter.IncrVar("eleRobustID",weight);

    float met =etMet[0];
    float metx=pxMet[0];
    float mety=pyMet[0];
    myOutTree -> fillEleId(eleSumPt04Ele[theEle], eleTipEle[theEle], pzEle[theEle], covEtaEtaEle[theEle], deltaEtaAtVtxEle[theEle], deltaPhiAtVtxEle[theEle], eSuperClusterOverPEle[theEle], hOverEEle[theEle], chargeEle[theEle], etEle[theEle], etaEle[theEle], phiEle[theEle]); 
    myOutTree -> fillAll(1, 1. , met, metx, mety); 
    // myOutTree -> fillAll(1, genWeight, met, metx, mety); 
    myOutTree -> store();

    if(_selection->getSwitch("MET1") && !_selection->passCut("MET1",met)) continue; 
    _counter.IncrVar("met1",weight);

    if(_selection->getSwitch("MET2") && !_selection->passCut("MET2",met)) continue; 
    _counter.IncrVar("met2",weight);
    
    _counter.IncrVar("final",weight);
  }
}

void WewkSelection::displayEfficiencies() {
  _eleCounter.Draw();
  _eleCounter.Draw("hOverE","electrons");
  _eleCounter.Draw("s9s25","hOverE");
  _eleCounter.Draw("deta","s9s25");
  _eleCounter.Draw("dphiIn","deta");
  _eleCounter.Draw("dphiOut","dphiIn");
  _eleCounter.Draw("invEMinusInvP","dphiOut");
  _eleCounter.Draw("bremFraction","invEMinusInvP");
  _eleCounter.Draw("covEtaEta","bremFraction");
  _eleCounter.Draw("covPhiPhi","covEtaEta");
  _eleCounter.Draw("eOverPout","covPhiPhi");
  _eleCounter.Draw("eOverPin","eOverPout");
  _eleCounter.Draw("finalEleID","eOverPin");
  _eleCounter.Draw("finalEleID","electrons");

  _counter.Draw();
  _counter.Draw("MCtruth",     "event");
  _counter.Draw("eveHLT",      "MCtruth");
  _counter.Draw("eleReco",     "eveHLT");
  _counter.Draw("elePt",       "eleReco");
  _counter.Draw("eleEta",      "elePt");
  _counter.Draw("eleCrack",    "eleEta");
  _counter.Draw("eleIso",      "eleCrack");
  _counter.Draw("eleGolden",   "eleIso");
  _counter.Draw("eleGoldenID", "eleGolden");
  _counter.Draw("eleRobustID", "eleGoldenID");
  _counter.Draw("met1",        "eleRobustID");
  _counter.Draw("met2",        "met1");
  _counter.Draw("final",       "met2");
  _counter.Draw("final",       "MCtruth");
}

int WewkSelection::getBestLepton() {
  int theEle  = -1;
  float maxPt = -1000.;
  for(int i=0;i<nEle;i++) {
    TVector3 pEle(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pEle.Pt();
    if (thisPt> maxPt){ maxPt = thisPt; theEle = i; }
  }
  return theEle;
}

bool WewkSelection::isGoldenID(int eleIndex) {

  TVector3 pTrkAtInner(pxAtInnerEle[eleIndex],pyAtInnerEle[eleIndex],pzAtInnerEle[eleIndex]);
  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  int GsfClass = classificationEle[eleIndex];
  Selection *selection;
  if(GsfClass<100)       selection=_eleGoldenSelection[0];
  else if(GsfClass>=100) selection=_eleGoldenSelection[1];
  
  _eleCounter.IncrVar("electrons");

  if(selection->getSwitch("hOverE")  && !selection->passCut("hOverE",hOverEEle[eleIndex])) return false;  
 _eleCounter.IncrVar("hOverE");

  if(selection->getSwitch("s9s25")   && !selection->passCut("s9s25",s9s25Ele[eleIndex])) return false; 
  _eleCounter.IncrVar("s9s25"); 

  if(selection->getSwitch("deta")    && !selection->passCut("deta",deltaEtaAtVtxEle[eleIndex])) return false; 
  _eleCounter.IncrVar("deta");

  if(selection->getSwitch("dphiIn")  && !selection->passCut("dphiIn",deltaPhiAtVtxEle[eleIndex])) return false; 
  _eleCounter.IncrVar("dphiIn");

  if(selection->getSwitch("dphiOut") && !selection->passCut("dphiOut",deltaPhiAtCaloEle[eleIndex])) return false; 
  _eleCounter.IncrVar("dphiOut");

  if(selection->getSwitch("invEMinusInvP") && !selection->passCut("invEMinusInvP",1./fabs(ecalEle[eleIndex]-1./pTrkAtInner.Mag()))) return false; 
  _eleCounter.IncrVar("invEMinusInvP");

  if(selection->getSwitch("bremFraction") && 
     !selection->passCut("bremFraction",fabs(pTrkAtInner.Mag()-pTrkAtOuter.Mag())/pTrkAtInner.Mag())) return false; 
  _eleCounter.IncrVar("bremFraction");

  if(selection->getSwitch("covEtaEta") && !selection->passCut("covEtaEta",covEtaEtaEle[eleIndex])) return false; 
  _eleCounter.IncrVar("covEtaEta");

  if(selection->getSwitch("covPhiPhi") && !selection->passCut("covPhiPhi",covPhiPhiEle[eleIndex])) return false; 
  _eleCounter.IncrVar("covPhiPhi");

  if(selection->getSwitch("eOverPout") && !selection->passCut("eOverPout",eSeedOverPoutEle[eleIndex])) return false; 
  _eleCounter.IncrVar("eOverPout");

  if(selection->getSwitch("eOverPin")  && !selection->passCut("eOverPin",eSuperClusterOverPEle[eleIndex])) return false; 
  _eleCounter.IncrVar("eOverPin");

  _eleCounter.IncrVar("finalEleID");
  return true;
}

bool WewkSelection::isRobustID(int eleIndex) {
  
  int GsfClass = classificationEle[eleIndex];
  Selection *selection;
  if(GsfClass <100)      selection=_eleRobustSelection[0];
  else if(GsfClass>=100) selection=_eleRobustSelection[1];
  
  _eleCounter.IncrVar("electrons");

  if(selection->getSwitch("hOverE")    && !selection->passCut("hOverE",hOverEEle[eleIndex])) return false;  
  _eleCounter.IncrVar("hOverE");

  if(selection->getSwitch("deta")      && !selection->passCut("deta",deltaEtaAtVtxEle[eleIndex])) return false; 
  _eleCounter.IncrVar("deta");

  if(selection->getSwitch("dphiIn")    && !selection->passCut("dphiIn",deltaPhiAtVtxEle[eleIndex])) return false; 
  _eleCounter.IncrVar("dphiIn");

  if(selection->getSwitch("covEtaEta") && !selection->passCut("covEtaEta",covEtaEtaEle[eleIndex])) return false; 
  _eleCounter.IncrVar("covEtaEta");

  _eleCounter.IncrVar("finalEleID");
  return true;
}

void WewkSelection::addVariables() {
  for(int i=0;i<nEle;i++) {
    _eOverP[i]=ecalEle[i]/energyEle[i];
  }
}
