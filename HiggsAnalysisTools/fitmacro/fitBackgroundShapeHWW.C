#include "TDirectory.h"
#include "TPad.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "THStack.h"
#include "TH2.h"
#include "TF1.h"
#include "TLine.h"
#include "TCut.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraphErrors.h"

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooCBShape.h"
#include "RooLandau.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "RooAddPdf.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "Math/MinimizerOptions.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ4LRooPdfs.h"
#include "YieldMaker.h"
#include "FitSelection.hh"

using namespace RooFit;

bool do7TeV_;

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

int Wait() {
     cout << " Continue [<RET>|q]?  ";
     char x;
     x = getchar();
     if ((x == 'q') || (x == 'Q')) return 1;
     return 0;
}

string getChannelSuffix(int channel) {
  if(channel==of0j) return string("of_0j");
  if(channel==of1j) return string("of_1j");
  if(channel==sf0j) return string("sf_0j");
  if(channel==sf1j) return string("sf_1j");
  return string("ERROR! Unclassified channel!");
}

string getStringChannel(int channel) {
  if(channel==of0j) return string("channel>=2 && njet==0");
  if(channel==of1j) return string("channel>=2 && njet==1");
  if(channel==sf0j) return string("channel<2 && njet==0");
  if(channel==sf1j) return string("channel<2 && njet==1");
  return string("ERROR! Unclassified channel!");
}

std::string fitLandauShapeMR(int channel, string sample,
                             double rangeLow, double rangeHigh,
                             double fitValues[2], double fitErrors[2], string syst="nom");

std::string fitWJetsLandauShapeMR(int channel, string sample,
                                  double rangeLow, double rangeHigh,
                                  double fitValues[2], double fitErrors[2], string syst="nom");

std::string fitGaussianShapeMR(int channel, string sample,
                               double rangeLow, double rangeHigh,
                               double fitValues[2], double fitErrors[2], string syst="nom");

std::string fitOthersSFShapeMR(int channel, string sample,
                               double rangeLow, double rangeHigh,
                               double fitValues[14], double fitErrors[14], string syst="nom");

void allqqWW(ofstream& file, int channel, string syst);
void allggWW(ofstream& file, int channel, string syst);
void allTop(ofstream& file, int channel, string syst);
void allDY(ofstream& file, int channel, string syst);
void allWgstar(ofstream& file, int channel, string syst);
void allOthers(ofstream& file, int channel, string syst);
void allWjets(ofstream& file, int channel, string syst);
                        
void doAllChannels(bool do7TeV) {

  do7TeV_=do7TeV;

  ofstream fileParams;
  if(do7TeV_) fileParams.open("paramsHWW_7TeV.txt",ios_base::trunc);
  else fileParams.open("paramsHWW_8TeV.txt",ios_base::trunc);

  vector<string> systematics;
  systematics.push_back("nominals");
  systematics.push_back("res_met");
  systematics.push_back("res_e");
  systematics.push_back("scaleup_e");
  systematics.push_back("scaledn_e");
  systematics.push_back("scaleup_m");
  systematics.push_back("scaledn_m");
  systematics.push_back("scaleup_j");
  systematics.push_back("scaledn_j");

  const char* e0 = "\033[44;37m";
  const char* e1 = "\033[41;37m";
  const char* en="\033[0m";

  for(int s=0; s<(int)systematics.size(); ++s) {

    cout << e0 << "%%% Parameterising samples with systematic " << systematics[s] << " ON..." << en << endl;

    cout << e1 << "==> Fitting qq->WW sample..." << en << endl;
    fileParams << "# qq->WW pdfs" << endl;
    for(int i=0; i<2; ++i) allqqWW(fileParams,i,systematics[s]);
    cout << e1 << "### Done qq->WW sample ###" << en << endl;

    cout << e1 << "==> Fitting gg->WW sample..." << en << endl;
    fileParams << "# gg->WW pdfs" << endl;
    for(int i=0; i<2; ++i) allggWW(fileParams,i,systematics[s]);
    cout << e1 << "### Done gg->WW sample ###" << en << endl;

    cout << e1 << "==> Fitting top sample..." << en << endl;
    fileParams << "# top pdfs" << endl;
    for(int i=0; i<2; ++i) allTop(fileParams,i,systematics[s]);
    cout << e1 << "### Done top sample ###" << en << endl;

    cout << e1 << "==> Fitting DY sample..." << en << endl;
    fileParams << "# DY pdfs" << endl;
    for(int i=0; i<2; ++i) allDY(fileParams,i,systematics[s]);
    cout << e1 << "### Done DY sample ###" << en << endl;

    cout << e1 << "==> Fitting Wg* sample..." << en << endl;
    fileParams << "# Wg* pdfs" << endl;
    for(int i=0; i<2; ++i) allWgstar(fileParams,i,systematics[s]);
    cout << e1 << "### Done Wg* sample ###" << en << endl;

    cout << e1 <<  "==> Fitting others sample..." << en << endl;
    fileParams << "# Wg,WZ,ZZ=others pdfs" << endl;
    for(int i=0; i<2; ++i) allOthers(fileParams,i,systematics[s]);
    cout << e1 << "### Done Others sample ###" << en << endl;
  }

  vector<string> systematics_fakerate;
  systematics_fakerate.push_back("nominals");
  systematics_fakerate.push_back("fakerateup");
  systematics_fakerate.push_back("fakeratedn");
  
  for(int s=0; s<(int)systematics_fakerate.size(); ++s) {

    cout << e0 << "%%% Parameterising samples with systematic " << systematics_fakerate[s] << " ON..." << en << endl;

    cout << e1 << "==> Fitting wjets sample..." << en << endl;
    fileParams << "# Wjets pdfs" << endl;
    for(int i=0; i<2; ++i) allWjets(fileParams,i,systematics_fakerate[s]);
    cout << e0 << "### Done Wjets sample ###" << en << endl;
  }

  fileParams.close();
}

void allqqWW(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[2];
  double fitErrors[2];

  // nominal sample: WW Madgraph
  file << fitLandauShapeMR(channel,"qqWWMadgraph",xLow,xHigh,fitValues,fitErrors,syst);
  cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
  cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;
  if(syst.find("nominals")!=string::npos) {
    file << fitLandauShapeMR(channel,"qqWWMCatNLONom",xLow,xHigh,fitValues,fitErrors,syst);
    file << fitLandauShapeMR(channel,"qqWWMCatNLOUp",xLow,xHigh,fitValues,fitErrors,syst);
    file << fitLandauShapeMR(channel,"qqWWMCatNLODown",xLow,xHigh,fitValues,fitErrors,syst);
  }
}

void allggWW(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[2];
  double fitErrors[2];

  file << fitLandauShapeMR(channel,"ggWW",xLow,xHigh,fitValues,fitErrors,syst);
  cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
  cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;
}

void allTop(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[2];
  double fitErrors[2];

  file << fitLandauShapeMR(channel,"Top",xLow,xHigh,fitValues,fitErrors,syst);
  cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
  cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;
}

void allWgstar(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 70; xHigh = 500;

  double fitValues[2];
  double fitErrors[2];

  file << fitLandauShapeMR(channel,"WGstar",xLow,xHigh,fitValues,fitErrors,syst);
  cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
  cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;
}

void allOthers(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[14];
  double fitErrors[14];

  if(channel<2) {
    file << fitLandauShapeMR(channel,"Ot",xLow,xHigh,fitValues,fitErrors,syst);
    cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
    cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;
  } else {
    file << fitOthersSFShapeMR(channel,"others",xLow,xHigh,fitValues,fitErrors,syst);    
    for(int i=0;i<14;++i) cout << "a" << i << " value,error = " << fitValues[i] << " , " << fitErrors[i] << endl;
  }
}

void allDY(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[14];
  double fitErrors[14];

  if(channel<2) {
    if(syst.find("nominals")==string::npos) return;
    file << fitGaussianShapeMR(channel,"embeddedtt",xLow,xHigh,fitValues,fitErrors,syst);
  } else {
    if(syst.find("scaleup_j")!=string::npos || syst.find("scaledn_j")!=string::npos) return;
    file << fitOthersSFShapeMR(channel,"Zjets",xLow,xHigh,fitValues,fitErrors,syst);    
    for(int i=0;i<14;++i) cout << "a" << i << " value,error = " << fitValues[i] << " , " << fitErrors[i] << endl;
  }
}

void allWjets(ofstream& file, int channel, string syst) {
  double xLow, xHigh;
  xLow = 50; xHigh = 500;

  double fitValues[2];
  double fitErrors[2];

  file << fitWJetsLandauShapeMR(channel,"wjets",xLow,xHigh,fitValues,fitErrors,syst);
  cout << "mean value,error = " << fitValues[0] << " , " << fitErrors[0] << endl;
  cout << "sigma value,error = " << fitValues[1] << " , " << fitErrors[1] << endl;

}

std::string fitLandauShapeMR(int channel, string sample,
                             double rangeLow, double rangeHigh,
                             double fitValues[2], double fitErrors[2], string syst){
 // ------ root settings ---------
  gROOT->Reset();  
  gROOT->SetStyle("Plain");
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  //gStyle->SetOptStat("kKsSiourRmMen");
  gStyle->SetOptStat("iourme");
  //gStyle->SetOptStat("rme");
  //gStyle->SetOptStat("");
  gStyle->SetOptFit(11);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.06);
  // ------------------------------ 

  ROOT::Math::MinimizerOptions::SetDefaultTolerance( 1.E-7);

  string dir;
  if(syst.find("res_e")!=string::npos) dir="electronResolution";
  else if(syst.find("res_met")!=string::npos) dir="metResolution";
  else if(syst.find("scaleup_e")!=string::npos) dir="electronScale_up";
  else if(syst.find("scaledn_e")!=string::npos) dir="electronScale_down";
  else if(syst.find("scaleup_m")!=string::npos) dir="muonScale_up";
  else if(syst.find("scaledn_m")!=string::npos) dir="muonScale_down";
  else if(syst.find("scaleup_j")!=string::npos) dir="jetEnergyScale_up";
  else if(syst.find("scaledn_j")!=string::npos) dir="jetEnergyScale_down";
  else dir="nominals";

  string tevstr = (do7TeV_ ? "_7TeV/" : "_8TeV/");

  YieldMaker  ymaker_hi;
  if(sample.find("qqWWMadgraph")!=string::npos) {
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_000_WWJets2LMad.root"));
  } else if(sample.find("qqWWMCatNLONom")!=string::npos) {
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_002_WWto2L2NuMCatNLO.root"));
  } else if(sample.find("qqWWMCatNLOUp")!=string::npos) {
      ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_004_WWto2L2NuMCatNLOUp.root"));
  } else if(sample.find("qqWWMCatNLODown")!=string::npos) {
      ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_003_WWto2L2NuMCatNLODown.root"));
  } else if(sample.find("ggWW")!=string::npos) {
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_001_GluGluToWWTo4L.root"));
  } else if(sample.find("Top")!=string::npos) {
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_019_TTTo2L2Nu2B.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_011_TtWFullDR.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_012_TbartWFullDR.root"));
  } else if(sample.find("WGstar")!=string::npos) {
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_082_WGstarToElNuMad.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_083_WGstarToMuNuMad.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_084_WGstarToTauNuMad.root"));
  } else if(sample.find("Ot")!=string::npos) {
    // remove those two: too few stat and large weights
    // ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_085_WgammaToLNuG.root"));
    // ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_086_ZgammaToLLuG.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_074_WZJetsMad.root"));
    ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_075_ZZJetsMad.root"));
  } 

  FitSelection sel;  

  //--- rooFit part
  double xMin,xMax,xInit;
  xInit = 140;
  xMin = rangeLow;
  xMax = rangeHigh ;
  
  stringstream weight;
  weight << "weight";
  RooRealVar x("mr","M_{R}",xInit,xMin,xMax,"GeV");
  RooRealVar w(weight.str().c_str(),weight.str().c_str(),1.0,-1000.,1000.);

  RooArgSet varset(x,w,"argset_obs");
  RooDataSet dataset("dataset","dataset",varset,WeightVar(weight.str().c_str()));
  ymaker_hi.getDataSet1D(channel, xMin, xMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, dataset, x, w);

  float mPars[3]={140,70,200};
  float sPars[3]={10,10,100};
  if(sample.find("WGstar")!=string::npos && channel==0) {
    mPars[0]=90; mPars[2]=98; 
    sPars[0]=5; sPars[1]=1;
  }
  //--- Landau
  RooRealVar mean("mean","mean",mPars[0],mPars[1],mPars[2]) ;
  RooRealVar sigma("#sigma","width",sPars[0],sPars[1],sPars[2]); 
  RooLandau landau("landau","landau",x,mean,sigma);

  if(sample.find("wgstar")!=string::npos) sigma.setRange(1,20);

  landau.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));

  stringstream frameTitle;
  if(channel==of0j){frameTitle << "e#mu,0-j";}
  if(channel==of1j){frameTitle << "e#mu,1-j";}
  if(channel==sf0j){frameTitle << "ee+#mu#mu,0-j";}
  if(channel==sf1j){frameTitle << "ee+#mu#mu,1-j";}

  RooPlot* xframe = x.frame(xMin,xMax,50) ;
  dataset.plotOn(xframe,DataError(RooAbsData::SumW2) );
  landau.plotOn(xframe);
  landau.paramOn(xframe);

  stringstream nameFile;
  nameFile << "fit_" << sample << "_" << getChannelSuffix(channel) 
           << "_" << syst << ".pdf";
  xframe->Draw(); gPad->Update(); gPad->Print(nameFile.str().c_str());


  if(fitValues!=0){
    fitValues[0] = mean.getVal();
    fitValues[1] = sigma.getVal();
  }

  if(fitErrors!=0){
    fitErrors[0] = mean.getError();
    fitErrors[1] = sigma.getError();
  }

  stringstream info;
  info << getStringFitChannel(channel) << "_" << sample << "_me_" << syst
       << " = " << mean.getVal() << " +/- " << mean.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_si_" << syst 
       << " = " << sigma.getVal() << " +/- " << sigma.getError() << endl;
  
  return info.str();

}

std::string fitWJetsLandauShapeMR(int channel, string sample,
                                  double rangeLow, double rangeHigh,
                                  double fitValues[2], double fitErrors[2], string syst){
 // ------ root settings ---------
  gROOT->Reset();  
  gROOT->SetStyle("Plain");
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  //gStyle->SetOptStat("kKsSiourRmMen");
  gStyle->SetOptStat("iourme");
  //gStyle->SetOptStat("rme");
  //gStyle->SetOptStat("");
  gStyle->SetOptFit(11);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.06);
  // ------------------------------ 

  ROOT::Math::MinimizerOptions::SetDefaultTolerance( 1.E-7);

  string dir="wjets";
  int ns=0;
  if(syst.find("fakerateup")!=string::npos) ns=1;
  if(syst.find("fakeratedn")!=string::npos) ns=-1;

  string tevstr = (do7TeV_ ? "_7TeV/" : "_8TeV/");

  WJetsYieldMaker  ymaker_wj(ns);
  if(do7TeV_) ymaker_wj.fill(string("latinos_tree_skim_of"+tevstr+"/wjets/WJetsEstimated_Full2011_added.root"));
  else ymaker_wj.fill(string("latinos_tree_skim_of"+tevstr+"wjets/latino_LooseLoose_19.5fb.root")); 

  FitSelection sel;

  //--- rooFit part
  double xMin,xMax,xInit;
  xInit = 140;
  xMin = rangeLow;
  xMax = rangeHigh ;
  
  stringstream weight;
  weight << "weight";
  RooRealVar x("mr","M_{R}",xInit,xMin,xMax,"GeV");
  RooRealVar w(weight.str().c_str(),weight.str().c_str(),1.0,-1000.,1000.);

  RooArgSet varset(x,w,"argset_obs");
  RooDataSet dataset("dataset","dataset",varset,WeightVar(weight.str().c_str()));
  ymaker_wj.getDataSet1D(channel, xMin, xMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, dataset, x, w);

  //--- Landau
  RooRealVar mean("mean","mean",140,70,200) ;
  RooRealVar sigma("#sigma","width",10,10,100); 
  RooLandau landau("landau","landau",x,mean,sigma);

  if(sample.find("wgstar")!=string::npos) sigma.setRange(1,20);

  x.setBins(10000,"fft");
  landau.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));

  stringstream frameTitle;
  if(channel==of0j){frameTitle << "e#mu,0-j";}
  if(channel==of1j){frameTitle << "e#mu,1-j";}
  if(channel==sf0j){frameTitle << "ee+#mu#mu,0-j";}
  if(channel==sf1j){frameTitle << "ee+#mu#mu,1-j";}

  RooPlot* xframe = x.frame(Title(frameTitle.str().c_str() )) ;
  dataset.plotOn(xframe,DataError(RooAbsData::SumW2) );
  landau.plotOn(xframe);
  landau.paramOn(xframe);

  stringstream nameFile;
  nameFile << "fit_" << sample << "_" << getChannelSuffix(channel) 
           << "_" << syst << ".pdf";
  xframe->Draw(); gPad->Update(); gPad->Print(nameFile.str().c_str());


  if(fitValues!=0){
    fitValues[0] = mean.getVal();
    fitValues[1] = sigma.getVal();
  }

  if(fitErrors!=0){
    fitErrors[0] = mean.getError();
    fitErrors[1] = sigma.getError();
  }

  stringstream info;
  info << getStringFitChannel(channel) << "_" << sample << "_me_" << syst
       << " = " << mean.getVal() << " +/- " << mean.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_si_" << syst 
       << " = " << sigma.getVal() << " +/- " << sigma.getError() << endl;
  
  return info.str();

}

std::string fitGaussianShapeMR(int channel, string sample,
                               double rangeLow, double rangeHigh,
                               double fitValues[14], double fitErrors[14], string syst){
 // ------ root settings ---------
  gROOT->Reset();  
  gROOT->SetStyle("Plain");
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  //gStyle->SetOptStat("kKsSiourRmMen");
  gStyle->SetOptStat("iourme");
  //gStyle->SetOptStat("rme");
  //gStyle->SetOptStat("");
  gStyle->SetOptFit(11);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.06);
  // ------------------------------ 

  ROOT::Math::MinimizerOptions::SetDefaultTolerance( 1.E-7);

  string dir="nominals";
  string tevstr = (do7TeV_ ? "_7TeV/" : "_8TeV/");

  YieldMaker  ymaker_hi;
  if(sample.find("embeddedtt")!=string::npos) { 
    dir="dyTemplate";
    if(do7TeV_) ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_DYtt_2011_added.root"));
    else ymaker_hi.fill(string("latinos_tree_skim_of"+tevstr+dir+"/latino_DYtt_19.5fb.root"));
  }

  FitSelection sel;

  //--- rooFit part
  double xMin,xMax,xInit;
  xInit = 140;
  xMin = rangeLow;
  xMax = rangeHigh ;
  
  RooRealVar x("mr","M_{R}",xInit,xMin,xMax,"GeV");
  RooRealVar w("baseW","baseW",1.0,0.,1000.);

  RooArgSet varset(x,w,"argset_obs");
  RooDataSet dataset("mass","mass",varset,WeightVar("baseW"));
  ymaker_hi.getDataSet1D(channel, xMin, xMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, dataset, x, w); 

  //--- simple Gaussian 1
  RooRealVar mean("mean_{1}","mean",140,50,200) ;
  RooRealVar sigma("#sigma_{1}","width",20,10,40); 
  RooGaussian gauss("gauss","gauss",x,mean,sigma);

  RooRealVar mean2("mean_{2}","mean2",140,0,300) ;
  RooRealVar sigma2("#sigma_{2}","width2",35,30,200); 
  RooGaussian gauss2("gauss2","gauss2",x,mean2,sigma2);
  RooRealVar fsig("f_{1}","signal fraction",0.95,0.7,1.);

  RooAddPdf dgauss("dgauss","model",gauss,gauss2,fsig);

  dgauss.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));


  stringstream frameTitle;
  if(channel==of0j){frameTitle << "e#mu,0-j";}
  if(channel==of1j){frameTitle << "e#mu,1-j";}
  if(channel==sf0j){frameTitle << "ee+#mu#mu,0-j";}
  if(channel==sf1j){frameTitle << "ee+#mu#mu,1-j";}

  RooPlot* xframe = x.frame(Title(frameTitle.str().c_str() )) ;
  dataset.plotOn(xframe,DataError(RooAbsData::SumW2) );
  dgauss.plotOn(xframe);
  dgauss.paramOn(xframe);

  stringstream nameFile;
  nameFile << "fit" << sample << "_" << getChannelSuffix(channel) << ".pdf";
  xframe->Draw(); gPad->Update(); gPad->Print(nameFile.str().c_str());


  cout << "mean1 = " << mean.getVal() << " +/-" << mean.getError() << endl;
  cout << "sigma1 = " << sigma.getVal() << " +/-" << sigma.getError() << endl;
  cout << "mean2 = " << mean2.getVal() << " +/-" << mean2.getError() << endl;
  cout << "sigma2 = " << sigma2.getVal() << " +/-" << sigma2.getError() << endl;
  cout << "frac = " << fsig.getVal() << " +/-" << fsig.getError() << endl;

  if(fitValues!=0){
    fitValues[0] = mean.getVal();
    fitValues[1] = sigma.getVal();
    fitValues[2] = mean2.getVal();
    fitValues[3] = sigma2.getVal();
    fitValues[4] = fsig.getVal();
  }

  if(fitErrors!=0){
    fitErrors[0] = mean.getError();
    fitErrors[1] = sigma.getError();
    fitValues[2] = mean2.getError();
    fitValues[3] = sigma2.getError();
    fitValues[4] = fsig.getError();
  }

  stringstream info;
  info << getStringFitChannel(channel) << "_" << sample << "_me1_" << syst
       << " = " << mean.getVal() << " +/- " << mean.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_si1_" << syst 
       << " = " << sigma.getVal() << " +/- " << sigma.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_me2_" << syst
       << " = " << mean2.getVal() << " +/- " << mean2.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_si2_" << syst 
       << " = " << sigma2.getVal() << " +/- " << sigma2.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_f_" << syst 
       << " = " << fsig.getVal() << " +/- " << fsig.getError() << endl;
  
  return info.str();


}


std::string fitOthersSFShapeMR(int channel, string sample,
                               double rangeLow, double rangeHigh,
                               double fitValues[14], double fitErrors[14], string syst){
 // ------ root settings ---------
  gROOT->Reset();  
  gROOT->SetStyle("Plain");
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  //gStyle->SetOptStat("kKsSiourRmMen");
  gStyle->SetOptStat("iourme");
  //gStyle->SetOptStat("rme");
  //gStyle->SetOptStat("");
  gStyle->SetOptFit(11);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.06);
  // ------------------------------ 

  ROOT::Math::MinimizerOptions::SetDefaultTolerance( 1.E-7);

  stringstream hFileName;
  hFileName << "results/datasets_trees/" << sample << "_ll.root";

  cout << "Opening ROOT file: " << hFileName.str() << endl;

  TFile* hFile = TFile::Open(hFileName.str().c_str());
  TTree* hTree = (TTree*) hFile->Get("latinoFitSkim");
  
  float mr, dphillr;
  float puW,effW,baseW;

  hTree->SetBranchAddress("mr",&mr);
  hTree->SetBranchAddress("dphillr",&dphillr);
  hTree->SetBranchAddress("puW",&puW);
  hTree->SetBranchAddress("baseW",&baseW);
  hTree->SetBranchAddress("effW",&effW);

  //--- rooFit part
  double xMin,xMax,xInit;
  xInit = 140;
  xMin = rangeLow;
  xMax = rangeHigh ;
  

  TCut cut1 = getStringChannel(channel).c_str();
  stringstream fitrangecut;
  fitrangecut << "mr > " << xMin << " && mr < " << xMax;
  TCut cut2 = fitrangecut.str().c_str();
  TCut cut = cut1 && cut2;

  stringstream weight;
  weight << "baseW";

  RooRealVar x("mr","M_{R}",xInit,xMin,xMax,"GeV");
  RooRealVar w(weight.str().c_str(),weight.str().c_str(),1.0,-1000.,1000.); 
  RooRealVar cha("channel","channel",0,-0.5,3.5);
  RooRealVar njet("njet","njet",0,-0.5,10.);

  RooArgSet varset(x,w,cha,njet);
  RooDataSet dataset("mass","mass",varset,Import(*hTree),WeightVar(weight.str().c_str()),Cut(cut));


  //--- RooqqZZPdf
  RooRealVar a0("a0","a0",100.,0.,200.);
  RooRealVar a1("a1","a1",15.,0.,50.);
  RooRealVar a2("a2","a2",120.,20.,200.);
  RooRealVar a3("a3","a3",0.04,0.,1.);
  RooRealVar a4("a4","a4",185.,100.,400.);
  RooRealVar a5("a5","a5",10.,0.,150.);
  RooRealVar a6("a6","a6",36.,0.,100.);
  RooRealVar a7("a7","a7",0.11,0.,1.);
  RooRealVar a8("a8","a8",60.,0.,150.);
  RooRealVar a9("a9","a9",0.06,0.,1.);
  RooRealVar a10("a10","a10",95.,20.,200.);
  RooRealVar a11("a11","a11",-6.,-20.,20.);
  RooRealVar a12("a12","a12",1000.,0.,10000.);
  RooRealVar a13("a13","a13",0.1,0.,1.);
  RooqqZZPdf_v2 othersSFPdf("othersSFPdf","othersSFPdf",x,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13);

  x.setBins(10000,"fft");
  othersSFPdf.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));

  stringstream frameTitle;
  if(channel==of0j){frameTitle << "e#mu,0-j";}
  if(channel==of1j){frameTitle << "e#mu,1-j";}
  if(channel==sf0j){frameTitle << "ee+#mu#mu,0-j";}
  if(channel==sf1j){frameTitle << "ee+#mu#mu,1-j";}

  RooPlot* xframe = x.frame(Title(frameTitle.str().c_str() )) ;
  dataset.plotOn(xframe,DataError(RooAbsData::SumW2) );
  othersSFPdf.plotOn(xframe);
  //  othersSFPdf.paramOn(xframe);

  stringstream nameFile;
  nameFile << "fit" << sample << "_" << getChannelSuffix(channel) 
           << "_" << syst << ".pdf";
  xframe->Draw(); gPad->Update(); gPad->Print(nameFile.str().c_str());


  if(fitValues!=0){
    fitValues[0] = a0.getVal();
    fitValues[1] = a1.getVal();
    fitValues[2] = a2.getVal();
    fitValues[3] = a3.getVal();
    fitValues[4] = a4.getVal();
    fitValues[5] = a5.getVal();
    fitValues[6] = a6.getVal();
    fitValues[7] = a7.getVal();
    fitValues[8] = a8.getVal();
    fitValues[9] = a9.getVal();
    fitValues[10] = a10.getVal();
    fitValues[11] = a11.getVal();
    fitValues[12] = a12.getVal();
    fitValues[13] = a13.getVal();
  }

  if(fitErrors!=0){
    fitErrors[0] = a0.getError();
    fitErrors[1] = a1.getError();
    fitErrors[2] = a2.getError();
    fitErrors[3] = a3.getError();
    fitErrors[4] = a4.getError();
    fitErrors[5] = a5.getError();
    fitErrors[6] = a6.getError();
    fitErrors[7] = a7.getError();
    fitErrors[8] = a8.getError();
    fitErrors[9] = a9.getError();
    fitErrors[10] = a10.getError();
    fitErrors[11] = a11.getError();
    fitErrors[12] = a12.getError();
    fitErrors[13] = a13.getError();
  }

  stringstream info;
  info << getStringFitChannel(channel) << "_" << sample << "_a0_" << syst
       << " = " << a0.getVal() << " +/- " << a0.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a1_" << syst
       << " = " << a1.getVal() << " +/- " << a1.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a2_" << syst
       << " = " << a2.getVal() << " +/- " << a2.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a3_" << syst
       << " = " << a3.getVal() << " +/- " << a3.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a4_" << syst
       << " = " << a4.getVal() << " +/- " << a4.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a5_" << syst
       << " = " << a5.getVal() << " +/- " << a5.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a6_" << syst
       << " = " << a6.getVal() << " +/- " << a6.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a7_" << syst
       << " = " << a7.getVal() << " +/- " << a7.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a8_" << syst
       << " = " << a8.getVal() << " +/- " << a8.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a9_" << syst
       << " = " << a9.getVal() << " +/- " << a9.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a10_" << syst
       << " = " << a10.getVal() << " +/- " << a10.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a11_" << syst
       << " = " << a11.getVal() << " +/- " << a11.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a12_" << syst
       << " = " << a12.getVal() << " +/- " << a12.getError() << endl;
  info << getStringFitChannel(channel) << "_" << sample << "_a13_" << syst
       << " = " << a13.getVal() << " +/- " << a13.getError() << endl;
  info << "\n\n";
  
  return info.str();


}

