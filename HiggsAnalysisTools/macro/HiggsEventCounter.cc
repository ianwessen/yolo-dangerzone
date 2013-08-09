#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#define NSAMPLES 30

using namespace std;

double weight(double ngen, double xsec, double filtereff, double lumi = 1);

void countEvents() {

  char nametree[200];
  sprintf(nametree,"FULL_SELECTION_EVENT_COUNTER_EE");

  cout << "nametree = " << nametree << endl;

  // signals - first set
  std::vector<std::vector<TChain*> > signalChains;
  int mH[17] = {120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600};
  for(int imass=0; imass<17;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH[imass]);

    std::vector<TChain*> massChain;

    TChain *sigChains[4];
    for(int i=0; i<4; i++) sigChains[i] = new TChain(nametree);

    TString hSample("results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[0]->Add(hSample.Data());
    
    hSample = TString("results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[1]->Add(hSample.Data());
    
    hSample = TString("results/Fall11_V1/VBF_HToWWTo2L2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[2]->Add(hSample.Data());
    
    hSample = TString("results/Fall11_V1/VBF_HToWWToLNuTauNu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[3]->Add(hSample.Data());

    for(int i=0; i<4; i++) massChain.push_back(sigChains[i]);
    signalChains.push_back(massChain);
  }

  // signals - second set
  std::vector<std::vector<TChain*> > signalChains2;
  int mH2[8] = {110,115,118,122,124,126,128,135};
  for(int imass=0; imass<8;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH2[imass]);

    std::vector<TChain*> massChain;

    TChain *sigChains[2];
    for(int i=0; i<2; i++) sigChains[i] = new TChain(nametree);

    TString hSample("results/Fall11_V1/GluGluToHToWWTo2LAndTau2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[0]->Add(hSample.Data());
    
    hSample = TString("results/Fall11_V1/VBF_HToWWTo2LAndTau2Nu_M-");
    hSample += TString(mass)+TString("_7TeV-powheg-pythia6/*Counters.root");
    sigChains[1]->Add(hSample.Data());
    
    for(int i=0; i<2; i++) massChain.push_back(sigChains[i]);
    signalChains2.push_back(massChain);
  }

  // backgrounds
  TChain *chains[NSAMPLES];
  for(int isample=0; isample<NSAMPLES; isample++) {
    chains[isample] = new TChain(nametree);
  }

  // nominal sample first, then the systematics ones
  chains[0]->Add("results/Fall11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*Counters.root");

  chains[1]->Add("results/Fall11_V1/DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[2]->Add("results/Fall11_V1/DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[3]->Add("results/Fall11_V1/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*Counters.root");

  chains[4]->Add("results/Fall11_V1/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[5]->Add("results/Fall11_V1/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*Counters.root");
  chains[6]->Add("results/Fall11_V1/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*Counters.root");

  // madgraph samples
  //  chains[7]->Add("results/Fall11_V1/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*Counters.root");
  //  chains[8]->Add("results/Fall11_V1/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*Counters.root");
  //  chains[9]->Add("results/Fall11_V1/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*Counters.root");

  chains[7]->Add("results/Fall11_V1/T_TuneZ2_s-channel_7TeV-powheg-tauola/*Counters.root");
  chains[8]->Add("results/Fall11_V1/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/*Counters.root");
  chains[9]->Add("results/Fall11_V1/T_TuneZ2_t-channel_7TeV-powheg-tauola/*Counters.root");
  chains[10]->Add("results/Fall11_V1/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/*Counters.root");
  chains[11]->Add("results/Fall11_V1/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*Counters.root");
  chains[12]->Add("results/Fall11_V1/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*Counters.root");

  //  chains[13]->Add("results/Fall11_V1/TTJets_TuneZ2_7TeV-madgraph-tauola/*Counters.root");
  chains[13]->Add("results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*Counters.root"); 

  chains[14]->Add("results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*Counters.root");
  chains[15]->Add("results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*Counters.root");
  chains[16]->Add("results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*Counters.root");

  chains[17]->Add("results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*Counters.root");
  chains[18]->Add("results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*Counters.root");
  chains[19]->Add("results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*Counters.root");
  chains[20]->Add("results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*Counters.root");

  chains[21]->Add("results/Fall11_V1/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/*Counters.root"); // nominal WW MADGRAPH sample
  chains[22]->Add("results/Fall11_V1/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*Counters.root");
  chains[23]->Add("results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*Counters.root");
  chains[24]->Add("results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*Counters.root");

  // samples for systematics
  chains[25]->Add("results/Fall11_V1/WWTo2L2Nu_CT10_7TeV-mcatnlo/*Counters.root"); // nominal MC@NLO sample
  chains[26]->Add("results/Fall11_V1/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*Counters.root"); // syst MC@NLO sample 
  chains[27]->Add("results/Fall11_V1/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*Counters.root"); // syst MC@NLO sample 
  chains[28]->Add("results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*Counters.root"); // PYTHIA sample 
  chains[29]->Add("results/Fall11_V1/GVJets_7TeV-madgraph/*Counters.root"); // Vgamma inclusive

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

  std::vector<std::vector<TString> > signalSampleName2;
  for(int imass=0; imass<8;imass++) {
    char mass[5];
    sprintf(mass,"%d",mH2[imass]);
    
    std::vector<TString> massSampleName;
    massSampleName.push_back(TString("results/merged/ggH")+TString(mass)+TString("2LAndTau2Nu_ee.root"));
    massSampleName.push_back(TString("results/merged/qqH")+TString(mass)+TString("2LAndTau2Nu_ee.root"));

    signalSampleName2.push_back(massSampleName);
  }

  std::vector<TString> sampleName;
  sampleName.push_back("results/merged/Wjets_ee.root"); // 0

  sampleName.push_back("results/merged/Zee_Lo_ee.root"); // 1
  sampleName.push_back("results/merged/Zmm_Lo_ee.root"); // 2
  sampleName.push_back("results/merged/Ztautau_Lo_ee.root"); // 3

  sampleName.push_back("results/merged/Zee_Hi_ee.root"); // 4 
  sampleName.push_back("results/merged/Zmm_Hi_ee.root"); // 5 
  sampleName.push_back("results/merged/Ztautau_Hi_ee.root"); // 6

  sampleName.push_back("results/merged/SingleT_sChannel_ee.root"); // 7 
  sampleName.push_back("results/merged/SingleTbar_sChannel_ee.root"); // 8
  sampleName.push_back("results/merged/SingleT_tChannel_ee.root"); // 9
  sampleName.push_back("results/merged/SingleTbar_tChannel_ee.root"); // 10
  sampleName.push_back("results/merged/SingleT_tWChannel_ee.root"); // 11
  sampleName.push_back("results/merged/SingleTbar_tWChannel_ee.root"); // 12

  sampleName.push_back("results/merged/TTbar_ee.root"); // 13

  sampleName.push_back("results/merged/WGToENuG_ee.root"); // 14
  sampleName.push_back("results/merged/WGToMuNuG_ee.root"); // 15
  sampleName.push_back("results/merged/WGToTauNuG_ee.root"); // 16

  sampleName.push_back("results/merged/ZGToEEG_ee.root"); // 17
  sampleName.push_back("results/merged/ZGToMuMuG_ee.root"); // 18
  sampleName.push_back("results/merged/ZGToTauTauG_ee.root"); // 19
  sampleName.push_back("results/merged/ZGToNuNuG_ee.root"); // 20

  sampleName.push_back("results/merged/WW_ee.root"); // 21
  sampleName.push_back("results/merged/ggWW_ee.root"); // 22
  sampleName.push_back("results/merged/WZ_ee.root"); // 23
  sampleName.push_back("results/merged/ZZ_ee.root"); // 24

  sampleName.push_back("results/merged/WW_mcatnlo_ee.root"); // 25
  sampleName.push_back("results/merged/WW_mcatnlo_scaledown_ee.root"); // 26
  sampleName.push_back("results/merged/WW_mcatnlo_scaleup_ee.root"); // 27
  sampleName.push_back("results/merged/WW_pythia_ee.root"); // 28
  sampleName.push_back("results/merged/Wgamma_inclusive_ee.root"); // 29

  std::map<int,float> ggHiggs_xsec;
  // samples are emu only
  ggHiggs_xsec.insert(std::make_pair(110,0.100387));
  ggHiggs_xsec.insert(std::make_pair(115,0.165009));
  ggHiggs_xsec.insert(std::make_pair(118,0.213183));
  ggHiggs_xsec.insert(std::make_pair(120,0.249642));
  ggHiggs_xsec.insert(std::make_pair(122,0.286962));
  ggHiggs_xsec.insert(std::make_pair(124,0.326685));
  ggHiggs_xsec.insert(std::make_pair(126,0.368359));
  ggHiggs_xsec.insert(std::make_pair(128,0.410187));
  ggHiggs_xsec.insert(std::make_pair(130,0.452090));
  ggHiggs_xsec.insert(std::make_pair(135,0.553354));
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
  qqHiggs_xsec.insert(std::make_pair(110,0.007073));
  qqHiggs_xsec.insert(std::make_pair(115,0.012126));
  qqHiggs_xsec.insert(std::make_pair(118,0.016029));
  qqHiggs_xsec.insert(std::make_pair(120,0.019057));
  qqHiggs_xsec.insert(std::make_pair(122,0.022236));
  qqHiggs_xsec.insert(std::make_pair(124,0.025656));
  qqHiggs_xsec.insert(std::make_pair(126,0.029327));
  qqHiggs_xsec.insert(std::make_pair(128,0.033085));
  qqHiggs_xsec.insert(std::make_pair(130,0.036942));
  qqHiggs_xsec.insert(std::make_pair(135,0.046536));
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

  // these for the 2L2Nu, 2Tau2Nu samples
  std::vector<std::vector<double> > signalXSec;
  for(int imass=0; imass<17;imass++) {
    std::vector<double> massXsec;
    double ggHiggs_xsec_2l2nu = ggHiggs_xsec[mH[imass]] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    double ggHiggs_xsec_taunulnu = ggHiggs_xsec[mH[imass]] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3
    double qqHiggs_xsec_2l2nu = qqHiggs_xsec[mH[imass]] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    double qqHiggs_xsec_taunulnu = qqHiggs_xsec[mH[imass]] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3

    massXsec.push_back(ggHiggs_xsec_2l2nu);
    massXsec.push_back(ggHiggs_xsec_taunulnu);
    massXsec.push_back(qqHiggs_xsec_2l2nu);
    massXsec.push_back(qqHiggs_xsec_taunulnu);

    signalXSec.push_back(massXsec);
  }

  // these for the 2LAndTauNu samples
  std::vector<std::vector<double> > signalXSec2;
  for(int imass=0; imass<8;imass++) {
    std::vector<double> massXsec;
    double ggHiggs_xsec_2landtau2nu = ggHiggs_xsec[mH2[imass]];
    double qqHiggs_xsec_2landtau2nu = qqHiggs_xsec[mH2[imass]];

    massXsec.push_back(ggHiggs_xsec_2landtau2nu);
    massXsec.push_back(qqHiggs_xsec_2landtau2nu);

    signalXSec2.push_back(massXsec);
  }

  std::vector<float> sampleXsec;
  sampleXsec.push_back(31314.); // madgraph // 0
  // sampleXsec.push_back(31314. * 0.00111); // PYTHIA BIG filtered
  sampleXsec.push_back(3319.61); // sigma(powheg_nlo_10-20) * sigma(fewz_nnlo_20-inf)/sigma(powheg_nlo_20-inf)=3216*1666/1614 =  3319.61 pb // 1
  sampleXsec.push_back(3319.61); // 2
  sampleXsec.push_back(3319.61); // 3
  sampleXsec.push_back(4998./3.); // 4
  sampleXsec.push_back(4998./3.); // 5
  sampleXsec.push_back(4998./3.); // 6

  // Madgraph single top
  //   sampleXsec.push_back(4.21 * (0.1080*3));
  //   sampleXsec.push_back(64.6 * (0.1080*3));
  //   sampleXsec.push_back(10.6);

  // powheg samples. Xsecs taken from PREP: http://cms.cern.ch/iCMS/prep/requestmanagement?pwg=TOP&campid=Summer11
  sampleXsec.push_back(3.19); // 7
  sampleXsec.push_back(1.44); // 8

  sampleXsec.push_back(41.92); // 9
  sampleXsec.push_back(22.65); // 10

  // prep says 7.46 pb. 7.87 is from Guillelmo
  sampleXsec.push_back(7.87); // 11
  sampleXsec.push_back(7.87); // 12

  // ttbar -> 2l2nu
  sampleXsec.push_back(163.0 * 3*0.108 * 3*0.108); // 13

  // Wgamma
  sampleXsec.push_back(137.5); // 14
  sampleXsec.push_back(137.5); // 15
  sampleXsec.push_back(137.5); // 16

  // Zgamma
  sampleXsec.push_back(41.3); // 17
  sampleXsec.push_back(41.3); // 18
  sampleXsec.push_back(41.3); // 19
  sampleXsec.push_back(41.3 / 0.033658 * 0.20); // 20

  // WW
  // s(NLO qqWW+ggWW) = 47 pb, [K. Ellis]
  // gg/Tot = 0.0305
  // sigma(qqWW) = 47*(1-0.0305) * 0.108*0.108*9 = 4.7833889
  // sigma(ggWW) = 47*0.0305*0.108*0.108*9 = 0.150483
  sampleXsec.push_back(4.7833889); // 21
  sampleXsec.push_back(0.150483); // 22
  sampleXsec.push_back(18.2 * 3*0.1080 * 3*0.033658); // 23
  sampleXsec.push_back(7.41 * 3*0.033658 * 0.2 * 2); // sigma_ZZ = 6.77 (1+0.12/1.277)=7.41pb, where 1.277 is the LO->NLO k-factor. // 24
  // once moving to Madgraph ZZ->2l2nu => 7.41 * 3*0.033658 * 0.2

  
  // systematic samples
  sampleXsec.push_back(4.7833889); // 25
  sampleXsec.push_back(4.7833889); // 26
  sampleXsec.push_back(4.7833889); // 27
  sampleXsec.push_back(4.7833889); // 28
  sampleXsec.push_back(137.5); // 29

  std::vector<std::vector<double> > signalProcId;
  for(int imass=0; imass<17;imass++) {
    std::vector<double> massId;
    massId.push_back(1000+mH[imass]); // ggH->2L2Nu
    massId.push_back(2000+mH[imass]); // ggH->LNuTauNu
    massId.push_back(4000+mH[imass]); // qqH->2L2Nu
    massId.push_back(5000+mH[imass]); // qqH->LNuTauNu
    signalProcId.push_back(massId);
  }

  std::vector<std::vector<double> > signalProcId2;
  for(int imass=0; imass<8;imass++) {
    std::vector<double> massId;
    massId.push_back(9000+mH2[imass]); // ggH->2LAndTau2Nu
    massId.push_back(8000+mH2[imass]); // qqH->2LAndTau2Nu
    signalProcId2.push_back(massId);
  }

  std::vector<int> sampleProcessId; 
  // ids are taken from: https://docs.google.com/spreadsheet/ccc?key=0Ankm0DuoD0h0dHRhV1VNSlV1NEdhNFdyOXh3eFpSMHc&hl=en_US#gid=31
  sampleProcessId.push_back(80); // Wjets
  sampleProcessId.push_back(33); // Z->ee [10-20]
  sampleProcessId.push_back(34); // Z->mumu [10-20]
  sampleProcessId.push_back(35); // Z->tautau [10-20]
  sampleProcessId.push_back(30); // Z->ee [20-inf]
  sampleProcessId.push_back(31); // Z->mumu [20-inf]
  sampleProcessId.push_back(32); // Z->tautau [20-inf]
  sampleProcessId.push_back(15); // t(s-cha) 
  sampleProcessId.push_back(16); // tbar(s-cha) 
  sampleProcessId.push_back(13); // t(t-cha)
  sampleProcessId.push_back(14); // tbar(t-cha)
  sampleProcessId.push_back(19); // tW
  sampleProcessId.push_back(20); // tbarW
  sampleProcessId.push_back(10); // ttbar
  sampleProcessId.push_back(82); // Vgamma->enu
  sampleProcessId.push_back(83); // Vgamma->munu
  sampleProcessId.push_back(84); // Vgamma->taunu
  sampleProcessId.push_back(38); // Zgamma->ee
  sampleProcessId.push_back(39); // Zgamma->mumu
  sampleProcessId.push_back(40); // Zgamma->tautau
  sampleProcessId.push_back(41); // Zgamma->nunu

  sampleProcessId.push_back(0); // qqWW Mad
  sampleProcessId.push_back(1); // ggWW
  sampleProcessId.push_back(74); // WZ->3LNu
  sampleProcessId.push_back(71); // ZZ->anything

  sampleProcessId.push_back(2); // qqWW MC@NLO nominal
  sampleProcessId.push_back(4); // qqWW MC@NLO scaledown
  sampleProcessId.push_back(3); // qqWW MC@NLO scaleup
  sampleProcessId.push_back(6); // qqWW PYTHIA
  sampleProcessId.push_back(81); // Vgamma madgraph inclusive

  // signal samples first set
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

  // signal samples second set
  float nEvH2[8][2];
  for(int imass=0; imass<8; imass++) {
    for(int i=0; i<2; i++) {
      nEvH2[imass][i] = 0.0;
    }
  }

  for(int imass=0; imass<8; imass++) {
    std::vector<TChain*> massChain = signalChains2[imass];
    for(int i=0; i<2; i++) {

      cout << "\tProcessing signal sample mass # " << mH2[imass] << "..." << endl;
      
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

        nEvH2[imass][i] += nSel[0];
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
      int release = 1;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      weightsFile << "addWeights(\"" << massSampleName[i].Data() << "\", " << w << "*$lumiEE, " << massId[i] << " ,1, " << release << ");" << std::endl;
    }
  }
  for(int imass=0; imass<8; imass++) {
    std::vector<double> massXsec = signalXSec2[imass];
    std::vector<TString> massSampleName = signalSampleName2[imass];
    std::vector<double> massId = signalProcId2[imass];
    for(int i=0; i<2; i++) {
      int release = 1;
      float w = weight(nEvH2[imass][i], massXsec[i], 1., 1.);
      weightsFile << "addWeights(\"" << massSampleName[i].Data() << "\", " << w << "*$lumiEE, " << massId[i] << " ,1, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    int release = 1;
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
      int release = 1;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameMM = massSampleName[i].ReplaceAll("_ee","_mm");
      weightsFile << "addWeights(\"" << massSampleNameMM.Data() << "\", " << w << "*$lumiMM, " << massId[i] << " ,0, " << release << ");" << std::endl;
    }
  }
  for(int imass=0; imass<8; imass++) {
    std::vector<double> massXsec = signalXSec2[imass];
    std::vector<TString> massSampleName = signalSampleName2[imass];
    std::vector<double> massId = signalProcId2[imass];
    for(int i=0; i<2; i++) {
      cout << "Events processed for sample: " << massSampleName[i] << " = " << nEvH2[imass][i] << endl;
      int release = 1;
      float w = weight(nEvH2[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameMM = massSampleName[i].ReplaceAll("_ee","_mm");
      weightsFile << "addWeights(\"" << massSampleNameMM.Data() << "\", " << w << "*$lumiMM, " << massId[i] << " ,0, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 1;
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
      int release = 1;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameEM = massSampleName[i].ReplaceAll("_ee","_em");
      weightsFile << "addWeights(\"" << massSampleNameEM.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,2, " << release << ");" << std::endl;
    }
  }
  for(int imass=0; imass<8; imass++) {
    std::vector<double> massXsec = signalXSec2[imass];
    std::vector<TString> massSampleName = signalSampleName2[imass];
    std::vector<double> massId = signalProcId2[imass];
    for(int i=0; i<2; i++) {
      cout << "Events processed for sample: " << massSampleName[i] << " = " << nEvH2[imass][i] << endl;
      int release = 1;
      float w = weight(nEvH2[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameEM = massSampleName[i].ReplaceAll("_ee","_em");
      weightsFile << "addWeights(\"" << massSampleNameEM.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,2, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 1;
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
      int release = 1;
      float w = weight(nEvH[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameME = massSampleName[i].ReplaceAll("_ee","_me");
      weightsFile << "addWeights(\"" << massSampleNameME.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,3, " << release << ");" << std::endl;
    }
  }
  for(int imass=0; imass<8; imass++) {
    std::vector<double> massXsec = signalXSec2[imass];
    std::vector<TString> massSampleName = signalSampleName2[imass];
    std::vector<double> massId = signalProcId2[imass];
    for(int i=0; i<2; i++) {
      cout << "Events processed for sample: " << massSampleName[i] << " = " << nEvH2[imass][i] << endl;
      int release = 1;
      float w = weight(nEvH2[imass][i], massXsec[i], 1., 1.);
      TString massSampleNameME = massSampleName[i].ReplaceAll("_ee","_me");
      weightsFile << "addWeights(\"" << massSampleNameME.Data() << "\", " << w << "*$lumiEM, " << massId[i] << " ,3, " << release << ");" << std::endl;
    }
  }
  for(int isample=0; isample<NSAMPLES; isample++) {
    //    cout << "Events processed for sample: " << sampleName[isample] << " = " << nEv[isample] << endl;
    int release = 1;
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

