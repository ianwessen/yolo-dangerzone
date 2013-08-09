// basic kine variable distributions
// g++ plotsKineVariab.cpp -o kine `root-config --libs --cflags --glibs` -Wno-deprecated

//! c++ includes              
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream.h>
#include <unistd.h>
#include <fstream>
#include <math.h>

//! ROOT includes        
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TTree.h"
#include "TF1.h"
#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include "TLegend.h"
#include "TSelector.h"
#include "TApplication.h"

// Main program
int main ( int argc, char **argv)
{ 
  // reading the list of input files with trees and building the chain
  /*
  TChain *T[6];
  for(int uu=0; uu<6; uu++){ T[uu] = new TChain("data"); }
  T[0]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh130.txt.root");
  T[1]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh140.txt.root");
  T[2]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh150.txt.root");
  T[3]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh160.txt.root");
  T[4]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh170.txt.root");
  T[5]->Add("/afs/cern.ch/user/c/crovelli/scratch0/HiggsAnalysisTools_cvs/HiggsAnalysisTools/nTupleAtPreselLevel/input_mh180.txt.root");
  */
  TChain *T[5];
  for(int uu=0; uu<5; uu++){ T[uu] = new TChain("T1"); }
//   T[0]->Add("/cmsrm/pc18/emanuele/HiggsAnalRoot/FinalDatasets/H160Reco.root");
//   T[1]->Add("/cmsrm/pc18/emanuele/HiggsAnalRoot/FinalDatasets/WW160Reco.root");
//   T[2]->Add("/cmsrm/pc18/emanuele/HiggsAnalRoot/FinalDatasets/ttbar160Reco.root");

  T[0]->Add("presel_redTree/tree_160.root");
  T[1]->Add("presel_redTree/tree_ttbar.root");
  T[2]->Add("presel_redTree/tree_ww.root");
  T[3]->Add("presel_redTree/tree_DY.root");
  T[4]->Add("presel_redTree/tree_zz.root");

  // --------------------------------------------------
  // declaration of leaves types
  float met;  
  float deltaPhi;  
  float detaLeptons;  
  float eleInvMass;  
  float maxPtEle;  
  float minPtEle;  
  float transvMass;  

  // setting branch addresses
  //for(int uu=0; uu<6; uu++){ 
  for(int uu=0; uu<5; uu++){ 
    T[uu]->SetMakeClass(1);  
    T[uu]->SetBranchStatus("*",0);
    T[uu]->SetBranchStatus("met",1);
    T[uu]->SetBranchStatus("deltaPhi",1);
    T[uu]->SetBranchStatus("detaLeptons",1);
    T[uu]->SetBranchStatus("eleInvMass",1);
    T[uu]->SetBranchStatus("maxPtEle",1);
    T[uu]->SetBranchStatus("minPtEle",1);
    T[uu]->SetBranchStatus("transvMass",1);
    T[uu]->SetBranchAddress("met",         &met);
    T[uu]->SetBranchAddress("deltaPhi",    &deltaPhi);
    T[uu]->SetBranchAddress("detaLeptons", &detaLeptons);
    T[uu]->SetBranchAddress("eleInvMass",  &eleInvMass);
    T[uu]->SetBranchAddress("maxPtEle",    &maxPtEle);
    T[uu]->SetBranchAddress("minPtEle",    &minPtEle);
    T[uu]->SetBranchAddress("transvMass",  &transvMass);
  }
  
  
  // ---------------------------------------------------------
  // histos
  char title[200];
  // TH1F *H_met[6], *H_dPhi[6], *H_maxPt[6], *H_minPt[6], *H_invMass[6];
  TH1F *H_met[5], *H_dPhi[5], *H_maxPt[5], *H_minPt[5], *H_invMass[5];
  // for(int uu=0; uu<6; uu++){
  for(int uu=0; uu<5; uu++){
    sprintf(title,"H_met[%d]");
    H_met[uu]     = new TH1F(title, title, 30, 0., 145.);
    H_met[0] -> Sumw2();

    sprintf(title,"H_dPhi[%d]");
    H_dPhi[uu]    = new TH1F(title, title, 30, 0., 180.); 
    H_dPhi[0] -> Sumw2();

    sprintf(title,"H_maxPt[%d]");
    H_maxPt[uu]   = new TH1F(title, title, 30, 0., 160.);
    H_maxPt[0] -> Sumw2();

    sprintf(title,"H_minPt[%d]");
    H_minPt[uu]   = new TH1F(title, title, 30, 0., 160.);
    H_minPt[0] -> Sumw2();

    sprintf(title,"H_invMass[%d]");
    H_invMass[uu] = new TH1F(title, title, 30, 0., 160.);
    H_invMass[0] -> Sumw2();

  }


  // -------------------------------------------------------------------------------------    
  // loop on the events
  //  for(int uu=0; uu<6; uu++){ 
  for(int uu=0; uu<5; uu++){ 

    float nEnt = T[uu]->GetEntries();
    cout << endl;    
    cout << "Total number of events in loop for sample " << uu << " is " << nEnt << endl; 
    cout << endl;
    
    for (int entry=0; entry<nEnt; entry++) { 
    
      // charging the value in the branches
      T[uu] -> GetEntry(entry);
      if (entry%1000 == 0){ cout << "entry = " << entry << endl; }

      if (minPtEle == 0){ continue; }

      // filling the histos
      H_met[uu]     -> Fill(met);
      H_dPhi[uu]    -> Fill(deltaPhi);
      H_maxPt[uu]   -> Fill(maxPtEle);
      H_minPt[uu]   -> Fill(minPtEle);
      H_invMass[uu] -> Fill(eleInvMass);
    }
  }

  // -------------------------------------------------------------------------------------    
  // rescaling for expected events after the preselections for mH=160GeV in 100 pb-1
  // note: these are not exactly pre-selections: but until MET nominal (~preselection for 160 GeV)
  float expEvents[5];
  expEvents[0]=4.7; //signal
  expEvents[1]=20.7; // ttbar
  expEvents[2]=4.2; // WW
  expEvents[3]=4.0; // DY
  expEvents[4]=0.2; // ZZ

  // for(int uu=0; uu<6; uu++){
  for(int uu=0; uu<5; uu++){
    double scaleMet     = 1./H_met[uu]    -> GetSum();
    double scaleDPhi    = 1./H_dPhi[uu]   -> GetSum();
    double scaleMaxPt   = 1./H_maxPt[uu]  -> GetSum();
    double scaleMinPt   = 1./H_minPt[uu]  -> GetSum();
    double scaleInvMass = 1./H_invMass[uu]-> GetSum();

    /*
    H_met[uu]     -> Sumw2();
    H_dPhi[uu]    -> Sumw2();
    H_maxPt[uu]   -> Sumw2();
    H_minPt[uu]   -> Sumw2();
    H_invMass[uu] -> Sumw2();
    */

    H_met[uu]     -> SetTitle("");
    H_dPhi[uu]    -> SetTitle("");
    H_maxPt[uu]   -> SetTitle("");
    H_minPt[uu]   -> SetTitle("");
    H_invMass[uu] -> SetTitle("");

    H_met[uu]     -> Scale(scaleMet * expEvents[uu] );
    H_dPhi[uu]    -> Scale(scaleDPhi * expEvents[uu] );
    H_maxPt[uu]   -> Scale(scaleMaxPt * expEvents[uu] );
    H_minPt[uu]   -> Scale(scaleMinPt * expEvents[uu] );
    H_invMass[uu] -> Scale(scaleInvMass * expEvents[uu] );

    H_met[uu]     -> SetLineWidth(2);
    H_dPhi[uu]    -> SetLineWidth(2);
    H_maxPt[uu]   -> SetLineWidth(2);
    H_minPt[uu]   -> SetLineWidth(2);
    H_invMass[uu] -> SetLineWidth(2);
    
    if (uu==0){
//       H_met[uu]     -> SetLineColor(30);
//       H_dPhi[uu]    -> SetLineColor(30);
//       H_maxPt[uu]   -> SetLineColor(30);
//       H_minPt[uu]   -> SetLineColor(30);
//       H_invMass[uu] -> SetLineColor(30);

      H_met[uu]     -> SetMarkerStyle(20);
      H_met[uu]     -> SetMarkerSize(1);

      H_dPhi[uu]     -> SetMarkerStyle(20);
      H_dPhi[uu]     -> SetMarkerSize(1);

      H_maxPt[uu]     -> SetMarkerStyle(20);
      H_maxPt[uu]     -> SetMarkerSize(1);

      H_minPt[uu]     -> SetMarkerStyle(20);
      H_minPt[uu]     -> SetMarkerSize(1);

      H_invMass[uu]     -> SetMarkerStyle(20);
      H_invMass[uu]     -> SetMarkerSize(1);

    }

    if (uu==1){
      
      H_met[uu]     -> SetFillColor(41);
      H_met[uu]     -> SetFillStyle(1001);
      H_met[uu]     -> SetLineStyle(0);

      H_dPhi[uu]     -> SetFillColor(41);
      H_dPhi[uu]     -> SetFillStyle(1001);
      H_dPhi[uu]     -> SetLineStyle(0);

      H_maxPt[uu]     -> SetFillColor(41);
      H_maxPt[uu]     -> SetFillStyle(1001);
      H_maxPt[uu]     -> SetLineStyle(0);

      H_minPt[uu]     -> SetFillColor(41);
      H_minPt[uu]     -> SetFillStyle(1001);
      H_minPt[uu]     -> SetLineStyle(0);

      H_invMass[uu]     -> SetFillColor(41);
      H_invMass[uu]     -> SetFillStyle(1001);
      H_invMass[uu]     -> SetLineStyle(0);

    }

    if (uu==2){

      H_met[uu]     -> SetFillColor(50);
      H_met[uu]     -> SetFillStyle(1001);
      H_met[uu]     -> SetLineStyle(0);

      H_dPhi[uu]     -> SetFillColor(50);
      H_dPhi[uu]     -> SetFillStyle(1001);
      H_dPhi[uu]     -> SetLineStyle(0);

      H_maxPt[uu]     -> SetFillColor(50);
      H_maxPt[uu]     -> SetFillStyle(1001);
      H_maxPt[uu]     -> SetLineStyle(0);

      H_minPt[uu]     -> SetFillColor(50);
      H_minPt[uu]     -> SetFillStyle(1001);
      H_minPt[uu]     -> SetLineStyle(0);

      H_invMass[uu]     -> SetFillColor(50);
      H_invMass[uu]     -> SetFillStyle(1001);
      H_invMass[uu]     -> SetLineStyle(0);

    }

    if (uu==3){

      H_met[uu]     -> SetFillColor(31);
      H_met[uu]     -> SetFillStyle(1001);
      H_met[uu]     -> SetLineStyle(0);

      H_dPhi[uu]     -> SetFillColor(31);
      H_dPhi[uu]     -> SetFillStyle(1001);
      H_dPhi[uu]     -> SetLineStyle(0);

      H_maxPt[uu]     -> SetFillColor(31);
      H_maxPt[uu]     -> SetFillStyle(1001);
      H_maxPt[uu]     -> SetLineStyle(0);

      H_minPt[uu]     -> SetFillColor(31);
      H_minPt[uu]     -> SetFillStyle(1001);
      H_minPt[uu]     -> SetLineStyle(0);

      H_invMass[uu]     -> SetFillColor(31);
      H_invMass[uu]     -> SetFillStyle(1001);
      H_invMass[uu]     -> SetLineStyle(0);

    }

    if (uu==4){

      H_met[uu]     -> SetFillColor(49);
      H_met[uu]     -> SetFillStyle(1001);
      H_met[uu]     -> SetLineStyle(0);

      H_dPhi[uu]     -> SetFillColor(49);
      H_dPhi[uu]     -> SetFillStyle(1001);
      H_dPhi[uu]     -> SetLineStyle(0);

      H_maxPt[uu]     -> SetFillColor(49);
      H_maxPt[uu]     -> SetFillStyle(1001);
      H_maxPt[uu]     -> SetLineStyle(0);

      H_minPt[uu]     -> SetFillColor(49);
      H_minPt[uu]     -> SetFillStyle(1001);
      H_minPt[uu]     -> SetLineStyle(0);

      H_invMass[uu]     -> SetFillColor(49);
      H_invMass[uu]     -> SetFillStyle(1001);
      H_invMass[uu]     -> SetLineStyle(0);

    }


    /*
    H_met[uu]     -> SetMarkerStyle(8);
    H_dPhi[uu]    -> SetMarkerStyle(8);
    H_maxPt[uu]   -> SetMarkerStyle(8);
    H_minPt[uu]   -> SetMarkerStyle(8);
    H_invMass[uu] -> SetMarkerStyle(8);
    
    H_met[uu]     -> SetMarkerSize(1);
    H_dPhi[uu]    -> SetMarkerSize(1);
    H_maxPt[uu]   -> SetMarkerSize(1);
    H_minPt[uu]   -> SetMarkerSize(1);
    H_invMass[uu] -> SetMarkerSize(1);

    H_met[uu]     -> SetMarkerColor(uu+1);
    H_dPhi[uu]    -> SetMarkerColor(uu+1);
    H_maxPt[uu]   -> SetMarkerColor(uu+1);
    H_minPt[uu]   -> SetMarkerColor(uu+1);
    H_invMass[uu] -> SetMarkerColor(uu+1);

    */
  }

  // -------------------------------------------------------------------------------------    
  // histos: after preselections
  TApplication* theApp = new TApplication("App", &argc, argv);

//   TStyle *tesiStyle = new TStyle("tesiStyle","");
//   tesiStyle->SetCanvasColor(0);
//   tesiStyle->SetFrameFillColor(0);
//   tesiStyle->SetStatColor(0);
//   tesiStyle->SetOptStat(0);
//   tesiStyle->SetTitleFillColor(0);
//   tesiStyle->SetCanvasBorderMode(0);
//   tesiStyle->SetPadBorderMode(0);
//   tesiStyle->SetFrameBorderMode(0);
//   tesiStyle->cd();

//   gROOT->SetStyle("Plain");
//   gStyle->SetOptStat(0);
//   gStyle->SetOptTitle(0);
//   gStyle->SetErrorX(0);
  

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  
  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(2);
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

// For the Global title:

//  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

// Postscript options:
  // tdrStyle->SetPaperSize(15.,15.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();



  TLegend *leg = new TLegend(0.11,0.65,0.45,0.89);
  leg->SetBorderSize(2);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(H_met[0], "Signal, m_{H}=160 GeV","p");
  leg->AddEntry(H_met[2], "WW",   "f");
  leg->AddEntry(H_met[1], "tt~","f");
  leg->AddEntry(H_met[3], "Drell Yan","f");
  leg->AddEntry(H_met[4], "ZZ","f");

  TCanvas cMet("cMet","cMet");
  //  cMet.SetLogy(1);
  H_met[3]->SetMaximum(2.5);
  H_met[3]->Draw();
  // for(int uu=0; uu<6; uu++){
  for(int uu=1; uu<5; uu++){
    H_met[uu]->Draw("same");
    H_met[uu] -> SetLabelSize(0.035,"X");
    H_met[uu] -> SetLabelSize(0.035,"Y");
    H_met[uu] -> GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
    H_met[uu] -> GetYaxis()->SetTitle("normalized Events");
  }
  H_met[0]->Draw("E1 same");
  leg->Draw();

  TCanvas cDPhi("cDPhi","cDPhi");
  H_dPhi[0]->SetMaximum(1.5);
  H_dPhi[0]->Draw("E1");
  H_dPhi[0] -> GetXaxis()->SetTitle("#Delta #Phi");
  H_dPhi[0] -> GetYaxis()->SetTitle("normalized Events");
  // for(int uu=0; uu<6; uu++){
  for(int uu=1; uu<5; uu++){
    H_dPhi[uu]->Draw("same");
    H_dPhi[uu] -> SetLabelSize(0.035,"X");
    H_dPhi[uu] -> SetLabelSize(0.035,"Y");
    H_dPhi[uu] -> GetXaxis()->SetTitle("#Delta #Phi");
    H_dPhi[uu] -> GetYaxis()->SetTitle("normalized Events");
  }
  H_dPhi[0]->Draw("E1 same");
  leg->Draw();

//   TCanvas cMaxPt("cMaxPt","cMaxPt");
//   H_maxPt[0]->SetMaximum(2.0);
//   H_maxPt[0]->Draw("E1");
//   H_maxPt[0] -> GetXaxis()->SetTitle("p_{T}^{max} [GeV]");
//   H_maxPt[0] -> GetYaxis()->SetTitle("normalized Events");
//   // for(int uu=0; uu<6; uu++){
//   for(int uu=1; uu<5; uu++){
//     H_maxPt[uu]->Draw("same");
//     H_maxPt[uu] -> SetLabelSize(0.035,"X");
//     H_maxPt[uu] -> SetLabelSize(0.035,"Y");
//     H_maxPt[uu] -> GetXaxis()->SetTitle("p_{T}^{max} [GeV]");
//     H_maxPt[uu] -> GetYaxis()->SetTitle("normalized Events");
//   }
//   H_maxPt[0]->Draw("E1 same");
//   leg->Draw();

//   TCanvas cMinPt("cMinPt","cMinPt");
//   H_minPt[4]->SetMaximum(4.);
//   H_minPt[4]->Draw();
//   // for(int uu=0; uu<6; uu++){
//   for(int uu=1; uu<5; uu++){
//     H_minPt[uu]->Draw("same");
//     H_minPt[uu] -> SetLabelSize(0.035,"X");
//     H_minPt[uu] -> SetLabelSize(0.035,"Y");
//     H_minPt[uu] -> GetXaxis()->SetTitle("p_{T}^{min} [GeV]");
//     H_minPt[uu] -> GetYaxis()->SetTitle("normalized Events");
//   }
//   H_minPt[0]->Draw("E1 same");
//   leg->Draw();

//   TCanvas cInvMass("cInvMass","cInvMass");
//   H_invMass[4]->SetMaximum(2.);
//   H_invMass[4]->Draw();
//   // for(int uu=0; uu<6; uu++){
//   for(int uu=1; uu<5; uu++){
//     H_invMass[uu] -> Draw("same");
//     H_invMass[uu] -> SetLabelSize(0.035,"X");
//     H_invMass[uu] -> SetLabelSize(0.035,"Y");
//     H_invMass[uu] -> GetXaxis()->SetTitle("m_{ll} [GeV]");
//     H_invMass[uu] -> GetYaxis()->SetTitle("normalized Events");
//   }
//   H_invMass[0] -> Draw("E1 same");
//   leg->Draw();

  theApp->Run(kFALSE);

}  // end main ;


