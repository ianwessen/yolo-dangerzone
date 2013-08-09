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

// ancora da fare: nella zuppa selezionare gli eventi secondo il processo e pesarli 

// variables to be used later on
float trackerCut,   hcalCut,   ecalCut,  dzCut,  dxySignCut;
float trackerStep,  hcalStep,  ecalStep, dzStep, dxySignStep;
float trackerInit,  hcalInit,  ecalInit, dzInit, dxySignInit;

// methods
void setScanValue();
bool isScan(float thisTracker, float thisHcal, float thisEcal, float thisDz, float thisDxySign, int trackerIsol, int hcalIsol, int ecalIsol, int dzVtx, int dxyVtx);

int main ( int argc, char **argv)
{
  if (argc < 5){
    cout << "Argument missing! Insert: "                                                          << std::endl; 
    cout << "1) inputFile - root tree for chowder"                                                << std::endl;
    cout << "2) inputFile - root tree for QCD"                                                    << std::endl;
    cout << "3) discovery (1) or exclusion (0) limits"                                            << std::endl;
    cout << "4) output scan file name"                                                            << std::endl;
    return 0;
  }

  // the luminosity for the optimization
  float lumi = 1000; // pb-1

  // reading the input trees --------------------------
  TChain *T[2];
  T[0]= new TChain("T1"); // signal: W+jets, W -> e nu + backgrounds W+jets, W -> other + ttbar
  T[1]= new TChain("T1"); // QCD di-jets

  T[0]->Add(argv[1]);   // signal + W->others + ttbar
  T[1]->Add(argv[2]);   // QCD

  float normalization[4];
  // 0 = signal W+jets, W->enu
  // 1 = bkg W+jets, W->others
  // 2 = ttbar
  // 3 = QCD
  for(int ii=0; ii<4; ii++)
    normalization[ii]=0;

  // QCD
  float nQCDBkgPreVtx = 3.28E+06 * lumi/100;
  float QCDbkgKineEff = 0.14;

  int WToENuDecay;
  float trackerIsol, hcalIsol, ecalIsol, dzVtx, dxyVtx, dxyErrVtx;
  float CSA07lumi;
  double CSA07weight, CSA07processId;

  for(int ii=0; ii<2; ii++){
    T[ii]->SetMakeClass(1);
    T[ii]->SetBranchStatus("*",0);
    T[ii]->SetBranchStatus("trackerIsol,",1);
    T[ii]->SetBranchStatus("hcalIsol,",1);
    T[ii]->SetBranchStatus("ecalIsol",1);
    T[ii]->SetBranchStatus("dzVtx",1);
    T[ii]->SetBranchStatus("dxyVtx",1);
    T[ii]->SetBranchStatus("dxyErrVtx",1);
    T[ii]->SetBranchStatus("WToENuDecay",1);
    T[ii]->SetBranchStatus("CSA07weight",1);
    T[ii]->SetBranchStatus("CSA07processId",1);
    T[ii]->SetBranchStatus("CSA07lumi",1);
    T[ii]->SetBranchAddress("trackerIsol",&trackerIsol);
    T[ii]->SetBranchAddress("hcalIsol", &hcalIsol);
    T[ii]->SetBranchAddress("ecalIsol", &ecalIsol);
    T[ii]->SetBranchAddress("dzVtx", &dzVtx);
    T[ii]->SetBranchAddress("dxyVtx", &dxyVtx);
    T[ii]->SetBranchAddress("dxyErrVtx", &dxyErrVtx);
    T[ii]->SetBranchAddress("WToENuDecay", &WToENuDecay);
    T[ii]->SetBranchAddress("CSA07weight", &CSA07weight);
    T[ii]->SetBranchAddress("CSA07processId", &CSA07processId);
    T[ii]->SetBranchAddress("CSA07lumi", &CSA07lumi);
  }

  // setting the scan
  setScanValue();
  
  // discovery or exclusion
  int discovery = atoi(argv[3]); 

  // counters
  float passedVtx[10][10][10][10][10][4];
  for(int ii=0; ii<4; ii++){
    for(int iiTracker=0; iiTracker<10; iiTracker++){
      for(int iiHcal=0; iiHcal<10; iiHcal++){
	for(int iiEcal=0; iiEcal<10; iiEcal++){
	  for(int iiDz=0; iiDz<10; iiDz++){
	    for(int iiDxySign=0; iiDxySign<10; iiDxySign++){
	      passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][ii]=0;
	    }}}}}}
  
  // loop: signal / background samples
  for(int ii=0; ii<2; ii++){

    // reading the tree
    float nEnt = T[ii]->GetEntries();
    cout << endl;
    cout << "Total number of events in loop for sample " << ii << " is " << nEnt << endl; 
    for (int entry=0; entry<nEnt; entry++) { 
      if (entry%1000==0) cout << "sample " << ii << ", entry " << entry << endl;
      T[ii] -> GetEntry(entry);

      int isample=-1;

      bool Wj_WToENu_Gt1Part = CSA07processId>1000 && CSA07processId<2000 && WToENuDecay==1;
      bool Wj_WToOther_Gt1Part = CSA07processId>1000 && CSA07processId<2000 && WToENuDecay==0;
      bool ttbar = CSA07processId>=3000;

      if(ii==0) { // chowder
	if(Wj_WToENu_Gt1Part) { // signal
	  isample=0;
	  normalization[0] += CSA07weight * lumi/100.;
	}
	else if(Wj_WToOther_Gt1Part) { // W->other
	  isample=1;
	  normalization[1] += CSA07weight * lumi/100.;
	}
	else if(ttbar) { // ttbar
	  isample=2;
	  normalization[2] += CSA07weight * lumi/100.;
	}
      }
      if(ii==1) { // QCD from ppEleX
	isample=3;
        normalization[3]++;
      }

      if(isample<0 || isample>3) continue; // do not consider Z+jets

      // scan to compute the efficiencies for each bin
      bool theScan  = false;
      for(int iiTracker=0; iiTracker<10; iiTracker++){
	for(int iiHcal=0; iiHcal<10; iiHcal++){
	  for(int iiEcal=0; iiEcal<10; iiEcal++){
	    for(int iiDz=0; iiDz<10; iiDz++){
	      for(int iiDxySign=0; iiDxySign<10; iiDxySign++){
		theScan=isScan(trackerIsol, hcalIsol, ecalIsol, dzVtx, dxyVtx/dxyErrVtx, iiTracker, iiHcal, iiEcal, iiDz, iiDxySign);
		if(theScan){
		  if(isample<3)
		    passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][isample] += CSA07weight * lumi/100.;
		  else
		    passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][isample]++;
		}
	      }}}}}
    } // loop over entries
  } // loop over signal / backgrounds 
  

  // maximization:
  ofstream *outTxtFile = new ofstream(argv[4],ios::app);
  float signPunziMax = -999.;
  float effMax       = -999.;
  int signBinMax[5];
  for(int yy=0; yy<5; yy++){signBinMax[yy]=-1;}  
  
  for(int iiTracker=0; iiTracker<10; iiTracker++){
    for(int iiHcal=0; iiHcal<10; iiHcal++){
      for(int iiEcal=0; iiEcal<10; iiEcal++){
	for(int iiDz=0; iiDz<10; iiDz++){
	  for(int iiDxySign=0; iiDxySign<10; iiDxySign++){
	    
	    float sigEff = passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][0]/((float)normalization[0]);
	    float ibkgEvents[3];
 	    float bkgEvents = 0;

	    for(int ibkg=1; ibkg<3; ibkg++) { // W(->others) + ttbar from chowder
	      ibkgEvents[ibkg-1] = passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][ibkg];
	      bkgEvents += ibkgEvents[ibkg-1];
	    }
	    float thisBinQCDBkgEff = passedVtx[iiTracker][iiHcal][iiEcal][iiDz][iiDxySign][3]/((float)normalization[3]);
	    ibkgEvents[2] = nQCDBkgPreVtx*thisBinQCDBkgEff*QCDbkgKineEff; 
	    bkgEvents += ibkgEvents[2];
	    
	    float sqrtB = sqrt(bkgEvents);
	    float signPunzi;
	    if(discovery==1){  // 5 sigma 
	      signPunzi = sigEff/(2.5+sqrtB);
	    }
	    if(discovery==0){ // 2 sigma
	      signPunzi = sigEff/(1.+sqrtB);
	    }
	    
	    // saving the full output
	    *outTxtFile << iiTracker << " " << iiHcal << " " << iiEcal << " " << iiDz << " " << iiDxySign << " ";
	    
	    for(int ibkg=0; ibkg<3; ibkg++) {
	      *outTxtFile << ibkgEvents[ibkg] << " ";
	    }

	    *outTxtFile <<  signPunzi << endl;
	    
	    // looking for the maximum
	    if (signPunzi>signPunziMax) { 
	      signPunziMax  = signPunzi; 
	      effMax        = sigEff;
	      signBinMax[0] = iiTracker;
	      signBinMax[1] = iiHcal;
	      signBinMax[2] = iiEcal;
	      signBinMax[3] = iiDz;
	      signBinMax[4] = iiDxySign;
	    }
	  }}}}}
  
  // max significance bin
  float trackerCut = trackerInit  + signBinMax[0]*trackerStep;
  float hcalCut    = hcalInit  + signBinMax[1]*hcalStep;
  float ecalCut    = ecalInit  + signBinMax[2]*ecalStep;
  float dzCut      = dzInit  + signBinMax[3]*dzStep;
  float dxySignCut = dxySignInit  + signBinMax[4]*dxySignStep;
  
  // output
  cout << endl;
  cout << "highest significance (Punzi) = " << signPunziMax << endl;
  cout << "eff vtx/isol signal = "     << passedVtx[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][0]/((float)normalization[0]) << endl;
  for(int i=0; i<3; i++) {
    cout << "eff vtx/iso background[" << i << "] = " << passedVtx[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][i+1]/((float)normalization[i+1]) << endl;
  }
  cout << "in the following bin: "   << endl;
  cout << "tracker = " << trackerCut << endl;
  cout << "hcal = "    << hcalCut    << endl;
  cout << "ecal = "    << ecalCut    << endl;
  cout << "dz = "      << dzCut      << endl;
  cout << "dxySign = " << dxySignCut << endl;

  
}


void setScanValue(){

  // starting point         
  trackerInit = 0.02;
  hcalInit    = 0.02;
  ecalInit    = 0.02;
  dzInit      = 0.02;
  dxySignInit = 2.0;

  // step for the scan
  trackerStep = 0.005;
  hcalStep    = 0.02;
  ecalStep    = 0.02;
  dzStep      = 0.02;
  dxySignStep = 2.0;
}

bool isScan(float thisTracker, float thisHcal, float thisEcal, float thisDz, float thisDxySign, int trackerIsol, int hcalIsol, int ecalIsol, int dzVtx, int dxyVtx) {

  // the current cut
  float trackerCut = trackerInit + trackerIsol*trackerStep;
  float hcalCut    = hcalInit + hcalIsol*hcalStep;
  float ecalCut    = ecalInit + ecalIsol*ecalStep;
  float dzCut      = dzInit + dzVtx*dzStep;
  float dxySignCut = dxySignInit + dxyVtx*dxySignStep;

  bool isPassed = false;
  if ( thisTracker<=trackerCut ) {
    if ( thisHcal<=hcalCut ) {
      if ( thisEcal<=ecalCut ) {
	if ( fabs(thisDz)<=dzCut ) {
	  if ( fabs(thisDxySign)<=dxySignCut ) {
      	    isPassed = true;
	  }}}}}

  return isPassed;
}
