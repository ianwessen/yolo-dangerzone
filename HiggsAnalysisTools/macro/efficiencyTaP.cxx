#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>
#include <TStyle.h>
#include <TLegend.h>


// single reconstruction cuts cuts on tags
void drawSingleRecoCutTagsEta() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/recoEleTaPEta.root";
  TFile file(fileName.c_str());

  TH1F *Htrigger_eta_EffWrtPrevious = (TH1F*)file.Get("Htrigger_eta_EffWrtPrevious");
  TH1F *Hiso_eta_EffWrtPrevious = (TH1F*)file.Get("Hiso_eta_EffWrtPrevious");
  TH1F *HeleID_eta_EffWrtPrevious = (TH1F*)file.Get("HeleID_eta_EffWrtPrevious");
  TH1F *Hpt_eta_EffWrtPrevious = (TH1F*)file.Get("Hpt_eta_EffWrtPrevious");

  Htrigger_eta_EffWrtPrevious->SetLineColor(1);
  Htrigger_eta_EffWrtPrevious->SetMarkerColor(1);

  Hiso_eta_EffWrtPrevious->SetLineColor(2);
  Hiso_eta_EffWrtPrevious->SetMarkerColor(2);
  
  HeleID_eta_EffWrtPrevious->SetLineColor(3);
  HeleID_eta_EffWrtPrevious->SetMarkerColor(3);

  Hpt_eta_EffWrtPrevious->SetLineColor(4);
  Hpt_eta_EffWrtPrevious->SetMarkerColor(4);

  Htrigger_eta_EffWrtPrevious->SetMaximum(1.1);
  Htrigger_eta_EffWrtPrevious->SetMinimum(0.3);
  Htrigger_eta_EffWrtPrevious->GetXaxis()->SetTitle("tag electron #eta");
  Htrigger_eta_EffWrtPrevious->GetYaxis()->SetTitle("efficiency");
  Htrigger_eta_EffWrtPrevious->SetTitle("efficiency w.r.t. previous cut on Z #rightarrow e^{+}e^{-} tags");  

  TCanvas c1;
  Htrigger_eta_EffWrtPrevious->Draw("pe1");
  Hiso_eta_EffWrtPrevious->Draw("same pe1");
  HeleID_eta_EffWrtPrevious->Draw("same pe1");
  Hpt_eta_EffWrtPrevious->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(Htrigger_eta_EffWrtPrevious,"HLT","pl");
  leg->AddEntry(Hiso_eta_EffWrtPrevious,"tracker isolation","pl");
  leg->AddEntry(HeleID_eta_EffWrtPrevious,"electron ID","pl");
  leg->AddEntry(Hpt_eta_EffWrtPrevious,"p_{T} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_tags_singlerecocut_Eta.eps";
  c1.SaveAs(psname.c_str());


}


void drawSingleRecoCutTagsPt() {

  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/recoEleTaPPt.root";
  TFile file(fileName.c_str());

  TH1F *Htrigger_pt_EffWrtPrevious = (TH1F*)file.Get("Htrigger_pt_EffWrtPrevious");
  TH1F *Hiso_pt_EffWrtPrevious = (TH1F*)file.Get("Hiso_pt_EffWrtPrevious");
  TH1F *HeleID_pt_EffWrtPrevious = (TH1F*)file.Get("HeleID_pt_EffWrtPrevious");
  TH1F *Hpt_pt_EffWrtPrevious = (TH1F*)file.Get("Hpt_pt_EffWrtPrevious");

  Htrigger_pt_EffWrtPrevious->SetLineColor(1);
  Htrigger_pt_EffWrtPrevious->SetMarkerColor(1);

  Hiso_pt_EffWrtPrevious->SetLineColor(2);
  Hiso_pt_EffWrtPrevious->SetMarkerColor(2);
  
  HeleID_pt_EffWrtPrevious->SetLineColor(3);
  HeleID_pt_EffWrtPrevious->SetMarkerColor(3);

  Hpt_pt_EffWrtPrevious->SetLineColor(4);
  Hpt_pt_EffWrtPrevious->SetMarkerColor(4);

  Htrigger_pt_EffWrtPrevious->SetMaximum(1.1);
  Htrigger_pt_EffWrtPrevious->SetMinimum(0.5);
  Htrigger_pt_EffWrtPrevious->GetXaxis()->SetTitle("tag electron p_{T} (GeV/c)");
  Htrigger_pt_EffWrtPrevious->GetYaxis()->SetTitle("efficiency");
  Htrigger_pt_EffWrtPrevious->SetTitle("efficiency w.r.t. previous cut on Z #rightarrow e^{+}e^{-} tags");  

  TCanvas c1;
  Htrigger_pt_EffWrtPrevious->Draw("pe1");
  Hiso_pt_EffWrtPrevious->Draw("same pe1");
  HeleID_pt_EffWrtPrevious->Draw("same pe1");
  Hpt_pt_EffWrtPrevious->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(Htrigger_pt_EffWrtPrevious,"HLT","pl");
  leg->AddEntry(Hiso_pt_EffWrtPrevious,"tracker isolation","pl");
  leg->AddEntry(HeleID_pt_EffWrtPrevious,"electron ID","pl");
  leg->AddEntry(Hpt_pt_EffWrtPrevious,"p_{T} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_tags_singlerecocut_Pt.eps";
  c1.SaveAs(psname.c_str());


}







// reco cuts in sequence on tags
void drawSequenceRecoCutTagsEta() {

  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/recoEleTaPEta.root";
  TFile file(fileName.c_str());

  TH1F *Htrigger_eta_Eff = (TH1F*)file.Get("Htrigger_eta_Eff");
  TH1F *Hiso_eta_Eff = (TH1F*)file.Get("Hiso_eta_Eff");
  TH1F *HeleID_eta_Eff = (TH1F*)file.Get("HeleID_eta_Eff");
  TH1F *Hpt_eta_Eff = (TH1F*)file.Get("Hpt_eta_Eff");

  Htrigger_eta_Eff->SetLineColor(1);
  Htrigger_eta_Eff->SetMarkerColor(1);

  Hiso_eta_Eff->SetLineColor(2);
  Hiso_eta_Eff->SetMarkerColor(2);
  
  HeleID_eta_Eff->SetLineColor(3);
  HeleID_eta_Eff->SetMarkerColor(3);

  Hpt_eta_Eff->SetLineColor(4);
  Hpt_eta_Eff->SetMarkerColor(4);

  Htrigger_eta_Eff->SetMaximum(1.0);
  Htrigger_eta_Eff->SetMinimum(0.3);
  Htrigger_eta_Eff->GetXaxis()->SetTitle("tag electron #eta");
  Htrigger_eta_Eff->GetYaxis()->SetTitle("efficiency");
  Htrigger_eta_Eff->SetTitle("efficiency on Z #rightarrow e^{+}e^{-} tags");  

  TCanvas c1;
  Htrigger_eta_Eff->Draw("pe1");
  Hiso_eta_Eff->Draw("same pe1");
  HeleID_eta_Eff->Draw("same pe1");
  //  Hpt_eta_Eff->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(Htrigger_eta_Eff,"HLT","pl");
  leg->AddEntry(Hiso_eta_Eff,"tracker isolation","pl");
  leg->AddEntry(HeleID_eta_Eff,"electron ID","pl");
  //  leg->AddEntry(Hpt_eta_Eff,"p_{T} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_tags_recocut_Eta.eps";
  c1.SaveAs(psname.c_str());


  // errors

  TH1F *Htrigger_eta_Eff_Err = new TH1F("Htrigger_eta_Eff_Err","Htrigger_eta_Eff_Err",
					Htrigger_eta_Eff->GetNbinsX(),-2.5,2.5);
  TH1F *Hiso_eta_Eff_Err = new TH1F("Hiso_eta_Eff_Err","Hiso_eta_Eff_Err",
					Hiso_eta_Eff->GetNbinsX(),-2.5,2.5);
  TH1F *HeleID_eta_Eff_Err = new TH1F("HeleID_eta_Eff_Err","HeleID_eta_Eff_Err",
					HeleID_eta_Eff->GetNbinsX(),-2.5,2.5);


  for(int i=0; i<=Htrigger_eta_Eff_Err->GetNbinsX()+1; i++) {
    Htrigger_eta_Eff_Err->SetBinContent(i, Htrigger_eta_Eff->GetBinError(i) );
    Hiso_eta_Eff_Err->SetBinContent(i, Hiso_eta_Eff->GetBinError(i) );
             HeleID_eta_Eff_Err->SetBinContent(i, HeleID_eta_Eff->GetBinError(i) );
  }

  TLegend* leg2 = new TLegend(0.15,0.75,0.40,0.90);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
  leg2->SetFillColor(0);
  leg2->AddEntry(Htrigger_eta_Eff_Err,"HLT","l");
  leg2->AddEntry(Hiso_eta_Eff_Err,"tracker isolation","l");
  leg2->AddEntry(HeleID_eta_Eff_Err,"electron ID","l");

  Htrigger_eta_Eff_Err->SetTitle("");
  Htrigger_eta_Eff_Err->SetMaximum(0.01);
  Htrigger_eta_Eff_Err->SetMinimum(0.0);
  Htrigger_eta_Eff_Err->SetLineColor(1);
  Hiso_eta_Eff_Err->SetLineColor(2);
  HeleID_eta_Eff_Err->SetLineColor(3);
  Htrigger_eta_Eff_Err->GetXaxis()->SetTitle("tag electron #eta");
  Htrigger_eta_Eff_Err->GetYaxis()->SetTitle("efficiency uncertainty");

  Htrigger_eta_Eff_Err->Draw();
  Hiso_eta_Eff_Err->Draw("same");
  HeleID_eta_Eff_Err->Draw("same");
  leg2->Draw();


  c1.SaveAs("efferr_ZTaP_tags_recocut_Eta.eps");

}


void drawSequenceRecoCutTagsPt() {

  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/recoEleTaPPt.root";
  TFile file(fileName.c_str());

  TH1F *Htrigger_pt_Eff = (TH1F*)file.Get("Htrigger_pt_Eff");
  TH1F *Hiso_pt_Eff = (TH1F*)file.Get("Hiso_pt_Eff");
  TH1F *HeleID_pt_Eff = (TH1F*)file.Get("HeleID_pt_Eff");
  TH1F *Hpt_pt_Eff = (TH1F*)file.Get("Hpt_pt_Eff");

  Htrigger_pt_Eff->SetLineColor(1);
  Htrigger_pt_Eff->SetMarkerColor(1);

  Hiso_pt_Eff->SetLineColor(2);
  Hiso_pt_Eff->SetMarkerColor(2);
  
  HeleID_pt_Eff->SetLineColor(3);
  HeleID_pt_Eff->SetMarkerColor(3);

  Hpt_pt_Eff->SetLineColor(4);
  Hpt_pt_Eff->SetMarkerColor(4);

  Htrigger_pt_Eff->SetMaximum(1.1);
  Htrigger_pt_Eff->SetMinimum(0.3);
  Htrigger_pt_Eff->GetXaxis()->SetTitle("tag electron p_{T} (GeV/c)");
  Htrigger_pt_Eff->GetYaxis()->SetTitle("efficiency");
  Htrigger_pt_Eff->SetTitle("efficiency cut on Z #rightarrow e^{+}e^{-} tags");  

  TCanvas c1;
  Htrigger_pt_Eff->Draw("pe1");
  Hiso_pt_Eff->Draw("same pe1");
  HeleID_pt_Eff->Draw("same pe1");
  //  Hpt_pt_Eff->Draw("same pe1");

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(Htrigger_pt_Eff,"HLT","pl");
  leg->AddEntry(Hiso_pt_Eff,"tracker isolation","pl");
  leg->AddEntry(HeleID_pt_Eff,"electron ID","pl");
  //  leg->AddEntry(Hpt_pt_Eff,"p_{T} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_tags_recocut_Pt.eps";
  c1.SaveAs(psname.c_str());

  // errors
  TH1F *Htrigger_pt_Eff_Err = new TH1F("Htrigger_pt_Eff_Err","Htrigger_pt_Eff_Err",
                                        Htrigger_pt_Eff->GetNbinsX(),15.,100.);
  TH1F *Hiso_pt_Eff_Err = new TH1F("Hiso_pt_Eff_Err","Hiso_pt_Eff_Err",
				    Hiso_pt_Eff->GetNbinsX(),15.,100.);
  TH1F *HeleID_pt_Eff_Err = new TH1F("HeleID_pt_Eff_Err","HeleID_pt_Eff_Err",
				      HeleID_pt_Eff->GetNbinsX(),15.,100.);

  
  for(int i=0; i<=Htrigger_pt_Eff_Err->GetNbinsX()+1; i++) {
    Htrigger_pt_Eff_Err->SetBinContent(i, Htrigger_pt_Eff->GetBinError(i) );
    Hiso_pt_Eff_Err->SetBinContent(i, Hiso_pt_Eff->GetBinError(i) );
    HeleID_pt_Eff_Err->SetBinContent(i, HeleID_pt_Eff->GetBinError(i) );
  }

  TLegend* leg2 = new TLegend(0.15,0.75,0.40,0.90);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0); leg2->SetTextSize(0.03);
  leg2->SetFillColor(0);
  leg2->AddEntry(Htrigger_pt_Eff_Err,"HLT","l");
  leg2->AddEntry(Hiso_pt_Eff_Err,"tracker isolation","l");
  leg2->AddEntry(HeleID_pt_Eff_Err,"electron ID","l");


  Htrigger_pt_Eff_Err->SetTitle("");
  Htrigger_pt_Eff_Err->SetMaximum(0.02);
  Htrigger_pt_Eff_Err->SetMinimum(0.0);
  Htrigger_pt_Eff_Err->SetLineColor(1);
  Hiso_pt_Eff_Err->SetLineColor(2);
  HeleID_pt_Eff_Err->SetLineColor(3);
  Htrigger_pt_Eff_Err->GetXaxis()->SetTitle("tag electron #eta");
  Htrigger_pt_Eff_Err->GetYaxis()->SetTitle("efficiency uncertainty");

  Htrigger_pt_Eff_Err->Draw();
  Hiso_pt_Eff_Err->Draw("same");
  HeleID_pt_Eff_Err->Draw("same");
  leg2->Draw();

  c1.SaveAs("efferr_ZTaP_tags_recocut_Pt.eps");


}






// single eleID cuts on probes 
void drawSingleEleIdCutProbesEta() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPEta.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeHoEOkEta_EffWrtPrevious = (TH1F*)file.Get("H_probeHoEOkEta_EffWrtPrevious");
  TH1F *H_probeS9S25OkEta_EffWrtPrevious = (TH1F*)file.Get("H_probeS9S25OkEta_EffWrtPrevious");
  TH1F *H_probedEtaOkEta_EffWrtPrevious = (TH1F*)file.Get("H_probedEtaOkEta_EffWrtPrevious");
  TH1F *H_probedPhiOkEta_EffWrtPrevious = (TH1F*)file.Get("H_probedPhiOkEta_EffWrtPrevious");
  TH1F *H_probeCovEtaEtaOkEta_EffWrtPrevious = (TH1F*)file.Get("H_probeCovEtaEtaOkEta_EffWrtPrevious");
  TH1F *H_probeEoPoutOkEta_EffWrtPrevious = (TH1F*)file.Get("H_probeEoPoutOkEta_EffWrtPrevious");
  
  H_probeHoEOkEta_EffWrtPrevious->SetLineColor(1);
  H_probeHoEOkEta_EffWrtPrevious->SetMarkerColor(1);

  H_probeS9S25OkEta_EffWrtPrevious->SetLineColor(2);
  H_probeS9S25OkEta_EffWrtPrevious->SetMarkerColor(2);
  
  H_probedEtaOkEta_EffWrtPrevious->SetLineColor(3);
  H_probedEtaOkEta_EffWrtPrevious->SetMarkerColor(3);

  H_probedPhiOkEta_EffWrtPrevious->SetLineColor(4);
  H_probedPhiOkEta_EffWrtPrevious->SetMarkerColor(4);

  H_probeCovEtaEtaOkEta_EffWrtPrevious->SetLineColor(6);
  H_probeCovEtaEtaOkEta_EffWrtPrevious->SetMarkerColor(6);

  H_probeEoPoutOkEta_EffWrtPrevious->SetLineColor(7);
  H_probeEoPoutOkEta_EffWrtPrevious->SetMarkerColor(7);


  H_probeHoEOkEta_EffWrtPrevious->SetMaximum(1.1);
  H_probeHoEOkEta_EffWrtPrevious->SetMinimum(0.5);
  H_probeHoEOkEta_EffWrtPrevious->GetXaxis()->SetTitle("probe electron #eta");
  H_probeHoEOkEta_EffWrtPrevious->GetYaxis()->SetTitle("efficiency");
  H_probeHoEOkEta_EffWrtPrevious->SetTitle("efficiency w.r.t. previous cut on Z #rightarrow e^{+}e^{-} tagged electrons");  

  TCanvas c1;
  H_probeHoEOkEta_EffWrtPrevious->Draw("pe1");
  H_probeS9S25OkEta_EffWrtPrevious->Draw("same pe1");
  H_probedEtaOkEta_EffWrtPrevious->Draw("same pe1");
  H_probedPhiOkEta_EffWrtPrevious->Draw("same pe1");
  H_probeCovEtaEtaOkEta_EffWrtPrevious->Draw("same pe1");
  H_probeEoPoutOkEta_EffWrtPrevious->Draw("same pe1");


  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeHoEOkEta_EffWrtPrevious,"H/E cut","pl");
  leg->AddEntry(H_probeS9S25OkEta_EffWrtPrevious,"s_{9}/s_{25} cut","pl");
  leg->AddEntry(H_probedEtaOkEta_EffWrtPrevious,"|#Delta #eta| cut","pl");
  leg->AddEntry(H_probedPhiOkEta_EffWrtPrevious,"|#Delta #phi| cut","pl");
  leg->AddEntry(H_probeCovEtaEtaOkEta_EffWrtPrevious,"#sigma_{#eta #eta} cut","pl");
  leg->AddEntry(H_probeEoPoutOkEta_EffWrtPrevious,"E_{seed}/P_{out} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_probes_singleeleidcut_Eta.eps";
  c1.SaveAs(psname.c_str());

  
}

void drawSingleEleIdCutProbesPt() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPPt.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeHoEOkPt_EffWrtPrevious = (TH1F*)file.Get("H_probeHoEOkPt_EffWrtPrevious");
  TH1F *H_probeS9S25OkPt_EffWrtPrevious = (TH1F*)file.Get("H_probeS9S25OkPt_EffWrtPrevious");
  TH1F *H_probedEtaOkPt_EffWrtPrevious = (TH1F*)file.Get("H_probedEtaOkPt_EffWrtPrevious");
  TH1F *H_probedPhiOkPt_EffWrtPrevious = (TH1F*)file.Get("H_probedPhiOkPt_EffWrtPrevious");
  TH1F *H_probeCovEtaEtaOkPt_EffWrtPrevious = (TH1F*)file.Get("H_probeCovEtaEtaOkPt_EffWrtPrevious");
  TH1F *H_probeEoPoutOkPt_EffWrtPrevious = (TH1F*)file.Get("H_probeEoPoutOkPt_EffWrtPrevious");
  
  H_probeHoEOkPt_EffWrtPrevious->SetLineColor(1);
  H_probeHoEOkPt_EffWrtPrevious->SetMarkerColor(1);

  H_probeS9S25OkPt_EffWrtPrevious->SetLineColor(2);
  H_probeS9S25OkPt_EffWrtPrevious->SetMarkerColor(2);
  
  H_probedEtaOkPt_EffWrtPrevious->SetLineColor(3);
  H_probedEtaOkPt_EffWrtPrevious->SetMarkerColor(3);

  H_probedPhiOkPt_EffWrtPrevious->SetLineColor(4);
  H_probedPhiOkPt_EffWrtPrevious->SetMarkerColor(4);

  H_probeCovEtaEtaOkPt_EffWrtPrevious->SetLineColor(6);
  H_probeCovEtaEtaOkPt_EffWrtPrevious->SetMarkerColor(6);

  H_probeEoPoutOkPt_EffWrtPrevious->SetLineColor(7);
  H_probeEoPoutOkPt_EffWrtPrevious->SetMarkerColor(7);


  H_probeHoEOkPt_EffWrtPrevious->SetMaximum(1.1);
  H_probeHoEOkPt_EffWrtPrevious->SetMinimum(0.5);
  H_probeHoEOkPt_EffWrtPrevious->GetXaxis()->SetTitle("probe electron p_{T} (GeV/c)");
  H_probeHoEOkPt_EffWrtPrevious->GetYaxis()->SetTitle("efficiency");
  H_probeHoEOkPt_EffWrtPrevious->SetTitle("efficiency w.r.t. previous cut on Z #rightarrow e^{+}e^{-} tagged electrons");  

  TCanvas c1;
  H_probeHoEOkPt_EffWrtPrevious->Draw("pe1");
  H_probeS9S25OkPt_EffWrtPrevious->Draw("same pe1");
  H_probedEtaOkPt_EffWrtPrevious->Draw("same pe1");
  H_probedPhiOkPt_EffWrtPrevious->Draw("same pe1");
  H_probeCovEtaEtaOkPt_EffWrtPrevious->Draw("same pe1");
  H_probeEoPoutOkPt_EffWrtPrevious->Draw("same pe1");


  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeHoEOkPt_EffWrtPrevious,"H/E cut","pl");
  leg->AddEntry(H_probeS9S25OkPt_EffWrtPrevious,"s_{9}/s_{25} cut","pl");
  leg->AddEntry(H_probedEtaOkPt_EffWrtPrevious,"|#Delta #eta| cut","pl");
  leg->AddEntry(H_probedPhiOkPt_EffWrtPrevious,"|#Delta #phi| cut","pl");
  leg->AddEntry(H_probeCovEtaEtaOkPt_EffWrtPrevious,"#sigma_{#eta #eta} cut","pl");
  leg->AddEntry(H_probeEoPoutOkPt_EffWrtPrevious,"E_{seed}/P_{out} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_probes_singleeleidcut_Pt.eps";
  c1.SaveAs(psname.c_str());

  
}



// sequence eleID cuts on probes 
void drawSequenceEleIdCutProbesEta() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPEta.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeHoEOkEta_Eff = (TH1F*)file.Get("H_probeHoEOkEta_Eff");
  TH1F *H_probeS9S25OkEta_Eff = (TH1F*)file.Get("H_probeS9S25OkEta_Eff");
  TH1F *H_probedEtaOkEta_Eff = (TH1F*)file.Get("H_probedEtaOkEta_Eff");
  TH1F *H_probedPhiOkEta_Eff = (TH1F*)file.Get("H_probedPhiOkEta_Eff");
  TH1F *H_probeCovEtaEtaOkEta_Eff = (TH1F*)file.Get("H_probeCovEtaEtaOkEta_Eff");
  TH1F *H_probeEoPoutOkEta_Eff = (TH1F*)file.Get("H_probeEoPoutOkEta_Eff");
  
  H_probeHoEOkEta_Eff->SetLineColor(1);
  H_probeHoEOkEta_Eff->SetMarkerColor(1);

  H_probeS9S25OkEta_Eff->SetLineColor(2);
  H_probeS9S25OkEta_Eff->SetMarkerColor(2);
  
  H_probedEtaOkEta_Eff->SetLineColor(3);
  H_probedEtaOkEta_Eff->SetMarkerColor(3);

  H_probedPhiOkEta_Eff->SetLineColor(4);
  H_probedPhiOkEta_Eff->SetMarkerColor(4);

  H_probeCovEtaEtaOkEta_Eff->SetLineColor(6);
  H_probeCovEtaEtaOkEta_Eff->SetMarkerColor(6);

  H_probeEoPoutOkEta_Eff->SetLineColor(7);
  H_probeEoPoutOkEta_Eff->SetMarkerColor(7);


  H_probeHoEOkEta_Eff->SetMaximum(1.1);
  H_probeHoEOkEta_Eff->SetMinimum(0.5);
  H_probeHoEOkEta_Eff->GetXaxis()->SetTitle("probe electron #eta");
  H_probeHoEOkEta_Eff->GetYaxis()->SetTitle("efficiency");
  H_probeHoEOkEta_Eff->SetTitle("efficiency on Z #rightarrow e^{+}e^{-} tagged electrons");  

  TCanvas c1;
  H_probeHoEOkEta_Eff->Draw("pe1");
  H_probeS9S25OkEta_Eff->Draw("same pe1");
  H_probedEtaOkEta_Eff->Draw("same pe1");
  H_probedPhiOkEta_Eff->Draw("same pe1");
  H_probeCovEtaEtaOkEta_Eff->Draw("same pe1");
  H_probeEoPoutOkEta_Eff->Draw("same pe1");


  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeHoEOkEta_Eff,"H/E cut","pl");
  leg->AddEntry(H_probeS9S25OkEta_Eff,"s_{9}/s_{25} cut","pl");
  leg->AddEntry(H_probedEtaOkEta_Eff,"|#Delta #eta| cut","pl");
  leg->AddEntry(H_probedPhiOkEta_Eff,"|#Delta #phi| cut","pl");
  leg->AddEntry(H_probeCovEtaEtaOkEta_Eff,"#sigma_{#eta #eta} cut","pl");
  leg->AddEntry(H_probeEoPoutOkEta_Eff,"E_{seed}/P_{out} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_probes_eleidcut_Eta.eps";
  c1.SaveAs(psname.c_str());



  
}

void drawSequenceEleIdCutProbesPt() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPPt.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeHoEOkPt_Eff = (TH1F*)file.Get("H_probeHoEOkPt_Eff");
  TH1F *H_probeS9S25OkPt_Eff = (TH1F*)file.Get("H_probeS9S25OkPt_Eff");
  TH1F *H_probedEtaOkPt_Eff = (TH1F*)file.Get("H_probedEtaOkPt_Eff");
  TH1F *H_probedPhiOkPt_Eff = (TH1F*)file.Get("H_probedPhiOkPt_Eff");
  TH1F *H_probeCovEtaEtaOkPt_Eff = (TH1F*)file.Get("H_probeCovEtaEtaOkPt_Eff");
  TH1F *H_probeEoPoutOkPt_Eff = (TH1F*)file.Get("H_probeEoPoutOkPt_Eff");
  
  H_probeHoEOkPt_Eff->SetLineColor(1);
  H_probeHoEOkPt_Eff->SetMarkerColor(1);

  H_probeS9S25OkPt_Eff->SetLineColor(2);
  H_probeS9S25OkPt_Eff->SetMarkerColor(2);
  
  H_probedEtaOkPt_Eff->SetLineColor(3);
  H_probedEtaOkPt_Eff->SetMarkerColor(3);

  H_probedPhiOkPt_Eff->SetLineColor(4);
  H_probedPhiOkPt_Eff->SetMarkerColor(4);

  H_probeCovEtaEtaOkPt_Eff->SetLineColor(6);
  H_probeCovEtaEtaOkPt_Eff->SetMarkerColor(6);

  H_probeEoPoutOkPt_Eff->SetLineColor(7);
  H_probeEoPoutOkPt_Eff->SetMarkerColor(7);


  H_probeHoEOkPt_Eff->SetMaximum(1.1);
  H_probeHoEOkPt_Eff->SetMinimum(0.5);
  H_probeHoEOkPt_Eff->GetXaxis()->SetTitle("probe electron p_{T} (GeV/c)");
  H_probeHoEOkPt_Eff->GetYaxis()->SetTitle("efficiency");
  H_probeHoEOkPt_Eff->SetTitle("efficiency on Z #rightarrow e^{+}e^{-} tagged electrons");  

  TCanvas c1;
  H_probeHoEOkPt_Eff->Draw("pe1");
  H_probeS9S25OkPt_Eff->Draw("same pe1");
  H_probedEtaOkPt_Eff->Draw("same pe1");
  H_probedPhiOkPt_Eff->Draw("same pe1");
  H_probeCovEtaEtaOkPt_Eff->Draw("same pe1");
  H_probeEoPoutOkPt_Eff->Draw("same pe1");


  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeHoEOkPt_Eff,"H/E cut","pl");
  leg->AddEntry(H_probeS9S25OkPt_Eff,"s_{9}/s_{25} cut","pl");
  leg->AddEntry(H_probedEtaOkPt_Eff,"|#Delta #eta| cut","pl");
  leg->AddEntry(H_probedPhiOkPt_Eff,"|#Delta #phi| cut","pl");
  leg->AddEntry(H_probeCovEtaEtaOkPt_Eff,"#sigma_{#eta #eta} cut","pl");
  leg->AddEntry(H_probeEoPoutOkPt_Eff,"E_{seed}/P_{out} cut","pl");

  leg->Draw();

  std::string psname = "eff_ZTaP_probes_eleidcut_Pt.eps";
  c1.SaveAs(psname.c_str());

  
}



// draw classification fraction
void drawClassFractionProbesEta() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPEta.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeGoldenEta_Eff =    (TH1F*)file.Get("H_probeGoldenEta_Eff");
  TH1F *H_probeBigBremEta_Eff =   (TH1F*)file.Get("H_probeBigBremEta_Eff");
  TH1F *H_probeNarrowEta_Eff =    (TH1F*)file.Get("H_probeNarrowEta_Eff");
  TH1F *H_probeShoweringEta_Eff = (TH1F*)file.Get("H_probeShoweringEta_Eff");
  //  TH1F *H_probeCracksEta_Eff =    (TH1F*)file.Get("H_probeCracksEta_Eff");
  
  H_probeGoldenEta_Eff->SetLineColor(1);
  H_probeGoldenEta_Eff->SetMarkerColor(1);

  H_probeBigBremEta_Eff->SetLineColor(2);
  H_probeBigBremEta_Eff->SetMarkerColor(2);

  H_probeNarrowEta_Eff->SetLineColor(3);
  H_probeNarrowEta_Eff->SetMarkerColor(3);

  H_probeShoweringEta_Eff->SetLineColor(4);
  H_probeShoweringEta_Eff->SetMarkerColor(4);

//   H_probeCracksEta_Eff->SetLineColor(6);
//   H_probeCracksEta_Eff->SetMarkerColor(6);

  H_probeGoldenEta_Eff->SetMaximum(1.0);
  H_probeGoldenEta_Eff->SetMinimum(0.0);
  H_probeGoldenEta_Eff->GetXaxis()->SetTitle("probe electron #eta");
  H_probeGoldenEta_Eff->GetYaxis()->SetTitle("Electron class fraction");
  H_probeGoldenEta_Eff->SetTitle("");  

  TCanvas c1;
  H_probeGoldenEta_Eff->Draw();
  H_probeBigBremEta_Eff->Draw("same");
  H_probeNarrowEta_Eff->Draw("same");
  H_probeShoweringEta_Eff->Draw("same");
  //  H_probeCracksEta_Eff->Draw();

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeGoldenEta_Eff,"Golden","pl");
  leg->AddEntry(H_probeBigBremEta_Eff,"BigBrem","pl");
  leg->AddEntry(H_probeNarrowEta_Eff,"Narrow","pl");
  leg->AddEntry(H_probeShoweringEta_Eff,"Showering","pl");
  //  leg->AddEntry(H_probeCracksEta_Eff,"Cracks","pl");

  leg->Draw();

  std::string psname = "classFractions_ZTaP_probes_Eta.eps";
  c1.SaveAs(psname.c_str());



  
}


void drawClassFractionProbesPt() {
  
  gStyle->SetOptStat(0);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerSize(2);

  std::string fileName = "Plots/ZTaP/eleIdTaPPt.root";
  TFile file(fileName.c_str());
  
  TH1F *H_probeGoldenPt_Eff =    (TH1F*)file.Get("H_probeGoldenPt_Eff");
  TH1F *H_probeBigBremPt_Eff =   (TH1F*)file.Get("H_probeBigBremPt_Eff");
  TH1F *H_probeNarrowPt_Eff =    (TH1F*)file.Get("H_probeNarrowPt_Eff");
  TH1F *H_probeShoweringPt_Eff = (TH1F*)file.Get("H_probeShoweringPt_Eff");
  //  TH1F *H_probeCracksPt_Eff =    (TH1F*)file.Get("H_probeCracksPt_Eff");
  
  H_probeGoldenPt_Eff->SetLineColor(1);
  H_probeGoldenPt_Eff->SetMarkerColor(1);

  H_probeBigBremPt_Eff->SetLineColor(2);
  H_probeBigBremPt_Eff->SetMarkerColor(2);

  H_probeNarrowPt_Eff->SetLineColor(3);
  H_probeNarrowPt_Eff->SetMarkerColor(3);

  H_probeShoweringPt_Eff->SetLineColor(4);
  H_probeShoweringPt_Eff->SetMarkerColor(4);

//   H_probeCracksPt_Eff->SetLineColor(6);
//   H_probeCracksPt_Eff->SetMarkerColor(6);

  H_probeGoldenPt_Eff->SetMaximum(1.0);
  H_probeGoldenPt_Eff->SetMinimum(0.0);
  H_probeGoldenPt_Eff->GetXaxis()->SetTitle("probe electron p_{T} (GeV/c)");
  H_probeGoldenPt_Eff->GetYaxis()->SetTitle("Electron class fraction");
  H_probeGoldenPt_Eff->SetTitle("");  

  TCanvas c1;
  H_probeGoldenPt_Eff->Draw();
  H_probeBigBremPt_Eff->Draw("same");
  H_probeNarrowPt_Eff->Draw("same");
  H_probeShoweringPt_Eff->Draw("same");
  //  H_probeCracksPt_Eff->Draw();

  TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(H_probeGoldenPt_Eff,"Golden","pl");
  leg->AddEntry(H_probeBigBremPt_Eff,"BigBrem","pl");
  leg->AddEntry(H_probeNarrowPt_Eff,"Narrow","pl");
  leg->AddEntry(H_probeShoweringPt_Eff,"Showering","pl");
  //  leg->AddEntry(H_probeCracksPt_Eff,"Cracks","pl");

  leg->Draw();

  std::string psname = "classFractions_ZTaP_probes_Pt.eps";
  c1.SaveAs(psname.c_str());



  
}


void all() {
  // reconstruction of tags
  drawSingleRecoCutTagsEta();
  drawSingleRecoCutTagsPt();

  drawSequenceRecoCutTagsEta();
  drawSequenceRecoCutTagsPt();

  
  // electron ID on probes
  drawSingleEleIdCutProbesEta();
  drawSingleEleIdCutProbesPt();

  drawSequenceEleIdCutProbesEta();
  drawSequenceEleIdCutProbesPt();

  // class fractions
  drawClassFractionProbesEta();
  drawClassFractionProbesPt();

}
