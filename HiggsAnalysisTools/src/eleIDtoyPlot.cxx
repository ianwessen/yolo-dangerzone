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

int whichStudy = 1;  // 0 = high vs low pt
                     // 1 = signal vs background

// files
TFile fileSgnEBshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_sgn_showeringEB.root");
TFile fileSgnEEshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_sgn_showeringEE.root");
TFile fileSgnEBgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_sgn_goldenEB.root");
TFile fileSgnEEgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_sgn_goldenEE.root");
//
TFile fileTTjetsEBshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_ttjets_showeringEB.root");
TFile fileTTjetsEEshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_ttjets_showeringEE.root");
TFile fileTTjetsEBgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_ttjets_goldenEB.root");
TFile fileTTjetsEEgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_ttjets_goldenEE.root");
//
TFile fileWjetsEBshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_Wjets_showeringEB.root");
TFile fileWjetsEEshow  ("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_Wjets_showeringEE.root");
TFile fileWjetsEBgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_Wjets_goldenEB.root");
TFile fileWjetsEEgolden("/afs/cern.ch/user/c/crovelli/scratch0/HWW_2009_2X/HiggsAnalysisTools/outHistos_Wjets_goldenEE.root");

TH1F *HH_dEta[12], *HH_dPhi[12], *HH_HoE[12], *HH_S9S25[12], *HH_See[12], *HH_EoPout[12];
TH1F *HL_dEta[12], *HL_dPhi[12], *HL_HoE[12], *HL_S9S25[12], *HL_See[12], *HL_EoPout[12];


// golden, signal, EB
HH_dEta[0]   = (TH1F*)fileSgnEBgolden -> Get("HH_dEta[0]");
HH_dPhi[0]   = (TH1F*)fileSgnEBgolden -> Get("HH_dPhi[0]");
HH_HoE[0]    = (TH1F*)fileSgnEBgolden -> Get("HH_HoE[0]");
HH_S9S25[0]  = (TH1F*)fileSgnEBgolden -> Get("HH_S9S25[0]");
HH_See[0]    = (TH1F*)fileSgnEBgolden -> Get("HH_See[0]");
HH_EoPout[0] = (TH1F*)fileSgnEBgolden -> Get("HH_EoPout[0]");
HL_dEta[0]   = (TH1F*)fileSgnEBgolden -> Get("HL_dEta[0]");
HL_dPhi[0]   = (TH1F*)fileSgnEBgolden -> Get("HL_dPhi[0]");
HL_HoE[0]    = (TH1F*)fileSgnEBgolden -> Get("HL_HoE[0]");
HL_S9S25[0]  = (TH1F*)fileSgnEBgolden -> Get("HL_S9S25[0]");
HL_See[0]    = (TH1F*)fileSgnEBgolden -> Get("HL_See[0]");
HL_EoPout[0] = (TH1F*)fileSgnEBgolden -> Get("HL_EoPout[0]");

// golden, signal, EE
HH_dEta[1]   = (TH1F*)fileSgnEEgolden -> Get("HH_dEta[1]");
HH_dPhi[1]   = (TH1F*)fileSgnEEgolden -> Get("HH_dPhi[1]");
HH_HoE[1]    = (TH1F*)fileSgnEEgolden -> Get("HH_HoE[1]");
HH_S9S25[1]  = (TH1F*)fileSgnEEgolden -> Get("HH_S9S25[1]");
HH_See[1]    = (TH1F*)fileSgnEEgolden -> Get("HH_See[1]");
HH_EoPout[1] = (TH1F*)fileSgnEEgolden -> Get("HH_EoPout[1]");
HL_dEta[1]   = (TH1F*)fileSgnEEgolden -> Get("HL_dEta[1]");
HL_dPhi[1]   = (TH1F*)fileSgnEEgolden -> Get("HL_dPhi[1]");
HL_HoE[1]    = (TH1F*)fileSgnEEgolden -> Get("HL_HoE[1]");
HL_S9S25[1]  = (TH1F*)fileSgnEEgolden -> Get("HL_S9S25[1]");
HL_See[1]    = (TH1F*)fileSgnEEgolden -> Get("HL_See[1]");
HL_EoPout[1] = (TH1F*)fileSgnEEgolden -> Get("HL_EoPout[1]");

// showering, signal, EB
HH_dEta[2]   = (TH1F*)fileSgnEBshow -> Get("HH_dEta[2]");
HH_dPhi[2]   = (TH1F*)fileSgnEBshow -> Get("HH_dPhi[2]");
HH_HoE[2]    = (TH1F*)fileSgnEBshow -> Get("HH_HoE[2]");
HH_S9S25[2]  = (TH1F*)fileSgnEBshow -> Get("HH_S9S25[2]");
HH_See[2]    = (TH1F*)fileSgnEBshow -> Get("HH_See[2]");
HH_EoPout[2] = (TH1F*)fileSgnEBshow -> Get("HH_EoPout[2]");
HL_dEta[2]   = (TH1F*)fileSgnEBshow -> Get("HL_dEta[2]");
HL_dPhi[2]   = (TH1F*)fileSgnEBshow -> Get("HL_dPhi[2]");
HL_HoE[2]    = (TH1F*)fileSgnEBshow -> Get("HL_HoE[2]");
HL_S9S25[2]  = (TH1F*)fileSgnEBshow -> Get("HL_S9S25[2]");
HL_See[2]    = (TH1F*)fileSgnEBshow -> Get("HL_See[2]");
HL_EoPout[2] = (TH1F*)fileSgnEBshow -> Get("HL_EoPout[2]");

// showering, signal, EE
HH_dEta[3]   = (TH1F*)fileSgnEEshow -> Get("HH_dEta[3]");
HH_dPhi[3]   = (TH1F*)fileSgnEEshow -> Get("HH_dPhi[3]");
HH_HoE[3]    = (TH1F*)fileSgnEEshow -> Get("HH_HoE[3]");
HH_S9S25[3]  = (TH1F*)fileSgnEEshow -> Get("HH_S9S25[3]");
HH_See[3]    = (TH1F*)fileSgnEEshow -> Get("HH_See[3]");
HH_EoPout[3] = (TH1F*)fileSgnEEshow -> Get("HH_EoPout[3]");
HL_dEta[3]   = (TH1F*)fileSgnEEshow -> Get("HL_dEta[3]");
HL_dPhi[3]   = (TH1F*)fileSgnEEshow -> Get("HL_dPhi[3]");
HL_HoE[3]    = (TH1F*)fileSgnEEshow -> Get("HL_HoE[3]");
HL_S9S25[3]  = (TH1F*)fileSgnEEshow -> Get("HL_S9S25[3]");
HL_See[3]    = (TH1F*)fileSgnEEshow -> Get("HL_See[3]");
HL_EoPout[3] = (TH1F*)fileSgnEEshow -> Get("HL_EoPout[3]");

// golden, ttjets, EB
HH_dEta[4]   = (TH1F*)fileTTjetsEBgolden -> Get("HH_dEta[0]");
HH_dPhi[4]   = (TH1F*)fileTTjetsEBgolden -> Get("HH_dPhi[0]");
HH_HoE[4]    = (TH1F*)fileTTjetsEBgolden -> Get("HH_HoE[0]");
HH_S9S25[4]  = (TH1F*)fileTTjetsEBgolden -> Get("HH_S9S25[0]");
HH_See[4]    = (TH1F*)fileTTjetsEBgolden -> Get("HH_See[0]");
HH_EoPout[4] = (TH1F*)fileTTjetsEBgolden -> Get("HH_EoPout[0]");
HL_dEta[4]   = (TH1F*)fileTTjetsEBgolden -> Get("HL_dEta[0]");
HL_dPhi[4]   = (TH1F*)fileTTjetsEBgolden -> Get("HL_dPhi[0]");
HL_HoE[4]    = (TH1F*)fileTTjetsEBgolden -> Get("HL_HoE[0]");
HL_S9S25[4]  = (TH1F*)fileTTjetsEBgolden -> Get("HL_S9S25[0]");
HL_See[4]    = (TH1F*)fileTTjetsEBgolden -> Get("HL_See[0]");
HL_EoPout[4] = (TH1F*)fileTTjetsEBgolden -> Get("HL_EoPout[0]");

// golden, ttjets, EE
HH_dEta[5]   = (TH1F*)fileTTjetsEEgolden -> Get("HH_dEta[1]");
HH_dPhi[5]   = (TH1F*)fileTTjetsEEgolden -> Get("HH_dPhi[1]");
HH_HoE[5]    = (TH1F*)fileTTjetsEEgolden -> Get("HH_HoE[1]");
HH_S9S25[5]  = (TH1F*)fileTTjetsEEgolden -> Get("HH_S9S25[1]");
HH_See[5]    = (TH1F*)fileTTjetsEEgolden -> Get("HH_See[1]");
HH_EoPout[5] = (TH1F*)fileTTjetsEEgolden -> Get("HH_EoPout[1]");
HL_dEta[5]   = (TH1F*)fileTTjetsEEgolden -> Get("HL_dEta[1]");
HL_dPhi[5]   = (TH1F*)fileTTjetsEEgolden -> Get("HL_dPhi[1]");
HL_HoE[5]    = (TH1F*)fileTTjetsEEgolden -> Get("HL_HoE[1]");
HL_S9S25[5]  = (TH1F*)fileTTjetsEEgolden -> Get("HL_S9S25[1]");
HL_See[5]    = (TH1F*)fileTTjetsEEgolden -> Get("HL_See[1]");
HL_EoPout[5] = (TH1F*)fileTTjetsEEgolden -> Get("HL_EoPout[1]");

// showering, ttjets, EB
HH_dEta[6]   = (TH1F*)fileTTjetsEBshow -> Get("HH_dEta[2]");
HH_dPhi[6]   = (TH1F*)fileTTjetsEBshow -> Get("HH_dPhi[2]");
HH_HoE[6]    = (TH1F*)fileTTjetsEBshow -> Get("HH_HoE[2]");
HH_S9S25[6]  = (TH1F*)fileTTjetsEBshow -> Get("HH_S9S25[2]");
HH_See[6]    = (TH1F*)fileTTjetsEBshow -> Get("HH_See[2]");
HH_EoPout[6] = (TH1F*)fileTTjetsEBshow -> Get("HH_EoPout[2]");
HL_dEta[6]   = (TH1F*)fileTTjetsEBshow -> Get("HL_dEta[2]");
HL_dPhi[6]   = (TH1F*)fileTTjetsEBshow -> Get("HL_dPhi[2]");
HL_HoE[6]    = (TH1F*)fileTTjetsEBshow -> Get("HL_HoE[2]");
HL_S9S25[6]  = (TH1F*)fileTTjetsEBshow -> Get("HL_S9S25[2]");
HL_See[6]    = (TH1F*)fileTTjetsEBshow -> Get("HL_See[2]");
HL_EoPout[6] = (TH1F*)fileTTjetsEBshow -> Get("HL_EoPout[2]");

// showering, ttjets, EE
HH_dEta[7]   = (TH1F*)fileTTjetsEEshow -> Get("HH_dEta[3]");
HH_dPhi[7]   = (TH1F*)fileTTjetsEEshow -> Get("HH_dPhi[3]");
HH_HoE[7]    = (TH1F*)fileTTjetsEEshow -> Get("HH_HoE[3]");
HH_S9S25[7]  = (TH1F*)fileTTjetsEEshow -> Get("HH_S9S25[3]");
HH_See[7]    = (TH1F*)fileTTjetsEEshow -> Get("HH_See[3]");
HH_EoPout[7] = (TH1F*)fileTTjetsEEshow -> Get("HH_EoPout[3]");
HL_dEta[7]   = (TH1F*)fileTTjetsEEshow -> Get("HL_dEta[3]");
HL_dPhi[7]   = (TH1F*)fileTTjetsEEshow -> Get("HL_dPhi[3]");
HL_HoE[7]    = (TH1F*)fileTTjetsEEshow -> Get("HL_HoE[3]");
HL_S9S25[7]  = (TH1F*)fileTTjetsEEshow -> Get("HL_S9S25[3]");
HL_See[7]    = (TH1F*)fileTTjetsEEshow -> Get("HL_See[3]");
HL_EoPout[7] = (TH1F*)fileTTjetsEEshow -> Get("HL_EoPout[3]");

// golden, wjets, EB
HH_dEta[8]   = (TH1F*)fileWjetsEBgolden -> Get("HH_dEta[0]");
HH_dPhi[8]   = (TH1F*)fileWjetsEBgolden -> Get("HH_dPhi[0]");
HH_HoE[8]    = (TH1F*)fileWjetsEBgolden -> Get("HH_HoE[0]");
HH_S9S25[8]  = (TH1F*)fileWjetsEBgolden -> Get("HH_S9S25[0]");
HH_See[8]    = (TH1F*)fileWjetsEBgolden -> Get("HH_See[0]");
HH_EoPout[8] = (TH1F*)fileWjetsEBgolden -> Get("HH_EoPout[0]");
HL_dEta[8]   = (TH1F*)fileWjetsEBgolden -> Get("HL_dEta[0]");
HL_dPhi[8]   = (TH1F*)fileWjetsEBgolden -> Get("HL_dPhi[0]");
HL_HoE[8]    = (TH1F*)fileWjetsEBgolden -> Get("HL_HoE[0]");
HL_S9S25[8]  = (TH1F*)fileWjetsEBgolden -> Get("HL_S9S25[0]");
HL_See[8]    = (TH1F*)fileWjetsEBgolden -> Get("HL_See[0]");
HL_EoPout[8] = (TH1F*)fileWjetsEBgolden -> Get("HL_EoPout[0]");

// golden, wjets, EE
HH_dEta[9]   = (TH1F*)fileWjetsEEgolden -> Get("HH_dEta[1]");
HH_dPhi[9]   = (TH1F*)fileWjetsEEgolden -> Get("HH_dPhi[1]");
HH_HoE[9]    = (TH1F*)fileWjetsEEgolden -> Get("HH_HoE[1]");
HH_S9S25[9]  = (TH1F*)fileWjetsEEgolden -> Get("HH_S9S25[1]");
HH_See[9]    = (TH1F*)fileWjetsEEgolden -> Get("HH_See[1]");
HH_EoPout[9] = (TH1F*)fileWjetsEEgolden -> Get("HH_EoPout[1]");
HL_dEta[9]   = (TH1F*)fileWjetsEEgolden -> Get("HL_dEta[1]");
HL_dPhi[9]   = (TH1F*)fileWjetsEEgolden -> Get("HL_dPhi[1]");
HL_HoE[9]    = (TH1F*)fileWjetsEEgolden -> Get("HL_HoE[1]");
HL_S9S25[9]  = (TH1F*)fileWjetsEEgolden -> Get("HL_S9S25[1]");
HL_See[9]    = (TH1F*)fileWjetsEEgolden -> Get("HL_See[1]");
HL_EoPout[9] = (TH1F*)fileWjetsEEgolden -> Get("HL_EoPout[1]");

// showering, wjets, EB
HH_dEta[10]   = (TH1F*)fileWjetsEBshow -> Get("HH_dEta[2]");
HH_dPhi[10]   = (TH1F*)fileWjetsEBshow -> Get("HH_dPhi[2]");
HH_HoE[10]    = (TH1F*)fileWjetsEBshow -> Get("HH_HoE[2]");
HH_S9S25[10]  = (TH1F*)fileWjetsEBshow -> Get("HH_S9S25[2]");
HH_See[10]    = (TH1F*)fileWjetsEBshow -> Get("HH_See[2]");
HH_EoPout[10] = (TH1F*)fileWjetsEBshow -> Get("HH_EoPout[2]");
HL_dEta[10]   = (TH1F*)fileWjetsEBshow -> Get("HL_dEta[2]");
HL_dPhi[10]   = (TH1F*)fileWjetsEBshow -> Get("HL_dPhi[2]");
HL_HoE[10]    = (TH1F*)fileWjetsEBshow -> Get("HL_HoE[2]");
HL_S9S25[10]  = (TH1F*)fileWjetsEBshow -> Get("HL_S9S25[2]");
HL_See[10]    = (TH1F*)fileWjetsEBshow -> Get("HL_See[2]");
HL_EoPout[10] = (TH1F*)fileWjetsEBshow -> Get("HL_EoPout[2]");

// showering, wjets, EE
HH_dEta[11]   = (TH1F*)fileWjetsEEshow -> Get("HH_dEta[3]");
HH_dPhi[11]   = (TH1F*)fileWjetsEEshow -> Get("HH_dPhi[3]");
HH_HoE[11]    = (TH1F*)fileWjetsEEshow -> Get("HH_HoE[3]");
HH_S9S25[11]  = (TH1F*)fileWjetsEEshow -> Get("HH_S9S25[3]");
HH_See[11]    = (TH1F*)fileWjetsEEshow -> Get("HH_See[3]");
HH_EoPout[11] = (TH1F*)fileWjetsEEshow -> Get("HH_EoPout[3]");
HL_dEta[11]   = (TH1F*)fileWjetsEEshow -> Get("HL_dEta[3]");
HL_dPhi[11]   = (TH1F*)fileWjetsEEshow -> Get("HL_dPhi[3]");
HL_HoE[11]    = (TH1F*)fileWjetsEEshow -> Get("HL_HoE[3]");
HL_S9S25[11]  = (TH1F*)fileWjetsEEshow -> Get("HL_S9S25[3]");
HL_See[11]    = (TH1F*)fileWjetsEEshow -> Get("HL_See[3]");
HL_EoPout[11] = (TH1F*)fileWjetsEEshow -> Get("HL_EoPout[3]");


// rescaling
double scale_HH_dEta[12];
double scale_HH_dPhi[12];
double scale_HH_HoE[12];
double scale_HH_S9S25[12];
double scale_HH_See[12];
double scale_HH_EoPout[12];
double scale_HL_dEta[12];
double scale_HL_dPhi[12];
double scale_HL_HoE[12];
double scale_HL_S9S25[12];
double scale_HL_See[12];
double scale_HL_EoPout[12];

for(int ii=0; ii<12; ii++){

  scale_HH_dEta[ii]   = 1./HH_dEta[ii].GetSum();
  scale_HH_dPhi[ii]   = 1./HH_dPhi[ii].GetSum();
  scale_HH_HoE[ii]    = 1./HH_HoE[ii].GetSum();
  scale_HH_S9S25[ii]  = 1./HH_S9S25[ii].GetSum();
  scale_HH_See[ii]    = 1./HH_See[ii].GetSum();
  scale_HH_EoPout[ii] = 1./HH_EoPout[ii].GetSum();
  scale_HL_dEta[ii]   = 1./HL_dEta[ii].GetSum();
  scale_HL_dPhi[ii]   = 1./HL_dPhi[ii].GetSum();
  scale_HL_HoE[ii]    = 1./HL_HoE[ii].GetSum();
  scale_HL_S9S25[ii]  = 1./HL_S9S25[ii].GetSum();
  scale_HL_See[ii]    = 1./HL_See[ii].GetSum();
  scale_HL_EoPout[ii] = 1./HL_EoPout[ii].GetSum();
}

for(int ii=0; ii<12; ii++){
  HH_dEta[ii].  Scale(scale_HH_dEta[ii]);
  HH_dPhi[ii].  Scale(scale_HH_dPhi[ii]);
  HH_HoE[ii].   Scale(scale_HH_HoE[ii]);
  HH_S9S25[ii]. Scale(scale_HH_S9S25[ii]);
  HH_See[ii].   Scale(scale_HH_See[ii]);
  HH_EoPout[ii].Scale(scale_HH_EoPout[ii]);
  HL_dEta[ii].  Scale(scale_HL_dEta[ii]);
  HL_dPhi[ii].  Scale(scale_HL_dPhi[ii]);
  HL_HoE[ii].   Scale(scale_HL_HoE[ii]);
  HL_S9S25[ii]. Scale(scale_HL_S9S25[ii]);
  HL_See[ii].   Scale(scale_HL_See[ii]);
  HL_EoPout[ii].Scale(scale_HL_EoPout[ii]);
}

// cosmetics
if(!whichStudy){
  for(int ii=0; ii<12; ii++){
    HH_dEta[ii].  SetFillColor(1);    HH_dEta[ii].  SetFillStyle(3004);
    HH_dPhi[ii].  SetFillColor(1);    HH_dPhi[ii].  SetFillStyle(3004);
    HH_HoE[ii].   SetFillColor(1);    HH_HoE[ii].   SetFillStyle(3004);
    HH_S9S25[ii]. SetFillColor(1);    HH_S9S25[ii]. SetFillStyle(3004);
    HH_See[ii].   SetFillColor(1);    HH_See[ii].   SetFillStyle(3004);
    HH_EoPout[ii].SetFillColor(1);    HH_EoPout[ii].SetFillStyle(3004);
    HL_dEta[ii].  SetFillColor(3);    HL_dEta[ii].  SetFillStyle(3005);
    HL_dPhi[ii].  SetFillColor(3);    HL_dPhi[ii].  SetFillStyle(3005);
    HL_HoE[ii].   SetFillColor(3);    HL_HoE[ii].   SetFillStyle(3005);
    HL_S9S25[ii]. SetFillColor(3);    HL_S9S25[ii]. SetFillStyle(3005);
    HL_See[ii].   SetFillColor(3);    HL_See[ii].   SetFillStyle(3005);
    HL_EoPout[ii].SetFillColor(3);    HL_EoPout[ii].SetFillStyle(3005);
  }
}

if(whichStudy){

  for(int ii=0; ii<12; ii++){
    HL_dEta[ii].  SetLineWidth(2);    
    HL_dPhi[ii].  SetLineWidth(2);    
    HL_HoE[ii].   SetLineWidth(2);    
    HL_S9S25[ii]. SetLineWidth(2);    
    HL_See[ii].   SetLineWidth(2);    
    HL_EoPout[ii].SetLineWidth(2);    
  }

  for(int ii=0; ii<4; ii++){
    HL_dEta[ii].  SetLineColor(2);    
    HL_dPhi[ii].  SetLineColor(2);    
    HL_HoE[ii].   SetLineColor(2);    
    HL_S9S25[ii]. SetLineColor(2);    
    HL_See[ii].   SetLineColor(2);    
    HL_EoPout[ii].SetLineColor(2);    
  }
  for(int ii=4; ii<8; ii++){
    HL_dEta[ii].  SetLineColor(3);    
    HL_dPhi[ii].  SetLineColor(3);    
    HL_HoE[ii].   SetLineColor(3);    
    HL_S9S25[ii]. SetLineColor(3);    
    HL_See[ii].   SetLineColor(3);    
    HL_EoPout[ii].SetLineColor(3);    
  }
  for(int ii=8; ii<12; ii++){
    HL_dEta[ii].  SetLineColor(4);    
    HL_dPhi[ii].  SetLineColor(4);    
    HL_HoE[ii].   SetLineColor(4);    
    HL_S9S25[ii]. SetLineColor(4);    
    HL_See[ii].   SetLineColor(4);    
    HL_EoPout[ii].SetLineColor(4);    
  }
}


// axes titles
for(int ii=0; ii<12; ii++){
  HH_dEta[ii].  GetXaxis()->SetTitle("#Delta #eta");
  HH_dPhi[ii].  GetXaxis()->SetTitle("#Delta #phi");
  HH_HoE[ii].   GetXaxis()->SetTitle("H/E");
  HH_S9S25[ii]. GetXaxis()->SetTitle("S9/S25");
  HH_See[ii].   GetXaxis()->SetTitle("#sigma_{#eta #eta}");
  HH_EoPout[ii].GetXaxis()->SetTitle("E/P_{out}");
  HL_dEta[ii].  GetXaxis()->SetTitle("#Delta #eta");
  HL_dPhi[ii].  GetXaxis()->SetTitle("#Delta #phi");
  HL_HoE[ii].   GetXaxis()->SetTitle("H/E");
  HL_S9S25[ii]. GetXaxis()->SetTitle("S9/S25");
  HL_See[ii].   GetXaxis()->SetTitle("#sigma_{#eta #eta}");
  HL_EoPout[ii].GetXaxis()->SetTitle("E/P_{out}");
}


// ------------- plots -----------------------------------

// high vs low pt study
if(!whichStudy){

  TLegend HvsLB2(0.50,0.6,0.75,0.82);
  HvsLB2.AddEntry(HH_dPhi[8], "high p_{T}, Wjets","f");
  HvsLB2.AddEntry(HL_dPhi[8], "low p_{T},  Wjets","f");
  HvsLB2.SetFillColor(0);
  HvsLB2.SetBorderSize(0.4);
  
  TCanvas *cb2gEB = new TCanvas("cb2gEB", "golden, barrel, wjets",1);  
  cb2gEB->SetTitle("golden, barrel");
  HH_See[8].SetTitle("golden, barrel");
  HH_See[8].Draw();
  HL_See[8].Draw("same");
  HvsLB2.Draw();
  cb2gEB->Print("EBgoldenWjets.eps");
  
  TCanvas *cb2gEE = new TCanvas("cb2gEE", "golden, endcap, wjets",1);  
  cb2gEE->SetTitle("golden, endcap");
  HH_See[9].SetTitle("golden, endcap");
  HH_See[9].Draw();
  HL_See[9].Draw("same");
  HvsLB2.Draw();
  cb2gEE->Print("EEgoldenWjets.eps");
  
  TCanvas *cb2sEB = new TCanvas("cb2sEB", "showering, barrel, wjets",1);  
  cb2sEB->SetTitle("showering, barrel");
  HH_See[10].SetTitle("showering, barrel");
  HH_See[10].Draw();
  HL_See[10].Draw("same");
  HvsLB2.Draw();
  cb2sEB->Print("EBshoweringWjets.eps");
  
  TCanvas *cb2sEE = new TCanvas("cb2sEE", "showering, endcap, wjets",1);  
  cb2sEE->SetTitle("showering, endcap");
  HH_See[11].SetTitle("showering, endcap");
  HH_See[11].Draw();
  HL_See[11].Draw("same");
  HvsLB2.Draw();
  cb2sEE->Print("EEshoweringWjets.eps");
}

// signal vs background study
if(whichStudy){

  TLegend SvsB(0.50,0.6,0.75,0.82);
  SvsB.AddEntry(HL_dPhi[0], "signal","f");
  SvsB.AddEntry(HL_dPhi[4], "ttjets","f");
  SvsB.AddEntry(HL_dPhi[8], "wjets","f");
  SvsB.SetFillColor(0);
  SvsB.SetBorderSize(0.4);
    
  TCanvas *cgEB = new TCanvas("cgEB", "golden, barrel",1);  
  cgEB->SetTitle("golden, barrel");
  // cgEB.SetLogy();
  HL_dPhi[8].SetTitle("golden, barrel");
  HL_dPhi[8].Draw();
  HL_dPhi[4].Draw("same");
  HL_dPhi[0].Draw("same");
  SvsB.Draw();
  cgEB->Print("EBgoldenSignalVsBack.eps");

  TCanvas *cgEE = new TCanvas("cgEE", "golden, endcap",1);  
  cgEB->SetTitle("golden, endcap");
  // cgEE.SetLogy();
  HL_dPhi[9].SetTitle("golden, endcap");
  HL_dPhi[9].Draw();
  HL_dPhi[5].Draw("same");
  HL_dPhi[1].Draw("same");
  SvsB.Draw();
  cgEE->Print("EEgoldenSignalVsBack.eps");

  TCanvas *csEB = new TCanvas("csEB", "showering, barrel",1);  
  cgEB->SetTitle("showering, barrel");
  // csEB.SetLogy();
  HL_dPhi[2].SetTitle("showering, barrel");
  HL_dPhi[2].Draw();
  HL_dPhi[6].Draw("same");
  HL_dPhi[10].Draw("same");
  SvsB.Draw();
  csEB->Print("EBshoweringSignalVsBack.eps");  

  TCanvas *csEE = new TCanvas("csEE", "showering, endcap",1);  
  cgEB->SetTitle("showering, endcap");
  // csEE.SetLogy();
  HL_dPhi[3].SetTitle("showering, endcap");
  HL_dPhi[3].Draw();
  HL_dPhi[7].Draw("same");
  HL_dPhi[11].Draw("same");
  SvsB.Draw();
  csEE->Print("EEshoweringSignalVsBack.eps");

}
}
