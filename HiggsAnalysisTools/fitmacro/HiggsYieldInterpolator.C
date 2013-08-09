#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooGenericPdf.h>
#include <RooPlot.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooWorkspace.h>
#include <RooLandau.h>
#include <RooBreitWigner.h>
#include <RooCBShape.h>
#include <RooFFTConvPdf.h>
#include <RooProdPdf.h>
#include <RooHistFunc.h>
#include <TGraphErrors.h>
#include <TStyle.h>
#include <TF1.h>
#include <TPad.h>

#include "YieldMaker.h"
#include "FitSelection.hh"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

enum prodmode {gg, vbf, wztt};

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

float getYield(int mH, int cha, int prod, float lumi, bool barecount, bool do7TeV);
float getYieldStatError(float yield, float n) { return yield/sqrt(n); }
void allmasses(int prod, int cha, float lumi, bool do7TeV);

void all(int lumi, bool do7TeV) {
  for(int i=0;i<4;++i) {
    for(int j=0;j<3;++j) {
      allmasses(j,i,lumi,do7TeV);
    }
  }
}

void allmasses(int prod, int cha, float lumi, bool do7TeV) {
  int mass[30];
  float yieldV[30], yieldE[30];
  float massV[30], massE[30];
  int maxMassBin;

  if(do7TeV) {
    mass[0] = 120;
    mass[1] = 130;
    mass[2] = 140;
    mass[3] = 150;
    mass[4] = 160;
    mass[5] = 170;
    mass[6] = 180;
    maxMassBin = 7;
  } else {
    mass[0] = 115;
    mass[1] = 120;
    mass[2] = 125;
    mass[3] = 130;
    mass[4] = 135;
    mass[5] = 140;
    mass[6] = 150;
    mass[7] = 160;
    mass[8] = 170;
    mass[9] = 180;
    maxMassBin = 10;
  }

  for(int i=0;i<maxMassBin;++i) {
    yieldV[i] = getYield(mass[i],cha,prod,lumi,false,do7TeV);
    // stat error is negligible
    // float staterr = getYieldStatError(yieldV[i],getYield(mass[i],cha,prod,lumi,true));
    float systerr = 0.15 *  yieldV[i];
    yieldE[i] = systerr;
    massV[i] = mass[i];
    massE[i]=0;
  }

  TGraphErrors* gY = new TGraphErrors(maxMassBin,massV,yieldV,massE,yieldE);
  gY->SetMarkerStyle(20);   gY->SetMarkerSize(1);
  gY->SetTitle("");
  gY->GetXaxis()->SetTitle("mass (GeV)");
  gY->GetYaxis()->SetTitle("signal yield");

  gStyle->SetOptFit(111111);
  stringstream nameFile;
  nameFile << "higgsYield_" <<  getChannelSuffix(cha);
  if(prod==gg) nameFile << "_ggH";
  else if(prod==vbf) nameFile << "_vbfH";
  else if(prod==wztt) nameFile << "_wzttH";
  nameFile << (do7TeV ? "_7TeV" : "_8TeV");
  nameFile << "_lumi" << lumi << "invfb";
  gY->Fit("pol3"); gY->Draw("Ap"); gPad->Update(); gPad->Print((nameFile.str()+string(".pdf")).c_str());

  TF1 *fY = (TF1*)gY->GetFunction("pol3");
  std::stringstream ss;
  
  for (int i = 0; i < fY->GetNumberFreeParameters(); i++) {
    if (i != 0) ss << " + (";
    else ss << "(";
    ss << fY->GetParameter(i);
    for (int j = 0; j < i; j++) {
      ss << "*@0";
    }
    ss << ")";
  }
  cout << "RooFormulaVar for signal yield = " << ss.str() << endl;
  Wait();

}

float getYield(int mH, int cha, int prod, float lumi, bool barecount, bool do7TeV) {
  stringstream hFileName1, hFileName2, hFileName3;
  
  string tevstr = (do7TeV ? "_7TeV/" : "_8TeV/");
  
  if(do7TeV) {
   if(prod==gg) {
     hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_1" << mH << "_ggToH" << mH << "toWWto2L2Nu.root";
     hFileName2 << "latinos_tree_skim_of"+tevstr+"nominals/latino_2" << mH << "_ggToH" << mH << "toWWtoLNuTauNu.root";
     hFileName3 << "latinos_tree_skim_of"+tevstr+"nominals/latino_3" << mH << "_ggToH" << mH << "toWWto2Tau2Nu.root";
   } else if(prod==vbf) hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_4" << mH << "_vbfToH" << mH << "toWWto2L2Nu.root";
   else if(prod==wztt) hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_3" << mH << "_wzttH" << mH << "ToWW.root";
} else {
   if(prod==gg) hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_1" << mH << "_ggToH" << mH << "toWWTo2LAndTau2Nu.root";
   else if(prod==vbf) hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_2" << mH << "_vbfToH" << mH << "toWWTo2LAndTau2Nu.root";
   else if(prod==wztt) hFileName1 << "latinos_tree_skim_of"+tevstr+"nominals/latino_3" << mH << "_wzttH" << mH << "ToWW.root";
 }

 FitSelection sel;

 YieldMaker ymaker_hi;
 cout << "Opening ROOT file: " << hFileName1.str() << endl;
 ymaker_hi.fill(hFileName1.str().c_str());
 if(do7TeV && prod==gg) {
   ymaker_hi.fill(hFileName2.str().c_str());
   ymaker_hi.fill(hFileName3.str().c_str());
 }

 float yield = 0.0;
 if(barecount) yield = ymaker_hi.getCount(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,sel.ptllmin,sel.ptllmax); 
 else yield = ymaker_hi.getYield(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,sel.ptllmin,sel.ptllmax) * lumi;

 return yield;
}


