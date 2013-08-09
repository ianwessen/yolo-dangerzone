//! c++ includes
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <math.h>
#include <map>

//! ROOT includes
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"

using namespace std;

void plotOverlap ( ) {

  // input file at WW level
  ifstream *fileWW = new ifstream("inputsTxt_Mc_0j/ww.txt");

  // input files mass x mass
  ifstream *intxt[18];
  intxt[0]  = new ifstream("inputsTxt_Mc_0j/mH115.txt");
  intxt[1]  = new ifstream("inputsTxt_Mc_0j/mH120.txt");
  intxt[2]  = new ifstream("inputsTxt_Mc_0j/mH130.txt");
  intxt[3]  = new ifstream("inputsTxt_Mc_0j/mH140.txt");
  intxt[4]  = new ifstream("inputsTxt_Mc_0j/mH150.txt");
  intxt[5]  = new ifstream("inputsTxt_Mc_0j/mH160.txt");
  intxt[6]  = new ifstream("inputsTxt_Mc_0j/mH170.txt");
  intxt[7]  = new ifstream("inputsTxt_Mc_0j/mH180.txt");
  intxt[8]  = new ifstream("inputsTxt_Mc_0j/mH190.txt");
  intxt[9]  = new ifstream("inputsTxt_Mc_0j/mH200.txt");
  intxt[10] = new ifstream("inputsTxt_Mc_0j/mH250.txt");
  intxt[11] = new ifstream("inputsTxt_Mc_0j/mH300.txt");
  intxt[12] = new ifstream("inputsTxt_Mc_0j/mH350.txt");
  intxt[13] = new ifstream("inputsTxt_Mc_0j/mH400.txt");
  intxt[14] = new ifstream("inputsTxt_Mc_0j/mH450.txt");
  intxt[15] = new ifstream("inputsTxt_Mc_0j/mH500.txt");
  intxt[16] = new ifstream("inputsTxt_Mc_0j/mH550.txt");
  intxt[17] = new ifstream("inputsTxt_Mc_0j/mH600.txt");


  // variables from the reference file (WW level) and per each mass
  char Buffer[2000];
  int myWWEntry[1000],  wwIFC;
  int myEntry[18][1000], IFC[18];
  int sumInCommon[19];
  int foundEvents;

  // initializations
  foundEvents=0;
  wwIFC=0;
  for (int iFile=0; iFile<18; iFile++){ 
    IFC[iFile] = 0; 
    for (int iLine=0; iLine<1000; iLine++) {
      myWWEntry[iLine]=0;
      myEntry[iFile][iLine]=0;
    }
  }
  for (int iFile=0; iFile<=18; iFile++) sumInCommon[iFile] = 0;  

  // reading from the file at WW level
  while( !(fileWW->eof()) ){
    fileWW->getline(Buffer,400);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
      sscanf(Buffer,"%d", &myWWEntry[wwIFC]);
    }
    wwIFC++;
  }
  fileWW->close();
  delete fileWW;

  // reading the text input files: mass per mass
  for (int iFile=0; iFile<18; iFile++){
    while( !(intxt[iFile]->eof()) ){
      intxt[iFile]->getline(Buffer,400);
      if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
	sscanf(Buffer,"%d", &myEntry[iFile][IFC[iFile]]);
      }
      IFC[iFile]++;
    }
    intxt[iFile]->close();
    delete intxt[iFile];
  } 

  // filling maps with the inputs read from trees
  map<int, int> myEntryMap[18];
  for (int iFile=0; iFile<18; iFile++){
    for (int theLine=0; theLine<IFC[iFile]; theLine++) {
      myEntryMap[iFile].insert(pair<int, int>(myEntry[iFile][theLine], 999));
    }}


  // checking events in common
  map<int, int>::const_iterator iterMap;    
  for (int iLine=0; iLine<wwIFC; iLine++){
    int thisLineValue = myWWEntry[iLine];

    // cout << endl;    
    // cout << "value: " << thisLineValue << endl;
    
    bool gotIt = false;
    int inCommon=0;
    for (int iFile=0; iFile<18; iFile++){ 
      iterMap = (myEntryMap[iFile]).find(thisLineValue);
      if (iterMap != myEntryMap[iFile].end()) { 
	// cout << "found in " << iFile << endl;
	inCommon++;
	gotIt = true;
      }
    }
   
    if (gotIt) foundEvents++;
    sumInCommon[inCommon]++;
    // cout << "incremento " << inCommon << endl;
  }

  cout << "summary: " << endl;
  cout << "in total " << wwIFC << " events at WW level" << endl;
  cout << foundEvents << " events found in at least 1 analysis" << endl;
  for (int iFile=0; iFile<=18; iFile++) 
    cout << "events found in " << iFile << " analyses: " << sumInCommon[iFile] << endl;

  // sumamry histo
  TH1F *mySummary1 = new TH1F("mySummary1","",18,0.5,18.5);
  TH1F *mySummary2 = new TH1F("mySummary2","",18,0.5,18.5);
  for (int iFile=1; iFile<=18; iFile++) {
    mySummary1->SetBinContent(iFile, sumInCommon[iFile]);
    mySummary2->SetBinContent(iFile, (float(sumInCommon[iFile])/float(foundEvents)));
  }

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","",1);
  c1->Divide(1,2);
  c1->cd(1); mySummary1->Draw();
  c1->cd(2); mySummary2->Draw();

  TCanvas *c2 = new TCanvas("c2","",1);
  mySummary2->Draw();
}
