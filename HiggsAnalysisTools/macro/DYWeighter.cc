#ifndef DYWEIGHT_H
#define DYWEIGHT_H

#include "TH2D.h"

class DYWeighter {
public:
  DYWeighter(const char* file);
  ~DYWeighter();
  float GetWeight(float mll, float pT, float rapidity);
protected:
  std::vector<TH2D*> weightH;
};

DYWeighter::DYWeighter(const char* file) {

  TFile *weightFile = TFile::Open(file);

  if(!weightFile) std::cout << "PROBLEM IN OPENING FILE: " << file << std::endl;

  char stringbin[20];
  for(int i=0;i<40;i++) {
    sprintf(stringbin,"weight_%02d",i+1);
    TH2D *map = (TH2D*)weightFile->Get(stringbin)->Clone("map");
    weightH.push_back(map);
  }
}

DYWeighter::~DYWeighter() {
  weightH.clear();
}
  
float DYWeighter::GetWeight(float mll, float pT, float rapidity) {
  
  float mllBins[41] = {15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 64, 68, 72, 76, 81, 86, 91, 96, 101, 106, 110, 115, 120, 126, 133, 141, 150, 160, 171, 185, 200, 220, 243, 273, 320, 380, 440, 510, 600, 1000, 1500};

  if(mll<15 || mll>=1500) return 1.0;

  int massBin = -1;
  for(int i=0; i<40; i++) {
    if(mll>=mllBins[i] && mll<mllBins[i+1]) {
      massBin = i+1;
      break;
    }
  }

  if(massBin<0) {
    std::cout << "ERROR. THIS SHOULD NOT HAPPEN" << std::endl;
    return -1.0;
  }

  TH2D *map = weightH[massBin];
  if(!map) return 1.0;

  float weight = -1.0;
  int   xBins = map->GetXaxis()->GetNbins();
  float xMin  = map->GetXaxis()->GetBinLowEdge(1);
  float xMax  = map->GetXaxis()->GetBinUpEdge(xBins);
  int   yBins = map->GetYaxis()->GetNbins();
  float yMin  = map->GetYaxis()->GetBinLowEdge(1);
  float yMax  = map->GetYaxis()->GetBinUpEdge(yBins);
  int theBin = map->FindBin(pT, rapidity);
  if (pT>xMin && pT<xMax && rapidity>yMin && rapidity<yMax) {
    weight = map->GetBinContent(theBin);
    //    std::cout << "mll =" << mll << " pt = " << pT << " rapidity = " << rapidity << " massbin = " << massBin << " weight = " << weight << std::endl;
  } else {
    weight = 1.;
  }

  return weight;
}
#endif  
