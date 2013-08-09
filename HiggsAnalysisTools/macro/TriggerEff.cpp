// to compile with 
// g++ TriggerEff.cpp -o triggerEff `root-config --libs --cflags --glibs` -Wno-deprecated

// input: txt inputFile with list of .root file + corresponding mass

// -------------------------------------
//
// Produce grafici con :
//         efficienza dell'OR dei trigger elettronici 
//         efficienza del trigger single electron          
//         efficienza del trigger single relaxed electron      
// calcolate rispetto agli eventi totali del tree ridott
// (quelli con (1)2 elettroni provenienti dalla W nell'accettanza )
// 
// Tabella con efficienza ed errore dell'OR dei 2 trigger elettronici  
//
// -------------------------------------


//! c++ includes                                                               
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream.h>
#include <unistd.h>
#include <fstream>
#include <math.h>

//! ROOT includes                                                             
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TBranch.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TLegend.h"
#include "TText.h"
#include "TSelector.h"
#include "TApplication.h"
#include "TLatex.h"

int main ( int argc, char **argv)
{
  // reading input/output file names
  char inFileName[150];
  if (argc==2) { strcpy(inFileName,argv[1]); }
  else
    {
      cout << "missing argument. Insert: " << endl; 
      cout << "inputFile with ntuples name + mass" << endl; 
      return 1;
    }

  // -------------------------
  // Arrays x TGraph 
  float massErr_ee_hlt_ele[100], massErr_mm_hlt_mu[100], massErr_em_hlt_mix[100];
  float eff_ee_hlt_ele[100], effErr_ee_hlt_ele[100];
  float eff_ee_hlt_se[100],  effErr_ee_hlt_se[100];
  float eff_ee_hlt_ser[100], effErr_ee_hlt_ser[100];
  float eff_mm_hlt_mu[100],  effErr_mm_hlt_mu[100];
  float eff_mm_hlt_sm[100],  effErr_mm_hlt_sm[100];
  float eff_mm_hlt_smr[100], effErr_mm_hlt_smr[100];
  float eff_em_hlt_ele[100], effErr_em_hlt_ele[100];
  float eff_em_hlt_se[100],  effErr_em_hlt_se[100];
  float eff_em_hlt_ser[100], effErr_em_hlt_ser[100];
  float eff_em_hlt_mu[100],  effErr_em_hlt_mu[100];
  float eff_em_hlt_sm[100],  effErr_em_hlt_sm[100];
  float eff_em_hlt_smr[100], effErr_em_hlt_smr[100];
  float eff_em_hlt_mix[100], effErr_em_hlt_mix[100];
  float eff_em_hlt_smix[100],  effErr_em_hlt_smix[100];
  float eff_em_hlt_smixr[100], effErr_em_hlt_smixr[100];

  // -------------------------
  // input file: list with ntuples and respective masses
  char Buffer[500]; 
  int IFC = 0;
  char RefFile[50][500];      
  float RefMass[50];        

  ifstream *inputFile = new ifstream(inFileName); 
  while( !(inputFile->eof()) ){  
    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
      sscanf(Buffer,"%s %f",RefFile[IFC], &RefMass[IFC]);
      cout << RefFile[IFC] << ", " << RefMass[IFC] << endl;
      IFC++; 
    }
  }
  inputFile->close();
  delete inputFile;

  cout << endl;
  cout << endl;
  
  
  // -------------------------
  //  Loop over the reference files
  for ( int my_file = 0; my_file < IFC; my_file++ ){
    
    cout << "Working with file " << RefFile[my_file] << ", for mass = " << RefMass[my_file] << endl;

    // -------------------------
    // loading file:
    TFile *f_ntupla = TFile::Open(RefFile[my_file]);

    // -------------------------  
    // tree construction:   
    TTree *tree = (TTree*) f_ntupla->Get("T1");

    // declaration of leaves types
    Char_t decayEE;
    Char_t decayMM;
    Char_t decayEM;
    Char_t promptDecayEE;
    Char_t promptDecayMM;
    Char_t promptDecayEM;
    Char_t HLTSingleElectron;
    Char_t HLTSingleElectronRelaxed;
    Char_t HLTSingleElectronOR;
    Char_t HLTSingleMuon;
    Char_t HLTSingleMuonRelaxed;
    Char_t HLTSingleMuonOR;

    // declaration of branches
    TBranch *b_decayEE;
    TBranch *b_decayMM;
    TBranch *b_decayEM;
    TBranch *b_promptDecayEE;
    TBranch *b_promptDecayMM;
    TBranch *b_promptDecayEM;
    TBranch *b_HLTSingleElectron;
    TBranch *b_HLTSingleElectronRelaxed;
    TBranch *b_HLTSingleElectronOR;
    TBranch *b_HLTSingleMuon;
    TBranch *b_HLTSingleMuonRelaxed;
    TBranch *b_HLTSingleMuonOR;

    // setting branch addresses
    tree->SetMakeClass(1);
    tree->SetBranchAddress("decayEE", &decayEE, &b_decayEE);
    tree->SetBranchAddress("decayMM", &decayMM, &b_decayMM);
    tree->SetBranchAddress("decayEM", &decayEM, &b_decayEM);
    tree->SetBranchAddress("promptDecayEE", &promptDecayEE, &b_promptDecayEE);
    tree->SetBranchAddress("promptDecayMM", &promptDecayMM, &b_promptDecayMM);
    tree->SetBranchAddress("promptDecayEM", &promptDecayEM, &b_promptDecayEM);
    tree->SetBranchAddress("HLTSingleElectron", &HLTSingleElectron, &b_HLTSingleElectron);
    tree->SetBranchAddress("HLTSingleElectronRelaxed", &HLTSingleElectronRelaxed, &b_HLTSingleElectronRelaxed);
    tree->SetBranchAddress("HLTSingleElectronOR", &HLTSingleElectronOR, &b_HLTSingleElectronOR);
    tree->SetBranchAddress("HLTSingleMuon", &HLTSingleMuon, &b_HLTSingleMuon);
    tree->SetBranchAddress("HLTSingleMuonRelaxed", &HLTSingleMuonRelaxed, &b_HLTSingleMuonRelaxed);
    tree->SetBranchAddress("HLTSingleMuonOR", &HLTSingleMuonOR, &b_HLTSingleMuonOR);

    // -------------------------  
    // counters
    float ev_total      = 0.;
    float ev_ee         = 0.;
    float ev_mm         = 0.;
    float ev_em         = 0.;
    float ev_ee_prompt  = 0.;
    float ev_mm_prompt  = 0.;
    float ev_em_prompt  = 0.;
    float ev_ee_hlt_ele    = 0.;  // se || ser
    float ev_ee_hlt_se     = 0.;  // single ele 
    float ev_ee_hlt_ser    = 0.;  // single ele relaxed
    float ev_mm_hlt_mu     = 0.;  // sm || smr
    float ev_mm_hlt_sm     = 0.;  // single muon 
    float ev_mm_hlt_smr    = 0.;  // single muon relaxed
    float ev_em_hlt_mix    = 0.;  // se || ser || sm || smr
    float ev_em_hlt_smix   = 0.;  // se || sm
    float ev_em_hlt_smixr  = 0.;  // ser || smr
    float ev_em_hlt_ele    = 0.;  // se || ser
    float ev_em_hlt_se     = 0.;  // single ele 
    float ev_em_hlt_ser    = 0.;  // single ele relaxed
    float ev_em_hlt_mu     = 0.;  // sm || smr
    float ev_em_hlt_sm     = 0.;  // single muon 
    float ev_em_hlt_smr    = 0.;  // single muon relaxed


    // -------------------------  
    // loop on the events
    float nEnt=tree->GetEntries();
    for (int entry=0; entry<nEnt; entry++){ 

      tree->GetEntry(entry);

      ev_total++;

      // to compute trigger efficiency
      if ( promptDecayEE ) {
	ev_ee_prompt++;
	if ( HLTSingleElectron ) ev_ee_hlt_se++;
	if ( HLTSingleElectronRelaxed ) ev_ee_hlt_ser++;
	if ( HLTSingleElectronOR ) ev_ee_hlt_ele++;
      } 
      
      if ( promptDecayMM ) {
	ev_mm_prompt++;
	if ( HLTSingleMuon ) ev_mm_hlt_sm++;
	if ( HLTSingleMuonRelaxed ) ev_mm_hlt_smr++;
	if ( HLTSingleMuonOR ) ev_mm_hlt_mu++;
      } 

      if ( promptDecayEM ) {
	ev_em_prompt++;
	if ( HLTSingleElectron ) ev_em_hlt_se++;
	if ( HLTSingleElectronRelaxed ) ev_em_hlt_ser++;
	if ( HLTSingleElectronOR ) ev_em_hlt_ele++;
	if ( HLTSingleMuon ) ev_em_hlt_sm++;
	if ( HLTSingleMuonRelaxed ) ev_em_hlt_smr++;
	if ( HLTSingleMuonOR ) ev_em_hlt_mu++;
	
	if ( HLTSingleElectron || HLTSingleMuon ) ev_em_hlt_smix++;
	if ( HLTSingleElectronRelaxed || HLTSingleMuonRelaxed ) ev_em_hlt_smixr++;
	if ( HLTSingleElectronOR || HLTSingleMuonOR ) ev_em_hlt_mix++;
      } 

    } // end loop on the events
    
    
    // -------------------------        
    // efficiencies
    float this_massErr     = 0;

    float this_ee_eff_hlt_ele    = ev_ee_hlt_ele/ev_ee_prompt;
    float this_ee_eff_hlt_se     = ev_ee_hlt_se/ev_ee_prompt;
    float this_ee_eff_hlt_ser    = ev_ee_hlt_ser/ev_ee_prompt;

    float this_ee_effErr_hlt_ele = (pow(ev_ee_hlt_ele*(1-this_ee_eff_hlt_ele),0.5))/ev_ee_hlt_ele;
    float this_ee_effErr_hlt_se  = (pow(ev_ee_hlt_se*(1-this_ee_eff_hlt_se),0.5))/ev_ee_hlt_se;
    float this_ee_effErr_hlt_ser = (pow(ev_ee_hlt_ser*(1-this_ee_eff_hlt_ser),0.5))/ev_ee_hlt_ser;


    float this_mm_eff_hlt_mu    = ev_mm_hlt_mu/ev_mm_prompt;
    float this_mm_eff_hlt_sm    = ev_mm_hlt_sm/ev_mm_prompt;
    float this_mm_eff_hlt_smr   = ev_mm_hlt_smr/ev_mm_prompt;

    float this_mm_effErr_hlt_mu  = (pow(ev_mm_hlt_mu*(1-this_mm_eff_hlt_mu),0.5))/ev_mm_hlt_mu;
    float this_mm_effErr_hlt_sm  = (pow(ev_mm_hlt_sm*(1-this_mm_eff_hlt_sm),0.5))/ev_mm_hlt_sm;
    float this_mm_effErr_hlt_smr = (pow(ev_mm_hlt_smr*(1-this_mm_eff_hlt_smr),0.5))/ev_mm_hlt_smr;


    float this_em_eff_hlt_mix     = ev_em_hlt_mix/ev_em_prompt;
    float this_em_eff_hlt_smix    = ev_em_hlt_smix/ev_em_prompt;
    float this_em_eff_hlt_smixr   = ev_em_hlt_smixr/ev_em_prompt;
    float this_em_eff_hlt_ele     = ev_em_hlt_ele/ev_em_prompt;
    float this_em_eff_hlt_se      = ev_em_hlt_se/ev_em_prompt;
    float this_em_eff_hlt_ser     = ev_em_hlt_ser/ev_em_prompt;
    float this_em_eff_hlt_mu      = ev_em_hlt_mu/ev_em_prompt;
    float this_em_eff_hlt_sm      = ev_em_hlt_sm/ev_em_prompt;
    float this_em_eff_hlt_smr     = ev_em_hlt_smr/ev_em_prompt;

    float this_em_effErr_hlt_mix   = (pow(ev_em_hlt_mix*(1-this_em_eff_hlt_mix),0.5))/ev_em_hlt_mix;
    float this_em_effErr_hlt_smix  = (pow(ev_em_hlt_smix*(1-this_em_eff_hlt_smix),0.5))/ev_em_hlt_smix;
    float this_em_effErr_hlt_smixr = (pow(ev_em_hlt_smixr*(1-this_em_eff_hlt_smixr),0.5))/ev_em_hlt_smixr;
    float this_em_effErr_hlt_ele  = (pow(ev_em_hlt_ele*(1-this_em_eff_hlt_ele),0.5))/ev_em_hlt_ele;
    float this_em_effErr_hlt_se  = (pow(ev_em_hlt_se*(1-this_em_eff_hlt_se),0.5))/ev_em_hlt_se;
    float this_em_effErr_hlt_ser = (pow(ev_em_hlt_ser*(1-this_em_eff_hlt_ser),0.5))/ev_em_hlt_ser;
    float this_em_effErr_hlt_mu  = (pow(ev_em_hlt_mu*(1-this_em_eff_hlt_mu),0.5))/ev_em_hlt_mu;
    float this_em_effErr_hlt_sm  = (pow(ev_em_hlt_sm*(1-this_em_eff_hlt_sm),0.5))/ev_em_hlt_sm;
    float this_em_effErr_hlt_smr = (pow(ev_em_hlt_smr*(1-this_em_eff_hlt_smr),0.5))/ev_em_hlt_smr;

       
    // filling the array with results      
    eff_ee_hlt_ele[my_file]     = this_ee_eff_hlt_ele;    
    eff_ee_hlt_se[my_file]      = this_ee_eff_hlt_se;    
    eff_ee_hlt_ser[my_file]     = this_ee_eff_hlt_ser;    
    effErr_ee_hlt_ele[my_file]  = this_ee_effErr_hlt_ele;    
    effErr_ee_hlt_se[my_file]   = this_ee_effErr_hlt_se;    
    effErr_ee_hlt_ser[my_file]  = this_ee_effErr_hlt_ser;    
    massErr_ee_hlt_ele[my_file] = 0.;

    eff_mm_hlt_mu[my_file]      = this_mm_eff_hlt_mu;    
    eff_mm_hlt_sm[my_file]      = this_mm_eff_hlt_sm;    
    eff_mm_hlt_smr[my_file]     = this_mm_eff_hlt_smr;    
    effErr_mm_hlt_mu[my_file]   = this_mm_effErr_hlt_mu;    
    effErr_mm_hlt_sm[my_file]   = this_mm_effErr_hlt_sm;    
    effErr_mm_hlt_smr[my_file]  = this_mm_effErr_hlt_smr;    
    massErr_mm_hlt_mu[my_file] = 0.;

    eff_em_hlt_ele[my_file]     = this_em_eff_hlt_ele;    
    eff_em_hlt_se[my_file]      = this_em_eff_hlt_se;    
    eff_em_hlt_ser[my_file]     = this_em_eff_hlt_ser;    
    effErr_em_hlt_ele[my_file]  = this_em_effErr_hlt_ele;    
    effErr_em_hlt_se[my_file]   = this_em_effErr_hlt_se;    
    effErr_em_hlt_ser[my_file]  = this_em_effErr_hlt_ser;    
    eff_em_hlt_mu[my_file]      = this_em_eff_hlt_mu;    
    eff_em_hlt_sm[my_file]      = this_em_eff_hlt_sm;    
    eff_em_hlt_smr[my_file]     = this_em_eff_hlt_smr;    
    effErr_em_hlt_mu[my_file]   = this_em_effErr_hlt_mu;    
    effErr_em_hlt_sm[my_file]   = this_em_effErr_hlt_sm;    
    effErr_em_hlt_smr[my_file]  = this_em_effErr_hlt_smr;    
    eff_em_hlt_mix[my_file]       = this_em_eff_hlt_mix;    
    eff_em_hlt_smix[my_file]      = this_em_eff_hlt_smix;    
    eff_em_hlt_smixr[my_file]     = this_em_eff_hlt_smixr;    
    effErr_em_hlt_mix[my_file]    = this_em_effErr_hlt_mix;    
    effErr_em_hlt_smix[my_file]   = this_em_effErr_hlt_smix;    
    effErr_em_hlt_smixr[my_file]  = this_em_effErr_hlt_smixr;    
    massErr_em_hlt_mix[my_file] = 0.;

  }// loop over the files

  
  // statistics
  for ( int my_file = 0; my_file<IFC; my_file++ ){
    cout << endl;
    cout << "file = " << RefFile[my_file] << endl;

    cout << "ee passing OR Electron HLT  : " << eff_ee_hlt_ele[my_file] << " wrt the total # of events" << endl;  
    cout << "ee passing SingleEl         : " << eff_ee_hlt_se[my_file]  << " wrt the total # of events" << endl;  
    cout << "ee passing SingleRelaxedEl  : " << eff_ee_hlt_ser[my_file] << " wrt the total # of events" << endl;  

    cout << "mm passing OR Muon HLT      : " << eff_mm_hlt_mu[my_file]  << " wrt the total # of events" << endl;  
    cout << "mm passing SingleMuon       : " << eff_mm_hlt_sm[my_file]  << " wrt the total # of events" << endl;  
    cout << "mm passing SingleRelaxedMuon: " << eff_mm_hlt_smr[my_file] << " wrt the total # of events" << endl;  

    cout << "em passing OR Electron HLT  : " << eff_em_hlt_ele[my_file] << " wrt the total # of events" << endl;  
    cout << "em passing SingleEl         : " << eff_em_hlt_se[my_file]  << " wrt the total # of events" << endl;  
    cout << "em passing SingleRelaxedEl  : " << eff_em_hlt_ser[my_file] << " wrt the total # of events" << endl;  
    cout << "em passing OR Muon HLT      : " << eff_em_hlt_mu[my_file] << " wrt the total # of events"  << endl;  
    cout << "em passing SingleMuon       : " << eff_em_hlt_sm[my_file]  << " wrt the total # of events" << endl;  
    cout << "em passing SingleRelaxedMuon: " << eff_em_hlt_smr[my_file] << " wrt the total # of events" << endl;  
    cout << "em passing OR Mix HLT      : " << eff_em_hlt_mix[my_file]   << " wrt the total # of events" << endl;  
    cout << "em passing SingleMix       : " << eff_em_hlt_smix[my_file]  << " wrt the total # of events" << endl;  
    cout << "em passing SingleRelaxedMix: " << eff_em_hlt_smixr[my_file] << " wrt the total # of events" << endl;  

    cout << endl;
  }

  cout << endl;
  cout << endl;
//   for ( int my_file = 0; my_file<IFC; my_file++ ){
//     cout << "file = " << RefFile[my_file] << ", eff HLT = " << eff_hlt_ele[my_file] << " +- " << effErr_hlt_ele[my_file] << endl;     
//   }

  // TGraphs
  // --- ee ---
  TGraphErrors *G_ee_Eff_HLT_ELE = new TGraphErrors(IFC, RefMass, eff_ee_hlt_ele, massErr_ee_hlt_ele, effErr_ee_hlt_ele);
  TGraphErrors *G_ee_Eff_HLT_SE  = new TGraphErrors(IFC, RefMass, eff_ee_hlt_se,  massErr_ee_hlt_ele, effErr_ee_hlt_se);
  TGraphErrors *G_ee_Eff_HLT_SER = new TGraphErrors(IFC, RefMass, eff_ee_hlt_ser, massErr_ee_hlt_ele, effErr_ee_hlt_ser);

  G_ee_Eff_HLT_ELE->SetMarkerColor(1);  G_ee_Eff_HLT_ELE->SetMarkerSize(1.2);  G_ee_Eff_HLT_ELE->SetMarkerStyle(8);
  G_ee_Eff_HLT_SE ->SetMarkerColor(1);  G_ee_Eff_HLT_SE ->SetMarkerSize(1.);   G_ee_Eff_HLT_SE ->SetMarkerStyle(22);
  G_ee_Eff_HLT_SER->SetMarkerColor(1);  G_ee_Eff_HLT_SER->SetMarkerSize(1.);   G_ee_Eff_HLT_SER->SetMarkerStyle(4); 

  TApplication* theApp = new TApplication("App", &argc, argv);

  TStyle *tesiStyle = new TStyle("tesiStyle","");
  tesiStyle->SetCanvasColor(0);
  tesiStyle->SetFrameFillColor(0);
  tesiStyle->SetStatColor(0);
  tesiStyle->SetOptStat(0);
  tesiStyle->SetTitleFillColor(0);
  tesiStyle->SetCanvasBorderMode(0);
  tesiStyle->SetPadBorderMode(0);
  tesiStyle->SetFrameBorderMode(0);
  tesiStyle->SetLineWidth(1);
  tesiStyle->cd();
  
  TH2F *myFuffa = new TH2F("myFuffa", "", 100, 120., 190., 100, 0., 1.);
  myFuffa -> SetTitle("");
  myFuffa -> SetLabelSize(0.035,"X");
  myFuffa -> SetLabelSize(0.035,"Y");
  myFuffa -> GetXaxis()->SetTitle("m_{H} (GeV)");
  myFuffa -> GetYaxis()->SetTitle("Efficiency");

  TCanvas c1 ("c1","",1000,700);
  myFuffa       -> Draw();
  G_ee_Eff_HLT_ELE -> Draw("P");
  G_ee_Eff_HLT_SE  -> Draw("P");
  G_ee_Eff_HLT_SER -> Draw("P");

  TLegend *leg =new TLegend(0.11,0.65,0.45,0.89);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetFillColor(0);
  leg->AddEntry(G_ee_Eff_HLT_ELE, "Single OR SingleRelaxed Electron","p");
  leg->AddEntry(G_ee_Eff_HLT_SE,  "Single Electron","p");
  leg->AddEntry(G_ee_Eff_HLT_SER, "Single Relaxed Electron","p");
  leg->Draw();

  c1.SaveAs("ee_trigger_eff.root");

  // --- mm ---
  TGraphErrors *G_mm_Eff_HLT_MU  = new TGraphErrors(IFC, RefMass, eff_mm_hlt_mu,  massErr_mm_hlt_mu, effErr_mm_hlt_mu);
  TGraphErrors *G_mm_Eff_HLT_SM  = new TGraphErrors(IFC, RefMass, eff_mm_hlt_sm,  massErr_mm_hlt_mu, effErr_mm_hlt_sm);
  TGraphErrors *G_mm_Eff_HLT_SMR = new TGraphErrors(IFC, RefMass, eff_mm_hlt_smr, massErr_mm_hlt_mu, effErr_mm_hlt_smr);

  G_mm_Eff_HLT_MU->SetMarkerColor(1);   G_mm_Eff_HLT_MU->SetMarkerSize(1.2);   G_mm_Eff_HLT_MU->SetMarkerStyle(8);
  G_mm_Eff_HLT_SM ->SetMarkerColor(1);  G_mm_Eff_HLT_SM ->SetMarkerSize(1.);   G_mm_Eff_HLT_SM ->SetMarkerStyle(22);
  G_mm_Eff_HLT_SMR->SetMarkerColor(1);  G_mm_Eff_HLT_SMR->SetMarkerSize(1.);   G_mm_Eff_HLT_SMR->SetMarkerStyle(4); 

  TCanvas c2 ("c2","",1000,700);
  myFuffa       -> Draw();
  G_mm_Eff_HLT_MU -> Draw("P");
  G_mm_Eff_HLT_SM  -> Draw("P");
  G_mm_Eff_HLT_SMR -> Draw("P");

  TLegend *leg2 =new TLegend(0.11,0.65,0.45,0.89);
  leg2->SetBorderSize(0);
  leg2->SetLineColor(0);
  leg2->SetFillColor(0);
  leg2->AddEntry(G_mm_Eff_HLT_MU,  "Single OR SingleRelaxed Muon","p");
  leg2->AddEntry(G_mm_Eff_HLT_SM,  "Single Muon","p");
  leg2->AddEntry(G_mm_Eff_HLT_SMR, "Single Relaxed Muon","p");
  leg2->Draw();

  c2.SaveAs("mm_trigger_eff.root");


  // --- em ---
  TGraphErrors *G_em_Eff_HLT_MU  = new TGraphErrors(IFC, RefMass, eff_em_hlt_mu,  massErr_em_hlt_mix, effErr_em_hlt_mu);
  TGraphErrors *G_em_Eff_HLT_SM  = new TGraphErrors(IFC, RefMass, eff_em_hlt_sm,  massErr_em_hlt_mix, effErr_em_hlt_sm);
  TGraphErrors *G_em_Eff_HLT_SMR = new TGraphErrors(IFC, RefMass, eff_em_hlt_smr, massErr_em_hlt_mix, effErr_em_hlt_smr);

  TGraphErrors *G_em_Eff_HLT_ELE = new TGraphErrors(IFC, RefMass, eff_em_hlt_ele, massErr_em_hlt_mix, effErr_em_hlt_ele);
  TGraphErrors *G_em_Eff_HLT_SE  = new TGraphErrors(IFC, RefMass, eff_em_hlt_se,  massErr_em_hlt_mix, effErr_em_hlt_se);
  TGraphErrors *G_em_Eff_HLT_SER = new TGraphErrors(IFC, RefMass, eff_em_hlt_ser, massErr_em_hlt_mix, effErr_em_hlt_ser);

  TGraphErrors *G_em_Eff_HLT_MIX   = new TGraphErrors(IFC, RefMass, eff_em_hlt_mix,   massErr_em_hlt_mix, effErr_em_hlt_mix);
  TGraphErrors *G_em_Eff_HLT_SMIX  = new TGraphErrors(IFC, RefMass, eff_em_hlt_smix,  massErr_em_hlt_mix, effErr_em_hlt_smix);
  TGraphErrors *G_em_Eff_HLT_SMIXR = new TGraphErrors(IFC, RefMass, eff_em_hlt_smixr, massErr_em_hlt_mix, effErr_em_hlt_smixr);

  G_em_Eff_HLT_MU->SetMarkerColor(2);   G_em_Eff_HLT_MU->SetLineColor(2);   G_em_Eff_HLT_MU->SetMarkerSize(1.);    G_em_Eff_HLT_MU->SetMarkerStyle(8);
  G_em_Eff_HLT_SM ->SetMarkerColor(2);  G_em_Eff_HLT_SM ->SetLineColor(2);  G_em_Eff_HLT_SM ->SetMarkerSize(1.);   G_em_Eff_HLT_SM ->SetMarkerStyle(22);
  G_em_Eff_HLT_SMR->SetMarkerColor(2);  G_em_Eff_HLT_SMR->SetLineColor(2);  G_em_Eff_HLT_SMR->SetMarkerSize(1.);   G_em_Eff_HLT_SMR->SetMarkerStyle(4); 

  G_em_Eff_HLT_ELE->SetMarkerColor(3);  G_em_Eff_HLT_ELE->SetLineColor(3);     G_em_Eff_HLT_ELE->SetMarkerSize(1.);   G_em_Eff_HLT_ELE->SetMarkerStyle(8);
  G_em_Eff_HLT_SE ->SetMarkerColor(3);  G_em_Eff_HLT_SE ->SetLineColor(3);  G_em_Eff_HLT_SE ->SetMarkerSize(1.);   G_em_Eff_HLT_SE ->SetMarkerStyle(22);
  G_em_Eff_HLT_SER->SetMarkerColor(3);  G_em_Eff_HLT_SER->SetLineColor(3);  G_em_Eff_HLT_SER->SetMarkerSize(1.);   G_em_Eff_HLT_SER->SetMarkerStyle(4); 

  G_em_Eff_HLT_MIX->SetMarkerColor(1);    G_em_Eff_HLT_MIX->SetMarkerSize(1.2);    G_em_Eff_HLT_MIX->SetMarkerStyle(8);
  G_em_Eff_HLT_SMIX ->SetMarkerColor(1);  G_em_Eff_HLT_SMIX ->SetMarkerSize(1.);   G_em_Eff_HLT_SMIX ->SetMarkerStyle(22);
  G_em_Eff_HLT_SMIXR->SetMarkerColor(1);  G_em_Eff_HLT_SMIXR->SetMarkerSize(1.);   G_em_Eff_HLT_SMIXR->SetMarkerStyle(4); 


  TCanvas c3 ("c3","",1000,700);
  myFuffa       -> Draw();
  G_em_Eff_HLT_MU  -> Draw("P");
  G_em_Eff_HLT_SM  -> Draw("P");
  G_em_Eff_HLT_SMR -> Draw("P");
  G_em_Eff_HLT_ELE -> Draw("P");
  G_em_Eff_HLT_SE  -> Draw("P");
  G_em_Eff_HLT_SER -> Draw("P");
  G_em_Eff_HLT_MIX -> Draw("P");
  G_em_Eff_HLT_SMIX  -> Draw("P");
  G_em_Eff_HLT_SMIXR -> Draw("P");

  TLegend *leg3 =new TLegend(0.11,0.65,0.45,0.89);
  leg3->SetBorderSize(0);
  leg3->SetLineColor(0);
  leg3->SetFillColor(0);
  leg3->AddEntry(G_em_Eff_HLT_MU,    "Single OR SingleRelaxed Muon","p");
  leg3->AddEntry(G_em_Eff_HLT_SM,    "Single Muon","p");
  leg3->AddEntry(G_em_Eff_HLT_SMR,   "Single Relaxed Muon","p");
  leg3->AddEntry(G_em_Eff_HLT_ELE,   "Single OR SingleRelaxed Electron","p");
  leg3->AddEntry(G_em_Eff_HLT_SE,    "Single Electron","p");
  leg3->AddEntry(G_em_Eff_HLT_SER,   "Single Relaxed Electron","p");
  leg3->AddEntry(G_em_Eff_HLT_MIX,   "Single Electron OR SingleRelaxed Electron OR Single Muon OR Single Relaxed Muon","p");
  leg3->AddEntry(G_em_Eff_HLT_SMIX,  "Single Electron OR Single Muon","p");
  leg3->AddEntry(G_em_Eff_HLT_SMIXR, "Single Relaxed Electron OR Single Relaxed Muon","p");
  leg3->Draw();

  c3.SaveAs("em_trigger_eff.root");

  theApp->Run(kFALSE);  
  
} // end;
  
