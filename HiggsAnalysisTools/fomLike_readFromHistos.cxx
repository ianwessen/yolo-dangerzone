#include <TFile.h>
#include <TChain.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <iostream>

void fomFromHistos() {
  
  // -----------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  //                                                                                        //
  // NOTATION: variable, barrel/endcap, pT                                                  //
  // [jet_pt_range]                                                                         // 
  // [ecalsubdet]: 0 = EB, 1 = EE                                                           //
  // [ptbin]:      0 = < 15GeV; 1 = > 15 GeV                                                //
  // classification: unsplit                                                                //  
  //                                                                                        // 
  // -----------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  //                                                                                        //
  // input files notation (Emanuele's macro):                                               // 
  // ecalsubdet: 0 = EB; 1 = EE                                                             // 
  // ptBin: 0 = < 15GeV; 1 = > 15 GeV                                                       //
  //                                                                                        //
  // -----------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------


  // input files with histos - to be set
  TFile *_fJet[6];
  TFile *_fEle    = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_zllIdeal.root");
  _fJet[0] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_015_notIsol.root");
  _fJet[1] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_1520_notIsol.root");
  _fJet[2] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_2030_notIsol.root");
  _fJet[3] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_3050_notIsol.root");
  _fJet[4] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_5080_notIsol.root");
  _fJet[5] = new TFile("/data/crovelli/HiggsAnalysis_2008/OfflineAnalysis/HiggsAnalysisTools/pdfs_80120_notIsol.root");
  int nBin = 100;  
  int nPtBin = 6;  
  double nEntriesEleStep[2][2][101];      // 101 = nBin + 1  
  double nEntriesJetStep[6][2][2][101];   // 101 = nBin + 1 
  double effSgn[2][2][101];               // 101 = nBin + 1
  double rejBkg[6][2][2][101];            // 101 = nBin + 1
  double nEntriesEleTot[2][2];   
  double nEntriesJetTot[6][2][2];   

  // fina graphs
  TGraph *effVsbkgRej[6][2][2]; 
  
  // -------------------------------------------------
  // vectors
  for(int uudet=0; uudet<2; uudet++){
    for(int uupt=0; uupt<2; uupt++){
      for(int uustep=0; uustep<nBin; uustep++){
	nEntriesEleStep[uudet][uupt][uustep] = 0.;
	nEntriesEleTot[uudet][uupt]          = 0.;
	effSgn[uudet][uupt][uustep]          = 0.;
 	for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
	  nEntriesJetStep[uujetpt][uudet][uupt][uustep] = 0.;
	  nEntriesJetTot[uujetpt][uudet][uupt]          = 0.;
	  rejBkg[uujetpt][uudet][uupt][uustep]          = 0.;
	}
      }}}
  

  // -------------------------------------------------
  // taking infos from histos - signal
  for(int uudet=0; uudet<2; uudet++){
    for(int uupt=0; uupt<2; uupt++){
      
      char histoEleName[500];
      sprintf(histoEleName,"likelihoodUnsplit_electrons_%d_%d",uudet,uupt); 	    
      TH1F *eleStep = (TH1F*)_fEle->Get(histoEleName);
      nEntriesEleTot[uudet][uupt] = eleStep->Integral();
      
      for(int uustep=0; uustep<=nBin; uustep++){
	int thisStep = uustep+1;
	nEntriesEleStep[uudet][uupt][uustep] = eleStep->Integral(thisStep,nBin+1);
      }
    }
  }

  // taking infos from histos - background
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    for(int uudet=0; uudet<2; uudet++){
      for(int uupt=0; uupt<2; uupt++){
	
	char histoJetName[500];
	sprintf(histoJetName,"likelihood_hadrons_%d_%d",uudet,uupt);
      	TH1F *jetStep = (TH1F*)_fJet[uujetpt]->Get(histoJetName);
	nEntriesJetTot[uujetpt][uudet][uupt] = jetStep->Integral();
	
	for(int uustep=0; uustep<=nBin; uustep++){
	  int thisStep = uustep+1;
	  nEntriesJetStep[uujetpt][uudet][uupt][uustep] = jetStep->Integral(thisStep,nBin+1);
	}
      }
    }
  }


  // -------------------------------------------------
  // computing efficiencies and rejection factors
  for(int uudet=0; uudet<2; uudet++){
    for(int uupt=0; uupt<2; uupt++){
      for(int uustep=0; uustep<=nBin; uustep++){
	effSgn[uudet][uupt][uustep] = nEntriesEleStep[uudet][uupt][uustep]/nEntriesEleTot[uudet][uupt];
	for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
	  rejBkg[uujetpt][uudet][uupt][uustep] = 1.- (nEntriesJetStep[uujetpt][uudet][uupt][uustep]/nEntriesJetTot[uujetpt][uudet][uupt]);
	}
      }}}

  // -------------------------------------------------
  // efficiency vs rejection studies
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    for(int uudet=0; uudet<2; uudet++){
      for(int uupt=0; uupt<2; uupt++){	  
	effVsbkgRej[uujetpt][uudet][uupt] = new TGraph(nBin+1,effSgn[uudet][uupt],rejBkg[uujetpt][uudet][uupt]);
      }}}
  
  
  // -------------------------------------------------
  // colors and styles
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    for(int uudet=0; uudet<2; uudet++){
      for(int uupt=0; uupt<2; uupt++){	  
	if(uujetpt==0) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(1); 
	if(uujetpt==1) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(2); 
	if(uujetpt==2) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(3); 
	if(uujetpt==3) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(4); 
	if(uujetpt==4) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(5); 
	if(uujetpt==5) effVsbkgRej[uujetpt][uudet][uupt]->SetLineColor(6); 
	effVsbkgRej[uujetpt][uudet][uupt]->SetLineStyle(1); 
      }}}
  
  
  
  // -------------------------------------------------
  // plots
  gStyle->SetOptStat(0000);
  
  TLegend* leg = new TLegend(0.15,0.15,0.50,0.35);
  leg->SetFillStyle(0); 
  leg->SetBorderSize(0); 
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(effVsbkgRej[0][0][0],"pT :  0 - 15","l");
  leg->AddEntry(effVsbkgRej[1][0][0],"pT : 15 - 20","l");
  leg->AddEntry(effVsbkgRej[2][0][0],"pT : 20 - 30","l");
  leg->AddEntry(effVsbkgRej[3][0][0],"pT : 30 - 50","l");
  leg->AddEntry(effVsbkgRej[4][0][0],"pT : 50 - 80","l");
  leg->AddEntry(effVsbkgRej[5][0][0],"pT : 80 - 120","l");
  
  TH2F *myFuffa = new TH2F("myFuffa","",100,0.,1.,100,0.,1.);

  TCanvas c1_EB_lt15("c1_EB_lt15","barrel, pt<15",600,600);
  c1_EB_lt15.SetTitle("electron ID figures of merit - barrel, pt < 15");  
  myFuffa->Draw();
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    effVsbkgRej[uujetpt][0][0]->SetLineWidth(2);
    effVsbkgRej[uujetpt][0][0]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uujetpt][0][0]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uujetpt][0][0]->Draw("LP");
  }
  leg->Draw();
  c1_EB_lt15.SaveAs("FOM_rejfactor_EBlt15.eps");

  TCanvas c1_EE_lt15("c1_EE_lt15","endcap, pt < 15",600,600);
  c1_EE_lt15.SetTitle("electron ID figures of merit - endcap, pt < 15");
  myFuffa->Draw();
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    effVsbkgRej[uujetpt][1][0]->SetLineWidth(2);
    effVsbkgRej[uujetpt][1][0]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uujetpt][1][0]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uujetpt][1][0]->Draw("LP");
  }
  leg->Draw();
  c1_EE_lt15.SaveAs("FOM_rejfactor_EElt15.eps");
  
  TCanvas c1_EB_gt15("c1_EB_lt15","barrel, pt>15",600,600);
  c1_EB_gt15.SetTitle("electron ID figures of merit - barrel, pt > 15");  
  myFuffa->Draw();
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    effVsbkgRej[uujetpt][0][1]->SetLineWidth(2);
    effVsbkgRej[uujetpt][0][1]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uujetpt][0][1]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uujetpt][0][1]->Draw("LP");
  }
  leg->Draw();
  c1_EB_gt15.SaveAs("FOM_rejfactor_EBgt15.eps");

  TCanvas c1_EE_gt15("c1_EE_gt15","endcap, pt > 15",600,600);
  c1_EE_gt15.SetTitle("electron ID figures of merit - endcap, pt > 15");
  myFuffa->Draw();
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    effVsbkgRej[uujetpt][1][1]->SetLineWidth(2);
    effVsbkgRej[uujetpt][1][1]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uujetpt][1][1]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uujetpt][1][1]->Draw("LP");
  }
  leg->Draw();
  c1_EE_gt15.SaveAs("FOM_rejfactor_EEgt15.eps");
  
  
  // -------------------------------------------------
  // outputfile
  TFile *fileFOM = new TFile ("FOM_rejfactor.root","RECREATE","istogrammi");
  for(int uujetpt=0; uujetpt<nPtBin; uujetpt++){
    for(int uudet=0; uudet<2; uudet++){
      for(int uupt=0; uupt<2; uupt++){	  
	char title[500];
	sprintf(title, "effVsbkgRej[%d][%d][%d]", uujetpt,uudet, uupt);
	effVsbkgRej[uujetpt][uudet][uupt]->Write(title);
      }}}
}
