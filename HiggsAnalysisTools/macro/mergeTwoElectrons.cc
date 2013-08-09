#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <iostream>

using namespace std;

void merge(const char* filename) {

  TFile *fileOrig = 0;
  TTree *treeOrig = 0;

  char fileN[500];
  sprintf(fileN,"%s.root",filename);  
  fileOrig = TFile::Open(fileN);
  if( fileOrig ) {
    fileOrig->cd();
    treeOrig = (TTree*)fileOrig->Get("T1");
  } else {
    cout << "File " << fileN << " not existing !" << endl;
    return;
  }
  
  if ( treeOrig ) {
    int nentriesOrig = treeOrig->GetEntries();
    
    Float_t         pt1;
    Float_t         eta1;
    Float_t         deta1;
    Float_t         dphi1;
    Float_t         hoe1;
    Float_t         see1;
    Int_t           matched1;
    Float_t         pt2;
    Float_t         eta2;
    Float_t         deta2;
    Float_t         dphi2;
    Float_t         hoe2;
    Float_t         see2;
    Char_t          finalLeptons;
    Int_t           matched2;
    Float_t         weight;

    treeOrig->SetBranchAddress("pt1",      &pt1);
    treeOrig->SetBranchAddress("eta1",     &eta1);
    treeOrig->SetBranchAddress("deta1",    &deta1);
    treeOrig->SetBranchAddress("dphi1",    &dphi1);
    treeOrig->SetBranchAddress("hoe1",     &hoe1);
    treeOrig->SetBranchAddress("see1",     &see1);
    treeOrig->SetBranchAddress("matched1", &matched1);
    treeOrig->SetBranchAddress("pt2",      &pt2);
    treeOrig->SetBranchAddress("eta2",     &eta2);
    treeOrig->SetBranchAddress("deta2",    &deta2);
    treeOrig->SetBranchAddress("dphi2",    &dphi2);
    treeOrig->SetBranchAddress("hoe2",     &hoe2);
    treeOrig->SetBranchAddress("see2",     &see2);
    treeOrig->SetBranchAddress("matched2", &matched2);
    treeOrig->SetBranchAddress("finalLeptons", &finalLeptons);
    treeOrig->SetBranchAddress("weight",   &weight);

    // copy branches
    char complete1[500];
    sprintf(complete1,"%s_signal1.root",filename);
    TFile *fileNew1 = TFile::Open(complete1,"recreate");  
    TTree *treeNew1 = new TTree("T1","tree with only selected events for signal");
    treeNew1->Branch("pt",   &pt1,   "pt/F");
    treeNew1->Branch("eta",  &eta1,  "eta/F");
    treeNew1->Branch("deta", &deta1, "deta/F");
    treeNew1->Branch("dphi", &dphi1, "dphi/F");
    treeNew1->Branch("hoe",  &hoe1,  "hoe/F");
    treeNew1->Branch("see",  &see1,  "see/F");
    treeNew1->Branch("matched", &matched1, "matched/I");
    treeNew1->Branch("weight",  &weight,   "weight/F");
    treeNew1->Branch("finalLeptons", &finalLeptons, "finalLeptons/B");

    char complete2[500];
    sprintf(complete2,"%s_signal2.root",filename);
    TFile *fileNew2 = TFile::Open(complete2,"recreate");  
    TTree *treeNew2 = new TTree("T1","tree with only selected events for signal");
    treeNew2->Branch("pt",   &pt2,   "pt/F");
    treeNew2->Branch("eta",  &eta2,  "eta/F");
    treeNew2->Branch("deta", &deta2, "deta/F");
    treeNew2->Branch("dphi", &dphi2, "dphi/F");
    treeNew2->Branch("hoe",  &hoe2,  "hoe/F");
    treeNew2->Branch("see",  &see2,  "see/F");
    treeNew2->Branch("matched", &matched2, "matched/I");    
    treeNew2->Branch("weight",  &weight,   "weight/F");
    treeNew2->Branch("finalLeptons", &finalLeptons, "finalLeptons/B");

    char complete3[500];
    sprintf(complete3,"%s_background1.root",filename);
    TFile *fileNew3 = TFile::Open(complete3,"recreate");  
    TTree *treeNew3 = new TTree("T1","tree with only selected events for background");
    treeNew3->Branch("pt",   &pt1,   "pt/F");
    treeNew3->Branch("eta",  &eta1,  "eta/F");
    treeNew3->Branch("deta", &deta1, "deta/F");
    treeNew3->Branch("dphi", &dphi1, "dphi/F");
    treeNew3->Branch("hoe",  &hoe1,  "hoe/F");
    treeNew3->Branch("see",  &see1,  "see/F");
    treeNew3->Branch("matched", &matched1, "matched/I");
    treeNew3->Branch("weight",  &weight,   "weight/F");
    treeNew3->Branch("finalLeptons", &finalLeptons, "finalLeptons/B");

    char complete4[500];
    sprintf(complete4,"%s_background2.root",filename);
    TFile *fileNew4 = TFile::Open(complete4,"recreate");  
    TTree *treeNew4 = new TTree("T1","tree with only selected events for background");
    treeNew4->Branch("pt",   &pt2,   "pt/F");
    treeNew4->Branch("eta",  &eta2,  "eta/F");
    treeNew4->Branch("deta", &deta2, "deta/F");
    treeNew4->Branch("dphi", &dphi2, "dphi/F");
    treeNew4->Branch("hoe",  &hoe2,  "hoe/F");
    treeNew4->Branch("see",  &see2,  "see/F");
    treeNew4->Branch("matched", &matched2, "matched/I");
    treeNew4->Branch("weight",  &weight,   "weight/F");
    treeNew4->Branch("finalLeptons", &finalLeptons, "finalLeptons/B");

    // select only matched electrons for signal
    for(int i=0; i<nentriesOrig; i++) {
      if (i%1000 == 0) std::cout << "Signal1, event " << i << std::endl;
      treeOrig->GetEntry(i);
      if (matched1==0) continue;
      treeNew1->Fill();
    }
    for(int i=0; i<nentriesOrig; i++) {
      if (i%1000 == 0) std::cout << "Signal2, event " << i << std::endl;
      treeOrig->GetEntry(i);
      if (matched2==0) continue;
      treeNew2->Fill();
    }

    // select only not matched electrons for background
    for(int i=0; i<nentriesOrig; i++) {
      if (i%1000 == 0) std::cout << "Background1, event " << i << std::endl;
      treeOrig->GetEntry(i);
      if (matched1==1) continue;
      treeNew3->Fill();
    }
    for(int i=0; i<nentriesOrig; i++) {
      if (i%1000 == 0) std::cout << "Background2, event " << i << std::endl;
      treeOrig->GetEntry(i);
      if (matched2==1) continue;
      treeNew4->Fill();
    }

    fileNew1->cd();
    treeNew1->Write();
    fileNew1->Close();

    fileNew2->cd();
    treeNew2->Write();
    fileNew2->Close();

    fileNew3->cd();
    treeNew3->Write();
    fileNew3->Close();

    fileNew4->cd();
    treeNew4->Write();
    fileNew4->Close();
    
    fileOrig->cd();
    fileOrig->Close();
    
  } else {
    cout << "Tree T1 not present in the file " << filename << endl;
    return;
  }
}
