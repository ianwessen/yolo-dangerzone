// macro to draw normalized distributions
// ----
// efficiencies are hardcoded for 2 selections:
//   1) full selection
//   2) after the CJV (trigger+reco+iso+ID+CJV)
// usage: 
// root -b
// .L macro/higgsPlots.cxx++
// drawKinematics("jetVeto"): draws distributions after CJV
// drawKinematics("finalSelection"): draw distributions after the full selection (but stat is poor)
// ---

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

std::vector<float> nEventsPresel;
std::vector<float> nEventsFinalLeptons;
std::vector<float> nEventsCJV;
std::vector<float> nEventsPreDeltaPhi;
std::vector<float> nEventsFinal;

void setExpectedEvents() {
  
  // events expected in 1000 pb-1, mH = 160 GeV
  nEventsPresel.push_back(371);   // H->WW
  nEventsPresel.push_back(8414);  // W+j
  nEventsPresel.push_back(5323);  // ttbar
  nEventsPresel.push_back(30032); // Z+j 
  nEventsPresel.push_back(1343);  // WW
  nEventsPresel.push_back(18);    // ZZ, 4l
  nEventsPresel.push_back(320);   // WZ 3leptons
  nEventsPresel.push_back(599);   // WZ incl

  nEventsFinalLeptons.push_back(52); 
  nEventsFinalLeptons.push_back(33);
  nEventsFinalLeptons.push_back(548);
  nEventsFinalLeptons.push_back(7116);
  nEventsFinalLeptons.push_back(181);
  nEventsFinalLeptons.push_back(5);
  nEventsFinalLeptons.push_back(86);
  nEventsFinalLeptons.push_back(152); 

  nEventsCJV.push_back(52);  
  nEventsCJV.push_back(33); 
  nEventsCJV.push_back(548);  
  nEventsCJV.push_back(7116);  
  nEventsCJV.push_back(181);  
  nEventsCJV.push_back(5); 
  nEventsCJV.push_back(86);  
  nEventsCJV.push_back(152); 

  nEventsPreDeltaPhi.push_back(26); 
  nEventsPreDeltaPhi.push_back(2); 
  nEventsPreDeltaPhi.push_back(45); 
  nEventsPreDeltaPhi.push_back(19); 
  nEventsPreDeltaPhi.push_back(20);
  nEventsPreDeltaPhi.push_back(0);
  nEventsPreDeltaPhi.push_back(1);
  nEventsPreDeltaPhi.push_back(1);

  nEventsFinal.push_back(23); 
  nEventsFinal.push_back(0);
  nEventsFinal.push_back(33);
  nEventsFinal.push_back(15); 
  nEventsFinal.push_back(15); 
  nEventsFinal.push_back(0);
  nEventsFinal.push_back(1);
  nEventsFinal.push_back(1);
}

//! lumi in pb-1
void drawKinematics(const char* selection, float lumi=1000) {

  setExpectedEvents();
  
  // get the expected events for each process considered
  char Selection[500];
  std::vector<float> expEvents;
  if (strcmp(selection,"Preselection")==0) {
    sprintf(Selection, "1==1");
    expEvents = nEventsPresel;
  }
  else if (strcmp(selection,"finalLeptons")==0) {
    sprintf(Selection, "finalLeptons");
    expEvents = nEventsFinalLeptons;
  }
  else if (strcmp(selection,"jetVeto")==0) {
    sprintf(Selection,"jetVeto");
    expEvents = nEventsCJV;
  }
  else if (strcmp(selection,"preDeltaPhi")==0) {
    sprintf(Selection,"preDeltaPhi");
    expEvents = nEventsPreDeltaPhi;
  }
  else {
    sprintf(Selection,"finalSelection");
    expEvents = nEventsFinal;
  }

  
  std::vector<TFile*> datasets;
  TFile *Higgs=0, *Wjets=0, *ttjets=0, *Zjets=0, *WW=0, *ZZ4l=0, *WZ3l=0, *WZincl=0;
  Higgs   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/H160_WW_2l/H160_WW_2l_0-datasetEE.root");
  Wjets   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/WjetsMADGRAPH_Fall08/WjetsMADGRAPH_Fall08-datasetEE.root");
  ttjets  = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/ttjetsMadgraph_Fall08/ttjetsMadgraph_Fall08-datasetEE.root");
  Zjets   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/ZjetsMadgraph_Fall08/ZjetsMadgraph_Fall08-datasetEE.root");
  WW      = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/WW_2l/WW_2l_0-datasetEE.root");  
  ZZ4l    = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/ZZ_4l/ZZ_4l_0-datasetEE.root");
  WZ3l    = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/WZ_3l/WZ_3l_0-datasetEE.root");
  WZincl  = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId/WZ_incl/WZ_incl_0-datasetEE.root");
  datasets.push_back(Higgs);  // 0
  datasets.push_back(Wjets);  // 1
  datasets.push_back(ttjets); // 2 
  datasets.push_back(Zjets);  // 3 
  datasets.push_back(WW);     // 4 
  datasets.push_back(ZZ4l);   // 5 
  datasets.push_back(WZ3l);   // 6
  datasets.push_back(WZincl); // 7
  

  std::vector<TH1F*> met;
  std::vector<TH1F*> mll;
  std::vector<TH1F*> deltaPhi;
  std::vector<TH1F*> ptmax;
  std::vector<TH1F*> ptmin;
  std::vector<TH1F*> dxyEvt;
  std::vector<TH1F*> dszEvt;

  TH1F* metH      = new TH1F("metH",      "metH",     25,0,200);
  TH1F* mllH      = new TH1F("mllH",      "mllH",     25,0,200);
  TH1F* deltaPhiH = new TH1F("deltaPhiH", "deltaPhiH",25,0,180);
  TH1F* ptmaxH    = new TH1F("ptmaxH",    "ptmaxH",   25,0,200);
  TH1F* ptminH    = new TH1F("ptminH",    "ptminH",   25,0,200);
  TH1F* dxyEvtH   = new TH1F("dxyEvtH",   "dxyEvtH",  25,0,1);
  TH1F* dszEvtH   = new TH1F("dszEvtH",   "dszEvtH",  25,0,1);

  std::cout << "# of samples = " <<  expEvents.size() << std::endl;

  for(int i=0; i<(int)expEvents.size(); i++) {

    std::cout << "Expected events for sample " << i 
	      << " at step " << selection 
	      << " in 100 pb-1 = " << expEvents[i] << std::endl;
    
    TTree *tree = (TTree*)datasets[i]->Get("T1");      
    std::cout << "dataset has " << tree->GetEntries() << " entries" << std::endl;
    
    // met
    char buf[50];
    sprintf(buf,"met_%d",i);
    TH1F* metProcessX = (TH1F*) metH->Clone(buf);
    metProcessX->Sumw2();
    met.push_back(metProcessX);
    tree->Project(buf,"met",Selection);
    met[i]->Scale( expEvents[i]/met[i]->Integral() * lumi/1000. );
    
    // mll
    sprintf(buf,"mll_%d",i);
    TH1F* mllProcessX = (TH1F*) mllH->Clone(buf);
    mllProcessX->Sumw2();
    mll.push_back(mllProcessX);
    tree->Project(buf,"eleInvMass",Selection);
    mll[i]->Scale( expEvents[i]/mll[i]->Integral() * lumi/1000. );

    // deltaPhi
    sprintf(buf,"deltaPhi_%d",i);
    TH1F* deltaPhiProcessX = (TH1F*) deltaPhiH->Clone(buf);
    deltaPhiProcessX->Sumw2();
    deltaPhi.push_back(deltaPhiProcessX);
    tree->Project(buf,"deltaPhi",Selection);
    deltaPhi[i]->Scale( expEvents[i]/deltaPhi[i]->Integral() * lumi/1000. );

    // pt max
    sprintf(buf,"ptmax_%d",i);
    TH1F* ptmaxProcessX = (TH1F*) ptmaxH->Clone(buf);
    ptmaxProcessX->Sumw2();
    ptmax.push_back(ptmaxProcessX);
    tree->Project(buf,"maxPtEle",Selection);
    ptmax[i]->Scale( expEvents[i]/ptmax[i]->Integral() * lumi/1000. );

    // pt min
    sprintf(buf,"ptmin_%d",i);
    TH1F* ptminProcessX = (TH1F*) ptminH->Clone(buf);
    ptminProcessX->Sumw2();
    ptmin.push_back(ptminProcessX);
    tree->Project(buf,"minPtEle",Selection);
    ptmin[i]->Scale( expEvents[i]/ptmin[i]->Integral() * lumi/1000. );

    // dxy evt
    sprintf(buf,"dxyEVT_%d",i);
    TH1F* dxyProcessX = (TH1F*) dxyEvtH->Clone(buf);
    dxyProcessX->Sumw2();
    dxyEvt.push_back(dxyProcessX);
    tree->Project(buf,"dxyEVT",Selection);
    dxyEvt[i]->Scale( expEvents[i]/dxyEvt[i]->Integral() * lumi/1000. );    

    // dsz evt
    sprintf(buf,"dszEVT_%d",i);
    TH1F* dszProcessX = (TH1F*) dszEvtH->Clone(buf);
    dszProcessX->Sumw2();
    dszEvt.push_back(dszProcessX);
    tree->Project(buf,"dszEVT",Selection);
    dszEvt[i]->Scale( expEvents[i]/dszEvt[i]->Integral() * lumi/1000. );    
  }
  
  

  TLegend *leg = new TLegend(0.11,0.65,0.45,0.89);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(met[0],"Signal, m_{H}=160 GeV","pl");
  leg->AddEntry(met[1],"W+jets","f");
  leg->AddEntry(met[2],"tt+jets","f");
  leg->AddEntry(met[3],"Z+jets","f");
  leg->AddEntry(met[4],"WW","f");
  leg->AddEntry(met[5],"ZZ$->$4l","f");
  leg->AddEntry(met[6],"WZ$->$3l","f");
  leg->AddEntry(met[7],"WZ inclusive","f");

  TH1F *histo0, *histo1, *histo2, *histo3, *histo4;

  gStyle->SetOptStat(0);


  // --------------
  // draw met
  TCanvas cmet("cmet","cmet",600,600);
  cmet.SetLogy();
  met[3]->SetMaximum(50000);
  met[3]->SetMinimum(0.01);
  met[3]->SetFillColor(6);
  met[3]->SetTitle("");
  met[3]->GetXaxis()->SetTitle("Missing E_{T} [GeV]");
  met[3]->GetYaxis()->SetTitle("normalized Events");
  met[3]->Draw("hist");

  met[1]->SetFillColor(2);        met[1]->Draw("same hist");  
  met[2]->SetFillColor(4);        met[2]->Draw("same hist");
  met[3]->SetFillColor(kPink+4);  met[3]->Draw("same hist");  
  met[4]->SetFillColor(5);        met[4]->Draw("same hist");
  met[5]->SetFillColor(3);        met[5]->Draw("same hist");
  met[6]->SetFillColor(7);        met[6]->Draw("same hist"); 
  met[7]->SetFillColor(28);       met[7]->Draw("same hist");   
  met[0]->SetMarkerStyle(8);      met[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fmet = new TFile("met.root","recreate");
  cmet.Write();
  histo0 = met[0];
  histo1 = met[3];
  histo2 = met[2];
  histo3 = met[4];
  met[1]->Add(met[5]);
  met[1]->Add(met[6]);
  met[1]->Add(met[7]);
  histo4 = met[1];
  
  TCanvas cmet2("cmet2","cmet2",600,600);
  cmet2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  TLegend *leg2 = new TLegend(0.11,0.65,0.45,0.89);
  leg2->SetBorderSize(0);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(histo0,"Signal, m_{H}=160 GeV","pl");
  leg2->AddEntry(histo1,"Z-jets","f");
  leg2->AddEntry(histo2,"t #bar{t}-jets","f");
  leg2->AddEntry(histo3,"WW","f");
  leg2->AddEntry(histo4,"W-jets + ZZ + WZ","f");

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cmet2.Write();
  fmet->Close();



  // --------------  
  // draw mll
  TCanvas cmll("cmll","cmll",600,600);
  cmll.SetLogy();
  mll[6]->SetMaximum(50000);
  mll[6]->SetMinimum(0.01);
  mll[6]->SetFillColor(6);
  mll[6]->SetTitle("");
  mll[6]->GetXaxis()->SetTitle("m_{ll} [GeV]");
  mll[6]->GetYaxis()->SetTitle("normalized Events");
  mll[6]->Draw("hist");

  mll[1]->SetFillColor(2);        mll[1]->Draw("same hist");  
  mll[2]->SetFillColor(4);        mll[2]->Draw("same hist");
  mll[3]->SetFillColor(kPink+4);  mll[3]->Draw("same hist");  
  mll[4]->SetFillColor(5);        mll[4]->Draw("same hist");
  mll[5]->SetFillColor(3);        mll[5]->Draw("same hist");
  mll[6]->SetFillColor(7);        mll[6]->Draw("same hist"); 
  mll[7]->SetFillColor(28);       mll[7]->Draw("same hist");   
  mll[0]->SetMarkerStyle(8);      mll[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fmll = new TFile("mll.root","recreate");
  cmll.Write();
  histo0 = mll[0];
  histo1 = mll[3];
  histo2 = mll[2];
  histo3 = mll[4];
  mll[1]->Add(mll[5]);
  mll[1]->Add(mll[6]);
  mll[1]->Add(mll[7]);
  histo4 = mll[1];
  
  TCanvas cmll2("cmll2","cmll2",600,600);
  cmll2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cmll2.Write();
  fmll->Close();



  // --------------
  // draw deltaPhi
  TCanvas cdeltaPhi("cdeltaPhi","cdeltaPhi",600,600);
  cdeltaPhi.SetLogy();
  deltaPhi[6]->SetMaximum(100000);
  deltaPhi[6]->SetMinimum(0.01);
  deltaPhi[6]->SetFillColor(6);
  deltaPhi[6]->SetTitle("");
  deltaPhi[6]->GetXaxis()->SetTitle("#Delta #Phi");
  deltaPhi[6]->GetYaxis()->SetTitle("normalized Events");
  deltaPhi[6]->Draw("hist");

  deltaPhi[1]->SetFillColor(2);        deltaPhi[1]->Draw("same hist");  
  deltaPhi[2]->SetFillColor(4);        deltaPhi[2]->Draw("same hist");
  deltaPhi[3]->SetFillColor(kPink+4);  deltaPhi[3]->Draw("same hist");  
  deltaPhi[4]->SetFillColor(5);        deltaPhi[4]->Draw("same hist");
  deltaPhi[5]->SetFillColor(3);        deltaPhi[5]->Draw("same hist");
  deltaPhi[6]->SetFillColor(7);        deltaPhi[6]->Draw("same hist"); 
  deltaPhi[7]->SetFillColor(28);       deltaPhi[7]->Draw("same hist");   
  deltaPhi[0]->SetMarkerStyle(8);      deltaPhi[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fdeltaPhi = new TFile("deltaPhi.root","recreate");
  cdeltaPhi.Write();
  histo0 = deltaPhi[0];
  histo1 = deltaPhi[3];
  histo2 = deltaPhi[2];
  histo3 = deltaPhi[4];
  deltaPhi[1]->Add(deltaPhi[5]);
  deltaPhi[1]->Add(deltaPhi[6]);
  deltaPhi[1]->Add(deltaPhi[7]);
  histo4 = deltaPhi[1];
  
  TCanvas cdeltaPhi2("cdeltaPhi2","cdeltaPhi2",600,600);
  cdeltaPhi2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cdeltaPhi2.Write();
  fdeltaPhi->Close();




  // --------------  
  // draw ptmax
  TCanvas cptmax("cptmax","cptmax",600,600);
  cptmax.SetLogy();
  ptmax[6]->SetMaximum(100000);
  ptmax[6]->SetMinimum(0.01);
  ptmax[6]->SetFillColor(6);
  ptmax[6]->SetTitle("");
  ptmax[6]->GetXaxis()->SetTitle("P^{e max}_{T} [GeV]");
  ptmax[6]->GetYaxis()->SetTitle("normalized Events");
  ptmax[6]->Draw("hist");

  ptmax[1]->SetFillColor(2);        ptmax[1]->Draw("same hist");  
  ptmax[2]->SetFillColor(4);        ptmax[2]->Draw("same hist");
  ptmax[3]->SetFillColor(kPink+4);  ptmax[3]->Draw("same hist");  
  ptmax[4]->SetFillColor(5);        ptmax[4]->Draw("same hist");
  ptmax[5]->SetFillColor(3);        ptmax[5]->Draw("same hist");
  ptmax[6]->SetFillColor(7);        ptmax[6]->Draw("same hist"); 
  ptmax[7]->SetFillColor(28);       ptmax[7]->Draw("same hist");   
  ptmax[0]->SetMarkerStyle(8);      ptmax[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fptmax = new TFile("ptmax.root","recreate");
  cptmax.Write();
  histo0 = ptmax[0];
  histo1 = ptmax[3];
  histo2 = ptmax[2];
  histo3 = ptmax[4];
  ptmax[1]->Add(ptmax[5]);
  ptmax[1]->Add(ptmax[6]);
  ptmax[1]->Add(ptmax[7]);
  histo4 = ptmax[1];
  
  TCanvas cptmax2("cptmax2","cptmax2",600,600);
  cptmax2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cptmax2.Write();
  fptmax->Close();



  // --------------
  // draw ptmin
  TCanvas cptmin("cptmin","cptmin",600,600);
  cptmin.SetLogy();
  ptmin[6]->SetMaximum(100000);
  ptmin[6]->SetMinimum(0.01);
  ptmin[6]->SetFillColor(6);
  ptmin[6]->SetTitle("");
  ptmin[6]->GetXaxis()->SetTitle("P^{e min}_{T} [GeV]");
  ptmin[6]->GetYaxis()->SetTitle("normalized Events");
  ptmin[6]->Draw("hist");

  ptmin[1]->SetFillColor(2);        ptmin[1]->Draw("same hist");  
  ptmin[2]->SetFillColor(4);        ptmin[2]->Draw("same hist");
  ptmin[3]->SetFillColor(kPink+4);  ptmin[3]->Draw("same hist");  
  ptmin[4]->SetFillColor(5);        ptmin[4]->Draw("same hist");
  ptmin[5]->SetFillColor(3);        ptmin[5]->Draw("same hist");
  ptmin[6]->SetFillColor(7);        ptmin[6]->Draw("same hist"); 
  ptmin[7]->SetFillColor(28);       ptmin[7]->Draw("same hist");   
  ptmin[0]->SetMarkerStyle(8);      ptmin[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fptmin = new TFile("ptmin.root","recreate");
  cptmin.Write();
  histo0 = ptmin[0];
  histo1 = ptmin[3];
  histo2 = ptmin[2];
  histo3 = ptmin[4];
  ptmin[1]->Add(ptmin[5]);
  ptmin[1]->Add(ptmin[6]);
  ptmin[1]->Add(ptmin[7]);
  histo4 = ptmin[1];
  
  TCanvas cptmin2("cptmin2","cptmin2",600,600);
  cptmin2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cptmin2.Write();
  fptmin->Close();



  // --------------
  // draw dxyEvt
  TCanvas cdxyEvt("cdxyEvt","cdxyEvt",600,600);
  cdxyEvt.SetLogy();
  dxyEvt[6]->SetMaximum(100000);
  dxyEvt[6]->SetMinimum(0.01);
  dxyEvt[6]->SetFillColor(6);
  dxyEvt[6]->SetTitle("");
  dxyEvt[6]->GetXaxis()->SetTitle("P^{e min}_{T} [GeV]");
  dxyEvt[6]->GetYaxis()->SetTitle("normalized Events");
  dxyEvt[6]->Draw("hist");

  dxyEvt[1]->SetFillColor(2);        dxyEvt[1]->Draw("same hist");  
  dxyEvt[2]->SetFillColor(4);        dxyEvt[2]->Draw("same hist");
  dxyEvt[3]->SetFillColor(kPink+4);  dxyEvt[3]->Draw("same hist");  
  dxyEvt[4]->SetFillColor(5);        dxyEvt[4]->Draw("same hist");
  dxyEvt[5]->SetFillColor(3);        dxyEvt[5]->Draw("same hist");
  dxyEvt[6]->SetFillColor(7);        dxyEvt[6]->Draw("same hist"); 
  dxyEvt[7]->SetFillColor(28);       dxyEvt[7]->Draw("same hist");   
  dxyEvt[0]->SetMarkerStyle(8);      dxyEvt[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fdxyEvt = new TFile("dxyEvt.root","recreate");
  cdxyEvt.Write();
  histo0 = dxyEvt[0];
  histo1 = dxyEvt[3];
  histo2 = dxyEvt[2];
  histo3 = dxyEvt[4];
  dxyEvt[1]->Add(dxyEvt[5]);
  dxyEvt[1]->Add(dxyEvt[6]);
  dxyEvt[1]->Add(dxyEvt[7]);
  histo4 = dxyEvt[1];
  
  TCanvas cdxyEvt2("cdxyEvt2","cdxyEvt2",600,600);
  cdxyEvt2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cdxyEvt2.Write();
  fdxyEvt->Close();


  // --------------
  // draw dszEvt
  TCanvas cdszEvt("cdszEvt","cdszEvt",600,600);
  cdszEvt.SetLogy();
  dszEvt[6]->SetMaximum(100000);
  dszEvt[6]->SetMinimum(0.01);
  dszEvt[6]->SetFillColor(6);
  dszEvt[6]->SetTitle("");
  dszEvt[6]->GetXaxis()->SetTitle("P^{e min}_{T} [GeV]");
  dszEvt[6]->GetYaxis()->SetTitle("normalized Events");
  dszEvt[6]->Draw("hist");

  dszEvt[1]->SetFillColor(2);        dszEvt[1]->Draw("same hist");  
  dszEvt[2]->SetFillColor(4);        dszEvt[2]->Draw("same hist");
  dszEvt[3]->SetFillColor(kPink+4);  dszEvt[3]->Draw("same hist");  
  dszEvt[4]->SetFillColor(5);        dszEvt[4]->Draw("same hist");
  dszEvt[5]->SetFillColor(3);        dszEvt[5]->Draw("same hist");
  dszEvt[6]->SetFillColor(7);        dszEvt[6]->Draw("same hist"); 
  dszEvt[7]->SetFillColor(28);       dszEvt[7]->Draw("same hist");   
  dszEvt[0]->SetMarkerStyle(8);      dszEvt[0]->Draw("same pe1");
  
  leg->Draw();
  
  TFile *fdszEvt = new TFile("dszEvt.root","recreate");
  cdszEvt.Write();
  histo0 = dszEvt[0];
  histo1 = dszEvt[3];
  histo2 = dszEvt[2];
  histo3 = dszEvt[4];
  dszEvt[1]->Add(dszEvt[5]);
  dszEvt[1]->Add(dszEvt[6]);
  dszEvt[1]->Add(dszEvt[7]);
  histo4 = dszEvt[1];
  
  TCanvas cdszEvt2("cdszEvt2","cdszEvt2",600,600);
  cdszEvt2.SetLogy();
  histo0->SetName("histo0"); // H->WW
  histo1->SetName("histo1"); // Z+jets
  histo2->SetName("histo2"); // ttbar
  histo3->SetName("histo3"); // WW
  histo4->SetName("histo4"); // others

  histo0->SetMarkerStyle(8);
  histo0->SetMarkerSize(1.5);
  histo1->SetFillColor(kRed+2);
  histo2->SetFillColor(kYellow-7);
  histo3->SetFillColor(kRed-4);
  histo4->SetFillColor(kBlue+3);
  histo4->SetFillStyle(3003);

  histo0->Write();
  histo1->Write();
  histo2->Write();
  histo3->Write();
  histo4->Write();

  histo1->Draw("hist");
  histo2->Draw("same hist");
  histo4->Draw("same hist");
  histo3->Draw("same hist");
  histo0->Draw("same pe1");
  leg2->Draw();
  cdszEvt2.Write();
  fdszEvt->Close();
}


//! lumi in pb-1
void drawCorrelations(const char* selection) {

  setExpectedEvents();

  float corrS[6], corrW[6], corrT[6], corrO[6];
  
  // get the expected events for each process considered
  char Selection[500];
  std::vector<float> expEvents;
  if (strcmp(selection,"Preselection")==0) {
    sprintf(Selection, "1==1");
    expEvents = nEventsPresel;
  }
  else if (strcmp(selection,"finalLeptons")==0) {
    sprintf(Selection, "finalLeptons");
    expEvents = nEventsFinalLeptons;
  }
  else if (strcmp(selection,"jetVeto")==0) {
    sprintf(Selection,"jetVeto");
    expEvents = nEventsCJV;
  }
  else if (strcmp(selection,"preDeltaPhi")==0) {
    sprintf(Selection,"preDeltaPhi");
    expEvents = nEventsPreDeltaPhi;
  }
  else {
    sprintf(Selection,"finalSelection");
    expEvents = nEventsFinal;
  }

  
  std::vector<TFile*> datasets;
  TFile *Higgs=0, *Wjets=0, *ttjets=0, *Zjets=0, *WW=0, *ZZ4l=0, *WZ3l=0, *WZincl=0;
  Higgs   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/H160_WW_2l/H160_WW_2l_0-datasetEE.root");
  Wjets   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/WjetsMADGRAPH_Fall08/WjetsMADGRAPH_Fall08-datasetEE.root");
  ttjets  = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/ttjetsMadgraph_Fall08/ttjetsMadgraph_Fall08-datasetEE.root");
  Zjets   = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/ZjetsMadgraph_Fall08/ZjetsMadgraph_Fall08-datasetEE.root");
  WW      = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/WW_2l/WW_2l_0-datasetEE.root");  
  ZZ4l    = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/ZZ_4l/ZZ_4l_0-datasetEE.root");
  WZ3l    = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/WZ_3l/WZ_3l_0-datasetEE.root");
  WZincl  = TFile::Open("/cmsrm/pc17/crovelli/data/Higgs2.1.X/fullAnalysis_nostraEleId_noCuts/WZ_incl/WZ_incl_0-datasetEE.root");
  datasets.push_back(Higgs);  // 0
  datasets.push_back(Wjets);  // 1
  datasets.push_back(ttjets); // 2 
  datasets.push_back(Zjets);  // 3 
  datasets.push_back(WW);     // 4 
  datasets.push_back(ZZ4l);   // 5 
  datasets.push_back(WZ3l);   // 6
  datasets.push_back(WZincl); // 7
  
  std::vector<TH2F*> metVsDeltaPhi;
  std::vector<TH2F*> metVsPtmin;
  std::vector<TH2F*> metVsPtmax;
  std::vector<TH2F*> ptminVsDeltaPhi;
  std::vector<TH2F*> ptmaxVsDeltaPhi;
  std::vector<TH2F*> ptminVsPtmax;  
  TH2F* metVsDeltaPhiH    = new TH2F("metVsDeltaPhiH",     "metVsDeltaPhiH",    50,0,180, 50,0,175);
  TH2F* metVsPtminH       = new TH2F("metVsPtminH",        "metVsPtminH",       50,0,100, 50,0,175);
  TH2F* metVsPtmaxH       = new TH2F("metVsPtmaxH",        "metVsPtmaxH",       50,0,100, 50,0,175);
  TH2F* ptminVsDeltaPhiH  = new TH2F("ptminVsDeltaPhiH",   "ptminVsDeltaPhiH",  50,0,180, 50,0,100);
  TH2F* ptmaxVsDeltaPhiH  = new TH2F("ptmaxVsDeltaPhiH",   "ptmaxVsDeltaPhiH",  50,0,180, 50,0,100);
  TH2F* ptminVsPtmaxH     = new TH2F("ptminVsPtmaxH",      "ptminVsPtmaxH",     50,0,100, 50,0,100);
  
  std::cout << "# of samples = " <<  expEvents.size() << std::endl;

  for(int i=0; i<(int)expEvents.size(); i++) {
    
    std::cout << "Expected events for sample " << i 
	      << " at step " << selection 
	      << " in 100 pb-1 = " << expEvents[i] << std::endl;
    
    TTree *tree = (TTree*)datasets[i]->Get("T1");      
    std::cout << "dataset has " << tree->GetEntries() << " entries" << std::endl;
    
    // met vs deltaPhi
    char buf[150];
    sprintf(buf,"met_%d:deltaPhi_%d",i,i);
    TH2F* metVsDeltaPhiProcessX = (TH2F*) metVsDeltaPhiH->Clone(buf);
    metVsDeltaPhiProcessX->GetXaxis()->SetTitle("#Delta #Phi");
    metVsDeltaPhiProcessX->GetYaxis()->SetTitle("Missing E_{T} [GeV]");
    metVsDeltaPhi.push_back(metVsDeltaPhiProcessX);
    tree->Project(buf,"met:deltaPhi",Selection);

    // met vs ptmin 
    sprintf(buf,"met_%d:minPtEle_%d",i,i);
    TH2F* metVsPtminProcessX = (TH2F*) metVsPtminH->Clone(buf);
    metVsPtminProcessX->GetXaxis()->SetTitle("min p_{T} [GeV]");
    metVsPtminProcessX->GetYaxis()->SetTitle("Missing E_{T} [GeV]");
    metVsPtmin.push_back(metVsPtminProcessX);
    tree->Project(buf,"met:minPtEle",Selection);

    // met vs ptmax
    sprintf(buf,"met_%d:maxPtEle_%d",i,i);
    TH2F* metVsPtmaxProcessX = (TH2F*) metVsPtmaxH->Clone(buf);
    metVsPtmaxProcessX->GetXaxis()->SetTitle("max p_{T} [GeV]");
    metVsPtmaxProcessX->GetYaxis()->SetTitle("Missing E_{T} [GeV]");
    metVsPtmax.push_back(metVsPtmaxProcessX);
    tree->Project(buf,"met:maxPtEle",Selection);

    // ptmin vs deltaPhi
    sprintf(buf,"minPtEle_%d:deltaPhi_%d",i,i);
    TH2F* ptminVsDeltaPhiProcessX = (TH2F*) ptminVsDeltaPhiH->Clone(buf);
    ptminVsDeltaPhiProcessX->GetYaxis()->SetTitle("min p_{T} [GeV]");
    ptminVsDeltaPhiProcessX->GetXaxis()->SetTitle("#Delta #Phi");
    ptminVsDeltaPhi.push_back(ptminVsDeltaPhiProcessX);
    tree->Project(buf,"minPtEle:deltaPhi",Selection);

    // ptmax vs deltaPhi
    sprintf(buf,"maxPtEle_%d:deltaPhi_%d",i,i);
    TH2F* ptmaxVsDeltaPhiProcessX = (TH2F*) ptmaxVsDeltaPhiH->Clone(buf);
    ptmaxVsDeltaPhiProcessX->GetYaxis()->SetTitle("max p_{T} [GeV]");
    ptmaxVsDeltaPhiProcessX->GetXaxis()->SetTitle("#Delta #Phi");
    ptmaxVsDeltaPhi.push_back(ptmaxVsDeltaPhiProcessX);
    tree->Project(buf,"maxPtEle:deltaPhi",Selection);

    // ptmin vs ptmax
    sprintf(buf,"minPtEle_%d:maxPtEle_%d",i,i);
    TH2F* ptminVsPtmaxProcessX = (TH2F*) ptminVsPtmaxH->Clone(buf);
    ptminVsPtmaxProcessX->GetYaxis()->SetTitle("min p_{T} [GeV]");
    ptminVsPtmaxProcessX->GetXaxis()->SetTitle("max p_{T} [GeV]");
    ptminVsPtmax.push_back(ptminVsPtmaxProcessX);
    tree->Project(buf,"minPtEle:maxPtEle",Selection);
  }
  
  gStyle->SetOptStat(0);

  TFile *FmetVsDeltaPhi = new TFile("metVsDeltaPhi.root","recreate");
  TCanvas cmetVsdeltaPhi("cmetVsdeltaPhi","cmet",600,600);
  metVsDeltaPhi[0]->Draw("colz"); metVsDeltaPhi[0]->SetTitle("signal"); metVsDeltaPhi[0]->Write("signal");
  metVsDeltaPhi[4]->Draw("colz"); metVsDeltaPhi[4]->SetTitle("ww");     metVsDeltaPhi[4]->Write("ww");
  metVsDeltaPhi[2]->Draw("colz"); metVsDeltaPhi[2]->SetTitle("tt");     metVsDeltaPhi[2]->Write("tt");
  metVsDeltaPhi[1]->Add(metVsDeltaPhi[3]);
  metVsDeltaPhi[1]->Add(metVsDeltaPhi[5]);
  metVsDeltaPhi[1]->Add(metVsDeltaPhi[6]);
  metVsDeltaPhi[1]->Add(metVsDeltaPhi[7]);
  metVsDeltaPhi[1]->Draw("colz"); metVsDeltaPhi[1]->SetTitle("others"); metVsDeltaPhi[1]->Write("others");
  corrS[0] = metVsDeltaPhi[0]->GetCorrelationFactor();
  corrW[0] = metVsDeltaPhi[4]->GetCorrelationFactor();
  corrT[0] = metVsDeltaPhi[2]->GetCorrelationFactor();
  corrO[0] = metVsDeltaPhi[1]->GetCorrelationFactor();

  TFile *FmetVsPtmin = new TFile("metVsPtmin.root","recreate");
  TCanvas cmetVsptmin("cmetVsptmin","cmet",600,600);
  metVsPtmin[0]->Draw("colz"); metVsPtmin[0]->SetTitle("signal"); metVsPtmin[0]->Write("signal");
  metVsPtmin[4]->Draw("colz"); metVsPtmin[4]->SetTitle("ww");     metVsPtmin[4]->Write("ww");
  metVsPtmin[2]->Draw("colz"); metVsPtmin[2]->SetTitle("tt");     metVsPtmin[2]->Write("tt");
  metVsPtmin[1]->Add(metVsPtmin[3]);
  metVsPtmin[1]->Add(metVsPtmin[5]);
  metVsPtmin[1]->Add(metVsPtmin[6]);
  metVsPtmin[1]->Add(metVsPtmin[7]);
  metVsPtmin[1]->Draw("colz"); metVsPtmin[1]->SetTitle("others"); metVsPtmin[1]->Write("others");
  corrS[1] = metVsPtmin[0]->GetCorrelationFactor();
  corrW[1] = metVsPtmin[4]->GetCorrelationFactor();
  corrT[1] = metVsPtmin[2]->GetCorrelationFactor();
  corrO[1] = metVsPtmin[1]->GetCorrelationFactor();

  TFile *FmetVsPtmax = new TFile("metVsPtmax.root","recreate");
  TCanvas cmetVsptmax("cmetVsptmax","cmet",600,600);
  metVsPtmax[0]->Draw("colz"); metVsPtmax[0]->SetTitle("signal"); metVsPtmax[0]->Write("signal");
  metVsPtmax[4]->Draw("colz"); metVsPtmax[4]->SetTitle("ww");     metVsPtmax[4]->Write("ww");
  metVsPtmax[2]->Draw("colz"); metVsPtmax[2]->SetTitle("tt");     metVsPtmax[2]->Write("tt");
  metVsPtmax[1]->Add(metVsPtmax[3]);
  metVsPtmax[1]->Add(metVsPtmax[5]);
  metVsPtmax[1]->Add(metVsPtmax[6]);
  metVsPtmax[1]->Add(metVsPtmax[7]);
  metVsPtmax[1]->Draw("colz"); metVsPtmax[1]->SetTitle("others"); metVsPtmax[1]->Write("others");
  corrS[2] = metVsPtmax[0]->GetCorrelationFactor();
  corrW[2] = metVsPtmax[4]->GetCorrelationFactor();
  corrT[2] = metVsPtmax[2]->GetCorrelationFactor();
  corrO[2] = metVsPtmax[1]->GetCorrelationFactor();

  TFile *FptminVsDeltaPhi = new TFile("ptminVsDeltaPhi.root","recreate");
  TCanvas cptminVsdeltaPhi("cptminVsdeltaPhi","cptmin",600,600);
  ptminVsDeltaPhi[0]->Draw("colz"); ptminVsDeltaPhi[0]->SetTitle("signal"); ptminVsDeltaPhi[0]->Write("signal");
  ptminVsDeltaPhi[4]->Draw("colz"); ptminVsDeltaPhi[4]->SetTitle("ww");     ptminVsDeltaPhi[4]->Write("ww");
  ptminVsDeltaPhi[2]->Draw("colz"); ptminVsDeltaPhi[2]->SetTitle("tt");     ptminVsDeltaPhi[2]->Write("tt");
  ptminVsDeltaPhi[1]->Add(ptminVsDeltaPhi[3]);
  ptminVsDeltaPhi[1]->Add(ptminVsDeltaPhi[5]);
  ptminVsDeltaPhi[1]->Add(ptminVsDeltaPhi[6]);
  ptminVsDeltaPhi[1]->Add(ptminVsDeltaPhi[7]);
  ptminVsDeltaPhi[1]->Draw("colz"); ptminVsDeltaPhi[1]->SetTitle("others"); ptminVsDeltaPhi[1]->Write("others");
  corrS[3] = ptminVsDeltaPhi[0]->GetCorrelationFactor();
  corrW[3] = ptminVsDeltaPhi[4]->GetCorrelationFactor();
  corrT[3] = ptminVsDeltaPhi[2]->GetCorrelationFactor();
  corrO[3] = ptminVsDeltaPhi[1]->GetCorrelationFactor();

  TFile *FptmaxVsDeltaPhi = new TFile("ptmaxVsDeltaPhi.root","recreate");
  TCanvas cptmaxVsdeltaPhi("cptmaxVsdeltaPhi","cptmax",600,600);
  ptmaxVsDeltaPhi[0]->Draw("colz"); ptmaxVsDeltaPhi[0]->SetTitle("signal"); ptmaxVsDeltaPhi[0]->Write("signal");
  ptmaxVsDeltaPhi[4]->Draw("colz"); ptmaxVsDeltaPhi[4]->SetTitle("ww");     ptmaxVsDeltaPhi[4]->Write("ww");
  ptmaxVsDeltaPhi[2]->Draw("colz"); ptmaxVsDeltaPhi[2]->SetTitle("tt");     ptmaxVsDeltaPhi[2]->Write("tt");
  ptmaxVsDeltaPhi[1]->Add(ptmaxVsDeltaPhi[3]);
  ptmaxVsDeltaPhi[1]->Add(ptmaxVsDeltaPhi[5]);
  ptmaxVsDeltaPhi[1]->Add(ptmaxVsDeltaPhi[6]);
  ptmaxVsDeltaPhi[1]->Add(ptmaxVsDeltaPhi[7]);
  ptmaxVsDeltaPhi[1]->Draw("colz"); ptmaxVsDeltaPhi[1]->SetTitle("others"); ptmaxVsDeltaPhi[1]->Write("others");
  corrS[4] = ptmaxVsDeltaPhi[0]->GetCorrelationFactor();
  corrW[4] = ptmaxVsDeltaPhi[4]->GetCorrelationFactor();
  corrT[4] = ptmaxVsDeltaPhi[2]->GetCorrelationFactor();
  corrO[4] = ptmaxVsDeltaPhi[1]->GetCorrelationFactor();

  TFile *FptminVsPtmax = new TFile("ptminVsPtmax.root","recreate");
  TCanvas cptminVsptmax("cptminVsptmax","cptmin",600,600);
  ptminVsPtmax[0]->Draw("colz"); ptminVsPtmax[0]->SetTitle("signal"); ptminVsPtmax[0]->Write("signal");
  ptminVsPtmax[4]->Draw("colz"); ptminVsPtmax[4]->SetTitle("ww");     ptminVsPtmax[4]->Write("ww");
  ptminVsPtmax[2]->Draw("colz"); ptminVsPtmax[2]->SetTitle("tt");     ptminVsPtmax[2]->Write("tt");
  ptminVsPtmax[1]->Add(ptminVsPtmax[3]);
  ptminVsPtmax[1]->Add(ptminVsPtmax[5]);
  ptminVsPtmax[1]->Add(ptminVsPtmax[6]);
  ptminVsPtmax[1]->Add(ptminVsPtmax[7]);
  ptminVsPtmax[1]->Draw("colz"); ptminVsPtmax[1]->SetTitle("others"); ptminVsPtmax[1]->Write("others");
  corrS[5] = ptminVsPtmax[0]->GetCorrelationFactor();
  corrW[5] = ptminVsPtmax[4]->GetCorrelationFactor();
  corrT[5] = ptminVsPtmax[2]->GetCorrelationFactor();
  corrO[5] = ptminVsPtmax[1]->GetCorrelationFactor();


  char namefile[200];
  sprintf(namefile,"correlation.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::trunc);
  textfile.precision(2);

  textfile << "\\documentclass{article}" << endl;
  textfile << "\\usepackage{rotating}" << endl;
  textfile << "\\begin{document}" << endl;
  
  textfile << "\\begin{small}" << endl
           << "\\begin{center}" << endl
           << "\\begin{tabular}{|c|c|c|c|c|}" << endl
           << "\\hline" << endl
           << "signal          & MET     & DeltaPhi  &  minPt  & maxPt \t\\\\" << endl
           << "\\hline" << endl;
  textfile << "\\hline"        << endl;
  textfile << " MET            &  & "      << corrS[0] << " & " << corrS[1] << " & " << corrS[2] << "\t\\\\" << endl;
  textfile << " DeltaPhi       &  &  & "   << corrS[3] << " & " << corrS[4] << "\t\\\\" << endl;
  textfile << " minPt          &  &  & & " << corrS[5] << "\t\\\\" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\end{center}"  << endl;
  textfile << "\\end{small}"   << endl;
  textfile << endl;  
  textfile << "\\" << endl;  
  textfile << endl;  
  textfile << "\\begin{small}" << endl
           << "\\begin{center}" << endl
           << "\\begin{tabular}{|c|c|c|c|c|}" << endl
           << "\\hline" << endl
           << "WW          & MET     & DeltaPhi  &  minPt  & maxPt \t\\\\" << endl
           << "\\hline" << endl;
  textfile << "\\hline"        << endl;
  textfile << " MET            &  & "      << corrW[0] << " & " << corrW[1] << " & " << corrW[2] << "\t\\\\" << endl;
  textfile << " DeltaPhi       &  &  & "   << corrW[3] << " & " << corrW[4] << "\t\\\\" << endl;
  textfile << " minPt          &  &  & & " << corrW[5] << "\t\\\\" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\end{center}"  << endl;
  textfile << "\\end{small}"   << endl;
  textfile << endl;  
  textfile << "\\" << endl;  
  textfile << endl;  
  textfile << "\\begin{small}" << endl
           << "\\begin{center}" << endl
           << "\\begin{tabular}{|c|c|c|c|c|}" << endl
           << "\\hline" << endl
           << "ttbar          & MET     & DeltaPhi  &  minPt  & maxPt \t\\\\" << endl
           << "\\hline" << endl;
  textfile << "\\hline"        << endl;
  textfile << " MET            &  & "      << corrT[0] << " & " << corrT[1] << " & " << corrT[2] << "\t\\\\" << endl;
  textfile << " DeltaPhi       &  &  & "   << corrT[3] << " & " << corrT[4] << "\t\\\\" << endl;
  textfile << " minPt          &  &  & & " << corrT[5] << "\t\\\\" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\end{center}"  << endl;
  textfile << "\\end{small}"   << endl;
  textfile << endl;  
  textfile << "\\" << endl;  
  textfile << endl;  
  textfile << "\\begin{small}" << endl
           << "\\begin{center}" << endl
           << "\\begin{tabular}{|c|c|c|c|c|}" << endl
           << "\\hline" << endl
           << "others          & MET     & DeltaPhi  &  minPt  & maxPt \t\\\\" << endl
           << "\\hline" << endl;
  textfile << "\\hline"        << endl;
  textfile << " MET            &  & "      << corrO[0] << " & " << corrO[1] << " & " << corrO[2] << "\t\\\\" << endl;
  textfile << " DeltaPhi       &  &  & "   << corrO[3] << " & " << corrO[4] << "\t\\\\" << endl;
  textfile << " minPt          &  &  & & " << corrO[5] << "\t\\\\" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\hline" << endl;
  textfile << "\\end{tabular}" << endl;
  textfile << "\\end{center}"  << endl;
  textfile << "\\end{small}"   << endl;
  textfile	   << "\\end{document}" << endl; 
}




// still old
void drawSignificances() {
  
  // events/fb-1
  std::vector< std::vector<float> > events;
  std::vector<float> higgsMass;
  
  // mH=120
  std::vector<float> eventsH120;
  eventsH120.resize(9);
  eventsH120[0] = 4.2;   // H->WW
  eventsH120[1] = 33.4;  // WW
  eventsH120[2] = 6.9;   // ttbar
  eventsH120[3] = 20.;   // W+jets - from QCD fake
  eventsH120[4] = 10.3;  // Z+jets
  eventsH120[5] = 7.2;   // DY < 40 GeV
  eventsH120[6] = 2.5;   // WZ
  eventsH120[7] = 1.0;   // tW
  eventsH120[8] = 0.5;   // ZZ
  events.push_back(eventsH120);
  higgsMass.push_back(120);

  // mH=130
  std::vector<float> eventsH130;
  eventsH130.resize(9);
  eventsH130[0] = 6.6;   // H->WW
  eventsH130[1] = 33.9;  // WW
  eventsH130[2] = 6.9;   // ttbar
  eventsH130[3] = 20.;   // W+jets - from QCD fake
  eventsH130[4] = 10.3;  // Z+jets
  eventsH130[5] = 7.3;   // DY < 40 GeV
  eventsH130[6] = 2.5;   // WZ
  eventsH130[7] = 1.0;   // tW
  eventsH130[8] = 0.7;   // ZZ
  events.push_back(eventsH130);
  higgsMass.push_back(130);

  // mH=140
  std::vector<float> eventsH140;
  eventsH140.resize(9);
  eventsH140[0] = 11.3;   // H->WW
  eventsH140[1] = 22.4;  // WW
  eventsH140[2] = 6.4;   // ttbar
  eventsH140[3] = 5.0;  // W+jets - from QCD fake
  eventsH140[4] = 2.0;  // Z+jets
  eventsH140[5] = 0.0;   // DY < 40 GeV
  eventsH140[6] = 2.2;   // WZ
  eventsH140[7] = 0.0;   // tW
  eventsH140[8] = 0.0;   // ZZ
  events.push_back(eventsH140);
  higgsMass.push_back(140);

  // mH=150
  std::vector<float> eventsH150;
  eventsH150.resize(9);
  eventsH150[0] = 12.5;   // H->WW
  eventsH150[1] = 13.5;  // WW
  eventsH150[2] = 4.3;   // ttbar
  eventsH150[3] = 2.9;  // W+jets - from QCD fake
  eventsH150[4] = 1.0;  // Z+jets
  eventsH150[5] = 0.0;   // DY < 40 GeV
  eventsH150[6] = 1.7;   // WZ
  eventsH150[7] = 0.0;   // tW
  eventsH150[8] = 0.0;   // ZZ
  events.push_back(eventsH150);
  higgsMass.push_back(150);
    
  // mH=160
  std::vector<float> eventsH160;
  eventsH160.resize(9);
  eventsH160[0] = 20.1;   // H->WW
  eventsH160[1] = 13.7;  // WW
  eventsH160[2] = 4.3;   // ttbar
  eventsH160[3] = 3.5;  // W+jets - from QCD fake
  eventsH160[4] = 1.0;  // Z+jets
  eventsH160[5] = 0.0;   // DY < 40 GeV
  eventsH160[6] = 1.7;   // WZ
  eventsH160[7] = 0.0;   // tW
  eventsH160[8] = 0.0;   // ZZ
  events.push_back(eventsH160);
  higgsMass.push_back(160);

  // mH=165
  std::vector<float> eventsH165;
  eventsH165.resize(9);
  eventsH165[0] = 21.8;   // H->WW
  eventsH165[1] = 13.7;  // WW
  eventsH165[2] = 4.3;   // ttbar
  eventsH165[3] = 3.5;  // W+jets - from QCD fake
  eventsH165[4] = 1.8;  // Z+jets
  eventsH165[5] = 0.0;   // DY < 40 GeV
  eventsH165[6] = 1.7;   // WZ
  eventsH165[7] = 0.0;   // tW
  eventsH165[8] = 0.0;   // ZZ
  events.push_back(eventsH165);
  higgsMass.push_back(165);

  // mH=170
  std::vector<float> eventsH170;
  eventsH170.resize(9);
  eventsH170[0] = 21.2;   // H->WW
  eventsH170[1] = 14.4;  // WW
  eventsH170[2] = 7.8;   // ttbar
  eventsH170[3] = 1.7;  // W+jets - from QCD fake
  eventsH170[4] = 1.8;  // Z+jets
  eventsH170[5] = 0.0;   // DY < 40 GeV
  eventsH170[6] = 2.9;   // WZ
  eventsH170[7] = 0.0;   // tW
  eventsH170[8] = 0.2;   // ZZ
  events.push_back(eventsH170);
  higgsMass.push_back(170);

  // mH=180
  std::vector<float> eventsH180;
  eventsH180.resize(9);
  eventsH180[0] = 15.1;   // H->WW
  eventsH180[1] = 14.8;  // WW
  eventsH180[2] = 8.2;   // ttbar
  eventsH180[3] = 1.7;  // W+jets - from QCD fake
  eventsH180[4] = 2.8;  // Z+jets
  eventsH180[5] = 0.0;   // DY < 40 GeV
  eventsH180[6] = 3.2;   // WZ
  eventsH180[7] = 0.5;   // tW
  eventsH180[8] = 0.2;   // ZZ
  events.push_back(eventsH180);
  higgsMass.push_back(180);

  // mH=190
  std::vector<float> eventsH190;
  eventsH190.resize(9);
  eventsH190[0] = 11.;   // H->WW
  eventsH190[1] = 15.0;  // WW
  eventsH190[2] = 8.6;   // ttbar
  eventsH190[3] = 1.7;  // W+jets - from QCD fake
  eventsH190[4] = 2.8;  // Z+jets
  eventsH190[5] = 0.0;   // DY < 40 GeV
  eventsH190[6] = 3.2;   // WZ
  eventsH190[7] = 0.5;   // tW
  eventsH190[8] = 0.2;   // ZZ
  events.push_back(eventsH190);
  higgsMass.push_back(190);

  // mH=200
  std::vector<float> eventsH200;
  eventsH200.resize(9);
  eventsH200[0] = 9.3;   // H->WW
  eventsH200[1] = 21.8;  // WW
  eventsH200[2] = 9.9;   // ttbar
  eventsH200[3] = 3.2;  // W+jets - from QCD fake
  eventsH200[4] = 5.0;  // Z+jets
  eventsH200[5] = 0.0;   // DY < 40 GeV
  eventsH200[6] = 4.2;   // WZ
  eventsH200[7] = 0.8;   // tW
  eventsH200[8] = 0.2;   // ZZ
  events.push_back(eventsH200);
  higgsMass.push_back(200);


  TGraph *gStatSig = new TGraph( (int)events.size() );
  TGraph *gSoverB = new TGraph( (int)events.size() );

  for(unsigned int imass=0; imass<events.size(); imass++) {
    
    std::vector<float> nEvents = events[imass];

    float nSig = nEvents[0];
    float nBkg = 0.0;

    for(unsigned int iBkg=1; iBkg<nEvents.size(); iBkg++) {
      nBkg += nEvents[iBkg];
    }

    gStatSig->SetPoint(imass, higgsMass[imass], nSig/sqrt(nBkg));
    gSoverB->SetPoint(imass, higgsMass[imass], nSig/nBkg);

    std::cout << "Higgs mass = " << higgsMass[imass] 
	      << " has S/sqrt(B) = " <<  nSig/sqrt(nBkg)
	      << " and S/B = " << nSig/nBkg << std::endl;

  }

  TCanvas csignificance;
  gStatSig->SetLineColor(34);
  gStatSig->SetLineWidth(2);
  gStatSig->SetMarkerColor(34);
  gStatSig->SetMarkerStyle(8);
  gStatSig->SetMarkerSize(2);
  gStatSig->SetMinimum(0);
  gStatSig->SetTitle("");
  gStatSig->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  gStatSig->GetYaxis()->SetTitle("n_{S} / #sqrt{n_{B}}");
  gStatSig->Draw("ACP");
  csignificance.SaveAs("statSign.eps");

  TCanvas csoverb;
  gSoverB->SetLineColor(34);
  gSoverB->SetLineWidth(2);
  gSoverB->SetMarkerColor(34);
  gSoverB->SetMarkerStyle(8);
  gSoverB->SetMarkerSize(2);
  gSoverB->SetMinimum(0);
  gSoverB->SetTitle("");
  gSoverB->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  gSoverB->GetYaxis()->SetTitle("n_{S} / n_{B}");
  gSoverB->Draw("ACP");
  csoverb.SaveAs("sigOverBkg.eps");

}
