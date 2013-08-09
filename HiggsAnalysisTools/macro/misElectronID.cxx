#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>

void plotMisIDEta() {
  
  gStyle->SetOptStat(0);
  
  TFile file("Plots/W1jetTaP/eleMisidEta.root");
  
  TH1F *fakeReco_Eff = (TH1F*)file.Get("fakeRecoEta_Eff");
  TH1F *fakeIdentified_Eff = (TH1F*)file.Get("fakeIdentifiedEta_Eff");
  
  TCanvas c1;
  fakeReco_Eff->SetTitle("electron misidentification from W+1jet (Pythia, 0<p_{T}<300 GeV)");
  fakeReco_Eff->GetXaxis()->SetTitle("fake electron #eta");
  fakeReco_Eff->GetYaxis()->SetTitle("efficiency");
  fakeReco_Eff->SetMaximum(0.15);
  fakeReco_Eff->SetMinimum(-0.02);
  fakeReco_Eff->SetLineColor(1);
  fakeReco_Eff->SetMarkerStyle(8);
  fakeReco_Eff->SetMarkerSize(1);
  fakeIdentified_Eff->SetLineColor(3);
  fakeIdentified_Eff->SetMarkerColor(3);
  fakeIdentified_Eff->SetMarkerStyle(8);
  fakeIdentified_Eff->SetMarkerSize(1);

  fakeReco_Eff->Draw("pe1");
  fakeIdentified_Eff->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(fakeReco_Eff,"jet reconstruced as an electron","p");
  leg->AddEntry(fakeIdentified_Eff,"jet identified as an electron","p");
  leg->Draw();

  c1.SaveAs("misid_eta.eps");
  

}

void plotMisIDEtaElectronIDOnly() {
  
  gStyle->SetOptStat(0);
  TFile file("Plots/W1jetTaP/eleMisidEta.root");
  
  TH1F *fakeIdentified_EffWrtPrevious = (TH1F*)file.Get("fakeIdentifiedEta_EffWrtPrevious");

  TCanvas c1;
  fakeIdentified_EffWrtPrevious->SetTitle("electronID efficiency from W+1jet (Pythia, 0<p_{T}<300 GeV)");
  fakeIdentified_EffWrtPrevious->GetXaxis()->SetTitle("fake electron #eta");
  fakeIdentified_EffWrtPrevious->GetYaxis()->SetTitle("electron ID efficiency");
  fakeIdentified_EffWrtPrevious->SetMaximum(0.5);
  fakeIdentified_EffWrtPrevious->SetMinimum(-0.02);
  fakeIdentified_EffWrtPrevious->SetLineColor(3);
  fakeIdentified_EffWrtPrevious->SetMarkerColor(3);
  fakeIdentified_EffWrtPrevious->SetMarkerStyle(8);
  fakeIdentified_EffWrtPrevious->SetMarkerSize(1);

  fakeIdentified_EffWrtPrevious->Draw("pe1");

  c1.SaveAs("misid-eleIDonly_eta.eps");

}

void plotMisIDPt() {
  
  gStyle->SetOptStat(0);
  
  TFile file("Plots/W1jetTaP/eleMisidPt.root");
  
  TH1F *fakeReco_Eff = (TH1F*)file.Get("fakeRecoPt_Eff");
  TH1F *fakeIdentified_Eff = (TH1F*)file.Get("fakeIdentifiedPt_Eff");
  
  TCanvas c1;
  fakeReco_Eff->SetTitle("electron misidentification from W+1jet (Pythia, 0<p_{T}<300 GeV)");
  fakeReco_Eff->GetXaxis()->SetTitle("fake electron p_{T} (GeV/c)");
  fakeReco_Eff->GetYaxis()->SetTitle("efficiency");
  fakeReco_Eff->SetMaximum(0.15);
  fakeReco_Eff->SetMinimum(-0.02);
  fakeReco_Eff->SetLineColor(1);
  fakeReco_Eff->SetMarkerStyle(8);
  fakeReco_Eff->SetMarkerSize(1);
  fakeIdentified_Eff->SetLineColor(3);
  fakeIdentified_Eff->SetMarkerColor(3);
  fakeIdentified_Eff->SetMarkerStyle(8);
  fakeIdentified_Eff->SetMarkerSize(1);

  fakeReco_Eff->Draw("pe1");
  fakeIdentified_Eff->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(fakeReco_Eff,"jet reconstruced as an electron","p");
  leg->AddEntry(fakeIdentified_Eff,"jet identified as an electron","p");
  leg->Draw();

  c1.SaveAs("misid_pt.eps");
  

}

void plotMisIDPtElectronIDOnly() {
  
  gStyle->SetOptStat(0);
  TFile file("Plots/W1jetTaP/eleMisidPt.root");
  
  TH1F *fakeIdentified_EffWrtPrevious = (TH1F*)file.Get("fakeIdentifiedPt_EffWrtPrevious");

  TCanvas c1;
  fakeIdentified_EffWrtPrevious->SetTitle("electronID efficiency from W+1jet (Pythia, 0<p_{T}<300 GeV)");
  fakeIdentified_EffWrtPrevious->GetXaxis()->SetTitle("fake electron p_{T} (GeV/c)");
  fakeIdentified_EffWrtPrevious->GetYaxis()->SetTitle("electron ID efficiency");
  fakeIdentified_EffWrtPrevious->SetMaximum(0.5);
  fakeIdentified_EffWrtPrevious->SetMinimum(-0.02);
  fakeIdentified_EffWrtPrevious->SetLineColor(3);
  fakeIdentified_EffWrtPrevious->SetMarkerColor(3);
  fakeIdentified_EffWrtPrevious->SetMarkerStyle(8);
  fakeIdentified_EffWrtPrevious->SetMarkerSize(1);

  fakeIdentified_EffWrtPrevious->Draw("pe1");

  c1.SaveAs("misid-eleIDonly_pt.eps");

}


void all() {
  plotMisIDEta();
  plotMisIDEtaElectronIDOnly();
  plotMisIDPt();
  plotMisIDPtElectronIDOnly();
}
