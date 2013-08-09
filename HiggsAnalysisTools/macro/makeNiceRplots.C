#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TAxis.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TLegend.h>

void makeNiceRPlots() {

  gStyle->SetOptStat(0);
  gROOT->SetStyle("Plain");

  int masses[10] = {0,115,120,130,140,150,160,170,180,190};
  
  for(int i=0;i<10;i++) {
    char filename[100];
    sprintf(filename,"R_mH%d.root",masses[i]);
    TFile *file = TFile::Open(filename);
    TCanvas *c1 = (TCanvas*)file->Get("c1");
    TH1F *RllData = (TH1F*)c1->GetPrimitive("RllData");
    TH1F *RllMC = (TH1F*)c1->GetPrimitive("RllMC");
    RllData->GetYaxis()->SetRangeUser(0,3);
    RllData->GetYaxis()->SetTitle("R_{out/in}");
    RllData->GetXaxis()->SetTitle("DY MVA bin");
    RllData->SetTitle("");
    RllMC->SetTitle("");

    TLegend* legend = new TLegend(0.2,
				  0.6,
				  0.2 + 0.2,
				  0.6 + 0.2);
    
    legend->SetBorderSize(     0);
    legend->SetFillColor (     0);
    legend->SetTextAlign (    12);
    legend->SetTextFont  (    42);
    legend->SetTextSize  (   0.05);

    legend->AddEntry(RllData, "R_{data}", "pl");
    legend->AddEntry(RllMC, "R_{MC}", "pl");
    
    TCanvas c2;
    RllData->Draw();
    RllMC->Draw("same");
    legend->Draw();
    
    TString newname(filename);
    newname.ReplaceAll(".root",".pdf");
    c2.SaveAs(newname.Data());
  }

}
