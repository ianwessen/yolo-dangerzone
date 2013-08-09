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
TFile fileSgn("../optimizationIsolation/studiesInputs/H160_WW_2l/H160_WW_2l_isOptimHistoMerged.root");
TFile fileWj ("../optimizationIsolation/studiesInputs/WjetsMadgraph_Fall08/WjetsMadgraph_Fall08_isolationMerged.root");
TFile filettj("../optimizationIsolation/studiesInputs/ttjetsMadgraph_Fall08/ttjetsMadgraph_Fall08_isolationMerged.root");

// histos
TH1F *Hass_eleSumPt03[5][3],    *Hass_eleSumPt04[5][3],    *Hass_eleSumPt05[5][3];
TH1F *Hass_eleSumHadEt04[5][3], *Hass_eleSumHadEt05[5][3];
TH1F *Hass_eleSumEmEt04[5][3],  *Hass_eleSumEmEt05[5][3];
TH1F *Hass_eleScBasedEcalSum04[5][3], *Hass_eleScBasedEcalSum05[5][3];
TH1F *Hass_eleScHaloBasedEcalSum04[5][3], *Hass_eleScHaloBasedEcalSum05[5][3];
TH1F *Hass_eleIsoFromDepsTk[5][3], *Hass_eleIsoFromDepsEcal[5][3], *Hass_eleIsoFromDepsHcal[5][3];

// signal
char title[200];
for (int ii=0; ii<5; ii++){ 
  sprintf(title,"Hass_eleSumPt03[%d]",ii);
  Hass_eleSumPt03[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumPt04[%d]",ii);
  Hass_eleSumPt04[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumPt05[%d]",ii);
  Hass_eleSumPt05[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumEmEt04[%d]",ii);
  Hass_eleSumEmEt04[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumEmEt05[%d]",ii);
  Hass_eleSumEmEt05[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumHadEt04[%d]",ii);
  Hass_eleSumHadEt04[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleSumHadEt05[%d]",ii);
  Hass_eleSumHadEt05[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum04[%d]",ii);
  Hass_eleScBasedEcalSum04[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum05[%d]",ii);
  Hass_eleScBasedEcalSum05[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum04[%d]",ii);
  Hass_eleScHaloBasedEcalSum04[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum05[%d]",ii);
  Hass_eleScHaloBasedEcalSum05[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsTk[%d]",ii);
  Hass_eleIsoFromDepsTk[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsEcal[%d]",ii);
  Hass_eleIsoFromDepsEcal[ii][0] = (TH1F*)fileSgn->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsHcal[%d]",ii);
  Hass_eleIsoFromDepsHcal[ii][0] = (TH1F*)fileSgn->Get(title);
}

// w+jets
for (int ii=0; ii<5; ii++){ 
  sprintf(title,"Hass_eleSumPt03[%d]",ii);
  Hass_eleSumPt03[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumPt04[%d]",ii);
  Hass_eleSumPt04[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumPt05[%d]",ii);
  Hass_eleSumPt05[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumEmEt04[%d]",ii);
  Hass_eleSumEmEt04[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumEmEt05[%d]",ii);
  Hass_eleSumEmEt05[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumHadEt04[%d]",ii);
  Hass_eleSumHadEt04[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleSumHadEt05[%d]",ii);
  Hass_eleSumHadEt05[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum04[%d]",ii);
  Hass_eleScBasedEcalSum04[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum05[%d]",ii);
  Hass_eleScBasedEcalSum05[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum04[%d]",ii);
  Hass_eleScHaloBasedEcalSum04[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum05[%d]",ii);
  Hass_eleScHaloBasedEcalSum05[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsTk[%d]",ii);
  Hass_eleIsoFromDepsTk[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsEcal[%d]",ii);
  Hass_eleIsoFromDepsEcal[ii][1] = (TH1F*)fileWj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsHcal[%d]",ii);
  Hass_eleIsoFromDepsHcal[ii][1] = (TH1F*)fileWj->Get(title);
}

// tt+jets
for (int ii=0; ii<5; ii++){ 
  sprintf(title,"Hass_eleSumPt03[%d]",ii);
  Hass_eleSumPt03[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumPt04[%d]",ii);
  Hass_eleSumPt04[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumPt05[%d]",ii);
  Hass_eleSumPt05[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumEmEt04[%d]",ii);
  Hass_eleSumEmEt04[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumEmEt05[%d]",ii);
  Hass_eleSumEmEt05[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumHadEt04[%d]",ii);
  Hass_eleSumHadEt04[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleSumHadEt05[%d]",ii);
  Hass_eleSumHadEt05[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum04[%d]",ii);
  Hass_eleScBasedEcalSum04[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleScBasedEcalSum05[%d]",ii);
  Hass_eleScBasedEcalSum05[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum04[%d]",ii);
  Hass_eleScHaloBasedEcalSum04[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleScHaloBasedEcalSum05[%d]",ii);
  Hass_eleScHaloBasedEcalSum05[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsTk[%d]",ii);
  Hass_eleIsoFromDepsTk[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsEcal[%d]",ii);
  Hass_eleIsoFromDepsEcal[ii][2] = (TH1F*)filettj->Get(title);
  sprintf(title,"Hass_eleIsoFromDepsHcal[%d]",ii);
  Hass_eleIsoFromDepsHcal[ii][2] = (TH1F*)filettj->Get(title);
}

// rescaling signal plots
for(int ii=0; ii<5; ii++){
  Hass_eleSumPt03[ii][0].             Scale(1./(Hass_eleSumPt03[ii][0].Integral()));
  Hass_eleSumPt04[ii][0].             Scale(1./(Hass_eleSumPt04[ii][0].Integral()));
  Hass_eleSumPt05[ii][0].             Scale(1./(Hass_eleSumPt05[ii][0].Integral()));
  Hass_eleSumEmEt04[ii][0].           Scale(1./(Hass_eleSumEmEt04[ii][0].Integral()));
  Hass_eleSumEmEt05[ii][0].           Scale(1./(Hass_eleSumEmEt05[ii][0].Integral()));
  Hass_eleSumHadEt04[ii][0].          Scale(1./(Hass_eleSumHadEt04[ii][0].Integral()));
  Hass_eleSumHadEt05[ii][0].          Scale(1./(Hass_eleSumHadEt05[ii][0].Integral()));
  Hass_eleScBasedEcalSum04[ii][0].    Scale(1./(Hass_eleScBasedEcalSum04[ii][0].Integral()));
  Hass_eleScBasedEcalSum05[ii][0].    Scale(1./(Hass_eleScBasedEcalSum05[ii][0].Integral()));
  Hass_eleScHaloBasedEcalSum04[ii][0].Scale(1./(Hass_eleScHaloBasedEcalSum04[ii][0].Integral()));
  Hass_eleScHaloBasedEcalSum05[ii][0].Scale(1./(Hass_eleScHaloBasedEcalSum05[ii][0].Integral()));
  Hass_eleIsoFromDepsTk[ii][0]       .Scale(1./(Hass_eleIsoFromDepsTk[ii][0].Integral()));
  Hass_eleIsoFromDepsEcal[ii][0]     .Scale(1./(Hass_eleIsoFromDepsEcal[ii][0].Integral()));
  Hass_eleIsoFromDepsHcal[ii][0]     .Scale(1./(Hass_eleIsoFromDepsHcal[ii][0].Integral()));
}

// rescaling w+jets plots
for(int ii=0; ii<5; ii++){
  Hass_eleSumPt03[ii][1].             Scale(1./(Hass_eleSumPt03[ii][1].Integral()));
  Hass_eleSumPt04[ii][1].             Scale(1./(Hass_eleSumPt04[ii][1].Integral()));
  Hass_eleSumPt05[ii][1].             Scale(1./(Hass_eleSumPt05[ii][1].Integral()));
  Hass_eleSumEmEt04[ii][1].           Scale(1./(Hass_eleSumEmEt04[ii][1].Integral()));
  Hass_eleSumEmEt05[ii][1].           Scale(1./(Hass_eleSumEmEt05[ii][1].Integral()));
  Hass_eleSumHadEt04[ii][1].          Scale(1./(Hass_eleSumHadEt04[ii][1].Integral()));
  Hass_eleSumHadEt05[ii][1].          Scale(1./(Hass_eleSumHadEt05[ii][1].Integral()));
  Hass_eleScBasedEcalSum04[ii][1].    Scale(1./(Hass_eleScBasedEcalSum04[ii][1].Integral()));
  Hass_eleScBasedEcalSum05[ii][1].    Scale(1./(Hass_eleScBasedEcalSum05[ii][1].Integral()));
  Hass_eleScHaloBasedEcalSum04[ii][1].Scale(1./(Hass_eleScHaloBasedEcalSum04[ii][1].Integral()));
  Hass_eleScHaloBasedEcalSum05[ii][1].Scale(1./(Hass_eleScHaloBasedEcalSum05[ii][1].Integral()));
  Hass_eleIsoFromDepsTk[ii][1]       .Scale(1./(Hass_eleIsoFromDepsTk[ii][1].Integral()));
  Hass_eleIsoFromDepsEcal[ii][1]     .Scale(1./(Hass_eleIsoFromDepsEcal[ii][1].Integral()));
  Hass_eleIsoFromDepsHcal[ii][1]     .Scale(1./(Hass_eleIsoFromDepsHcal[ii][1].Integral()));
}

// rescaling tt+jets plots
for(int ii=0; ii<5; ii++){
  Hass_eleSumPt03[ii][2].             Scale(1./(Hass_eleSumPt03[ii][2].Integral()));
  Hass_eleSumPt04[ii][2].             Scale(1./(Hass_eleSumPt04[ii][2].Integral()));
  Hass_eleSumPt05[ii][2].             Scale(1./(Hass_eleSumPt05[ii][2].Integral()));
  Hass_eleSumEmEt04[ii][2].           Scale(1./(Hass_eleSumEmEt04[ii][2].Integral()));
  Hass_eleSumEmEt05[ii][2].           Scale(1./(Hass_eleSumEmEt05[ii][2].Integral()));
  Hass_eleSumHadEt04[ii][2].          Scale(1./(Hass_eleSumHadEt04[ii][2].Integral()));
  Hass_eleSumHadEt05[ii][2].          Scale(1./(Hass_eleSumHadEt05[ii][2].Integral()));
  Hass_eleScBasedEcalSum04[ii][2].    Scale(1./(Hass_eleScBasedEcalSum04[ii][2].Integral()));
  Hass_eleScBasedEcalSum05[ii][2].    Scale(1./(Hass_eleScBasedEcalSum05[ii][2].Integral()));
  Hass_eleScHaloBasedEcalSum04[ii][2].Scale(1./(Hass_eleScHaloBasedEcalSum04[ii][2].Integral()));
  Hass_eleScHaloBasedEcalSum05[ii][2].Scale(1./(Hass_eleScHaloBasedEcalSum05[ii][2].Integral()));
  Hass_eleIsoFromDepsTk[ii][2]       .Scale(1./(Hass_eleIsoFromDepsTk[ii][2].Integral()));
  Hass_eleIsoFromDepsEcal[ii][2]     .Scale(1./(Hass_eleIsoFromDepsEcal[ii][2].Integral()));
  Hass_eleIsoFromDepsHcal[ii][2]     .Scale(1./(Hass_eleIsoFromDepsHcal[ii][2].Integral()));
}

// cosmetics
int theColor = 0;
int theStyle = 0;
for(int jj=0; jj<3; jj++){

  if (jj==0) { theColor = 1; theStyle = 3004; } 
  if (jj==1) { theColor = 2; theStyle = 3006; } 
  if (jj==2) { theColor = 3; theStyle = 3005; } 

  for(int ii=0; ii<5; ii++){
    Hass_eleSumPt03[ii][jj].SetFillColor(theColor);                Hass_eleSumPt03[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumPt04[ii][jj].SetFillColor(theColor);                Hass_eleSumPt04[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumPt05[ii][jj].SetFillColor(theColor);                Hass_eleSumPt05[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumEmEt04[ii][jj].SetFillColor(theColor);              Hass_eleSumEmEt04[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumEmEt05[ii][jj].SetFillColor(theColor);              Hass_eleSumEmEt05[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumHadEt04[ii][jj].SetFillColor(theColor);             Hass_eleSumHadEt04[ii][jj].SetFillStyle(theStyle);
    Hass_eleSumHadEt05[ii][jj].SetFillColor(theColor);             Hass_eleSumHadEt05[ii][jj].SetFillStyle(theStyle);
    Hass_eleScBasedEcalSum04[ii][jj].SetFillColor(theColor);       Hass_eleScBasedEcalSum04[ii][jj].SetFillStyle(theStyle);    
    Hass_eleScBasedEcalSum05[ii][jj].SetFillColor(theColor);       Hass_eleScBasedEcalSum05[ii][jj].SetFillStyle(theStyle);    
    Hass_eleScHaloBasedEcalSum04[ii][jj].SetFillColor(theColor);   Hass_eleScHaloBasedEcalSum04[ii][jj].SetFillStyle(theStyle);    
    Hass_eleScHaloBasedEcalSum05[ii][jj].SetFillColor(theColor);   Hass_eleScHaloBasedEcalSum05[ii][jj].SetFillStyle(theStyle);    
    Hass_eleIsoFromDepsTk[ii][jj].SetFillColor(theColor);          Hass_eleIsoFromDepsTk[ii][jj].SetFillStyle(theStyle);             
    Hass_eleIsoFromDepsEcal[ii][jj].SetFillColor(theColor);        Hass_eleIsoFromDepsEcal[ii][jj].SetFillStyle(theStyle);             
    Hass_eleIsoFromDepsHcal[ii][jj].SetFillColor(theColor);        Hass_eleIsoFromDepsHcal[ii][jj].SetFillStyle(theStyle);             

    // rebinning
    Hass_eleSumPt03[ii][jj].Rebin(4);    
    Hass_eleSumPt04[ii][jj].Rebin(4);               
    Hass_eleSumPt05[ii][jj].Rebin(4);               
    Hass_eleSumEmEt04[ii][jj].Rebin(4);                  
    Hass_eleSumEmEt05[ii][jj].Rebin(4);                  
    Hass_eleSumHadEt04[ii][jj].Rebin(4);                
    Hass_eleSumHadEt05[ii][jj].Rebin(4);                 
    Hass_eleScBasedEcalSum04[ii][jj].Rebin(4);            
    Hass_eleScBasedEcalSum05[ii][jj].Rebin(4);            
    Hass_eleScHaloBasedEcalSum04[ii][jj].Rebin(4);        
    Hass_eleScHaloBasedEcalSum05[ii][jj].Rebin(4);        
    Hass_eleIsoFromDepsTk[ii][jj].Rebin(4);                
    Hass_eleIsoFromDepsEcal[ii][jj].Rebin(4);            
    Hass_eleIsoFromDepsHcal[ii][jj].Rebin(4);             
  }
}


// axes titles
for(int jj=0; jj<3; jj++){
  for(int ii=0; ii<5; ii++){
    Hass_eleSumPt03[ii][jj].GetXaxis()->SetTitle("tracker based, #DeltaR = 03");
    Hass_eleSumPt04[ii][jj].GetXaxis()->SetTitle("tracker based, #DeltaR = 04");
    Hass_eleSumPt05[ii][jj].GetXaxis()->SetTitle("tracker based, #DeltaR = 05");
    Hass_eleSumEmEt04[ii][jj].GetXaxis()->SetTitle("ECAL, cone based, #DeltaR = 04");
    Hass_eleSumEmEt05[ii][jj].GetXaxis()->SetTitle("ECAL, cone based, #DeltaR = 05");
    Hass_eleSumHadEt04[ii][jj].GetXaxis()->SetTitle("HCAL based, #DeltaR = 04");
    Hass_eleSumHadEt05[ii][jj].GetXaxis()->SetTitle("HCAL based, #DeltaR = 05");
    Hass_eleScBasedEcalSum04[ii][jj].GetXaxis()->SetTitle("ECAL, rechits based, #DeltaR = 04");
    Hass_eleScBasedEcalSum05[ii][jj].GetXaxis()->SetTitle("ECAL, rechits based, #DeltaR = 05");
    Hass_eleScHaloBasedEcalSum04[ii][jj].GetXaxis()->SetTitle("ECAL, rechits based with halo, #DeltaR = 04");
    Hass_eleScHaloBasedEcalSum05[ii][jj].GetXaxis()->SetTitle("ECAL, rechits based with halo, #DeltaR = 05");
    Hass_eleIsoFromDepsTk[ii][jj].GetXaxis()->SetTitle("tracker based, jurassic, #DeltaR = 04");
    Hass_eleIsoFromDepsEcal[ii][jj].GetXaxis()->SetTitle("ECAL based, jurassic, #DeltaR = 04");
    Hass_eleIsoFromDepsHcal[ii][jj].GetXaxis()->SetTitle("HCAL based, jurassic, #DeltaR = 04");
  }}
  

// ------------- plots -----------------------------------

TLegend SvsB(0.50,0.6,0.75,0.82);
SvsB.AddEntry(Hass_eleSumPt04[0][0], "signal","f");
//SvsB.AddEntry(Hass_eleSumPt04[0][2], "ttjets","f");
SvsB.AddEntry(Hass_eleSumPt04[0][1], "wjets","f");
SvsB.SetFillColor(0);
SvsB.SetBorderSize(0.4);

TCanvas *c1 = new TCanvas("c1", "",1);  
Hass_eleSumHadEt04[2][0].Draw();
Hass_eleSumHadEt04[2][1].Draw("same");
//Hass_eleSumHadEt04[2][2].Draw("same");
SvsB.Draw();
c1->Print("Hass_eleSumHadEt04.eps");
c1->Print("Hass_eleSumHadEt04.root");

TCanvas *c2 = new TCanvas("c2", "",1);  
Hass_eleSumPt04[2][0].Draw();
Hass_eleSumPt04[2][1].Draw("same");
//Hass_eleSumPt04[2][2].Draw("same");
SvsB.Draw();
c2->Print("Hass_eleSumPt04.eps");
c2->Print("Hass_eleSumPt04.root");


TCanvas *c3 = new TCanvas("c3", "",1);  
Hass_eleIsoFromDepsEcal[2][0].Draw();
Hass_eleIsoFromDepsEcal[2][1].Draw("same");
//Hass_eleIsoFromDepsEcal[2][2].Draw("same");
SvsB.Draw();
c3->Print("Hass_eleIsoFromDepsEcal.eps");
c3->Print("Hass_eleIsoFromDepsEcal.root");


}
