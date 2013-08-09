#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH2F.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <iostream>
#include <PUWeight.C>
#include <DYWeighter.cc>

using namespace std;

Float_t lumiA = 2.1;
Float_t lumiB = 2.5;

float getOfflineEff(float pT, float eta, TH2F *myH);

void addWeights(const char* filename, float baseW, int processId, int finalstate, int release) {

  cout << "Adding weight branch to file " << filename << " with weight "  << baseW << endl;
  if (release==0) cout << "Wrong release, there must be an error"         << endl;
  if (release==1) cout << "Offline efficiency computed using 42X samples" << endl;

  TFile *fileOrig = 0;
  TTree *treeOrig = 0;

  fileOrig = TFile::Open(filename);
  if( fileOrig ) {
    fileOrig->cd();
    treeOrig = (TTree*)fileOrig->Get("T1");
  } else {
    cout << "File " << filename << " not existing !" << endl;
    return;
  }

  // used for PU reweighting
  PUWeight* fPUWeight2011A    = new PUWeight("summer11","DY",-1,"2011A",0); 
  PUWeight* fPUWeight2011B    = new PUWeight("summer11","DY",-1,"2011B",1); 
  PUWeight* fPUWeightFull2011 = new PUWeight("summer11","DY",-1,"Full2011",-1); 

  // used for DY NNLO reweighting
  DYWeighter* DYNNLOWeight = new DYWeighter("/afs/cern.ch/user/e/emanuele/public/DYReweighting/fewz_powheg_weights_stepwise_2011_fine7.root");

  // reading root files with electrons and muons efficiencies
  TFile fileSFmuons42A("/cmsrm/pc24_2/emanuele/data/Higgs4.2.X/LeptonSFs/8thNovemberReload_Muon_Efficiencies_Run2011_ALL/OutputScaleFactorMap_MC42X_2011AReweighted.root");
  TFile fileSFmuons42B("/cmsrm/pc24_2/emanuele/data/Higgs4.2.X/LeptonSFs/8thNovemberReload_Muon_Efficiencies_Run2011_ALL/OutputScaleFactorMap_MC42X_2011BReweighted.root");
  TFile fileSFEle42A("/cmsrm/pc24_2/emanuele/data/Higgs4.2.X/LeptonSFs/8thNovemberReload_Electron_Efficiencies_Run2011_ALL/OutputScaleFactorMap_MC42X_2011AReweighted.root");
  TFile fileSFEle42B("/cmsrm/pc24_2/emanuele/data/Higgs4.2.X/LeptonSFs/8thNovemberReload_Electron_Efficiencies_Run2011_ALL/OutputScaleFactorMap_MC42X_2011BReweighted.root");

  TH2F *histoSFmuons42A = (TH2F*)fileSFmuons42A.Get("hScaleFactorMap")->Clone("effSFmuons42A");
  TH2F *histoSFmuons42B = (TH2F*)fileSFmuons42B.Get("hScaleFactorMap")->Clone("effSFmuons42B");
  TH2F *histoSFele42A   = (TH2F*)fileSFEle42A.Get("hScaleFactorMap")->Clone("effSFele42A");
  TH2F *histoSFele42B   = (TH2F*)fileSFEle42B.Get("hScaleFactorMap")->Clone("effSFele42B");

  fileOrig->cd();

  if ( treeOrig ) {
    int nentriesOrig = treeOrig->GetEntries();

    TFile *fileNew = TFile::Open(filename,"recreate");
    TTree *treeNew = new TTree("EVENTS","tree with all events");

    // only events passing the full selection
    TString skimFile(filename);
    skimFile.ReplaceAll("merged","merged_skim");
    TFile *fileNewSkim = TFile::Open(skimFile.Data(),"recreate");
    TTree *treeNewSkim = new TTree("EVENTS","tree with only fully selected events");

    std::vector<TTree*> trees; 
    trees.push_back(treeNew);
    trees.push_back(treeNewSkim);
    
    Int_t           run;
    Int_t           lumi;
    Int_t           event;
    Float_t         puweight;
    Float_t         maxPtEle;
    Float_t         minPtEle;
    Bool_t          finalLeptons;
    Bool_t          finalSelection;
    Float_t         npu[3];
    Bool_t          hlt;
    Int_t           njets;
    Int_t           nSoftMu;
    Float_t         mtr;
    Float_t         mr;
    Float_t         gammamr;
    Int_t           nVtx;
    Int_t           numExtraLep;
    Float_t         pxL1;
    Float_t         pyL1;
    Float_t         pzL1;
    Float_t         pxL2;
    Float_t         pyL2;
    Float_t         pzL2;
    Float_t         KFactor;
    // DY generator level quantities
    Float_t         genmll;
    Float_t         genptll;
    Float_t         genyll;

    treeOrig->SetBranchAddress("run", &run);
    treeOrig->SetBranchAddress("lumi", &lumi);
    treeOrig->SetBranchAddress("event", &event);
    treeOrig->SetBranchAddress("puweight", &puweight);
    treeOrig->SetBranchAddress("maxPtEle", &maxPtEle);
    treeOrig->SetBranchAddress("minPtEle", &minPtEle);
    treeOrig->SetBranchAddress("finalLeptons", &finalLeptons);
    treeOrig->SetBranchAddress("npu", npu);
    treeOrig->SetBranchAddress("hlt", &hlt);
    treeOrig->SetBranchAddress("njets", &njets);
    treeOrig->SetBranchAddress("nSoftMu", &nSoftMu);
    treeOrig->SetBranchAddress("numExtraLep", &numExtraLep);
    treeOrig->SetBranchAddress("mtr", &mtr);
    treeOrig->SetBranchAddress("mr",  &mr);
    treeOrig->SetBranchAddress("gammamr", &gammamr);
    treeOrig->SetBranchAddress("nVtx", &nVtx);
    treeOrig->SetBranchAddress("pxL1", &pxL1);
    treeOrig->SetBranchAddress("pyL1", &pyL1);
    treeOrig->SetBranchAddress("pzL1", &pzL1);
    treeOrig->SetBranchAddress("pxL2", &pxL2);
    treeOrig->SetBranchAddress("pyL2", &pyL2);
    treeOrig->SetBranchAddress("pzL2", &pzL2);
    treeOrig->SetBranchAddress("genmll",  &genmll);
    treeOrig->SetBranchAddress("genptll", &genptll);
    treeOrig->SetBranchAddress("genyll",  &genyll);
    treeOrig->SetBranchAddress("KFactor", &KFactor);

    // convert the booleans into integers (to insert in RooDataset)
    Int_t i_finalLeptons;
    Int_t i_finalSelection;
    Int_t i_hlt;

    // change name to razor variables
    float R, dgammamr;
    
    // variables to be converted in float
    float f_run, f_lumi, f_event, f_hlt, f_njets; 
    float f_nVtx, f_nSoftMu, f_numExtraLep;
    float f_finalstate, f_processId;
      
    // additional: efficiencies 
    Float_t effW = 1.0;   
    Float_t effAW, effBW; effAW = effBW = 1.0;
    Float_t triggW = 1.0;

    // additional: pileup
    float puAW, puBW, puW;
    
    // loop over trees
    for(int i=0; i<(int)trees.size();i++) {
      TTree *theTreeNew = trees[i];

      // the selected final state: mm=0, ee=1, em=2, mue=3
      theTreeNew->Branch("channel", &f_finalstate, "channel/F");

      // one integer containing the process identifier (for MC, 0 for data)
      theTreeNew->Branch("dataset", &f_processId, "dataset/F");

      // Copies branches
      theTreeNew->Branch("run",     &f_run,   "run/F");
      theTreeNew->Branch("lumi",    &f_lumi,  "lumi/F");
      theTreeNew->Branch("event",   &f_event, "event/F");
      theTreeNew->Branch("genmll",  &genmll,  "genmll/F");
      theTreeNew->Branch("genptll", &genptll, "genptll/F");
      theTreeNew->Branch("genyll",  &genyll,  "genyll/F");
      theTreeNew->Branch("puW",     &puW,     "puW/F");
      theTreeNew->Branch("puAW",    &puAW,    "puAW/F");
      theTreeNew->Branch("puBW",    &puBW,    "puBW/F");
      theTreeNew->Branch("effW",    &effW,    "effW/F");
      theTreeNew->Branch("effAW",   &effAW,   "effAW/F");
      theTreeNew->Branch("effBW",   &effBW,   "effBW/F");
      theTreeNew->Branch("kfW",     &KFactor, "kfW/F");
      theTreeNew->Branch("triggW",  &triggW,  "triggW/F");
      theTreeNew->Branch("trigger", &f_hlt,   "trigger/F");
      theTreeNew->Branch("baseW",   &baseW,   "baseW/F");
      theTreeNew->Branch("pt1",     &maxPtEle, "pt1/F");
      theTreeNew->Branch("pt2",     &minPtEle, "pt2/F");
      theTreeNew->Branch("nvtx",    &f_nVtx,   "nvtx/F");
      theTreeNew->Branch("finalLeptons",   &i_finalLeptons,   "finalLeptons/I");
      theTreeNew->Branch("finalSelection", &i_finalSelection, "finalSelection/I");
      theTreeNew->Branch("njet",    &f_njets,       "njet/F");
      theTreeNew->Branch("nSoftMu", &f_nSoftMu,     "nSoftMu/I");
      theTreeNew->Branch("nextra",  &f_numExtraLep, "nextra/F");
      theTreeNew->Branch("pxL1", &pxL1, "pxL1/F");
      theTreeNew->Branch("pyL1", &pyL1, "pyL1/F");
      theTreeNew->Branch("pzL1", &pzL1, "pzL1/F");
      theTreeNew->Branch("pxL2", &pxL2, "pxL2/F");
      theTreeNew->Branch("pyL2", &pyL2, "pyL2/F");
      theTreeNew->Branch("pzL2", &pzL2, "pzL2/F");
      theTreeNew->Branch("mtr",         &mtr,      "mtr/F");  
      theTreeNew->Branch("mr",          &mr,       "mr/F");
      theTreeNew->Branch("gammaMRStar", &dgammamr, "gammaMRStar/F");
      theTreeNew->Branch("R",           &R,        "R/F");
    }

    for(int i=0; i<nentriesOrig; i++) {
      if (i%10000 == 0) std::cout << ">>> Weighting event # " << i << " / " << nentriesOrig << " entries" << std::endl; 
      treeOrig->GetEntry(i);
      
      R = mtr/mr;
      dgammamr = 2*gammamr;
      
      i_finalLeptons = (finalLeptons) ? 1 : 0;
      i_hlt = (hlt) ? 1 : 0;

      if (finalLeptons) {

	// leptons
	TVector3 TV_L1( pxL1, pyL1, pzL1 );  
        TVector3 TV_L2( pxL2, pyL2, pzL2 );
	float l1eta = TV_L1.Eta();
        float l2eta = TV_L2.Eta();
        float l1pt  = TV_L1.Pt();
        float l2pt  = TV_L2.Pt();

	// PU weights
	puAW = fPUWeight2011A->GetWeight(npu[1]); 
	puBW = fPUWeight2011B->GetWeight(npu[1]); 
        puW  = fPUWeightFull2011->GetWeight(npu[1]);

	//  offline efficiency scale factors
	Float_t eff1=1.; 
	Float_t eff2=1.;
        Float_t effA1, effA2, effB1, effB2;
        effA1 = effA2 = effB1 = effB2 = 1.;
	if (processId>0) { // MC => apply scale factors
	  if (finalstate==0) {   // mm
	    if (release==1) { // can be only this...
	      effA1 = getOfflineEff(l1pt, l1eta, histoSFmuons42A);    
	      effA2 = getOfflineEff(l2pt, l2eta, histoSFmuons42A);    
	      effB1 = getOfflineEff(l1pt, l1eta, histoSFmuons42B);    
	      effB2 = getOfflineEff(l2pt, l2eta, histoSFmuons42B);    
              eff1 = (effA1 * lumiA + effB1 * lumiB) / (lumiA+lumiB);
              eff2 = (effA2 * lumiA + effB2 * lumiB) / (lumiA+lumiB);
	    }
	  }
	  else if (finalstate==1) { // ee
	    if (release==1) {
	      effA1 = getOfflineEff(l1pt, l1eta, histoSFele42A);
	      effA2 = getOfflineEff(l2pt, l2eta, histoSFele42A);
	      effB1 = getOfflineEff(l1pt, l1eta, histoSFele42B);
	      effB2 = getOfflineEff(l2pt, l2eta, histoSFele42B);
              eff1 = (effA1 * lumiA + effB1 * lumiB) / (lumiA+lumiB);
              eff2 = (effA2 * lumiA + effB2 * lumiB) / (lumiA+lumiB);
	    }
	  } else if (finalstate==2) {  // em
	    if (release==1) {
	      effA1 = getOfflineEff(l1pt, l1eta, histoSFele42A);
	      effA2 = getOfflineEff(l2pt, l2eta, histoSFmuons42A);
	      effB1 = getOfflineEff(l1pt, l1eta, histoSFele42B);
	      effB2 = getOfflineEff(l2pt, l2eta, histoSFmuons42B);
              eff1 = (effA1 * lumiA + effB1 * lumiB) / (lumiA+lumiB);
              eff2 = (effA2 * lumiA + effB2 * lumiB) / (lumiA+lumiB);              
	    }
	  } else if (finalstate==3) {  // me
	    if (release==1) {
	      effA1 = getOfflineEff(l1pt, l1eta, histoSFmuons42A);
	      effA2 = getOfflineEff(l2pt, l2eta, histoSFele42A);
	      effB1 = getOfflineEff(l1pt, l1eta, histoSFmuons42B);
	      effB2 = getOfflineEff(l2pt, l2eta, histoSFele42B);
              eff1 = (effA1 * lumiA + effB1 * lumiB) / (lumiA+lumiB);
              eff2 = (effA2 * lumiA + effB2 * lumiB) / (lumiA+lumiB);              
	    }
	  } 
	  effW  = eff1*eff2;
	  effAW = effA1*effA2;
	  effBW = effB1*effB2;
	} else { // data
	  effW  = 1.;
	  effAW = 1.;
	  effBW = 1.;
	}
	
        // replace 1 with the POWHEG -> FEWZ NNLO x-sec
        if(processId==30 || processId==31 || processId==33 || processId==34) {
          KFactor = DYNNLOWeight->GetWeight(genmll,genptll,genyll);
        }
      }
      
      // final selection, hardcoded here
      i_finalSelection = (finalLeptons && minPtEle>30 && maxPtEle>30 && nSoftMu==0 && numExtraLep==0) ? 1 : 0;

      // change the format of the integers -> float
      f_run   = (float)run;
      f_lumi  = (float)lumi;
      f_event = (float)event;
      f_hlt   = (float)i_hlt;
      f_nVtx  = (float)nVtx; 
      f_njets   = (float)njets;
      f_nSoftMu = (float)nSoftMu;
      f_numExtraLep = (float)numExtraLep;
      f_finalstate  = (float)finalstate;
      f_processId   = (float)processId;

      if(finalLeptons) {
        if(processId < 100 || processId >= 1000 ) { // MC
          treeNew->Fill();
          if(i_finalSelection) treeNewSkim->Fill();    
        } else {    // data: apply the trigger 
          if(hlt) {
            treeNew->Fill();
	    if(i_finalSelection) treeNewSkim->Fill();  
          }
        }
      }
    }
    
    fileNew->cd();
    treeNew->Write();
    fileNew->Close();
    
    fileNewSkim->cd();
    treeNewSkim->Write();
    fileNewSkim->Close();
    
    fileOrig->cd();
    fileOrig->Close();

  } else {
    cout << "Tree T1 not present in the file " << filename << endl;
    return;
  }
}

float getOfflineEff(float pT, float eta, TH2F *myH) {

  float theEff=-1.;
  
  //  int numberOfBins = myH->GetNbinsX()*myH->GetNbinsY();
  int   xBins = myH->GetXaxis()->GetNbins();
  float xMin  = myH->GetXaxis()->GetBinLowEdge(1);
  float xMax  = myH->GetXaxis()->GetBinUpEdge(xBins);
  int   yBins = myH->GetYaxis()->GetNbins();
  float yMin  = myH->GetYaxis()->GetBinLowEdge(1);
  float yMax  = myH->GetYaxis()->GetBinUpEdge(yBins);
  int theBin = myH->FindBin(pT, fabs(eta));
  if (pT>xMin && pT<xMax && fabs(eta)>yMin && fabs(eta)<yMax) {
    theEff = myH->GetBinContent(theBin);
  } else {
    theEff = 1.;
  }

  return theEff;
}


