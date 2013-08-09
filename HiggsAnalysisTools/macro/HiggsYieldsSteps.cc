#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

// parameters to configure
int exampleHiggsMass = -1;
bool runStandalone = true;
bool printCutsBreakdown = false;

float yieldErrPoisson(float nEst1, float n1, float nEst2=0, float n2=0, float nEst3=0, float n3=0, float nEst4=0, float n4=0, float nEst5=0, float n5=0, float nEst6=0, float n6=0);

using namespace std;

enum { ee=0, mm=1, em=2, me=3, ll=4 };

string fullSelCuts[26];

int UseCuts[4][26];

float H_fullSel[26];
float Wj_fullSel[26];
float ttj_fullSel[26];
float SingleTop_fullSel[26];
float Zj_fullSel[26];
float WW_fullSel[26];
float ZZ_fullSel[26];
float WZ_fullSel[26];
float Wgamma_fullSel[26];
float QCDem_fullSel[26];
float QCDbc_fullSel[26];
float Photj_fullSel[26];
float QCDmu_fullSel[26];
float ttbar_fullSel[26];
float data_fullSel[26];

float H_fullSel_err[26];
float Wj_fullSel_err[26];
float ttj_fullSel_err[26];
float SingleTop_fullSel_err[26];
float Zj_fullSel_err[26];
float WW_fullSel_err[26];
float ZZ_fullSel_err[26];
float WZ_fullSel_err[26];
float Wgamma_fullSel_err[26];
float QCDem_fullSel_err[26];
float QCDbc_fullSel_err[26];
float Photj_fullSel_err[26];
float QCDmu_fullSel_err[26];
float ttbar_fullSel_err[26];
float data_fullSel_err[26];

float H_eff_fullSel[26];
float Wj_eff_fullSel[26];
float ttj_eff_fullSel[26];
float SingleTop_eff_fullSel[26];
float Zj_eff_fullSel[26];
float WW_eff_fullSel[26];
float ZZ_eff_fullSel[26];
float WZ_eff_fullSel[26];
float Wgamma_eff_fullSel[26];
float QCDem_eff_fullSel[26];
float QCDbc_eff_fullSel[26];
float Photj_eff_fullSel[26];
float QCDmu_eff_fullSel[26];
float ttbar_eff_fullSel[26];

float H_finaleff_fullSel;    
float Wj_finaleff_fullSel;
float ttj_finaleff_fullSel;
float SingleTop_finaleff_fullSel;
float Zj_finaleff_fullSel;
float WW_finaleff_fullSel;
float ZZ_finaleff_fullSel;
float WZ_finaleff_fullSel;
float Wgamma_finaleff_fullSel;
float QCDem_finaleff_fullSel;
float QCDbc_finaleff_fullSel;
float Photj_finaleff_fullSel;
float QCDmu_finaleff_fullSel;
float ttbar_finaleff_fullSel;

float H_finaleff;    
float Wj_finaleff;
float ttj_finaleff;
float SingleTop_finaleff;
float Zj_finaleff;
float WW_finaleff;
float ZZ_finaleff;
float WZ_finaleff;
float Wgamma_finaleff;
float QCDem_finaleff;
float QCDbc_finaleff;
float Photj_finaleff;
float QCDmu_finaleff;
float ttbar_finaleff;

float H_final[4][3];
float Wj_final[4][3];
float ttj_final[4][3];
float SingleTop_final[4][3];
float Zj_final[4][3];
float WW_final[4][3];
float ZZ_final[4][3];
float WZ_final[4][3];
float Wgamma_final[4][3];
float bkg_final[4][3];
float H_final_err[4][3];
float Wj_final_err[4][3];
float ttj_final_err[4][3];
float SingleTop_final_err[4][3];
float Zj_final_err[4][3];
float WW_final_err[4][3];
float ZZ_final_err[4][3];
float WZ_final_err[4][3];
float Wgamma_final_err[4][3];
float bkg_final_err[4][3];
float data_final[4][3];

// xsections
std::map<int,float> Higgs_xsec_masses, vbfHiggs_xsec_masses;

float Wgamma_xsec      = 165.;      // Madgraph VPhoton+jets
float Wlnu_xsec        = 31314.;     // madgraph 
//float Wlnu_xsec        = 31314. * 0.00111; // PYTHIA + filter on second jet x filter efficiency 
//float Wlnu_xsec        = 31314./3. * 0.742;  // pythia
float ZjetsLoMass_xsec = 3457.;            // pythia <20
float ZjetsHiMass_xsec = 4998./3.;         // pythia >20
float TTjets_xsec      = 157.5;
float ggWW_xsec        = 0.1538; // gg->WW->4l
float WW_xsec          = 4.513968; // WW_2l2nu PYTHIA
float WZ_xsec          = 0.599442; // WZ_3l
//float ZZ_xsec = 0.25252; // ZZ_2l2nu
float ZZ_xsec = 7.41; // sigma_ZZ = 6.77 (1+0.12/1.277)=7.41pb, where 1.277 is the LO->NLO k-factor.

float TTbar_xsec = 157.5;

// here xsec = x-sec * filter_eff (pb)
float QCD_EMenriched_Pt20to30_xsec = 235.5E+06 * 0.0073;
float QCD_EMenriched_Pt30to80_xsec = 59.3E+06 * 0.059;
float QCD_EMenriched_Pt80to170_xsec = 0.906E+06 * 0.148;
float QCD_BCtoE_Pt20to30_xsec = 235.5E+06 * 0.00046;
float QCD_BCtoE_Pt30to80_xsec = 59.3E+06 * 0.00234;
float QCD_BCtoE_Pt80to170_xsec = 0.906E+06 * 0.0104;

// xsec (pb)
float SingleTopS_xsec = 4.21 * (0.1080*3);
float SingleTopT_xsec = 64.6 * (0.1080*3);
float SingleTopTW_xsec = 10.6;

// xsec (pb)
float PhotonJet_Pt0to15_xsec = 84.46E+06;
float PhotonJet_Pt15to20_xsec = 114700;
float PhotonJet_Pt20to30_xsec = 57180;
float PhotonJet_Pt30to50_xsec = 16520;
float PhotonJet_Pt50to80_xsec = 2723;
float PhotonJet_Pt80to120_xsec = 446.2;
float PhotonJet_Pt120to170_xsec = 84.43;
float PhotonJet_Pt170to300_xsec = 22.55;
float PhotonJet_Pt300to500_xsec = 1.545;
float PhotonJet_Pt500toInf_xsec = 0.0923;

float InclusiveMu15_xsec = 48.44*0.00176*1.0E+09; // ppMuX

string sampleNames[23];

void computeYields(float lumi, const char* finalstate, int mass=0) {

  TChain *chains_fullSel[23];

  for(int isample=0; isample<23; isample++) {
    char fullsel_treename[200];
    sprintf(fullsel_treename,"FULL_SELECTION_EVENT_COUNTER_%s",finalstate);
    chains_fullSel[isample] = new TChain(fullsel_treename);
  }

  // data
  sampleNames[0] = "data";
  // backgrounds
  sampleNames[1]  = "TTbar+jets";
  sampleNames[2]  = "Z(ee) 10-20 GeV";
  sampleNames[3]  = "Z(mumu) 10-20 GeV";
  sampleNames[4]  = "Z(tautau) 10-20 GeV ";
  sampleNames[5]  = "Z(ee) >20 GeV";
  sampleNames[6]  = "Z(mumu) >20 GeV";
  sampleNames[7]  = "Z(tautau) >20 GeV ";
  sampleNames[8]  = "WW";
  sampleNames[9]  = "gg->WW";
  sampleNames[10] = "ZZ";
  sampleNames[11] = "WZ";
  sampleNames[12] = "Wgamma";
  sampleNames[13] = "SingleTop_sChannel";
  sampleNames[14] = "SingleTop_tChannel";
  sampleNames[15] = "SingleTop_tWChannel";
  sampleNames[16] = "WToENu";
  sampleNames[17] = "WToMuNu";
  sampleNames[18] = "WToTauNu";
  // signal
  sampleNames[19]  = "ggH->WW->2L2Nu";
  sampleNames[20]  = "ggH->WW->TauNuLNu";
  sampleNames[21]  = "qqH->WW->2L2Nu";
  sampleNames[22]  = "qqH->WW->TauNuLNu";

  float Higgs_xsec_2l2nu, Higgs_xsec_taunulnu;
  Higgs_xsec_masses.insert(std::make_pair(120,0.249642));
  Higgs_xsec_masses.insert(std::make_pair(130,0.452090));
  Higgs_xsec_masses.insert(std::make_pair(140,0.641773));
  Higgs_xsec_masses.insert(std::make_pair(150,0.770471));
  Higgs_xsec_masses.insert(std::make_pair(160,0.866443));
  Higgs_xsec_masses.insert(std::make_pair(170,0.782962));
  Higgs_xsec_masses.insert(std::make_pair(180,0.659328));
  Higgs_xsec_masses.insert(std::make_pair(190,0.486486));
  Higgs_xsec_masses.insert(std::make_pair(200,0.408305));
  Higgs_xsec_masses.insert(std::make_pair(210,0.358465));
  Higgs_xsec_masses.insert(std::make_pair(220,0.321398));
  Higgs_xsec_masses.insert(std::make_pair(230,0.290454));
  Higgs_xsec_masses.insert(std::make_pair(250,0.243724));
  Higgs_xsec_masses.insert(std::make_pair(300,0.175652));
  Higgs_xsec_masses.insert(std::make_pair(350,0.160052));
  Higgs_xsec_masses.insert(std::make_pair(400,0.124330));
  Higgs_xsec_masses.insert(std::make_pair(450,0.078433));
  Higgs_xsec_masses.insert(std::make_pair(500,0.048702));
  Higgs_xsec_masses.insert(std::make_pair(550,0.030364));
  Higgs_xsec_masses.insert(std::make_pair(600,0.019184));

  float vbfHiggs_xsec_2l2nu, vbfHiggs_xsec_taunulnu;
  vbfHiggs_xsec_masses.insert(std::make_pair(120,0.019057));
  vbfHiggs_xsec_masses.insert(std::make_pair(130,0.036942));
  vbfHiggs_xsec_masses.insert(std::make_pair(140,0.055686));
  vbfHiggs_xsec_masses.insert(std::make_pair(150,0.070561));
  vbfHiggs_xsec_masses.insert(std::make_pair(160,0.083858));
  vbfHiggs_xsec_masses.insert(std::make_pair(170,0.082794));
  vbfHiggs_xsec_masses.insert(std::make_pair(180,0.073183));
  vbfHiggs_xsec_masses.insert(std::make_pair(190,0.057139));
  vbfHiggs_xsec_masses.insert(std::make_pair(200,0.049566));
  vbfHiggs_xsec_masses.insert(std::make_pair(210,0.044544));
  vbfHiggs_xsec_masses.insert(std::make_pair(220,0.040624));
  vbfHiggs_xsec_masses.insert(std::make_pair(230,0.037243));
  vbfHiggs_xsec_masses.insert(std::make_pair(250,0.031672));
  vbfHiggs_xsec_masses.insert(std::make_pair(300,0.021873));
  vbfHiggs_xsec_masses.insert(std::make_pair(350,0.015113));
  vbfHiggs_xsec_masses.insert(std::make_pair(400,0.009885));
  vbfHiggs_xsec_masses.insert(std::make_pair(450,0.007143));
  vbfHiggs_xsec_masses.insert(std::make_pair(500,0.005439));
  vbfHiggs_xsec_masses.insert(std::make_pair(550,0.004249));
  vbfHiggs_xsec_masses.insert(std::make_pair(600,0.003374));

  if(mass==0) { // use the default mass to print the cut-by cut table: the one pointed by results/ dir

    Higgs_xsec_2l2nu = Higgs_xsec_masses[exampleHiggsMass] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    Higgs_xsec_taunulnu = Higgs_xsec_masses[exampleHiggsMass] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3
    vbfHiggs_xsec_2l2nu = vbfHiggs_xsec_masses[exampleHiggsMass] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    vbfHiggs_xsec_taunulnu = vbfHiggs_xsec_masses[exampleHiggsMass] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3

    // mc
    char dir_mc[1000];                  
    sprintf(dir_mc,"/cmsrm/pc21_2/emanuele/data/Higgs4.2.X/MC2011_Merged_V3/OptimMH%d/Spring11_V5HWW/",exampleHiggsMass);

    char HiggsSample_2L2Nu[500];
    sprintf(HiggsSample_2L2Nu,"GluGluToHToWWTo2L2Nu_M-%d_7TeV-powheg-pythia6/*Counters.root",exampleHiggsMass);

    char HiggsSample_TauNuLNu[500];
    sprintf(HiggsSample_TauNuLNu,"GluGluToHToWWToLNuTauNu_M-%d_7TeV-powheg-pythia6/*Counters.root",exampleHiggsMass);

    char vbfHiggsSample_2L2Nu[500];
    sprintf(vbfHiggsSample_2L2Nu,"VBF_HToWWTo2L2Nu_M-%d_7TeV-powheg-pythia6/*Counters.root",exampleHiggsMass);

    char vbfHiggsSample_TauNuLNu[500];
    sprintf(vbfHiggsSample_TauNuLNu,"VBF_HToWWToLNuTauNu_M-%d_7TeV-powheg-pythia6/*Counters.root",exampleHiggsMass);

    // data
    char dir_data[1000]; 
    sprintf(dir_data,"/cmsrm/pc21_2/emanuele/data/Higgs4.2.X/Data2011_Merged_V2xxxx/OptimMH%d/Data7TeVHWW",exampleHiggsMass);
    cout << "dir_data = " << dir_data << endl;

    // backgrounds
    chains_fullSel[1]->Add(TString(dir_mc)+TString("/TTJets_TuneZ2_7TeV-madgraph-tauola/*Counters.root"));       
    chains_fullSel[2]->Add(TString(dir_mc)+TString("/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[3]->Add(TString(dir_mc)+TString("/DYToMuMu_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[4]->Add(TString(dir_mc)+TString("/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));       
    chains_fullSel[5]->Add(TString(dir_mc)+TString("/DYToEE_M-20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[6]->Add(TString(dir_mc)+TString("/DYToMuMu_M-20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[7]->Add(TString(dir_mc)+TString("/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));       
    // chains_fullSel[8]->Add(TString(dir_mc)+TString("/WWTo2L2Nu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[8]->Add(TString(dir_mc)+TString("/VVJetsTo4L_TuneD6T_7TeV-madgraph-tauola-WWFilter/*Counters.root"));
    chains_fullSel[9]->Add(TString(dir_mc)+TString("/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*Counters.root"));
    chains_fullSel[10]->Add(TString(dir_mc)+TString("/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));
    chains_fullSel[11]->Add(TString(dir_mc)+TString("/WZTo3LNu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[12]->Add(TString(dir_mc)+TString("/PhotonVJets_7TeV-madgraph/*Counters.root"));
    chains_fullSel[13]->Add(TString(dir_mc)+TString("TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[14]->Add(TString(dir_mc)+TString("TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[15]->Add(TString(dir_mc)+TString("TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[16]->Add(TString(dir_mc)+TString("WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*Counters.root"));
    //    chains_fullSel[16]->Add(TString(dir_mc)+TString("WToENu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[16]->Add(TString(dir_mc)+TString("WToLNu_TuneZ2_7TeV_pythia6_veryBIG/*Counters.root"));
    chains_fullSel[17]->Add(TString(dir_mc)+TString("WToMuNu_TuneZ2_7TeV-pythia6/*Counters.rootXXX")); // not used when using madgraph inclusive W+jets
    chains_fullSel[18]->Add(TString(dir_mc)+TString("WToTauNu_TuneZ2_7TeV-pythia6/*Counters.rootXXX")); // "  "
    chains_fullSel[19]->Add(TString(dir_mc)+TString(HiggsSample_2L2Nu));       
    chains_fullSel[20]->Add(TString(dir_mc)+TString(HiggsSample_TauNuLNu));       
    chains_fullSel[21]->Add(TString(dir_mc)+TString(vbfHiggsSample_2L2Nu));    
    chains_fullSel[22]->Add(TString(dir_mc)+TString(vbfHiggsSample_TauNuLNu));    
    
    // data
    if (strcmp(finalstate,"EE")==0) chains_fullSel[0]->Add(TString(dir_data)+TString("/DoubleElectron/*Counters.root"));
    if (strcmp(finalstate,"MM")==0) {
      chains_fullSel[0]->Add(TString(dir_data)+TString("/DoubleMu/*Counters.root"));
      chains_fullSel[0]->Add(TString(dir_data)+TString("/SingleMu/*Counters.root"));
    }
    if (strcmp(finalstate,"EM")==0 || strcmp(finalstate,"ME")==0) {
      chains_fullSel[0]->Add(TString(dir_data)+TString("/MuEG/*Counters.root"));
      chains_fullSel[0]->Add(TString(dir_data)+TString("/SingleMu/*Counters.root"));
    }
  } else {

    Higgs_xsec_2l2nu = Higgs_xsec_masses[mass] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    Higgs_xsec_taunulnu = Higgs_xsec_masses[exampleHiggsMass] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3
    vbfHiggs_xsec_2l2nu = vbfHiggs_xsec_masses[exampleHiggsMass] * 4./9.; // 4/9 because we are considering only the samples containing e-mu combinations.
    vbfHiggs_xsec_taunulnu = vbfHiggs_xsec_masses[exampleHiggsMass] * 4./9.; // i.e. 2*BR(W->tau nu) * BR(W->e/mu nu) = 2 * 1/3 * 2/3

    // mc
    char dir[1000];                  
    sprintf(dir,"/cmsrm/pc21_2/emanuele/data/Higgs4.2.X/MC2011_Merged_V3/OptimMH%d/Spring11_V5HWW/",mass);

    char HiggsSample_2L2Nu[500];
    sprintf(HiggsSample_2L2Nu,"GluGluToHToWWTo2L2Nu_M-%d_7TeV-powheg-pythia6/*Counters.root",mass);

    char HiggsSample_TauNuLNu[500];
    sprintf(HiggsSample_TauNuLNu,"GluGluToHToWWToLNuTauNu_M-%d_7TeV-powheg-pythia6/*Counters.root",mass);

    char vbfHiggsSample_2L2Nu[500];
    sprintf(vbfHiggsSample_2L2Nu,"VBF_HToWWTo2L2Nu_M-%d_7TeV-powheg-pythia6/*Counters.root",mass);

    char vbfHiggsSample_TauNuLNu[500];
    sprintf(vbfHiggsSample_TauNuLNu,"VBF_HToWWToLNuTauNu_M-%d_7TeV-powheg-pythia6/*Counters.root",mass);

    // data
    char dir_data[1000]; 
    sprintf(dir_data,"/cmsrm/pc21_2/emanuele/data/Higgs4.2.X/Data2011_Merged_V2xxxxxx/OptimMH%d/Data7TeVHWW/",mass);

    // backgrounds
    chains_fullSel[1]->Add(TString(dir)+TString("/TTJets_TuneZ2_7TeV-madgraph-tauola/*Counters.root"));       
    chains_fullSel[2]->Add(TString(dir)+TString("/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[3]->Add(TString(dir)+TString("/DYToMuMu_M-10To20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[4]->Add(TString(dir)+TString("/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));       
    chains_fullSel[5]->Add(TString(dir)+TString("/DYToEE_M-20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[6]->Add(TString(dir)+TString("/DYToMuMu_M-20_TuneZ2_7TeV-pythia6/*Counters.root"));       
    chains_fullSel[7]->Add(TString(dir)+TString("/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));       
    // chains_fullSel[8]->Add(TString(dir)+TString("/WWTo2L2Nu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[8]->Add(TString(dir)+TString("/VVJetsTo4L_TuneD6T_7TeV-madgraph-tauola-WWFilter/*Counters.root"));
    chains_fullSel[9]->Add(TString(dir)+TString("/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*Counters.root"));
    chains_fullSel[10]->Add(TString(dir)+TString("/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/*Counters.root"));
    chains_fullSel[11]->Add(TString(dir)+TString("/WZTo3LNu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[12]->Add(TString(dir)+TString("/PhotonVJets_7TeV-madgraph/*Counters.root"));
    chains_fullSel[13]->Add(TString(dir)+TString("TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[14]->Add(TString(dir)+TString("TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[15]->Add(TString(dir)+TString("TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*Counters.root"));
    chains_fullSel[16]->Add(TString(dir)+TString("WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*Counters.root"));
    //    chains_fullSel[16]->Add(TString(dir)+TString("WToENu_TuneZ2_7TeV-pythia6/*Counters.root"));
    chains_fullSel[16]->Add(TString(dir)+TString("WToLNu_TuneZ2_7TeV_pythia6_veryBIG/*Counters.root"));
    chains_fullSel[17]->Add(TString(dir)+TString("WToMuNu_TuneZ2_7TeV-pythia6/*Counters.rootXXX")); // not used when using madgraph inclusive W+jets
    chains_fullSel[18]->Add(TString(dir)+TString("WToTauNu_TuneZ2_7TeV-pythia6/*Counters.rootXXX")); // "  "
    // signal
    chains_fullSel[19]->Add(TString(dir)+TString(HiggsSample_2L2Nu));
    chains_fullSel[20]->Add(TString(dir)+TString(HiggsSample_TauNuLNu));
    chains_fullSel[21]->Add(TString(dir)+TString(vbfHiggsSample_2L2Nu));
    chains_fullSel[22]->Add(TString(dir)+TString(vbfHiggsSample_TauNuLNu));

    // data
    if (strcmp(finalstate,"EE")==0) chains_fullSel[0]->Add(TString(dir_data)+TString("/DoubleElectron/*Counters.root"));
    if (strcmp(finalstate,"MM")==0) {
      chains_fullSel[0]->Add(TString(dir_data)+TString("/DoubleMu/*Counters.root"));
      chains_fullSel[0]->Add(TString(dir_data)+TString("/SingleMu/*Counters.root"));
    }
    if (strcmp(finalstate,"EM") || strcmp(finalstate,"ME")==0) {
      chains_fullSel[0]->Add(TString(dir_data)+TString("/MuEG/*Counters.root"));
      chains_fullSel[0]->Add(TString(dir_data)+TString("/SingleMu/*Counters.root"));
    }
  }

  float nFullSelTot[26][23];

  for(int isample=0; isample<23; isample++) {
    for(int icut=0; icut<26; icut++) { nFullSelTot[icut][isample] = 0.0; }
  }
  
  // full selection
  int nCutsAnaFull = 26;
  for(int isample=0; isample<23; isample++) {

    // List of branches    
    Int_t           nCutsFull;
    Float_t         nSelFull[26];   //[nCuts]
    TBranch        *b_nCutsFull;   //!
    TBranch        *b_nSelFull;    //!
    chains_fullSel[isample]->SetBranchAddress("nCuts", &nCutsFull, &b_nCutsFull);
    chains_fullSel[isample]->SetBranchAddress("nSel",  nSelFull,   &b_nSelFull);
    
    Long64_t nentriesFull = chains_fullSel[isample]->GetEntries();

    // full selection
    for (Long64_t jentry=0; jentry<nentriesFull;jentry++) {
      Long64_t nb2;
      nb2 = chains_fullSel[isample]->GetEntry(jentry);   
      for(int icut=0; icut<nCutsFull; icut++) nFullSelTot[icut][isample] += nSelFull[icut];
    }
  }

  // eff at full selection level
  for(int icut=0; icut<nCutsAnaFull; icut++) {

    float Higgs_tmp=0.;
    if(nFullSelTot[0][19]) Higgs_tmp += lumi * Higgs_xsec_2l2nu  * nFullSelTot[icut][19]/nFullSelTot[0][19];
    if(nFullSelTot[0][20]) Higgs_tmp += lumi * Higgs_xsec_taunulnu * nFullSelTot[icut][20]/nFullSelTot[0][20];
    if(nFullSelTot[0][21]) Higgs_tmp += lumi * vbfHiggs_xsec_2l2nu  * nFullSelTot[icut][21]/nFullSelTot[0][21];
    if(nFullSelTot[0][22]) Higgs_tmp += lumi * vbfHiggs_xsec_taunulnu * nFullSelTot[icut][22]/nFullSelTot[0][22];
    H_fullSel[icut] = Higgs_tmp;
    H_fullSel_err[icut] = yieldErrPoisson(lumi * Higgs_xsec_2l2nu  * nFullSelTot[icut][19]/nFullSelTot[0][19], nFullSelTot[icut][19],
                                          lumi * Higgs_xsec_taunulnu * nFullSelTot[icut][20]/nFullSelTot[0][20], nFullSelTot[icut][20],
                                          lumi * vbfHiggs_xsec_2l2nu  * nFullSelTot[icut][21]/nFullSelTot[0][21], nFullSelTot[icut][21],
                                          lumi * vbfHiggs_xsec_taunulnu * nFullSelTot[icut][22]/nFullSelTot[0][22], nFullSelTot[icut][22]);

    ttj_fullSel[icut] = lumi * TTjets_xsec * nFullSelTot[icut][1]/nFullSelTot[0][1];
    ttj_fullSel_err[icut] = yieldErrPoisson(lumi * TTjets_xsec * nFullSelTot[icut][1]/nFullSelTot[0][1], nFullSelTot[icut][1]);

    float Z_tmp=0.;
    Z_tmp += lumi * ZjetsLoMass_xsec  * nFullSelTot[icut][2]/nFullSelTot[0][2];
    Z_tmp += lumi * ZjetsLoMass_xsec  * nFullSelTot[icut][3]/nFullSelTot[0][3];
    // Z_tmp += lumi * ZjetsLoMass_xsec  * nFullSelTot[icut][4]/nFullSelTot[0][4];
    Z_tmp += lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][5]/nFullSelTot[0][5];
    Z_tmp += lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][6]/nFullSelTot[0][6];
    Z_tmp += lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][7]/nFullSelTot[0][7];
    Zj_fullSel[icut]  = Z_tmp;
    Zj_fullSel_err[icut] = yieldErrPoisson(lumi * ZjetsLoMass_xsec  * nFullSelTot[icut][2]/nFullSelTot[0][2], nFullSelTot[icut][2],
                                           lumi * ZjetsLoMass_xsec  * nFullSelTot[icut][3]/nFullSelTot[0][3], nFullSelTot[icut][3],
                                           lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][5]/nFullSelTot[0][5], nFullSelTot[icut][5],
                                           lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][6]/nFullSelTot[0][6], nFullSelTot[icut][6],
                                           lumi * ZjetsHiMass_xsec  * nFullSelTot[icut][7]/nFullSelTot[0][7], nFullSelTot[icut][7]);

    float WW_tmp=0.;
    WW_tmp += lumi * WW_xsec   * nFullSelTot[icut][8]/nFullSelTot[0][8];
    WW_tmp += lumi * ggWW_xsec * nFullSelTot[icut][9]/nFullSelTot[0][9];
    WW_fullSel[icut] = WW_tmp;
    WW_fullSel_err[icut] = yieldErrPoisson(lumi * WW_xsec   * nFullSelTot[icut][8]/nFullSelTot[0][8], nFullSelTot[icut][8],
                                           lumi * ggWW_xsec * nFullSelTot[icut][9]/nFullSelTot[0][9], nFullSelTot[icut][9]);

    ZZ_fullSel[icut]     = lumi * ZZ_xsec     * nFullSelTot[icut][10]/nFullSelTot[0][10];
    ZZ_fullSel_err[icut] = yieldErrPoisson(lumi * ZZ_xsec     * nFullSelTot[icut][10]/nFullSelTot[0][10], nFullSelTot[icut][10]);
    WZ_fullSel[icut]     = lumi * WZ_xsec     * nFullSelTot[icut][11]/nFullSelTot[0][11];
    WZ_fullSel_err[icut] = yieldErrPoisson(lumi * WZ_xsec     * nFullSelTot[icut][11]/nFullSelTot[0][11],  nFullSelTot[icut][11]);
    Wgamma_fullSel[icut] = lumi * Wgamma_xsec * nFullSelTot[icut][12]/nFullSelTot[0][12];
    Wgamma_fullSel_err[icut] = yieldErrPoisson(lumi * Wgamma_xsec * nFullSelTot[icut][12]/nFullSelTot[0][12], nFullSelTot[icut][12]);

    float singletop_tmp=0.;
    singletop_tmp += lumi * SingleTopS_xsec  * nFullSelTot[icut][13]/nFullSelTot[0][13];
    singletop_tmp += lumi * SingleTopT_xsec  * nFullSelTot[icut][14]/nFullSelTot[0][14];
    singletop_tmp += lumi * SingleTopTW_xsec * nFullSelTot[icut][15]/nFullSelTot[0][15];
    SingleTop_fullSel[icut] = singletop_tmp;
    SingleTop_fullSel_err[icut] = yieldErrPoisson(lumi * SingleTopS_xsec  * nFullSelTot[icut][13]/nFullSelTot[0][13], nFullSelTot[icut][13],
                                                  lumi * SingleTopT_xsec  * nFullSelTot[icut][14]/nFullSelTot[0][14], nFullSelTot[icut][14],
                                                  lumi * SingleTopTW_xsec * nFullSelTot[icut][15]/nFullSelTot[0][15],  nFullSelTot[icut][15]);

    Wj_fullSel[icut]  = lumi * Wlnu_xsec * nFullSelTot[icut][16]/nFullSelTot[0][16]; 
    Wj_fullSel_err[icut]  = yieldErrPoisson(lumi * Wlnu_xsec * nFullSelTot[icut][16]/nFullSelTot[0][16], nFullSelTot[icut][16]);
    // float W_tmp=0.;
    // W_tmp += lumi * Wlnu_xsec  * nFullSelTot[icut][16]/nFullSelTot[0][16]; // W->enu                                
    // W_tmp += lumi * Wlnu_xsec  * nFullSelTot[icut][17]/nFullSelTot[0][17]; // W->munu                               
    // W_tmp += lumi * Wlnu_xsec  * nFullSelTot[icut][18]/nFullSelTot[0][18]; // W->taunu                              
    // Wj_fullSel[icut]  = W_tmp;

    // data
    data_fullSel[icut] = nFullSelTot[icut][0];

    // efficiencies
    if(icut>0 && nFullSelTot[icut-1][0]>0)  H_eff_fullSel[icut]      = H_fullSel[icut]/H_fullSel[icut-1];
    else H_eff_fullSel[icut] = 0.0;

    if(icut>0 && nFullSelTot[icut-1][1]>0)  ttj_eff_fullSel[icut]    = nFullSelTot[icut][1]/nFullSelTot[icut-1][1];
    else ttj_eff_fullSel[icut] = 0.0;

    if(icut>0 && Zj_fullSel[icut-1]>0)      Zj_eff_fullSel[icut]     = Zj_fullSel[icut]/Zj_fullSel[icut-1];
    else Zj_eff_fullSel[icut] = 0.0;

    if(icut>0 && WW_fullSel[icut-1]>0)      WW_eff_fullSel[icut]     = WW_fullSel[icut]/WW_fullSel[icut-1];
    else WW_eff_fullSel[icut] = 0.0;

    if(icut>0 && nFullSelTot[icut-1][10]>0) ZZ_eff_fullSel[icut]     = nFullSelTot[icut][10]/nFullSelTot[icut-1][10];
    else ZZ_eff_fullSel[icut] = 0.0;

    if(icut>0 && nFullSelTot[icut-1][11]>0) WZ_eff_fullSel[icut]     = nFullSelTot[icut][11]/nFullSelTot[icut-1][11];
    else WZ_eff_fullSel[icut] = 0.0;

    if(icut>0 && nFullSelTot[icut-1][12]>0) Wgamma_eff_fullSel[icut] = nFullSelTot[icut][12]/nFullSelTot[icut-1][12];
    else Wgamma_eff_fullSel[icut] = 0.0;

    if(icut>0 && SingleTop_fullSel[icut-1]>0) SingleTop_eff_fullSel[icut] = SingleTop_fullSel[icut]/SingleTop_fullSel[icut-1];
    else SingleTop_eff_fullSel[icut] = 0.0;

    if(icut>0 && Wj_fullSel[icut-1]>0)       Wj_eff_fullSel[icut]     = Wj_fullSel[icut]/Wj_fullSel[icut-1];
    else Wj_eff_fullSel[icut] = 0.0;

    
    if(icut==0) { 
      H_eff_fullSel[icut]         = H_fullSel[icut]/H_fullSel[0];
      ttj_eff_fullSel[icut]       = nFullSelTot[icut][1]/nFullSelTot[0][1];
      Zj_eff_fullSel[icut]        = Zj_fullSel[icut]/Zj_fullSel[0];
      WW_eff_fullSel[icut]        = WW_fullSel[icut]/WW_fullSel[0];
      ZZ_eff_fullSel[icut]        = nFullSelTot[icut][10]/nFullSelTot[0][10];
      WZ_eff_fullSel[icut]        = nFullSelTot[icut][11]/nFullSelTot[0][11];
      Wgamma_eff_fullSel[icut]    = nFullSelTot[icut][12]/nFullSelTot[0][12];
      SingleTop_eff_fullSel[icut] = SingleTop_fullSel[icut]/SingleTop_fullSel[0];
      Wj_eff_fullSel[icut]        = Wj_fullSel[icut]/Wj_fullSel[0];
    }
  }
  
  // final efficiency after full selections (-4 = 3 x jets + 1=final)
  if(nFullSelTot[0][0]>0)  H_finaleff_fullSel  = H_fullSel[nCutsAnaFull-4]/H_fullSel[0];
  else H_finaleff_fullSel = 0.0;
  if(nFullSelTot[0][1]>0) ttj_finaleff_fullSel = nFullSelTot[nCutsAnaFull-4][1]/nFullSelTot[0][1];
  else ttj_finaleff_fullSel = 0.0;
  if(Zj_fullSel[0]>0) Zj_finaleff_fullSel      = Zj_fullSel[nCutsAnaFull-4]/Zj_fullSel[0];
  else Zj_finaleff_fullSel = 0.0;
  if(WW_fullSel[0]>0) WW_finaleff_fullSel     = WW_fullSel[nCutsAnaFull-4]/WW_fullSel[0];
  else WW_finaleff_fullSel = 0.0;
  if(nFullSelTot[0][10]>0) ZZ_finaleff_fullSel     = nFullSelTot[nCutsAnaFull-4][10]/nFullSelTot[0][10];
  else ZZ_finaleff_fullSel = 0.0;
  if(nFullSelTot[0][11]>0) WZ_finaleff_fullSel     = nFullSelTot[nCutsAnaFull-4][11]/nFullSelTot[0][11];
  else WZ_finaleff_fullSel = 0.0;
  if(nFullSelTot[0][12]>0) Wgamma_finaleff_fullSel = nFullSelTot[nCutsAnaFull-4][12]/nFullSelTot[0][12];
  else Wgamma_finaleff_fullSel = 0.0;
  if(SingleTop_fullSel[0]>0) SingleTop_finaleff_fullSel = SingleTop_fullSel[nCutsAnaFull-4]/SingleTop_fullSel[0];
  else SingleTop_finaleff_fullSel = 0.0;
  if(Wj_fullSel[0]>0) Wj_finaleff_fullSel      = Wj_fullSel[nCutsAnaFull-4]/Wj_fullSel[0];
  else Wj_finaleff_fullSel = 0.0;
  
  // final efficiency combining pre and full selections
  if(nFullSelTot[0][0]>0) H_finaleff       = H_fullSel[nCutsAnaFull-4]/H_fullSel[0];
  else H_finaleff = 0.0;
  if(nFullSelTot[0][1]>0) ttj_finaleff     = nFullSelTot[nCutsAnaFull-4][1]/nFullSelTot[0][1];
  else ttj_finaleff = 0.0;
  if(Zj_fullSel[0]>0)     Zj_finaleff      = Zj_fullSel[nCutsAnaFull-4]/Zj_fullSel[0];
  else Zj_finaleff = 0.0;
  if(WW_fullSel[0]>0)     WW_finaleff      = WW_fullSel[nCutsAnaFull-4]/WW_fullSel[0];
  else WW_finaleff = 0.0;
  if(nFullSelTot[0][10]>0) ZZ_finaleff     = nFullSelTot[nCutsAnaFull-4][10]/nFullSelTot[0][10];
  else ZZ_finaleff = 0.0;
  if(nFullSelTot[0][11]>0) WZ_finaleff     = nFullSelTot[nCutsAnaFull-4][11]/nFullSelTot[0][11];
  else WZ_finaleff = 0.0;
  if(nFullSelTot[0][12]>0) Wgamma_finaleff = nFullSelTot[nCutsAnaFull-4][12]/nFullSelTot[0][12];
  else Wgamma_finaleff = 0.0;
  if(SingleTop_fullSel[0]>0) SingleTop_finaleff = SingleTop_fullSel[nCutsAnaFull-4]/SingleTop_fullSel[0];
  else SingleTop_finaleff = 0.0;
  if(Wj_fullSel[0]>0)     Wj_finaleff      = Wj_fullSel[nCutsAnaFull-4]/Wj_fullSel[0];
  else Wj_finaleff = 0.0;

  cout << "\n\nPROCESSED EVENTS:" << endl;
  for(int i=0; i<23; i++) {
    cout << sampleNames[i] << "\t" << nFullSelTot[0][i] << endl;
  }

}

void setupCuts() {
  
  for(int ichan=0; ichan<4; ichan++) {
    for(int i=0; i<25; i++) {
      UseCuts[ichan][i] = 1;
    }
    UseCuts[ichan][22] = 0;
    UseCuts[ichan][23] = 0;
    UseCuts[ichan][24] = 0;
  }


  fullSelCuts[0]="event";
  fullSelCuts[1]="MCtruth";
  fullSelCuts[2]="trigger";
  fullSelCuts[3]="channel preSel.";
  fullSelCuts[4]="e/$\\mu$ ID";
  fullSelCuts[5]="e/$\\mu$ isolation";
  fullSelCuts[6]="conv. rej.";
  fullSelCuts[7]="e/$\\mu$ d0";
  fullSelCuts[8]="$MET>20$ GeV";
  fullSelCuts[9]="$m_{ll}$";
  fullSelCuts[10]="$|m_{ll}-m_Z|>15$ GeV";
  fullSelCuts[11]="proj. MET";
  fullSelCuts[12]="MET/$p_T^{ll}$";
  fullSelCuts[13]="jet veto";
  fullSelCuts[14]="$\\mu^{soft}$ veto";
  fullSelCuts[15]="extra lepton veto";
  fullSelCuts[16]="anti b-tag";
  fullSelCuts[17]="$m_{ll}2$";
  fullSelCuts[18]="sel $p_T^{max}$";
  fullSelCuts[19]="sel $p_T^{min}$";
  fullSelCuts[20]="$\\Delta \\phi$";
  fullSelCuts[21]="$M_T$";
  fullSelCuts[22]="$M_T$ (1jet)";
  fullSelCuts[23]="final";
  fullSelCuts[24]="1 jets";
  fullSelCuts[25]="$>1$ jets";
}


void printLatex(float lumi, const char* finalstate) {

  setupCuts();

  if(strcmp(finalstate,"EE") && strcmp(finalstate,"EM") && strcmp(finalstate,"MM") && strcmp(finalstate,"ME")) {
    cout << "ERROR! finalstate must be one among EE/EM/MM/ME. Exiting..." << endl;
    return;
  } else {
    cout << " === NOW COMPUTING YIELDS FOR FINAL STATE: " << finalstate << " ===" << endl;
  }
  
  computeYields(lumi,finalstate);
  
  if(printCutsBreakdown) {
  /// ==============  print detailed breakdown  ================== ///
  char namefile[200];
  sprintf(namefile,"yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(2);
  
  textfile << "\\begin{table}[p]" << endl
           << "\\begin{tiny}" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  //  textfile << "selection & data & H(WW) & W$(l\\nu)$+jets & $t\\bar{t}$ & single t & Z$(ll)$+jets & WW & ZZ & WZ & W$\\gamma$ \t\\\\" << endl;
  textfile << "selection & data & H(WW) & W$(l\\nu)$+jets & $t\\bar{t}$ & single t \t\\\\" << endl;
  textfile << "\\hline" << endl; 
    
  textfile << "\\hline"        << endl;
  textfile << "\\hline"        << endl;
    
  for(int icut=0; icut<26; icut++) {

    if(!strcmp(finalstate,"EE")) {
      if(!UseCuts[ee][icut]) continue;
    }
    if(!strcmp(finalstate,"MM")) {
      if(!UseCuts[mm][icut]) continue;
    }
    if(!strcmp(finalstate,"EM")) {
      if(!UseCuts[em][icut]) continue;
    }
    if(!strcmp(finalstate,"ME")) {
      if(!UseCuts[me][icut]) continue;
    }

    textfile << fullSelCuts[icut] << "\t&\t";
      
    textfile << fixed
             << data_fullSel[icut]   << "\t&\t"
             << H_fullSel[icut]      << " (" << 100. * H_eff_fullSel[icut]   << "\\%)" << "\t&\t"
             << Wj_fullSel[icut]     << " (" << 100. * Wj_eff_fullSel[icut]  << "\\%)" << "\t&\t"
             << ttj_fullSel[icut]    << " (" << 100. * ttj_eff_fullSel[icut] << "\\%)" << "\t&\t"
             << SingleTop_fullSel[icut]    << " (" << 100. * SingleTop_eff_fullSel[icut] << "\\%)" << "\t";
    textfile << "\t\\\\" << endl;
  }
    
  textfile << "\\hline" << endl;

  textfile << "total fullselection " << "\t&\t"
           << data_fullSel[23]   << "\t&\t"
           << H_fullSel[23]      << " (" << 100. * H_finaleff_fullSel  << "\\%)"     << "\t&\t"
           << Wj_fullSel[23]     << " (" << 100. * Wj_finaleff_fullSel << "\\%)"     << "\t&\t"
           << ttj_fullSel[23]    << " (" << 100. * ttj_finaleff_fullSel << "\\%)"    << "\t&\t"
           << SingleTop_fullSel[23]    << " (" << 100. * SingleTop_finaleff_fullSel << "\\%)"    << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "\\hline" << endl;
    
  textfile << "0 jets bin " << "\t&\t"
           << data_fullSel[23]   << "\t&\t"
           << H_fullSel[23]      << "\t&\t"
           << Wj_fullSel[23]     << "\t&\t"
           << ttj_fullSel[23]    << "\t&\t"
           << SingleTop_fullSel[23]    << "\t";
  textfile << "\t\\\\" << endl;

  textfile << "1 jets bin " << "\t&\t"
           << data_fullSel[24]   << "\t&\t"
           << H_fullSel[24]      << "\t&\t"
           << Wj_fullSel[24]     << "\t&\t"
           << ttj_fullSel[24]    << "\t&\t"
           << SingleTop_fullSel[24]    << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "$>1$ jets bin " << "\t&\t"
           << data_fullSel[25]   << "\t&\t"
           << H_fullSel[25]      << "\t&\t"
           << Wj_fullSel[25]     << "\t&\t"
           << ttj_fullSel[25]    << "\t&\t"
           << SingleTop_fullSel[25]    << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\end{tiny}" << endl
           << "\\caption{Higgs $m_H$ = " << exampleHiggsMass << " GeV/c$^2$. Breakdown of signal and backgrounds events in "
           << lumi << " $pb^{-1}$ for " << finalstate << " final state.} " << endl 
           << "\\end{table}" << endl;



  textfile << "\\begin{table}[p]" << endl
           << "\\begin{tiny}" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;
  textfile << "selection & Z$(ll)$+jets & WW & ZZ & WZ & W$\\gamma$ \t\\\\" << endl;
  textfile << "\\hline" << endl; 
    
  textfile << "\\hline"        << endl;
  textfile << "\\hline"        << endl;
    
  for(int icut=0; icut<26; icut++) {

    if(!strcmp(finalstate,"EE")) {
      if(!UseCuts[ee][icut]) continue;
    }
    if(!strcmp(finalstate,"MM")) {
      if(!UseCuts[mm][icut]) continue;
    }
    if(!strcmp(finalstate,"EM")) {
      if(!UseCuts[em][icut]) continue;
    }
    if(!strcmp(finalstate,"ME")) {
      if(!UseCuts[me][icut]) continue;
    }

    textfile << fullSelCuts[icut] << "\t&\t";
      
    textfile << fixed
             << Zj_fullSel[icut]     << " (" << 100. * Zj_eff_fullSel[icut]  << "\\%)" << "\t&\t"
             << WW_fullSel[icut]     << " (" << 100. * WW_eff_fullSel[icut]  << "\\%)" << "\t&\t"
             << ZZ_fullSel[icut]     << " (" << 100. * ZZ_eff_fullSel[icut]     << "\\%)" << "\t&\t"
             << WZ_fullSel[icut]     << " (" << 100. * WZ_eff_fullSel[icut]     << "\\%)" << "\t&\t"
             << Wgamma_fullSel[icut] << " (" << 100. * Wgamma_eff_fullSel[icut] << "\\%)" << "\t";
    textfile << "\t\\\\" << endl;
  }
    
  textfile << "\\hline" << endl;

  textfile << "total fullselection " << "\t&\t"
           << Zj_fullSel[23]     << " (" << 100. * Zj_finaleff_fullSel << "\\%)"     << "\t&\t"
           << WW_fullSel[23]     << " (" << 100. * WW_finaleff_fullSel << "\\%)"     << "\t&\t"
           << ZZ_fullSel[23]     << " (" << 100. * ZZ_finaleff_fullSel << "\\%)"     << "\t&\t"
           << WZ_fullSel[23]     << " (" << 100. * WZ_finaleff_fullSel << "\\%)"   << "\t&\t"
           << Wgamma_fullSel[23] << " (" << 100. * Wgamma_finaleff_fullSel << "\\%)" << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "\\hline" << endl;
    
  textfile << "0 jets bin " << "\t&\t"
           << Zj_fullSel[23]     << "\t&\t"
           << WW_fullSel[23]     << "\t&\t"
           << ZZ_fullSel[23]     << "\t&\t"
           << WZ_fullSel[23]     << "\t&\t"
           << Wgamma_fullSel[23] << "\t";
  textfile << "\t\\\\" << endl;

  textfile << "1 jets bin " << "\t&\t"
           << Zj_fullSel[24]     << "\t&\t"
           << WW_fullSel[24]     << "\t&\t"
           << ZZ_fullSel[24]   << "\t&\t"
           << WZ_fullSel[24]   << "\t&\t"
           << Wgamma_fullSel[24] << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "$>1$ jets bin " << "\t&\t"
           << Zj_fullSel[25]     << "\t&\t"
           << WW_fullSel[25]     << "\t&\t"
           << ZZ_fullSel[25]   << "\t&\t"
           << WZ_fullSel[25]   << "\t&\t"
           << Wgamma_fullSel[25] << "\t";
  textfile << "\t\\\\" << endl;
    
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\end{tiny}" << endl
           << "\\caption{Higgs $m_H$ = " << exampleHiggsMass << " GeV/c$^2$. Breakdown of signal and backgrounds events in "
           << lumi << " $pb^{-1}$ for " << finalstate << " final state.} " << endl 
           << "\\end{table}" << endl;

  textfile << "\\clearpage\n\n\n" << endl;

  }
  // assign the final yields
  int channel=-1;
  if(!strcmp(finalstate,"MM")) channel = mm;
  if(!strcmp(finalstate,"EM")) channel = em;
  if(!strcmp(finalstate,"EE")) channel = ee;
  if(!strcmp(finalstate,"ME")) channel = me;

  for(int jet=0; jet<3; ++jet) {
    
    int step;
    if(jet==0) step = 23;
    else if(jet==1) step = 24;
    else if(jet==2) step = 25;
    else {
      std::cout << "WRONG JET BIN. Stopping now." << std::endl;
    }

    data_final[channel][jet] = data_fullSel[step];
    H_final[channel][jet] = H_fullSel[step];
    Wj_final[channel][jet] = Wj_fullSel[step];
    ttj_final[channel][jet] = ttj_fullSel[step];
    SingleTop_final[channel][jet] = SingleTop_fullSel[step];
    Zj_final[channel][jet] = Zj_fullSel[step];
    WW_final[channel][jet] = WW_fullSel[step];
    ZZ_final[channel][jet] = ZZ_fullSel[step];
    WZ_final[channel][jet] = WZ_fullSel[step];
    Wgamma_final[channel][jet] = Wgamma_fullSel[step];
    bkg_final[channel][jet] = Wj_final[channel][jet] + ttj_final[channel][jet] + SingleTop_final[channel][jet]+
      Zj_final[channel][jet] + WW_final[channel][jet] + ZZ_final[channel][jet] + WZ_final[channel][jet] + 
      WZ_final[channel][jet] + WZ_final[channel][jet]; // Wgamma is in madgraph W+j

    H_final_err[channel][jet] = H_fullSel_err[step];
    Wj_final_err[channel][jet] = Wj_fullSel_err[step];
    ttj_final_err[channel][jet] = ttj_fullSel_err[step];
    SingleTop_final_err[channel][jet] = SingleTop_fullSel_err[step];
    Zj_final_err[channel][jet] = Zj_fullSel_err[step];
    WW_final_err[channel][jet] = WW_fullSel_err[step];
    ZZ_final_err[channel][jet] = ZZ_fullSel_err[step];
    WZ_final_err[channel][jet] = WZ_fullSel_err[step];
    Wgamma_final_err[channel][jet] = Wgamma_fullSel_err[step];
    bkg_final_err[channel][jet] = sqrt(pow(WW_final_err[channel][jet],2)+pow(WZ_final_err[channel][jet],2)+pow(ZZ_final_err[channel][jet],2)+
                                       pow(Zj_final_err[channel][jet],2)+pow(Wj_final_err[channel][jet],2)+pow(ttj_final_err[channel][jet],2)+pow(SingleTop_final_err[channel][jet],2));

  }

}

void printShortBkgSummary(float lumiEE, float lumiMM, float lumiEM) {

  /// ==============  print short summary  ================== ///

  char namefile[200];
  sprintf(namefile,"yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(2);

  std::string channelName[5][3];
  channelName[mm][0] = "$\\mu\\mu$ 0 j";
  channelName[ee][0] = "$ee$ 0 j";
  channelName[em][0] = "$e\\mu$ 0 j";
  channelName[me][0] = "$\\mu e$ 0 j";
  channelName[ll][0] = "$\\ell\\ell$ 0 j";

  channelName[mm][1] = "$\\mu\\mu$ 1 j";
  channelName[ee][1] = "$ee$ 1 j";
  channelName[em][1] = "$e\\mu$ 1 j";
  channelName[me][1] = "$\\mu e$ 1 j";
  channelName[ll][0] = "$\\ell\\ell$ 1 j";

  channelName[mm][2] = "$\\mu\\mu$ 2 j";
  channelName[ee][2] = "$ee$ 2 j";
  channelName[em][2] = "$e\\mu$ 2 j";
  channelName[me][2] = "$\\mu e$ 2 j";
  channelName[ll][2] = "$\\ell\\ell$ 0 j";

  for(int jet=0; jet<3; ++jet) {

  textfile << "\\begin{table}[p]" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline" << endl;

  textfile << "final state & H & WW & WZ & ZZ & Z+jets \t\\\\" << endl;
  textfile << "\\hline" << endl; 
  for(int ichan=0; ichan<4; ++ichan) {
    textfile << channelName[ichan][jet] << "\t&\t";
    textfile << fixed 
             << H_final[ichan][jet] << "$\\pm$" <<  H_final_err[ichan][jet] << "\t&\t"
             << WW_final[ichan][jet] << "$\\pm$" <<  WW_final_err[ichan][jet] << "\t&\t"
             << WZ_final[ichan][jet] << "$\\pm$" <<  WZ_final_err[ichan][jet] << "\t&\t"
             << ZZ_final[ichan][jet] << "$\\pm$" <<  ZZ_final_err[ichan][jet] << "\t&\t"
             << Zj_final[ichan][jet] << "$\\pm$" <<  Zj_final_err[ichan][jet] << "\t\\\\"
             << endl;
  }
  textfile << "ll" << "\t&\t";
  textfile << fixed
           << H_final[mm][jet]  + H_final[ee][jet]  + H_final[em][jet]  + H_final[me][jet] << "$\\pm$"
           << sqrt(pow(H_final_err[mm][jet],2)+pow(H_final_err[ee][jet],2)+pow(H_final_err[em][jet],2)+pow(H_final_err[me][jet],2)) << "\t&\t"
           << WW_final[mm][jet] + WW_final[ee][jet] + WW_final[em][jet] + WW_final[me][jet] << "$\\pm$"
           << sqrt(pow(WW_final_err[mm][jet],2) + pow(WW_final_err[ee][jet],2) + pow(WW_final_err[em][jet],2) + pow(WW_final_err[me][jet],2)) <<  "\t&\t"
           << WZ_final[mm][jet] + WZ_final[ee][jet] + WZ_final[em][jet] + WZ_final[me][jet] << "$\\pm$"
           << sqrt(pow(WZ_final_err[mm][jet],2) + pow(WZ_final_err[ee][jet],2) + pow(WZ_final_err[em][jet],2) + pow(WZ_final_err[me][jet],2)) << "\t&\t"
           << ZZ_final[mm][jet] + ZZ_final[ee][jet] + ZZ_final[em][jet] + ZZ_final[me][jet] << "$\\pm$"
           << sqrt(pow(ZZ_final_err[mm][jet],2) + pow(ZZ_final_err[ee][jet],2) + pow(ZZ_final_err[em][jet],2) + pow(ZZ_final_err[me][jet],2)) << "\t&\t"
           << Zj_final[mm][jet] + Zj_final[ee][jet] + Zj_final[em][jet] + Zj_final[me][jet] << "$\\pm$"
           << sqrt(pow(Zj_final_err[mm][jet],2) + pow(Zj_final_err[ee][jet],2) + pow(Zj_final_err[em][jet],2) + pow(Zj_final_err[me][jet],2)) << "\t\\\\"
           << endl;
  textfile << "\\hline" << endl; 

  //  textfile << "final state & W+jets & W+$\\gamma$ & $t\\bar t$ & single $t$ & data \t\\\\" << endl;
  textfile << "final state & W+jets & $t\\bar t$ & single $t$ & all bkg & data \t\\\\" << endl;
  textfile << "\\hline" << endl; 
  for(int ichan=0; ichan<4; ++ichan) {
    textfile << channelName[ichan][jet] << "\t&\t";
    textfile << fixed 
             << Wj_final[ichan][jet] << "$\\pm$" << Wj_final_err[ichan][jet] << "\t&\t"
      //     << Wgamma_final[ichan][jet] << "$\\pm$" << Wgamma_final_err[ichan][jet] << "\t&\t"
             << ttj_final[ichan][jet] << "$\\pm$" << ttj_final_err[ichan][jet] << "\t&\t"
             << SingleTop_final[ichan][jet] << "$\\pm$" << SingleTop_final_err[ichan][jet] << "\t&\t"
             << bkg_final[ichan][jet] << "$\\pm$" << bkg_final_err[ichan][jet] << "\t&\t"
             << data_final[ichan][jet] << "\t\\\\"
             << endl;
  }
  textfile << "ll" << "\t&\t";
  textfile << fixed
           << Wj_final[mm][jet]     + Wj_final[ee][jet]     + Wj_final[em][jet]     + Wj_final[me][jet]      << "$\\pm$" 
           << sqrt(pow(Wj_final_err[mm][jet],2)     + pow(Wj_final_err[ee][jet],2)     + pow(Wj_final_err[em][jet],2)     + pow(Wj_final_err[me][jet],2) ) << "\t&\t"
    //     << Wgamma_final[mm][jet] + Wgamma_final[ee][jet] + Wgamma_final[em][jet] + Wgamma_final[me][jet]  << "$\\pm$" 
    //     << sqrt(pow(Wgamma_final_err[mm][jet],2) + pow(Wgamma_final_err[ee][jet],2) + pow(Wgamma_final_err[em][jet],2) + pow(Wgamma_final_err[me][jet],2)) <<  "\t&\t"
           << ttj_final[mm][jet]    + ttj_final[ee][jet]    + ttj_final[em][jet]    + ttj_final[me][jet] << "$\\pm$"
           << sqrt(pow(ttj_final_err[mm][jet],2) + pow(ttj_final_err[ee][jet],2) + pow(ttj_final_err[em][jet],2) + pow(ttj_final_err[me][jet],2)) << "\t&\t"
           << SingleTop_final[mm][jet] + SingleTop_final[ee][jet] + SingleTop_final[em][jet] + SingleTop_final[me][jet] << "$\\pm$"
           << sqrt(pow(SingleTop_final_err[mm][jet],2) + pow(SingleTop_final_err[ee][jet],2) + pow(SingleTop_final_err[em][jet],2) + pow(SingleTop_final_err[me][jet],2)) << "\t&\t"
           << bkg_final[mm][jet] + bkg_final[ee][jet] + bkg_final[em][jet] + bkg_final[me][jet] << "$\\pm$"
           << sqrt(pow(bkg_final_err[mm][jet],2) + pow(bkg_final_err[ee][jet],2) + pow(bkg_final_err[em][jet],2) + pow(bkg_final_err[me][jet],2)) << "\t&\t"
           << data_final[mm][jet] + data_final[ee][jet] + data_final[em][jet]  + data_final[me][jet]  << "\t\\\\"
           << endl;
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\caption{Higgs $m_H$ = " << exampleHiggsMass << " GeV/c$^2$. Expected backgrounds events in "
           << lumiEE << " pb$^-1$ (EE), " << lumiMM << " pb$^-1$ (MM), " << lumiEM << " pb$^-1$ (EM,ME).} "
           << "\\end{table}" << endl;
  }
}

void printLatex(float lumiEE, float lumiMM, float lumiEM, int HiggsMassOptim) {

  exampleHiggsMass = HiggsMassOptim;
  
  char namefile[200];
  sprintf(namefile,"yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::trunc);
  textfile.precision(2);

  if(runStandalone) {
    textfile << "\\documentclass{article}" << endl;
    textfile << "\\setlength\\textheight{9.8in}" << endl;
    textfile << "\\usepackage{rotating}" << endl;
    textfile << "\\begin{document}" << endl;
  }

  textfile.close();

  printLatex(lumiEE, "EE");
  printLatex(lumiMM, "MM");
  printLatex(lumiEM, "EM");
  printLatex(lumiEM, "ME");

  printShortBkgSummary(lumiEE,lumiMM,lumiEM);

  textfile.open(namefile, ios_base::app);
  textfile << "\\clearpage" << endl;
  textfile.close();  

  if(runStandalone) {
    textfile.open(namefile, ios_base::app);
    textfile << "\\end{document}" << endl;
    textfile.close();
  }
}

void printSuperSummary(float lumiEE, float lumiMM, float lumiEM, int massset) {

  int masses[3];
  if(massset==0) {
    masses[0] = 120;
    masses[1] = 130;
    masses[2] = 140;
  } else if(massset==1) {
    masses[0] = 150;
    masses[1] = 160;
    masses[2] = 170;
  } else if(massset==2) {
    masses[0] = 180;
    masses[1] = 190;
    masses[2] = 200;    
  } else if(massset==3) {
    masses[0] = 210;
    masses[1] = 220;
    masses[2] = 230;    
  } else if(massset==4) {
    masses[0] = 250;
    masses[1] = 300;
    masses[2] = 350;
  } else if(massset==5) {
    masses[0] = 400;
    masses[1] = 450;
    masses[2] = 500;
  } else if(massset==6) {
    masses[0] = 550;
    masses[1] = 600;
    masses[2] = 600;
  }
 
  char namefile[200];
  sprintf(namefile,"yieldsSummary_byCut.tex");
  ofstream simpletextfile;
  ofstream textfile;
  if(massset==0) {
    textfile.open(namefile, ios_base::trunc);
    simpletextfile.open("yields_plain.txt", ios_base::trunc);
  }
  else {
    textfile.open(namefile, ios_base::app);
    simpletextfile.open("yields_plain.txt", ios_base::app);
  }
  textfile.precision(2);
  simpletextfile.precision(2);

  if(massset==0) {
    if(runStandalone) {
      textfile << "\\documentclass{article}" << endl;
      textfile << "\\setlength\\textheight{9.8in}" << endl;
      textfile << "\\usepackage{rotating}" << endl;
      textfile << "\\begin{document}" << endl;
    }
  }

  std::string channelName[5][3];
  channelName[mm][0] = "$\\mu\\mu$ 0 j";
  channelName[ee][0] = "$ee$ 0 j";
  channelName[em][0] = "$e\\mu$ 0 j";
  channelName[me][0] = "$\\mu e$ 0 j";

  channelName[mm][1] = "$\\mu\\mu$ 1 j";
  channelName[ee][1] = "$ee$ 1 j";
  channelName[em][1] = "$e\\mu$ 1 j";
  channelName[me][1] = "$\\mu e$ 1 j";
  
  channelName[mm][2] = "$\\mu\\mu$ 2 j";
  channelName[ee][2] = "$ee$ 2 j";
  channelName[em][2] = "$e\\mu$ 2 j";
  channelName[me][2] = "$\\mu e$ 2 j";

  textfile << "\\begin{table}[p]" << endl
           << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c|c|c|c|c|c|c|c|c|}" << endl;
  textfile << "\\hline\\hline" << endl;

  char title[1000];
  sprintf(title,"final state & \\multicolumn{3}{|c|} {$m_H=%d$ GeV} & \\multicolumn{3}{|c|} {$m_H=%d$ GeV} & \\multicolumn{3}{|c|} {$m_H=%d$ GeV} \t\\\\",masses[0],masses[1],masses[2]);
  textfile << title << endl;
  textfile << "\\hline" << endl; 
  textfile << " & signal & background & data & signal & background & data & signal & background & data \\\\" << endl;
  textfile << "\\hline" << endl;

  for(int ichan=0; ichan<4; ++ichan) {

    for(int jet=0; jet<3; ++jet) {
      
      textfile << channelName[ichan][jet] << "\t&\t";
      
      for(int i=0; i<3; i++) {
        int mass = masses[i];
        cout << "======> Now analyzing mass = " << mass << "<======" << endl;
        exampleHiggsMass = mass;
        /*
          if(ichan==ee) computeYields(lumi,"EE",mass);
          if(ichan==mm) computeYields(lumi,"MM",mass);
          if(ichan==em) computeYields(lumi,"EM",mass);
          if(ichan==me) computeYields(lumi,"ME",mass);
        */
        if(ichan==ee) computeYields(lumiEE,"EE");
        if(ichan==mm) computeYields(lumiMM,"MM");
        if(ichan==em) computeYields(lumiEM,"EM");
        if(ichan==me) computeYields(lumiEM,"ME");

        int step;
        if(jet==0) step = 23;
        else if(jet==1) step = 24;
        else if(jet==2) step = 25;
        else {
          std::cout << "WRONG JET BIN. Stopping now." << std::endl;
        }

        H_final[ichan][jet] = H_fullSel[step];
        Wj_final[ichan][jet] = Wj_fullSel[step];
        ttj_final[ichan][jet] = ttj_fullSel[step];
        SingleTop_final[ichan][jet] = SingleTop_fullSel[step];
        Zj_final[ichan][jet] = Zj_fullSel[step];
        WW_final[ichan][jet] = WW_fullSel[step];
        ZZ_final[ichan][jet] = ZZ_fullSel[step];
        WZ_final[ichan][jet] = WZ_fullSel[step];
        Wgamma_final[ichan][jet] = Wgamma_fullSel[step];
        data_final[ichan][jet] = data_fullSel[step];
        
        H_final_err[ichan][jet] = H_fullSel_err[step];
        Wj_final_err[ichan][jet] = Wj_fullSel_err[step];
        ttj_final_err[ichan][jet] = ttj_fullSel_err[step];
        SingleTop_final_err[ichan][jet] = SingleTop_fullSel_err[step];
        Zj_final_err[ichan][jet] = Zj_fullSel_err[step];
        WW_final_err[ichan][jet] = WW_fullSel_err[step];
        ZZ_final_err[ichan][jet] = ZZ_fullSel_err[step];
        WZ_final_err[ichan][jet] = WZ_fullSel_err[step];
        Wgamma_final_err[ichan][jet] = Wgamma_fullSel_err[step];
        
        textfile << fixed 
                 << H_final[ichan][jet] << "$\\pm$" <<  H_final_err[ichan][jet] << "\t&\t"
                 << WW_final[ichan][jet] + 
          WZ_final[ichan][jet] +
          ZZ_final[ichan][jet] +
          Zj_final[ichan][jet] +
          Wj_final[ichan][jet] +
          ttj_final[ichan][jet] +
          SingleTop_final[ichan][jet] 
                 << "$\\pm$" << sqrt(pow(WW_final_err[ichan][jet],2)+pow(WZ_final_err[ichan][jet],2)+pow(ZZ_final_err[ichan][jet],2)+
                                     pow(Zj_final_err[ichan][jet],2)+pow(Wj_final_err[ichan][jet],2)+pow(ttj_final_err[ichan][jet],2)+pow(SingleTop_final_err[ichan][jet],2)) << "\t&\t"
                 << data_final[ichan][jet];
        
        simpletextfile << exampleHiggsMass << "\t"
                       << channelName[ichan][jet] << "\t"
                       << H_final[ichan][jet] << "\t+/-\t" << H_final_err[ichan][jet] << "\t"
                       << WW_final[ichan][jet] + WZ_final[ichan][jet] + ZZ_final[ichan][jet] + Zj_final[ichan][jet] + Wj_final[ichan][jet] + ttj_final[ichan][jet] << "\t+/-\t"
                       << sqrt(pow(WW_final_err[ichan][jet],2)+pow(WZ_final_err[ichan][jet],2)+pow(ZZ_final_err[ichan][jet],2)+
                               pow(Zj_final_err[ichan][jet],2)+pow(Wj_final_err[ichan][jet],2)+pow(ttj_final_err[ichan][jet],2)+pow(SingleTop_final_err[ichan][jet],2)) << "\t"
                       << data_final[ichan][jet] << endl;
        
        if(i<2) textfile << "\t&\t";
        else textfile << "\t\\\\" << endl;
        cout << "$$$$$$$> Done with mass = " << mass << "<$$$$$$$" << endl;
      }
    }
    cout << "done with ichannel = " << ichan << endl;
    textfile << "\\hline" << endl;
  }
    
  textfile << "\\hline" << endl
           << "\\end{tabular}" << endl
           << "\\end{center}" << endl
           << "\\caption{Breakdown of signal and backgrounds events for an integrated luminosity of " 
           << lumiEE << " pb$^-1$ (EE), " << lumiMM << " pb$^-1$ (MM), " << lumiEM << " pb$^-1$ (EM,ME).} "
           << "\\end{table}" << endl;

  if(massset==6) {
    textfile << "\\end{document}" << endl;
    textfile.close();
    simpletextfile.close();
  }

}

void setRunStandalone(bool what) { runStandalone = what; }
void setPrintCutsBreakdown(bool what) { printCutsBreakdown = what; }

float yieldErrPoisson(float nEst1, float n1, float nEst2, float n2, float nEst3, float n3, float nEst4, float n4, float nEst5, float n5, float nEst6, float n6) {

  float sum=0;
  if(n1>0) sum += pow(nEst1,2)/n1;
  if(n2>0) sum += pow(nEst2,2)/n2;
  if(n3>0) sum += pow(nEst3,2)/n3;
  if(n4>0) sum += pow(nEst4,2)/n4;
  if(n5>0) sum += pow(nEst5,2)/n5;
  if(n6>0) sum += pow(nEst6,2)/n6;
  
  return sqrt(sum);
}
