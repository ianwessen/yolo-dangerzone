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
TFile *file[2];
file[0] = new TFile("ttjetsMADGRAPH_Fall08/root/ttjets_mergedHistos.root");
file[1] = new TFile("WW_2l/root/WW_mergedHistos.root");

TH1F *H_compXY[2],   *H_compXYZ[2],   *H_compZ[2];
TH1F *H_deltaXY[2],  *H_deltaXYZ[2],  *H_deltaZ[2];
TH1F *H_trackDxy[2], *H_trackDxyz[2], *H_trackDz[2];
for(int ii=0; ii<2; ii++){
  H_compXY[ii]    = (TH1F*)file[ii] -> Get("H_compXY");
  H_compXYZ[ii]   = (TH1F*)file[ii] -> Get("H_compXYZ");
  H_compZ[ii]     = (TH1F*)file[ii] -> Get("H_compZ");
  H_deltaXY[ii]   = (TH1F*)file[ii] -> Get("H_deltaXY");
  H_deltaXYZ[ii]  = (TH1F*)file[ii] -> Get("H_deltaXYZ");
  H_deltaZ[ii]    = (TH1F*)file[ii] -> Get("H_deltaZ");
  H_trackDxy[ii]  = (TH1F*)file[ii] -> Get("H_trackDxy");
  H_trackDxyz[ii] = (TH1F*)file[ii] -> Get("H_trackDxyz");
  H_trackDz[ii]   = (TH1F*)file[ii] -> Get("H_trackDz");
}

// rescaling
for(int ii=0; ii<2; ii++){
  H_compXY[ii].   Scale(1./H_compXY[ii].Integral());		     
  H_compXYZ[ii].  Scale(1./H_compXYZ[ii].Integral());		     
  H_compZ[ii].    Scale(1./H_compZ[ii].Integral());		     
  H_deltaXY[ii].  Scale(1./H_deltaXY[ii].Integral());		     
  H_deltaXYZ[ii]. Scale(1./H_deltaXYZ[ii].Integral());		     
  H_deltaZ[ii].   Scale(1./H_deltaZ[ii].Integral());		    
  H_trackDxy[ii]. Scale(1./H_trackDxy[ii].Integral());		     
  H_trackDxyz[ii].Scale(1./H_trackDxyz[ii].Integral());		     
  H_trackDz[ii].  Scale(1./H_trackDz[ii].Integral());		     
}

// cosmetics
for(int ii=0; ii<2; ii++){
  int color;
  if(ii==0) color=1;
  if(ii==1) color=2;
  H_compXY[ii].   SetLineColor(color);     
  H_compXYZ[ii].  SetLineColor(color);          
  H_compZ[ii].    SetLineColor(color);          
  H_deltaXY[ii].  SetLineColor(color);          
  H_deltaXYZ[ii]. SetLineColor(color);          
  H_deltaZ[ii].   SetLineColor(color);         
  H_trackDxy[ii]. SetLineColor(color);          
  H_trackDxyz[ii].SetLineColor(color);          
  H_trackDz[ii].  SetLineColor(color);     	     
}

// axes titles
for(int ii=0; ii<2; ii++){
  //HH_like[ii].GetXaxis()->SetTitle("likelihood");
}


// ------------- plots -----------------------------------

// high vs low pt study
TLegend leg(0.50,0.6,0.75,0.82);
leg.AddEntry(H_compXY[0], "tt+jets","l");
leg.AddEntry(H_compXY[1], "WW","l");
leg.SetFillColor(0);
leg.SetBorderSize(0.4);

TCanvas *c1 = new TCanvas("c1", "",1);  
c1.SetLogy();
H_compXY[0].Draw();
H_compXY[1].Draw("same");
leg.Draw();
c1->Print("compXY.png");

TCanvas *c2 = new TCanvas("c2", "",1);  
c2.SetLogy();
H_compXYZ[0].Draw();
H_compXYZ[1].Draw("same");
leg.Draw();
c2->Print("compXYZ.png");

TCanvas *c3 = new TCanvas("c3", "",1);  
c3.SetLogy();
H_compZ[0].Draw();
H_compZ[1].Draw("same");
leg.Draw();
c3->Print("compZ.png");

TCanvas *c4 = new TCanvas("c4", "",1);  
c4.SetLogy();
H_deltaXY[0].Draw();
H_deltaXY[1].Draw("same");
leg.Draw();
c4->Print("deltaXY.png");

TCanvas *c5 = new TCanvas("c5", "",1);  
c5.SetLogy();
H_deltaXYZ[0].Draw();
H_deltaXYZ[1].Draw("same");
leg.Draw();
c5->Print("deltaXYZ.png");

TCanvas *c6 = new TCanvas("c6", "",1);  
c6.SetLogy();
H_deltaZ[0].Draw();
H_deltaZ[1].Draw("same");
leg.Draw();
c6->Print("deltaZ.png");

TCanvas *c9 = new TCanvas("c9", "",1);  
c9.SetLogy();
H_trackDxy[0].Draw();
H_trackDxy[1].Draw("same");
leg.Draw();
c9->Print("trackDxy.png");

TCanvas *c7 = new TCanvas("c7", "",1);  
c7.SetLogy();
H_trackDxyz[0].Draw();
H_trackDxyz[1].Draw("same");
leg.Draw();
c7->Print("trackDxyz.png");

TCanvas *c8 = new TCanvas("c8", "",1);  
c8.SetLogy();
H_trackDz[0].Draw();
H_trackDz[1].Draw("same");
leg.Draw();
c8->Print("trackDz.png");

}
