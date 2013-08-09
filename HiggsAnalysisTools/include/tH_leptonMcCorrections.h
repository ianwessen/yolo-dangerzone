//--------------------------------------------------------------------------------------------------
// code from G.Petrucciani and C.Botta 
// Originally in /UserCode/CMG/CMGTools/TTHAnalysis/python/plotter/mcCorrections.cc                          
//--------------------------------------------------------------------------------------------------

#include <cmath>
#include <TRandom.h>

#ifndef HIGGSANALYSIS_tH_leptonMcCorrections_H
#define HIGGSANALYSIS_tH_leptonMcCorrections_H

using namespace std;

class tH_leptonMcCorrections {

 public:

  tH_leptonMcCorrections();

  ~tH_leptonMcCorrections();

  double scaleIpVarsMC(double ipvar, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);

  double scaleSip3dMC(double sip3d, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);

  double scaleDzMC(double dz, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);
  
  double scaleDxyMC(double dxy, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);

  double correctJetPtRatioMC(double jetPtRatio, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);

  double correctJetDRMC(double jetDR, int pdgId, double pt, double eta, int mcMatchId, int mcMatchAny);

 protected:

  TRandom *gSmearer_;

  double smearMC(double x, double mu, double sigma);

  double logSmearMC(double x, double mu, double sigma);

  double shiftMC(double x, double delta);

  double scaleShiftMC(double x, double scale, double shift);

};

#endif
