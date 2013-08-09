// macro to get the number of exected events 
// ----
// numbers of events are hardcoded
// lumi to normalize is hardcoded (100 pb-1)
// ----
// usage: 
// root -b
// .L macro/higgsEleIDPlots.cxx++
// expectedEvents()


#include <vector>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

void expectedEvents() {

  // normalization lumi (pb-1)
  float lumi = 100.0;

  // for the normal processes, exp ev = L_int * xsec * efficiency
  float totalEve;
  vector<float> xsec;
  vector<float> eveNumber;               // the final one - tight eleID from egamma
  vector<float> eveNumberAfterEle;       // after CJV - tight eleID from egamma
  vector<float> efficiency;         
  vector<float> efficiencyAfterEle; 
  vector<float> efficiencyErr;         
  vector<float> efficiencyErrAfterEle;
  vector<float> expEv;
  vector<float> expEvAfterEle;
  vector<float> expEvErr;
  vector<float> expEvErrAfterEle;

  /*
  // Mh = 160
  // here put the signal cross section in pb
  xsec.push_back(2.36);                        // signal H160 pb-1 -> WW -> 2l2nu  
  // here put signal total number of events (all 2l2nu)
  totalEve = 51566.000;
  // here put signal number of events after the full selection
  // eveNumber.push_back(147.000);                // loose eGamma eleID
  // eveNumber.push_back(112.000);                // tight eGamma eleID
  // eveNumber.push_back(102.000);                // H->WW AN eleID
  // eveNumber.push_back(124.000);                // new, loose for BB & narrow
  eveNumber.push_back(121.000);                // new, tight for BB & narrow  
  // eveNumberAfterEle.push_back(2732.000);       // loose eGamma eleID
  // eveNumberAfterEle.push_back(2034.000);       // tight eGamma eleID
  // eveNumberAfterEle.push_back(1704.000);       // H->WW AN eleID
  // eveNumberAfterEle.push_back(2157.000);       // new, loose for BB & narrow
  eveNumberAfterEle.push_back(2133.000);       // new, tight for BB & narrow
  */

  /*
  // Mh = 130   
  xsec.push_back(1.06);                        // signal H130 pb-1 -> WW -> 2l2nu  (taken from the note)
  totalEve = 208793.000;
  // eveNumber.push_back(821.000);                // new, loose for BB & narrow  
  // eveNumberAfterEle.push_back(6487.000);       // new, loose for BB & narrow
  eveNumber.push_back(803.000);                // optim @ 160
  eveNumberAfterEle.push_back(6328.000);       // optim @ 160
  */

  // Mh = 190
  // here put the signal cross section in pb
  xsec.push_back(1.51);                        // signal H190 pb-1 -> WW -> 2l2nu - taken from the note
  // here put signal total number of events (all 2l2nu)
  totalEve = 19238.000;
  // here put signal number of events after the full selection
  // eveNumber.push_back(33.000);                // loose eGamma eleID
  // eveNumber.push_back(27.000);                // tight eGamma eleID
  // eveNumber.push_back(27.000);                // H->WW AN eleID
  // eveNumber.push_back(30.000);                // new, loose for BB & narrow
  // eveNumber.push_back(30.000);                // new, tight for BB & narrow
  eveNumber.push_back(29.000);                // new, loose for BB & narrow optimized at 160  
  // eveNumberAfterEle.push_back(1102.000);       // loose eGamma eleID
  // eveNumberAfterEle.push_back(827.000);       // tight eGamma eleID
  // eveNumberAfterEle.push_back(708.000);       // H->WW AN eleID
  // eveNumberAfterEle.push_back(860.000);       // new, loose for BB & narrow
  // eveNumberAfterEle.push_back(854.000);       // new, tight for BB & narrow
  eveNumberAfterEle.push_back(866.000);       // new, loose for BB & narrow optimized at 160


  // now evaluate the expected events from Chowder CSA07
  TFile *fileChowderPDElectronSkim = 0;
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/higgsPDElectronChowder_okPresel-datasetEE-eGammaLooseEleID.root");    // loose eGamma eleID
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/higgsPDElectronChowder_okPresel-datasetEE-eGammaTightEleID.root");    // tight eGamma eleID
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/higgsPDElectronChowder_okPresel-datasetEE-hwwAnEleID.root");          // H->WW AN eleID
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/higgsPDElectronChowder_okPresel-datasetEE-newEleIDothersLoose.root"); // new, loose others
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/higgsPDElectronChowder_okPresel-datasetEE-newEleIDothersTight.root"); // new, tight others
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh130/higgsPDElectronChowder_okPresel-datasetEE-newEleIDothersLoose.root");
  // fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh130/conEleIdOptimA160/higgsPDElectronChowder_okPresel-datasetEE-newEleIDothersLoose-Backm130_eleIdOptim160.root");
  fileChowderPDElectronSkim = TFile::Open("../../Results/applied/results_mh190/conEleIdOptimA160/higgsPDElectronChowder_okPresel-datasetEEe-BkgM190_eleIdOptim160.root");





  // -----------------------------------------------------------------

  // computing efficiencies
  for(unsigned int ii=0; ii<eveNumber.size(); ii++){
    efficiencyErr        .push_back( pow(eveNumber[ii]*(1- (eveNumber[ii]/totalEve) ),0.5)/totalEve );
    efficiencyErrAfterEle.push_back( pow(eveNumberAfterEle[ii]*(1- (eveNumberAfterEle[ii]/totalEve) ),0.5)/totalEve );
    efficiency           .push_back( eveNumber[ii]/totalEve ); 
    efficiencyAfterEle   .push_back( eveNumberAfterEle[ii]/totalEve ); 
  }

  // expected number of events for signal
  for(unsigned int ii=0; ii<eveNumber.size(); ii++){
    expEv           .push_back( efficiency[ii] * xsec[ii] * lumi );
    expEvAfterEle   .push_back( efficiencyAfterEle[ii] * xsec[ii] * lumi );
    expEvErr        .push_back( efficiencyErr[ii] * xsec[ii] * lumi );
    expEvErrAfterEle.push_back( efficiencyErrAfterEle[ii] * xsec[ii] * lumi );
  }


  // ALPGEN procees id:
  // 1000 + jet multiplicity for W+jets
  // 2000 + jet multiplicity for Z+jets
  // 3000 + jet multiplicity for ttbar

  TTree *treeChowderPDElectronSkim = (TTree*) fileChowderPDElectronSkim->Get("T1");

  // evaluate W+jets expected events (weights were evaluated for 1000pb-1, the equivalent lumi of CSA07 sample)
  TH1F *dummyVar          = new TH1F("dummyVar",        "dummyVar",        10,0,10000);
  TH1F *dummyVarAfterEle  = new TH1F("dummyVarAfterEle","dummyVarAfterEle",10,0,10000);
  treeChowderPDElectronSkim->Project("dummyVar",        "CSA07lumi","(CSA07processId>=1000 && CSA07processId<2000 && finalSelection)*CSA07weight");
  treeChowderPDElectronSkim->Project("dummyVarAfterEle","CSA07lumi","(CSA07processId>=1000 && CSA07processId<2000 && finalLeptons)*CSA07weight");
  float expEvWj         = float( dummyVar->Integral() ) * lumi / 1000. ;
  float expEvWjAfterEle = float( dummyVarAfterEle->Integral() ) * lumi / 1000. ;
  dummyVar->Reset();
  dummyVarAfterEle->Reset();
  expEv.push_back(expEvWj);
  expEvAfterEle.push_back(expEvWjAfterEle);

  std::cout << "Summary after the lepton selection " << std::endl;
  std::cout << "signal expected events in " << lumi << " pb-1:\t" << expEvAfterEle[0] << " +- " << expEvErrAfterEle[0] << std::endl;
  std::cout << "W+jets expected events in " << lumi << " pb-1:\t" << expEvAfterEle[1] << std::endl;

  std::cout << "Summary after the full selection " << std::endl;
  std::cout << "signal expected events in " << lumi << " pb-1:\t" << expEv[0] << " +- " << expEvErr[0] << std::endl;
  std::cout << "W+jets expected events in " << lumi << " pb-1:\t" << expEv[1] << std::endl;

}

