#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <string>
#include <iostream>

void drawOne(const char* dir, const char* var) {

  gStyle->SetOptStat(0);

  TFile qqH160("Histograms/histogramsqqH160.root");
  TFile Wjets2080("Histograms/histogramsWjets2080.root");
  TFile ttbar("Histograms/histogramsTTbar.root");

  std::string nameH(dir);
  nameH+="/";
  nameH+=std::string(var);

  std::cout << "now taking " << nameH << std::endl;
  TH1F* qqH160H = (TH1F*)qqH160.Get(nameH.c_str());
  TH1F* Wjets2080H = (TH1F*)Wjets2080.Get(nameH.c_str());
  TH1F* ttbarH = (TH1F*)ttbar.Get(nameH.c_str());

  qqH160H->SetFillColor(38);
  Wjets2080H->SetLineColor(2);
  ttbarH->SetLineColor(3);

  Wjets2080H->Scale( (float)qqH160H->Integral()/(float)Wjets2080H->Integral() );
  ttbarH->Scale( (float)qqH160H->Integral()/(float)ttbarH->Integral() );

  float max = std::max(qqH160H->GetMaximum(),std::max(Wjets2080H->GetMaximum(),ttbarH->GetMaximum()));

  qqH160H->SetMaximum(max+sqrt(max));

  TCanvas c0;
  qqH160H->Draw();
  Wjets2080H->Draw("same");
  ttbarH->Draw("same");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(qqH160H,"Signal H #rightarrow WW #rightarrow 2e2#nu","f");
  leg->AddEntry(Wjets2080H,"W(#rightarrow e#nu)+jets, p_{T}=20-80 GeV/c","l");
  leg->AddEntry(ttbarH,"t #bar t (2e2#nu b #bar b)","l");
  
  leg->Draw();

  std::string filename(var);
  filename+=".eps";
  c0.SaveAs(filename.c_str());

}

void drawAll() {

  drawOne("EventAfterReco","nEle_All");
  drawOne("EventAfterReco","nJets_All");
  drawOne("EventAfterReco","highestPtEle_All");
  drawOne("EventAfterReco","lowestPtEle_All");
  drawOne("EventAfterReco","deltaPhi_All");
  drawOne("EventAfterReco","WWtrMass_All");
  drawOne("EventAfterReco","mll_All");
  drawOne("MET","metEt_All");


}
