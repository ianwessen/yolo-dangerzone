#include <fstream>
#include <iostream>
#include "TString.h"
#include "HiggsAnalysisTools/include/kFactorEvaluator.hh"

kFactorEvaluator::kFactorEvaluator(int mH) {
  HiggsMass_ = mH;
  
  ranges.clear();
  kfactors.clear();
  
  char mass[5];
  sprintf(mass,"%d",mH);

  TString path = TString("config/kfactors_Std/kfactors_mh")+
    TString(mass)+TString("_ren")+TString(mass)+TString("_fac")+TString(mass)+
    TString(".dat");
  std::ifstream file(path.Data());

  if(!file.good()) {
    std::cout << "kFactorEvaluator::Error!   Unable to open the file " << path.Data() << std::endl;
  }
  else {
    std::cout << "KFactors read from " << path.Data() << "..." << std::endl;

    // first data describes the map: histo bin, max, min
    int nbinspt=0;
    double minpt=0;
    double maxpt=0;
    double alternativeK=0;
    double alternativeNNLOK=0;
    std::string dummy;
    file >> nbinspt; file >> dummy;
    file >> minpt; file >> dummy;
    file >> maxpt; file >> dummy;
    file >> alternativeK; file >> dummy;
    file >> alternativeNNLOK; file>>dummy; file >> dummy;

    while(!file.eof()) {
      float min, max, val;
      file >> min >> max >> val;
      range bin;
      bin.min = min;
      bin.max = max;
      ranges.push_back(bin);
      kfactors.push_back(val);
    }
  }

  std::cout << kfactors.size() << " values read." << std::endl;

}

// new k-factor for WW, computed by Giovanni Petrucciani 
float kFactorEvaluator::evaluate(float ptH) {
  for(int i=0; i<(int)kfactors.size(); i++) {
    float min = (ranges[i]).min;
    float max = (ranges[i]).max;
    if(ptH>=min && ptH<max) return kfactors[i];
  }
  //  std::cout << "H pT = " << ptH << ": out of range, use 1" << std::endl;
  return 1;
}

