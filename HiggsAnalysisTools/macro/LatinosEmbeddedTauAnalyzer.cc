// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include "addWeightsToTreeLooseLoose.h"

using namespace std;

int main(int argc, char* argv[]) {

  char fileName[500];
  if ( argc < 2 ) {
    std::cout << "Format should be: LatinosLooseLooseAnalyzer filename.root" << std::endl;
    return 1;
  }
  strcpy(fileName,argv[1]);

  std::cout << "Processing sample " << fileName << std::endl;

  TFile *file = TFile::Open(fileName);
  TTree *tree = (TTree*)gDirectory->Get("latino");
  addWeightsToTreeLooseLoose weighter(tree);
  weighter.SetSample(embeddedDYtt);
  weighter.Loop();

  return 0;

}
