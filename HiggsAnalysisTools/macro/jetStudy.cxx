#include "TFile.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <string>


void drawJetMatch() {

  gStyle->SetOptStat(0);

  TFile fileTTbar("Histograms/jetMatchFileTTbar.root");
  TFile fileHAllMasses("Histograms/jetMatchFileHAllMasses.root");

  // draw the matching fraction
  TH1F *MatchFracJetsTTbar = (TH1F*)fileTTbar.Get("MatchFracJets_pt");
  TH1F *MatchFracJetsH = (TH1F*)fileHAllMasses.Get("MatchFracJets_pt");
  
  MatchFracJetsTTbar->SetMarkerStyle(8);
  MatchFracJetsTTbar->SetMarkerColor(1);
  MatchFracJetsH->SetMarkerStyle(9);
  MatchFracJetsH->SetMarkerColor(2);
  MatchFracJetsH->SetLineColor(2);

  MatchFracJetsTTbar->SetMinimum(0.2);
  MatchFracJetsTTbar->SetMaximum(1.2);
  MatchFracJetsTTbar->GetXaxis()->SetTitle("E_{T}^{jet} (GeV)");
  MatchFracJetsTTbar->GetYaxis()->SetTitle("N jet matched / N reconstructed jets");
  MatchFracJetsTTbar->SetTitle("Geometrical match (#Delta R < 0.3)");

  TCanvas c1;
  MatchFracJetsTTbar->Draw("pe");
  MatchFracJetsH->Draw("same pe");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(MatchFracJetsTTbar,"jets in t #bar{t} sample","p");
  leg->AddEntry(MatchFracJetsH,"jets in H #rightarrow WW^{(*)} sample","p");

  leg->Draw("");
  c1.SaveAs("jetFakes.eps");

  
  // draw the leading jet spectrum
  TH1F *leadingSpectrumTTbar = (TH1F*)fileTTbar.Get("etHighestJet");
  TH1F *leadingSpectrumH = (TH1F*)fileHAllMasses.Get("etHighestJet");

  leadingSpectrumTTbar->SetLineColor(1);
  leadingSpectrumH->SetLineColor(2);
  leadingSpectrumTTbar->GetXaxis()->SetTitle("E_{T}^{jet}");
  leadingSpectrumTTbar->GetYaxis()->SetTitle("Normalized entries");
  leadingSpectrumTTbar->SetTitle("leading jet spectrum");

  leadingSpectrumH->Scale(leadingSpectrumTTbar->Integral()/leadingSpectrumTTbar->Integral());

  float max = TMath::Max(leadingSpectrumH->GetMaximum(),leadingSpectrumTTbar->GetMaximum());
  leadingSpectrumTTbar->SetMaximum(max+sqrt(max));

  leadingSpectrumTTbar->Draw();
  leadingSpectrumH->Draw("same");

  TLegend* leg2 = new TLegend(0.15,0.75,0.40,0.90);
  leg2->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg2->SetFillColor(0); 
  leg2->AddEntry(leadingSpectrumTTbar,"jets in t #bar{t} sample","p");
  leg2->AddEntry(leadingSpectrumH,"jets in H #rightarrow WW^{(*)} sample","p");

  leg->Draw();
  c1.SaveAs("jetSpectrum.eps");




  
}

void drawOneAlpha(const char* var, const char* title) {

  gStyle->SetOptStat(0);

  TFile fileTTbar("Histograms/histogramsTTbarExcludedJetPtLt20GeV.root");
  TFile fileHAllMasses("Histograms/histogramsHAllMassesExcludedJetPtLt20GeV.root");
  
  std::string variable(var);
  std::string electronVar = "Electrons/"+variable+"Ele_Best";
  std::string bestJetsVar = "Jets/"+variable+"Jet_Best";
  std::string fakeJetsVar = "Jets/"+variable+"Jet_Fake";

  TH1F *eleH = (TH1F*)fileHAllMasses.Get(electronVar.c_str());
  TH1F *bestJetsH = (TH1F*)fileTTbar.Get(bestJetsVar.c_str());
  TH1F *fakeJetsH = (TH1F*)fileTTbar.Get(fakeJetsVar.c_str());
  
  bestJetsH->Scale(eleH->Integral()/bestJetsH->Integral());
  fakeJetsH->Scale(eleH->Integral()/fakeJetsH->Integral());

  eleH->GetXaxis()->SetTitle(title);
  float max = TMath::Max(TMath::Max(eleH->GetMaximum(),bestJetsH->GetMaximum()),fakeJetsH->GetMaximum());
  eleH->SetMaximum(max+sqrt(max));
  eleH->SetTitle("Jets with E_{T}>20 GeV");

  eleH->SetLineColor(1);
  bestJetsH->SetLineColor(2);
  fakeJetsH->SetLineColor(3);

  TCanvas c1;
  c1.SetLogy(1);
  eleH->Draw();
  bestJetsH->Draw("same");
  fakeJetsH->Draw("same");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(eleH,"electrons from H #rightarrow WW^{(*)} decay","l");
  leg->AddEntry(bestJetsH,"MC - matched jets in t #bar{t} sample","l");
  leg->AddEntry(fakeJetsH,"fake jets in t #bar{t} sample","l");
  leg->Draw();

  std::string cname = variable + "-NoJetLt20GeV.eps";
    
  c1.SaveAs(cname.c_str());


}

void drawAlphas() {

  drawOneAlpha("alpha","#alpha parameter");
  drawOneAlpha("emFrac","e.m. fraction");

}
