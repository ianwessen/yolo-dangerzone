//-------------------------------------------------------
// Description:
//    Class for selection of reconstructed W -> enu
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
// Original code:
//    CutAnaHiggs_2e2nu.cpp
//-------------------------------------------------------

#ifndef WplusJets_h
#define WplusJets_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>


class WplusJets : public HiggsBase{
public:
  WplusJets(TTree *tree=0);
  virtual ~WplusJets();
  void Loop();
  void displayEfficiencies();
  
private:

  bool findMcTree(const char* processType);

  void bookHistos();
  int getBestLepton();
  int getBestJet();
  void setKinematics(int theEle, int theJet);
  bool jetVeto();
  bool isEleID(int eleIndex);
  float Fisher(int eleIndex);
  std::vector<int> findProbe(float deltaR);

  // private members
  Counters _counter;
  Counters _eleCounter;
  Counters _jetCounter;
  Selection* _selection;
  std::vector<Selection*> _electronSelection;
  bool _verbose;

  TVector3 _p3Ele, _p3Met, _p3Jet, _p3System;
  float _elePt;

  // vectors to store indices of best candidates
  std::vector<int> *_bestElectrons;
  std::vector<int> *_bestJets;
  std::vector<int> *_bestGenJets;
  int _bestProbe;
  
  // monitoring tools
  Monitor *_monitorEventAfterSelection, *_monitorMet, *_monitorElectrons, *_monitorJets;
  Monitor *_monitorEventAfterReco;
  Monitor *_monitorGenerator, *_monitorGenJets;

  // simple histograms
  TH1F *fakeRecoEta, *fakeIdentifiedEta, *fakeGenEta;
  TH1F *fakeRecoPt, *fakeIdentifiedPt, *fakeGenPt;

  // new variables
  float _transvMass[1];
  float _deltaPhi[1];
  float _wplusjets[1];
  float _histopt[1];
  float _nEle[1], _nJet[1];

  float _elePtGen[1];
  float _ptGen[1];
  float _nGenJet[1];

  // Histograms of PDFs
  TH1F *_EoverPoutPDF;
  TH1F *_deltaEtaPDF;
  TH1F *_deltaPhiPDF;
  TH1F *_HoverEPDF;
  TH1F *_shapeFisherPDF;

};
#endif
