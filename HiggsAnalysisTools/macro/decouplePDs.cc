#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <iostream>

using namespace std;

class Event {
public:
  Event (int theRun, int theLumi, int theEvent);
  bool operator==(Event obj);
protected:
  int run, lumi, event;
};

Event::Event(int theRun, int theLumi, int theEvent) {
  run=theRun;
  lumi=theLumi;
  event=theEvent;
}

bool Event::operator==(Event obj) {
  if(run==obj.run && 
     lumi==obj.lumi &&
     event==obj.event) return true;
  else return false;
}

void decouple(const char* filenameMU, const char* filenameEG,  const char* filename) {
  
  TFile *fileMU = 0;
  TTree *treeMU = 0;

  fileMU = TFile::Open(filenameMU);
  if( fileMU ) {
    fileMU->cd();
    treeMU = (TTree*)fileMU->Get("T1");
  } else {
    cout << "File " << filenameMU << " not existing !" << endl;
    return;
  }

  TFile *fileEG = 0;
  TTree *treeEG = 0;

  fileEG = TFile::Open(filenameEG);
  if( fileEG ) {
    fileEG->cd();
    treeEG = (TTree*)fileEG->Get("T1");
  } else {
    cout << "File " << filenameEG << " not existing !" << endl;
    return;
  }


  if ( treeEG && treeMU ) {
    int nentriesEG = treeEG->GetEntries();
    int nentriesMU = treeMU->GetEntries();

    TFile *fileNew = TFile::Open(filename,"recreate");
    TTree *treeNew = new TTree("T1","tree with only selected events");

    // Declaration of leaf types
    Int_t           finalstate;
    Int_t           run;
    Int_t           lumi;
    Int_t           event;
    Float_t         jetcat;
    Float_t         projMet;
    Float_t         deltaPhi;
    Float_t         expCosDphi;
    Float_t         maxPtEle;
    Float_t         eleInvMass;
    Int_t           finalSelection;
   
    // MU is the principal PD
    treeMU->SetBranchAddress("finalstate", &finalstate);
    treeMU->SetBranchAddress("run", &run);
    treeMU->SetBranchAddress("lumi", &lumi);
    treeMU->SetBranchAddress("event", &event);
    treeMU->SetBranchAddress("projMet", &projMet);
    treeMU->SetBranchAddress("deltaPhi", &deltaPhi);
    treeMU->SetBranchAddress("eleInvMass", &eleInvMass);
    treeMU->SetBranchAddress("maxPtEle", &maxPtEle);
    treeMU->SetBranchAddress("finalSelection", &finalSelection);
    treeMU->SetBranchAddress("expCosDphi", &expCosDphi);
    treeMU->SetBranchAddress("jetcat", &jetcat);

    // copy branches
    treeNew->Branch("finalstate", &finalstate,  "finalstate/I");
    treeNew->Branch("run",   &run,   "run/I");
    treeNew->Branch("lumi",  &lumi,  "lumi/I");
    treeNew->Branch("event", &event, "event/I");
    treeNew->Branch("projMet", &projMet, "projMet/F");
    treeNew->Branch("deltaPhi", &deltaPhi, "deltaPhi/F");
    treeNew->Branch("eleInvMass", &eleInvMass, "eleInvMass/F");
    treeNew->Branch("maxPtEle", &maxPtEle, "eleInvMass/F");
    treeNew->Branch("finalSelection", &finalSelection, "finalSelection/I");
    treeNew->Branch("expCosDphi", &expCosDphi, "expCosDphi/F");
    treeNew->Branch("jetcat", &jetcat, "jetcat/F");

    std::vector<Event> muEvents;

    for(int i=0; i<nentriesMU; i++) {
      if (i%1000 == 0) std::cout << ">>> Considering event # " << i << " / " << nentriesMU << " entries of MU dataset" << std::endl;
      treeMU->GetEntry(i);
      muEvents.push_back(Event(run,lumi,event));
      treeNew->Fill();
    }
    
    // now use the EG dataset
    treeEG->SetBranchAddress("finalstate", &finalstate);
    treeEG->SetBranchAddress("run", &run);
    treeEG->SetBranchAddress("lumi", &lumi);
    treeEG->SetBranchAddress("event", &event);
    treeEG->SetBranchAddress("projMet", &projMet);
    treeEG->SetBranchAddress("deltaPhi", &deltaPhi);
    treeEG->SetBranchAddress("eleInvMass", &eleInvMass);
    treeEG->SetBranchAddress("maxPtEle", &maxPtEle);
    treeEG->SetBranchAddress("finalSelection", &finalSelection);
    treeEG->SetBranchAddress("expCosDphi", &expCosDphi);
    treeEG->SetBranchAddress("jetcat", &jetcat);
    
    for(int i=0; i<nentriesEG; i++) {
      if (i%1000 == 0) std::cout << ">>> Considering event # " << i << " / " << nentriesEG << " entries of EG dataset" << std::endl;
      treeEG->GetEntry(i);
      
      Event theEv(run,lumi,event);
      vector<Event>::const_iterator muEv;
      bool duplicate=false;
      for(muEv=muEvents.begin(); muEv!=muEvents.end(); muEv++) {
        if( theEv==(*muEv) ) {
          duplicate = true;
          break;
        }
      }

      if(!duplicate) {
        cout << "This event: run = " << run << "\tlumi = " << lumi << "\tevent = " << event << " is GOOD" << endl;
        treeNew->Fill();
      }
      else {
        cout << "This event: run = " << run << "\tlumi = " << lumi << "\tevent = " << event << " is duplicated" << endl;
      }
    }
    
    fileNew->cd();
    treeNew->Write();
    fileNew->Close();
    
    fileMU->cd();
    fileMU->Close();
    
    fileEG->cd();
    fileEG->Close();
    
  }  else {
    cout << "Tree T1 not present in the file " << filenameMU << " or in the file " << filenameEG << endl;
    return;
  }
  
}
