#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

std::vector<float> nEventsFinalLeptons;

void setExpectedEvents() {
  
  nEventsFinalLeptons.push_back(10); 
  nEventsFinalLeptons.push_back(32);
}


//! lumi in pb-1
void drawKinematics() {

  setExpectedEvents();
  
  // get the expected events for each process considered
  std::vector<float> expEvents = nEventsFinalLeptons;
  
  TFile *Higgs=0, *ttjets=0;
  Higgs  = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/H160_WW_2l/H160_WW_2l_0-datasetEE.root");
  ttjets = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/ttjetsMadgraph_Fall08/ttjetsMadgraph_Fall08-datasetEE.root");
  TTree *treeHiggs = (TTree*)Higgs->Get("T1");      
  TTree *treeTTbar = (TTree*)ttjets->Get("T1");      
  

  TH1F* dxyEvtH = new TH1F("dxyEvtH", "dxyEvtH", 100,0,10);
  TH2F *H_2dim  = new TH2F("H_2dim","",150,0.,150.,150,0.,150.);
  H_2dim->SetMinimum(0.0);
  H_2dim->SetMaximum(1.0);

  char buf[50];
  char Selection[1000];  
  sprintf(buf,"dxyEvtH");
  sprintf(Selection,"njets>0");  

  // total number of events with >0 jets   
  float noCutSignal = treeHiggs->Project(buf,"dxyEVT",Selection);
  float noCutTTbar  = treeTTbar->Project(buf,"dxyEVT",Selection);
  
  // events step x step
  for (int theCut1=0; theCut1<150; theCut1++){ 
    for (int theCut2=0; theCut2<150; theCut2++){ 
      
      if (theCut1%10==0 && theCut2%10==0) cout << theCut1 << " " << theCut2 << endl;
      float theCut1F = (float)theCut1;
      float theCut2F = (float)theCut2;
      
      // signal
      sprintf(Selection,"abs(dxyEVT)<%f && abs(dszEVT)<%f && njets>0",0.001*(theCut1F+1), 0.001*(theCut2F+1));
      float passedH = treeHiggs->Project(buf,"dxyEVT",Selection);
      float signal  = (passedH/noCutSignal)*expEvents[0];
      
      // ttbar
      sprintf(Selection,"abs(dxyEVT)<%f && abs(dszEVT)<%f && njets>0",0.001*(theCut1F+1), 0.001*(theCut2F+1));
      float passedTTbar = treeTTbar->Project(buf,"dxyEVT",Selection);
      float ttbar = (passedTTbar/noCutTTbar)*expEvents[1];
      
      // fill histo with S/B
      H_2dim->SetBinContent(theCut1+1,theCut2+1,signal/ttbar);
    }
  }
  
  TCanvas c2dim("c2dim","c2dim",600,600);
  H_2dim->GetXaxis()->SetTitle("dxy^{EVT} [#mum]");
  H_2dim->GetYaxis()->SetTitle("dz^{EVT} [#mum]");
  H_2dim->Draw("colz");
  c2dim.SaveAs("c2dim.root");
 
  TFile *outfile = new TFile("H2dim.root","RECREATE");
  outfile->cd();
  H_2dim -> Write();
  outfile->Close();
}

