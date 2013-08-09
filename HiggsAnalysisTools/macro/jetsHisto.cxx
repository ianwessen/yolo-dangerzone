#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <string>
#include <iostream>

void drawOne(const char* var) {

  gStyle->SetOptStat(0);

  TFile H160("Histograms/histogramsH160.root");

  std::string nameVar(var);
  std::string nameEleBest = nameVar + "Ele_Best";
  std::string nameEleFake = nameVar + "Ele_Fake";
  std::string nameJets = nameVar + "Jet_Best";
  
  H160.cd("Electrons");
  TH1F *Ele_Best = (TH1F*)gDirectory->Get(nameEleBest.c_str());
  TH1F *Ele_Fake = (TH1F*)gDirectory->Get(nameEleFake.c_str());
  // in the convention used, "Best" jets are the ones that have to be vetoed
  // "Fake" jets are the ones on which do not apply the central veto
  H160.cd("Jets");
  TH1F *Jets = (TH1F*)gDirectory->Get(nameJets.c_str());
  
  if (!Ele_Best || !Ele_Fake || !Jets) return;

  Ele_Best->SetLineColor(1);
  Ele_Fake->SetLineColor(12);
  Jets->SetLineColor(2);

  Ele_Best->Scale(Jets->Integral()/Ele_Best->Integral());
  Ele_Fake->Scale(Jets->Integral()/Ele_Fake->Integral());

  float max = std::max(Ele_Best->GetMaximum(),Jets->GetMaximum() );

  Jets->SetMaximum(max+sqrt(max));

  TCanvas c0;
  Jets->Draw();
  Ele_Best->Draw("same");
  //  Ele_Fake->Draw("same");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
  leg->SetFillColor(0); 
  leg->AddEntry(Jets,"Jets to be vetoed","l");
  leg->AddEntry(Ele_Best,"selected electrons","l");
  //  leg->AddEntry(Ele_Fake,"discarded electrons","l");
  
  leg->Draw();

  std::string filename(var);
  filename+=".eps";
  c0.SaveAs(filename.c_str());

}

void drawAll() {

  gStyle->SetOptStat(0);

  drawOne("emFrac");
  drawOne("alpha");


}
