#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TObjArray.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TF1.h"
#include "TH2D.h"
#include "TProfile2D.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "limitmacros/RooHZZStyle.C"

#include <sstream>
#include <iostream>

using namespace std;

TLatex *CMSPreliminary(float lumi7TeV=5.1, float lumi8TeV=19.6) {

  stringstream line;
  line << "CMS Preliminary  #sqrt{s}=7 TeV, L=" << lumi7TeV << " fb^{-1}  #sqrt{s}=8 TeV, L=" << lumi8TeV << " fb^{-1}";
  TLatex* CP = new TLatex(0.15,0.96, line.str().c_str());
  CP->SetNDC(kTRUE);
  CP->SetTextSize(0.032);

  return CP;

}

TPaveText *text(const char *txt, float x1, float y1, float x2, float y2) {
  TPaveText *text = new TPaveText(x1,y1,x2,y2,"brNDC");
  text->AddText(txt);
  text->SetBorderSize(0);
  text->SetFillStyle(0);
  text->SetTextAlign(12);
  text->SetTextFont(42);
  text->SetTextSize(0.05);
  return text;
}

void plot2DScan() {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->SetPalette(1);
  mystyle->cd();

  stringstream file78TeV;
  TFile *fit78TeV = TFile::Open("scan2d_fast.root");
  TTree *tree78TeV = (TTree*)fit78TeV->Get("limit");

  TProfile2D *poi2 = new TProfile2D("poi2","",50,115,150,40,0.0,2.0,0,20);
  poi2->GetXaxis()->SetTitle("M_{H} (GeV)");
  poi2->GetYaxis()->SetTitle("#sigma/#sigma_{SM}");
  poi2->GetZaxis()->SetTitle("-2 #Delta lnN");
  poi2->GetZaxis()->SetLabelFont(42);
  poi2->GetZaxis()->SetLabelOffset(0.007);
  poi2->GetZaxis()->SetLabelSize(0.045);
  poi2->GetZaxis()->SetTitleSize(0.05);
  poi2->GetZaxis()->SetTitleFont(42);

  float MH;
  float r;
  float deltaNLL;
  tree78TeV->SetBranchAddress("MH", &MH);
  tree78TeV->SetBranchAddress("r", &r);
  tree78TeV->SetBranchAddress("deltaNLL", &deltaNLL);
  
  for(int i=1; i<(int)tree78TeV->GetEntries();++i) {
    tree78TeV->GetEntry(i);
    if(2*deltaNLL>20) continue;
    poi2->Fill(MH,r,2*deltaNLL);
  }

   TCanvas *c1 = new TCanvas("c1", "",0,22,763,622);
   gStyle->SetOptStat(0);
   c1->Range(106.8241,-0.3755458,161.5748,2.122271);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.1493289);
   c1->SetRightMargin(0.2114094);
   c1->SetTopMargin(0.04895105);
   c1->SetBottomMargin(0.1503496);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);

  poi2->Draw("colz");

  TLatex *CP = CMSPreliminary();
  CP->Draw();

  TPaveText *comment;
  comment = text("H #rightarrow WW, 2D",0.20,0.90,0.40,0.90);

  c1->SaveAs("mh_scan2D_fast_hww.pdf");
  c1->SaveAs("mh_scan2D_fast_hww.png");

}

void plotScanByChannel(int ndim) {

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);

  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  stringstream file7TeV, file8TeV, file78TeV;
  file7TeV    << "higgsCombineSCAN" << ndim << "D_7TeV_FAST.MultiDimFit.mH125.8.root";
  file8TeV    << "higgsCombineSCAN" << ndim << "D_8TeV_FAST.MultiDimFit.mH125.8.root";
  file78TeV   << "higgsCombineSCAN" << ndim << "D_78TeV_FAST.MultiDimFit.mH125.8.root";

  TFile *fit7TeV = TFile::Open(file7TeV.str().c_str());
  TTree *tree7TeV = (TTree*)fit7TeV->Get("limit");

  TFile *fit8TeV = TFile::Open(file8TeV.str().c_str());
  TTree *tree8TeV = (TTree*)fit8TeV->Get("limit");

  TFile *fit78TeV = TFile::Open(file78TeV.str().c_str());
  TTree *tree78TeV = (TTree*)fit78TeV->Get("limit");

  TGraph *g7TeV = new TGraph();
  TGraph *g8TeV = new TGraph();
  TGraph *g78TeV = new TGraph();

  vector<TTree*> trees;
  trees.push_back(tree7TeV);
  trees.push_back(tree8TeV);
  trees.push_back(tree78TeV);

  vector<TGraph*> graphs;
  graphs.push_back(g7TeV);
  graphs.push_back(g8TeV);
  graphs.push_back(g78TeV);

  vector<string> runs;
  runs.push_back("7TeV");
  runs.push_back("8TeV");
  runs.push_back("7+8TeV");

  for(int cha=0; cha<(int)trees.size(); ++cha) {
    
    cout << "Analyzing scan for run period = " << runs[cha] << endl;

    float MH;
    float deltaNLL;
    trees[cha]->SetBranchAddress("MH", &MH);
    trees[cha]->SetBranchAddress("deltaNLL", &deltaNLL);

    if(cha<2) graphs[cha]->SetLineWidth(2);
    else graphs[cha]->SetLineWidth(4);

    for(int i=1; i<(int)trees[cha]->GetEntries();++i) {
       trees[cha]->GetEntry(i);
       graphs[cha]->SetPoint(i-1,MH,2*deltaNLL);
    }

  }

  graphs[0]->SetLineColor(kGreen+2);
  graphs[1]->SetLineColor(kRed+2);
  graphs[2]->SetLineColor(kBlue+2);

  TCanvas *c1 = new TCanvas("c1","",750,750);
  graphs[2]->GetXaxis()->SetRangeUser(110,140);
  graphs[2]->GetYaxis()->SetRangeUser(0,10);
  graphs[2]->GetXaxis()->SetTitle("m_{X} (GeV)");
  graphs[2]->GetYaxis()->SetTitle("-2 #Delta lnL");

  graphs[2]->Draw("al");
  //  for(int i=1;i<3;++i) graphs[i]->Draw("l");

  // draw the legend
  TLegend *legend = new TLegend(0.20,0.75,0.55,0.90,NULL,"brNDC");
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.05);
  legend->AddEntry(graphs[2], "H #rightarrow WW #rightarrow 2l2#nu, #mu=1","l");
  //  legend->AddEntry(graphs[0], "H #rightarrow WW #rightarrow 2l2#nu, #sqrt{s}=7 TeV ","l");
  //  legend->AddEntry(graphs[1], "H #rightarrow WW #rightarrow 2l2#nu, #sqrt{s}=8 TeV ","l");
  legend->Draw();

  TLine *line1 = new TLine(110,1,140,1);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(3.0);
  line1->Draw();

  TLine *line2 = new TLine(110,4,140,4);
  line2->SetLineColor(kRed);
  line2->SetLineWidth(1.5);
  line2->Draw();

  TLatex *CP = CMSPreliminary();
  CP->Draw();

  TPaveText *comment;
  if(ndim==1) comment = text("H #rightarrow WW, 1D",0.20,0.90,0.40,0.90);
  if(ndim==2) comment = text("H #rightarrow WW, 2D",0.20,0.90,0.40,0.90);
  //  comment->Draw();


  stringstream outnamepdf;
  outnamepdf << "scanmass_bychannel_" << ndim << "D.pdf";
  c1->SaveAs(outnamepdf.str().c_str());
  stringstream outnamepng;
  outnamepng << "scanmass_bychannel_" << ndim << "D.png";
  c1->SaveAs(outnamepng.str().c_str());
}

    
void cccPlot(int ndim) {

  float fitval[4], fiterrl[4], fiterrh[4];
  for(int cha=0; cha<4; ++cha) {
    fiterrl[cha]=0;
    fiterrh[cha]=0;
  }

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1111);
  
  TStyle *mystyle = RooHZZStyle("ZZ");
  mystyle->cd();

  stringstream file4mu,file4e,file2e2mu,filecomb;
  file4mu   << "higgsCombineSCAN" << ndim << "D4muFast.MultiDimFit.mH125.8.root";
  file4e    << "higgsCombineSCAN" << ndim << "D4eFast.MultiDimFit.mH125.8.root";
  file2e2mu << "higgsCombineSCAN" << ndim << "D2e2muFast.MultiDimFit.mH125.8.root";
  filecomb  << "higgsCombineSCAN" << ndim << "DcombFast.MultiDimFit.mH125.8.root";

  TFile *fit4mu = TFile::Open(file4mu.str().c_str());
  TTree *tree4mu = (TTree*)fit4mu->Get("limit");

  TFile *fit4e = TFile::Open(file4e.str().c_str());
  TTree *tree4e = (TTree*)fit4e->Get("limit");

  TFile *fit2e2mu = TFile::Open(file2e2mu.str().c_str());
  TTree *tree2e2mu = (TTree*)fit2e2mu->Get("limit");

  TFile *fitcomb = TFile::Open(filecomb.str().c_str());
  TTree *treecomb = (TTree*)fitcomb->Get("limit");

  vector<TTree*> trees;
  trees.push_back(tree4mu);
  trees.push_back(tree4e);
  trees.push_back(tree2e2mu);
  trees.push_back(treecomb);

  for(int cha=0; cha<(int)trees.size(); ++cha) {
    
    cout << "Analyzing scan for channel = " << cha << endl;

    float MH;
    float deltaNLL;
    trees[cha]->SetBranchAddress("MH", &MH);
    trees[cha]->SetBranchAddress("deltaNLL", &deltaNLL);
    
    bool firstcross=false;
    bool secondcross=false;
    float prevStepMH=-1;
    for(int i=0; i<(int)trees[cha]->GetEntries();++i) {
       trees[cha]->GetEntry(i);
       if(i==0) fitval[cha]=MH;
       else {
	 if(2*deltaNLL<1 && firstcross==false) {
	   fiterrl[cha]=MH;
	   firstcross=true;
	 }
	 if(2*deltaNLL>1 && firstcross==true && secondcross==false) {
	   fiterrh[cha]=prevStepMH;
	   secondcross=true;
	 }
	 prevStepMH=MH;
       }
    }

  }

  for(int cha=0; cha<4; ++cha) {
    fiterrl[cha]=fitval[cha]-fiterrl[cha];
    fiterrh[cha]=fiterrh[cha]-fitval[cha];
    // patch if the scan arrested too early
    if(fiterrh[cha]==(-fitval[cha])) fiterrh[cha]=fiterrl[cha];
      cout << "Mass for channel " << cha << " = " << fitval[cha] 
	   << " -" << fiterrl[cha] << " +" << fiterrh[cha] << " GeV" << endl;
  }


    TLatex l; l.SetTextFont(43); l.SetNDC(); l.SetTextSize(25);

    TCanvas *c1 = new TCanvas("c1","",750,750);
    c1->SetLeftMargin(0.4);
    c1->SetGridx(1);

    int nChann = 3;
    TH2F frame("frame",";best fit m_{X} (GeV);",1,122,132,nChann,0,nChann);

    TGraphAsymmErrors points(nChann);
    for (int cha=0; cha<3; ++cha) {
      TString channame("");
      if (cha==0) channame+=" 4#mu";
      if (cha==1) channame+=" 4e";
      if (cha==2) channame+=" 2e2#mu";
      points.SetPoint(cha,       fitval[cha],  cha+0.5);
      points.SetPointError(cha,  fiterrl[cha], fiterrh[cha], 0, 0);
      frame.GetYaxis()->SetBinLabel(cha+1, channame);
    }
    points.SetLineColor(kRed);
    points.SetLineWidth(3);
    points.SetMarkerStyle(21);
    frame.GetXaxis()->SetNdivisions(5,kFALSE);
    frame.GetXaxis()->SetTitleSize(0.05);
    frame.GetXaxis()->SetLabelSize(0.04);
    frame.GetYaxis()->SetLabelSize(0.06);
    frame.Draw(); gStyle->SetOptStat(0);
    TBox globalFitBand(fitval[3]-fiterrl[3], 0, fitval[3]+fiterrh[3], nChann);
    globalFitBand.SetFillStyle(3013);
    globalFitBand.SetFillColor(65);
    globalFitBand.SetLineStyle(0);
    globalFitBand.DrawClone();
    TLine globalFitLine(fitval[3], 0, fitval[3], nChann);
    globalFitLine.SetLineWidth(4);
    globalFitLine.SetLineColor(214);
    globalFitLine.DrawClone();
    points.Draw("P SAME");
    if(ndim==1) l.DrawLatex(0.45, 0.90, Form("H #rightarrow ZZ, 1D"));
    if(ndim==2) l.DrawLatex(0.45, 0.90, Form("H #rightarrow ZZ, 2D"));
    if(ndim==3) l.DrawLatex(0.45, 0.90, Form("H #rightarrow ZZ, 3D"));

    stringstream outnamepdf;
    outnamepdf << "bestfit_bychannel_" << ndim << "D.pdf";
    c1->SaveAs(outnamepdf.str().c_str());
    stringstream outnamepng;
    outnamepng << "bestfit_bychannel_" << ndim << "D.png";
    c1->SaveAs(outnamepng.str().c_str());

}
