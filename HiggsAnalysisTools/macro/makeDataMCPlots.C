// cvs co -d scripts UserCode/Mangano/WWAnalysis/Misc/scripts

#include "TMath.h"
#include "TTree.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TFile.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "LatinoPlot.C"
#include "massDependentCuts.cc"

#include <iostream>

#define NSPECIES 8
#define NVARIABLES 11
#define NCUTS 5
#define JETBINS 2

void makeDataMCPlots(int mH, const char *finalstate, float lumi, bool blindData=false, int signalFactor=1)
{
  gROOT->SetStyle("Plain");
  gROOT->ProcessLine(".x LatinoStyle.C");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);  // Show overflow, underflow + SumOfWeights 
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.0);
  gStyle->SetMarkerColor(1);

  TString suffix="";

  TString species[NSPECIES];
  species[0]="Data";
  if(signalFactor==1) {
    char mass[10];
    sprintf(mass,"H%d",mH);
    species[1]=TString(mass);
  }
  else {
    char scaleF[10];
    sprintf(scaleF,"%dxH%d",signalFactor,mH);
    species[1]=TString(scaleF);
  }
  species[2]="Wjets";
  species[3]="diboson";
  species[4]="top";
  species[5]="Zjets";
  species[6]="Ztaus";
  species[7]="WW";
  
  TString scalefactor_datadriven[NSPECIES][JETBINS];
  scalefactor_datadriven[0][0] = "1.";
  scalefactor_datadriven[1][0] = "1.";
  scalefactor_datadriven[2][0] = "1.";     // here the data are used directly
  //  scalefactor_datadriven[2][0] = "1."; // if W+jets taken from MC
  scalefactor_datadriven[3][0] = "1.";      // taken from MC, + scale factor
  scalefactor_datadriven[4][0] = "1.08";
  scalefactor_datadriven[5][0] = "8.0"; // 
  scalefactor_datadriven[6][0] = "1."; // 4.0: Ztt
  scalefactor_datadriven[7][0] = "1.07";


  scalefactor_datadriven[0][1] = "1.";
  scalefactor_datadriven[1][1] = "1.";
  scalefactor_datadriven[2][1] = "1.";    // here the data are used directly 
  // scalefactor_datadriven[2][1] = "1."; // if W+jets taken from MC
  scalefactor_datadriven[3][1] = "1.";    // taken from MC + scalefactor
  scalefactor_datadriven[4][1] = "1.03";
  scalefactor_datadriven[5][1] = "1."; // 
  scalefactor_datadriven[6][1] = "1."; // 2.0: Ztt
  scalefactor_datadriven[7][1] = "0.96";

  Color_t colors[NSPECIES];
  colors[0]=kBlack;
  colors[1]=kBlack;       
  colors[2]=kAzure-1;
  colors[3]=kOrange;
  colors[4]=kViolet;
  colors[5]=kGreen+3;
  colors[6]=kGreen+3;
  colors[7]=kOrange+10;

  Color_t lineColors[NSPECIES];
  lineColors[0]=kBlack;
  lineColors[1]=kBlack;      
  lineColors[2]=kAzure+3;
  lineColors[3]=kOrange+3;
  lineColors[4]=kViolet+3;
  lineColors[5]=kGreen+4;
  lineColors[6]=kGreen+4;
  lineColors[7]=kOrange+9;

  int legendOrder[NSPECIES];
  legendOrder[0]=0;
  legendOrder[1]=1;
  legendOrder[2]=2;
  legendOrder[3]=3;
  legendOrder[4]=4;
  legendOrder[5]=5;
  legendOrder[6]=7;

  TString files[NSPECIES];
  char mass[10];
  sprintf(mass,"%d",mH);
  files[0]="results_data/datasets_trees/dataset_ll.root";  
  files[1]="results/datasets_trees/H"+TString(mass)+"_ll.root";  
  files[2]="results_data/datasets_trees/dataset_looseloose.root";
  //files[2]="results/datasets_trees/Wjets_ll.root";
  files[3]="results/datasets_trees/others_ll.root";
  files[4]="results/datasets_trees/top_ll.root";
  files[5]="results/datasets_trees/Zjets_ll.root";
  files[6]="results/datasets_trees/Zjets_ll.root";
  files[7]="results/datasets_trees/WW_ll.root";

  TString plotsDir="./HWW/"+TString(finalstate)+"/";

  TFile* fOut=new TFile("HWW_histos_"+suffix+".root","RECREATE");
  
  char icut[NCUTS][100];
  TH1F* histos[NSPECIES][NCUTS][NVARIABLES];    // 5 species, 2 cut levels, 8 variables
  
  TString variables[NVARIABLES];
  variables[0]="pfmet";
  variables[1]="mpmet";
  variables[2]="mth";
  variables[3]="mll";
  variables[4]="pt1";
  variables[5]="pt2";
  variables[6]="dphill";
  variables[7]="njet";
  variables[8]="nvtx";
  variables[9]="gammaMRStar";
  variables[10]="dymva1";

  TString units[NVARIABLES];
  units[0]="GeV";
  units[1]="GeV";
  units[2]="GeV/c^{2}";
  units[3]="GeV/c^{2}";
  units[4]="GeV/c";
  units[5]="GeV/c";
  units[6]="rad.";
  units[7]="";
  units[8]="";
  units[9]="GeV/c^{2}";
  units[10]="";

  int nbins[NVARIABLES];
  nbins[0]=25;
  nbins[1]=25;
  nbins[2]=20;  
  nbins[3]=20;  
  nbins[4]=20;  
  nbins[5]=20;  
  nbins[6]=18;  
  nbins[7]=7;
  nbins[8]=30;
  nbins[9]=25;
  nbins[10]=25;

  float range[NVARIABLES][2]; // 8 variables, min, max
  // met
  range[0][0]=0.;
  range[0][1]=150.;
  // projected met
  range[1][0]=0.;
  range[1][1]=150.;
  // MT
  range[2][0]=0.;    
  range[2][1]=200.;   
  // mll
  range[3][0]=0.;
  range[3][1]=200.;
  // max pt
  range[4][0]=20.;
  range[4][1]=300.;   
  // min pt
  range[5][0]=10.;
  range[5][1]=200.;   
  // delta phi
  range[6][0]=0.;
  range[6][1]=TMath::Pi();
  // njets
  range[7][0]=0.;
  range[7][1]=7.;
  // nvtx
  range[8][0]=1.;
  range[8][1]=60.;
  // mR
  range[9][0]=90.;
  range[9][1]=300.;
  // DY MVA
  range[10][0]=-1.0;
  range[10][1]=1.0;
  
  //                         0,1 2 3 4 5 6 7 8 9 10
  int doplot[NVARIABLES] = { 1,1,1,1,1,1,1,1,1,1,1 };

  TString xaxisLabel[NVARIABLES];
  xaxisLabel[0]="PF E_{T}^{miss}";
  xaxisLabel[1]="min(pr. PF E_{T}^{miss}, tk E_{T}^{miss})";
  xaxisLabel[2]="m_{T}^{ll E_{T}^{miss}}";
  xaxisLabel[3]="m_{ll}";
  xaxisLabel[4]="p_{T}^{l,max}";
  xaxisLabel[5]="p_{T}^{l,min}";
  xaxisLabel[6]="#Delta #phi_{ll}";
  xaxisLabel[7]="n jets";
  xaxisLabel[8]="n vtx";
  xaxisLabel[9]="M_{R}";
  xaxisLabel[10]="DY MVA";

  TString binSize[NVARIABLES];

  for (int z=0;z<NVARIABLES;++z) {
    for (int j=0;j<NCUTS;++j) {
      sprintf(icut[j],"icut%d",j);
      for (int i=0;i<NSPECIES;++i) {
	histos[i][j][z]=new TH1F(variables[z]+"_W_"+species[i]+"_"+TString(icut[j]),variables[z]+"_W_"+species[i]+"_"+TString(icut[j]),nbins[z],range[z][0],range[z][1]);
	if(i==0)
	  histos[i][j][z]->Sumw2();
	char binsiz[10];
        sprintf(binsiz,"%2.0f",(range[z][1]-range[z][0])/nbins[z]);
	binSize[z]=TString(binsiz);
      }
    }
  }

  TString HCut0j = higgsCuts(mH,true,0);
  TString HCut1j = higgsCuts(mH,true,1);
  TString HCutIn0j = higgsCuts(mH,false,0);
  TString HCutIn1j = higgsCuts(mH,false,1);

  TString cut[NCUTS];
  cut[0]="(mll>12)*";
  cut[1]="(step[14] && (dymva1>0.6 || !sameflav) && njet==0)*"; // std WW 0j
  cut[2]="(step[14] && (dymva1>0.3 || !sameflav) && njet==1)*"; // std WW 1j
  cut[3]="(step[14] && (dymva1>0.6 || !sameflav) && njet==0 &&"+HCut0j+")*";   // final 0j
  cut[4]="(step[14] && (dymva1>0.3 || !sameflav) && njet==1 &&"+HCut1j+")*";   // final 1j
  /*
  cut[3]="(WWSel)*";
  cut[4]="(WWSel1j)*";
  */

  TString channelcut("1*");
  if(TString(finalstate).Contains("sf")) channelcut=TString("(channel<2)*");
  if(TString(finalstate).Contains("of")) channelcut=TString("(channel>1)*");
  if(TString(finalstate).Contains("mm")) channelcut=TString("(channel==0)*");
  if(TString(finalstate).Contains("ee")) channelcut=TString("(channel==1)*");
  if(TString(finalstate).Contains("em")) channelcut=TString("(channel==2)*");
  if(TString(finalstate).Contains("me")) channelcut=TString("(channel==3)*");
  for(int i=0;i<NCUTS;i++) cut[i]+=channelcut;

  char lumistr[5];
  sprintf(lumistr,"%.1f",lumi);
  TString intLumi=TString(lumistr);     
  TFile *_file[NSPECIES];
  TTree *T1[NSPECIES];

  char lumiwgt[10];
  //sprintf(lumiwgt,"&f*",lumi);
  sprintf(lumiwgt,"%f*",lumi);

  if(!blindData) {
    _file[0]=TFile::Open(files[0]);
    T1[0] = (TTree*)_file[0]->Get("latino");
  } else T1[0] = 0;
  
  for (int i=1;i<NSPECIES;++i) {
    _file[i]=TFile::Open(files[i]);
    T1[i] = (TTree*)_file[i]->Get("latino");
    if(i==2) T1[i]->AddFriend("flatino=latino","results_data/datasets_trees/dataset_looseloose_wwbits.root");
   }

  int nspeciesToRun=NSPECIES;

  for (int z=0;z<NVARIABLES;++z)
    {
      if(doplot[z]==0) continue;
      for (int j=0;j<NCUTS;++j)
      {
          int firstSpecie = 0;
          if(blindData) firstSpecie = 1;
	  for (int i=firstSpecie;i<nspeciesToRun;++i)
	    {
	      fOut->cd();
	      TString histoName=variables[z]+"_W_"+species[i]+"_"+TString(icut[j]);
	      std::cout << "Producing " << histoName << std::endl;
	      if (T1[i]==0)
		{
		  std::cout << "Species " << i << " Tree not found" << std::endl;
		  return;
		}

              // ATTENTION. IF CHANGING THE CUTS CHANGE HERE THE DEFINITION OF JET BIN. IMPORTANT FOR SCALE FACTORS!
              int jetbin = -1;
              if(j==2 || j==4) 
		jetbin = 1;
              else 
		jetbin = 0;

              if (i==6) continue; // Ztt is included in madgraph sample
	      
              if(i>0) {
                if(j>0 && j<5) { // scalefactors are valid from WW level on
                  if(i!=2 && i!=5 && i!=6) T1[i]->Project(histoName,variables[z],cut[j]+TString(lumiwgt)+TString("baseW*puW*effW*")+scalefactor_datadriven[i][jetbin]);
                  if(i==2) T1[i]->Project(histoName,variables[z],cut[j]+TString("fake2W")); // W+jets uses the FR data weight only
                  // if(i==2) T1[i]->Project(histoName,variables[z],cut[j]+TString("baseW*puW*effW*")+scalefactor_datadriven[i][jetbin]); // when usign Wjets MC
		  if(i==5) T1[i]->Project(histoName,variables[z],cut[j]+TString(lumiwgt)+TString("baseW*puW*effW*")+scalefactor_datadriven[i][jetbin]);
		  //if(i==6) T1[i]->Project(histoName,variables[z],cut[j]+TString(lumiwgt)+TString("baseW*(dataset==32 || dataset==35)*")+scalefactor_datadriven[i][jetbin]);
                } else {
                  if(i!=2) T1[i]->Project(histoName,variables[z],cut[j]+TString(lumiwgt)+TString("baseW*puW*effW"));
                  // fake2W is not good at 2l level. Do Not plot (W+jets is invisible) 
                  // T1[i]->Project(histoName,variables[z],cut[j]+TString("fake2W")); // W+jets uses the FR data weight only
                  // else T1[i]->Project(histoName,variables[z],cut[j]+TString("baseW*puW*effW")); // when usign Wjets MC 
                }
              } else T1[i]->Project(histoName,variables[z],cut[j]+TString("1"));
	      std::cout << "Done " << histoName << std::endl;
	    }
          
          LatinoPlot myPlot;
          myPlot.setLumi(lumi);
          myPlot.addLabel("");
          myPlot.setLabel((xaxisLabel[z]).Data());
          myPlot.setUnits((units[z]).Data());
          myPlot.setMass(mH);
          myPlot.setMCHist(iHWW,     histos[1][j][z]);
          myPlot.setMCHist(iWJets,   histos[2][j][z]);
          myPlot.setMCHist(iWZ,      histos[3][j][z]);
          myPlot.setMCHist(iTop,     histos[4][j][z]);
          myPlot.setMCHist(iZJets,   histos[5][j][z]);
          myPlot.setMCHist(iZTau,    histos[6][j][z]);          
	  myPlot.setMCHist(iWW,      histos[7][j][z]);

          if(!blindData) myPlot.setDataHist(histos[0][j][z]);
          
	  // Draw
	  //--------------------------------------------------------------------
	  TCanvas* c1 = new TCanvas(Form("test_%d_%d_lin", z, j),
				    Form("test_%d_%d_lin", z, j));

 	  c1->SetLogy(0);

          //myPlot.setNoStack();
          myPlot.Draw();
          //          else if(j == 1) myPlot.Draw(2);
          //          else  myPlot.Draw(4);

          c1->GetFrame()->DrawClone();

	  c1->SaveAs(plotsDir+variables[z]+TString(icut[j])+suffix+".lin.png");
          c1->SaveAs(plotsDir+variables[z]+TString(icut[j])+suffix+".root");
	  c1->SaveAs(plotsDir+variables[z]+TString(icut[j])+suffix+".lin.eps");

	  TCanvas* c2 = new TCanvas(Form("test_%d_%d_log", z, j),
				    Form("test_%d_%d_log", z, j));

	  c2->SetLogy(1);

          myPlot.Draw();

          c2->GetFrame()->DrawClone();
          
	  c2->SaveAs(plotsDir+variables[z]+TString(icut[j])+suffix+".log.png");


	}
    }
  
  fOut->Write();
  fOut->Close();
  
}
