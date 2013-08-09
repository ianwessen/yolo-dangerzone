{
// style
TStyle *tesiStyle = new TStyle("tesiStyle","");
tesiStyle->SetCanvasColor(0);
tesiStyle->SetFrameFillColor(0);
tesiStyle->SetStatColor(0);
tesiStyle->SetOptStat(0000);
tesiStyle->SetOptFit(1111);
tesiStyle->SetTitleFillColor(0);
tesiStyle->SetCanvasBorderMode(0);
tesiStyle->SetPadBorderMode(0);
tesiStyle->SetFrameBorderMode(0);
tesiStyle->cd();


// files
TFile fileSgn_show   ("../outHistos_showering_signal.root");
TFile fileSgn_notShow("../outHistos_notShowering_signal.root");
TFile fileSgn_all    ("../outHistos_allClasses_signal.root");
//
TFile filettjets_show   ("../outHistos_showering_ttjets.root");
TFile filettjets_notShow("../outHistos_notShowering_ttjets.root");
TFile filettjets_all    ("../outHistos_allClasses_ttjets.root");
//
TFile fileWjets_show   ("../outHistos_showering_wjets.root");
TFile fileWjets_notShow("../outHistos_notShowering_wjets.root");
TFile fileWjets_all    ("../outHistos_allClasses_wjets.root");


// charging histos
TH1F *HH_like[9], *HL_like[9];
HH_like[0] = (TH1F*)fileSgn_show       -> Get("HH_Likelihood");
HH_like[1] = (TH1F*)fileSgn_notShow    -> Get("HH_Likelihood");
HH_like[2] = (TH1F*)fileSgn_all        -> Get("HH_Likelihood");
HH_like[3] = (TH1F*)filettjets_show    -> Get("HH_Likelihood");
HH_like[4] = (TH1F*)filettjets_notShow -> Get("HH_Likelihood");
HH_like[5] = (TH1F*)filettjets_all     -> Get("HH_Likelihood");
HH_like[6] = (TH1F*)fileWjets_show     -> Get("HH_Likelihood");
HH_like[7] = (TH1F*)fileWjets_notShow  -> Get("HH_Likelihood");
HH_like[8] = (TH1F*)fileWjets_all      -> Get("HH_Likelihood");
// 
HL_like[0] = (TH1F*)fileSgn_show        -> Get("HL_Likelihood");
HL_like[1] = (TH1F*)fileSgn_notShow     -> Get("HL_Likelihood");
HL_like[2] = (TH1F*)fileSgn_all         -> Get("HL_Likelihood");
HL_like[3] = (TH1F*)filettjets_show     -> Get("HL_Likelihood");
HL_like[4] = (TH1F*)filettjets_notShow  -> Get("HL_Likelihood");
HL_like[5] = (TH1F*)filettjets_all      -> Get("HL_Likelihood");
HL_like[6] = (TH1F*)fileWjets_show      -> Get("HL_Likelihood");
HL_like[7] = (TH1F*)fileWjets_notShow   -> Get("HL_Likelihood");
HL_like[8] = (TH1F*)fileWjets_all       -> Get("HL_Likelihood");


// rescaling
double scale_HH[9];
double scale_HL[9];
for(int ii=0; ii<9; ii++){
  HH_like[ii].Scale(1./(HH_like[ii].Integral()));
  HL_like[ii].Scale(1./(HL_like[ii].Integral()));
}


// cosmetics
for(int ii=0; ii<9; ii++){
  HH_like[ii].SetLineWidth(2);    
  HL_like[ii].SetLineWidth(2);    
}
for(int ii=0; ii<3; ii++){
  HH_like[ii].SetLineColor(2);    
  HL_like[ii].SetLineColor(2);    
}
for(int ii=3; ii<6; ii++){
  HH_like[ii].SetLineColor(3);    
  HL_like[ii].SetLineColor(3);    
}
for(int ii=6; ii<9; ii++){
  HH_like[ii].SetLineColor(4);    
  HL_like[ii].SetLineColor(4);    
}

// axes titles
for(int ii=0; ii<9; ii++){
  HH_like[ii].GetXaxis()->SetTitle("eleID likelihood");
  HL_like[ii].GetXaxis()->SetTitle("eleID likelihood");
}



// ------------- plots -----------------------------------

TLegend Leg(0.50,0.6,0.75,0.82);
Leg.AddEntry(HL_like[0], "signal", "f");
Leg.AddEntry(HL_like[3], "tt+jets","f");
Leg.AddEntry(HL_like[6], "W+jets", "f");
Leg.SetFillColor(0);
Leg.SetBorderSize(0.4);

/*
TCanvas *c1 = new TCanvas("c1", "high p_{T}, showering",1);  
c1.SetLogy();
HH_like[0].Draw();
HH_like[3].Draw("same");
HH_like[6].Draw("same");
Leg.Draw();
c1->Print("HighPt_showering.png");

TCanvas *c2 = new TCanvas("c2", "high p_{T}, not showering",1);  
c2.SetLogy();
HH_like[1].Draw();
HH_like[4].Draw("same");
HH_like[7].Draw("same");
Leg.Draw();
c2->Print("HighPt_notShowering.png");
*/

TCanvas *c3 = new TCanvas("c3", "high p_{T}, all classes",1);  
c3.SetLogy();
HH_like[2].Draw();
HH_like[5].Draw("same");
HH_like[8].Draw("same");
Leg.Draw();
c3->Print("HighPt_allClasses.png");

/*
TCanvas *c11 = new TCanvas("c11", "low p_{T}, showering",1);  
c11.SetLogy();
HL_like[0].Draw();
HL_like[3].Draw("same");
HL_like[6].Draw("same");
Leg.Draw();
c11->Print("LowPt_showering.png");

TCanvas *c12 = new TCanvas("c12", "low p_{T}, not showering",1);  
c12.SetLogy();
HL_like[1].Draw();
HL_like[4].Draw("same");
HL_like[7].Draw("same");
Leg.Draw();
c12->Print("LowPt_notShowering.png");
*/

TCanvas *c13 = new TCanvas("c13", "low p_{T}, all classes",1);  
c13.SetLogy();
HL_like[2].Draw();
HL_like[5].Draw("same");
HL_like[8].Draw("same");
Leg.Draw();
c13->Print("LowPt_allClasses.png");



}
