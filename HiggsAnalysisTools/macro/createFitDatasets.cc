#include <TChain.h>
#include <TMath.h>
#include <TFile.h>
#include <TSystem.h>

#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooCategory.h>

#include <iostream>

using namespace std;

void createSingleDataset(const char *treefile, const char *roofitfile);

void createAllData() {

  cout << "CREATING ROODATASET ON DATA FOR EE CHANNEL..." << endl;
  createSingleDataset("results_data/merged/dataset_eg_ee.root","results_data/datasets/dataset_eg_ee.root");

  cout << "CREATING ROODATASET ON DATA FOR MM CHANNEL..." << endl;
  createSingleDataset("results_data/merged/dataset_mu_mm.root","results_data/datasets/dataset_mu_mm.root");

  cout << "CREATING ROODATASET ON DATA FOR EM CHANNEL..." << endl;
  createSingleDataset("results_data/merged/dataset_muAndEg_em.root","results_data/datasets/dataset_muAndEg_em.root");

  cout << "CREATING ROODATASET ON DATA FOR ALL CHANNELS TOGETHER..." << endl;
  createSingleDataset("results_data/merged/dataset_muAndEg_ll.root","results_data/datasets/dataset_muAndEg_ll.root");

}

void createAll() {

  cout << "CREATING ROODATSETS FOR EE CHANNEL..." << endl;
  createSingleDataset("results/datasets_trees/H160_ee.root","results/datasets/H160_ee.root");
  createSingleDataset("results/datasets_trees/others_ee.root","results/datasets/others_ee.root");
  createSingleDataset("results/datasets_trees/top_ee.root","results/datasets/top_ee.root");
  createSingleDataset("results/datasets_trees/WW_ee.root","results/datasets/WW_ee.root");

  cout << "CREATING ROODATSETS FOR MM CHANNEL..." << endl;
  createSingleDataset("results/datasets_trees/H160_mm.root","results/datasets/H160_mm.root");
  createSingleDataset("results/datasets_trees/others_mm.root","results/datasets/others_mm.root");
  createSingleDataset("results/datasets_trees/top_mm.root","results/datasets/top_mm.root");
  createSingleDataset("results/datasets_trees/WW_mm.root","results/datasets/WW_mm.root");

  cout << "CREATING ROODATSETS FOR EM CHANNEL..." << endl;
  createSingleDataset("results/datasets_trees/H160_em.root","results/datasets/H160_em.root");
  createSingleDataset("results/datasets_trees/others_em.root","results/datasets/others_em.root");
  createSingleDataset("results/datasets_trees/top_em.root","results/datasets/top_em.root");
  createSingleDataset("results/datasets_trees/WW_em.root","results/datasets/WW_em.root");

}

void createSingleDataset(const char *treefile, const char *roofitfile) {

  cout << "roofitting file " << treefile << " in " << roofitfile << endl;

  RooRealVar *jetcat = new RooRealVar("jetcat","jetcat",0,10); // cut the -2 ( >1 jet )
  RooRealVar *projmet  = new RooRealVar("projMet","proj. E_{T}^{miss}",0,250,"GeV");
  RooRealVar *deltaPhi = new RooRealVar("deltaPhi","#Delta#phi",0,180,"#deg");
  RooRealVar *exCosDphi = new RooRealVar("expCosDphi","e^{cos(#Delta#phi)}",exp(-1.),exp(1.));
  RooRealVar *maxPtEle = new RooRealVar("maxPtEle","p_{T}^{max}",0,150,"GeV");
  RooRealVar *eleInvMass = new RooRealVar("eleInvMass","m(l^{+}l^{-})",0,90,"GeV"); // <---- put the cut in the sel! (otherwise maxPtEle has two bums for </> Z veto cut)
  RooRealVar *bTagImpPar = new RooRealVar("bTagImpPar","b-tag",-1e+30,1e+30);
  RooRealVar *weight = new RooRealVar("weight","weight",0,1e+30);
  RooRealVar *consecevent = new RooRealVar("consecevent","progressive event number",0,1e+30);
  RooRealVar *finalSelection = new RooRealVar("finalSelection","finalSelection",1,1); // <--- only the final selected events!

  // run variables
  RooRealVar *run = new RooRealVar("run","run number",-1e+32,1e+32);
  RooRealVar *lumi = new RooRealVar("lumi","lumi section number",-1e+32,1e+32);
  RooRealVar *event = new RooRealVar("event","event number",-1e+32,1e+32);

  // finalstate: 0=ee/1=mm/2=em
  RooRealVar *finalstate = new RooRealVar("finalstate","finalstate",-1,3);

  RooArgSet setHiggs(*jetcat,*projmet,*deltaPhi,*maxPtEle,*eleInvMass,*bTagImpPar,*weight,*consecevent);
  RooArgSet setHiggs1(*finalSelection,*exCosDphi,*run,*lumi,*event,*finalstate);
  setHiggs.add(setHiggs1);

  TFile *file = TFile::Open(treefile);
  TTree *tree = (TTree*)file->Get("T1");

  RooDataSet *data = new RooDataSet("T1","dataset",tree,setHiggs);

  TFile *roofitFile = TFile::Open(roofitfile,"recreate");
  data->Write();
  roofitFile->Close();

}
