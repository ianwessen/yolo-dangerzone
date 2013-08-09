#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#define NSAMPLES 15
std::map<int,float> ggHiggs_xsec;
std::map<int,float> qqHiggs_xsec;

using namespace std;

double weight(double ngen, double xsec, double filtereff, double lumi = 1);
void fillGGHXsec();
void fillQQHXsec();

void countEvents() {

  char nametree[200];
  sprintf(nametree,"FULL_SELECTION_EVENT_COUNTER_EE");

  cout << "nametree = " << nametree << endl;

  // signals
  std::vector<std::vector<TChain*> > signalChains;
  int mH[27] = {110,115,120,125,130,135,140,145,150,155,160,170,180,190,200,250,300,350,400,450,500,550,600,700,800,900,1000};
  for(int imass=0; imass<27;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH[imass]);

    std::vector<TChain*> massChain;

    TChain *sigChains[2];
    for(int i=0; i<2; i++) sigChains[i] = new TChain(nametree);

    TString hSample("results/Summer12_V14_52X/GluGluToHToWWTo2LAndTau2Nu_M-");
    hSample += TString(mass)+TString("_8TeV-powheg-pythia6/*Counters.root");
    sigChains[0]->Add(hSample.Data());
    
    hSample = TString("results/Summer12_V14_52X/VBF_HToWWTo2LAndTau2Nu_M-");
    hSample += TString(mass)+TString("_8TeV-powheg-pythia6/*Counters.root");
    sigChains[1]->Add(hSample.Data());
    
    for(int i=0; i<2; i++) massChain.push_back(sigChains[i]);
    signalChains.push_back(massChain);
  }
  
  // fill the xsec maps
  fillGGHXsec();
  fillQQHXsec();

  // backgrounds
  TChain *chains[NSAMPLES];
  for(int isample=0; isample<NSAMPLES; isample++) {
    chains[isample] = new TChain(nametree);
  }

  // nominal sample first, then the systematics ones
  chains[0]->Add("results/Summer12_V14_52X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/*Counters.root");
  chains[1]->Add("results/Summer12_V14_52X/DYJetsToLL_M-10To50filter_8TeV-madgraph/*Counters.root");
  chains[2]->Add("results/Summer12_V14_52X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/*Counters.root");

  chains[3]->Add("results/Summer12_V14_52X/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*Counters.root");
  chains[4]->Add("results/Summer12_V14_52X/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*Counters.root");
  //chains[5]->Add("results/Summer12_V14_52X/TTJets_TuneZ2star_8TeV-madgraph-tauola/*Counters.root"); 
  chains[5]->Add("results/Summer12_V14_52X/TTTo2L2Nu2B_8TeV-powheg-pythia6/*Counters.root");

  chains[6]->Add("results/Summer12_V14_52X/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/*Counters.root"); // nominal WW MADGRAPH sample
  chains[7]->Add("results/Summer12_V14_52X/GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6/*Counters.root");
  chains[8]->Add("results/Summer12_V14_52X/WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola/*Counters.root");
  chains[9]->Add("results/Summer12_V14_52X/ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*Counters.root");

  // WW sample for systematics
  chains[10]->Add("results/Summer12_V14_52X/WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*Counters.root"); // PYTHIA sample 
  chains[11]->Add("results/Summer12_V14_52X/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/*Counters.root");
  chains[12]->Add("results/Summer12_V14_52X/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/*Counters.root");
  chains[13]->Add("results/Summer12_V14_52X/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/*Counters.root");
  chains[14]->Add("results/Summer12_V14_52X/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/*Counters.root");

  cout << "chains added. " << endl;

  std::vector<std::vector<TString> > signalSampleName;
  for(int imass=0; imass<27;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH[imass]);
    
    std::vector<TString> massSampleName;
    massSampleName.push_back(TString("results/merged/ggH")+TString(mass)+TString("2LAndTau2Nu_ee.root"));
    massSampleName.push_back(TString("results/merged/qqH")+TString(mass)+TString("2LAndTau2Nu_ee.root"));

    signalSampleName.push_back(massSampleName);
  }

  std::vector<TString> sampleName;
  sampleName.push_back("results/merged/Wjets_ee.root"); // 0
  sampleName.push_back("results/merged/Zll_Lo_ee.root"); // 1
  sampleName.push_back("results/merged/Zll_Hi_ee.root"); // 2
  sampleName.push_back("results/merged/SingleT_tWChannel_ee.root"); // 3
  sampleName.push_back("results/merged/SingleTbar_tWChannel_ee.root"); // 4
  sampleName.push_back("results/merged/TTbar_ee.root"); // 5
  sampleName.push_back("results/merged/WW_ee.root"); // 6
  sampleName.push_back("results/merged/ggWW_ee.root"); // 7
  sampleName.push_back("results/merged/WZ_ee.root"); // 8
  sampleName.push_back("results/merged/ZZ_ee.root"); // 9
  sampleName.push_back("results/merged/WW_pythia_ee.root"); // 10
  sampleName.push_back("results/merged/WGToLNuG_ee.root"); // 11
  sampleName.push_back("results/merged/WGstarToLNu2E_ee.root"); // 12
  sampleName.push_back("results/merged/WGstarToLNu2Mu_ee.root"); // 13
  sampleName.push_back("results/merged/WGstarToLNu2Tau_ee.root"); // 14

  // these for the 2LAndTauNu samples
  std::vector<std::vector<double> > signalXSec;
  for(int imass=0; imass<27;imass++) {
    std::vector<double> massXsec;
    double ggHiggs_xsec_2landtau2nu = ggHiggs_xsec[float(mH[imass])];
    double qqHiggs_xsec_2landtau2nu = qqHiggs_xsec[float(mH[imass])];

    cout << "mH = " << mH[imass] << "\tggH xsec = " << ggHiggs_xsec_2landtau2nu << "\tqqH xsec = " << qqHiggs_xsec_2landtau2nu << endl;

    massXsec.push_back(ggHiggs_xsec_2landtau2nu);
    massXsec.push_back(qqHiggs_xsec_2landtau2nu);

    signalXSec.push_back(massXsec);
  }

  std::vector<float> sampleXsec;
  sampleXsec.push_back(37509.); // 0
  sampleXsec.push_back(860.5013); // 1
  sampleXsec.push_back(3532.8149); // 2
  sampleXsec.push_back(11.1773); // 3
  sampleXsec.push_back(11.1773); // 4
  //  sampleXsec.push_back(225.1967); // 5 = TTJets inclusive
  sampleXsec.push_back(23.6402); // 5 = TTbar->2l2nu
  sampleXsec.push_back(5.8123); // 6
  sampleXsec.push_back(0.182852); // 7
  sampleXsec.push_back(0.7346); // 8
  sampleXsec.push_back(0.3649); // 9
  sampleXsec.push_back(5.8123); // 10
  sampleXsec.push_back(553.92); // 11
  sampleXsec.push_back(5.873); // 12
  sampleXsec.push_back(1.914); // 13
  sampleXsec.push_back(0.336); // 14

  std::vector<std::vector<double> > signalProcId;
  for(int imass=0; imass<27;imass++) {
    std::vector<double> massId;
    massId.push_back(9000+mH[imass]); // ggH->2LAndTau2Nu
    massId.push_back(8000+mH[imass]); // qqH->2LAndTau2Nu
    signalProcId.push_back(massId);
  }

  std::vector<int> sampleProcessId; 
  // ids are taken from: https://docs.google.com/spreadsheet/ccc?key=0Ankm0DuoD0h0dHRhV1VNSlV1NEdhNFdyOXh3eFpSMHc&hl=en_US#gid=31
  sampleProcessId.push_back(80); // Wjets
  sampleProcessId.push_back(36); // Z->ll [10-50]
  sampleProcessId.push_back(37); // Z->ll [50-inf]
  sampleProcessId.push_back(19); // tW
  sampleProcessId.push_back(20); // tbarW
  sampleProcessId.push_back(10); // ttbar
  sampleProcessId.push_back(0); // qqWW Mad
  sampleProcessId.push_back(1); // ggWW
  sampleProcessId.push_back(74); // WZ->3LNu
  sampleProcessId.push_back(71); // ZZ->anything
  sampleProcessId.push_back(6); // qqWW PYTHIA
  sampleProcessId.push_back(85); // Wgamma
  sampleProcessId.push_back(82); // WGstarToElNu
  sampleProcessId.push_back(83); // WGstarToMuNu
  sampleProcessId.push_back(84); // WGstarToTauNu

  // signal samples
  float nEvH[27][2];
  for(int imass=0; imass<27; imass++) {
    for(int i=0; i<2; i++) {
      nEvH[imass][i] = 0.0;
    }
  }

  for(int imass=0; imass<27; imass++) {
    std::vector<TChain*> massChain = signalChains[imass];
    for(int i=0; i<2; i++) {

      cout << "\tProcessing signal sample mass # " << mH[imass] << "..." << endl;
      
      Int_t           nCuts;
      Float_t         nSel[29];   //[nCuts]
      
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

  cout << "debug: start bkg" << endl;

  // backgrounds
  float nEv[NSAMPLES];
  for(int isample=0; isample<NSAMPLES; isample++) {
    nEv[isample] = 0.0;
  }

  for(int isample=0; isample<NSAMPLES; isample++) {

    cout << "processing sample " << isample << endl;
    Int_t           nCuts;
    Float_t         nSel[29];   //[nCuts]
    
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

  cout << "Processed all the chains." << endl; 

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
  weightsFile << "make LatinosAnalyzer" << std::endl;
  for(int imass=0; imass<27; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<2; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      weightsFile << "./LatinosAnalyzer " << massSampleName[i].Data() << " " << w << "*$lumiEE " << massId[i] << " 1 " << release << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    int release = 0;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    weightsFile << "./LatinosAnalyzer " << sampleName[isample].Data() << " " << w << "*$lumiEE " << sampleProcessId[isample] << " 1 " << release << std::endl;
  }
  
  // now write mm
  weightsFile << "echo \"Adding weights for mm datasets for \" $lumiMM \" pb-1...\"" << std::endl;
  for(int imass=0; imass<27; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<2; i++) {
      cout << "Events processed for sample: " << massSampleName[i] << " = " << nEvH[imass][i] << endl;
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameMM = massSampleName[i].ReplaceAll("_ee","_mm");
      weightsFile << "./LatinosAnalyzer " << massSampleNameMM.Data() << " " << w << "*$lumiMM " << massId[i] << " 0 " << release << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameMM = sampleName[isample].ReplaceAll("_ee","_mm");
    weightsFile << "./LatinosAnalyzer " << sampleNameMM.Data() << " " << w << "*$lumiMM " << sampleProcessId[isample] << " 0 " << release << std::endl;
  }


  // now write em
  weightsFile << "echo \"Adding weights for em datasets for \" $lumiEM \" pb-1...\"" << std::endl;
  for(int imass=0; imass<27; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<2; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameEM = massSampleName[i].ReplaceAll("_ee","_em");
      weightsFile << "./LatinosAnalyzer " << massSampleNameEM.Data() << " " << w << "*$lumiEM " << massId[i] << " 2 " << release << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameEM = sampleName[isample].ReplaceAll("_mm","_em");
    weightsFile << "./LatinosAnalyzer " << sampleNameEM.Data() << " " << w << "*$lumiEM " << sampleProcessId[isample] << " 2 " << release << std::endl;
  }

  // now write me
  weightsFile << "echo \"Adding weights for me datasets for \" $lumiEM \" pb-1...\"" << std::endl;
  for(int imass=0; imass<27; imass++) {
    std::vector<double> massXsec = signalXSec[imass];
    std::vector<TString> massSampleName = signalSampleName[imass];
    std::vector<double> massId = signalProcId[imass];
    for(int i=0; i<2; i++) {
      int release = 0;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameME = massSampleName[i].ReplaceAll("_ee","_me");
      weightsFile << "./LatinosAnalyzer " << massSampleNameME.Data() << " " << w << "*$lumiEM " << massId[i] << " 3 " << release << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 0;
    float w = weight(nEv[isample], sampleXsec[isample], 1., 1.);
    TString sampleNameEM = sampleName[isample].ReplaceAll("_em","_me");
    weightsFile << "./LatinosAnalyzer " << sampleNameEM.Data() << " " << w << "*$lumiEM " << sampleProcessId[isample] << " 3 " << release << std::endl;
  }
  weightsFile << "echo \"done weighting.\"" << std::endl;

}

double weight(double ngen, double xsec, double filtereff, double lumi) {

  if(ngen==0) return 0;
  return xsec * filtereff * lumi / ngen;

}

void fillGGHXsec() {
  ggHiggs_xsec.insert(std::make_pair(110 , 0.126698));
  ggHiggs_xsec.insert(std::make_pair(111 , 0.141));
  ggHiggs_xsec.insert(std::make_pair(112 , 0.15636));
  ggHiggs_xsec.insert(std::make_pair(113 , 0.17285));
  ggHiggs_xsec.insert(std::make_pair(114 , 0.190294));
  ggHiggs_xsec.insert(std::make_pair(115 , 0.208969));
  ggHiggs_xsec.insert(std::make_pair(116 , 0.228503));
  ggHiggs_xsec.insert(std::make_pair(117 , 0.24936));
  ggHiggs_xsec.insert(std::make_pair(118 , 0.270536));
  ggHiggs_xsec.insert(std::make_pair(119 , 0.293135));
  ggHiggs_xsec.insert(std::make_pair(120 , 0.317194));
  ggHiggs_xsec.insert(std::make_pair(121 , 0.340626));
  ggHiggs_xsec.insert(std::make_pair(122 , 0.365484));
  ggHiggs_xsec.insert(std::make_pair(123 , 0.391324));
  ggHiggs_xsec.insert(std::make_pair(124 , 0.416335));
  ggHiggs_xsec.insert(std::make_pair(125 , 0.442612));
  ggHiggs_xsec.insert(std::make_pair(126 , 0.47011));
  ggHiggs_xsec.insert(std::make_pair(127 , 0.496536));
  ggHiggs_xsec.insert(std::make_pair(128 , 0.524128));
  ggHiggs_xsec.insert(std::make_pair(129 , 0.550925));
  ggHiggs_xsec.insert(std::make_pair(130 , 0.578559));
  ggHiggs_xsec.insert(std::make_pair(131 , 0.605758));
  ggHiggs_xsec.insert(std::make_pair(132 , 0.631919));
  ggHiggs_xsec.insert(std::make_pair(133 , 0.658858));
  ggHiggs_xsec.insert(std::make_pair(134 , 0.685107));
  ggHiggs_xsec.insert(std::make_pair(135 , 0.710306));
  ggHiggs_xsec.insert(std::make_pair(136 , 0.7349));
  ggHiggs_xsec.insert(std::make_pair(137 , 0.758486));
  ggHiggs_xsec.insert(std::make_pair(138 , 0.781551));
  ggHiggs_xsec.insert(std::make_pair(139 , 0.803649));
  ggHiggs_xsec.insert(std::make_pair(140 , 0.82531));
  ggHiggs_xsec.insert(std::make_pair(141 , 0.846596));
  ggHiggs_xsec.insert(std::make_pair(142 , 0.86643));
  ggHiggs_xsec.insert(std::make_pair(143 , 0.886522));
  ggHiggs_xsec.insert(std::make_pair(144 , 0.905163));
  ggHiggs_xsec.insert(std::make_pair(145 , 0.922023));
  ggHiggs_xsec.insert(std::make_pair(146 , 0.938737));
  ggHiggs_xsec.insert(std::make_pair(147 , 0.954694));
  ggHiggs_xsec.insert(std::make_pair(148 , 0.971364));
  ggHiggs_xsec.insert(std::make_pair(149 , 0.985784));
  ggHiggs_xsec.insert(std::make_pair(150 , 1.00018));
  ggHiggs_xsec.insert(std::make_pair(151 , 1.01386));
  ggHiggs_xsec.insert(std::make_pair(152 , 1.02759));
  ggHiggs_xsec.insert(std::make_pair(153 , 1.04202));
  ggHiggs_xsec.insert(std::make_pair(154 , 1.05438));
  ggHiggs_xsec.insert(std::make_pair(155 , 1.0674));
  ggHiggs_xsec.insert(std::make_pair(156 , 1.08189));
  ggHiggs_xsec.insert(std::make_pair(157 , 1.09698));
  ggHiggs_xsec.insert(std::make_pair(158 , 1.11173));
  ggHiggs_xsec.insert(std::make_pair(159 , 1.12692));
  ggHiggs_xsec.insert(std::make_pair(160 , 1.13905));
  ggHiggs_xsec.insert(std::make_pair(162 , 1.13643));
  ggHiggs_xsec.insert(std::make_pair(164 , 1.11111));
  ggHiggs_xsec.insert(std::make_pair(166 , 1.08258));
  ggHiggs_xsec.insert(std::make_pair(168 , 1.05245));
  ggHiggs_xsec.insert(std::make_pair(170 , 1.02411));
  ggHiggs_xsec.insert(std::make_pair(172 , 0.995654));
  ggHiggs_xsec.insert(std::make_pair(174 , 0.967156));
  ggHiggs_xsec.insert(std::make_pair(176 , 0.939141));
  ggHiggs_xsec.insert(std::make_pair(178 , 0.906938));
  ggHiggs_xsec.insert(std::make_pair(180 , 0.868211));
  ggHiggs_xsec.insert(std::make_pair(182 , 0.818825));
  ggHiggs_xsec.insert(std::make_pair(184 , 0.762372));
  ggHiggs_xsec.insert(std::make_pair(186 , 0.715526));
  ggHiggs_xsec.insert(std::make_pair(188 , 0.678581));
  ggHiggs_xsec.insert(std::make_pair(190 , 0.649198));
  ggHiggs_xsec.insert(std::make_pair(192 , 0.622804));
  ggHiggs_xsec.insert(std::make_pair(194 , 0.600109));
  ggHiggs_xsec.insert(std::make_pair(196 , 0.581706));
  ggHiggs_xsec.insert(std::make_pair(198 , 0.566431));
  ggHiggs_xsec.insert(std::make_pair(200 , 0.554389));
  ggHiggs_xsec.insert(std::make_pair(202 , 0.540991));
  ggHiggs_xsec.insert(std::make_pair(204 , 0.52906));
  ggHiggs_xsec.insert(std::make_pair(206 , 0.517151));
  ggHiggs_xsec.insert(std::make_pair(208 , 0.505659));
  ggHiggs_xsec.insert(std::make_pair(210 , 0.495915));
  ggHiggs_xsec.insert(std::make_pair(212 , 0.485991));
  ggHiggs_xsec.insert(std::make_pair(214 , 0.477548));
  ggHiggs_xsec.insert(std::make_pair(216 , 0.470348));
  ggHiggs_xsec.insert(std::make_pair(218 , 0.460705));
  ggHiggs_xsec.insert(std::make_pair(220 , 0.452565));
  ggHiggs_xsec.insert(std::make_pair(222 , 0.443823));
  ggHiggs_xsec.insert(std::make_pair(224 , 0.436707));
  ggHiggs_xsec.insert(std::make_pair(226 , 0.430502));
  ggHiggs_xsec.insert(std::make_pair(228 , 0.422751));
  ggHiggs_xsec.insert(std::make_pair(230 , 0.415689));
  ggHiggs_xsec.insert(std::make_pair(232 , 0.409164));
  ggHiggs_xsec.insert(std::make_pair(234 , 0.403027));
  ggHiggs_xsec.insert(std::make_pair(236 , 0.396653));
  ggHiggs_xsec.insert(std::make_pair(238 , 0.390171));
  ggHiggs_xsec.insert(std::make_pair(240 , 0.38304));
  ggHiggs_xsec.insert(std::make_pair(242 , 0.37698));
  ggHiggs_xsec.insert(std::make_pair(244 , 0.370467));
  ggHiggs_xsec.insert(std::make_pair(246 , 0.364413));
  ggHiggs_xsec.insert(std::make_pair(248 , 0.35918));
  ggHiggs_xsec.insert(std::make_pair(250 , 0.35337));
  ggHiggs_xsec.insert(std::make_pair(252 , 0.348366));
  ggHiggs_xsec.insert(std::make_pair(254 , 0.342946));
  ggHiggs_xsec.insert(std::make_pair(256 , 0.33817));
  ggHiggs_xsec.insert(std::make_pair(258 , 0.333064));
  ggHiggs_xsec.insert(std::make_pair(260 , 0.328661));
  ggHiggs_xsec.insert(std::make_pair(262 , 0.323941));
  ggHiggs_xsec.insert(std::make_pair(264 , 0.319911));
  ggHiggs_xsec.insert(std::make_pair(266 , 0.315661));
  ggHiggs_xsec.insert(std::make_pair(268 , 0.311185));
  ggHiggs_xsec.insert(std::make_pair(270 , 0.30716));
  ggHiggs_xsec.insert(std::make_pair(272 , 0.30314));
  ggHiggs_xsec.insert(std::make_pair(274 , 0.29956));
  ggHiggs_xsec.insert(std::make_pair(276 , 0.296126));
  ggHiggs_xsec.insert(std::make_pair(278 , 0.292417));
  ggHiggs_xsec.insert(std::make_pair(280 , 0.289207));
  ggHiggs_xsec.insert(std::make_pair(282 , 0.28607));
  ggHiggs_xsec.insert(std::make_pair(284 , 0.282671));
  ggHiggs_xsec.insert(std::make_pair(286 , 0.27983));
  ggHiggs_xsec.insert(std::make_pair(288 , 0.277061));
  ggHiggs_xsec.insert(std::make_pair(290 , 0.274043));
  ggHiggs_xsec.insert(std::make_pair(295 , 0.267546));
  ggHiggs_xsec.insert(std::make_pair(300 , 0.261952));
  ggHiggs_xsec.insert(std::make_pair(305 , 0.256713));
  ggHiggs_xsec.insert(std::make_pair(310 , 0.252213));
  ggHiggs_xsec.insert(std::make_pair(315 , 0.248736));
  ggHiggs_xsec.insert(std::make_pair(320 , 0.245338));
  ggHiggs_xsec.insert(std::make_pair(325 , 0.243313));
  ggHiggs_xsec.insert(std::make_pair(330 , 0.241876));
  ggHiggs_xsec.insert(std::make_pair(335 , 0.241525));
  ggHiggs_xsec.insert(std::make_pair(340 , 0.242823));
  ggHiggs_xsec.insert(std::make_pair(345 , 0.244849));
  ggHiggs_xsec.insert(std::make_pair(350 , 0.241703));
  ggHiggs_xsec.insert(std::make_pair(360 , 0.23167));
  ggHiggs_xsec.insert(std::make_pair(370 , 0.219926));
  ggHiggs_xsec.insert(std::make_pair(380 , 0.206815));
  ggHiggs_xsec.insert(std::make_pair(390 , 0.192866));
  ggHiggs_xsec.insert(std::make_pair(400 , 0.178645));
  ggHiggs_xsec.insert(std::make_pair(420 , 0.151095));
  ggHiggs_xsec.insert(std::make_pair(440 , 0.126782));
  ggHiggs_xsec.insert(std::make_pair(450 , 0.115962));
  ggHiggs_xsec.insert(std::make_pair(460 , 0.10587));
  ggHiggs_xsec.insert(std::make_pair(480 , 0.0881534));
  ggHiggs_xsec.insert(std::make_pair(500 , 0.0735376));
  ggHiggs_xsec.insert(std::make_pair(520 , 0.061384));
  ggHiggs_xsec.insert(std::make_pair(540 , 0.0513615));
  ggHiggs_xsec.insert(std::make_pair(550 , 0.0474351));
  ggHiggs_xsec.insert(std::make_pair(560 , 0.043124));
  ggHiggs_xsec.insert(std::make_pair(580 , 0.0362912));
  ggHiggs_xsec.insert(std::make_pair(600 , 0.0306356));
  ggHiggs_xsec.insert(std::make_pair(620 , 0.0259762));
  ggHiggs_xsec.insert(std::make_pair(640 , 0.022091));
  ggHiggs_xsec.insert(std::make_pair(650 , 0.0203921));
  ggHiggs_xsec.insert(std::make_pair(660 , 0.0188273));
  ggHiggs_xsec.insert(std::make_pair(680 , 0.0161205));
  ggHiggs_xsec.insert(std::make_pair(700 , 0.0138587));
  ggHiggs_xsec.insert(std::make_pair(720 , 0.0119664));
  ggHiggs_xsec.insert(std::make_pair(740 , 0.0103423));
  ggHiggs_xsec.insert(std::make_pair(750 , 0.0096334));
  ggHiggs_xsec.insert(std::make_pair(760 , 0.00898112));
  ggHiggs_xsec.insert(std::make_pair(780 , 0.00781714));
  ggHiggs_xsec.insert(std::make_pair(800 , 0.00682795));
  ggHiggs_xsec.insert(std::make_pair(820 , 0.00598317));
  ggHiggs_xsec.insert(std::make_pair(840 , 0.00526554));
  ggHiggs_xsec.insert(std::make_pair(850 , 0.0049362));
  ggHiggs_xsec.insert(std::make_pair(860 , 0.00464192));
  ggHiggs_xsec.insert(std::make_pair(880 , 0.00409874));
  ggHiggs_xsec.insert(std::make_pair(900 , 0.0036338));
  ggHiggs_xsec.insert(std::make_pair(920 , 0.00323154));
  ggHiggs_xsec.insert(std::make_pair(940 , 0.00287664));
  ggHiggs_xsec.insert(std::make_pair(950 , 0.00271982));
  ggHiggs_xsec.insert(std::make_pair(960 , 0.00257073));
  ggHiggs_xsec.insert(std::make_pair(980 , 0.00229965));
  ggHiggs_xsec.insert(std::make_pair(1000 , 0.00206196));
}

void fillQQHXsec() {
  qqHiggs_xsec.insert(std::make_pair(110 , 0.00906218));
  qqHiggs_xsec.insert(std::make_pair(111 , 0.0101737));
  qqHiggs_xsec.insert(std::make_pair(112 , 0.0113681));
  qqHiggs_xsec.insert(std::make_pair(113 , 0.0126728));
  qqHiggs_xsec.insert(std::make_pair(114 , 0.0140521));
  qqHiggs_xsec.insert(std::make_pair(115 , 0.0155543));
  qqHiggs_xsec.insert(std::make_pair(116 , 0.0171326));
  qqHiggs_xsec.insert(std::make_pair(117 , 0.018848));
  qqHiggs_xsec.insert(std::make_pair(118 , 0.0205751));
  qqHiggs_xsec.insert(std::make_pair(119 , 0.0224764));
  qqHiggs_xsec.insert(std::make_pair(120 , 0.0244989));
  qqHiggs_xsec.insert(std::make_pair(121 , 0.0264804));
  qqHiggs_xsec.insert(std::make_pair(122 , 0.0285892));
  qqHiggs_xsec.insert(std::make_pair(123 , 0.0308398));
  qqHiggs_xsec.insert(std::make_pair(124 , 0.0330254));
  qqHiggs_xsec.insert(std::make_pair(125 , 0.03535));
  qqHiggs_xsec.insert(std::make_pair(126 , 0.0377653));
  qqHiggs_xsec.insert(std::make_pair(127 , 0.0401796));
  qqHiggs_xsec.insert(std::make_pair(128 , 0.0426786));
  qqHiggs_xsec.insert(std::make_pair(129 , 0.0451548));
  qqHiggs_xsec.insert(std::make_pair(130 , 0.0477063));
  qqHiggs_xsec.insert(std::make_pair(131 , 0.0502361));
  qqHiggs_xsec.insert(std::make_pair(132 , 0.0526779));
  qqHiggs_xsec.insert(std::make_pair(133 , 0.0552922));
  qqHiggs_xsec.insert(std::make_pair(134 , 0.0578562));
  qqHiggs_xsec.insert(std::make_pair(135 , 0.0602851));
  qqHiggs_xsec.insert(std::make_pair(136 , 0.0627441));
  qqHiggs_xsec.insert(std::make_pair(137 , 0.0651526));
  qqHiggs_xsec.insert(std::make_pair(138 , 0.0674622));
  qqHiggs_xsec.insert(std::make_pair(139 , 0.0697679));
  qqHiggs_xsec.insert(std::make_pair(140 , 0.072076));
  qqHiggs_xsec.insert(std::make_pair(141 , 0.0742281));
  qqHiggs_xsec.insert(std::make_pair(142 , 0.0764396));
  qqHiggs_xsec.insert(std::make_pair(143 , 0.0785459));
  qqHiggs_xsec.insert(std::make_pair(144 , 0.0806017));
  qqHiggs_xsec.insert(std::make_pair(145 , 0.0825334));
  qqHiggs_xsec.insert(std::make_pair(146 , 0.0844212));
  qqHiggs_xsec.insert(std::make_pair(147 , 0.0862651));
  qqHiggs_xsec.insert(std::make_pair(148 , 0.0881295));
  qqHiggs_xsec.insert(std::make_pair(149 , 0.0898824));
  qqHiggs_xsec.insert(std::make_pair(150 , 0.0916648));
  qqHiggs_xsec.insert(std::make_pair(151 , 0.0933321));
  qqHiggs_xsec.insert(std::make_pair(152 , 0.0949554));
  qqHiggs_xsec.insert(std::make_pair(153 , 0.0966627));
  qqHiggs_xsec.insert(std::make_pair(154 , 0.0982785));
  qqHiggs_xsec.insert(std::make_pair(155 , 0.0998967));
  qqHiggs_xsec.insert(std::make_pair(156 , 0.101679));
  qqHiggs_xsec.insert(std::make_pair(157 , 0.103453));
  qqHiggs_xsec.insert(std::make_pair(158 , 0.105388));
  qqHiggs_xsec.insert(std::make_pair(159 , 0.107304));
  qqHiggs_xsec.insert(std::make_pair(160 , 0.109235));
  qqHiggs_xsec.insert(std::make_pair(162 , 0.112455));
  qqHiggs_xsec.insert(std::make_pair(164 , 0.112819));
  qqHiggs_xsec.insert(std::make_pair(166 , 0.111692));
  qqHiggs_xsec.insert(std::make_pair(168 , 0.110102));
  qqHiggs_xsec.insert(std::make_pair(170 , 0.108281));
  qqHiggs_xsec.insert(std::make_pair(172 , 0.106349));
  qqHiggs_xsec.insert(std::make_pair(174 , 0.104304));
  qqHiggs_xsec.insert(std::make_pair(176 , 0.102063));
  qqHiggs_xsec.insert(std::make_pair(178 , 0.0994123));
  qqHiggs_xsec.insert(std::make_pair(180 , 0.0960766));
  qqHiggs_xsec.insert(std::make_pair(182 , 0.0916651));
  qqHiggs_xsec.insert(std::make_pair(184 , 0.0864897));
  qqHiggs_xsec.insert(std::make_pair(186 , 0.081957));
  qqHiggs_xsec.insert(std::make_pair(188 , 0.078277));
  qqHiggs_xsec.insert(std::make_pair(190 , 0.0754069));
  qqHiggs_xsec.insert(std::make_pair(192 , 0.0729211));
  qqHiggs_xsec.insert(std::make_pair(194 , 0.0707317));
  qqHiggs_xsec.insert(std::make_pair(196 , 0.0688815));
  qqHiggs_xsec.insert(std::make_pair(198 , 0.0671448));
  qqHiggs_xsec.insert(std::make_pair(200 , 0.0656602));
  qqHiggs_xsec.insert(std::make_pair(202 , 0.0641973));
  qqHiggs_xsec.insert(std::make_pair(204 , 0.0628417));
  qqHiggs_xsec.insert(std::make_pair(206 , 0.0615278));
  qqHiggs_xsec.insert(std::make_pair(208 , 0.0603305));
  qqHiggs_xsec.insert(std::make_pair(210 , 0.0592381));
  qqHiggs_xsec.insert(std::make_pair(212 , 0.0581811));
  qqHiggs_xsec.insert(std::make_pair(214 , 0.0571216));
  qqHiggs_xsec.insert(std::make_pair(216 , 0.0560896));
  qqHiggs_xsec.insert(std::make_pair(218 , 0.0550925));
  qqHiggs_xsec.insert(std::make_pair(220 , 0.0541834));
  qqHiggs_xsec.insert(std::make_pair(222 , 0.053217));
  qqHiggs_xsec.insert(std::make_pair(224 , 0.052366));
  qqHiggs_xsec.insert(std::make_pair(226 , 0.0515246));
  qqHiggs_xsec.insert(std::make_pair(228 , 0.0506706));
  qqHiggs_xsec.insert(std::make_pair(230 , 0.0498484));
  qqHiggs_xsec.insert(std::make_pair(232 , 0.0490581));
  qqHiggs_xsec.insert(std::make_pair(234 , 0.0482698));
  qqHiggs_xsec.insert(std::make_pair(236 , 0.0475658));
  qqHiggs_xsec.insert(std::make_pair(238 , 0.0468175));
  qqHiggs_xsec.insert(std::make_pair(240 , 0.0460712));
  qqHiggs_xsec.insert(std::make_pair(242 , 0.0454061));
  qqHiggs_xsec.insert(std::make_pair(244 , 0.0446921));
  qqHiggs_xsec.insert(std::make_pair(246 , 0.0439874));
  qqHiggs_xsec.insert(std::make_pair(248 , 0.0433684));
  qqHiggs_xsec.insert(std::make_pair(250 , 0.0426959));
  qqHiggs_xsec.insert(std::make_pair(252 , 0.0420924));
  qqHiggs_xsec.insert(std::make_pair(254 , 0.0414445));
  qqHiggs_xsec.insert(std::make_pair(256 , 0.0408714));
  qqHiggs_xsec.insert(std::make_pair(258 , 0.0402406));
  qqHiggs_xsec.insert(std::make_pair(260 , 0.0396829));
  qqHiggs_xsec.insert(std::make_pair(262 , 0.0390766));
  qqHiggs_xsec.insert(std::make_pair(264 , 0.0385344));
  qqHiggs_xsec.insert(std::make_pair(266 , 0.0380068));
  qqHiggs_xsec.insert(std::make_pair(268 , 0.0374329));
  qqHiggs_xsec.insert(std::make_pair(270 , 0.036928));
  qqHiggs_xsec.insert(std::make_pair(272 , 0.0363709));
  qqHiggs_xsec.insert(std::make_pair(274 , 0.0358741));
  qqHiggs_xsec.insert(std::make_pair(276 , 0.0353992));
  qqHiggs_xsec.insert(std::make_pair(278 , 0.0348741));
  qqHiggs_xsec.insert(std::make_pair(280 , 0.0343998));
  qqHiggs_xsec.insert(std::make_pair(282 , 0.0339402));
  qqHiggs_xsec.insert(std::make_pair(284 , 0.0334397));
  qqHiggs_xsec.insert(std::make_pair(286 , 0.0330026));
  qqHiggs_xsec.insert(std::make_pair(288 , 0.0325654));
  qqHiggs_xsec.insert(std::make_pair(290 , 0.0343081));
  qqHiggs_xsec.insert(std::make_pair(295 , 0.0331399));
  qqHiggs_xsec.insert(std::make_pair(300 , 0.0320212));
  qqHiggs_xsec.insert(std::make_pair(305 , 0.0309449));
  qqHiggs_xsec.insert(std::make_pair(310 , 0.0299223));
  qqHiggs_xsec.insert(std::make_pair(315 , 0.0289661));
  qqHiggs_xsec.insert(std::make_pair(320 , 0.0280273));
  qqHiggs_xsec.insert(std::make_pair(325 , 0.0271449));
  qqHiggs_xsec.insert(std::make_pair(330 , 0.0262677));
  qqHiggs_xsec.insert(std::make_pair(335 , 0.025429));
  qqHiggs_xsec.insert(std::make_pair(340 , 0.024679));
  qqHiggs_xsec.insert(std::make_pair(345 , 0.0237514));
  qqHiggs_xsec.insert(std::make_pair(350 , 0.0227084));
  qqHiggs_xsec.insert(std::make_pair(360 , 0.0206932));
  qqHiggs_xsec.insert(std::make_pair(370 , 0.0190919));
  qqHiggs_xsec.insert(std::make_pair(380 , 0.0177471));
  qqHiggs_xsec.insert(std::make_pair(390 , 0.0165866));
  qqHiggs_xsec.insert(std::make_pair(400 , 0.0155367));
  qqHiggs_xsec.insert(std::make_pair(420 , 0.0137181));
  qqHiggs_xsec.insert(std::make_pair(440 , 0.0122304));
  qqHiggs_xsec.insert(std::make_pair(450 , 0.0115904));
  qqHiggs_xsec.insert(std::make_pair(460 , 0.0109789));
  qqHiggs_xsec.insert(std::make_pair(480 , 0.00988143));
  qqHiggs_xsec.insert(std::make_pair(500 , 0.00894717));
  qqHiggs_xsec.insert(std::make_pair(520 , 0.00811945));
  qqHiggs_xsec.insert(std::make_pair(540 , 0.00739416));
  qqHiggs_xsec.insert(std::make_pair(550 , 0.00712605));
  qqHiggs_xsec.insert(std::make_pair(560 , 0.00675659));
  qqHiggs_xsec.insert(std::make_pair(580 , 0.00618739));
  qqHiggs_xsec.insert(std::make_pair(600 , 0.0056749));
  qqHiggs_xsec.insert(std::make_pair(620 , 0.00522768));
  qqHiggs_xsec.insert(std::make_pair(640 , 0.00482521));
  qqHiggs_xsec.insert(std::make_pair(650 , 0.00463723));
  qqHiggs_xsec.insert(std::make_pair(660 , 0.00445536));
  qqHiggs_xsec.insert(std::make_pair(680 , 0.00412938));
  qqHiggs_xsec.insert(std::make_pair(700 , 0.00383415));
  qqHiggs_xsec.insert(std::make_pair(720 , 0.00356981));
  qqHiggs_xsec.insert(std::make_pair(740 , 0.00332278));
  qqHiggs_xsec.insert(std::make_pair(750 , 0.00322036));
  qqHiggs_xsec.insert(std::make_pair(760 , 0.00310605));
  qqHiggs_xsec.insert(std::make_pair(780 , 0.00290475));
  qqHiggs_xsec.insert(std::make_pair(800 , 0.00272183));
  qqHiggs_xsec.insert(std::make_pair(820 , 0.00255571));
  qqHiggs_xsec.insert(std::make_pair(840 , 0.00240691));
  qqHiggs_xsec.insert(std::make_pair(850 , 0.00234203));
  qqHiggs_xsec.insert(std::make_pair(860 , 0.00226928));
  qqHiggs_xsec.insert(std::make_pair(880 , 0.00213939));
  qqHiggs_xsec.insert(std::make_pair(900 , 0.00202276));
  qqHiggs_xsec.insert(std::make_pair(920 , 0.00191837));
  qqHiggs_xsec.insert(std::make_pair(940 , 0.00181764));
  qqHiggs_xsec.insert(std::make_pair(950 , 0.00177506));
  qqHiggs_xsec.insert(std::make_pair(960 , 0.00172872));
  qqHiggs_xsec.insert(std::make_pair(980 , 0.0016401));
  qqHiggs_xsec.insert(std::make_pair(1000 , 0.00156391));
}
