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
float meeMinCut, meeMaxCut, metMinCut, metMaxCut, dphiMaxCut, ptHighMinCut, ptHighMaxCut, ptLowMinCut, ptLowMaxCut;
float meeMinStep, meeMaxStep, metMinStep, metMaxStep, dphiMaxStep, ptHighMinStep, ptHighMaxStep, ptLowMinStep, ptLowMaxStep;
float meeMinInit, meeMaxInit, metMinInit, metMaxInit, dphiMaxInit, ptHighMinInit, ptHighMaxInit, ptLowMinInit, ptLowMaxInit;


// methods
void setScanValue();
bool isKineScan(float thisMee, float thisMet, float thisDphi, float thisPtHigh, float thisPtLow, 
		int meemin, int meemax, int metmin, int metmax, int dphimax, int pthighmin, int pthighmax, int ptlowmin, int ptlowmax);

int main ( int argc, char **argv)
{
  if (argc < 7){ 
    cout << "Argument missing! Insert: "                                       << std::endl; 
    cout << "1) inputFile - root tree for sgn"                                 << std::endl;
    cout << "2) signal pre kine efficiency"                                    << std::endl;
    cout << "3) inputFile - text file with ALPGEN root file and nevents "      << std::endl;
    cout << "4) inputFile - text file with other bkg root file, eff and xsec " << std::endl;
    cout << "5) discovery (1) or exclusion (0) limits"                         << std::endl;
    cout << "6) output scan file name"                                         << std::endl;
    return 0;
  }

  // the luminosity for the optimization
  float lumi = 1000; // pb-1

  std::string rootFileAlpgen[3], rootFileOthers[4];
  float nEventsAlpgenPreKin[3];
  float effPreKinOthers[4], xsecOhters[4];

  // reading the text file for ALPGEN
  ifstream inFileAlpgen(argv[3]);
  if( inFileAlpgen.is_open() ) {
    for(int i=0; i<3; i++) {
      inFileAlpgen >> rootFileAlpgen[i] >> nEventsAlpgenPreKin[i];
      std::cout << "Expected events pre-kine in 1fb-1 of process " << rootFileAlpgen[i] 
		<< " = " << nEventsAlpgenPreKin[i] << std::endl;
    }
  }
  else {
    cout << "Unable to open file " << inFileAlpgen << std::endl;
  }

  // reading the text file for other backgrounds
  ifstream inFileOthers(argv[4]);
  if( inFileOthers.is_open() ) {
    for(int i=0; i<4; i++) {
      inFileOthers >> rootFileOthers[i] >> effPreKinOthers[i] >> xsecOhters[i];
      std::cout << "Sample " << rootFileOthers[i] << " has eff pre kine = " << effPreKinOthers[i] 
		<< " and x-sec = " << xsecOhters[i] << " pb" << std::endl;
    }
  }
  else {
    cout << "Unable to open file " << inFileOthers << std::endl;
  }

  // reading the input trees --------------------------
  TChain *T[9];
  T[0]= new TChain("T1"); // signal
  T[1]= new TChain("T1"); // ttbar
  T[2]= new TChain("T1"); // W+jets
  T[3]= new TChain("T1"); // Z+jets
  T[4]= new TChain("T1"); // WW
  T[5]= new TChain("T1"); // tW
  T[6]= new TChain("T1"); // WZ
  T[7]= new TChain("T1"); // ZZ

  T[0]->Add(argv[1]);   // signal
  T[1]->Add(rootFileAlpgen[0].c_str());   // background
  T[2]->Add(rootFileAlpgen[1].c_str());   // background
  T[3]->Add(rootFileAlpgen[2].c_str());   // background
  T[4]->Add(rootFileOthers[0].c_str());   // background
  T[5]->Add(rootFileOthers[1].c_str());   // background
  T[6]->Add(rootFileOthers[2].c_str());   // background
  T[7]->Add(rootFileOthers[3].c_str());   // background

  float mee, met, dphi, ptHigh, ptLow;

  for(int ii=0; ii<8; ii++){
    T[ii]->SetMakeClass(1);
    T[ii]->SetBranchStatus("*",0);
    T[ii]->SetBranchStatus("mee",1);
    T[ii]->SetBranchStatus("met",1);
    T[ii]->SetBranchStatus("dphi",1);
    T[ii]->SetBranchStatus("ptHigh",1);
    T[ii]->SetBranchStatus("ptLow",1);
    T[ii]->SetBranchAddress("mee",&mee);
    T[ii]->SetBranchAddress("met",&met);
    T[ii]->SetBranchAddress("dphi",&dphi);
    T[ii]->SetBranchAddress("ptHigh",&ptHigh);
    T[ii]->SetBranchAddress("ptLow",&ptLow);
  }

  // setting the scan
  setScanValue();
  
  // kinematical / preselection efficiencies
  float sgnPreKineEff = atof(argv[2]);
  
  float nBkgPreKine[7];
  // ttbar, w+jets, z+jets in chowder
  for(int i=0; i<3; i++) {
    nBkgPreKine[i] = nEventsAlpgenPreKin[i] / 1000. * lumi;
    std::cout << "Expected nBkgPreKine[" << i << "]= " << nBkgPreKine[i] << std::endl;
  }
  for(int i=0; i<4; i++) {
    nBkgPreKine[i+3] = effPreKinOthers[i] * xsecOhters[i] * lumi;
    std::cout << "Expected nBkgPreKine[" << i << "]= " << nBkgPreKine[i+3] << std::endl;
  }

  // discovery or exclusion
  int discovery = atoi(argv[5]); 

  // counters
  float passedKine[1][3][3][3][4][4][4][4][1][8];
  for(int ii=0; ii<8; ii++){
    for(int iiMeeMin=0; iiMeeMin<1; iiMeeMin++){
      for(int iiMeeMax=0; iiMeeMax<3; iiMeeMax++){
	for(int iiMetMin=0; iiMetMin<3; iiMetMin++){
	  for(int iiMetMax=0; iiMetMax<3; iiMetMax++){
	    for(int iiDphiMax=0; iiDphiMax<4; iiDphiMax++){
	      for(int iiPtHighMin=0; iiPtHighMin<4; iiPtHighMin++){
		for(int iiPtHighMax=0; iiPtHighMax<4; iiPtHighMax++){
		  for(int iiPtLowMin=0; iiPtLowMin<4; iiPtLowMin++){
		    for(int iiPtLowMax=0; iiPtLowMax<1; iiPtLowMax++){
		      passedKine[iiMeeMin][iiMeeMax][iiMetMin][iiMetMax][iiDphiMax][iiPtHighMin][iiPtHighMax][iiPtLowMin][iiPtLowMax][ii]=0;
		    }}}}}}}}}}
    

  
  // loop: signal / background samples
  for(int ii=0; ii<8; ii++){
    
    // reading the tree
    float nEnt = T[ii]->GetEntries();
    cout << endl;
    cout << "Total number of events in loop for sample " << ii << " is " << nEnt << endl; 
    for (int entry=0; entry<nEnt; entry++) { 
      if (entry%1000==0) cout << "sample " << ii << ", entry " << entry << endl;
      T[ii] -> GetEntry(entry);
      
      // scan to compute the efficiencies for each bin
      bool theScan  = false;
      for(int iiMeeMin=0; iiMeeMin<1; iiMeeMin++){
	for(int iiMeeMax=0; iiMeeMax<3; iiMeeMax++){
	  for(int iiMetMin=0; iiMetMin<3; iiMetMin++){
	    for(int iiMetMax=0; iiMetMax<3; iiMetMax++){
	      for(int iiDphiMax=0; iiDphiMax<4; iiDphiMax++){
		for(int iiPtHighMin=0; iiPtHighMin<4; iiPtHighMin++){
		  for(int iiPtHighMax=0; iiPtHighMax<4; iiPtHighMax++){
		    for(int iiPtLowMin=0; iiPtLowMin<4; iiPtLowMin++){
		      for(int iiPtLowMax=0; iiPtLowMax<1; iiPtLowMax++){
			theScan=isKineScan(mee,met,dphi,ptHigh,ptLow,iiMeeMin,iiMeeMax,iiMetMin,iiMetMax,iiDphiMax,iiPtHighMin,iiPtHighMax,iiPtLowMin,iiPtLowMax);
			if(theScan){ 
			  passedKine[iiMeeMin][iiMeeMax][iiMetMin][iiMetMax][iiDphiMax][iiPtHighMin][iiPtHighMax][iiPtLowMin][iiPtLowMax][ii]+=1;
			}
		      }}}}}}}}}
    } // loop over entries
  } // loop over signal / backgrounds 
  

    // maximization:
  ofstream *outTxtFile = new ofstream(argv[6],ios::app);
  float signPunziMax = -999.;
  float effMax       = -999.;
  int signBinMax[9];
  for(int yy=0; yy<9; yy++){signBinMax[yy]=-1;}  
  
  for(int iiMeeMin=0; iiMeeMin<1; iiMeeMin++){
    for(int iiMeeMax=0; iiMeeMax<3; iiMeeMax++){
      for(int iiMetMin=0; iiMetMin<3; iiMetMin++){
	for(int iiMetMax=0; iiMetMax<3; iiMetMax++){
	  for(int iiDphiMax=0; iiDphiMax<4; iiDphiMax++){
	    for(int iiPtHighMin=0; iiPtHighMin<4; iiPtHighMin++){
	      for(int iiPtHighMax=0; iiPtHighMax<4; iiPtHighMax++){
		for(int iiPtLowMin=0; iiPtLowMin<4; iiPtLowMin++){
		  for(int iiPtLowMax=0; iiPtLowMax<1; iiPtLowMax++){
		    
		    float thisBinSgnEff = passedKine[iiMeeMin][iiMeeMax][iiMetMin][iiMetMax][iiDphiMax][iiPtHighMin][iiPtHighMax][iiPtLowMin][iiPtLowMax][0]/((float)T[0]->GetEntries());
		    float thisBinBkgEff[7];
		    for(int ibkg=0; ibkg<7; ibkg++) {
		      thisBinBkgEff[ibkg] = passedKine[iiMeeMin][iiMeeMax][iiMetMin][iiMetMax][iiDphiMax][iiPtHighMin][iiPtHighMax][iiPtLowMin][iiPtLowMax][ibkg+1]/((float)T[ibkg+1]->GetEntries());
		    }
		    
		    float effSgn        = sgnPreKineEff*thisBinSgnEff;
		    float bkgEvents     = 0;
		    for(int ibkg=0; ibkg<7; ibkg++) {
		      bkgEvents += nBkgPreKine[ibkg]*thisBinBkgEff[ibkg];
		    }
		    
		    float sqrtB         = sqrt(bkgEvents);
		    float signPunzi;
		    if(discovery==1){  // 5 sigma 
		      signPunzi = effSgn/(2.5+sqrtB);
		    }
		    if(discovery==0){ // 2 sigma
		      signPunzi = effSgn/(1.+sqrtB);
		    }
	
		    // saving the full output
		    *outTxtFile << iiMeeMin << " " << iiMeeMax << " " 
				<< iiMetMin << " " << iiMetMax << " "
				<< iiDphiMax << " "
				<< iiPtHighMin << " " << iiPtHighMax << " "
				<< iiPtLowMin << " " << iiPtLowMax << " "
				<< thisBinSgnEff << " ";
		    
		    for(int ibkg=0; ibkg<7; ibkg++) {
		      *outTxtFile << nBkgPreKine[ibkg]*thisBinBkgEff[ibkg] << " "; 
		    }

		    *outTxtFile <<  signPunzi << endl;

	
		    // looking for the maximum
		    if (signPunzi>signPunziMax) { 
		      signPunziMax  = signPunzi; 
		      effMax        = thisBinSgnEff;
		      signBinMax[0] = iiMeeMin;
		      signBinMax[1] = iiMeeMax;
		      signBinMax[2] = iiMetMin;
		      signBinMax[3] = iiMetMax;
		      signBinMax[4] = iiDphiMax;
		      signBinMax[5] = iiPtHighMin;
		      signBinMax[6] = iiPtHighMax;
		      signBinMax[7] = iiPtLowMin;
		      signBinMax[8] = iiPtLowMax;
		    }
		  }}}}}}}}}
  
  // max significance bin
  float meeMinCut = meeMinInit  + signBinMax[0]*meeMinStep;
  float meeMaxCut = meeMaxInit  + signBinMax[1]*meeMaxStep;
  float metMinCut = metMinInit  + signBinMax[2]*metMinStep;
  float metMaxCut = metMaxInit  + signBinMax[3]*metMaxStep;
  float dphiMaxCut = dphiMaxInit  + signBinMax[4]*dphiMaxStep;
  float ptHighMinCut = ptHighMinInit  + signBinMax[5]*ptHighMinStep;
  float ptHighMaxCut = ptHighMaxInit  + signBinMax[6]*ptHighMaxStep;
  float ptLowMinCut = ptLowMinInit  + signBinMax[7]*ptLowMinStep;
  float ptLowMaxCut = ptLowMaxInit  + signBinMax[8]*ptLowMaxStep;
  
  // output
  cout << endl;
  cout << "highest significance (Punzi) = " << signPunziMax << endl;
  cout << "eff kine signal = "     << passedKine[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][signBinMax[5]][signBinMax[6]][signBinMax[7]][signBinMax[8]][0]/((float)T[0]->GetEntries()) << endl;
  for(int i=0; i<7; i++) {
    cout << "eff kine background[" << i << "] = " << passedKine[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][signBinMax[5]][signBinMax[6]][signBinMax[7]][signBinMax[8]][i+1]/((float)T[i+1]->GetEntries()) << endl;
  }
  cout << "in the following bin: "   << endl;
  cout << "mee = " << meeMinCut << " - " << meeMaxCut << endl;
  cout << "met = " << metMinCut << " - " << metMaxCut << endl;
  cout << "deltaPhi = 0 - " << dphiMaxCut << endl;
  cout << "ptHigh = " << ptHighMinCut << " - " << ptHighMaxCut << endl;
  cout << "ptLow = " << ptLowMinCut << " - " << ptLowMaxCut << endl;
  
}


void setScanValue(){

  // starting point         step  
  meeMinInit = 12.0;        meeMinStep    =  2.0; // fix to pre-selection
  meeMaxInit = 70.0;        meeMaxStep    = -5.0;
  metMinInit = 30.0;        metMinStep    =  5.0;
  metMaxInit = 90.0;        metMaxStep    = -5.0;
  dphiMaxInit = 100.0;      dphiMaxStep   = -20.0;
  ptHighMinInit = 20.0;     ptHighMinStep =  3.0;
  ptHighMaxInit = 78.0;     ptHighMaxStep = -4.0;
  ptLowMinInit = 10.0;      ptLowMinStep  =  3.0;
  ptLowMaxInit = 90.0;      ptLowMaxStep  = -4.0;

}

bool isKineScan(float thisMee, float thisMet, float thisDphi, float thisPtHigh, float thisPtLow, 
		int meemin, int meemax, int metmin, int metmax, int dphimax, int pthighmin, int pthighmax, int ptlowmin, int ptlowmax) { 

  // the current cut
  float meeMinCut = meeMinInit + meemin*meeMinStep;
  float meeMaxCut = meeMaxInit + meemax*meeMaxStep;
  float metMinCut = metMinInit + metmin*metMinStep;
  float metMaxCut = metMaxInit + metmax*meeMaxStep;
  float dphiMaxCut = dphiMaxInit + dphimax*dphiMaxStep;
  float ptHighMinCut = ptHighMinInit + pthighmin*ptHighMinStep;
  float ptHighMaxCut = ptHighMaxInit + pthighmax*ptHighMaxStep;
  float ptLowMinCut = ptLowMinInit + ptlowmin*ptLowMinStep;
  float ptLowMaxCut = ptLowMaxInit + ptlowmax*ptLowMaxStep;

  bool isPassed = false;
  if ( (thisMee<=meeMaxCut) && (thisMee>=meeMinCut) ) {
    if ( (thisMet<=metMaxCut) && (thisMet>=metMinCut) ) {
      if ( thisDphi<=dphiMaxCut ) {
	if ( (thisPtHigh<=ptHighMaxCut) && (thisPtHigh>=ptHighMinCut) ) {
	  if ( (thisPtLow<=ptLowMaxCut) && (thisPtLow>=ptLowMinCut) ) {
	    isPassed = true;
	  }}}}}

  return isPassed;
}
