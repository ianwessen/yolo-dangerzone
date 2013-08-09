#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <string>
#include <iostream>

void drawOne(const char* dir, const char* var) {

  gStyle->SetOptStat(0);

  TFile weighted("Histograms/WW160Histograms-kFactor.root");
  TFile unweighted("Histograms/WW160Histograms-nokFactor.root");

  std::string nameH(dir);
  nameH+="/";
  nameH+=std::string(var);

  std::cout << "now taking " << nameH << std::endl;
  TH1F* weightedH = (TH1F*)weighted.Get(nameH.c_str());
  TH1F* unweightedH = (TH1F*)unweighted.Get(nameH.c_str());
  TH1F* kFactorH = (TH1F*)unweightedH->Clone("kFactor");
  kFactorH->Divide(weightedH,unweightedH,1,1);

  weightedH->SetMarkerStyle(8);

  weightedH->Rebin(3);
  unweightedH->Rebin(3);

  unweightedH->Scale(weightedH->Integral()/unweightedH->Integral());
  float max = std::max(weightedH->GetMaximum(),unweightedH->GetMaximum());
  weightedH->SetMaximum(max+sqrt(max));

  TCanvas c0;
  weightedH->Draw("pe1");
  unweightedH->Draw("same");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(weightedH,"H #rightarrow WW #rightarrow 2e2#nu, kFactors applied","p");
  leg->AddEntry(unweightedH,"H #rightarrow WW #rightarrow 2e2#nu, no kFactors applied","f");
  
  leg->Draw();

  std::string filename(var);
  filename+=".eps";
  c0.SaveAs(filename.c_str());

  if(strcmp("genHiggsPt_All",var)==0) {
    kFactorH->Draw();
    c0.SaveAs("kFactor.eps");
  }

}

void drawAll() {

  drawOne("Generator","highestPtGenEle_All");
  drawOne("Generator","lowestPtGenEle_All");
  drawOne("Generator","genHiggsPt_All");
  drawOne("EventAfterReco","deltaPhi_All");
  drawOne("EventAfterReco","mll_All");
  drawOne("EventAfterReco","highestPtEle_All");
  drawOne("EventAfterReco","lowestPtEle_All");
  drawOne("MET","metEt_All");

}
