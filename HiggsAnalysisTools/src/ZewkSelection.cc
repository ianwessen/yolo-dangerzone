#include <iostream>
#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"   
#include "HiggsAnalysisTools/include/kFactorEvaluator.hh"
#include "HiggsAnalysisTools/include/ZewkSelection.hh"

ZewkSelection::ZewkSelection(TTree *tree) 
  : HiggsBase(tree) {

  // Initialize kine selection
  std::string fileCuts     = "config/noteEWK/zeeCuts.txt";
  std::string fileSwitches = "config/noteEWK/zeeSwitches.txt";

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
  _selection->addSwitch("TwoGolden");
  _selection->addSwitch("TwoGoldenID");
  _selection->addSwitch("OneTightGolden");
  _selection->addSwitch("robustID");
  _selection->addCut("Mee");

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
  _counter.AddVar("eleTwoGolden");
  _counter.AddVar("eleTwoGoldenID");
  _counter.AddVar("eleOneTightGolden");
  _counter.AddVar("eleRobustID");
  _counter.AddVar("mee");
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

  // the kinematic vectors
  _p4Ele = new TLorentzVector(0.,0.,0.,0.);
  _p4Pos = new TLorentzVector(0.,0.,0.,0.);
}

ZewkSelection::~ZewkSelection(){
  delete _p4Ele;
  delete _p4Pos;
}

bool ZewkSelection::findMcTree(const char* processType) {
  
//   bool processOk = false;  
//   if(strcmp(processType,"bbar_550")==0) {
//     if ( (genFilterEff == 0.00019) 
// 	 && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
// 	 && (genPtHat > 5 && genPtHat < 50)
// 	 )	 
//       { processOk = true; }
//   }
  
//   if(strcmp(processType,"bbar_50170")==0) {
//     if ( (genFilterEff == 0.0068) 
// 	 && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
// 	 && (genPtHat > 50 && genPtHat < 170)
// 	 ){ processOk = true; }
//   }

//   if (strcmp(processType,"bbar_170up")==0) {
//     if ( (genFilterEff == 0.0195) 
// 	 && (genProcessId == 11 || genProcessId == 12 || genProcessId == 13 || genProcessId == 28 || genProcessId == 68 || genProcessId == 53 || genProcessId == 95)
// 	 && (genPtHat > 170)
// 	 ){ processOk = true; }
//   }
  
//  return processOk;

  return true;

}

void ZewkSelection::Loop() {
  _verbose=true;
  if(fChain == 0) return;

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
    if(_selection->getSwitch("apply_weight")) weight = 1.0;

    _counter.IncrVar("event",weight);
    
    // event type for the soup
    bool foundMcTree = findMcTree("bbar_550");
    if(_selection->getSwitch("MCtruth") && 
       !foundMcTree ) continue;              
    _counter.IncrVar("MCtruth",weight);

    Utils anaUtils;
    if(_selection->getSwitch("trigger") && !( anaUtils.getTriggersOR(m_requiredTriggers, firedTrg) ) ) continue;
    _counter.IncrVar("eveHLT",weight); 
    
    
    if(!_selection->passCut("nRecoEle",nEle)) continue;   	
    _counter.IncrVar("eleReco",weight);
    int theEle(getBestLeptonPair().first);
    int thePos(getBestLeptonPair().second);
    
    if(theEle<0 || thePos<0) {
      std::cout << "mah, non dovrebbe succedere mai" << std::endl;
      continue; 
    }

    setKinematics(theEle, thePos);
    addVariables();

    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",etEle[theEle]) ) continue;
    if(_selection->getSwitch("ptEleAcc") && !_selection->passCut("ptEleAcc",etEle[thePos]) ) continue;
    _counter.IncrVar("elePt",weight);
    
    if(_selection->getSwitch("etaEleAcc") && !_selection->passCut("etaEleAcc",etaEle[theEle]) ) continue;
    if(_selection->getSwitch("etaEleAcc") && !_selection->passCut("etaEleAcc",etaEle[thePos]) ) continue;
    _counter.IncrVar("eleEta",weight);

    float absEleEta = fabs(etaEle[theEle]);
    float absPosEta = fabs(etaEle[thePos]);
    if(_selection->getSwitch("etaCrack1") &&
       _selection->getSwitch("etaCrack2") &&
       !_selection->passCut("etaCrack1",absEleEta) &&
       !_selection->passCut("etaCrack2",absEleEta)
       ) continue;
    if(_selection->getSwitch("etaCrack1") &&
       _selection->getSwitch("etaCrack2") &&
       !_selection->passCut("etaCrack1",absPosEta) &&
       !_selection->passCut("etaCrack2",absPosEta)
       ) continue;
    _counter.IncrVar("eleCrack",weight);

    float myEleIso = eleSumPt04Ele[theEle];
    float myPosIso = eleSumPt04Ele[thePos];
    if(_selection->getSwitch("trackerPtSum") &&
       ( !_selection->passCut("trackerPtSum",myEleIso) )) continue;
    if(_selection->getSwitch("trackerPtSum") &&
       ( !_selection->passCut("trackerPtSum",myPosIso) )) continue;
    _counter.IncrVar("eleIso",weight);

    bool isEleGolden = false;
    bool isPosGolden = false;
    if((classificationEle[theEle]==0) || (classificationEle[theEle]==100)) isEleGolden = true; 
    if((classificationEle[thePos]==0) || (classificationEle[thePos]==100)) isPosGolden = true; 
    if(_selection->getSwitch("TwoGolden") && !isEleGolden) continue;
    if(_selection->getSwitch("TwoGolden") && !isPosGolden) continue;
    _counter.IncrVar("eleTwoGolden",weight);

    if(_selection->getSwitch("TwoGoldenID") && !isGoldenID(theEle)) continue;
    if(_selection->getSwitch("TwoGoldenID") && !isGoldenID(thePos)) continue;
    _counter.IncrVar("eleTwoGoldenID",weight);
    
    bool noGoldenEle = false;
    bool noGoldenPos = false;
    if(!isGoldenID(theEle)) noGoldenEle = true;
    if(!isGoldenID(thePos)) noGoldenPos = true;
    if(_selection->getSwitch("OneTightGolden") && noGoldenEle && noGoldenPos) continue;
    _counter.IncrVar("eleOneTightGolden",weight);
    
    if(_selection->getSwitch("robustID") && (!isRobustID(theEle) || !isRobustID(thePos)) ) continue;   
    _counter.IncrVar("eleRobustID",weight);

    float mee = (*_p4Ele+*_p4Pos).M();
    if(_selection->getSwitch("Mee") && !_selection->passCut("Mee",mee)) continue; 
    _counter.IncrVar("mee",weight);

    _counter.IncrVar("final",weight);
  }
}

void ZewkSelection::displayEfficiencies() {
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
  _counter.Draw("MCtruth",             "event");
  _counter.Draw("eveHLT",              "MCtruth");
  _counter.Draw("eleReco",             "eveHLT");
  _counter.Draw("elePt",               "eleReco");
  _counter.Draw("eleEta",              "elePt");
  _counter.Draw("eleCrack",            "eleEta");
  _counter.Draw("eleIso",              "eleCrack");
  _counter.Draw("eleTwoGolden",        "eleIso");
  _counter.Draw("eleTwoGoldenID",      "eleTwoGolden");
  _counter.Draw("eleOneTightGoldenID", "eleTwoGoldenID");
  _counter.Draw("eleRobustID",         "eleOneTightGolden");
  _counter.Draw("mee",                 "eleRobustID");
  _counter.Draw("final",               "mee");
  _counter.Draw("final",               "MCtruth");
}

// also charge requirement?
std::pair<int,int> ZewkSelection::getBestLeptonPair() {
  int theEle1=-1;
  int theEle2=-1;
  float maxPt1=-1000.;
  float maxPt2=-1001.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nEle;i++) {
    TVector3 pEle(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pEle.Pt();
    if (thisPt>maxPt1 && thisPt>maxPt2){ maxPt2 = maxPt1; maxPt1 = thisPt; theEle2 = theEle1; theEle1 = i; }
    if (thisPt<maxPt1 && thisPt>maxPt2){ maxPt2 = thisPt; theEle2 = i; }
  }
  return make_pair(theEle1,theEle2);
}

bool ZewkSelection::isGoldenID(int eleIndex) {

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

bool ZewkSelection::isRobustID(int eleIndex) {
  
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

void ZewkSelection::addVariables() {
  for(int i=0;i<nEle;i++) {
    _eOverP[i]=ecalEle[i]/energyEle[i];
  }
}

void ZewkSelection::setKinematics(int theEle, int thePos) {
  _p4Ele->SetXYZT(pxEle[theEle],pyEle[theEle],pzEle[theEle],energyEle[theEle]);
  _p4Pos->SetXYZT(pxEle[thePos],pyEle[thePos],pzEle[thePos],energyEle[thePos]);
}
