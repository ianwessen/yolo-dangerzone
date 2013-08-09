#include <TFile.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TString.h>
#include <TH2F.h>
#include <TMath.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <math.h>

#include "YieldMaker.h"
#include "FitSelection.hh"

using namespace std;

string getChannelSuffix(int channel) {
  if(channel==of0j) return string("of_0j");
  if(channel==of1j) return string("of_1j");
  if(channel==sf0j) return string("sf_0j");
  if(channel==sf1j) return string("sf_1j");
  return string("ERROR! Unclassified channel!");
}

string getStringChannel(int channel) {
  if(channel==of0j) return string("channel>=2 && njet==0");
  if(channel==of1j) return string("channel>=2 && njet==1");
  if(channel==sf0j) return string("channel<2 && njet==0");
  if(channel==sf1j) return string("channel<2 && njet==1");
  return string("ERROR! Unclassified channel!");
}

void cutMinimum(TH2F* h, float threshold) {
  TH2F* hist = (TH2F*)(h->Clone((std::string(h->GetName())+"_temp").c_str()));
  for (int i = 1; i <= hist->GetNbinsX(); ++i) {
     for(int j = 1; j <= hist->GetNbinsY(); ++j) {
       float val = hist->GetBinContent(i,j);
       if(val<threshold) h->SetBinContent(i,j,threshold);
     }
  }
}

void fakeAlternativeShapes(TH2F* h, TH2F *hUp, TH2F *hDn) {
  for (int i = 1; i <= h->GetNbinsX(); ++i) { 
    for(int j = 1; j <= h->GetNbinsY(); ++j) {
      double alpha = 1.0 + 0.5*(j-0.5*h->GetNbinsY())/h->GetNbinsY();
      double beta  = 1.0/pow(alpha,1.5);
      hUp->SetBinContent(i,j, alpha*h->GetBinContent(i,j));
      hDn->SetBinContent(i,j, beta*h->GetBinContent(i,j));
    }
  }
}

void smooth(TH2F* h, float threshold) {
    TH2F* hist = (TH2F*)(h->Clone((std::string(h->GetName())+"_temp").c_str()));
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        for(int j = 1; j <= hist->GetNbinsY(); ++j) {
            float count = 0.;
            float val = hist->GetBinContent(i,j);
            if (val<threshold) {
                if (i-1 != 0)                                                  val += hist->GetBinContent(i-1,j  );
                else                                                           count -= 1.;
                if (i+1 != hist->GetNbinsX()+1)                                val += hist->GetBinContent(i+1,j  );
                else                                                           count -= 1.;
                if (j-1 != 0)                                                  val += hist->GetBinContent(i  ,j-1);
                else                                                           count -= 1.;
                if (j+1 != hist->GetNbinsY()+1)                                val += hist->GetBinContent(i  ,j+1);
                else                                                           count -= 1.;
                if (i-1 != 0 && j-1 != 0)                                      val += hist->GetBinContent(i-1,j-1);
                else                                                           count -= 1.;
                if (i-1 != 0 && j+1 != hist->GetNbinsY()+1)                    val += hist->GetBinContent(i-1,j+1);
                else                                                           count -= 1.;
                if (i+1 != hist->GetNbinsX()+1 && j-1 != 0)                    val += hist->GetBinContent(i+1,j-1);
                else                                                           count -= 1.;
                if (i+1 != hist->GetNbinsX()+1 && j+1 != hist->GetNbinsY()+1)  val += hist->GetBinContent(i+1,j+1);
                else                                                           count -= 1.;
                val /= (9.0+count);
                h->SetBinContent(i,j,val);
            }
        }
    }
}

void smoothSwiss(TH2F* h, float threshold) {
    TH2F* hist = (TH2F*)(h->Clone((std::string(h->GetName())+"_temp").c_str()));
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        for(int j = 1; j <= hist->GetNbinsY(); ++j) {
            float val = hist->GetBinContent(i,j);
            float count = 0.;
            if (val < threshold) {
                if (i-1 != 0)                                                  val += hist->GetBinContent(i-1,j  );
                else                                                           count -= 1.0;;
                if (i+1 != hist->GetNbinsX()+1)                                val += hist->GetBinContent(i+1,j  );
                else                                                           count -= 1.0;
                if (j-1 != 0)                                                  val += hist->GetBinContent(i  ,j-1);
                else                                                           count -= 1.0;;
                if (j+1 != hist->GetNbinsY()+1)                                val += hist->GetBinContent(i  ,j+1);
                else                                                           count -= 1.0;
                val /= (5.0+count);
                h->SetBinContent(i,j,val);
            }
        }
    }
}

void smoothVertical(TH2F* h, float threshold) {
    TH2F* hist = (TH2F*)(h->Clone((std::string(h->GetName())+"_vert").c_str()));
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        for(int j = 1; j <= hist->GetNbinsY(); ++j) {
            float val = hist->GetBinContent(i,j);
            float count = 0.;
            if (val < threshold) {
                if (j-1 != 0)                                                  val += hist->GetBinContent(i  ,j-1);
                else                                                           count -= 1.0;;
                if (j+1 != hist->GetNbinsY()+1)                                val += hist->GetBinContent(i  ,j+1);
                else                                                           count -= 1.0;
                val /= (3.0+count);
                h->SetBinContent(i,j,val);
            }
        }
    }
}

void smoothHorizontal(TH2F* h, float threshold) {
    TH2F* hist = (TH2F*)(h->Clone((std::string(h->GetName())+"_hori").c_str()));
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        for(int j = 1; j <= hist->GetNbinsY(); ++j) {
            float val = hist->GetBinContent(i,j);
            float count = 0.;
            if (val < threshold) { 
                if (i-1 != 0)                                                  val += hist->GetBinContent(i-1,j  );
                else                                                           count -= 1.0;;
                if (i+1 != hist->GetNbinsX()+1)                                val += hist->GetBinContent(i+1,j  );
                else                                                           count -= 1.0;
                val /= (3.0+count);
                h->SetBinContent(i,j,val);
            }
        }
    }
}


void normalize(TH2F* hist) {
    for (int i = 1; i <= hist->GetNbinsX(); ++i) {
        double histsum = 0;
        for(int j = 1; j <= hist->GetNbinsY(); ++j) histsum += hist->GetBinContent(i,j);
        if (histsum > 0) histsum = 1.0/histsum;
        for(int j = 1; j <= hist->GetNbinsY(); ++j) hist->SetBinContent(i,j, hist->GetBinContent(i,j) * histsum);
    }
}

void all(int cha, float dphiMin, float dphiMax, bool do7TeV) {

  cout << "Filling 2D maps for channel " << cha << endl;

  FitSelection sel;
  sel.mrmin=50;
  sel.mrmax=500;
  sel.dphimin=dphiMin;
  sel.dphimax=dphiMax;

  gStyle->SetOptStat(0);

  DataYieldMaker  ymaker_data;
  YieldMaker      ymaker_hi;
  YieldMaker      ymaker_qqww;
  YieldMaker      ymaker_ggww;
  YieldMaker      ymaker_top;
  YieldMaker      ymaker_dysf;
  YieldMaker      ymaker_dyof;
  YieldMaker      ymaker_wgstar;
  YieldMaker      ymaker_others;
  WJetsYieldMaker ymaker_wj;

  string treeFolder(do7TeV ? "latinos_tree_skim_of_7TeV" : "latinos_tree_skim_of_8TeV");

  if(do7TeV) {
    ymaker_data   .fill(treeFolder+"/data/latino_100_SingleElectron2011Av4.root");
    ymaker_data   .fill(treeFolder+"/data/latino_101_SingleMuon2011Av4.root");
    ymaker_data   .fill(treeFolder+"/data/latino_102_DoubleElectron2011Av4.root");
    ymaker_data   .fill(treeFolder+"/data/latino_103_DoubleMuon2011Av4.root");
    ymaker_data   .fill(treeFolder+"/data/latino_104_MuEG2011Av4.root");
    ymaker_data   .fill(treeFolder+"/data/latino_120_SingleElectron2011Av6.root");
    ymaker_data   .fill(treeFolder+"/data/latino_121_SingleMuon2011Av6.root");
    ymaker_data   .fill(treeFolder+"/data/latino_122_DoubleElectron2011Av6.root");
    ymaker_data   .fill(treeFolder+"/data/latino_123_DoubleMuon2011Av6.root");
    ymaker_data   .fill(treeFolder+"/data/latino_124_MuEG2011Av6.root");
    ymaker_data   .fill(treeFolder+"/data/latino_140_SingleElectron2011Bv1a.root");
    ymaker_data   .fill(treeFolder+"/data/latino_141_SingleMuon2011Bv1a.root");
    ymaker_data   .fill(treeFolder+"/data/latino_142_DoubleElectron2011Bv1a.root");
    ymaker_data   .fill(treeFolder+"/data/latino_143_DoubleMuon2011Bv1a.root");
    ymaker_data   .fill(treeFolder+"/data/latino_144_MuEG2011Bv1a.root");
    ymaker_data   .fill(treeFolder+"/data/latino_150_SingleElectron2011AMay10.root");
    ymaker_data   .fill(treeFolder+"/data/latino_151_SingleMuon2011AMay10.root");
    ymaker_data   .fill(treeFolder+"/data/latino_152_DoubleMuon2011AMay10.root");
    ymaker_data   .fill(treeFolder+"/data/latino_153_DoubleElectron2011AMay10.root");
    ymaker_data   .fill(treeFolder+"/data/latino_154_MuEG2011AMay10.root");
    ymaker_data   .fill(treeFolder+"/data/latino_160_SingleElectron2011AAug05.root");
    ymaker_data   .fill(treeFolder+"/data/latino_161_SingleMuon2011AAug05.root");
    ymaker_data   .fill(treeFolder+"/data/latino_162_DoubleElectron2011AAug05.root");
    ymaker_data   .fill(treeFolder+"/data/latino_163_DoubleMuon2011AAug05.root");
    ymaker_data   .fill(treeFolder+"/data/latino_164_MuEG2011AAug05.root");
    ymaker_qqww   .fill(treeFolder+"/nominals/latino_000_WWJets2LMad.root");
    ymaker_ggww   .fill(treeFolder+"/nominals/latino_001_GluGluToWWTo4L.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_019_TTTo2L2Nu2B.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_011_TtWFullDR.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_012_TbartWFullDR.root");
    ymaker_dysf   .fill(treeFolder+"/nominals/latino_037_DY50toLLMad.root");
    ymaker_dysf   .fill(treeFolder+"/nominals/latino_036_DY10toLLMad.root");
    ymaker_dyof   .fill(treeFolder+"/dyTemplate/latino_DYtt_2011_added.root");
    ymaker_wj     .fill(treeFolder+"/wjets/WJetsEstimated_Full2011_added.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_082_WGstarToElNuMad.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_083_WGstarToMuNuMad.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_084_WGstarToTauNuMad.root");
    ymaker_others .fill(treeFolder+"/nominals/latino_074_WZJetsMad.root");
    ymaker_others .fill(treeFolder+"/nominals/latino_075_ZZJetsMad.root");
  } else {
    ymaker_data   .fill(treeFolder+"/data/latino_RunA_892pbinv.root");
    ymaker_data   .fill(treeFolder+"/data/latino_RunB_4404pbinv.root");
    ymaker_data   .fill(treeFolder+"/data/latino_RunC_7032pbinv.root");
    ymaker_data   .fill(treeFolder+"/data/latino_RunD_7274pbinv.root");
    ymaker_qqww   .fill(treeFolder+"/nominals/latino_000_WWJets2LMad.root");
    ymaker_ggww   .fill(treeFolder+"/nominals/latino_001_GluGluToWWTo4L.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_019_TTTo2L2Nu2B.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_011_TtWFullDR.root");
    ymaker_top    .fill(treeFolder+"/nominals/latino_012_TbartWFullDR.root");
    ymaker_dysf   .fill(treeFolder+"/nominals/latino_037_DY50toLLMad.root");
    ymaker_dysf   .fill(treeFolder+"/nominals/latino_036_DY10toLLMad.root");
    ymaker_dyof   .fill(treeFolder+"/nominals/latino_DYtt_19.5fb.root");
    ymaker_wj     .fill(treeFolder+"/wjets/latino_LooseLoose_19.5fb.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_082_WGstarToElNuMad.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_083_WGstarToMuNuMad.root");
    ymaker_wgstar .fill(treeFolder+"/nominals/latino_084_WGstarToTauNuMad.root");
    ymaker_others .fill(treeFolder+"/nominals/latino_074_WZJetsMad.root");
    ymaker_others .fill(treeFolder+"/nominals/latino_075_ZZJetsMad.root");
  }

  // === alternative shapes for 2D CondPdf ===
  YieldMaker        ymaker_qqww_up, ymaker_qqww_dn;
  WJetsYieldMaker   ymaker_wj_up(1), ymaker_wj_dn(-1);
  ymaker_qqww_up .fill(treeFolder+"/nominals/latino_004_WWto2L2NuMCatNLOUp.root");
  ymaker_qqww_dn .fill(treeFolder+"/nominals/latino_003_WWto2L2NuMCatNLODown.root");
  if(do7TeV) {
    ymaker_wj_up   .fill(treeFolder+"/wjets/WJetsEstimated_Full2011_added.root");
    ymaker_wj_dn   .fill(treeFolder+"/wjets/WJetsEstimated_Full2011_added.root");
  } else {
    ymaker_wj_up   .fill(treeFolder+"/wjets/latino_LooseLoose_19.5fb.root");
    ymaker_wj_dn   .fill(treeFolder+"/wjets/latino_LooseLoose_19.5fb.root");
  }

  int xNBins = 3;
  Double_t xLowerEdges[xNBins+1];
  xLowerEdges[0]=sel.mrmin;
  xLowerEdges[1]=150;
  xLowerEdges[2]=250;
  xLowerEdges[3]=sel.mrmax;

  float dphiBinSize = 20.0 * TMath::Pi() / 180.; // 20 degrees = 9 bins
  int yNBins = 6;
  Double_t yLowerEdges[yNBins+1];
  yLowerEdges[0]=0.0;
  for(int i=1;i<=3;++i) yLowerEdges[i]=yLowerEdges[i-1]+dphiBinSize;
  yLowerEdges[4]=yLowerEdges[3]+dphiBinSize*2;
  yLowerEdges[5]=yLowerEdges[4]+dphiBinSize*2;
  yLowerEdges[6]=TMath::Pi();

  cout << "Filling the backgrounds now..." << endl;

  TH2F *bkg_qqww   = new TH2F((string("hist2D_bkg_qqww_")+getChannelSuffix(cha)).c_str(),  "",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_ggww   = new TH2F((string("hist2D_bkg_ggww_")+getChannelSuffix(cha)).c_str(),  "",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_top    = new TH2F((string("hist2D_bkg_top_")+getChannelSuffix(cha)).c_str(),   "",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_dy     = new TH2F((string("hist2D_bkg_dy_")+getChannelSuffix(cha)).c_str(),    "",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_wj     = new TH2F((string("hist2D_bkg_wj_")+getChannelSuffix(cha)).c_str(),    "",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_others = new TH2F((string("hist2D_bkg_others_")+getChannelSuffix(cha)).c_str(),"",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *bkg_wgstar = new TH2F((string("hist2D_bkg_wgstar_")+getChannelSuffix(cha)).c_str(),"",xNBins,xLowerEdges,yNBins,yLowerEdges);
  TH2F *sig_higgs  = new TH2F((string("hist2D_sig_")+getChannelSuffix(cha)).c_str(),       "",xNBins,xLowerEdges,yNBins,yLowerEdges);

  TH2F *bkg_qqww_up = (TH2F*)(bkg_qqww->Clone((std::string(bkg_qqww->GetName())+"_Up").c_str()));
  TH2F *bkg_qqww_dn = (TH2F*)(bkg_qqww->Clone((std::string(bkg_qqww->GetName())+"_Dn").c_str()));
  TH2F *bkg_wj_up   = (TH2F*)(bkg_wj->Clone((std::string(bkg_wj->GetName())+"_Up").c_str()));
  TH2F *bkg_wj_dn   = (TH2F*)(bkg_wj->Clone((std::string(bkg_wj->GetName())+"_Dn").c_str()));

  ymaker_qqww.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_qqww);
  ymaker_ggww.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_ggww);
  ymaker_top.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_top);
  if(cha==of0j || cha==of1j) ymaker_dyof.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_dy);
  else ymaker_dysf.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_dy);
  ymaker_wj.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_wj);
  ymaker_wgstar.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_wgstar);
  ymaker_others.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_others);

  // and now the signals
  if(do7TeV) {
    int mH[19] = {110,115,120,122,124,125,126,128,130,140,150,160,170,180,190,200,300,400,500};
    for(int i=0; i<12;i++) {
      if(mH[i]!=125) continue;
      cout << "Filling mass mH = " << mH[i] << "..." << endl;
      char gghsample1[1000], gghsample2[1000], gghsample3[1000], qqhsample[1000];
      if(mH[i]==110 || mH[i]==115 || mH[i]==122 || mH[i]==124 || mH[i]==125 || mH[i]==126 || mH[i]==128) {
        sprintf(gghsample1,"%s/nominals/latino_9%d_ggToH%dtoWWTo2LAndTau2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
        sprintf(qqhsample,"%s/nominals/latino_8%d_vbfToH%dtoWWTo2LAndTau2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
        ymaker_hi   .fill(gghsample1); 
        ymaker_hi   .fill(qqhsample);
      } else {
        sprintf(gghsample1,"%s/nominals/latino_1%d_ggToH%dtoWWto2L2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
        sprintf(gghsample2,"%s/nominals/latino_2%d_ggToH%dtoWWtoLNuTauNu.root",treeFolder.c_str(),mH[i],mH[i]);
        sprintf(gghsample3,"%s/nominals/latino_3%d_ggToH%dtoWWto2Tau2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
        sprintf(qqhsample, "%s/nominals/latino_4%d_vbfToH%dtoWWto2L2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
        ymaker_hi   .fill(gghsample1); 
        ymaker_hi   .fill(gghsample2); 
        ymaker_hi   .fill(gghsample3); 
        ymaker_hi   .fill(qqhsample);
      }
    }
  } else {
    int mH[21] = {110,115,120,125,130,135,140,145,150,155,160,170,180,190,200,250,300,350,400,450,500};
    for(int i=0; i<21;i++) {
      if(mH[i]!=125) continue;
      cout << "Filling mass mH = " << mH[i] << "..." << endl;
      char gghsample[1000], qqhsample[1000];
      sprintf(gghsample,"%s/nominals/latino_1%d_ggToH%dtoWWTo2LAndTau2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
      sprintf(qqhsample,"%s/nominals/latino_2%d_vbfToH%dtoWWTo2LAndTau2Nu.root",treeFolder.c_str(),mH[i],mH[i]);
      ymaker_hi   .fill(gghsample); 
      ymaker_hi   .fill(qqhsample);
    }
  }

  // do not use the xsection to populate the 2D plane, since we need the dphi as a function of Higgs mass
  // we will normalize slices in MR later to make the conditional PDF
  ymaker_hi.get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,sig_higgs,false); 

  // get the systematic 2D shapes
  ymaker_qqww_up .get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_qqww_up);
  ymaker_qqww_dn .get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_qqww_dn);
  ymaker_wj_up   .get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_wj_up);
  ymaker_wj_dn   .get2DHist(cha,sel.mrmin,sel.mrmax,sel.dphimin,sel.dphimax,sel.mtmin,sel.mtmax,bkg_wj_dn);

  // === SMOOTHING ===
  smooth(bkg_dy,     1.0);
  cutMinimum(bkg_wj, 0.0);
  smooth(bkg_wj,     1.0);
  smooth(bkg_wgstar, 1.0);

  // === SMOOTHING SYST ===
  cutMinimum(bkg_wj_up, 0.0);  cutMinimum(bkg_wj_dn, 0.0);
  smooth(bkg_wj_up,     1.0);  smooth(bkg_wj_dn,     1.0);     

  normalize(sig_higgs);
  normalize(bkg_qqww);
  normalize(bkg_ggww);
  normalize(bkg_top);
  normalize(bkg_dy);
  normalize(bkg_wj);
  normalize(bkg_wgstar);
  normalize(bkg_others);

  normalize(bkg_qqww_up); normalize(bkg_qqww_dn);
  normalize(bkg_wj_up);   normalize(bkg_wj_dn);

  
  // === SAVING ===
  TFile *fileOut;
  if(do7TeV) {
    if(cha==0) fileOut = TFile::Open("hww2DShapes_7TeV.root","recreate");
    else fileOut = TFile::Open("hww2DShapes_7TeV.root","update");
  } else {
    if(cha==0) fileOut = TFile::Open("hww2DShapes_8TeV.root","recreate");
    else fileOut = TFile::Open("hww2DShapes_8TeV.root","update");
  }
  fileOut->cd();
  sig_higgs->Write();
  bkg_qqww->Write();
  bkg_ggww->Write();
  bkg_top->Write();
  bkg_dy->Write();
  bkg_wj->Write();
  bkg_wgstar->Write();
  bkg_others->Write();

  bkg_qqww_up->Write(); bkg_qqww_dn->Write();
  bkg_wj_up->Write();   bkg_wj_dn->Write();

  fileOut->Close();

}

void make2DHistogram(bool do7TeV) {
  
  stringstream fileName;
  if(do7TeV) fileName << "hww2DShapes_7TeV.root";
  else fileName << "hww2DShapes_8TeV.root";

  TFile *fileOut = TFile::Open(fileName.str().c_str(),"recreate");
  fileOut->Close();

  float dphiMin, dphiMax;
  dphiMin = 0.0;
  dphiMax = TMath::Pi();

  for(int i=0;i<4;++i) all(i,dphiMin,dphiMax,do7TeV);

}
