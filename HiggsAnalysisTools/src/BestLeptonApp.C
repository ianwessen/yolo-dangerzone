// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>


// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include "CommonTools/include/TriggerMask.hh"
#include "HiggsAnalysisTools/include/BestLeptonSelectorWjets.hh"

int main(int argc, char* argv[]) {

  char inputFileName[150];
  char outputFileName[150];
  if ( argc < 2 ){
    std::cout << "missing argument: insert at least inputFile with list of root files" << std::endl; 
    std::cout << "HiggsApp inputFile [outputFile]" << std::endl;
    return 1;
  }
  strcpy(inputFileName,argv[1]);
  if ( argc < 3 ) sprintf(outputFileName,"def.root");
  else strcpy(outputFileName,argv[2]);

  // -------------------------
  // loading file:
  TChain *theChain = new TChain("ntp1");
  char Buffer[500];
  char MyRootFile[2000];  
  std::cout << "input: " << inputFileName << std::endl;
  ifstream *inputFile = new ifstream(inputFileName);

  // get the tree with the conditions from the first file
  TTree *treeCond = new TTree();

  int nfiles=1;
  while( !(inputFile->eof()) ){
    inputFile->getline(Buffer,500);
    if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer)))
      {
	sscanf(Buffer,"%s",MyRootFile);
	theChain->Add(MyRootFile);
	if ( nfiles==1 ) {
	  TFile *firstfile = TFile::Open(MyRootFile);
	  treeCond = (TTree*)firstfile->Get("Conditions");
	}
	std::cout << "chaining " << MyRootFile << std::endl;
	nfiles++;
      }
  }
  inputFile->close();
  delete inputFile;

  BestLeptonSelectorWjets selector(theChain);

  TriggerMask mask(treeCond);
  mask.requireTrigger("HLT_Ele15_LW_L1R");
  std::vector<int> requiredTriggers = mask.getBits();
  selector.requireTrigger(requiredTriggers);
  selector.Loop();

}
