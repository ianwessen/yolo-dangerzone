// ! c++ includes
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream.h>
#include <math.h>

//! ROOT includes
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TApplication.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"

using namespace std;

// variables to be used later on
float trackerCut,   hcalCut,   ecalCut;
float trackerStep,  hcalStep,  ecalStep;
float trackerInit,  hcalInit,  ecalInit;

// methods
void setScanValue();
bool isIsolScan(float thisTracker, float thisHcal, float thisEcal, int tracker, int hcal, int ecal);

int main ( int argc, char **argv)
{
  if (argc < 8){ 
    cout << "Argument missing! Insert: "               << std::endl; 
    cout << "1) inputFile - root tree for sgn "        << std::endl;
    cout << "2) inputFile - root tree for bkg "        << std::endl;
    cout << "3) signal pre isol efficiency"            << std::endl;
    cout << "4) signal eleID x kine efficiency"        << std::endl;
    cout << "5) background pre isol # events"          << std::endl;
    cout << "6) background eleID x kine efficiency"    << std::endl;
    cout << "7) discovery (1) or exclusion (0) limits" << std::endl;
    return 0;
  }


  // reading the input trees --------------------------
  TChain *T[2];
  T[0]= new TChain("T1");
  T[1]= new TChain("T1");
  T[0]->Add(argv[1]);   // signal
  T[1]->Add(argv[2]);   // background
  float H_trackerIsol, L_trackerIsol;
  float H_hcalIsol,    L_hcalIsol;
  float H_ecalIsol,    L_ecalIsol;
  for(int ii=0; ii<2; ii++){
    T[ii]->SetMakeClass(1);
    T[ii]->SetBranchStatus("*",0);
    T[ii]->SetBranchStatus("H_trackerIsol",1);
    T[ii]->SetBranchStatus("H_hcalIsol",1);
    T[ii]->SetBranchStatus("H_ecalIsol",1);
    T[ii]->SetBranchStatus("L_trackerIsol",1);
    T[ii]->SetBranchStatus("L_hcalIsol",1);
    T[ii]->SetBranchStatus("L_ecalIsol",1);
    T[ii]->SetBranchAddress("H_trackerIsol",&H_trackerIsol);
    T[ii]->SetBranchAddress("H_hcalIsol",   &H_hcalIsol);
    T[ii]->SetBranchAddress("H_ecalIsol",   &H_ecalIsol);
    T[ii]->SetBranchAddress("L_trackerIsol",&L_trackerIsol);
    T[ii]->SetBranchAddress("L_hcalIsol",   &L_hcalIsol);
    T[ii]->SetBranchAddress("L_ecalIsol",   &L_ecalIsol);
  }

  // setting the scan
  setScanValue();
  
  // kinematical / preselection efficiencies
  float sgnPreIsolEff = atof(argv[3]);    
  float sgnKineEff    = atof(argv[4]);   
  float bkgPreIsolEvt = atof(argv[5]);    
  float bkgKineEff    = atof(argv[6]);     

  // discovery or exclusion
  int discovery = atoi(argv[7]); 

  // counters
  float passedIsol[10][10][10][2];
  for(int ii=0; ii<2; ii++){
    for(int iiTracker=0; iiTracker<10; iiTracker++){
      for(int iiHcal=0; iiHcal<10; iiHcal++){
	for(int iiEcal=0; iiEcal<10; iiEcal++){
	  passedIsol[iiTracker][iiHcal][iiEcal][ii]=0.;
	}}}}

  
  // loop: signal / background samples
  for(int ii=0; ii<2; ii++){
    
    // reading the tree
    float nEnt = T[ii]->GetEntries();
    cout << endl;
    cout << "Total number of events in loop for sample " << ii << " is " << nEnt << endl; 
    for (int entry=0; entry<nEnt; entry++) { 
      if (entry%1000==0) cout << "sample " << ii << ", entry " << entry << endl;
      T[ii] -> GetEntry(entry);
      
      // scan to compute the efficiencies for each bin
      bool theHighScan = false;
      bool theLowScan  = false;
      for(int iiTracker=0; iiTracker<10; iiTracker++){
	for(int iiHcal=0; iiHcal<10; iiHcal++){
	  for(int iiEcal=0; iiEcal<10; iiEcal++){
	    theHighScan=isIsolScan(H_trackerIsol, H_hcalIsol, H_ecalIsol, iiTracker, iiHcal, iiEcal);
	    theLowScan =isIsolScan(L_trackerIsol, L_hcalIsol, L_ecalIsol, iiTracker, iiHcal, iiEcal);
	    if(theHighScan && theLowScan){ 
	      passedIsol[iiTracker][iiHcal][iiEcal][ii]=passedIsol[iiTracker][iiHcal][iiEcal][ii]+1;
	    }
	  }}}
    } // loop over entries
  } // loop over signal / background 
  

  // maximization:
  ofstream *outTxtFile = new ofstream("outputFileScan.txt",ios::app);
  float signPunziMax = -999.;
  float effMax       = -999.;
  int signBinMax[8];
  for(int yy=0; yy<3; yy++){signBinMax[yy]=-1;}  
  
  for(int iiTracker=0; iiTracker<10; iiTracker++){
    for(int iiHcal=0; iiHcal<10; iiHcal++){
      for(int iiEcal=0; iiEcal<10; iiEcal++){
	float thisBinSgnEff = passedIsol[iiTracker][iiHcal][iiEcal][0]/((float)T[0]->GetEntries());
	float thisBinBkgEff = passedIsol[iiTracker][iiHcal][iiEcal][1]/((float)T[1]->GetEntries());
	float effSgn        = sgnPreIsolEff*thisBinSgnEff*sgnKineEff;
	float bkgEvents     = bkgPreIsolEvt*thisBinBkgEff*bkgKineEff;
	float sqrtB         = sqrt(bkgEvents);
	float signPunzi;
	if(discovery==1){  // 5 sigma 
	  signPunzi = effSgn/(2.5+sqrtB);
	}
	if(discovery==0){ // 2 sigma
	  signPunzi = effSgn/(1.+sqrtB);
	}
	
	// saving the full output
	*outTxtFile << iiTracker << " " << iiHcal << " " << iiEcal << " " << thisBinSgnEff << " " << thisBinBkgEff << " " << signPunzi << endl;
	
	// looking for the maximum
	if (signPunzi>signPunziMax) { 
	  signPunziMax  = signPunzi; 
	  effMax        = thisBinSgnEff;
	  signBinMax[0] = iiTracker;
	  signBinMax[1] = iiHcal;
	  signBinMax[2] = iiEcal;
	}
      }}}
  
  // max significance bin
  float trackerCut = trackerInit + signBinMax[0]*trackerStep;
  float hcalCut    = hcalInit    + signBinMax[1]*hcalStep;
  float ecalCut    = ecalInit    + signBinMax[2]*ecalStep;
  
  // output
  cout << endl;
  cout << "highest significance (Punzi) = " << signPunziMax << endl;
  cout << "eff isol signal = "     << passedIsol[signBinMax[0]][signBinMax[1]][signBinMax[2]][0]/((float)T[0]->GetEntries()) << endl;
  cout << "eff isol background = " << passedIsol[signBinMax[0]][signBinMax[1]][signBinMax[2]][1]/((float)T[1]->GetEntries()) << endl;
  cout << "in the following bin: "   << endl;
  cout << "tracker < " << trackerCut << endl; 
  cout << "hcal < "    << hcalCut    << endl; 
  cout << "ecal < "    << ecalCut    << endl; 
}


void setScanValue(){

  // starting point           
  trackerInit = 0.02;      
  hcalInit    = 0.02;      
  ecalInit    = 0.02;      

  // step for the scan
  trackerStep = 0.005;      
  hcalStep    = 0.02;      
  ecalStep    = 0.02;      
}

bool isIsolScan(float thisTracker, float thisHcal, float thisEcal, int tracker, int hcal, int ecal){ 

  // the current cut
  float trackerCut = trackerInit + tracker*trackerStep;
  float hcalCut    = hcalInit + hcal*hcalStep;
  float ecalCut    = ecalInit + ecal*ecalStep;
  
  bool isPassed = false;
  if ( thisTracker<=trackerCut ){
    if ( thisHcal<=hcalCut ){
      if ( thisEcal<=ecalCut ){
	isPassed = true;
      }}}
  return isPassed;
}
