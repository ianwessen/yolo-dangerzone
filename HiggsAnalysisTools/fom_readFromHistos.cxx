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
  // NOTATION: variable, barrel, endcap, classification                                     //
  // variable: 0=sEE, 1=sPP, 2=sPE, 3=s1/s9, 4=s9/s25, 5=lat, 6=etaLat, 7=phiLat, 8=A20     //
  // [ecalsubdet]: 0 = EB, 1 = EE                                                           //
  // [ptbin]:      0 = < 15GeV; 1 = > 15 GeV                                                //
  // [class]:      0 = non showering; 1 = showering; 2 = allClasses                         //
  //                                                                                        //
  // -----------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------
  //                                                                                        //
  // input files notation (Emanuele's macro):                                               // 
  // ecalsubdet: 0 = EB; 1 = EE                                                             // 
  // ptBin: 0 = < 15GeV; 1 = > 15 GeV                                                       //
  // class: 0 = non-showering; 1 = showering                                                //
  // fullclass: 0 = golden; 1 = bigbrem; 2 = narrow; 3 = showering                          // 
  //                                                                                        //
  // -----------------------------------------------------------------------------------------
  // -----------------------------------------------------------------------------------------


  // input files with histos - to be set
  // TFile *_fileEle = new TFile("/afs/cern.ch/user/e/emanuele/public/4Likelihood/pdfs168DiEle.root");
  // TFile *_fileJet = new TFile("/afs/cern.ch/user/e/emanuele/public/4Likelihood/pdfs168QCDJets2550GeV.root");
  TFile *_fileEle = new TFile("/afs/cern.ch/user/e/emanuele/public/4Likelihood/pdfs168DiEleTkLooseIsolated.root");
  TFile *_fileJet = new TFile("/afs/cern.ch/user/e/emanuele/public/4Likelihood/pdfs168QCDJets2550GeVTkLooseIsolated.root");
  int nBin = 100;  
  double nEntriesEleStep[9][2][2][3][100];   // 100 = nBin   
  double nEntriesJetStep[9][2][2][3][100];   // 100 = nBin
  double effSgn[9][2][2][3][100];            // 100 = nBin
  double rejBkg[9][2][2][3][100];            // 100 = nBin
  double nEntriesEleTot[9][2][2][3];   
  double nEntriesJetTot[9][2][2][3];   


  // -------------------------------------------------
  // vectors
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){
	  for(int uustep=0; uustep<nBin; uustep++){
	    nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep] = 0.;
	    nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep] = 0.;
	    nEntriesEleTot[uuvar][uudet][uupt][uuclass]          = 0.;
	    nEntriesJetTot[uuvar][uudet][uupt][uuclass]          = 0.;
	    effSgn[uuvar][uudet][uupt][uuclass][uustep]          = 0.;
	    rejBkg[uuvar][uudet][uupt][uuclass][uustep]          = 0.;
	  }}}}}


  // -------------------------------------------------
  // taking infos from histos
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){

      char variableName[200];  
      if (uuvar==0) { sprintf(variableName,"sigmaEtaEta"); }     
      if (uuvar==1) { sprintf(variableName,"sigmaPhiPhi"); }     
      if (uuvar==2) { sprintf(variableName,"sigmaEtaPhi"); }     
      if (uuvar==3) { sprintf(variableName,"s1s9");        }     
      if (uuvar==4) { sprintf(variableName,"s9s25");       }     
      if (uuvar==5) { sprintf(variableName,"LAT");         }     
      if (uuvar==6) { sprintf(variableName,"etaLAT");      }     
      if (uuvar==7) { sprintf(variableName,"phiLAT");      }     
      if (uuvar==8) { sprintf(variableName,"a20");         }      
      
      char className[200];
      if (uuclass==0) { sprintf(className,"Class"); }     
      if (uuclass==1) { sprintf(className,"Class"); }     
      if (uuclass==2) { sprintf(className,"Unsplit"); }     
      
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){

	  char histoEleName[500];
	  if (uuclass==0) { sprintf(histoEleName,"%s%s_electrons_%d_%d_%d",variableName,className,uudet,uupt,uuclass);}      	    
	  if (uuclass==1) { sprintf(histoEleName,"%s%s_electrons_%d_%d_%d",variableName,className,uudet,uupt,uuclass);}      	    
	  if (uuclass==2) { sprintf(histoEleName,"%s%s_electrons_%d_%d",   variableName,className,uudet,uupt); }	    
	  
	  char histoJetName[500];
	  sprintf(histoJetName,"%s_hadrons_%d_%d",variableName,uudet,uupt);
	  
	  TH1F *eleStep = (TH1F*)_fileEle->Get(histoEleName);
	  TH1F *jetStep = (TH1F*)_fileJet->Get(histoJetName);
	  nEntriesEleTot[uuvar][uudet][uupt][uuclass] = eleStep->GetSum();
	  nEntriesJetTot[uuvar][uudet][uupt][uuclass] = jetStep->GetSum();
	  
	  double tmpEleStep = 0.; 
	  double tmpJetStep = 0.;
	  
	  for(int uustep=0; uustep<nBin; uustep++){
	    
	    int thisStep = uustep+1;
	    if( uuvar==0 || uuvar==1 || uuvar==2 || uuvar==5 || uuvar==6 || uuvar==7 ){ 
	      if (uustep==0){ 
		nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep] = eleStep->GetBinContent(thisStep);  
		nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep] = jetStep->GetBinContent(thisStep);  
	      }
	      else {
		nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep] = nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep-1] + eleStep->GetBinContent(thisStep);  
		nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep] = nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep-1] + jetStep->GetBinContent(thisStep);  
	      }
	    } 
	    else if ( uuvar==3 || uuvar==4 || uuvar==8){
	      tmpEleStep = tmpEleStep + eleStep->GetBinContent(thisStep);  
	      tmpJetStep = tmpJetStep + jetStep->GetBinContent(thisStep);  
	      nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep] = nEntriesEleTot[uuvar][uudet][uupt][uuclass] - tmpEleStep;
	      nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep] = nEntriesJetTot[uuvar][uudet][uupt][uuclass] - tmpJetStep;
	    }
	  }
	}
      }
    }    
  }


  // -------------------------------------------------
  // computing efficiencies and rejection factors
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){
	  for(int uustep=0; uustep<nBin; uustep++){
	    effSgn[uuvar][uudet][uupt][uuclass][uustep] = nEntriesEleStep[uuvar][uudet][uupt][uuclass][uustep]/nEntriesEleTot[uuvar][uudet][uupt][uuclass];
	    rejBkg[uuvar][uudet][uupt][uuclass][uustep] = 1.- (nEntriesJetStep[uuvar][uudet][uupt][uuclass][uustep]/nEntriesJetTot[uuvar][uudet][uupt][uuclass]);
	  }}}}}

  // -------------------------------------------------
  // efficiency vs rejection studies
  TGraph *effVsbkgRej[9][2][2][3]; 
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){	  
	  effVsbkgRej[uuvar][uudet][uupt][uuclass] = new TGraph(nBin,effSgn[uuvar][uudet][uupt][uuclass],rejBkg[uuvar][uudet][uupt][uuclass]);
	}}}}


  // -------------------------------------------------
  // colors and styles
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){	  
	  if(uuvar==0){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(1); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(1); }
	  if(uuvar==1){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(2); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(1); }
	  if(uuvar==2){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(3); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(1); }
	  if(uuvar==3){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(4); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(1); }
	  if(uuvar==4){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(6); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(1); }
	  if(uuvar==5){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(1); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(2); }
	  if(uuvar==6){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(2); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(2); }
	  if(uuvar==7){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(3); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(2); }
	  if(uuvar==8){ effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineColor(4); effVsbkgRej[uuvar][uudet][uupt][uuclass]->SetLineStyle(2); }
	}}}}
  


  // -------------------------------------------------
  // plots
  gStyle->SetOptStat(0000);

  TLegend* leg = new TLegend(0.15,0.15,0.50,0.35);
  leg->SetFillStyle(0); 
  leg->SetBorderSize(0); 
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(effVsbkgRej[0][0][0][0],"#sigma_{#eta #eta}","l");
  leg->AddEntry(effVsbkgRej[1][0][0][0],"#sigma_{#phi #phi}","l");
  leg->AddEntry(effVsbkgRej[2][0][0][0],"#sigma_{#eta #phi}","l");
  leg->AddEntry(effVsbkgRej[3][0][0][0],"S1/S9","l");
  leg->AddEntry(effVsbkgRej[4][0][0][0],"S9/S25","l");
  leg->AddEntry(effVsbkgRej[5][0][0][0],"Lat","l");
  leg->AddEntry(effVsbkgRej[6][0][0][0],"Lat_{#eta}","l");
  leg->AddEntry(effVsbkgRej[7][0][0][0],"Lat_{#phi}","l");
  leg->AddEntry(effVsbkgRej[8][0][0][0],"A20","l");

  TH2F *myFuffa = new TH2F("myFuffa","",100,0.,1.,100,0.,1.);

  TCanvas c1_EB_lt15("c1_EB_lt15","barrel, pt<15",600,600);
  c1_EB_lt15.SetTitle("electron ID figures of merit - barrel, pt < 15");  
  myFuffa->Draw();
  for(int uu=0; uu<9; uu++){
    effVsbkgRej[uu][0][0][2]->SetLineWidth(2);
    effVsbkgRej[uu][0][0][2]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uu][0][0][2]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uu][0][0][2]->Draw("LP");
  }
  leg->Draw();
  c1_EB_lt15.SaveAs("FOM_rejfactor_EBlt15.eps");

  TCanvas c1_EE_lt15("c1_EE_lt15","endcap, pt < 15",600,600);
  c1_EE_lt15.SetTitle("electron ID figures of merit - endcap, pt < 15");
  myFuffa->Draw();
  for(int uu=0; uu<9; uu++){
    effVsbkgRej[uu][1][0][2]->SetLineWidth(2);
    effVsbkgRej[uu][1][0][2]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uu][1][0][2]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uu][1][0][2]->Draw("LP");
  }
  leg->Draw();
  c1_EE_lt15.SaveAs("FOM_rejfactor_EElt15.eps");

  TCanvas c1_EB_gt15("c1_EB_lt15","barrel, pt>15",600,600);
  c1_EB_gt15.SetTitle("electron ID figures of merit - barrel, pt > 15");  
  myFuffa->Draw();
  for(int uu=0; uu<9; uu++){
    effVsbkgRej[uu][0][1][2]->SetLineWidth(2);
    effVsbkgRej[uu][0][1][2]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uu][0][1][2]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uu][0][1][2]->Draw("LP");
  }
  leg->Draw();
  c1_EB_gt15.SaveAs("FOM_rejfactor_EBgt15.eps");

  TCanvas c1_EE_gt15("c1_EE_gt15","endcap, pt > 15",600,600);
  c1_EE_gt15.SetTitle("electron ID figures of merit - endcap, pt > 15");
  myFuffa->Draw();
  for(int uu=0; uu<9; uu++){
    effVsbkgRej[uu][1][1][2]->SetLineWidth(2);
    effVsbkgRej[uu][1][1][2]->GetXaxis()->SetTitle("signal (pure electrons) efficiency - barrel");
    effVsbkgRej[uu][1][1][2]->GetYaxis()->SetTitle("background (pure jets) rejection - barrel");
    effVsbkgRej[uu][1][1][2]->Draw("LP");
  }
  leg->Draw();
  c1_EE_gt15.SaveAs("FOM_rejfactor_EEgt15.eps");


  // -------------------------------------------------
  // outputfile
  TFile *fileFOM = new TFile ("FOM_rejfactor.root","RECREATE","istogrammi");
  for(int uuvar=0; uuvar<9; uuvar++){
    for(int uuclass=0; uuclass<3; uuclass++){
      for(int uudet=0; uudet<2; uudet++){
	for(int uupt=0; uupt<2; uupt++){	  
	  char title[500];
	  sprintf(title, "effVsbkgRej[%d][%d][%d][%d]", uuvar, uudet, uupt, uuclass);
	  effVsbkgRej[uuvar][uudet][uupt][uuclass]->Write(title);
	}}}}
}
