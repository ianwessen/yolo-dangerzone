#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#define NSAMPLES 16

using namespace std;

double weight(double ngen, double xsec, double filtereff, double lumi = 1);

void countEvents() {

  char nametree[200];
  sprintf(nametree,"FULL_SELECTION_EVENT_COUNTER_EE");

  cout << "nametree = " << nametree << endl;

  // signals
  std::vector<std::vector<TChain*> > signalChains;
  int mH[17] = {120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600};
  for(int imass=0; imass<17;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH[imass]);

    std::vector<TChain*> massChain;

    TChain *sigChains[4];
    for(int i=0; i<4; i++) sigChains[i] = new TChain(nametree);

    TString hSample = TString("results/Spring11_V5HWW/GluGluToHToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[0]->Add(hSample.Data());
    
    hSample = TString("results/Spring11_V5HWW/GluGluToHToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[1]->Add(hSample.Data());
    
    hSample = TString("results/Spring11_V5HWW/GluGluToHToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[2]->Add(hSample.Data());
    
    hSample = TString("results/Spring11_V5HWW/GluGluToHToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[3]->Add(hSample.Data());

    for(int i=0; i<4; i++) massChain.push_back(sigChains[i]);
    signalChains.push_back(massChain);
  }

  // backgrounds
  TChain *chains[NSAMPLES];
  for(int isample=0; isample<NSAMPLES; isample++) {
    chains[isample] = new TChain(nametree);
  }

  chains[0]->Add("results/Spring11_V5HWW/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*Counters.root");
  
  chains[1]->Add("results/Spring11_V5HWW/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root");
  chains[2]->Add("results/Spring11_V5HWW/DYToMuMu_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root");
  chains[3]->Add("results/Spring11_V5HWW/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*Counters.root");

  chains[4]->Add("results/Spring11_V5HWW/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[5]->Add("results/Spring11_V5HWW/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[6]->Add("results/Spring11_V5HWW/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*Counters.root");

  chains[7]->Add("results/Spring11_V5HWW/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*Counters.root");
  chains[8]->Add("results/Spring11_V5HWW/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*Counters.root");
  chains[9]->Add("results/Spring11_V5HWW/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*Counters.root");

  chains[10]->Add("results/Spring11_V5HWW/TTJets_TuneZ2_7TeV-madgraph-tauola/*Counters.root");

  chains[11]->Add("results/Spring11_V5HWW/PhotonVJets_7TeV-madgraph/*Counters.root");
  // chains[12]->Add("results/Spring11_V5HWW/WWTo2L2Nu_TuneZ2_7TeV-pythia6/*Counters.root"); 
  chains[12]->Add("results/Spring11_V5HWW/VVJetsTo4L_TuneD6T_7TeV-madgraph-tauola-WWFilter/*Counters.root"); 

  chains[13]->Add("results/Spring11_V5HWW/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*Counters.root");
  chains[14]->Add("results/Spring11_V5HWW/WZTo3LNu_TuneZ2_7TeV-pythia6/*Counters.root");
  chains[15]->Add("results/Spring11_V5HWW/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/*Counters.root");


  cout << "chains added. " << endl;

  std::vector<std::vector<TString> > signalSampleName;
  for(int imass=0; imass<17;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH[imass]);
    
    std::vector<TString> massSampleName;
    massSampleName.push_back(TString("results/merged/ggH")+TString(mass)+TString("2L2Nu_ee.root"));
    massSampleName.push_back(TString("results/merged/ggH")+TString(mass)+TString("LNuTauNu_ee.root"));
    massSampleName.push_back(TString("results/merged/qqH")+TString(mass)+TString("2L2Nu_ee.root"));
    massSampleName.push_back(TString("results/merged/qqH")+TString(mass)+TString("LNuTauNu_ee.root"));

    signalSampleName.push_back(massSampleName);
  }

  std::vector<TString> sampleName;
  sampleName.push_back("results/merged/Wjets_ee.root");

  sampleName.push_back("results/merged/Zee_Lo_ee.root");
  sampleName.push_back("results/merged/Zmm_Lo_ee.root");
  sampleName.push_back("results/merged/Ztautau_Lo_ee.root");

  sampleName.push_back("results/merged/Zee_Hi_ee.root");
  sampleName.push_back("results/merged/Zmm_Hi_ee.root");
  sampleName.push_back("results/merged/Ztautau_Hi_ee.root");

  sampleName.push_back("results/merged/SingleTop_sChannel_ee.root");
  sampleName.push_back("results/merged/SingleTop_tChannel_ee.root");
  sampleName.push_back("results/merged/SingleTop_tWChannel_ee.root");

  sampleName.push_back("results/merged/TTbar_ee.root");


  sampleName.push_back("results/merged/Wgamma_ee.root");
  sampleName.push_back("results/merged/WW_ee.root");
  sampleName.push_back("results/merged/ggWW_ee.root");
  sampleName.push_back("results/merged/WZ_ee.root");
  sampleName.push_back("results/merged/ZZ_ee.root");

  std::map<int,float> ggHiggs_xsec;
  // samples are emu only
  ggHiggs_xsec.insert(std::make_pair(120,0.249642));
  ggHiggs_xsec.insert(std::make_pair(130,0.452090));
  ggHiggs_xsec.insert(std::make_pair(140,0.641773));
  ggHiggs_xsec.insert(std::make_pair(150,0.770471));
  ggHiggs_xsec.insert(std::make_pair(160,0.866443));
  ggHiggs_xsec.insert(std::make_pair(170,0.782962));
  ggHiggs_xsec.insert(std::make_pair(180,0.659328));
  ggHiggs_xsec.insert(std::make_pair(190,0.486486));
  ggHiggs_xsec.insert(std::make_pair(200,0.408305));
  ggHiggs_xsec.insert(std::make_pair(210,0.358465));
  ggHiggs_xsec.insert(std::make_pair(220,0.321398));
  ggHiggs_xsec.insert(std::make_pair(230,0.290454));
  ggHiggs_xsec.insert(std::make_pair(250,0.243724));
  ggHiggs_xsec.insert(std::make_pair(300,0.175652));
  ggHiggs_xsec.insert(std::make_pair(350,0.160052));
  ggHiggs_xsec.insert(std::make_pair(400,0.124330));
  ggHiggs_xsec.insert(std::make_pair(450,0.078433));
  ggHiggs_xsec.insert(std::make_pair(500,0.048702));
  ggHiggs_xsec.insert(std::make_pair(550,0.030364));
  ggHiggs_xsec.insert(std::make_pair(600,0.019184));

  std::map<int,float> qqHiggs_xsec;
  // samples are emu only
  qqHiggs_xsec.insert(std::make_pair(120,0.019057));
  qqHiggs_xsec.insert(std::make_pair(130,0.036942));
  qqHiggs_xsec.insert(std::make_pair(140,0.055686));
  qqHiggs_xsec.insert(std::make_pair(150,0.070561));
  qqHiggs_xsec.insert(std::make_pair(160,0.083858));
  qqHiggs_xsec.insert(std::make_pair(170,0.082794));
  qqHiggs_xsec.insert(std::make_pair(180,0.073183));
  qqHiggs_xsec.insert(std::make_pair(190,0.057139));
  qqHiggs_xsec.insert(std::make_pair(200,0.049566));
  qqHiggs_xsec.insert(std::make_pair(210,0.044544));
  qqHiggs_xsec.insert(std::make_pair(220,0.040624));
  qqHiggs_xsec.insert(std::make_pair(230,0.037243));
  qqHiggs_xsec.insert(std::make_pair(250,0.031672));
  qqHiggs_xsec.insert(std::make_pair(300,0.021873));
  qqHiggs_xsec.insert(std::make_pair(350,0.015113));
  qqHiggs_xsec.insert(std::make_pair(400,0.009885));
  qqHiggs_xsec.insert(std::make_pair(450,0.007143));
  qqHiggs_xsec.insert(std::make_pair(500,0.005439));
  qqHiggs_xsec.insert(std::make_pair(550,0.004249));
  qqHiggs_xsec.insert(std::make_pair(600,0.003374));

  std::vector<std::vector<double> > signalXSec;
  for(int imass=0; imass<17;imass++) {
    std::vector<double> massXsec;
    double ggHiggs_xsec_2l2nu = ggHiggs_xsec[mH[imass]] * 4./9.; 
    double ggHiggs_xsec_taunulnu = ggHiggs_xsec[mH[imass]] * 4./9.; 
    double qqHiggs_xsec_2l2nu = qqHiggs_xsec[mH[imass]] * 4./9.; 
    double qqHiggs_xsec_taunulnu = qqHiggs_xsec[mH[imass]] * 4./9.; 

    massXsec.push_back(ggHiggs_xsec_2l2nu);
    massXsec.push_back(ggHiggs_xsec_taunulnu);
    massXsec.push_back(qqHiggs_xsec_2l2nu);
    massXsec.push_back(qqHiggs_xsec_taunulnu);

    signalXSec.push_back(massXsec);
  }


  std::vector<float> sampleXsec;
  sampleXsec.push_back(31314.); // madgraph
  sampleXsec.push_back(3457.);
  sampleXsec.push_back(3457.);
  sampleXsec.push_back(3457.);
  sampleXsec.push_back(4998./3.);
  sampleXsec.push_back(4998./3.);
  sampleXsec.push_back(4998./3.);
  sampleXsec.push_back(4.21 * (0.1080*3));
  sampleXsec.push_back(64.6 * (0.1080*3));
  sampleXsec.push_back(10.6);
  sampleXsec.push_back(157.5);
  sampleXsec.push_back(165.);
  // s(NLO qqWW+ggWW) = 47 pb, [K. Ellis]
  // gg/Tot = 0.0305
  // sigma(qqWW) = 47*(1-0.0305) * 0.108*0.108*9 = 4.7833889
  // sigma(ggWW) = 47*0.0305*0.108*0.108*9 = 0.150483
  sampleXsec.push_back(4.7833889);
  sampleXsec.push_back(0.150483);
  sampleXsec.push_back(0.599442);
  sampleXsec.push_back(7.41); // sigma_ZZ = 6.77 (1+0.12/1.277)=7.41pb, where 1.277 is the LO->NLO k-factor.

  std::vector<std::vector<double> > signalProcId;
  for(int imass=0; imass<17;imass++) {
    std::vector<double> massId;
    massId.push_back(1001); // ggH->2L2Nu
    massId.push_back(1002); // ggH->LNuTauNu
    massId.push_back(1003); // qqH->2L2Nu
    massId.push_back(1004); // qqH->LNuTauNu
    signalProcId.push_back(massId);
  }

  std::vector<int> sampleProcessId;
  sampleProcessId.push_back(1); // Wjets
  sampleProcessId.push_back(2); // Z->ee [10-20]
  sampleProcessId.push_back(3); // Z->mumu [10-20]
  sampleProcessId.push_back(4); // Z->tautau [10-20]
  sampleProcessId.push_back(5); // Z->ee [20-inf]
  sampleProcessId.push_back(6); // Z->mumu [20-inf]
  sampleProcessId.push_back(7); // Z->tautau [20-inf]
  sampleProcessId.push_back(8); // t(s-cha) 
  sampleProcessId.push_back(9); // t(t-cha)
  sampleProcessId.push_back(10); // tW
  sampleProcessId.push_back(11); // ttbar
  sampleProcessId.push_back(12); // Vgamma
  sampleProcessId.push_back(13); // qqWW
  sampleProcessId.push_back(14); // ggWW
  sampleProcessId.push_back(15); // WZ->3LNu
  sampleProcessId.push_back(16); // ZZ->anything

  float nEvH[17][4];
  for(int imass=0; imass<17; imass++) {
    for(int i=0; i<4; i++) {
      nEvH[imass][i] = 0.0;
    }
  }

  for(int imass=0; imass<17; imass++) {
    std::vector<TChain*> massChain = signalChains[imass];
    for(int i=0; i<4; i++) {

      cout << "\tProcessing signal sample mass # " << mH[imass] << "..." << endl;
      
      Int_t           nCuts;
      Float_t         nSel[25];   //[nCuts]
      
      // List of branches
      TBranch        *b_nCuts;   //!
      TBranch        *b_nSel;   //!
      
      massChain[i]->SetBranchAddress("nCuts", &nCuts, &b_nCuts);
      massChain[i]->SetBranchAddress("nSel", nSel, &b_nSel);
    
      Long64_t nentries = massChain[i]->GetEntries();

      Long64_t nbytes = 0, nb = 0;
      // loop over files (>1 if VecBos in batch, splitted in many jobs)
      for (Long64_t jentry=0; jentry<nentries;jentry++) {
        
        nb = massChain[i]->GetEntry(jentry);   nbytes += nb;

        nEvH[imass][i] += nSel[0];
      }
    }
  }

  // backgrounds
  float nEv[NSAMPLES];
  for(int isample=0; isample<NSAMPLES; isample++) {
    nEv[isample] = 0.0;
  }

  for(int isample=0; isample<NSAMPLES; isample++) {

    Int_t           nCuts;
    Float_t         nSel[25];   //[nCuts]
    
    // List of branches
    TBranch        *b_nCuts;   //!
    TBranch        *b_nSel;   //!
    
    chains[isample]->SetBranchAddress("nCuts", &nCuts, &b_nCuts);
    chains[isample]->SetBranchAddress("nSel", nSel, &b_nSel);
    
    Long64_t nentries = chains[isample]->GetEntries();
    
    Long64_t nbytes = 0, nb = 0;
    // loop over files (>1 if VecBos in batch, splitted in many jobs)
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      
      nb = chains[isample]->GetEntry(jentry);   nbytes += nb;

      nEv[isample] += nSel[0];
    }
  }

  if (sampleXsec.size() != sampleName.size() ) cout << "nasty error! check sizes..." << endl;

  std::ofstream weightsFile;
  weightsFile.open("weightTrees.sh");
  weightsFile << "#! /bin/sh\n\n" << std::endl;
  weightsFile << "mkdir -p results/merged_skim" << std::endl;
  weightsFile << "lumiEE=$1" << std::endl;
  weightsFile << "lumiMM=$2" << std::endl;
  weightsFile << "lumiEM=$3" << std::endl;
  
  // now write ee
  weightsFile << "echo \"Adding weights for ee datasets for \" $lumiEE \" pb-1...\"" << std::endl;
  weightsFile << "root -l -b <<EOF" << std::endl;
  weightsFile << ".L addWeightsToTree.cc+" << std::endl;
  for(int imass=0; imass<17; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<4; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      weightsFile << "addWeights(\"" << massSampleName[i].Data() << "\", " << w << "*$lumiEE, " << massId[i] << " ,1, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    int release = 0;
    if (isample>0 && isample<7) release = 1;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    weightsFile << "addWeights(\"" << sampleName[isample].Data() << "\", " << w << "*$lumiEE, " << sampleProcessId[isample] << " ,1, " << release << ");" << std::endl;
  }
  weightsFile << ".q\n\nEOF\n" << std::endl;
  
  // now write mm
  weightsFile << "echo \"Adding weights for mm datasets for \" $lumiMM \" pb-1...\"" << std::endl;
  weightsFile << "root -l -b <<EOF" << std::endl;
  weightsFile << ".L addWeightsToTree.cc+" << std::endl;
  for(int imass=0; imass<17; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<4; i++) {
      cout << "Events processed for sample: " << massSampleName[i] << " = " << nEvH[imass][i] << endl;
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameMM = massSampleName[i].ReplaceAll("_ee","_mm");
      weightsFile << "addWeights(\"" << massSampleNameMM.Data() << "\", " << w << "*$lumiMM, " << massId[i] << " ,0, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    if (isample>0 && isample<7) release = 1;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameMM = sampleName[isample].ReplaceAll("_ee","_mm");
    weightsFile << "addWeights(\"" << sampleNameMM.Data() << "\", " << w << "*$lumiMM, " << sampleProcessId[isample] << " ,0, " << release << ");" << std::endl;
  }
  weightsFile << ".q\n\nEOF\n" << std::endl;


  // now write em
  weightsFile << "echo \"Adding weights for em datasets for \" $lumiEM \" pb-1...\"" << std::endl;
  weightsFile << "root -l -b <<EOF" << std::endl;
  weightsFile << ".L addWeightsToTree.cc+" << std::endl;
  for(int imass=0; imass<17; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<4; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameEM = massSampleName[i].ReplaceAll("_ee","_em");
      weightsFile << "addWeights(\"" << massSampleNameEM.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,2, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    if (isample>0 && isample<7) release = 1;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameEM = sampleName[isample].ReplaceAll("_mm","_em");
    weightsFile << "addWeights(\"" << sampleNameEM.Data() << "\", " << w << "*$lumiEM, " << sampleProcessId[isample] << " ,2, " << release << ");" << std::endl;
  }
  weightsFile << ".q\n\nEOF\n" << std::endl;


  // now write me
  weightsFile << "echo \"Adding weights for me datasets for \" $lumiEM \" pb-1...\"" << std::endl;
  weightsFile << "root -l -b <<EOF" << std::endl;
  weightsFile << ".L addWeightsToTree.cc+" << std::endl;
  for(int imass=0; imass<17; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<4; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameME = massSampleName[i].ReplaceAll("_ee","_me");
      weightsFile << "addWeights(\"" << massSampleNameME.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,3, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    if (isample>0 && isample<7) release = 1;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameEM = sampleName[isample].ReplaceAll("_em","_me");
    weightsFile << "addWeights(\"" << sampleNameEM.Data() << "\", " << w << "*$lumiEM, " << sampleProcessId[isample] << " ,3, " << release << ");" << std::endl;
  }
  weightsFile << ".q\n\nEOF\n" << std::endl;

  weightsFile << "echo \"done weighting.\"" << std::endl;

}

double weight(double ngen, double xsec, double filtereff, double lumi) {

  if(ngen==0) return 0;
  return xsec * filtereff * lumi / ngen;

}

