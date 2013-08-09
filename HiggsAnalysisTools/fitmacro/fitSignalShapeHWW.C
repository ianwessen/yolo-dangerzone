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
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TPaveText.h"
#include "RooAddPdf.h"
#include "RooBreitWigner.h"
#include "RooFFTConvPdf.h"
#include "Math/MinimizerOptions.h"
#include "FitSelection.hh"
#include "YieldMaker.h"

using namespace RooFit;

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

bool do7TeV_;

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

std::string fitSignalShapeMR(int massBin, int channel, 
                             double rangeLow, double rangeHigh,
                             double bwSigma,
                             double fitValues[5], double fitErrors[5], string syst);
                        
void all(int channel=0, bool do7TeV=false) {
  double bwSigma[30];
  int mass[30]; double xLow[30]; double xHigh[30];  
  int maxMassBin;

  do7TeV_=do7TeV;

  if(do7TeV) {
    mass[0] = 120; xLow[0] = 70; xHigh[0] = 160; bwSigma[0] = 3.5/1000.;
    mass[1] = 130; xLow[1] = 70; xHigh[1] = 170; bwSigma[1] = 4.9/1000.;
    mass[2] = 140; xLow[2] = 70; xHigh[2] = 190; bwSigma[2] = 8.1/1000.;
    mass[3] = 150; xLow[3] = 70; xHigh[3] = 210; bwSigma[3] = 1.7/100.;
    mass[4] = 160; xLow[4] = 85; xHigh[4] = 210; bwSigma[4] = 8.3/100.;
    mass[5] = 170; xLow[5] = 85; xHigh[5] = 240; bwSigma[5] = 3.8/10.;
    mass[6] = 180; xLow[6] = 90; xHigh[6] = 240; bwSigma[6] = 6.3/10.;
    maxMassBin = 7;    
  } else {
    mass[0] = 115; xLow[0] = 70; xHigh[0] = 160; bwSigma[0] = 3.1/1000.;
    mass[1] = 120; xLow[1] = 70; xHigh[1] = 160; bwSigma[1] = 3.5/1000.;
    mass[2] = 125; xLow[2] = 70; xHigh[2] = 170; bwSigma[2] = 4.1/1000.;
    mass[3] = 130; xLow[3] = 70; xHigh[3] = 170; bwSigma[3] = 4.9/1000.;
    mass[4] = 135; xLow[4] = 70; xHigh[4] = 180; bwSigma[4] = 4.9/1000.;
    mass[5] = 140; xLow[5] = 70; xHigh[5] = 190; bwSigma[5] = 8.1/1000.;
    mass[6] = 150; xLow[6] = 70; xHigh[6] = 210; bwSigma[6] = 1.7/100.;
    mass[7] = 160; xLow[7] = 85; xHigh[7] = 210; bwSigma[7] = 8.3/100.;
    mass[8] = 170; xLow[8] = 85; xHigh[8] = 240; bwSigma[8] = 3.8/10.;
    mass[9] = 180; xLow[9] = 90; xHigh[9] = 240; bwSigma[9] = 6.3/10.;
    maxMassBin = 10;
  }

  double massV[30],massE[30];
  for(int i=0; i<maxMassBin;++i){
    massV[i]=mass[i];
    massE[i]=0;
  }

  double aVal[30],aErr[30];
  double nVal[30],nErr[30];
  double meanCBVal[30],meanCBErr[30];
  double sigmaCBVal[30],sigmaCBErr[30];
  double meanBWVal[30],meanBWErr[30];

  double fitValues[5];
  double fitErrors[5];

//   double extendL(1),extendH(1);

//   if(channels==0) {extendL=1.0;extendH=1.0;}
//   if(channels==1) {extendL=0.90;extendH=1.05;}
//   if(channels==2) {extendL=0.95;extendH=1.04;}

  for(int i=0; i<maxMassBin;++i){

    fitSignalShapeMR(mass[i],channel,xLow[i],xHigh[i],bwSigma[i],fitValues,fitErrors,"nominals");  
  
    cout << "a value,error: " << fitValues[0] << " , " << fitErrors[0] << endl; 
    aVal[i]=fitValues[0]; aErr[i]=fitErrors[0];

    cout << "n value,error: " << fitValues[3] << " , " << fitErrors[3] << endl; 
    nVal[i]=fitValues[3]; nErr[i]=fitErrors[3];

    cout << "meanCB value,error: " << fitValues[1] << " , " << fitErrors[1] << endl;
    meanCBVal[i]=fitValues[1]; meanCBErr[i]=fitErrors[1];
    
    cout << "sigmaCB value,error: " << fitValues[4] << " , " << fitErrors[4] << endl; 
    sigmaCBVal[i]=fitValues[4]; sigmaCBErr[i]=fitErrors[4];

    cout << "meanBW value,error: " << fitValues[2] << " , " << fitErrors[2] << endl; 
    meanBWVal[i]=fitValues[2]; meanBWErr[i]=fitErrors[2];

    //Wait();
  }

  TGraphErrors* gA = new TGraphErrors(maxMassBin,massV,aVal,massE,aErr);
  TGraphErrors* gN = new TGraphErrors(maxMassBin,massV,nVal,massE,nErr);
  TGraphErrors* gMeanCB = new TGraphErrors(maxMassBin,massV,meanCBVal,massE,meanCBErr);
  TGraphErrors* gSigmaCB = new TGraphErrors(maxMassBin,massV,sigmaCBVal,massE,sigmaCBErr);
  TGraphErrors* gMeanBW = new TGraphErrors(maxMassBin,massV,meanBWVal,massE,meanBWErr);

  gA->SetMarkerStyle(20);   gA->SetMarkerSize(1);
  gN->SetMarkerStyle(20);   gN->SetMarkerSize(1);
  gMeanCB->SetMarkerStyle(20);   gMeanCB->SetMarkerSize(1);
  gSigmaCB->SetMarkerStyle(20);   gSigmaCB->SetMarkerSize(1);
  gMeanBW->SetMarkerStyle(20);   gMeanBW->SetMarkerSize(1);


  gA->SetTitle("");
  gA->GetXaxis()->SetTitle("mass (GeV)");
  gA->GetYaxis()->SetTitle("CB a-parameter");

  gN->SetTitle("");
  gN->GetXaxis()->SetTitle("mass (GeV)");
  gN->GetYaxis()->SetTitle("CB n-parameter");

  gMeanCB->SetTitle("");
  gMeanCB->GetXaxis()->SetTitle("mass (GeV)");
  gMeanCB->GetYaxis()->SetTitle("CB mean (GeV)");

  gSigmaCB->SetTitle("");
  gSigmaCB->GetXaxis()->SetTitle("mass (GeV)");
  gSigmaCB->GetYaxis()->SetTitle("CB sigma (GeV)");


  gStyle->SetOptFit(111111);
  stringstream nameFile;
  nameFile << "fitParams_" <<  getChannelSuffix(channel) << (do7TeV ? "_7TeV" : "_8TeV");
  gA->Fit("pol0"); gA->Draw("Ap"); gPad->Update(); gPad->Print((nameFile.str()+string("_aCB.pdf")).c_str()); Wait();
  gN->Fit("pol1"); gN->Draw("Ap"); gPad->Update(); gPad->Print((nameFile.str()+string("_nCB.pdf")).c_str()); Wait();
  gMeanCB->Fit("pol1"); gMeanCB->Draw("Ap"); gPad->Update(); gPad->Print((nameFile.str()+string("_meanCB.pdf")).c_str()); Wait();
  gSigmaCB->Fit("pol1"); gSigmaCB->Draw("Ap"); gPad->Update(); gPad->Print((nameFile.str()+string("_sigmaCB.pdf")).c_str()); Wait();

  TF1 *fA = (TF1*)gA->GetFunction("pol0");
  TF1 *fN = (TF1*)gN->GetFunction("pol1");
  TF1 *fMeanCB = (TF1*)gMeanCB->GetFunction("pol1");
  TF1 *fSigmaCB = (TF1*)gSigmaCB->GetFunction("pol1");

  std::vector<string> names;
  std::vector<TF1*> fcns;
  fcns.push_back(fA);       names.push_back("fA");
  fcns.push_back(fN);       names.push_back("fN");
  fcns.push_back(fMeanCB);  names.push_back("fMeanCB");
  fcns.push_back(fSigmaCB); names.push_back("fSigmaCB");

  for(int fcn=0;fcn<(int)fcns.size();++fcn) {

    std::stringstream ss;
    
    for (int i = 0; i < fcns[fcn]->GetNumberFreeParameters(); i++) {
      if (i != 0) ss << " + (";
      else ss << "(";
      ss << fcns[fcn]->GetParameter(i);
      for (int j = 0; j < i; j++) {
        ss << "*@0";
      }
      ss << ")";
    }
    cout << "RooFormulaVar for " << names[fcn] << " = " << ss.str() << endl;
  }


}

void signalSystematics(bool do7TeV=false) {

  const char* e0 = "\033[44;37m";
  const char* e1 = "\033[41;37m";
  const char* en="\033[0m";

  double bwSigma[30];
  int mass[30]; double xLow[30]; double xHigh[30];  
  int maxMassBin;

  do7TeV_=do7TeV;

  if(do7TeV) {
    mass[0] = 120; xLow[0] = 85; xHigh[0] = 180; bwSigma[0] = 3.5/1000.;
    mass[1] = 130; xLow[1] = 85; xHigh[1] = 190; bwSigma[1] = 4.9/1000.;
    mass[2] = 140; xLow[2] = 85; xHigh[2] = 190; bwSigma[2] = 8.1/1000.;
    mass[3] = 150; xLow[3] = 85; xHigh[3] = 220; bwSigma[3] = 1.7/100.;
    mass[4] = 160; xLow[4] = 85; xHigh[4] = 220; bwSigma[4] = 8.3/100.;
    mass[5] = 170; xLow[5] = 85; xHigh[5] = 240; bwSigma[5] = 3.8/10.;
    mass[6] = 180; xLow[6] = 85; xHigh[6] = 250; bwSigma[6] = 6.3/10.;
    maxMassBin = 7;    
  } else {
    mass[0] = 115; xLow[0] = 80; xHigh[0] = 170; bwSigma[0] = 3.1/1000.;
    mass[1] = 120; xLow[1] = 80; xHigh[1] = 180; bwSigma[1] = 3.5/1000.;
    mass[2] = 125; xLow[2] = 80; xHigh[2] = 180; bwSigma[2] = 4.1/1000.;
    mass[3] = 130; xLow[3] = 80; xHigh[3] = 190; bwSigma[3] = 4.9/1000.;
    mass[4] = 135; xLow[4] = 80; xHigh[4] = 190; bwSigma[4] = 4.9/1000.;
    mass[5] = 140; xLow[5] = 80; xHigh[5] = 190; bwSigma[5] = 8.1/1000.;
    mass[6] = 150; xLow[6] = 80; xHigh[6] = 220; bwSigma[6] = 1.7/100.;
    mass[7] = 160; xLow[7] = 80; xHigh[7] = 220; bwSigma[7] = 8.3/100.;
    mass[8] = 170; xLow[8] = 80; xHigh[8] = 240; bwSigma[8] = 3.8/10.;
    mass[9] = 180; xLow[9] = 80; xHigh[9] = 250; bwSigma[9] = 6.3/10.;
    maxMassBin = 10;
  }

  double fitValues[5];
  double fitErrors[5];

  for(int i=0; i<maxMassBin;++i){

    cout << e1 << "==> Fitting H->WW sample with mH = " << mass[i] << " ..." << en << endl;

    ofstream fileParams;
    stringstream filename;
    filename << "sigParamsHWW_mH" << mass[i] << ".txt";
    
    fileParams.open(filename.str().c_str());
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
    
    for(int s=0; s<(int)systematics.size(); ++s) {
      cout << e0 << "%%% Parameterising samples with systematic " << systematics[s] << " ON..." << en << endl;
      fileParams << "# qq->H->WW pdfs with systematics: " << systematics[s] << endl;
      for(int ch=0; ch<4; ++ch) fileParams << fitSignalShapeMR(mass[i],ch,xLow[i],xHigh[i],bwSigma[i],fitValues,fitErrors,systematics[s]);
    }

  }

}

std::string fitSignalShapeMR(int massBin, int channel, 
                             double rangeLow, double rangeHigh,
                             double bwSigma,
                             double fitValues[5], double fitErrors[5], string syst){
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

  YieldMaker  ymaker_hi;

  stringstream hFileName1, hFileName2, hFileName3;
  if(!do7TeV_) hFileName1 << "latinos_tree_skim_of/"+dir+"/latino_1" << massBin << "_ggToH" << massBin << "toWWTo2LAndTau2Nu.root";
  else {
    hFileName1 << "latinos_tree_skim_of/"+dir+"/latino_1" << massBin << "_ggToH" << massBin << "toWWto2L2Nu.root";
    hFileName2 << "latinos_tree_skim_of/"+dir+"/latino_2" << massBin << "_ggToH" << massBin << "toWWtoLNuTauNu.root";
    hFileName3 << "latinos_tree_skim_of/"+dir+"/latino_3" << massBin << "_ggToH" << massBin << "toWWto2Tau2Nu.root";
  }
  cout << "ggH ==> Opening ROOT file: " << hFileName1.str() << endl;
  ymaker_hi.fill(hFileName1.str().c_str());
  if(do7TeV_) {
    cout << "ggH ==> Opening ROOT file: " << hFileName2.str() << endl;
    ymaker_hi.fill(hFileName2.str().c_str());
    cout << "ggH ==> Opening ROOT file: " << hFileName3.str() << endl;
    ymaker_hi.fill(hFileName3.str().c_str());
  }

  FitSelection sel;

  //--- rooFit part
  double xMin,xMax,xInit;
  xInit = (double) massBin;
  xMin = rangeLow;
  xMax = rangeHigh ;

  RooRealVar x("mr","M_{R}",xInit,xMin,xMax,"GeV");
  RooRealVar w("weight","weight",1.0,0.,1000.);

  RooArgSet varset(x,w,"argset_obs");
  RooDataSet dataset("dataset", "dataset", varset, WeightVar("weight"));
  ymaker_hi.getDataSet1D(channel, xMin, xMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, dataset, x, w);

  //--- simple CrystalBall
  RooRealVar mean("mean","mean of gaussian",0,-20.0,20.0) ;
  RooRealVar sigma("sigma","width of gaussian",15,10.,50.); 
  RooRealVar a("a","a",1.46,0.,10.);
  //  RooRealVar n("n","n",1.92,0.,20.);   
  RooRealVar n("n","n",5);
  RooCBShape CBall("CBall","Crystal ball",x, mean,sigma, a,n);
  

  //--- simple Gaussian
  RooRealVar mean2("mean2","mean2 of gaussian",xInit,xInit*0.80,xInit*1.2) ;
  RooRealVar sigma2("sigma2","width2 of gaussian",10.,5.,500.); 
  RooGaussian tailCatcher("tailCatcher","tailCatcher",x,mean2,sigma2);
  RooRealVar fsig("fsig","signal fraction",0.95,0.7,1.);


  //--- Breit-Wigner
  RooRealVar mean3("mean3","mean3",xInit) ;
  RooRealVar sigma3("sigma3","width3",bwSigma); 
  RooRealVar scale3("scale3","scale3 ",1.); 
  RooBreitWigner bw("bw","bw",x,mean3,sigma3);

  //RooAddPdf model("model","model",RooArgList(CBall,tailCatcher),fsig);
  //RooCBShape model("model","model",x, mean,sigma, a,n);
  x.setBins(100000,"fft");
  RooFFTConvPdf model("model","model",x,bw,CBall);
  model.setBufferFraction(0.2);

  model.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));

  stringstream frameTitle;
  if(channel==of0j){frameTitle << "e#mu,0-j, m_{H} = ";}
  if(channel==of1j){frameTitle << "e#mu,1-j, m_{H} = ";}
  if(channel==sf0j){frameTitle << "ee+#mu#mu,0-j, m_{H} = ";}
  if(channel==sf1j){frameTitle << "ee+#mu#mu,1-j, m_{H} = ";}
  frameTitle << massBin << " GeV";

  int col;
  if(channel==of0j) col=kOrange+7;
  if(channel==of1j) col=kAzure+2;
  if(channel==sf0j) col=kGreen+3;
  if(channel==sf1j) col=kViolet+3;

  TCanvas *c1 = new TCanvas("c1","c1",725,725);
  c1->cd();

  RooPlot* xframe = x.frame(xMin,xMax,50) ;
  xframe->SetTitle("");
  dataset.plotOn(xframe,DataError(RooAbsData::SumW2), MarkerStyle(kOpenCircle), MarkerSize(1.1) );
  model.plotOn(xframe,LineColor(col));
  //  model.paramOn(xframe, LineColor(col));

  // cosmetics
  TLegend *legend = new TLegend(0.20,0.45,0.45,0.60,NULL,"brNDC");
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.03);

  TH1F *dummyPoints = new TH1F("dummyP","dummyP",1,0,1);
  TH1F *dummyLine = new TH1F("dummyL","dummyL",1,0,1);
  dummyPoints->SetMarkerStyle(kOpenCircle);
  dummyPoints->SetMarkerSize(1.1);
  dummyLine->SetLineColor(col);
  
  legend->AddEntry(dummyPoints, "Simulation", "pe");
  legend->AddEntry(dummyLine, "Parametric Model", "l");
  

//   RooArgSet selParms(sigma);
//   model.paramOn(xframe,Parameters(selParms));

  TPaveText *text = new TPaveText(0.15,0.90,0.77,0.98,"brNDC");
  text->AddText("CMS Simulation");
  text->SetBorderSize(0);
  text->SetFillStyle(0);
  text->SetTextAlign(12);
  text->SetTextFont(42);
  text->SetTextSize(0.03);

  TPaveText *titlet = new TPaveText(0.15,0.80,0.60,0.85,"brNDC");
  titlet->AddText(frameTitle.str().c_str());
  titlet->SetBorderSize(0);
  titlet->SetFillStyle(0);
  titlet->SetTextAlign(12);
  titlet->SetTextFont(132);
  titlet->SetTextSize(0.045);

  TPaveText *sigmat = new TPaveText(0.15,0.65,0.77,0.78,"brNDC");
  stringstream sigmaval0, sigmaval1;
  sigmaval0 << fixed;
  sigmaval0 << setprecision(1);
  sigmaval0 << "m_{CB} = " << mean.getVal() + massBin << " GeV";
  sigmaval1 << fixed;
  sigmaval1 << setprecision(1);
  sigmaval1 << "#sigma_{CB} = " << sigma.getVal() << " GeV";
  sigmat->AddText(sigmaval0.str().c_str());
  sigmat->AddText(sigmaval1.str().c_str());
  sigmat->SetBorderSize(0);
  sigmat->SetFillStyle(0);
  sigmat->SetTextAlign(12);
  sigmat->SetTextFont(132);
  sigmat->SetTextSize(0.04);
  
  xframe->GetYaxis()->SetTitleOffset(1.5);


  stringstream nameFile;
  nameFile << "fitM" << massBin << "_" << getChannelSuffix(channel) << (do7TeV_ ? "_7TeV" : "_8TeV") << ".pdf";
  xframe->Draw(); gPad->Update(); 
  // legend->Draw(); 
  text->Draw(); sigmat->Draw(); titlet->Draw();
  gPad->Print(nameFile.str().c_str());

  if(fitValues!=0){
    fitValues[0] = a.getVal();
    fitValues[1] = mean.getVal();
    fitValues[2] = mean3.getVal();
    fitValues[3] = n.getVal();
    fitValues[4] = sigma.getVal();
  }

  if(fitErrors!=0){
    fitErrors[0] = a.getError();
    fitErrors[1] = mean.getError();
    fitErrors[2] = mean3.getError();
    fitErrors[3] = n.getError();
    fitErrors[4] = sigma.getError();
  }

  stringstream info;
  info << getStringFitChannel(channel) << "_sig_me_" << syst
       << " = " << mean.getVal() << " +/- " << mean.getError() << endl;
  info << getStringFitChannel(channel) << "_sig_si_" << syst 
       << " = " << sigma.getVal() << " +/- " << sigma.getError() << endl;
  info << getStringFitChannel(channel) << "_sig_alpha_" << syst 
       << " = " << a.getVal() << " +/- " << a.getError() << endl;
  info << getStringFitChannel(channel) << "_sig_n_" << syst 
       << " = " << n.getVal() << " +/- " << n.getError() << endl;

  return info.str();

}

