#ifndef YIELDMAKER_H
#define YIELDMAKER_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooDataSet.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include "HWWKinematics.hh"

enum channels { of0j, of1j, sf0j, sf1j };

float getFitChannel(float channel, float njet) {
  if((int)channel>=2 && (int)njet==0) return of0j;
  if((int)channel>=2 && (int)njet==1) return of1j;
  if((int)channel<2 && (int)njet==0) return sf0j;
  if((int)channel<2 && (int)njet==1) return sf1j;
  return -1;
}

std::string getStringFitChannel(float fitchannel) {
  if(fitchannel==of0j) return std::string("of0j");
  if(fitchannel==of1j) return std::string("of1j");
  if(fitchannel==sf0j) return std::string("sf0j");
  if(fitchannel==sf1j) return std::string("sf1j");
  return std::string("wrongchannel");
}

class YieldMaker {

 protected:
  RooRealVar rrchannel  ;	
  RooRealVar rrmr     ;
  RooRealVar rrD     ;
  RooRealVar rrmt    ;
  RooRealVar rrweight   ;
  RooRealVar rrnoxsecweight ;
  RooRealVar rrptll  ;
  RooArgSet argset      ;
  RooDataSet dataset    ;
  
 public :        

 YieldMaker():
  rrchannel  (RooRealVar("channel",  "channel",   0., 10.)), 
    rrmr     (RooRealVar("mr",       "mr",      0., 10000000.)),
    rrD      (RooRealVar("dphillr",  "dphillr",      0., TMath::Pi())),
    rrmt     (RooRealVar("mt",       "mt",           0., 10000000.)),
    rrweight (RooRealVar("weight",   "weight",    -100000., 10000000.)),
    rrnoxsecweight (RooRealVar("noxsecweight",  "noxsecweight", -100000., 10000000.)),
    rrptll(RooRealVar("ptll",  "ptll",   0., 10000000.)),
    argset(RooArgSet(rrchannel, rrmr, rrD, rrmt, rrweight, rrnoxsecweight, rrptll, "argset")),
    dataset(RooDataSet("dataset", "dataset", argset))
      {}

   float getYield(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, float ptllmin, float ptllmax) {
    
    float yield = 0.0;
    
    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float D      = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float weight = dataset.get(i)->getRealValue("weight");
      float ch     = dataset.get(i)->getRealValue("channel");
      float ptll   = dataset.get(i)->getRealValue("ptll");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && D>dphimin && D<dphimax && mt>mtmin && mt<mtmax && ptll>=ptllmin && ptll<=ptllmax) yield += weight;
    }

    return yield;

  }

   float getCount(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, float ptllmin, float ptllmax) {
    
    float yield = 0.0;
    
    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float D      = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float ch     = dataset.get(i)->getRealValue("channel");
      float ptll   = dataset.get(i)->getRealValue("ptll");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && D>dphimin && D<dphimax && mt>mtmin && mt<mtmax && ptll>=ptllmin && ptll<=ptllmax) yield += 1.0;
    }

    return yield;

  }

   void get1DHist(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, TH1* hist, bool usexsecweight=true) {
    
    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float D      = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float weight = dataset.get(i)->getRealValue("weight");
      float noxsecweight = dataset.get(i)->getRealValue("noxsecweight");
      float ch     = dataset.get(i)->getRealValue("channel");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && D>dphimin && D<dphimax && mt>mtmin && mt<mtmax) hist->Fill(mr,(usexsecweight) ? weight : noxsecweight);
    }

  }


   void get2DHist(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, TH2* hist, bool usexsecweight=true) {
    
    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float D      = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float weight = dataset.get(i)->getRealValue("weight");
      float noxsecweight = dataset.get(i)->getRealValue("noxsecweight");
      float ch     = dataset.get(i)->getRealValue("channel");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && D>dphimin && D<dphimax && mt>mtmin && mt<mtmax) hist->Fill(mr,D,(usexsecweight) ? weight : noxsecweight);
      }

  }


   RooDataSet getFitDataSet(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, float ptllmin, float ptllmax) {
    RooRealVar mr("mr",   "mr",            100, 50, 1000,      "GeV/c^{2}");
    RooRealVar D("dphillr",   "dphillr",   100, 0,  TMath::Pi());
    RooRealVar w("weight", "weight", 0.,  -10.,  10000.);
    RooArgSet  aset(mr, D, w, "aset");
    RooDataSet dset("dataset","", aset,RooFit::WeightVar("weight"));
    

    for (int i = 0; i < dataset.numEntries(); i++) {
      float m      = dataset.get(i)->getRealValue("mr");
      float dphi   = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float weight = dataset.get(i)->getRealValue("weight");
      float ch     = dataset.get(i)->getRealValue("channel");
      float ptll   = dataset.get(i)->getRealValue("ptll");

      if (channel == (int)ch && m>mrmin && m<mrmax && dphi>dphimin && dphi<dphimax && mt>mtmin && mt<mtmax && ptll>=ptllmin && ptll<=ptllmax) {
	aset.setRealValue("mr", m);
	aset.setRealValue("D",  dphi);
        aset.setRealValue("mt", mt);
	aset.setRealValue("weight", weight);
	dset.add(aset);
      }
    }

    return dset;

  }

   void getDataSet1D(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, float ptllmin, float ptllmax, RooDataSet& dset, RooRealVar& m, RooRealVar& w) {

    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float dphi   = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float ch     = dataset.get(i)->getRealValue("channel");
      float ptll   = dataset.get(i)->getRealValue("ptll");
      float weight = dataset.get(i)->getRealValue("weight");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && dphi>dphimin && dphi<dphimax && mt>mtmin && mt<mtmax && ptll>=ptllmin && ptll<=ptllmax) {
	m.setVal(mr);
        w.setVal(weight);
        RooArgSet aset(m, "argset_obs");
	dset.add(aset,weight);
      }
    }
  }

   void getDataSet2D(int channel, float mrmin, float mrmax, float dphimin, float dphimax, float mtmin, float mtmax, float ptllmin, float ptllmax, RooDataSet& dset, RooRealVar& m, RooRealVar& D, RooRealVar& w) {

    for (int i = 0; i < dataset.numEntries(); i++) {
      float mr     = dataset.get(i)->getRealValue("mr");
      float dphi   = dataset.get(i)->getRealValue("dphillr");
      float mt     = dataset.get(i)->getRealValue("mt");
      float ch     = dataset.get(i)->getRealValue("channel");
      float ptll   = dataset.get(i)->getRealValue("ptll");
      float weight = dataset.get(i)->getRealValue("weight");

      if (channel == (int)ch && mr>mrmin && mr<mrmax && dphi>dphimin && dphi<dphimax && mt>mtmin && mt<mtmax && ptll>=ptllmin && ptll<=ptllmax) {
	m.setVal(mr);
	D.setVal(dphi);
        w.setVal(weight);
	RooArgSet aset(m, D, "argset_obs");
	dset.add(aset,weight);
      }
    }
  }


  void fill(std::string filepath) {

    cout << "\tYieldMaker. Filling dataset from: " << filepath << endl;

    TFile *file = TFile::Open(filepath.c_str());
    TTree *tree = (TTree*)file->Get("latino");

    float eta1       = 0.0;
    float eta2       = 0.0;
    float phi1       = 0.0;
    float phi2       = 0.0;
    float pt1        = 0.0;
    float pt2        = 0.0;
    float ch1        = 0.0;
    float ch2        = 0.0;
    float ptll       = 0.0;
    float pfmet      = 0.0;
    float pfmetphi   = 0.0;
    float mth        = 0.0;
    float dphill     = 0.0;
    float baseweight = 0.0;
    float effweight  = 0.0;
    float puweight   = 0.0;
    float kfweight   = 0.0;
    float ch         = 0.0;
    float proc       = 0.0;
    float njet       = 0.0;
    UInt_t event     = 0.0;

    tree->SetBranchAddress("eta1",    &eta1);
    tree->SetBranchAddress("eta2",    &eta2);
    tree->SetBranchAddress("phi1",    &phi1);
    tree->SetBranchAddress("phi2",    &phi2);
    tree->SetBranchAddress("pt1",     &pt1);
    tree->SetBranchAddress("pt2",     &pt2);
    tree->SetBranchAddress("ch1",     &ch1);
    tree->SetBranchAddress("ch2",     &ch2);
    tree->SetBranchAddress("ptll",    &ptll);
    tree->SetBranchAddress("pfmet",   &pfmet);
    tree->SetBranchAddress("pfmetphi",&pfmetphi);
    tree->SetBranchAddress("mth",     &mth);
    tree->SetBranchAddress("dphill",  &dphill);
    tree->SetBranchAddress("baseW",   &baseweight);
    tree->SetBranchAddress("puW",     &puweight);
    tree->SetBranchAddress("effW",    &effweight);
    tree->SetBranchAddress("kfW",     &kfweight);
    tree->SetBranchAddress("channel", &ch);
    tree->SetBranchAddress("dataset", &proc);
    tree->SetBranchAddress("njet",    &njet);
    tree->SetBranchAddress("event",   &event);

    cout << "Number of entries to process = " << tree->GetEntries() << endl;
    
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);

      if(i%10000 == 0) cout << ">>> Processing event # " << i << endl;

      if(ch1*ch2>0) continue;

      float mr         = 0.0;
      float dphillr    = 0.0;

      int l1id,l2id;
      if(ch==0) { l1id=13; l2id=13; }
      if(ch==1) { l1id=11; l2id=11; }
      if(ch==2) { l1id=11; l2id=13; }
      if(ch==3) { l1id=13; l2id=11; }

      float elmass=0.51E-03;
      float mumass=105.7E-03;
      float l1m = (l1id==11) ? elmass : mumass;
      float l2m = (l2id==11) ? elmass : mumass;
      TLorentzVector l1,l2;
      TVector3 met;
      l1.SetPtEtaPhiM(pt1,eta1,phi1,l1m);
      l2.SetPtEtaPhiM(pt2,eta2,phi2,l2m);
      met.SetPtEtaPhi(pfmet,0.0,pfmetphi);

      HWWKinematics kine(l1,l2,met);
      //float mrold = 2*kine.CalcMR();
      mr = 2*kine.CalcMRNEW();
      dphillr = fabs(kine.CalcDeltaPhiRFRAME());

      //      cout << event << "," << mr << "," << dphillr << endl;

      argset.setRealValue("mr",      mr);
      argset.setRealValue("dphillr", dphillr);
      argset.setRealValue("dphill",  dphill);
      argset.setRealValue("mt", mth);
      argset.setRealValue("ptll", ptll);
      float channel = getFitChannel(ch,njet);
      argset.setRealValue("channel", channel);
      argset.setRealValue("dataset", proc);
      float weight = baseweight*effweight*puweight*kfweight;
      argset.setRealValue("weight", weight);
      float noxsecweight = effweight*puweight*kfweight;
      argset.setRealValue("noxsecweight", noxsecweight);
      if(channel<0) continue;
      dataset.add(argset);
    }

  }

};



class WJetsYieldMaker : public YieldMaker {

 public :        

  WJetsYieldMaker(int sigmaup=0):YieldMaker() {
    _systematic=sigmaup;
  }

  int _systematic;

  void fill(std::string filepath) {

    cout << "\tWJetsYieldMaker. Filling dataset from: " << filepath << endl;

    TFile *file = TFile::Open(filepath.c_str());
    TTree *tree = (TTree*)file->Get("latino");

    float eta1       = 0.0;
    float eta2       = 0.0;
    float phi1       = 0.0;
    float phi2       = 0.0;
    float pt1        = 0.0;
    float pt2        = 0.0;
    float ch1        = 0.0;
    float ch2        = 0.0;
    float ptll       = 0.0;
    float pfmet      = 0.0;
    float pfmetphi   = 0.0;
    float mth        = 0.0;
    float dphill     = 0.0;
    float fakeweight = 0.0;
    float ch         = 0.0;
    float proc       = 0.0;
    float njet       = 0.0;

    tree->SetBranchAddress("eta1",    &eta1);
    tree->SetBranchAddress("eta2",    &eta2);
    tree->SetBranchAddress("phi1",    &phi1);
    tree->SetBranchAddress("phi2",    &phi2);
    tree->SetBranchAddress("pt1",     &pt1);
    tree->SetBranchAddress("pt2",     &pt2);
    tree->SetBranchAddress("ch1",     &ch1);
    tree->SetBranchAddress("ch2",     &ch2);
    tree->SetBranchAddress("ptll",    &ptll);
    tree->SetBranchAddress("pfmet",   &pfmet);
    tree->SetBranchAddress("pfmetphi",&pfmetphi);
    tree->SetBranchAddress("mth",     &mth);
    tree->SetBranchAddress("dphill",  &dphill);
    tree->SetBranchAddress("channel", &ch);
    tree->SetBranchAddress("dataset", &proc);
    tree->SetBranchAddress("njet",    &njet);
    if(_systematic==1)       tree->SetBranchAddress("fakeWUp",   &fakeweight);
    else if(_systematic==-1) tree->SetBranchAddress("fakeWDown", &fakeweight);
    else tree->SetBranchAddress("fakeW",   &fakeweight);

    cout << "Number of entries to process = " << tree->GetEntries() << endl;

    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);

      if(i%10000 == 0) cout << ">>> Processing event # " << i << endl;

      if(ch1*ch2>0) continue;

      float mr         = 0.0;
      float dphillr    = 0.0;

      int l1id,l2id;
      if(ch==0) { l1id=13; l2id=13; }
      if(ch==1) { l1id=11; l2id=11; }
      if(ch==2) { l1id=11; l2id=13; }
      if(ch==3) { l1id=13; l2id=11; }

      float elmass=0.51E-03;
      float mumass=105.7E-03;
      float l1m = (l1id==11) ? elmass : mumass;
      float l2m = (l2id==11) ? elmass : mumass;
      TLorentzVector l1,l2;
      TVector3 met;
      l1.SetPtEtaPhiM(pt1,eta1,phi1,l1m);
      l2.SetPtEtaPhiM(pt2,eta2,phi2,l2m);
      met.SetPtEtaPhi(pfmet,0.0,pfmetphi);

      HWWKinematics kine(l1,l2,met);
      mr = 2*kine.CalcMRNEW();
      dphillr = fabs(kine.CalcDeltaPhiRFRAME());

      argset.setRealValue("mr",      mr);
      argset.setRealValue("dphillr", dphillr);
      argset.setRealValue("dphill",  dphill);
      argset.setRealValue("mt", mth);
      argset.setRealValue("ptll", ptll);
      float channel = getFitChannel(ch,njet);
      argset.setRealValue("channel", channel);
      argset.setRealValue("dataset", proc);
      float weight = fakeweight;
      argset.setRealValue("weight", weight);
      argset.setRealValue("noxsecweight", weight);
      if(channel<0) continue;
      dataset.add(argset);
    }

  }


};


class DataYieldMaker : public YieldMaker {

 public :        

  DataYieldMaker():YieldMaker(){}

  void fill(std::string filepath) {

    cout << "\tDataYieldMaker. Filling dataset from: " << filepath << endl;

    TFile *file = TFile::Open(filepath.c_str());
    TTree *tree = (TTree*)file->Get("latino");

    float eta1       = 0.0;
    float eta2       = 0.0;
    float phi1       = 0.0;
    float phi2       = 0.0;
    float pt1        = 0.0;
    float pt2        = 0.0;
    float ch1        = 0.0;
    float ch2        = 0.0;
    float ptll       = 0.0;
    float pfmet      = 0.0;
    float pfmetphi   = 0.0;
    float mth        = 0.0;
    float dphill     = 0.0;
    float ch         = 0.0;
    float proc       = 0.0;
    float njet       = 0.0;

    tree->SetBranchAddress("eta1",    &eta1);
    tree->SetBranchAddress("eta2",    &eta2);
    tree->SetBranchAddress("phi1",    &phi1);
    tree->SetBranchAddress("phi2",    &phi2);
    tree->SetBranchAddress("pt1",     &pt1);
    tree->SetBranchAddress("pt2",     &pt2);
    tree->SetBranchAddress("ch1",     &ch1);
    tree->SetBranchAddress("ch2",     &ch2);
    tree->SetBranchAddress("ptll",    &ptll);
    tree->SetBranchAddress("pfmet",   &pfmet);
    tree->SetBranchAddress("pfmetphi",&pfmetphi);
    tree->SetBranchAddress("mth",     &mth);
    tree->SetBranchAddress("dphill",  &dphill);
    tree->SetBranchAddress("channel", &ch);
    tree->SetBranchAddress("dataset", &proc);
    tree->SetBranchAddress("njet",    &njet);

    cout << "Number of entries to process = " << tree->GetEntries() << endl;

    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);

      if(i%10000 == 0) cout << ">>> Processing event # " << i << endl;

      if(ch1*ch2>0) continue;

      float mr         = 0.0;
      float dphillr    = 0.0;

      int l1id,l2id;
      if(ch==0) { l1id=13; l2id=13; }
      if(ch==1) { l1id=11; l2id=11; }
      if(ch==2) { l1id=11; l2id=13; }
      if(ch==3) { l1id=13; l2id=11; }

      float elmass=0.51E-03;
      float mumass=105.7E-03;
      float l1m = (l1id==11) ? elmass : mumass;
      float l2m = (l2id==11) ? elmass : mumass;
      TLorentzVector l1,l2;
      TVector3 met;
      l1.SetPtEtaPhiM(pt1,eta1,phi1,l1m);
      l2.SetPtEtaPhiM(pt2,eta2,phi2,l2m);
      met.SetPtEtaPhi(pfmet,0.0,pfmetphi);

      HWWKinematics kine(l1,l2,met);
      mr = 2*kine.CalcMRNEW();
      dphillr = fabs(kine.CalcDeltaPhiRFRAME());

      argset.setRealValue("mr",      mr);
      argset.setRealValue("dphillr", dphillr);
      argset.setRealValue("dphill",  dphill);
      argset.setRealValue("mt", mth);
      argset.setRealValue("ptll", ptll);
      float channel = getFitChannel(ch,njet);
      argset.setRealValue("channel", channel);
      argset.setRealValue("dataset", proc);
      argset.setRealValue("weight",  1.0);
      argset.setRealValue("noxsecweight", 1.0);
      if(channel<0) continue;
      dataset.add(argset);
    }

  }


};

#endif

