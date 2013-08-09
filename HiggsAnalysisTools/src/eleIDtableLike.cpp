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

// methods
bool isEleIDScan(float thisLike, float cutLike);

int main ( int argc, char **argv)
{
  if (argc < 7){ 
    cout << "Argument missing! Insert: "               << std::endl; 
    cout << "1) inputFile - root tree for sgn "        << std::endl;
    cout << "2) signal preEleID efficiency"            << std::endl;
    cout << "3) signal kine efficiency"                << std::endl;
    cout << "4) inputTextFile with W+jets root file,  pre eleID eff, kine eff, and xsec (pb)" << std::endl;
    cout << "5) inputTextFile with tt+jets root file, pre eleID eff, kine eff, and xsec (pb)" << std::endl;
    cout << "6) class to be analyzed:"                  << std::endl;
    cout << "   0 --> not showering"                    << std::endl;
    cout << "   1 --> showering"                        << std::endl;
    cout << "7) luminosity per optimization (in pb-1) " << std::endl;
    cout << "8) discovery (1) or exclusion (0) limits"  << std::endl;
    return 0;
  }

  // we apply loose electronID on the highest pt electron (since it's already applied, we assume eff = 1)
  // and we optimize the like on the second one (the possible fake) when it belongs to the class we are analyzing
  // class: showering / not showering
  
  // charging infos on backgrounds
  std::string rootFileBkg[2];
  float effPreEleIDBkg[2], effKineBkg[2], xsecBkg[2];

  std::string rootFileWjets;
  float effPreEleIDWjets,  effKineWjets,  xsecWjets;
  ifstream inFileWjets(argv[4]);
  if( inFileWjets.is_open() ) inFileWjets >> rootFileWjets >> effPreEleIDWjets >> effKineWjets >> xsecWjets;
  else { cout << "Unable to open file " << inFileWjets << std::endl; }
  rootFileBkg[0]    = rootFileWjets;
  effPreEleIDBkg[0] = effPreEleIDWjets;
  effKineBkg[0]     = effKineWjets;
  xsecBkg[0]        = xsecWjets;

  std::string rootFileTTjets;
  float effPreEleIDTTjets, effKineTTjets, xsecTTjets;
  ifstream inFileTTjets(argv[5]);
  if( inFileTTjets.is_open() ) inFileTTjets >> rootFileTTjets >> effPreEleIDTTjets >> effKineTTjets >> xsecTTjets;
  else { cout << "Unable to open file " << inFileTTjets << std::endl; }
  rootFileBkg[1]    = rootFileTTjets;
  effPreEleIDBkg[1] = effPreEleIDTTjets;
  effKineBkg[1]     = effKineTTjets;
  xsecBkg[1]        = xsecTTjets;

  // check
  for(int ii=0; ii<2; ii++) 
    std::cout << "Sample " << rootFileBkg[ii] 
	      << " has eff pre eleID = " << effPreEleIDBkg[ii] 
	      << " and x-sec = " << xsecBkg[ii] 
	      << " pb" << std::endl;


  // reading the input trees for signal and background
  TChain *T[3];
  T[0]= new TChain("T1");   // signal
  T[1]= new TChain("T1");   // w+jets
  T[2]= new TChain("T1");   // ttbar
  T[0]->Add(argv[1]);
  T[1]->Add(rootFileBkg[0].c_str());
  T[2]->Add(rootFileBkg[1].c_str());

  float L_Likelihood;
  int L_EleClass;
  for(int ii=0; ii<3; ii++){
    T[ii]->SetMakeClass(1);
    T[ii]->SetBranchStatus("*",0);
    T[ii]->SetBranchStatus("L_Likelihood",1);
    T[ii]->SetBranchStatus("L_EleClass",1);
    T[ii]->SetBranchAddress("L_Likelihood",&L_Likelihood);
    T[ii]->SetBranchAddress("L_EleClass",  &L_EleClass);
  }



  // taking all other inputs -----------------------------------

  // electron class
  int theClass = atoi(argv[6]);
  
  // wanted luminosity
  float lumi = atof(argv[7]);

  // discovery or exclusion
  int discovery = atoi(argv[8]);

  // kinematical / preselection efficiencies on signal
  float sgnPreEleIDEff = atof(argv[2]);
  float sgnKineEff     = atof(argv[3]);
  cout << "Signal: pre eleID efficiency = " << sgnPreEleIDEff << ", kine efficiency = " << sgnKineEff << endl;

  // kinematical / preselection efficiencies on backgrounds
  float nBkgPreEleID[2];
  for(int i=0; i<2; i++) {
    nBkgPreEleID[i] = effPreEleIDBkg[i] * xsecBkg[i] * lumi;
    std::cout << "Expected nBkgPreEleID[" << i << "]= " << nBkgPreEleID[i] << std::endl;
  }



  // counters
  float passedEleID[100][3];
  for(int ii=0; ii<3; ii++){                  //  signal/background
    for(int iiLL=0; iiLL<100; iiLL++){        //  likelihood cut
      passedEleID[iiLL][ii]=0.;
    }}
  
  float passedLClass[3];
  for(int ii=0; ii<3; ii++){ passedLClass[ii]=0.; }

  // loop: signal / background samples
  for(int ii=0; ii<3; ii++){
    
    // reading the tree
    float nEnt = T[ii]->GetEntries();
    cout << endl;
    cout << "Total number of events in loop for sample " << ii << " is " << nEnt << endl; 

    for (int entry=0; entry<nEnt; entry++) { 
      if (entry%1000==0) cout << "sample " << ii << ", entry " << entry << endl;
      T[ii] -> GetEntry(entry);
      
      // only if the 2nd electron belongs to the wanted class
      float thisLClass = 4;
      if (L_EleClass==0 || L_EleClass==100 || L_EleClass==10 || L_EleClass==110 || L_EleClass==20 || L_EleClass==120) thisLClass=0;
      if ( (L_EleClass>=30  && L_EleClass<40) || (L_EleClass>=130  && L_EleClass<140) ) thisLClass=1;
      // if (thisLClass != theClass) continue;
      passedLClass[ii]++;
	
      
      // scan to compute the efficiencies for each bin
      bool theLowScan  = false;
      for(int iiLL=0; iiLL<100; iiLL++){
	theLowScan = isEleIDScan(L_Likelihood, iiLL);
	if(theLowScan) { passedEleID[iiLL][ii]++; }
      }
    } // loop over entries
  } // loop over signal / background 




  // maximization:
  ofstream *outTxtFile = new ofstream("outputFileScan.txt",ios::app);
  
  float signPunziMax = -999.;
  float effMax       = -999.;
  int signBinMax     = -999;

  for(int iiLL=0; iiLL<100; iiLL++){
    float thisClassSgnEff = passedLClass[0]/((float)T[0]->GetEntries());
    float thisBinSgnEff   = passedEleID[iiLL][0]/passedLClass[0];
    float effSgn          = sgnPreEleIDEff*thisClassSgnEff*thisBinSgnEff*sgnKineEff;  
    
    float thisClassBkgEff[2];
    float thisBinBkgEff[2];
    for(int ibkg=0; ibkg<2; ibkg++) {
      thisClassBkgEff[ibkg] = passedLClass[ibkg+1]/((float)T[ibkg+1]->GetEntries());
      thisBinBkgEff[ibkg]   = passedEleID[iiLL][ibkg+1]/passedLClass[ibkg+1];
    }
    
    float bkgEvents = 0;
    for(int ibkg=0; ibkg<2; ibkg++) {
      bkgEvents += nBkgPreEleID[ibkg]*thisClassBkgEff[ibkg]*thisBinBkgEff[ibkg]*effKineBkg[ibkg];
    }
    
    float sqrtB = sqrt(bkgEvents);
      float signPunzi;
      if(discovery==1){ signPunzi = effSgn/(2.5+sqrtB); } // 5 sigma
      if(discovery==0){ signPunzi = effSgn/(1.+sqrtB); }  // 2 sigma

      // saving the full output
      *outTxtFile << iiLL << " " << thisBinSgnEff << " " << thisBinBkgEff[0] << " " << thisBinBkgEff[1] << " " << signPunzi << endl;
  
      // looking for the maximum
      if (signPunzi>signPunziMax) { 
	signPunziMax = signPunzi; 
	effMax       = thisBinSgnEff;
	signBinMax   = iiLL;
      }
  }
  
  // max significance bin
  float lowCut = signBinMax*0.01;

  // output
  cout << endl;
  //
  cout << "Class = " << theClass << endl;
  cout << "eff class signal = " << passedLClass[0]/((float)T[0]->GetEntries()) << endl;
  cout << "eff class wjets  = " << passedLClass[1]/((float)T[1]->GetEntries()) << endl;
  cout << "eff class ttjets = " << passedLClass[2]/((float)T[2]->GetEntries()) << endl;
  //
  cout << "highest significance (Punzi) = " << signPunziMax << endl;
  cout << "in the following bin: low Pt electron cut = "  << lowCut  << endl; 
  //
  cout << "eff eleID signal = " << passedEleID[signBinMax][0]/passedLClass[0] << endl;
  cout << "eff eleID wjets  = " << passedEleID[signBinMax][1]/passedLClass[1] << endl;
  cout << "eff eleID ttjets = " << passedEleID[signBinMax][2]/passedLClass[2] << endl;
  
}

bool isEleIDScan(float thisLike, float cutLike) {
  
  bool isPassed = false;
  float theCut = cutLike*0.01;
  if ( thisLike>=theCut ){ isPassed = true; }
  return isPassed;
}
