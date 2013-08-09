//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed H->WW->2e2nu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include "HiggsAnalysisTools/include/Application.hh"
#include "CommonTools/include/TriggerMask.hh"
#if Application == 1
#include "HiggsAnalysisTools/src/HiggsSelection.cc"
#endif
#if Application == 2
#include "HiggsAnalysisTools/src/ElectronID.cc"
#endif
#if Application == 3
#include "HiggsAnalysisTools/src/plotsEleID.cc"
#endif
#if Application == 4
#include "HiggsAnalysisTools/src/ZSelection.cc"
#endif
#if Application == 5
#include "HiggsAnalysisTools/src/WSelection.cc"
#endif
#if Application == 6
#include "HiggsAnalysisTools/src/ClassEfficiencyStudy.cc"
#endif
#if Application == 7
#include "HiggsAnalysisTools/src/WplusJets.cc"
#endif
#if Application == 8
#include "HiggsAnalysisTools/src/ZTandProbe.cc"
#endif
#if Application == 9
#include "HiggsAnalysisTools/src/WewkSelection.cc"
#endif
#if Application == 10
#include "HiggsAnalysisTools/src/ZewkSelection.cc"
#endif
#if Application == 11
#include "HiggsAnalysisTools/src/HiggsEleIdOptimToyMC.cc"
#endif
#if Application == 12
#include "HiggsAnalysisTools/src/ZplusJetsSelection.cc"
#endif
#if Application == 13
#include "HiggsAnalysisTools/src/HiggsIsolationOptimToyMC.cc"
#endif
#if Application == 14
#include "HiggsAnalysisTools/src/HiggsKinematicsOptimToyMC.cc"
#endif
#if Application == 15
#include "HiggsAnalysisTools/include/LeptonPlusFakeSelection.hh"
#endif
#if Application == 16
#include "HiggsAnalysisTools/src/HiggsVertexing.cpp"
#endif
#if Application == 17
#include "HiggsAnalysisTools/src/HiggsIsolationStudiesInput.cc"
#endif
#if Application == 18
#include "HiggsAnalysisTools/src/HiggsMLSelection.cc"
#endif
#if Application == 19
#include "HiggsAnalysisTools/src/LeptonPlusFakeMLSelection.cc"
#endif
#if Application == 20
#include "HiggsAnalysisTools/src/LeptonPlusFakeMLSelection_ME.cc"
#endif
#if Application == 21
#include "HiggsAnalysisTools/src/LeptonPlusFakeMLSelection_fullEE.cc"
#endif
#if Application == 22
#include "HiggsAnalysisTools/src/LeptonPlusFakeMLSelection_fullME.cc"
#endif

int main(int argc, char* argv[]) {

  char inputFileName[500];
  char outputFileName[500];
  char dataset[150];
  if ( argc < 2 ){
    std::cout << "missing argument: insert at least inputFile with list of root files" << std::endl; 
    std::cout << "HiggsApp inputFile [outputFile] [1=MC,0=data] [dataset]" << std::endl;
    return 1;
  }
  strcpy(inputFileName,argv[1]);
  if (argc < 3 ) strcpy(outputFileName,argv[1]);
  else strcpy(outputFileName,argv[2]);
  int isMC=1;
  if(argc==5) {
    isMC=atoi(argv[3]);
    strcpy(dataset,argv[4]);
  }
  
  // -------------------------
  // Loading the file
  TChain *theChain = new TChain("ntp1");
  char Buffer[500];
  char MyRootFile[2000];
  std::cout << "input: " << inputFileName << std::endl;
  ifstream *inputFile = new ifstream(inputFileName);
  char tmpFileName[256];
  vector<string> filesToRemove;
  while( !(inputFile->eof()) ){
    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
      {
        sscanf(Buffer,"%s",MyRootFile);
        // theChain->Add("root://castorcms/"+TString(MyRootFile)); 
        // theChain->Add("rfio:"+TString(MyRootFile));
        theChain->Add(TString(MyRootFile));
        std::cout << "chaining " << MyRootFile << std::endl;
      }
  }
  inputFile->close();
  delete inputFile;


#if Application == 1

  HiggsSelection htoww(theChain);
  htoww.SetDatasetName(outputFileName);

  TriggerMask mask(treeCond);

  // require triggers
  mask.requireTrigger("HLT_Ele15_LW_L1R");
  mask.requireTrigger("HLT_Mu9");

  std::vector<int> requiredTriggers = mask.getBits();
  htoww.requireTrigger(requiredTriggers);

  htoww.Loop();
  htoww.displayEfficiencies(outputFileName);

#endif

#if Application == 2
  ElectronID eidanalyzer(theChain);
  eidanalyzer.Loop();
  eidanalyzer.displayEfficiencies();
#endif

#if Application == 3
  plotsEleID eidplots(theChain);
  eidplots.Loop();
#endif

#if Application == 4  
  ZSelection ztoee(theChain);
  ztoee.Loop();
  ztoee.displayEfficiencies();
#endif

#if Application == 5  
  WSelection wtoenu(theChain);
  wtoenu.Loop();
  wtoenu.displayEfficiencies();
#endif

#if Application == 6  
  ClassEfficiencyStudy classEff(theChain);
  classEff.Loop();
  classEff.displayEfficiencies();
#endif

#if Application == 7  
  WplusJets wjets(theChain);
  wjets.Loop();
  wjets.displayEfficiencies();
#endif

#if Application == 8
  ZTandProbe zTaP(theChain);
  TriggerMask mask(treeCond); // require triggers
  mask.requireTrigger("HLT1Electron");
  mask.requireTrigger("HLT1ElectronRelaxed");
  mask.requireTrigger("HLT2Electron");
  mask.requireTrigger("HLT2ElectronRelaxed");
  mask.requireTrigger("HLTXElectronMuon");
  mask.requireTrigger("HLTXElectronMuonRelaxed");
  std::vector<int> requiredTriggers = mask.getBits();
  zTaP.requireTrigger(requiredTriggers);
  zTaP.Loop();
  zTaP.displayEfficiencies();
  zTaP.writeHistos();
#endif

#if Application == 9  
  WewkSelection wtoenu(theChain);
  wtoenu.Loop();
  wtoenu.displayEfficiencies();
#endif

#if Application == 10  
  ZewkSelection ztoee(theChain);

  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT1Electron");
  mask.requireTrigger("HLT1ElectronRelaxed");
  mask.requireTrigger("HLT2Electron");
  mask.requireTrigger("HLT2ElectronRelaxed");

  std::vector<int> requiredTriggers = mask.getBits();

  ztoee.requireTrigger(requiredTriggers);

  ztoee.Loop();
  ztoee.displayEfficiencies();
#endif

#if Application == 11

  HiggsEleIdOptimToyMC heleIdtoy(theChain);
  std::string outFileName(inputFileName);
  outFileName+=".root";

  TriggerMask mask(treeCond);

  // require triggers for ee channel
  mask.requireTrigger("HLT_Ele15_LW_L1R");
  // mask.requireTrigger("HLT1Electron");
  // mask.requireTrigger("HLT1ElectronRelaxed");
  // mask.requireTrigger("HLT2Electron");
  // mask.requireTrigger("HLT2ElectronRelaxed");

  std::vector<int> requiredTriggers = mask.getBits();
  heleIdtoy.requireTrigger(requiredTriggers);

  heleIdtoy.Loop(outputFileName);

#endif

#if Application == 12  
  ZplusJetsSelection zjetstoee(theChain);

  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT1Electron");
  mask.requireTrigger("HLT1ElectronRelaxed");
  mask.requireTrigger("HLT2Electron");
  mask.requireTrigger("HLT2ElectronRelaxed");

  std::vector<int> requiredTriggers = mask.getBits();

  zjetstoee.requireTrigger(requiredTriggers);

  zjetstoee.Loop();
  zjetstoee.writeHistos();
  zjetstoee.displayEfficiencies();
#endif

#if Application == 13

  HiggsIsolationOptimToyMC hisoltoy(theChain);
  std::string outFileName(inputFileName);
  outFileName+=".root";
  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT_Ele15_LW_L1R");
  std::vector<int> requiredTriggers = mask.getBits();
  hisoltoy.requireTrigger(requiredTriggers);
  hisoltoy.Loop(outputFileName);

#endif

#if Application == 14

  HiggsKinematicsOptimToyMC hkinemtoy(theChain);
  std::string outFileName(inputFileName);
  outFileName+=".root";
  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT1Electron");
  mask.requireTrigger("HLT1ElectronRelaxed");
  mask.requireTrigger("HLT2Electron");
  mask.requireTrigger("HLT2ElectronRelaxed");
  std::vector<int> requiredTriggers = mask.getBits();
  hkinemtoy.requireTrigger(requiredTriggers);
  hkinemtoy.Loop();

#endif

#if Application == 15

  LeptonPlusFakeSelection lplusfake(theChain);

  TriggerMask mask(treeCond);

  // require triggers for ee channel
  mask.requireTrigger("HLT1Electron");
  mask.requireTrigger("HLT1ElectronRelaxed");
//   mask.requireTrigger("HLT2Electron");
//   mask.requireTrigger("HLT2ElectronRelaxed");
  mask.requireTrigger("HLT1MuonIso");
  mask.requireTrigger("HLT1MuonNonIso");
//   mask.requireTrigger("HLT2MuonNonIso");
//   mask.requireTrigger("HLTXElectronMuon");
//   mask.requireTrigger("HLTXElectronMuonRelaxed");

  std::vector<int> requiredTriggers = mask.getBits();
  lplusfake.requireTrigger(requiredTriggers);

  lplusfake.Loop();

#endif

#if Application == 16

  HiggsVertexing vertex(theChain);
  std::string outFileName(inputFileName);
  outFileName+=".root";
  
  // require triggers for ee channel
  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT_Ele15_LW_L1R");
  
  std::vector<int> requiredTriggers = mask.getBits();
  vertex.requireTrigger(requiredTriggers);

  vertex.Loop(outputFileName);

#endif  

#if Application == 17

  HiggsIsolationStudiesInput isolInput(theChain);
  std::string outFileName(inputFileName);
  outFileName+=".root";

  TriggerMask mask(treeCond);

  // require triggers for ee channel
  mask.requireTrigger("HLT_Ele15_LW_L1R");

  std::vector<int> requiredTriggers = mask.getBits();
  isolInput.requireTrigger(requiredTriggers);
  
  isolInput.Loop(outputFileName);

#endif

#if Application == 18

  HiggsMLSelection htoww(theChain);
  htoww.SetDatasetName(outputFileName);

  std::vector<std::string> maskEE, maskMM, maskEM;
  std::vector<std::string> maskNotEE, maskNotMM, maskNotEM;
  
  if(isMC) {
    maskEE.push_back("1-1:HLT_Ele17_SW_TighterEleIdIsol_L1R_v3");
    maskMM.push_back("1-1:HLT_DoubleMu5_v1");
    maskMM.push_back("1-1:HLT_Mu25_v1");
    maskEM.push_back("1-1:HLT_Mu5_Ele17_v2");
    maskEM.push_back("1-1:HLT_Mu25_v1");
  } else {
    TString DatasetName(dataset);
    if(DatasetName.Contains("DoubleElectron")) {
      maskEE.push_back("1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v");
      maskEE.push_back("171050-180252:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"); // end 2011
      maskEE.push_back("190456-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    } else if(DatasetName.Contains("DoubleMu")) {
      maskMM.push_back("1-165208:HLT_DoubleMu7_v");
      maskMM.push_back("165364-178419:HLT_Mu13_Mu8_v");
      maskMM.push_back("178420-180252:HLT_Mu17_Mu8_v");
      maskMM.push_back("175832-180252:HLT_Mu17_TkMu8_v"); // end 2011
      maskMM.push_back("190456-999999:HLT_Mu17_Mu8_v");
      maskMM.push_back("190456-999999:HLT_Mu17_TkMu8_v");
    } else if(DatasetName.Contains("MuEG")) {
      maskEM.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskEM.push_back("175973-180252:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskEM.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskEM.push_back("167914-180252:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"); // end 2011
      maskEM.push_back("190456-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
      maskEM.push_back("190456-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    } else if(DatasetName.Contains("SingleMu")) {
      maskMM.push_back("1-163261:HLT_Mu15_v2");
      maskMM.push_back("163262-165099:HLT_Mu24_v");
      maskMM.push_back("165102-173235:HLT_Mu30_v");
      maskMM.push_back("173236-175972:HLT_Mu40_v");
      maskMM.push_back("175973-999999:HLT_Mu40_eta2p1_v");
      maskMM.push_back("163262-170901:HLT_IsoMu17_v");
      maskMM.push_back("171050-175910:HLT_IsoMu20_v");
      maskMM.push_back("175911-175921:HLT_IsoMu24_v");
      maskMM.push_back("175922-180252:HLT_IsoMu24_eta2p1_v"); // end 2011
      maskMM.push_back("190456-999999:HLT_IsoMu24_eta2p1_v"); 
      maskNotMM.push_back("1-165208:HLT_DoubleMu7_v");
      maskNotMM.push_back("165364-178419:HLT_Mu13_Mu8_v");
      maskNotMM.push_back("178420-180252:HLT_Mu17_Mu8_v");
      maskNotMM.push_back("175832-180252:HLT_Mu17_TkMu8_v"); // end 2011
      maskNotMM.push_back("190456-999999:HLT_Mu17_Mu8_v");
      maskNotMM.push_back("190456-999999:HLT_Mu17_TkMu8_v");

      maskEM.push_back("1-163261:HLT_Mu15_v2");
      maskEM.push_back("163262-165099:HLT_Mu24_v");
      maskEM.push_back("165102-173235:HLT_Mu30_v");
      maskEM.push_back("173236-175972:HLT_Mu40_v");
      maskEM.push_back("175973-999999:HLT_Mu40_eta2p1_v");
      maskEM.push_back("163262-170901:HLT_IsoMu17_v");
      maskEM.push_back("171050-175910:HLT_IsoMu20_v");
      maskEM.push_back("175911-175921:HLT_IsoMu24_v");
      maskEM.push_back("175922-999999:HLT_IsoMu24_eta2p1_v");
      maskNotEM.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskNotEM.push_back("175973-180252:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskNotEM.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskNotEM.push_back("167914-180252:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"); // end 2011
      maskNotEM.push_back("190456-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
      maskNotEM.push_back("190456-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    } else if(DatasetName.Contains("SingleElectron")) {
      maskEE.push_back("1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEE.push_back("165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEE.push_back("166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v");
      maskEE.push_back("170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v");
      maskEE.push_back("178420-180252:HLT_Ele80_CaloIdVT_TrkIdT_v"); // end of 2011
      maskEE.push_back("190456-999999:HLT_Ele27_WP80_v");
      maskNotEE.push_back("1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v");
      maskNotEE.push_back("171050-180252:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v"); // end 2011
      maskNotEE.push_back("190456-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");

      maskEM.push_back("1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEM.push_back("165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEM.push_back("166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v");
      maskEM.push_back("170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v");
      maskEM.push_back("178420-180252:HLT_Ele80_CaloIdVT_TrkIdT_v"); // end of 2011 
      maskEM.push_back("190456-999999:HLT_Ele27_WP80_v");
      maskNotEM.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskNotEM.push_back("175973-180252:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskNotEM.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskNotEM.push_back("167914-180252:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v"); // end 2011
      maskNotEM.push_back("190456-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
      maskNotEM.push_back("190456-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    }
  }

  htoww.setRequiredTriggers(maskEE,0);
  htoww.setRequiredTriggers(maskMM,1);
  htoww.setRequiredTriggers(maskEM,2);

  htoww.setNotRequiredTriggers(maskNotEE,0);
  htoww.setNotRequiredTriggers(maskNotMM,1);
  htoww.setNotRequiredTriggers(maskNotEM,2);

  htoww.Loop();
  htoww.displayEfficiencies(outputFileName);

#endif

#if Application == 19

  LeptonPlusFakeMLSelection lplusfake(theChain);
  lplusfake.SetDatasetName(outputFileName);

  std::vector<std::string> maskEE, maskNotEE;
  
  if(isMC) {
    maskEE.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v3");
  } else {
    TString DatasetName(dataset);
    if(DatasetName.Contains("DoubleElectron")) {
      maskEE.push_back("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL");
    }
  }

  lplusfake.setRequiredTriggers(maskEE);
  lplusfake.Loop();
  lplusfake.displayEfficiencies(outputFileName);
  
#endif

#if Application == 20

  LeptonPlusFakeMLSelection_ME lplusfake(theChain);
  lplusfake.SetDatasetName(outputFileName);

  std::vector<std::string> maskME, maskNotME;
  
  if(isMC) {
    maskME.push_back("HLT_Mu5_Ele17_v2");
    maskME.push_back("HLT_Mu25_v1");
  } else {
    TString DatasetName(dataset);
    if(DatasetName.Contains("MuEG")) {
      maskME.push_back("HLT_Mu8_Ele17_CaloIdL");
      maskME.push_back("HLT_Mu17_Ele8_CaloIdL");
      maskNotME.push_back("HLT_Mu24");
    } else if(DatasetName.Contains("SingleMu")) {
      maskME.push_back("HLT_Mu24");
    }
  }

  lplusfake.setRequiredTriggers(maskME);
  lplusfake.setNotRequiredTriggers(maskNotME);
  lplusfake.Loop();
  lplusfake.displayEfficiencies(outputFileName);

#endif

#if Application == 21

  LeptonPlusFakeMLSelection_fullEE lplusfake(theChain);
  lplusfake.SetDatasetName(outputFileName);
  
  std::vector<std::string> maskEE, maskNotEE;

  if(isMC) {
    maskEE.push_back("1-1:HLT_Ele17_SW_TighterEleIdIsol_L1R_v3");
  } else {
    TString DatasetName(dataset);
    if(DatasetName.Contains("DoubleElectron")) {
      maskEE.push_back("1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v");
      maskEE.push_back("171050-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    } else if(DatasetName.Contains("SingleElectron")) {
      maskEE.push_back("1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEE.push_back("165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskEE.push_back("166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v");
      maskEE.push_back("170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v");
      maskEE.push_back("178420-999999:HLT_Ele80_CaloIdVT_TrkIdT_v");
      maskNotEE.push_back("1-170901:HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v");
      maskNotEE.push_back("171050-999999:HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v");
    }
  }
  lplusfake.setRequiredTriggers(maskEE);
  lplusfake.setNotRequiredTriggers(maskNotEE);
  lplusfake.Loop();
  lplusfake.displayEfficiencies(outputFileName);
  
#endif

#if Application == 22

  LeptonPlusFakeMLSelection_fullME lplusfake(theChain);
  lplusfake.SetDatasetName(outputFileName);

  std::vector<std::string> maskME, maskNotME;
  
  if(isMC) {
    maskME.push_back("1-1:HLT_Mu5_Ele17_v2");
    maskME.push_back("1-1:HLT_Mu25_v1");
  } else {
    TString DatasetName(dataset);
    if(DatasetName.Contains("MuEG")) {
      maskME.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskME.push_back("175973-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskME.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskME.push_back("167914-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v");
    } else if(DatasetName.Contains("SingleMu")) {
      maskME.push_back("1-163261:HLT_Mu15_v2");
      maskME.push_back("163262-165099:HLT_Mu24_v");
      maskME.push_back("165102-173235:HLT_Mu30_v");
      maskME.push_back("173236-175972:HLT_Mu40_v");
      maskME.push_back("175973-999999:HLT_Mu40_eta2p1_v");
      maskME.push_back("163262-170901:HLT_IsoMu17_v");
      maskME.push_back("171050-175910:HLT_IsoMu20_v");
      maskME.push_back("175911-175921:HLT_IsoMu24_v");
      maskME.push_back("175922-999999:HLT_IsoMu24_eta2p1_v");
      maskNotME.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskNotME.push_back("175973-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskNotME.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskNotME.push_back("167914-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v");
    } else if(DatasetName.Contains("SingleElectron")) {
      maskME.push_back("1-164237:HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskME.push_back("165085-166967:HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v");
      maskME.push_back("166968-170901:HLT_Ele52_CaloIdVT_TrkIdT_v");
      maskME.push_back("170902-178419:HLT_Ele65_CaloIdVT_TrkIdT_v");
      maskME.push_back("178420-999999:HLT_Ele80_CaloIdVT_TrkIdT_v");
      maskNotME.push_back("1-175972:HLT_Mu17_Ele8_CaloIdL_v");
      maskNotME.push_back("175973-999999:HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_v");
      maskNotME.push_back("1-167913:HLT_Mu8_Ele17_CaloIdL_v");
      maskNotME.push_back("167914-999999:HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_v");
    }
  }

  lplusfake.setRequiredTriggers(maskME);
  lplusfake.setNotRequiredTriggers(maskNotME);
  lplusfake.Loop();
  lplusfake.displayEfficiencies(outputFileName);

#endif

  return 0;

}
