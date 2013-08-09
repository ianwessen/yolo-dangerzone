// Electron studies code

//! c++ includes              
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream.h>
#include <unistd.h>
#include <fstream>
#include <math.h>

//! ROOT includes        
#include "TROOT.h"
#include "TSystem.h"
#include "TBranch.h"
#include "TTree.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TFile.h"

//! vectors
#include <TVector3.h>
#include <TLorentzVector.h>

// Main program
int main ( int argc, char **argv)
{ 
  char inputRootFileName[150];
  char outputRootFileName[150];
  char outputFileName[150];
  int  mySample = 0;
  
  // input parameters
  if (argc==4){
    strcpy(inputRootFileName,argv[1]); 
    strcpy(outputFileName,argv[2]);
    mySample = atoi(argv[3]);
  }
  else { 
    cout << " " << endl;
    cout << "   Input: 1) input file with (full) xanatrees name" << endl;
    cout << "          2) output txt and root file name "        << endl;
    cout << "          3) sample: 1 = higgs signal, 2 = w+jet"   << endl;
    cout << endl;
    return 1; 
  }
  
  // ----------------------------------------------
  // electron ID selection
  float hoeCut[8];           
  float dEtaCut[8];
  float dPhiVtxCut[8];
  float eopOutCut[8];
  float e9e25Cut[8];
  float seeInfCut[8];
  float seeSupCut[8];
  float fisherInfCut[8];

  // golden, barrel                  // narrow, barrel	   
  hoeCut[0]       = 0.028;	     hoeCut[1]       = 0.05;  
  dEtaCut[0]      = 0.0026;	     dEtaCut[1]      = 0.004; 
  dPhiVtxCut[0]   = 0.0051;	     dPhiVtxCut[1]   = 0.02;  
  eopOutCut[0]    = 0.972;	     eopOutCut[1]    = 0.75;  
  seeInfCut[0]    = 0.005;	     seeInfCut[1]    = 0.005;  
  seeSupCut[0]    = 0.0114;	     seeSupCut[1]    = 0.011;  
  e9e25Cut[0]     = 0.71;	     e9e25Cut[1]     = 0.75;  
  fisherInfCut[0] = 1.05;	     fisherInfCut[1] = 1.05;

  // bb, barrel                      // showering, barrel	   
  hoeCut[2]       = 0.05;	     hoeCut[3]       = 0.026;  
  dEtaCut[2]      = 0.004;	     dEtaCut[3]      = 0.0043; 
  dPhiVtxCut[2]   = 0.03;	     dPhiVtxCut[3]   = 0.030;  
  eopOutCut[2]    = 0.75;	     eopOutCut[3]    = 0.950;  
  seeInfCut[2]    = 0.05;	     seeInfCut[3]    = 0.004123;  
  seeSupCut[2]    = 0.011;	     seeSupCut[3]    = 0.00964365;  
  e9e25Cut[2]     = 0.65;	     e9e25Cut[3]     = 0.65;  
  fisherInfCut[2] = 1.05;	     fisherInfCut[3] = 1.24;
  
  // golden, endcap                  // narrow, endcap	   
  hoeCut[4]       = 0.039;	     hoeCut[5]       = 0.07;  
  dEtaCut[4]      = 0.0039;	     dEtaCut[5]      = 0.005; 
  dPhiVtxCut[4]   = 0.015;	     dPhiVtxCut[5]   = 0.04;  
  eopOutCut[4]    = 0.520;	     eopOutCut[5]    = 0.5;  
  seeInfCut[4]    = 0.0076811;	     seeInfCut[5]    = 0.008;  
  seeSupCut[4]    = 0.0282843;	     seeSupCut[5]    = 0.03;  
  e9e25Cut[4]     = 0.82;	     e9e25Cut[5]     = 0.70;  
  fisherInfCut[4] = 1.10;	     fisherInfCut[5] = 1.10;

  // bb, endcap                      // showering, endcap
  hoeCut[6]       = 0.07;	     hoeCut[7]       = 0.007;  
  dEtaCut[6]      = 0.005;	     dEtaCut[7]      = 0.029; 
  dPhiVtxCut[6]   = 0.04;	     dPhiVtxCut[7]   = 0.018;  
  eopOutCut[6]    = 0.8;	     eopOutCut[7]    = 0.697;  
  seeInfCut[6]    = 0.008;	     seeInfCut[7]    = 0.00458;  
  seeSupCut[6]    = 0.03;	     seeSupCut[7]    = 0.023875;  
  e9e25Cut[6]     = 0.70;	     e9e25Cut[7]     = 0.42;  
  fisherInfCut[6] = 1.10;	     fisherInfCut[7] = 1.70;


  // ---------------------------------------------
  // output files
  char outputDir0[150];  
  strcpy(outputDir0,"./");
  char outputRootFile[150];  
  strcpy(outputRootFile,outputDir0);
  strcat(outputRootFile,outputFileName);
  strcat(outputRootFile,".root");
  char outputNevFile[150];   
  strcpy(outputNevFile,outputDir0);
  strcat(outputNevFile,outputFileName);
  strcat(outputNevFile,".txt");


  // ------------------------------------------
  // reading the list of input files with trees and building the chain
  TChain *T = new TChain("ntp1");
  char Buffer[500];
  char MyXanaFile[2000];  
  cout << "input: " << inputRootFileName << endl;
  ifstream *inputFile = new ifstream(inputRootFileName);
  while( !(inputFile->eof()) ){
    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
      sscanf(Buffer,"%s",MyXanaFile);
      T->Add(MyXanaFile);
      cout << "chaining file " << MyXanaFile << endl;
    }
  }
  inputFile->close();
  delete inputFile;
  


  // --------------------------------------------------
  // declaration of leaves types
  float pMc[101],  thetaMc[101], etaMc[101], phiMc[101]; 
  int idMc[101], mothMc[101];    
  char eTrg, singleElePassedTrg, singleEleRelaxPassedTrg; 
  char doubleElePassedTrg, doubleEleRelaxPassedTrg; 
  int nEle;
  int chargeEle[50];  
  float energyEle[50], etEle[50];   
  float momentumEle[50], thetaEle[50];  
  float etaEle[50], phiEle[50];
  float latEle[50], a20Ele[50];   
  float s1s9Ele[50], s9s25Ele[50];   
  float covEtaEtaEle[50], covPhiPhiEle[50];   
  int eleClassEle[50];  
  float eleHoEEle[50];  
  float eleCorrEoPEle[50], eleNotCorrEoPEle[50];   
  float eleCorrEoPoutEle[50], eleNotCorrEoPoutEle[50];   
  float eleDeltaEtaAtVtxEle[50], eleDeltaPhiAtVtxEle[50];   
  float eleDeltaEtaAtCaloEle[50], eleDeltaPhiAtCaloEle[50];   
  float eleTrackerIso_minDREle[50];   
  float eleTrackerIso_minDR_vetoEle[50]; 
  float eleTrackerIso_sumPtEle[50];   
  float eleCaloIso_minDREle[50];   
  float eleCaloIso_sumPtEle[50];   
  float eleLikelihoodEle[50]; 
  

  // setting branch addresses
  T->SetMakeClass(1);  
  T->SetBranchStatus("*",0);
  T->SetBranchStatus("pMc",1);
  T->SetBranchStatus("thetaMc",1);
  T->SetBranchStatus("etaMc",1); 
  T->SetBranchStatus("phiMc",1);  
  T->SetBranchStatus("idMc",1);   
  T->SetBranchStatus("mothMc",1);   
  T->SetBranchStatus("singleElePassedTrg",1);      
  T->SetBranchStatus("singleEleRelaxPassedTrg",1); 
  T->SetBranchStatus("doubleElePassedTrg",1);      
  T->SetBranchStatus("doubleEleRelaxPassedTrg",1); 
  T->SetBranchStatus("nEle",1);
  T->SetBranchStatus("chargeEle", 1);
  T->SetBranchStatus("energyEle", 1);
  T->SetBranchStatus("etEle", 1);
  T->SetBranchStatus("momentumEle", 1);
  T->SetBranchStatus("thetaEle", 1);
  T->SetBranchStatus("etaEle",1); 
  T->SetBranchStatus("phiEle",1);
  T->SetBranchStatus("latEle",1);
  T->SetBranchStatus("a20Ele",1);
  T->SetBranchStatus("s1s9Ele",1);
  T->SetBranchStatus("s9s25Ele",1);
  T->SetBranchStatus("covEtaEtaEle", 1);
  T->SetBranchStatus("covPhiPhiEle", 1);
  T->SetBranchStatus("eleClassEle",1);   
  T->SetBranchStatus("eleHoEEle",1);      
  T->SetBranchStatus("eleCorrEoPEle",1);  
  T->SetBranchStatus("eleNotCorrEoPEle",1); 
  T->SetBranchStatus("eleCorrEoPoutEle",1);
  T->SetBranchStatus("eleNotCorrEoPoutEle",1); 
  T->SetBranchStatus("eleDeltaEtaAtVtxEle",1); 
  T->SetBranchStatus("eleDeltaPhiAtVtxEle",1); 
  T->SetBranchStatus("eleDeltaEtaAtCaloEle",1); 
  T->SetBranchStatus("eleDeltaPhiAtCaloEle",1); 
  T->SetBranchStatus("eleTrackerIso_minDREle",1); 
  T->SetBranchStatus("eleTrackerIso_minDR_vetoEle",1); 
  T->SetBranchStatus("eleTrackerIso_sumPtEle",1);
  T->SetBranchStatus("eleCaloIso_minDREle",1);
  T->SetBranchStatus("eleCaloIso_sumPtEle",1);
  T->SetBranchStatus("eleLikelihoodEle",1);
  // 
  T->SetBranchAddress("pMc", pMc);
  T->SetBranchAddress("thetaMc", thetaMc);
  T->SetBranchAddress("etaMc", etaMc);
  T->SetBranchAddress("phiMc", phiMc);
  T->SetBranchAddress("mothMc", mothMc);
  T->SetBranchAddress("idMc", idMc);
  T->SetBranchAddress("singleElePassedTrg", &singleElePassedTrg);
  T->SetBranchAddress("singleEleRelaxPassedTrg", &singleEleRelaxPassedTrg);
  T->SetBranchAddress("doubleElePassedTrg", &doubleElePassedTrg);
  T->SetBranchAddress("doubleEleRelaxPassedTrg", &doubleEleRelaxPassedTrg);
  T->SetBranchAddress("nEle", &nEle);
  T->SetBranchAddress("chargeEle", chargeEle);
  T->SetBranchAddress("energyEle", energyEle);
  T->SetBranchAddress("etEle", etEle);
  T->SetBranchAddress("momentumEle", momentumEle);
  T->SetBranchAddress("thetaEle", thetaEle);
  T->SetBranchAddress("etaEle", etaEle);
  T->SetBranchAddress("phiEle", phiEle);
  T->SetBranchAddress("a20Ele", a20Ele);
  T->SetBranchAddress("latEle", latEle);
  T->SetBranchAddress("s1s9Ele", s1s9Ele);
  T->SetBranchAddress("s9s25Ele", s9s25Ele);
  T->SetBranchAddress("covEtaEtaEle", covEtaEtaEle);
  T->SetBranchAddress("covPhiPhiEle", covPhiPhiEle);
  T->SetBranchAddress("eleClassEle", eleClassEle);
  T->SetBranchAddress("eleHoEEle", eleHoEEle);
  T->SetBranchAddress("eleCorrEoPEle", eleCorrEoPEle);
  T->SetBranchAddress("eleNotCorrEoPEle", eleNotCorrEoPEle);
  T->SetBranchAddress("eleCorrEoPoutEle", eleCorrEoPoutEle);
  T->SetBranchAddress("eleNotCorrEoPoutEle", eleNotCorrEoPoutEle);
  T->SetBranchAddress("eleDeltaEtaAtVtxEle", eleDeltaEtaAtVtxEle);
  T->SetBranchAddress("eleDeltaPhiAtVtxEle", eleDeltaPhiAtVtxEle);
  T->SetBranchAddress("eleDeltaEtaAtCaloEle", eleDeltaEtaAtCaloEle);
  T->SetBranchAddress("eleDeltaPhiAtCaloEle", eleDeltaPhiAtCaloEle);
  T->SetBranchAddress("eleTrackerIso_minDREle", eleTrackerIso_minDREle);
  T->SetBranchAddress("eleTrackerIso_minDR_vetoEle", eleTrackerIso_minDR_vetoEle);
  T->SetBranchAddress("eleTrackerIso_sumPtEle", eleTrackerIso_sumPtEle);
  T->SetBranchAddress("eleCaloIso_minDREle", eleCaloIso_minDREle);
  T->SetBranchAddress("eleCaloIso_sumPtEle", eleCaloIso_sumPtEle);
  T->SetBranchAddress("eleLikelihoodEle", eleLikelihoodEle);

  // -------------------------------
  // counters: total number of good events
  int numEveOk = 0;

  // counters: total number of ele/jets per class
  int allallFW  = 0;
  int allallNFW = 0;
  int allFW[8],        allNFW[8];
  int hoeOkFW[8],      hoeOkNFW[8];
  int e9e25OkFW[8],    e9e25OkNFW[8];
  int dEtaOkFW[8],     dEtaOkNFW[8];
  int dPhiVtxOkFW[8],  dPhiVtxOkNFW[8]; 
  int seeOkFW[8],      seeOkNFW[8];
  int eopoutOkFW[8],   eopoutOkNFW[8];
  int fisherOkFW[8],   fisherOkNFW[8];
  for (int ii = 0; ii<8; ii++){
    allFW[ii]        = 0;  allNFW[ii]        = 0;
    hoeOkFW[ii]      = 0;  hoeOkNFW[ii]      = 0;
    e9e25OkFW[ii]    = 0;  e9e25OkNFW[ii]    = 0;
    dEtaOkFW[ii]     = 0;  dEtaOkNFW[ii]     = 0;
    dPhiVtxOkFW[ii]  = 0;  dPhiVtxOkNFW[ii]  = 0; 
    seeOkFW[ii]      = 0;  seeOkNFW[ii]      = 0;
    eopoutOkFW[ii]   = 0;  eopoutOkNFW[ii]   = 0;
    fisherOkFW[ii]   = 0;  fisherOkNFW[ii]   = 0;
  }

  // counters: total number of ele/jets all classes together
  int hoetOkFW     = 0;    int hoetOkNFW     = 0;
  int e9e25tOkFW   = 0;	   int e9e25tOkNFW   = 0;
  int dEtatOkFW    = 0;	   int dEtatOkNFW    = 0;
  int dPhiVtxtOkFW = 0;	   int dPhiVtxtOkNFW = 0;
  int seetOkFW     = 0;	   int seetOkNFW     = 0;
  int eopouttOkFW  = 0;	   int eopouttOkNFW  = 0;
  int fishertOkFW  = 0;	   int fishertOkNFW  = 0;

  
  // ---------------------------------------------
  // Histos initialization
  
  // histos with eleID distributions 
  TH1F *HseeFW[8],          *HseeNFW[8];
  TH1F *HhoeFW[8],          *HhoeNFW[8];
  TH1F *He9e25FW[8],        *He9e25NFW[8];
  TH1F *HEoPoutFW[8],       *HEoPoutNFW[8];
  TH1F *HdPhiVtxFW[8],      *HdPhiVtxNFW[8];
  TH1F *HdEtaVtxFW[8],      *HdEtaVtxNFW[8];
  TH1F *HfisherFW[8],       *HfisherNFW[8];
  // after trigger
  TH1F *HseeFW_hlt[8],      *HseeNFW_hlt[8];
  TH1F *HhoeFW_hlt[8],      *HhoeNFW_hlt[8];
  TH1F *He9e25FW_hlt[8],    *He9e25NFW_hlt[8];
  TH1F *HEoPoutFW_hlt[8],   *HEoPoutNFW_hlt[8];
  TH1F *HdPhiVtxFW_hlt[8],  *HdPhiVtxNFW_hlt[8];
  TH1F *HdEtaVtxFW_hlt[8],  *HdEtaVtxNFW_hlt[8];
  TH1F *HfisherFW_hlt[8],   *HfisherNFW_hlt[8];
  // after isolation
  TH1F *HseeFW_iso[8],      *HseeNFW_iso[8];
  TH1F *HhoeFW_iso[8],      *HhoeNFW_iso[8];
  TH1F *He9e25FW_iso[8],    *He9e25NFW_iso[8];
  TH1F *HEoPoutFW_iso[8],   *HEoPoutNFW_iso[8];
  TH1F *HdPhiVtxFW_iso[8],  *HdPhiVtxNFW_iso[8];
  TH1F *HdEtaVtxFW_iso[8],  *HdEtaVtxNFW_iso[8];
  TH1F *HfisherFW_iso[8],   *HfisherNFW_iso[8];
  // after trigger and isolation
  TH1F *HseeFW_hlt_iso[8],      *HseeNFW_hlt_iso[8];
  TH1F *HhoeFW_hlt_iso[8],      *HhoeNFW_hlt_iso[8];
  TH1F *He9e25FW_hlt_iso[8],    *He9e25NFW_hlt_iso[8];
  TH1F *HEoPoutFW_hlt_iso[8],   *HEoPoutNFW_hlt_iso[8];
  TH1F *HdPhiVtxFW_hlt_iso[8],  *HdPhiVtxNFW_hlt_iso[8];
  TH1F *HdEtaVtxFW_hlt_iso[8],  *HdEtaVtxNFW_hlt_iso[8];
  TH1F *HfisherFW_hlt_iso[8],   *HfisherNFW_hlt_iso[8];

  char name[100];
  for (int ii=0; ii<8; ii++){
    sprintf (name,  "HseeFW[%d]", ii);
    HseeFW[ii]       = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeFW[ii]       ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeFW[%d]", ii);
    HhoeFW[ii]       = new TH1F(name,  "H/E",                   50,-0.1,0.1); 
    HhoeFW[ii]       ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25FW[%d]", ii);
    He9e25FW[ii]     = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25FW[ii]     ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutFW[%d]", ii);
    HEoPoutFW[ii]    = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutFW[ii]    ->GetXaxis()->SetTitle("E_{rec}/P_{out}");    
    sprintf (name,  "HdPhiVtxFW[%d]", ii);
    HdPhiVtxFW[ii]   = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxFW[ii]   ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxFW[%d]", ii);
    HdEtaVtxFW[ii]   = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxFW[ii]   ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherFW[%d]", ii);
    HfisherFW[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherFW[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeFW_hlt[%d]", ii);
    HseeFW_hlt[ii]       = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeFW_hlt[ii]       ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeFW_hlt[%d]", ii);
    HhoeFW_hlt[ii]       = new TH1F(name,  "H/E",                   50,-0.1,0.1); 
    HhoeFW_hlt[ii]       ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25FW_hlt[%d]", ii);
    He9e25FW_hlt[ii]     = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25FW_hlt[ii]     ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutFW_hlt[%d]", ii);
    HEoPoutFW_hlt[ii]    = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutFW_hlt[ii]    ->GetXaxis()->SetTitle("E_{rec}/P_{out}");    
    sprintf (name,  "HdPhiVtxFW_hlt[%d]", ii);
    HdPhiVtxFW_hlt[ii]   = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxFW_hlt[ii]   ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxFW_hlt[%d]", ii);
    HdEtaVtxFW_hlt[ii]   = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxFW_hlt[ii]   ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherFW_hlt[%d]", ii);
    HfisherFW_hlt[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherFW_hlt[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeFW_iso[%d]", ii);
    HseeFW_iso[ii]       = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeFW_iso[ii]       ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeFW_iso[%d]", ii);
    HhoeFW_iso[ii]       = new TH1F(name,  "H/E",                   50,-0.1,0.1); 
    HhoeFW_iso[ii]       ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25FW_iso[%d]", ii);
    He9e25FW_iso[ii]     = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25FW_iso[ii]     ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutFW_iso[%d]", ii);
    HEoPoutFW_iso[ii]    = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutFW_iso[ii]    ->GetXaxis()->SetTitle("E_{rec}/P_{out}");    
    sprintf (name,  "HdPhiVtxFW_iso[%d]", ii);
    HdPhiVtxFW_iso[ii]   = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxFW_iso[ii]   ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxFW_iso[%d]", ii);
    HdEtaVtxFW_iso[ii]   = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxFW_iso[ii]   ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherFW_iso[%d]", ii);
    HfisherFW_iso[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherFW_iso[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeFW_hlt_iso[%d]", ii);
    HseeFW_hlt_iso[ii]       = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeFW_hlt_iso[ii]       ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeFW_hlt_iso[%d]", ii);
    HhoeFW_hlt_iso[ii]       = new TH1F(name,  "H/E",                   50,-0.1,0.1); 
    HhoeFW_hlt_iso[ii]       ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25FW_hlt_iso[%d]", ii);
    He9e25FW_hlt_iso[ii]     = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25FW_hlt_iso[ii]     ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutFW_hlt_iso[%d]", ii);
    HEoPoutFW_hlt_iso[ii]    = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutFW_hlt_iso[ii]    ->GetXaxis()->SetTitle("E_{rec}/P_{out}");    
    sprintf (name,  "HdPhiVtxFW_hlt_iso[%d]", ii);
    HdPhiVtxFW_hlt_iso[ii]   = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxFW_hlt_iso[ii]   ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxFW_hlt_iso[%d]", ii);
    HdEtaVtxFW_hlt_iso[ii]   = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxFW_hlt_iso[ii]   ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherFW_hlt_iso[%d]", ii);
    HfisherFW_hlt_iso[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherFW_hlt_iso[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeNFW[%d]", ii);
    HseeNFW[ii]      = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeNFW[ii]      ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeNFW[%d]", ii);
    HhoeNFW[ii]      = new TH1F(name,  "H/E",                  100,-0.1,0.1); 
    HhoeNFW[ii]      ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25NFW[%d]", ii);
    He9e25NFW[ii]    = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25NFW[ii]    ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutNFW[%d]", ii);
    HEoPoutNFW[ii]   = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutNFW[ii]   ->GetXaxis()->SetTitle("E_{rec}/P");    
    sprintf (name,  "HdPhiVtxNFW[%d]", ii);
    HdPhiVtxNFW[ii]  = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxNFW[ii]  ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxNFW[%d]", ii);
    HdEtaVtxNFW[ii]  = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxNFW[ii]  ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherNFW[%d]", ii);
    HfisherNFW[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherNFW[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeNFW_hlt[%d]", ii);
    HseeNFW_hlt[ii]      = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeNFW_hlt[ii]      ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeNFW_hlt[%d]", ii);
    HhoeNFW_hlt[ii]      = new TH1F(name,  "H/E",                  100,-0.1,0.1); 
    HhoeNFW_hlt[ii]      ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25NFW_hlt[%d]", ii);
    He9e25NFW_hlt[ii]    = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25NFW_hlt[ii]    ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutNFW_hlt[%d]", ii);
    HEoPoutNFW_hlt[ii]   = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutNFW_hlt[ii]   ->GetXaxis()->SetTitle("E_{rec}/P");    
    sprintf (name,  "HdPhiVtxNFW_hlt[%d]", ii);
    HdPhiVtxNFW_hlt[ii]  = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxNFW_hlt[ii]  ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxNFW_hlt[%d]", ii);
    HdEtaVtxNFW_hlt[ii]  = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxNFW_hlt[ii]  ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherNFW_hlt[%d]", ii);
    HfisherNFW_hlt[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherNFW_hlt[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeNFW_iso[%d]", ii);
    HseeNFW_iso[ii]      = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeNFW_iso[ii]      ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeNFW_iso[%d]", ii);
    HhoeNFW_iso[ii]      = new TH1F(name,  "H/E",                  100,-0.1,0.1); 
    HhoeNFW_iso[ii]      ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25NFW_iso[%d]", ii);
    He9e25NFW_iso[ii]    = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25NFW_iso[ii]    ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutNFW_iso[%d]", ii);
    HEoPoutNFW_iso[ii]   = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutNFW_iso[ii]   ->GetXaxis()->SetTitle("E_{rec}/P");    
    sprintf (name,  "HdPhiVtxNFW_iso[%d]", ii);
    HdPhiVtxNFW_iso[ii]  = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxNFW_iso[ii]  ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxNFW_iso[%d]", ii);
    HdEtaVtxNFW_iso[ii]  = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxNFW_iso[ii]  ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherNFW_iso[%d]", ii);
    HfisherNFW_iso[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherNFW_iso[ii]   ->GetXaxis()->SetTitle("fisher");    

    sprintf (name,  "HseeNFW_hlt_iso[%d]", ii);
    HseeNFW_hlt_iso[ii]      = new TH1F(name,  "#sigma_{#eta #eta}",   100,0.,0.05); 
    HseeNFW_hlt_iso[ii]      ->GetXaxis()->SetTitle("sigma_{#eta #eta}");    
    sprintf (name,  "HhoeNFW_hlt_iso[%d]", ii);
    HhoeNFW_hlt_iso[ii]      = new TH1F(name,  "H/E",                  100,-0.1,0.1); 
    HhoeNFW_hlt_iso[ii]      ->GetXaxis()->SetTitle("H/E");    
    sprintf (name,  "He9e25NFW_hlt_iso[%d]", ii);
    He9e25NFW_hlt_iso[ii]    = new TH1F(name,  "E9/E25",               100,0.,2.);
    He9e25NFW_hlt_iso[ii]    ->GetXaxis()->SetTitle("E9/E25");    
    sprintf (name,  "HEoPoutNFW_hlt_iso[%d]", ii);
    HEoPoutNFW_hlt_iso[ii]   = new TH1F(name,  "E/P_{out}",            100,0.,4.); 
    HEoPoutNFW_hlt_iso[ii]   ->GetXaxis()->SetTitle("E_{rec}/P");    
    sprintf (name,  "HdPhiVtxNFW_hlt_iso[%d]", ii);
    HdPhiVtxNFW_hlt_iso[ii]  = new TH1F(name,  "#Delta #phi vertex",   100,-0.1,0.1); 
    HdPhiVtxNFW_hlt_iso[ii]  ->GetXaxis()->SetTitle("#Delta #Phi_{in} (rad)");    
    sprintf (name,  "HdEtaVtxNFW_hlt_iso[%d]", ii);
    HdEtaVtxNFW_hlt_iso[ii]  = new TH1F(name,  "#Delta #eta vertex",   100,-0.02,0.02); 
    HdEtaVtxNFW_hlt_iso[ii]  ->GetXaxis()->SetTitle("#Delta #eta_{in} (rad)");    
    sprintf (name,  "HfisherNFW_hlt_iso[%d]", ii);
    HfisherNFW_hlt_iso[ii]   = new TH1F(name,  "fisher discriminant",   100,-15,15); 
    HfisherNFW_hlt_iso[ii]   ->GetXaxis()->SetTitle("fisher");    
  }

  // histos for checks
  TH1F *H_PoPtrue = new TH1F("H_PoPtrue", "P/Ptrue", 100,0.,2.); 
  H_PoPtrue ->GetXaxis()->SetTitle("P/P_{TRUE}");


  // -------------------------------------------------------------------------------------    
  // -------------------------------------------------------------------------------------    

  // loop on the events
  float nEnt = T->GetEntries();
  cout << endl;
  cout << "Total number of events in loop is " << nEnt << endl; 
  cout << endl;
  for (int entry=0; entry<nEnt; entry++) { 
    
    // charging the value in the branches
    T -> GetEntry(entry);

    if (entry%1000 == 0){ cout << "entry = " << entry << endl; }

    // only signal or W+jets events with electrons from W at |eta|<2.5 
    bool sampleOk = false;
    int indeminus=999, indeplus=999;
    if(mySample==1){
      for(int imc=6;imc<25;imc++) {
	if(idMc[imc]== 11 && idMc[mothMc[imc]]==-24) indeminus=imc;
	if(idMc[imc]==-11 && idMc[mothMc[imc]]==24)  indeplus=imc;
      }
      sampleOk = ( indeminus<25 && indeplus<25 &&
		   (idMc[indeminus]*idMc[indeplus]<0) &&
		   (fabs(etaMc[indeminus])<2.5) && (fabs(etaMc[indeplus])<2.5));
    }
    if(mySample==2){
      sampleOk = ( (abs(idMc[8])==11) && (abs(idMc[9])==12) && (fabs(etaMc[8])<2.5));
    }
    if (!sampleOk){ continue; }
    numEveOk++;


    // single or double electron trigger
    bool okTrigger = false;
    if (singleElePassedTrg || doubleElePassedTrg || singleEleRelaxPassedTrg || doubleEleRelaxPassedTrg ){ okTrigger = true; } 
    

    // taking generated e+e- from W
    int genEle = -1000; 
    int genPos = -1000; 
    if(mySample==1){ genEle=indeminus; genPos=indeplus; }
    if(mySample==2){
      if (idMc[8] == 11) { genEle = 8; }
      if (idMc[8] == -11){ genPos = 8; }
    }

    // looking for the electrons which best match the generated ones from W [dR, charge match, E/P]
    int recEle = -1000;  
    int recPos = -1000;  
    if (nEle > 0){
      float bestPoPele = 1000.;
      float bestPoPpos = 1000.;
      
      for (int thisEle = 0; thisEle < nEle; thisEle++){
	TVector3 p3Rec;
	p3Rec.SetMagThetaPhi(momentumEle[thisEle], thetaEle[thisEle], phiEle[thisEle]);       

	// electrons
	if ( (chargeEle[thisEle]<0) && (genEle>-10)){
	  TVector3 p3Gen;
	  p3Gen.SetMagThetaPhi(pMc[genEle], thetaMc[genEle], phiMc[genEle]); 
	  float myDr = p3Rec.DeltaR(p3Gen);
	  if ( fabs(myDr)<0.05 ){
	    float PoPtrue = momentumEle[thisEle]/pMc[genEle];
	    if ( (fabs(PoPtrue-1)< fabs(bestPoPele-1)) ){recEle = thisEle; bestPoPele = PoPtrue;}
	  }}
	
	// positrons
	if ( (chargeEle[thisEle]>0) && (genPos>-10)){
	  TVector3 p3Gen;
	  p3Gen.SetMagThetaPhi(pMc[genPos], thetaMc[genPos], phiMc[genPos]); 
	  float myDr = p3Rec.DeltaR(p3Gen);
	  if ( fabs(myDr)<0.05 ){
	    float PoPtrue = momentumEle[thisEle]/pMc[genPos];
	    if ( (fabs(PoPtrue-1)< fabs(bestPoPpos-1)) ){recPos = thisEle; bestPoPpos = PoPtrue;}
	  }}
      }
    }

    // from here: 
    // genEle,genPos = generated ele - pos from W 
    // recEle,recPos = reconstructed ele - pos with best natch wtih those generated from W       
    // I can use all other reconstructed electrons as fakes
    
    // some general distributions to check and to compute efficiencies
    if (genEle > -10){
      if (recEle > -10){ H_PoPtrue -> Fill(momentumEle[recEle]/pMc[genEle]);
      }}
    if (genPos > -10){
      if (recPos > -10){ H_PoPtrue -> Fill(momentumEle[recPos]/pMc[genPos]);
      }}

    // eleID study: SIGNAL
    int myIndexEle = 10;
    int myIndexPos = 10;
    if (recEle > -10){
      
      // all
      allallFW++;

      // others
      if (eleClassEle[recEle] == 0)  {myIndexEle = 0;}
      if (eleClassEle[recEle] == 10) {myIndexEle = 1;}
      if (eleClassEle[recEle] == 20) {myIndexEle = 2;}
      if (eleClassEle[recEle] == 100){myIndexEle = 4;}
      if (eleClassEle[recEle] == 110){myIndexEle = 5;}
      if (eleClassEle[recEle] == 120){myIndexEle = 6;}
      if (eleClassEle[recEle] >= 30  && eleClassEle[recEle] <= 39) {myIndexEle = 3;}
      if (eleClassEle[recEle] >= 130 && eleClassEle[recEle] <= 139){myIndexEle = 7;}
      // cracks as showering
      if (eleClassEle[recEle] == 40) {myIndexEle = 3; }
      if (eleClassEle[recEle] == 140){myIndexEle = 7; }

	// distribution
	float myFish;
	if (eleClassEle[recEle] < 100){ 
	  myFish = 42.0238-3.38943*s9s25Ele[recEle]-794.092*sqrt(covEtaEtaEle[recEle])-15.3449*latEle[recEle]-31.1032*a20Ele[recEle]; }
	if (eleClassEle[recEle] >  99){ 
	  myFish = 27.2967+2.97453*s9s25Ele[recEle]-169.219*sqrt(covEtaEtaEle[recEle])-17.0445*latEle[recEle]-24.8542*a20Ele[recEle]; }
	
	allFW[myIndexEle]++;
	HseeFW[myIndexEle]      -> Fill(sqrt(covEtaEtaEle[recEle]));
	HhoeFW[myIndexEle]      -> Fill(eleHoEEle[recEle]);
	He9e25FW[myIndexEle]    -> Fill(s9s25Ele[recEle]);
	HEoPoutFW[myIndexEle]   -> Fill(eleCorrEoPoutEle[recEle]);
	HdPhiVtxFW[myIndexEle]  -> Fill(eleDeltaPhiAtVtxEle[recEle]);
	HdEtaVtxFW[myIndexEle]  -> Fill(eleDeltaEtaAtVtxEle[recEle]);
	HfisherFW[myIndexEle]   -> Fill(myFish);
	
	// selections 
	if (eleHoEEle[recEle] < hoeCut[myIndexEle])                    { hoeOkFW[myIndexEle]++;     hoetOkFW++;     }
	if (fabs(eleDeltaEtaAtVtxEle[recEle]) < dEtaCut[myIndexEle])   { dEtaOkFW[myIndexEle]++;    dEtatOkFW++;    }
	if (fabs(eleDeltaPhiAtVtxEle[recEle]) < dPhiVtxCut[myIndexEle]){ dPhiVtxOkFW[myIndexEle]++; dPhiVtxtOkFW++; }
	if (eleCorrEoPoutEle[recEle]>eopOutCut[myIndexEle])            { eopoutOkFW[myIndexEle]++;  eopouttOkFW++;  }
	if (s9s25Ele[recEle] > e9e25Cut[recEle])                       { e9e25OkFW[myIndexEle]++;   e9e25tOkFW++;   }
	if (myFish>fisherInfCut[myIndexEle])                           { fisherOkFW[myIndexEle]++;  fishertOkFW++;  }
	if ((sqrt(covEtaEtaEle[recEle])>seeInfCut[myIndexEle]) && (sqrt(covEtaEtaEle[recEle])<seeSupCut[myIndexEle])){seeOkFW[myIndexEle]++; seetOkFW++;}


	// to check distribution after HLT
	if(okTrigger){
	  HseeFW_hlt[myIndexEle]      -> Fill(sqrt(covEtaEtaEle[recEle]));
	  HhoeFW_hlt[myIndexEle]      -> Fill(eleHoEEle[recEle]);
	  He9e25FW_hlt[myIndexEle]    -> Fill(s9s25Ele[recEle]);
	  HEoPoutFW_hlt[myIndexEle]   -> Fill(eleCorrEoPoutEle[recEle]);
	  HdPhiVtxFW_hlt[myIndexEle]  -> Fill(eleDeltaPhiAtVtxEle[recEle]);
	  HdEtaVtxFW_hlt[myIndexEle]  -> Fill(eleDeltaEtaAtVtxEle[recEle]);
	  HfisherFW_hlt[myIndexEle]   -> Fill(myFish);
	}

	// to check distributions with electrons passing isolation
	if (eleTrackerIso_sumPtEle[recEle]<0.05){
	  HseeFW_iso[myIndexEle]      -> Fill(sqrt(covEtaEtaEle[recEle]));
	  HhoeFW_iso[myIndexEle]      -> Fill(eleHoEEle[recEle]);
	  He9e25FW_iso[myIndexEle]    -> Fill(s9s25Ele[recEle]);
	  HEoPoutFW_iso[myIndexEle]   -> Fill(eleCorrEoPoutEle[recEle]);
	  HdPhiVtxFW_iso[myIndexEle]  -> Fill(eleDeltaPhiAtVtxEle[recEle]);
	  HdEtaVtxFW_iso[myIndexEle]  -> Fill(eleDeltaEtaAtVtxEle[recEle]);
	  HfisherFW_iso[myIndexEle]   -> Fill(myFish);
	}
	
	// to check distributions with electrons passing trigger and isolation only
	if(okTrigger){
	  if (eleTrackerIso_sumPtEle[recEle]<0.05){
	    HseeFW_hlt_iso[myIndexEle]      -> Fill(sqrt(covEtaEtaEle[recEle]));
	    HhoeFW_hlt_iso[myIndexEle]      -> Fill(eleHoEEle[recEle]);
	    He9e25FW_hlt_iso[myIndexEle]    -> Fill(s9s25Ele[recEle]);
	    HEoPoutFW_hlt_iso[myIndexEle]   -> Fill(eleCorrEoPoutEle[recEle]);
	    HdPhiVtxFW_hlt_iso[myIndexEle]  -> Fill(eleDeltaPhiAtVtxEle[recEle]);
	    HdEtaVtxFW_hlt_iso[myIndexEle]  -> Fill(eleDeltaEtaAtVtxEle[recEle]);
	    HfisherFW_hlt_iso[myIndexEle]   -> Fill(myFish);
	  }
	}
    }
    
    
    if (recPos > -10){

      // all
      allallFW++;
      
      // others
      if (eleClassEle[recPos] == 0)  {myIndexPos = 0;}
      if (eleClassEle[recPos] == 10) {myIndexPos = 1;}
      if (eleClassEle[recPos] == 20) {myIndexPos = 2;}
      if (eleClassEle[recPos] == 100){myIndexPos = 4;}
      if (eleClassEle[recPos] == 110){myIndexPos = 5;}
      if (eleClassEle[recPos] == 120){myIndexPos = 6;}
      if (eleClassEle[recPos] >= 30  && eleClassEle[recPos] <= 39) {myIndexPos = 3;}
      if (eleClassEle[recPos] >= 130 && eleClassEle[recPos] <= 139){myIndexPos = 7;}
      // cracks as showering
      if (eleClassEle[recPos] == 40) {myIndexPos = 3; }
      if (eleClassEle[recPos] == 140){myIndexPos = 7; }      

      float myFish;
      if (eleClassEle[recPos] < 100){ 
	myFish = 42.0238-3.38943*s9s25Ele[recPos]-794.092*sqrt(covEtaEtaEle[recPos])-15.3449*latEle[recPos]-31.1032*a20Ele[recPos]; }
      if (eleClassEle[recPos] >  99){ 
	myFish = 27.2967+2.97453*s9s25Ele[recPos]-169.219*sqrt(covEtaEtaEle[recPos])-17.0445*latEle[recPos]-24.8542*a20Ele[recPos]; }

      // distributions
      allFW[myIndexPos]++;
      HseeFW[myIndexPos]      -> Fill(sqrt(covEtaEtaEle[recPos]));
      HhoeFW[myIndexPos]      -> Fill(eleHoEEle[recPos]);
      He9e25FW[myIndexPos]    -> Fill(s9s25Ele[recPos]);
      HEoPoutFW[myIndexPos]   -> Fill(eleCorrEoPoutEle[recPos]);
      HdPhiVtxFW[myIndexPos]  -> Fill(eleDeltaPhiAtVtxEle[recPos]);
      HdEtaVtxFW[myIndexPos]  -> Fill(eleDeltaEtaAtVtxEle[recPos]);
      HfisherFW[myIndexPos]   -> Fill(myFish);
      
      // selections 
      if (eleHoEEle[recPos] < hoeCut[myIndexPos])                    { hoeOkFW[myIndexPos]++;     hoetOkFW++;     }
      if (fabs(eleDeltaEtaAtVtxEle[recPos]) < dEtaCut[myIndexPos])   { dEtaOkFW[myIndexPos]++;    dEtatOkFW++;    }
      if (fabs(eleDeltaPhiAtVtxEle[recPos]) < dPhiVtxCut[myIndexPos]){ dPhiVtxOkFW[myIndexPos]++; dPhiVtxtOkFW++; }
      if (eleCorrEoPoutEle[recPos]>eopOutCut[myIndexPos])            { eopoutOkFW[myIndexPos]++;  eopouttOkFW++;  }
      if (s9s25Ele[recPos] > e9e25Cut[recPos])                       { e9e25OkFW[myIndexPos]++;   e9e25tOkFW++;   }
      if (myFish>fisherInfCut[myIndexPos])                           { fisherOkFW[myIndexPos]++;  fishertOkFW++;  }
      if ((sqrt(covEtaEtaEle[recPos])>seeInfCut[myIndexPos]) && (sqrt(covEtaEtaEle[recPos])<seeSupCut[myIndexPos])){seeOkFW[myIndexPos]++; seetOkFW++;}
      
      // to check distributions after hlt
      if(okTrigger){
	HseeFW_hlt[myIndexPos]      -> Fill(sqrt(covEtaEtaEle[recPos]));
	HhoeFW_hlt[myIndexPos]      -> Fill(eleHoEEle[recPos]);
	He9e25FW_hlt[myIndexPos]    -> Fill(s9s25Ele[recPos]);
	HEoPoutFW_hlt[myIndexPos]   -> Fill(eleCorrEoPoutEle[recPos]);
	HdPhiVtxFW_hlt[myIndexPos]  -> Fill(eleDeltaPhiAtVtxEle[recPos]);
	HdEtaVtxFW_hlt[myIndexPos]  -> Fill(eleDeltaEtaAtVtxEle[recPos]);
	HfisherFW_hlt[myIndexPos]   -> Fill(myFish);
      }
      
      // to check distributions with electrons passing isolation
      if (eleTrackerIso_sumPtEle[recPos]<0.05){
	HseeFW_iso[myIndexPos]      -> Fill(sqrt(covEtaEtaEle[recPos]));
	HhoeFW_iso[myIndexPos]      -> Fill(eleHoEEle[recPos]);
	He9e25FW_iso[myIndexPos]    -> Fill(s9s25Ele[recPos]);
	HEoPoutFW_iso[myIndexPos]   -> Fill(eleCorrEoPoutEle[recPos]);
	HdPhiVtxFW_iso[myIndexPos]  -> Fill(eleDeltaPhiAtVtxEle[recPos]);
	HdEtaVtxFW_iso[myIndexPos]  -> Fill(eleDeltaEtaAtVtxEle[recPos]);
	HfisherFW_iso[myIndexPos]   -> Fill(myFish);
      }
      
      // to check distributions with electrons passing trigger and isolation only
      if(okTrigger){
	if (eleTrackerIso_sumPtEle[recPos]<0.05){
	  HseeFW_hlt_iso[myIndexPos]      -> Fill(sqrt(covEtaEtaEle[recPos]));
	  HhoeFW_hlt_iso[myIndexPos]      -> Fill(eleHoEEle[recPos]);
	  He9e25FW_hlt_iso[myIndexPos]    -> Fill(s9s25Ele[recPos]);
	  HEoPoutFW_hlt_iso[myIndexPos]   -> Fill(eleCorrEoPoutEle[recPos]);
	  HdPhiVtxFW_hlt_iso[myIndexPos]  -> Fill(eleDeltaPhiAtVtxEle[recPos]);
	  HdEtaVtxFW_hlt_iso[myIndexPos]  -> Fill(eleDeltaEtaAtVtxEle[recPos]);
	  HfisherFW_hlt_iso[myIndexPos]   -> Fill(myFish);
	}
      }
    }
    


    // some distributions for the eleID studies: JETS
    int myIndexJet = 10;
    for (int ii=0; ii<nEle; ii++){
      if (ii != recEle && ii != recPos){

	// all 
	allallNFW++;
	if (eleClassEle[ii] == 0)  {myIndexJet = 0;}
	if (eleClassEle[ii] == 10) {myIndexJet = 1;}
	if (eleClassEle[ii] == 20) {myIndexJet = 2;}
	if (eleClassEle[ii] == 100){myIndexJet = 4;}
	if (eleClassEle[ii] == 110){myIndexJet = 5;}
	if (eleClassEle[ii] == 120){myIndexJet = 6;}
	if (eleClassEle[ii] >= 30  && eleClassEle[ii] <= 39) {myIndexJet = 3;}
	if (eleClassEle[ii] >= 130 && eleClassEle[ii] <= 139){myIndexJet = 7;}
	// cracks as showering
	if (eleClassEle[ii] == 40) {myIndexJet = 3; }
	if (eleClassEle[ii] == 140){myIndexJet = 7; }      

	float myFish;
	if (eleClassEle[ii] < 100){ 
	  myFish = 42.0238-3.38943*s9s25Ele[ii]-794.092*sqrt(covEtaEtaEle[ii])-15.3449*latEle[ii]-31.1032*a20Ele[ii]; }
	if (eleClassEle[ii] >  99){ 
	  myFish = 27.2967+2.97453*s9s25Ele[ii]-169.219*sqrt(covEtaEtaEle[ii])-17.0445*latEle[ii]-24.8542*a20Ele[ii]; }

	// distibutions
	allNFW[myIndexJet]++;
	HseeNFW[myIndexJet]      -> Fill(sqrt(covEtaEtaEle[ii]));
	HhoeNFW[myIndexJet]      -> Fill(eleHoEEle[ii]);
	He9e25NFW[myIndexJet]    -> Fill(s9s25Ele[ii]);
	HEoPoutNFW[myIndexJet]   -> Fill(eleCorrEoPoutEle[ii]);
	HdPhiVtxNFW[myIndexJet]  -> Fill(eleDeltaPhiAtVtxEle[ii]);
	HdEtaVtxNFW[myIndexJet]  -> Fill(eleDeltaEtaAtVtxEle[ii]);
	HfisherNFW[myIndexJet]   -> Fill(myFish);	  
	
	// selection efficiency class by class 
	if (eleHoEEle[ii] < hoeCut[myIndexJet])                    { hoeOkNFW[myIndexJet]++;     hoetOkNFW++;     }
	if (fabs(eleDeltaEtaAtVtxEle[ii]) < dEtaCut[myIndexJet])   { dEtaOkNFW[myIndexJet]++;    dEtatOkNFW++;    }
	if (fabs(eleDeltaPhiAtVtxEle[ii]) < dPhiVtxCut[myIndexJet]){ dPhiVtxOkNFW[myIndexJet]++; dPhiVtxtOkNFW++; }
	if (eleCorrEoPoutEle[ii]>eopOutCut[myIndexJet])            { eopoutOkNFW[myIndexJet]++;  eopouttOkNFW++;  }
	if (s9s25Ele[ii] > e9e25Cut[ii])                           { e9e25OkNFW[myIndexJet]++;   e9e25tOkNFW++;   }
	if (myFish>fisherInfCut[myIndexJet])                       { fisherOkNFW[myIndexJet]++;  fishertOkNFW++;  }
	if ((sqrt(covEtaEtaEle[ii])>seeInfCut[myIndexJet]) && (sqrt(covEtaEtaEle[ii])<seeSupCut[myIndexJet])){seeOkNFW[myIndexJet]++; seetOkNFW++; }

	// to check distributions after hlt
	if(okTrigger){
	  HseeNFW_hlt[myIndexJet]      -> Fill(sqrt(covEtaEtaEle[ii]));
	  HhoeNFW_hlt[myIndexJet]      -> Fill(eleHoEEle[ii]);
	  He9e25NFW_hlt[myIndexJet]    -> Fill(s9s25Ele[ii]);
	  HEoPoutNFW_hlt[myIndexJet]   -> Fill(eleCorrEoPoutEle[ii]);
	  HdPhiVtxNFW_hlt[myIndexJet]  -> Fill(eleDeltaPhiAtVtxEle[ii]);
	  HdEtaVtxNFW_hlt[myIndexJet]  -> Fill(eleDeltaEtaAtVtxEle[ii]);
	  HfisherNFW_hlt[myIndexJet]   -> Fill(myFish);	  
	}

	// to check distributions with electrons passing isolation
	if (eleTrackerIso_sumPtEle[ii]<0.05){ 
	  HseeNFW_iso[myIndexJet]      -> Fill(sqrt(covEtaEtaEle[ii]));
	  HhoeNFW_iso[myIndexJet]      -> Fill(eleHoEEle[ii]);
	  He9e25NFW_iso[myIndexJet]    -> Fill(s9s25Ele[ii]);
	  HEoPoutNFW_iso[myIndexJet]   -> Fill(eleCorrEoPoutEle[ii]);
	  HdPhiVtxNFW_iso[myIndexJet]  -> Fill(eleDeltaPhiAtVtxEle[ii]);
	  HdEtaVtxNFW_iso[myIndexJet]  -> Fill(eleDeltaEtaAtVtxEle[ii]);
	  HfisherNFW_iso[myIndexJet]   -> Fill(myFish);	  
	}

	// to check distributions with electrons passing isolation and hlt
	if(okTrigger){
	  if (eleTrackerIso_sumPtEle[ii]<0.05){ 
	    HseeNFW_hlt_iso[myIndexJet]      -> Fill(sqrt(covEtaEtaEle[ii]));
	    HhoeNFW_hlt_iso[myIndexJet]      -> Fill(eleHoEEle[ii]);
	    He9e25NFW_hlt_iso[myIndexJet]    -> Fill(s9s25Ele[ii]);
	    HEoPoutNFW_hlt_iso[myIndexJet]   -> Fill(eleCorrEoPoutEle[ii]);
	    HdPhiVtxNFW_hlt_iso[myIndexJet]  -> Fill(eleDeltaPhiAtVtxEle[ii]);
	    HdEtaVtxNFW_hlt_iso[myIndexJet]  -> Fill(eleDeltaEtaAtVtxEle[ii]);
	    HfisherNFW_hlt_iso[myIndexJet]   -> Fill(myFish);	  
	  }
	}
      }
    }
    
  }


  
  // --------------------------------------------------
  // some statistics
  ofstream *MyNumber = new ofstream(outputNevFile,ios::out);
  *MyNumber << endl;
  *MyNumber << "number of ok events: " << numEveOk << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << "electron classification" << endl;
  /*
  *MyNumber << endl;
  *MyNumber << "total number of ele/pos from W = " << allallFW << endl;
  *MyNumber << "golden EB ele/pos from W    = "    << allFW[0]    << ", " << float(allFW[0])/float(allallFW)    << endl;
  *MyNumber << "narrow EB ele/pos from W    = "    << allFW[1]    << ", " << float(allFW[1])/float(allallFW)    << endl;
  *MyNumber << "bigbrem EB ele/pos from W   = "    << allFW[2]    << ", " << float(allFW[2])/float(allallFW)    << endl;
  *MyNumber << "showering EB ele/pos from W = "    << allFW[3]    << ", " << float(allFW[3])/float(allallFW)    << endl;
  *MyNumber << "golden EE ele/pos from W    = "    << allFW[4]    << ", " << float(allFW[4])/float(allallFW)    << endl;
  *MyNumber << "narrow EE ele/pos from W    = "    << allFW[5]    << ", " << float(allFW[5])/float(allallFW)    << endl;
  *MyNumber << "bigbrem EE ele/pos from W   = "    << allFW[6]    << ", " << float(allFW[6])/float(allallFW)    << endl;
  *MyNumber << "showering EE ele/pos from W = "    << allFW[7]    << ", " << float(allFW[7])/float(allallFW)    << endl;
  *MyNumber << endl;
  */
  *MyNumber << "total number of ele/pos not from W = " << allallNFW << endl;
  *MyNumber << "golden EB ele/pos not from W    = "    << allNFW[0]    << ", " << float(allNFW[0])/float(allallNFW)    << endl;
  *MyNumber << "narrow EB ele/pos not from W    = "    << allNFW[1]    << ", " << float(allNFW[1])/float(allallNFW)    << endl;
  *MyNumber << "bigbrem EB ele/pos not from W   = "    << allNFW[2]    << ", " << float(allNFW[2])/float(allallNFW)    << endl;
  *MyNumber << "showering EB ele/pos not from W = "    << allNFW[3]    << ", " << float(allNFW[3])/float(allallNFW)    << endl;
  *MyNumber << "golden EE ele/pos not from W    = "    << allNFW[4]    << ", " << float(allNFW[4])/float(allallNFW)    << endl;
  *MyNumber << "narrow EE ele/pos not from W    = "    << allNFW[5]    << ", " << float(allNFW[5])/float(allallNFW)    << endl;
  *MyNumber << "bigbrem EE ele/pos not from W   = "    << allNFW[6]    << ", " << float(allNFW[6])/float(allallNFW)    << endl;
  *MyNumber << "showering EE ele/pos not from W = "    << allNFW[7]    << ", " << float(allNFW[7])/float(allallNFW)    << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  /*
  *MyNumber << "Electron ID, signal"   << endl;
  *MyNumber << endl;
  *MyNumber << "passing H/E cut:      EB golden    = " << hoeOkFW[0]   << ", " << float(hoeOkFW[0])/float(allFW[0])   << endl;
  *MyNumber << "                      EB bigbrem   = " << hoeOkFW[1]   << ", " << float(hoeOkFW[1])/float(allFW[1])   << endl;
  *MyNumber << "                      EB narrow    = " << hoeOkFW[2]   << ", " << float(hoeOkFW[2])/float(allFW[2])   << endl;
  *MyNumber << "                      EB showering = " << hoeOkFW[3]   << ", " << float(hoeOkFW[3])/float(allFW[3])   << endl;
  *MyNumber << "                      EE golden    = " << hoeOkFW[4]   << ", " << float(hoeOkFW[4])/float(allFW[4])   << endl;
  *MyNumber << "                      EE bigbrem   = " << hoeOkFW[5]   << ", " << float(hoeOkFW[5])/float(allFW[5])   << endl;
  *MyNumber << "                      EE narrow    = " << hoeOkFW[6]   << ", " << float(hoeOkFW[6])/float(allFW[6])   << endl;
  *MyNumber << "                      EE showering = " << hoeOkFW[7]   << ", " << float(hoeOkFW[7])/float(allFW[7])   << endl;
  *MyNumber << endl;
  *MyNumber << "passing dEtaVtx cut:  EB golden    = " << dEtaOkFW[0] << ", " << float(dEtaOkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << dEtaOkFW[1] << ", " << float(dEtaOkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << dEtaOkFW[2] << ", " << float(dEtaOkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << dEtaOkFW[3] << ", " << float(dEtaOkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << dEtaOkFW[4] << ", " << float(dEtaOkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << dEtaOkFW[5] << ", " << float(dEtaOkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << dEtaOkFW[6] << ", " << float(dEtaOkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << dEtaOkFW[7] << ", " << float(dEtaOkFW[7])/float(allFW[7]) << endl;
  *MyNumber << endl;
  *MyNumber << "passing dPhiVtx cut:  EB golden    = " << dPhiVtxOkFW[0] << ", " << float(dPhiVtxOkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << dPhiVtxOkFW[1] << ", " << float(dPhiVtxOkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << dPhiVtxOkFW[2] << ", " << float(dPhiVtxOkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << dPhiVtxOkFW[3] << ", " << float(dPhiVtxOkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << dPhiVtxOkFW[4] << ", " << float(dPhiVtxOkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << dPhiVtxOkFW[5] << ", " << float(dPhiVtxOkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << dPhiVtxOkFW[6] << ", " << float(dPhiVtxOkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << dPhiVtxOkFW[7] << ", " << float(dPhiVtxOkFW[7])/float(allFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing EOP_OUT cut:  EB golden    = " << eopoutOkFW[0] << ", " << float(eopoutOkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << eopoutOkFW[1] << ", " << float(eopoutOkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << eopoutOkFW[2] << ", " << float(eopoutOkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << eopoutOkFW[3] << ", " << float(eopoutOkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << eopoutOkFW[4] << ", " << float(eopoutOkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << eopoutOkFW[5] << ", " << float(eopoutOkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << eopoutOkFW[6] << ", " << float(eopoutOkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << eopoutOkFW[7] << ", " << float(eopoutOkFW[7])/float(allFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing SEE cut:      EB golden    = " << seeOkFW[0] << ", " << float(seeOkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << seeOkFW[1] << ", " << float(seeOkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << seeOkFW[2] << ", " << float(seeOkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << seeOkFW[3] << ", " << float(seeOkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << seeOkFW[4] << ", " << float(seeOkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << seeOkFW[5] << ", " << float(seeOkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << seeOkFW[6] << ", " << float(seeOkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << seeOkFW[7] << ", " << float(seeOkFW[7])/float(allFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing E9/E25 cut:   EB golden    = " << e9e25OkFW[0] << ", " << float(e9e25OkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << e9e25OkFW[1] << ", " << float(e9e25OkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << e9e25OkFW[2] << ", " << float(e9e25OkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << e9e25OkFW[3] << ", " << float(e9e25OkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << e9e25OkFW[4] << ", " << float(e9e25OkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << e9e25OkFW[5] << ", " << float(e9e25OkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << e9e25OkFW[6] << ", " << float(e9e25OkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << e9e25OkFW[7] << ", " << float(e9e25OkFW[7])/float(allFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing fisher cut:   EB golden    = " << fisherOkFW[0] << ", " << float(fisherOkFW[0])/float(allFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << fisherOkFW[1] << ", " << float(fisherOkFW[1])/float(allFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << fisherOkFW[2] << ", " << float(fisherOkFW[2])/float(allFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << fisherOkFW[3] << ", " << float(fisherOkFW[3])/float(allFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << fisherOkFW[4] << ", " << float(fisherOkFW[4])/float(allFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << fisherOkFW[5] << ", " << float(fisherOkFW[5])/float(allFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << fisherOkFW[6] << ", " << float(fisherOkFW[6])/float(allFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << fisherOkFW[7] << ", " << float(fisherOkFW[7])/float(allFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  */
  *MyNumber << endl;
  *MyNumber << "Electron ID, jets"   << endl;
  *MyNumber << "passing H/E cut:      EB golden    = " << hoeOkNFW[0]   << ", " << float(hoeOkNFW[0])/float(allNFW[0])   << endl;
  *MyNumber << "                      EB bigbrem   = " << hoeOkNFW[1]   << ", " << float(hoeOkNFW[1])/float(allNFW[1])   << endl;
  *MyNumber << "                      EB narrow    = " << hoeOkNFW[2]   << ", " << float(hoeOkNFW[2])/float(allNFW[2])   << endl;
  *MyNumber << "                      EB showering = " << hoeOkNFW[3]   << ", " << float(hoeOkNFW[3])/float(allNFW[3])   << endl;
  *MyNumber << "                      EE golden    = " << hoeOkNFW[4]   << ", " << float(hoeOkNFW[4])/float(allNFW[4])   << endl;
  *MyNumber << "                      EE bigbrem   = " << hoeOkNFW[5]   << ", " << float(hoeOkNFW[5])/float(allNFW[5])   << endl;
  *MyNumber << "                      EE narrow    = " << hoeOkNFW[6]   << ", " << float(hoeOkNFW[6])/float(allNFW[6])   << endl;
  *MyNumber << "                      EE showering = " << hoeOkNFW[7]   << ", " << float(hoeOkNFW[7])/float(allNFW[7])   << endl;
  *MyNumber << endl;
  *MyNumber << "passing dEtaVtx cut:  EB golden    = " << dEtaOkNFW[0] << ", " << float(dEtaOkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << dEtaOkNFW[1] << ", " << float(dEtaOkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << dEtaOkNFW[2] << ", " << float(dEtaOkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << dEtaOkNFW[3] << ", " << float(dEtaOkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << dEtaOkNFW[4] << ", " << float(dEtaOkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << dEtaOkNFW[5] << ", " << float(dEtaOkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << dEtaOkNFW[6] << ", " << float(dEtaOkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << dEtaOkNFW[7] << ", " << float(dEtaOkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing dPhiVtx cut:  EB golden    = " << dPhiVtxOkNFW[0] << ", " << float(dPhiVtxOkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << dPhiVtxOkNFW[1] << ", " << float(dPhiVtxOkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << dPhiVtxOkNFW[2] << ", " << float(dPhiVtxOkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << dPhiVtxOkNFW[3] << ", " << float(dPhiVtxOkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << dPhiVtxOkNFW[4] << ", " << float(dPhiVtxOkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << dPhiVtxOkNFW[5] << ", " << float(dPhiVtxOkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << dPhiVtxOkNFW[6] << ", " << float(dPhiVtxOkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << dPhiVtxOkNFW[7] << ", " << float(dPhiVtxOkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing EOP_OUT cut:  EB golden    = " << eopoutOkNFW[0] << ", " << float(eopoutOkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << eopoutOkNFW[1] << ", " << float(eopoutOkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << eopoutOkNFW[2] << ", " << float(eopoutOkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << eopoutOkNFW[3] << ", " << float(eopoutOkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << eopoutOkNFW[4] << ", " << float(eopoutOkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << eopoutOkNFW[5] << ", " << float(eopoutOkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << eopoutOkNFW[6] << ", " << float(eopoutOkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << eopoutOkNFW[7] << ", " << float(eopoutOkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << "passing SEE cut:      EB golden    = " << seeOkNFW[0] << ", " << float(seeOkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << seeOkNFW[1] << ", " << float(seeOkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << seeOkNFW[2] << ", " << float(seeOkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << seeOkNFW[3] << ", " << float(seeOkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << seeOkNFW[4] << ", " << float(seeOkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << seeOkNFW[5] << ", " << float(seeOkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << seeOkNFW[6] << ", " << float(seeOkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << seeOkNFW[7] << ", " << float(seeOkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing E9/E25 cut:   EB golden    = " << e9e25OkNFW[0] << ", " << float(e9e25OkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << e9e25OkNFW[1] << ", " << float(e9e25OkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << e9e25OkNFW[2] << ", " << float(e9e25OkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << e9e25OkNFW[3] << ", " << float(e9e25OkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << e9e25OkNFW[4] << ", " << float(e9e25OkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << e9e25OkNFW[5] << ", " << float(e9e25OkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << e9e25OkNFW[6] << ", " << float(e9e25OkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << e9e25OkNFW[7] << ", " << float(e9e25OkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << "passing fisher cut:   EB golden    = " << fisherOkNFW[0] << ", " << float(fisherOkNFW[0])/float(allNFW[0]) << endl; 
  *MyNumber << "                      EB bigbrem   = " << fisherOkNFW[1] << ", " << float(fisherOkNFW[1])/float(allNFW[1]) << endl; 
  *MyNumber << "                      EB narrow    = " << fisherOkNFW[2] << ", " << float(fisherOkNFW[2])/float(allNFW[2]) << endl; 
  *MyNumber << "                      EB showering = " << fisherOkNFW[3] << ", " << float(fisherOkNFW[3])/float(allNFW[3]) << endl; 
  *MyNumber << "                      EE golden    = " << fisherOkNFW[4] << ", " << float(fisherOkNFW[4])/float(allNFW[4]) << endl; 
  *MyNumber << "                      EE bigbrem   = " << fisherOkNFW[5] << ", " << float(fisherOkNFW[5])/float(allNFW[5]) << endl; 
  *MyNumber << "                      EE narrow =  = " << fisherOkNFW[6] << ", " << float(fisherOkNFW[6])/float(allNFW[6]) << endl; 
  *MyNumber << "                      EE showering = " << fisherOkNFW[7] << ", " << float(fisherOkNFW[7])/float(allNFW[7]) << endl; 
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << "Electron ID - all classes, signal"   << endl;
  *MyNumber << "passing H/E cut:     " << float(hoetOkFW)/float(allallFW)     << " +- " << pow(hoetOkFW*(1-(hoetOkFW/allallFW)),0.5)/allallFW         << endl;
  *MyNumber << "passing dEtaVtx cut: " << float(dEtatOkFW)/float(allallFW)    << " +- " << pow(dEtatOkFW*(1-(dEtatOkFW/allallFW)),0.5)/allallFW       << endl;
  *MyNumber << "passing dPhiVtx cut: " << float(dPhiVtxtOkFW)/float(allallFW) << " +- " << pow(dPhiVtxtOkFW*(1-(dPhiVtxtOkFW/allallFW)),0.5)/allallFW << endl;
  *MyNumber << "passing EOP_OUT cut: " << float(eopouttOkFW)/float(allallFW)  << " +- " << pow(eopouttOkFW*(1-(eopouttOkFW/allallFW)),0.5)/allallFW   << endl;
  *MyNumber << "passing SEE cut:     " << float(seetOkFW)/float(allallFW)     << " +- " << pow(seetOkFW*(1-(seetOkFW/allallFW)),0.5)/allallFW         << endl;
  *MyNumber << "passing E9/E25 cut:  " << float(e9e25tOkFW)/float(allallFW)   << " +- " << pow(e9e25tOkFW*(1-(e9e25tOkFW/allallFW)),0.5)/allallFW     << endl; 
  *MyNumber << "passing fisher cut:  " << float(fishertOkFW)/float(allallFW)  << " +- " << pow(fishertOkFW*(1-(fishertOkFW/allallFW)),0.5)/allallFW   << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << endl;
  *MyNumber << "Electron ID - all classes, jets"   << endl;
  *MyNumber << "passing H/E cut:     " << float(hoetOkNFW)/float(allallNFW)     << " +- " << pow(hoetOkNFW*(1-(hoetOkNFW/allallNFW)),0.5)/allallNFW         << endl;
  *MyNumber << "passing dEtaVtx cut: " << float(dEtatOkNFW)/float(allallNFW)    << " +- " << pow(dEtatOkNFW*(1-(dEtatOkNFW/allallNFW)),0.5)/allallNFW       << endl;
  *MyNumber << "passing dPhiVtx cut: " << float(dPhiVtxtOkNFW)/float(allallNFW) << " +- " << pow(dPhiVtxtOkNFW*(1-(dPhiVtxtOkNFW/allallNFW)),0.5)/allallNFW << endl;
  *MyNumber << "passing EOP_OUT cut: " << float(eopouttOkNFW)/float(allallNFW)  << " +- " << pow(eopouttOkNFW*(1-(eopouttOkNFW/allallNFW)),0.5)/allallNFW   << endl;
  *MyNumber << "passing SEE cut:     " << float(seetOkNFW)/float(allallNFW)     << " +- " << pow(seetOkNFW*(1-(seetOkNFW/allallNFW)),0.5)/allallNFW         << endl;
  *MyNumber << "passing E9/E25 cut:  " << float(e9e25tOkNFW)/float(allallNFW)   << " +- " << pow(e9e25tOkNFW*(1-(e9e25tOkNFW/allallNFW)),0.5)/allallNFW     << endl; 
  *MyNumber << "passing fisher cut:  " << float(fishertOkNFW)/float(allallNFW)  << " +- " << pow(fishertOkNFW*(1-(fishertOkNFW/allallNFW)),0.5)/allallNFW   << endl;
  *MyNumber << endl;
  *MyNumber << endl;

  MyNumber->close();
  delete MyNumber;  

     
  // ----------------------------------
  //  writing histos in the root file
  TFile *hf = new TFile(outputRootFile, "recreate");  

  for (int ii=0; ii<8; ii++){
    HseeFW[ii]       -> Write();  
    HhoeFW[ii]       ->	Write();  
    He9e25FW[ii]     ->	Write();  
    HEoPoutFW[ii]    ->	Write();  
    HdPhiVtxFW[ii]   ->	Write();  
    HdEtaVtxFW[ii]   ->	Write();  
    HfisherFW[ii]    -> Write();  
    // 
    HseeFW_hlt[ii]       -> Write();  
    HhoeFW_hlt[ii]       -> Write();  
    He9e25FW_hlt[ii]     -> Write();  
    HEoPoutFW_hlt[ii]    -> Write();  
    HdPhiVtxFW_hlt[ii]   -> Write();  
    HdEtaVtxFW_hlt[ii]   -> Write();  
    HfisherFW_hlt[ii]    -> Write();  
    //
    HseeFW_iso[ii]       -> Write();  
    HhoeFW_iso[ii]       -> Write();  
    He9e25FW_iso[ii]     -> Write();  
    HEoPoutFW_iso[ii]    -> Write();  
    HdPhiVtxFW_iso[ii]   -> Write();  
    HdEtaVtxFW_iso[ii]   -> Write();  
    HfisherFW_iso[ii]    -> Write();  
    //
    HseeFW_hlt_iso[ii]       -> Write();  
    HhoeFW_hlt_iso[ii]       -> Write();  
    He9e25FW_hlt_iso[ii]     -> Write();  
    HEoPoutFW_hlt_iso[ii]    -> Write();  
    HdPhiVtxFW_hlt_iso[ii]   -> Write();  
    HdEtaVtxFW_hlt_iso[ii]   -> Write();  
    HfisherFW_hlt_iso[ii]    -> Write();  
    //
    HseeNFW[ii]       -> Write();  
    HhoeNFW[ii]       -> Write();  
    He9e25NFW[ii]     -> Write();  
    HEoPoutNFW[ii]    -> Write();  
    HdPhiVtxNFW[ii]   -> Write();  
    HdEtaVtxNFW[ii]   -> Write();  
    HfisherNFW[ii]    -> Write();  
    //
    HseeNFW_hlt[ii]       -> Write();  
    HhoeNFW_hlt[ii]       -> Write();  
    He9e25NFW_hlt[ii]     -> Write();  
    HEoPoutNFW_hlt[ii]    -> Write();  
    HdPhiVtxNFW_hlt[ii]   -> Write();  
    HdEtaVtxNFW_hlt[ii]   -> Write();  
    HfisherNFW_hlt[ii]    -> Write();  
    //
    HseeNFW_iso[ii]       -> Write();  
    HhoeNFW_iso[ii]       -> Write();  
    He9e25NFW_iso[ii]     -> Write();  
    HEoPoutNFW_iso[ii]    -> Write();  
    HdPhiVtxNFW_iso[ii]   -> Write();  
    HdEtaVtxNFW_iso[ii]   -> Write();  
    HfisherNFW_iso[ii]    -> Write();  
    //
    HseeNFW_hlt_iso[ii]       -> Write();  
    HhoeNFW_hlt_iso[ii]       -> Write();  
    He9e25NFW_hlt_iso[ii]     -> Write();  
    HEoPoutNFW_hlt_iso[ii]    -> Write();  
    HdPhiVtxNFW_hlt_iso[ii]   -> Write();  
    HdEtaVtxNFW_hlt_iso[ii]   -> Write();  
    HfisherNFW_hlt_iso[ii]    -> Write();  
  }			
  
  H_PoPtrue -> Write();
  hf->Close();
  

}  // end main ;


