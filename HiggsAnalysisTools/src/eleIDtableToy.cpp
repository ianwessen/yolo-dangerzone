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
float HOverEMaxCut,  S9S25MaxCut,  DEtaMaxCut,  DPhiMaxCut,  SeeMaxCut,  EoPoutMaxCut;
float HOverEMinCut,  S9S25MinCut,  DEtaMinCut,  DPhiMinCut,  SeeMinCut,  EoPoutMinCut;
float HOverEMaxStep, S9S25MinStep, DEtaMaxStep, DPhiMaxStep, SeeMaxStep, SeeMinStep, EoPoutMaxStep, EoPoutMinStep;
float HOverEMaxInit, S9S25MinInit, DEtaMaxInit, DPhiMaxInit, SeeMaxInit, SeeMinInit, EoPoutMaxInit, EoPoutMinInit;

// methods
void setScanValue(int theClass);
bool isEleIDScan(float thisDeta, float thisDphi, float thisHoE, float thisS9s25, float thisEopOut, float thisSee, int deta, int dphi, int hoe, int s9s25, int eopmin, int eopmax, int seemin, int seemax);

// we apply loose electronID on the highest pt electron (since it's alrteady applied, we assume eff = 1)
// and we optimize eleId on the second one (the possible fake) when it belongs to the class we are analyzing

int main ( int argc, char **argv) {

  if (argc < 9){ 
    cout << "Argument missing! Insert: "                << std::endl; 
    cout << "1) inputFile - root tree for sgn "         << std::endl;
    cout << "2) signal preEleID efficiency"             << std::endl;
    cout << "3) signal kine efficiency"                 << std::endl;
    cout << "4) inputTextFile with W+jets root file,  pre eleID eff, kine eff, and xsec (pb)" << std::endl;
    cout << "5) inputTextFile with tt+jets root file, pre eleID eff, kine eff, and xsec (pb)" << std::endl;
    cout << "6) class to be analyzed:"                  << std::endl;
    cout << "   0 --> golden EB"                        << std::endl;
    cout << "   1 --> golden EE"                        << std::endl;
    cout << "   2 --> showering EB"                     << std::endl;
    cout << "   3 --> showering EE"                     << std::endl;
    cout << "7) luminosity per optimization (in pb-1) " << std::endl;
    cout << "8) discovery (1) or exclusion (0) limits"  << std::endl;
    return 0;
  }


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
  for(int ii=0; ii<2; ii++) std::cout << "Sample " << rootFileBkg[ii] << " has eff pre eleID = " << effPreEleIDBkg[ii] << " and x-sec = " << xsecBkg[ii] << " pb" << std::endl;                


  // reading the input trees for signal and backgrounds
  TChain *T[3];
  T[0]= new TChain("T1");   // signal
  T[1]= new TChain("T1");   // w+jets
  T[2]= new TChain("T1");   // ttbar
  T[0]->Add(argv[1]);      
  T[1]->Add(rootFileBkg[0].c_str());   
  T[2]->Add(rootFileBkg[1].c_str());   

  float L_deltaEta, L_deltaPhi;
  float L_hoe,      L_s9s25;
  float L_see,      L_eopOut;  
  int L_class;
  for(int ii=0; ii<3; ii++){
    T[ii]->SetMakeClass(1);
    T[ii]->SetBranchStatus("*",0);
    T[ii]->SetBranchStatus("L_deltaEta",1);
    T[ii]->SetBranchStatus("L_deltaPhi",1);
    T[ii]->SetBranchStatus("L_hoe",1);
    T[ii]->SetBranchStatus("L_s9s25",1);
    T[ii]->SetBranchStatus("L_see",1);
    T[ii]->SetBranchStatus("L_eopOut",1);
    T[ii]->SetBranchStatus("L_class",1);
    T[ii]->SetBranchAddress("L_deltaEta",&L_deltaEta);
    T[ii]->SetBranchAddress("L_deltaPhi",&L_deltaPhi);
    T[ii]->SetBranchAddress("L_hoe",&L_hoe);
    T[ii]->SetBranchAddress("L_s9s25",&L_s9s25);
    T[ii]->SetBranchAddress("L_see",&L_see);
    T[ii]->SetBranchAddress("L_eopOut",&L_eopOut);
    T[ii]->SetBranchAddress("L_class",&L_class);
  }


  // taking all other inputs -----------------------------------

  // electron class
  int theClass = atoi(argv[6]);
  setScanValue(theClass);

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
  
  // taking all other inputs -----------------------------------
  
  
  
  // counters
  float passedEleID[5][5][5][3][6][1][4][4][3];
  for(int ii=0; ii<3; ii++){
    for(int iiDeta=0; iiDeta<5; iiDeta++){
      for(int iiDphi=0; iiDphi<5; iiDphi++){
	for(int iiHoE=0; iiHoE<5; iiHoE++){
	  for(int iiS9S25=0; iiS9S25<3; iiS9S25++){
	    for(int iiEoPmin=0; iiEoPmin<6; iiEoPmin++){
	      for(int iiEoPmax=0; iiEoPmax<1; iiEoPmax++){
		for(int iiSeemin=0; iiSeemin<4; iiSeemin++){
		  for(int iiSeemax=0; iiSeemax<4; iiSeemax++){
		    passedEleID[iiDeta][iiDphi][iiHoE][iiS9S25][iiEoPmin][iiEoPmax][iiSeemin][iiSeemax][ii]=0.;
		  }}}}}}}}}

  float passedClass[3];
  for(int ii=0; ii<3; ii++){ passedClass[ii]=0.; }
  
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
      float thisClass = 4;
      if (L_class==0)   thisClass=0;
      if (L_class==100) thisClass=1;
      if (L_class>=30  && L_class<40)  thisClass=2;
      if (L_class>=130 && L_class<140) thisClass=3;
      if (thisClass != theClass) continue;
      passedClass[ii]++;

      // scan to compute the efficiencies for each bin
      bool theLowScan  = false;
      for(int iiDeta=0; iiDeta<5; iiDeta++){
	for(int iiDphi=0; iiDphi<5; iiDphi++){
	  for(int iiHoE=0; iiHoE<5; iiHoE++){
	    for(int iiS9S25=0; iiS9S25<3; iiS9S25++){
	      for(int iiEoPmin=0; iiEoPmin<6; iiEoPmin++){
		for(int iiEoPmax=0; iiEoPmax<1; iiEoPmax++){
		  for(int iiSeemin=0; iiSeemin<4; iiSeemin++){
		    for(int iiSeemax=0; iiSeemax<4; iiSeemax++){
		      theLowScan=isEleIDScan(L_deltaEta, L_deltaPhi, L_hoe, L_s9s25, L_eopOut, L_see, iiDeta, iiDphi, iiHoE, iiS9S25, iiEoPmin, iiEoPmax, iiSeemin, iiSeemax);
      		      if(theLowScan) passedEleID[iiDeta][iiDphi][iiHoE][iiS9S25][iiEoPmin][iiEoPmax][iiSeemin][iiSeemax][ii]=passedEleID[iiDeta][iiDphi][iiHoE][iiS9S25][iiEoPmin][iiEoPmax][iiSeemin][iiSeemax][ii]+1.;  
		      
		    }}}}}}}}
    } // loop over entries
  } // loop over signal / backgrounds 


  
  // maximization:
  ofstream *outTxtFile = new ofstream("outputFileScan.txt",ios::app);
  
  float signPunziMax = -999.;
  float effMax       = -999.;
  int signBinMax[8];
  for(int yy=0; yy<8; yy++){signBinMax[yy]=-1;}  

  for(int iiDeta=0; iiDeta<5; iiDeta++){
    for(int iiDphi=0; iiDphi<5; iiDphi++){
      for(int iiHoE=0; iiHoE<5; iiHoE++){
	for(int iiS9S25=0; iiS9S25<3; iiS9S25++){
	  for(int iiEoPmin=0; iiEoPmin<6; iiEoPmin++){
	    for(int iiEoPmax=0; iiEoPmax<1; iiEoPmax++){
	      for(int iiSeemin=0; iiSeemin<4; iiSeemin++){
		for(int iiSeemax=0; iiSeemax<4; iiSeemax++){

		  float thisClassSgnEff = passedClass[0]/((float)T[0]->GetEntries());
		  float thisBinSgnEff   = passedEleID[iiDeta][iiDphi][iiHoE][iiS9S25][iiEoPmin][iiEoPmax][iiSeemin][iiSeemax][0]/passedClass[0];
		  float effSgn          = sgnPreEleIDEff*thisClassSgnEff*thisBinSgnEff*sgnKineEff;
		                          
                  float thisClassBkgEff[2];
		  float thisBinBkgEff[2];
		  for(int ibkg=0; ibkg<2; ibkg++) {
		    thisClassBkgEff[ibkg] = passedClass[ibkg+1]/((float)T[ibkg+1]->GetEntries());
		    thisBinBkgEff[ibkg]   = passedEleID[iiDeta][iiDphi][iiHoE][iiS9S25][iiEoPmin][iiEoPmax][iiSeemin][iiSeemax][ibkg+1]/passedClass[ibkg+1];
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
		  *outTxtFile << iiDeta << " " << iiDphi << " " << iiHoE << " " << iiS9S25 << " " << iiEoPmin << " " << iiEoPmax << " " << iiSeemin << " " << iiSeemax << " " << thisBinSgnEff << " " << thisBinBkgEff[0] << " " << thisBinBkgEff[1] << " " << signPunzi << endl;
		  
		  // looking for the maximum
		  if (signPunzi>signPunziMax) { 
		    signPunziMax  = signPunzi; 
		    effMax        = thisBinSgnEff;
		    signBinMax[0] = iiDeta;
		    signBinMax[1] = iiDphi;
		    signBinMax[2] = iiHoE;
		    signBinMax[3] = iiS9S25;
		    signBinMax[4] = iiEoPmin;
		    signBinMax[5] = iiEoPmax;
		    signBinMax[6] = iiSeemin;
		    signBinMax[7] = iiSeemax;
		  }
		}}}}}}}}
  
  // max significance bin
  float DEtaMaxCut   = DEtaMaxInit   + signBinMax[0]*DEtaMaxStep;
  float DPhiMaxCut   = DPhiMaxInit   + signBinMax[1]*DPhiMaxStep;
  float HOverEMaxCut = HOverEMaxInit + signBinMax[2]*HOverEMaxStep;
  float S9S25MinCut  = S9S25MinInit  + signBinMax[3]*S9S25MinStep;
  float EoPoutMinCut = EoPoutMinInit + signBinMax[4]*EoPoutMinStep;
  float EoPoutMaxCut = EoPoutMaxInit + signBinMax[5]*EoPoutMaxStep;
  float SeeMinCut    = SeeMinInit    + signBinMax[6]*SeeMinStep;
  float SeeMaxCut    = SeeMaxInit    + signBinMax[7]*SeeMaxStep;

  // output
  cout << endl;
  cout << "highest significance (Punzi) = " << signPunziMax << endl;
  cout << "Class = " << theClass << endl;
  // 
  cout << "eff class signal = " << passedClass[0]/((float)T[0]->GetEntries()) << endl;
  cout << "eff class ttjets = " << passedClass[1]/((float)T[1]->GetEntries()) << endl;
  cout << "eff class wjets  = " << passedClass[2]/((float)T[2]->GetEntries()) << endl;
  //
  cout << "eff eleID signal = " << passedEleID[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][signBinMax[5]][signBinMax[6]][signBinMax[7]][0]/passedClass[0] << endl;
  cout << "eff eleID ttjets = " << passedEleID[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][signBinMax[5]][signBinMax[6]][signBinMax[7]][1]/passedClass[1] << endl;
  cout << "eff eleID wjets  = " << passedEleID[signBinMax[0]][signBinMax[1]][signBinMax[2]][signBinMax[3]][signBinMax[4]][signBinMax[5]][signBinMax[6]][signBinMax[7]][2]/passedClass[2] << endl;
  // 
  cout << "in the following bin: " << endl;
  cout << "|deta| = " << "0 - " << DEtaMaxCut   << endl; 
  cout << "|dphi| = " << "0 - " << DPhiMaxCut   << endl;
  cout << "H/E  = "   << "0 - " << HOverEMaxCut << endl;
  cout << "S9/S25 = " << S9S25MinCut  << " - 1" << endl; 
  cout << "E/Pout = " << EoPoutMinCut << " - " << EoPoutMaxCut << endl;
  cout << "See = "    << SeeMinCut    << " - " << SeeMaxCut    << endl;
}


void setScanValue(int theClass){

  // golden barrel
  if (theClass==0){     
    // starting point           // step
    DEtaMaxInit   = 0.002;      DEtaMaxStep   = 0.001;
    DPhiMaxInit   = 0.002;      DPhiMaxStep   = 0.003;   
    HOverEMaxInit = 0.03;       HOverEMaxStep = 0.01;   
    S9S25MinInit  = 0.6;        S9S25MinStep  = 0.1; 
    SeeMaxInit    = 0.007;      SeeMaxStep    = 0.003;   
    SeeMinInit    = 0.0;        SeeMinStep    = 0.003;   
    EoPoutMaxInit = 1.9;        EoPoutMaxStep = 0.03;    
    EoPoutMinInit = 0.4;        EoPoutMinStep = 0.1;  
  }

  // golden endcap
  if (theClass==1){     
    DEtaMaxInit   = 0.002;      DEtaMaxStep   = 0.001;
    DPhiMaxInit   = 0.002;      DPhiMaxStep   = 0.003;   
    HOverEMaxInit = 0.03;       HOverEMaxStep = 0.01;   
    S9S25MinInit  = 0.6;        S9S25MinStep  = 0.1; 
    SeeMaxInit    = 0.025;      SeeMaxStep    = 0.003;   
    SeeMinInit    = 0.0;        SeeMinStep    = 0.003;   
    EoPoutMaxInit = 1.9;        EoPoutMaxStep = 0.03;    
    EoPoutMinInit = 0.4;        EoPoutMinStep = 0.1;  
  }

  // showering barrel
  if (theClass==2){     
    DEtaMaxInit   = 0.002;      DEtaMaxStep   = 0.001;
    DPhiMaxInit   = 0.025;      DPhiMaxStep   = 0.01;   
    HOverEMaxInit = 0.03;       HOverEMaxStep = 0.01;   
    S9S25MinInit  = 0.6;        S9S25MinStep  = 0.1; 
    SeeMaxInit    = 0.007;      SeeMaxStep    = 0.003;   
    SeeMinInit    = 0.0;        SeeMinStep    = 0.003;   
    EoPoutMaxInit = 999.;       EoPoutMaxStep = 1.0;    
    EoPoutMinInit = 0.4;        EoPoutMinStep = 0.1;  
  }

  // showering endcap
  if (theClass==3){     
    DEtaMaxInit   = 0.002;      DEtaMaxStep   = 0.001;
    DPhiMaxInit   = 0.025;      DPhiMaxStep   = 0.01;   
    HOverEMaxInit = 0.03;       HOverEMaxStep = 0.01;   
    S9S25MinInit  = 0.6;        S9S25MinStep  = 0.1; 
    SeeMaxInit    = 0.025;      SeeMaxStep    = 0.003;   
    SeeMinInit    = 0.002;      SeeMinStep    = 0.003;   
    EoPoutMaxInit = 999.;       EoPoutMaxStep = 1.0;    
    EoPoutMinInit = 0.4;        EoPoutMinStep = 0.1;  
  }
}

bool isEleIDScan(float thisDeta, float thisDphi, float thisHoE, float thisS9s25, float thisEopOut, float thisSee, int deta, int dphi, int hoe, int s9s25, int eopmin, int eopmax, int seemin, int seemax) {

  // the current cut
  float DEtaMaxCut   = DEtaMaxInit   + deta*DEtaMaxStep;
  float DPhiMaxCut   = DPhiMaxInit   + dphi*DPhiMaxStep;
  float HOverEMaxCut = HOverEMaxInit + hoe*HOverEMaxStep;
  float S9S25MinCut  = S9S25MinInit  + s9s25*S9S25MinStep;
  float EoPoutMaxCut = EoPoutMaxInit + eopmax*EoPoutMaxStep;
  float EoPoutMinCut = EoPoutMinInit + eopmin*EoPoutMinStep;
  float SeeMaxCut    = SeeMaxInit    + seemax*SeeMaxStep;
  float SeeMinCut    = SeeMinInit    + seemin*SeeMinStep;
  
  bool isPassed = false;
  if ( thisDeta<=DEtaMaxCut ){
    if ( thisDphi<=DPhiMaxCut ){
      if ( fabs(thisHoE)<=HOverEMaxCut ){
	if ( thisS9s25>=S9S25MinCut ) {
	  if( (thisEopOut<=EoPoutMaxCut) && (thisEopOut>=EoPoutMinCut) ){
	    if( (thisSee<=SeeMaxCut) && (thisSee>=SeeMinCut) ){
	      isPassed = true;
	    }}}}}}

    return isPassed;
}
