#include <iostream>
#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "HiggsAnalysisTools/include/ClassEfficiencyStudy.hh"

ClassEfficiencyStudy::ClassEfficiencyStudy(TTree *tree, const char* ECALSection) 
  : HiggsBase(tree) {
  
  _ECALSection = "EB";
  ECALSection = _ECALSection;

  // Initialize selection
  std::string fileGoldenCuts((strcmp(ECALSection,"EB")==0) ? "config/higgs/electronIDGoldenCutsEB.txt" : "config/higgs/electronIDGoldenCutsEE.txt");
  std::string fileBigBremCuts((strcmp(ECALSection,"EB")==0) ? "config/higgs/electronIDBigBremCutsEB.txt" : "config/higgs/electronIDBigBremCutsEE.txt");
  std::string fileNarrowCuts((strcmp(ECALSection,"EB")==0) ? "config/higgs/electronIDNarrowCutsEB.txt" : "config/higgs/electronIDNarrowCutsEE.txt");
  std::string fileShoweringCuts((strcmp(ECALSection,"EB")==0) ? "config/higgs/electronIDShoweringCutsEB.txt" : "config/higgs/electronIDShoweringCutsEE.txt");
  std::string fileSwitches("config/higgs/electronIDSwitches.txt");
  _goldenSelection = new Selection(fileGoldenCuts,fileSwitches);
  _bigbremSelection = new Selection(fileBigBremCuts,fileSwitches);
  _narrowSelection = new Selection(fileNarrowCuts,fileSwitches);
  _showeringSelection = new Selection(fileShoweringCuts,fileSwitches);

  _goldenSelection->addCut("nRecoEle");
  _goldenSelection->addCut("hOverE");
  _goldenSelection->addCut("s9s25");
  _goldenSelection->addCut("deta");
  _goldenSelection->addCut("dphiIn");
  _goldenSelection->addCut("dphiOut");
  _goldenSelection->addCut("covEtaEta");
  _goldenSelection->addCut("eOverPout");
  _goldenSelection->addCut("Fisher");
  _goldenSelection->addCut("Likelihood");

  _bigbremSelection->addCut("nRecoEle");
  _bigbremSelection->addCut("hOverE");
  _bigbremSelection->addCut("s9s25");
  _bigbremSelection->addCut("deta");
  _bigbremSelection->addCut("dphiIn");
  _bigbremSelection->addCut("dphiOut");
  _bigbremSelection->addCut("covEtaEta");
  _bigbremSelection->addCut("eOverPout");
  _bigbremSelection->addCut("Fisher");
  _bigbremSelection->addCut("Likelihood");

  _narrowSelection->addCut("nRecoEle");
  _narrowSelection->addCut("hOverE");
  _narrowSelection->addCut("s9s25");
  _narrowSelection->addCut("deta");
  _narrowSelection->addCut("dphiIn");
  _narrowSelection->addCut("dphiOut");
  _narrowSelection->addCut("covEtaEta");
  _narrowSelection->addCut("eOverPout");
  _narrowSelection->addCut("Fisher");
  _narrowSelection->addCut("Likelihood");

  _showeringSelection->addCut("nRecoEle");
  _showeringSelection->addCut("hOverE");
  _showeringSelection->addCut("s9s25");
  _showeringSelection->addCut("deta");
  _showeringSelection->addCut("dphiIn");
  _showeringSelection->addCut("dphiOut");
  _showeringSelection->addCut("covEtaEta");
  _showeringSelection->addCut("eOverPout");
  _showeringSelection->addCut("Fisher");
  _showeringSelection->addCut("Likelihood");

  _goldenSelection->summary();
  _bigbremSelection->summary();
  _narrowSelection->summary();
  _showeringSelection->summary();

  // Initialize additional counters...
  _goldenCounter = new Counters("GOLDEN");
  _bigbremCounter = new Counters("BIGBREM");
  _narrowCounter = new Counters("NARROW");
  _showeringCounter = new Counters("SHOWERING");
  _globalCounter = new Counters("GLOBAL");

  _showeringCounter->AddVar("event");

  _goldenCounter->AddVar("nRecoEle");
  _goldenCounter->AddVar("hOverE");
  _goldenCounter->AddVar("s9s25");
  _goldenCounter->AddVar("deta");
  _goldenCounter->AddVar("dphiIn");
  _goldenCounter->AddVar("dphiOut");
  _goldenCounter->AddVar("covEtaEta");
  _goldenCounter->AddVar("eOverPout");
  _goldenCounter->AddVar("Fisher");
  _goldenCounter->AddVar("Likelihood");
  _goldenCounter->AddVar("final");

  _bigbremCounter->AddVar("nRecoEle");
  _bigbremCounter->AddVar("hOverE");
  _bigbremCounter->AddVar("s9s25");
  _bigbremCounter->AddVar("deta");
  _bigbremCounter->AddVar("dphiIn");
  _bigbremCounter->AddVar("dphiOut");
  _bigbremCounter->AddVar("covEtaEta");
  _bigbremCounter->AddVar("eOverPout");
  _bigbremCounter->AddVar("Fisher");
  _bigbremCounter->AddVar("Likelihood");
  _bigbremCounter->AddVar("final");

  _narrowCounter->AddVar("nRecoEle");
  _narrowCounter->AddVar("hOverE");
  _narrowCounter->AddVar("s9s25");
  _narrowCounter->AddVar("deta");
  _narrowCounter->AddVar("dphiIn");
  _narrowCounter->AddVar("dphiOut");
  _narrowCounter->AddVar("covEtaEta");
  _narrowCounter->AddVar("eOverPout");
  _narrowCounter->AddVar("Fisher");
  _narrowCounter->AddVar("Likelihood");
  _narrowCounter->AddVar("final");

  _showeringCounter->AddVar("nRecoEle");
  _showeringCounter->AddVar("hOverE");
  _showeringCounter->AddVar("s9s25");
  _showeringCounter->AddVar("deta");
  _showeringCounter->AddVar("dphiIn");
  _showeringCounter->AddVar("dphiOut");
  _showeringCounter->AddVar("covEtaEta");
  _showeringCounter->AddVar("eOverPout");
  _showeringCounter->AddVar("Fisher");
  _showeringCounter->AddVar("Likelihood");
  _showeringCounter->AddVar("final");

  _globalCounter->AddVar("nRecoEle");
  _globalCounter->AddVar("final");

}

ClassEfficiencyStudy::~ClassEfficiencyStudy() {
  delete _goldenSelection;
  delete _bigbremSelection;
  delete _narrowSelection;
  delete _showeringSelection;
}

void ClassEfficiencyStudy::Loop() {
  _verbose=true;
  if(fChain == 0) return;

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  if(_verbose) std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (_verbose && jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;
      
      // loop over electrons
      for(int iele=0; iele < nEle; iele++) {
	if(isClass(iele,"golden")) { 
	  _goldenCounter->IncrVar("nRecoEle"); _globalCounter->IncrVar("nRecoEle");
	  if(_goldenSelection->getSwitch("hOverE") && !_goldenSelection->passCut("hOverE",hOverEEle[iele])) continue; _goldenCounter->IncrVar("hOverE");
	  if(_goldenSelection->getSwitch("s9s25") && !_goldenSelection->passCut("s9s25",s9s25Ele[iele])) continue; _goldenCounter->IncrVar("s9s25");
	  if(_goldenSelection->getSwitch("deta") && !_goldenSelection->passCut("deta",deltaEtaAtVtxEle[iele])) continue; _goldenCounter->IncrVar("deta");
	  if(_goldenSelection->getSwitch("dphiIn") && !_goldenSelection->passCut("dphiIn",deltaPhiAtVtxEle[iele])) continue; _goldenCounter->IncrVar("dphiIn");
	  if(_goldenSelection->getSwitch("dphiOut") && !_goldenSelection->passCut("dphiOut",deltaPhiAtCaloEle[iele])) continue; _goldenCounter->IncrVar("dphiOut");
	  if(_goldenSelection->getSwitch("covEtaEta") && !_goldenSelection->passCut("covEtaEta",covEtaEtaEle[iele])) continue; _goldenCounter->IncrVar("covEtaEta");
	  if(_goldenSelection->getSwitch("eOverPout") && !_goldenSelection->passCut("eOverPout",eleNotCorrEoPoutEle[iele])) continue; _goldenCounter->IncrVar("eOverPout");
	  if(_goldenSelection->getSwitch("Fisher") && !_goldenSelection->passCut("Fisher",Fisher(iele))) continue; _goldenCounter->IncrVar("Fisher");
	  if(_goldenSelection->getSwitch("Likelihood") && !_goldenSelection->passCut("Likelihood",eleIdLikelihoodEle[iele])) continue; _goldenCounter->IncrVar("Likelihood");
	  _goldenCounter->IncrVar("final"); _globalCounter->IncrVar("final");
	}
	else if(isClass(iele,"bigbrem")) {
	  _bigbremCounter->IncrVar("nRecoEle"); _globalCounter->IncrVar("nRecoEle");
	  if(_bigbremSelection->getSwitch("hOverE") && !_bigbremSelection->passCut("hOverE",hOverEEle[iele])) continue; _bigbremCounter->IncrVar("hOverE");
	  if(_bigbremSelection->getSwitch("s9s25") && !_bigbremSelection->passCut("s9s25",s9s25Ele[iele])) continue; _bigbremCounter->IncrVar("s9s25");
	  if(_bigbremSelection->getSwitch("deta") && !_bigbremSelection->passCut("deta",deltaEtaAtVtxEle[iele])) continue; _bigbremCounter->IncrVar("deta");
	  if(_bigbremSelection->getSwitch("dphiIn") && !_bigbremSelection->passCut("dphiIn",deltaPhiAtVtxEle[iele])) continue; _bigbremCounter->IncrVar("dphiIn");
	  if(_bigbremSelection->getSwitch("dphiOut") && !_bigbremSelection->passCut("dphiOut",deltaPhiAtCaloEle[iele])) continue; _bigbremCounter->IncrVar("dphiOut");
	  if(_bigbremSelection->getSwitch("covEtaEta") && !_bigbremSelection->passCut("covEtaEta",covEtaEtaEle[iele])) continue; _bigbremCounter->IncrVar("covEtaEta");
	  if(_bigbremSelection->getSwitch("eOverPout") && !_bigbremSelection->passCut("eOverPout",eleNotCorrEoPoutEle[iele])) continue; _bigbremCounter->IncrVar("eOverPout");
	  if(_bigbremSelection->getSwitch("Fisher") && !_bigbremSelection->passCut("Fisher",Fisher(iele))) continue; _bigbremCounter->IncrVar("Fisher");
	  if(_bigbremSelection->getSwitch("Likelihood") && !_bigbremSelection->passCut("Likelihood",eleIdLikelihoodEle[iele])) continue; _bigbremCounter->IncrVar("Likelihood");
	  _bigbremCounter->IncrVar("final"); _globalCounter->IncrVar("final");

	}
	else if(isClass(iele,"narrow")) {
	  _narrowCounter->IncrVar("nRecoEle"); _globalCounter->IncrVar("nRecoEle");
	  if(_narrowSelection->getSwitch("hOverE") && !_narrowSelection->passCut("hOverE",hOverEEle[iele])) continue; _narrowCounter->IncrVar("hOverE");
	  if(_narrowSelection->getSwitch("s9s25") && !_narrowSelection->passCut("s9s25",s9s25Ele[iele])) continue; _narrowCounter->IncrVar("s9s25");
	  if(_narrowSelection->getSwitch("deta") && !_narrowSelection->passCut("deta",deltaEtaAtVtxEle[iele])) continue; _narrowCounter->IncrVar("deta");
	  if(_narrowSelection->getSwitch("dphiIn") && !_narrowSelection->passCut("dphiIn",deltaPhiAtVtxEle[iele])) continue; _narrowCounter->IncrVar("dphiIn");
	  if(_narrowSelection->getSwitch("dphiOut") && !_narrowSelection->passCut("dphiOut",deltaPhiAtCaloEle[iele])) continue; _narrowCounter->IncrVar("dphiOut");
	  if(_narrowSelection->getSwitch("covEtaEta") && !_narrowSelection->passCut("covEtaEta",covEtaEtaEle[iele])) continue; _narrowCounter->IncrVar("covEtaEta");
	  if(_narrowSelection->getSwitch("eOverPout") && !_narrowSelection->passCut("eOverPout",eleNotCorrEoPoutEle[iele])) continue; _narrowCounter->IncrVar("eOverPout");
	  if(_narrowSelection->getSwitch("Fisher") && !_narrowSelection->passCut("Fisher",Fisher(iele))) continue; _narrowCounter->IncrVar("Fisher");
	  if(_narrowSelection->getSwitch("Likelihood") && !_narrowSelection->passCut("Likelihood",eleIdLikelihoodEle[iele])) continue; _narrowCounter->IncrVar("Likelihood");
	  _narrowCounter->IncrVar("final"); _globalCounter->IncrVar("final");
	}
	else if(isClass(iele,"showering")) {
	  _showeringCounter->IncrVar("nRecoEle"); _globalCounter->IncrVar("nRecoEle");
	  if(_showeringSelection->getSwitch("hOverE") && !_showeringSelection->passCut("hOverE",hOverEEle[iele])) continue; _showeringCounter->IncrVar("hOverE");
	  if(_showeringSelection->getSwitch("s9s25") && !_showeringSelection->passCut("s9s25",s9s25Ele[iele])) continue; _showeringCounter->IncrVar("s9s25");
	  if(_showeringSelection->getSwitch("deta") && !_showeringSelection->passCut("deta",deltaEtaAtVtxEle[iele])) continue; _showeringCounter->IncrVar("deta");
	  if(_showeringSelection->getSwitch("dphiIn") && !_showeringSelection->passCut("dphiIn",deltaPhiAtVtxEle[iele])) continue; _showeringCounter->IncrVar("dphiIn");
	  if(_showeringSelection->getSwitch("dphiOut") && !_showeringSelection->passCut("dphiOut",deltaPhiAtCaloEle[iele])) continue; _showeringCounter->IncrVar("dphiOut");
	  if(_showeringSelection->getSwitch("covEtaEta") && !_showeringSelection->passCut("covEtaEta",covEtaEtaEle[iele])) continue; _showeringCounter->IncrVar("covEtaEta");
	  if(_showeringSelection->getSwitch("eOverPout") && !_showeringSelection->passCut("eOverPout",eleNotCorrEoPoutEle[iele])) continue; _showeringCounter->IncrVar("eOverPout");
	  if(_showeringSelection->getSwitch("Fisher") && !_showeringSelection->passCut("Fisher",Fisher(iele))) continue; _showeringCounter->IncrVar("Fisher");
	  if(_showeringSelection->getSwitch("Likelihood") && !_showeringSelection->passCut("Likelihood",eleIdLikelihoodEle[iele])) continue; _showeringCounter->IncrVar("Likelihood");
	  _showeringCounter->IncrVar("final"); _globalCounter->IncrVar("final");
	}
      }
      
  }
}

bool ClassEfficiencyStudy::isClass(int eleInd, const char* gsfclass) {
  int offset=0;
  if(strcmp(_ECALSection,"EE")==0) offset=100;
  if(strcmp(gsfclass,"golden")==0) return (classificationEle[eleInd]==offset);
  else if(strcmp(gsfclass,"bigbrem")==0) return (classificationEle[eleInd]==offset+10);
  else if(strcmp(gsfclass,"narrow")==0) return (classificationEle[eleInd]==offset+20);
  else if(strcmp(gsfclass,"showering")==0) return (classificationEle[eleInd]>=offset+30 && classificationEle[eleInd]<=offset+34);
  else {
    std::cout << "Class : " << gsfclass << " not defined ! " << std::endl;
    return false;
  }
}

float ClassEfficiencyStudy::Fisher(int eleInd) {
  float Fisher=-999;
  if(strcmp(_ECALSection,"EB")==0)
    Fisher=42.0238-3.38943*s9s25Ele[eleInd]-794.092*sqrt(covEtaEtaEle[eleInd])-15.3449*latEle[eleInd]-31.1032*a20Ele[eleInd];
  else
    Fisher=27.2967+2.97453*s9s25Ele[eleInd]-169.219*sqrt(covEtaEtaEle[eleInd])-17.0445*latEle[eleInd]-24.8542*a20Ele[eleInd];
  return Fisher;
}

void ClassEfficiencyStudy::displayEfficiencies() {

  std::cout << "ELECTRON-ID EFFICIENCIES FOR ECAL " << _ECALSection << std::endl;

  _goldenCounter->Draw();
  _goldenCounter->Draw("hOverE","nRecoEle");
  _goldenCounter->Draw("s9s25","hOverE");
  _goldenCounter->Draw("deta","s9s25");
  _goldenCounter->Draw("dphiIn","deta");
  _goldenCounter->Draw("dphiOut","dphiIn");
  _goldenCounter->Draw("covEtaEta","dphiOut");
  _goldenCounter->Draw("eOverPout","covEtaEta");
  _goldenCounter->Draw("Fisher","eOverPout");
  _goldenCounter->Draw("Likelihood","Fisher");
  _goldenCounter->Draw("final","nRecoEle");

  _bigbremCounter->Draw();
  _bigbremCounter->Draw("hOverE","nRecoEle");
  _bigbremCounter->Draw("s9s25","hOverE");
  _bigbremCounter->Draw("deta","s9s25");
  _bigbremCounter->Draw("dphiIn","deta");
  _bigbremCounter->Draw("dphiOut","dphiIn");
  _bigbremCounter->Draw("covEtaEta","dphiOut");
  _bigbremCounter->Draw("eOverPout","covEtaEta");
  _bigbremCounter->Draw("Fisher","eOverPout");
  _bigbremCounter->Draw("Likelihood","Fisher");
  _bigbremCounter->Draw("final","nRecoEle");

  _narrowCounter->Draw();
  _narrowCounter->Draw("hOverE","nRecoEle");
  _narrowCounter->Draw("s9s25","hOverE");
  _narrowCounter->Draw("deta","s9s25");
  _narrowCounter->Draw("dphiIn","deta");
  _narrowCounter->Draw("dphiOut","dphiIn");
  _narrowCounter->Draw("covEtaEta","dphiOut");
  _narrowCounter->Draw("eOverPout","covEtaEta");
  _narrowCounter->Draw("Fisher","eOverPout");
  _narrowCounter->Draw("Likelihood","Fisher");
  _narrowCounter->Draw("final","nRecoEle");

  _showeringCounter->Draw();
  _showeringCounter->Draw("hOverE","nRecoEle");
  _showeringCounter->Draw("s9s25","hOverE");
  _showeringCounter->Draw("deta","s9s25");
  _showeringCounter->Draw("dphiIn","deta");
  _showeringCounter->Draw("dphiOut","dphiIn");
  _showeringCounter->Draw("covEtaEta","dphiOut");
  _showeringCounter->Draw("eOverPout","covEtaEta");
  _showeringCounter->Draw("Fisher","eOverPout");
  _showeringCounter->Draw("Likelihood","Fisher");
  _showeringCounter->Draw("final","nRecoEle");

  _globalCounter->Draw();
  _globalCounter->Draw("final","nRecoEle");
}


