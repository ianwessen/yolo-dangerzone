#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>

enum { ee=0, mm=1, em=2, me=3 };

// numbers filled from counters
float nEv_endWW[4];
float nEv_end0j[4];
float nEv_end1j[4];

float Rmm, Rmm_err, Ree, Ree_err;

float quadrSum(float x1, float x2, float x3=0, float x4=0, float x5=0, float x6=0, float x7=0, float x8=0);
std::pair<float,float> nVeto(float ntag, float eff2b, float eff2berr);
std::pair<float,float> nDYout(float nDYin, float nemu, float R, float sigmaR, float K, float sigmaK);
float yieldErrPoisson(float nEst1, float n1, float nEst2=0, float n2=0, float nEst3=0, float n3=0, float nEst4=0, float n4=0, float nEst5=0, float n5=0, float nEst6=0, float n6=0);
void countEvents(int mass, const char* channel);
void estimateRWWRegion();

void estimateWW() {

  // constants
  // constants
  float eff_2b = 0.438;
  float eff_2b_err = 0.069; 
  float eff_1b_softmu = 0.053; // MC

  float eff_softmu_Z = 0.867;

  float kmm = 0.592288;
  float kmm_err = 0.0139356;
  float kee = 0.422092;
  float kee_err = 0.00874687;

  TFile *fileData = TFile::Open("results_data/merged/dataset_ll.root");
  TFile *fileWW = TFile::Open("results/datasets_trees/WW_ll.root");
  TFile *fileTop = TFile::Open("results/datasets_trees/top_ll.root");
  TFile *fileWjets = TFile::Open("results/datasets_trees/Wjets_ll.root");
  TFile *fileZjets = TFile::Open("results/datasets_trees/Zjets_ll.root");
  TFile *fileDiBosons = TFile::Open("results/datasets_trees/others_ll.root");

  TTree *treeData = (TTree*)fileData->Get("T1");
  TTree *treeWW = (TTree*)fileWW->Get("T1");
  TTree *treeTop= (TTree*)fileTop->Get("T1");
  TTree *treeWjets = (TTree*)fileWjets->Get("T1");
  TTree *treeZjets = (TTree*)fileZjets->Get("T1");
  TTree *treeDiBosons = (TTree*)fileDiBosons->Get("T1");

  std::vector<TFile*> bakgrounds;
  bakgrounds.push_back(fileTop);
  bakgrounds.push_back(fileWjets);
  bakgrounds.push_back(fileZjets);
  bakgrounds.push_back(fileDiBosons);

  std::vector<float> backgroundsUnc; // dummy values so far
  backgroundsUnc.push_back(0.5);
  backgroundsUnc.push_back(0.5);
  backgroundsUnc.push_back(0.1);
  backgroundsUnc.push_back(0.1);
  
  TH1F *dataEEH = new TH1F("dataEEH","",50,0,180);
  TH1F *dataMMH = new TH1F("dataMMH","",50,0,180);
  TH1F *dataEMH = new TH1F("dataEMH","",50,0,180);
  TH1F *dataMEH = new TH1F("dataMEH","",50,0,180);
  TH1F *WWCEEH = new TH1F("WWCEEH","",50,0,180); // control region
  TH1F *WWCMMH = new TH1F("WWCMMH","",50,0,180); // control region
  TH1F *WWCEMH = new TH1F("WWCEMH","",50,0,180); // control region
  TH1F *WWCMEH = new TH1F("WWCMEH","",50,0,180); // control region
  TH1F *WWEEH = new TH1F("WWEEH","",50,0,180); // all
  TH1F *WWMMH = new TH1F("WWMMH","",50,0,180); // all
  TH1F *WWEMH = new TH1F("WWEMH","",50,0,180); // all
  TH1F *WWMEH = new TH1F("WWMEH","",50,0,180); // all
  TH1F *topEEH = new TH1F("topEEH","",50,0,180);
  TH1F *topMMH = new TH1F("topMMH","",50,0,180);
  TH1F *topEMH = new TH1F("topEMH","",50,0,180);
  TH1F *topMEH = new TH1F("topMEH","",50,0,180);
  TH1F *btagEEHData = new TH1F("btagEEHData","",50,0,180);
  TH1F *btagMMHData = new TH1F("btagMMHData","",50,0,180);
  TH1F *btagEMHData = new TH1F("btagEMHData","",50,0,180);
  TH1F *btagMEHData = new TH1F("btagMEHData","",50,0,180);
  TH1F *btagLLHData = new TH1F("btagLLHData","",50,0,180);
  TH1F *WjetsEEH = new TH1F("WjetsEEH","",50,0,180);
  TH1F *WjetsMEH = new TH1F("WjetsMEH","",50,0,180);
  TH1F *WjetsMMH = new TH1F("WjetsMMH","",50,0,180);
  TH1F *WjetsEMH = new TH1F("WjetsEMH","",50,0,180);
  TH1F *ZeejetsH = new TH1F("ZeejetsH","",50,0,180);
  TH1F *ZmmjetsH = new TH1F("ZmmjetsH","",50,0,180);
  TH1F *ZemjetsH = new TH1F("ZemjetsH","",50,0,180);
  TH1F *ZmejetsH = new TH1F("ZmejetsH","",50,0,180);
  TH1F *neeInH = new TH1F("neeInH","",50,0,180);
  TH1F *nmmInH = new TH1F("nmmInH","",50,0,180);
  TH1F *nemInH = new TH1F("nemInH","",50,0,180);
  TH1F *nmeInH = new TH1F("nmeInH","",50,0,180);
  TH1F *DiBosonsEEH = new TH1F("DiBosonsEEH","",50,0,180);
  TH1F *DiBosonsMMH = new TH1F("DiBosonsMMH","",50,0,180);
  TH1F *DiBosonsEMH = new TH1F("DiBosonsEMH","",50,0,180);
  TH1F *DiBosonsMEH = new TH1F("DiBosonsMEH","",50,0,180);

  treeData->Project("dataEEH","deltaPhi","eleInvMass>100 && WWSel && finalstate==0");
  treeData->Project("dataMMH","deltaPhi","eleInvMass>100 && WWSel && finalstate==1");
  treeData->Project("dataEMH","deltaPhi","eleInvMass>100 && WWSel && finalstate==2");
  treeData->Project("dataMEH","deltaPhi","eleInvMass>100 && WWSel && finalstate==3");
  treeWW->Project("WWCEEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==0)*weight*puweight");
  treeWW->Project("WWCMMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==1)*weight*puweight");
  treeWW->Project("WWCEMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==2)*weight*puweight");
  treeWW->Project("WWCMEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==3)*weight*puweight");
  treeWW->Project("WWEEH","deltaPhi","(WWSel && finalstate==0)*weight*puweight");
  treeWW->Project("WWMMH","deltaPhi","(WWSel && finalstate==1)*weight*puweight");
  treeWW->Project("WWEMH","deltaPhi","(WWSel && finalstate==2)*weight*puweight");
  treeWW->Project("WWMEH","deltaPhi","(WWSel && finalstate==3)*weight*puweight");
  treeTop->Project("topEEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==0)*weight*puweight");
  treeTop->Project("topMMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==1)*weight*puweight");
  treeTop->Project("topEMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==2)*weight*puweight");
  treeTop->Project("topMEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==3)*weight*puweight");
  treeData->Project("btagEEHData","deltaPhi","eleInvMass>100 && jetVeto && bTagTrackCount>2.1 && finalstate==0");
  treeData->Project("btagMMHData","deltaPhi","eleInvMass>100 && jetVeto && bTagTrackCount>2.1 && finalstate==1");
  treeData->Project("btagEMHData","deltaPhi","eleInvMass>100 && jetVeto && bTagTrackCount>2.1 && finalstate==2");
  treeData->Project("btagMEHData","deltaPhi","eleInvMass>100 && jetVeto && bTagTrackCount>2.1 && finalstate==3");
  treeData->Project("btagLLHData","deltaPhi","eleInvMass>100 && jetVeto && bTagTrackCount>2.1");
  treeWjets->Project("WjetsEEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==0)*weight*puweight");
  treeWjets->Project("WjetsMEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==3)*weight*puweight");
  treeWjets->Project("WjetsMMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==1)*weight*puweight");
  treeWjets->Project("WjetsEMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==2)*weight*puweight");
  treeZjets->Project("ZeejetsH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==0)*weight*puweight");
  treeZjets->Project("ZmmjetsH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==1)*weight*puweight");
  treeZjets->Project("ZemjetsH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==2)*weight*puweight");
  treeZjets->Project("ZmejetsH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==3)*weight*puweight");
  treeData->Project("neeInH","deltaPhi","finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==0"); // missing softmu... not available in red trees... hopefully small contrib here
  treeData->Project("nmmInH","deltaPhi","finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==1"); // missing softmu... not available in red trees... hopefully small contrib here
  treeData->Project("nemInH","deltaPhi","finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==2"); // missing softmu... not available in red trees... hopefully small contrib here
  treeData->Project("nmeInH","deltaPhi","finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==3"); // missing softmu... not available in red trees... hopefully small contrib here
  treeDiBosons->Project("DiBosonsEEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==0)*weight*puweight");
  treeDiBosons->Project("DiBosonsMMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==1)*weight*puweight");
  treeDiBosons->Project("DiBosonsEMH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==2)*weight*puweight");
  treeDiBosons->Project("DiBosonsMEH","deltaPhi","(eleInvMass>100 && WWSel && finalstate==3)*weight*puweight");

  ///// TOP ESTIMATION ///////
  float nTopOut = topEEH->Integral() + topMMH->Integral() + topEMH->Integral() + topMEH->Integral();
  float nTopOut_err = yieldErrPoisson(nTopOut,topEEH->GetEntries()+topMMH->GetEntries()+topMEH->GetEntries()+topEMH->GetEntries());
  float eff_2b_softmu = 1 - pow(1-eff_1b_softmu,2);

  // top estimation from data (0-jet bin method)
  float nBTagTagOut_data_tot(0), nTopOutBTagVeto_data_tot(0);
  float nBTagTagOut_data, nTopOutBTagVeto_data, nTopOutBTagVeto_data_err;
  float nTopOutSoftMuVeto_data[4], nTopOutSoftMuVeto_data_err[4];

  //EE
  nBTagTagOut_data = btagEEHData->Integral();
  nBTagTagOut_data_tot += nBTagTagOut_data;
  nTopOutBTagVeto_data = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).first;
  nTopOutBTagVeto_data_tot += nTopOutBTagVeto_data;
  nTopOutBTagVeto_data_err = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).second; 
  nTopOutSoftMuVeto_data[ee] = nTopOutBTagVeto_data * (1-eff_2b_softmu); // efficiency of passing the soft muon veto (both the b's).
  nTopOutSoftMuVeto_data_err[ee] = nTopOutBTagVeto_data_err * (1-eff_2b_softmu); 

  //MM
  nBTagTagOut_data = btagMMHData->Integral();
  nBTagTagOut_data_tot += nBTagTagOut_data;
  nTopOutBTagVeto_data = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).first;
  nTopOutBTagVeto_data_tot += nTopOutBTagVeto_data;
  nTopOutBTagVeto_data_err = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).second; 
  nTopOutSoftMuVeto_data[mm] = nTopOutBTagVeto_data * (1-eff_2b_softmu); // efficiency of passing the soft muon veto (both the b's).
  nTopOutSoftMuVeto_data_err[mm] = nTopOutBTagVeto_data_err * (1-eff_2b_softmu); 

  //EM
  nBTagTagOut_data = btagEMHData->Integral();
  nBTagTagOut_data_tot += nBTagTagOut_data;
  nTopOutBTagVeto_data = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).first;
  nTopOutBTagVeto_data_tot += nTopOutBTagVeto_data;
  nTopOutBTagVeto_data_err = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).second; 
  nTopOutSoftMuVeto_data[em] = nTopOutBTagVeto_data * (1-eff_2b_softmu); // efficiency of passing the soft muon veto (both the b's).
  nTopOutSoftMuVeto_data_err[em] = nTopOutBTagVeto_data_err * (1-eff_2b_softmu); 

  //ME
  nBTagTagOut_data = btagMEHData->Integral();
  nBTagTagOut_data_tot += nBTagTagOut_data;
  nTopOutBTagVeto_data = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).first;
  nTopOutBTagVeto_data_tot += nTopOutBTagVeto_data;
  nTopOutBTagVeto_data_err = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).second; 
  nTopOutSoftMuVeto_data[me] = nTopOutBTagVeto_data * (1-eff_2b_softmu); // efficiency of passing the soft muon veto (both the b's).
  nTopOutSoftMuVeto_data_err[me] = nTopOutBTagVeto_data_err * (1-eff_2b_softmu); 

//   float nTopOutSoftMuVeto_data_tot = nTopOutSoftMuVeto_data[ee] + nTopOutSoftMuVeto_data[mm] + nTopOutSoftMuVeto_data[em] + nTopOutSoftMuVeto_data[me];
//   float nTopOutSoftMuVeto_data_tot_err = quadrSum(nTopOutSoftMuVeto_data_err[ee],nTopOutSoftMuVeto_data_err[mm],nTopOutSoftMuVeto_data_err[em],nTopOutSoftMuVeto_data_err[me]);

  // LL
  nBTagTagOut_data = btagLLHData->Integral();
  nTopOutBTagVeto_data = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).first;
  nTopOutBTagVeto_data_err = (nVeto(nBTagTagOut_data, eff_2b, eff_2b_err)).second; 
  float nTopOutSoftMuVeto_data_tot = nTopOutBTagVeto_data * (1-eff_2b_softmu); // efficiency of passing the soft muon veto (both the b's).
  float nTopOutSoftMuVeto_data_tot_err = nTopOutBTagVeto_data_err * (1-eff_2b_softmu); 


  std::cout << "TOP ESTIMATION..." << std::endl;
  std::cout << "Using eff_2b = " << eff_2b << " +/- " << eff_2b_err << std::endl;
  std::cout << "Using eff_2b_softmu = " << eff_2b_softmu << std::endl;
  std::cout << "Tagged events = " << nBTagTagOut_data
            << "   Top out from data after btag veto = " << nTopOutBTagVeto_data_tot << " +/- " << nTopOutBTagVeto_data_err // error not eval
            << "   Top out from data after soft mu veto =  " << nTopOutSoftMuVeto_data_tot << " +/- " << nTopOutSoftMuVeto_data_tot_err << std::endl;

  std::cout << "Top from MC = " << nTopOut << " +/- " << nTopOut_err << "\tTop from data = " << nTopOutSoftMuVeto_data_tot << " +/-" << nTopOutSoftMuVeto_data_tot_err << std::endl;
  std::cout << "END TOP ESTIMATION." << std::endl;
  ///// END TOP /////////


  /// W + JETS ESTIMATION ////
  // MC estimation
  float nWjetsEEOut = WjetsEEH->Integral();
  float nWjetsEEOut_err = yieldErrPoisson(nWjetsEEOut,WjetsEEH->GetEntries());
  float nWjetsMEOut = WjetsMEH->Integral();
  float nWjetsMEOut_err = yieldErrPoisson(nWjetsMEOut,WjetsMEH->GetEntries());
  float nWjetsMMOut = WjetsMMH->Integral();
  float nWjetsMMOut_err = yieldErrPoisson(nWjetsMMOut,WjetsMMH->GetEntries());
  float nWjetsEMOut = WjetsEMH->Integral();
  float nWjetsEMOut_err = yieldErrPoisson(nWjetsEMOut,WjetsEMH->GetEntries());

  float nWjetsOut = nWjetsEEOut + nWjetsMEOut + nWjetsMMOut + nWjetsEMOut;
  float nWjetsOut_err = quadrSum(nWjetsEEOut_err,nWjetsMEOut_err,nWjetsMMOut_err,nWjetsEMOut_err);

  // data estimation (ee)
  float nWjetsEEOutData = 0.478;
  float nWjetsEEOutData_err = 0.0983;
  float nWjetsMEOutData = 0.752; 
  float nWjetsMEOutData_err = 0.116;
  float nWjetsMMOutData = 0.073;
  float nWjetsMMOutData_err = 0.0392;
  float nWjetsEMOutData = 0.;
  float nWjetsEMOutData_err = 2.3;

  // 
  float nWjetsOutData = nWjetsEEOutData + nWjetsMEOutData + nWjetsMMOutData + nWjetsEMOutData;
  float nWjetsOutData_err = quadrSum(nWjetsEEOutData_err,nWjetsMEOutData_err,nWjetsMMOutData_err,nWjetsEMOutData_err);
  /////////////////////////////
  
  // DY estimation /////
  std::cout << "DY ESTIMATION..." << std::endl;

  estimateRWWRegion();
  std::cout << "first calculate R WW region/on-peak" << std::endl;
  std::cout << "Ree = " << Ree << " +/- " << Ree_err << std::endl;
  std::cout << "Rmm = " << Rmm << " +/- " << Rmm_err << std::endl;

  float nZeejetsOut = ZeejetsH->Integral();
  float nZeejetsOut_err = yieldErrPoisson(nZeejetsOut,ZeejetsH->GetEntries());
  float nZmmjetsOut = ZmmjetsH->Integral();
  float nZmmjetsOut_err = yieldErrPoisson(nZmmjetsOut,ZmmjetsH->GetEntries());
  float nZemjetsOut = ZemjetsH->Integral();
  float nZemjetsOut_err = yieldErrPoisson(nZemjetsOut,ZemjetsH->GetEntries());
  float nZmejetsOut = ZmejetsH->Integral();
  float nZmejetsOut_err = yieldErrPoisson(nZmejetsOut,ZmejetsH->GetEntries());

  float neeIn = neeInH->Integral() * eff_softmu_Z;
  float nmmIn = nmmInH->Integral() * eff_softmu_Z;
  float nemIn = nemInH->Integral() * eff_softmu_Z;
  float nmeIn = nmeInH->Integral() * eff_softmu_Z;

  float neeExp = (nDYout(neeIn, nemIn+nmeIn, Ree, Ree_err, kee, kee_err)).first;
  float neeExp_err = (nDYout(neeIn, nemIn+nmeIn, Ree, Ree_err, kee, kee_err)).second;
  float nmmExp = (nDYout(nmmIn, nemIn+nmeIn, Rmm, Rmm_err, kmm, kmm_err)).first;
  float nmmExp_err = (nDYout(nmmIn, nemIn+nmeIn, Rmm, Rmm_err, kmm, kmm_err)).second;

  // ee and mm from data and em from MC
  float nemExp = nZemjetsOut;
  float nemExp_err = yieldErrPoisson(nZemjetsOut,ZemjetsH->GetEntries());
  float nmeExp = nZmejetsOut;
  float nmeExp_err = yieldErrPoisson(nZmejetsOut,ZmejetsH->GetEntries());

  std::cout << "neeIn = " << neeIn << "\tnmmIn = " << nmmIn << "\tnemIn+nmeIn = " << nemIn+nmeIn << std::endl;
  std::cout << "nEE MC = " << nZeejetsOut << "\tData = " << neeExp << " +/- " << neeExp_err << std::endl;
  std::cout << "nMM MC = " << nZmmjetsOut << "\tData = " << nmmExp << " +/- " << nmmExp_err << std::endl;
  std::cout << "nEM MC = " << nemExp << " +/- " << nemExp_err << std::endl; 
  std::cout << "nME MC = " << nmeExp << " +/- " << nmeExp_err << std::endl; 

  std::cout << "END DY ESTIMATION." << std::endl;
  ////////// END DY ///////////

  float nDiBosonsOut = DiBosonsEEH->Integral() + DiBosonsMMH->Integral() + DiBosonsEMH->Integral() + DiBosonsMEH->Integral();
  float nDiBosonsOut_err = yieldErrPoisson(nDiBosonsOut,DiBosonsEEH->GetEntries()+DiBosonsMMH->GetEntries()+DiBosonsEMH->GetEntries()+DiBosonsMEH->GetEntries());

  float nDiBosonsOutCha[4], nDiBosonsOutCha_err[4]; 
  nDiBosonsOutCha[ee] = DiBosonsEEH->Integral();
  nDiBosonsOutCha_err[ee] = yieldErrPoisson(nDiBosonsOutCha[ee],DiBosonsEEH->GetEntries());
  nDiBosonsOutCha[mm] = DiBosonsMMH->Integral();
  nDiBosonsOutCha_err[mm] = yieldErrPoisson(nDiBosonsOutCha[mm],DiBosonsMMH->GetEntries());
  nDiBosonsOutCha[em] = DiBosonsEMH->Integral();
  nDiBosonsOutCha_err[em] = yieldErrPoisson(nDiBosonsOutCha[em],DiBosonsEMH->GetEntries());
  nDiBosonsOutCha[me] = DiBosonsMEH->Integral();
  nDiBosonsOutCha_err[me] = yieldErrPoisson(nDiBosonsOutCha[me],DiBosonsMEH->GetEntries());

  std::cout << "--> Dibosons from MC = " << nDiBosonsOut << " +/- " << nDiBosonsOut_err << std::endl;
  
  // sum of the backgrounds ///
  // data estimation (where possible)
  float DYTot = neeExp + nmmExp + nemExp + nmeExp;
  float DYTot_err = quadrSum(neeExp_err,nmmExp_err,nemExp_err,nmeExp_err);
  float bkgTot = nWjetsOutData + nTopOutSoftMuVeto_data_tot + DYTot + nDiBosonsOut;
  float bkgTot_err = quadrSum(nWjetsOutData_err,nTopOutSoftMuVeto_data_tot_err,DYTot_err,nDiBosonsOut_err);

  float bkgTotCha[4], bkgTotCha_err[4];
  bkgTotCha[ee] = nWjetsEEOutData + nTopOutSoftMuVeto_data[ee] + neeExp + nDiBosonsOutCha[ee];
  bkgTotCha_err[ee] = quadrSum(nWjetsEEOutData_err,nTopOutSoftMuVeto_data_err[ee],neeExp_err,nDiBosonsOutCha_err[ee]);

  bkgTotCha[mm] = nWjetsMMOutData + nTopOutSoftMuVeto_data[mm] + nmmExp + nDiBosonsOutCha[mm];
  bkgTotCha_err[mm] = quadrSum(nWjetsMMOutData_err,nTopOutSoftMuVeto_data_err[mm],nmmExp_err,nDiBosonsOutCha_err[mm]);

  bkgTotCha[em] = nWjetsEMOutData + nTopOutSoftMuVeto_data[em] + nemExp + nDiBosonsOutCha[em];
  bkgTotCha_err[em] = quadrSum(nWjetsEMOutData_err,nTopOutSoftMuVeto_data_err[em],nemExp_err,nDiBosonsOutCha_err[em]);

  bkgTotCha[me] = nWjetsMEOutData + nTopOutSoftMuVeto_data[me] + nmeExp + nDiBosonsOutCha[me];
  bkgTotCha_err[me] = quadrSum(nWjetsMEOutData_err,nTopOutSoftMuVeto_data_err[me],nmeExp_err,nDiBosonsOutCha_err[me]);

  // MC estimation
  float DYTotMC = nZeejetsOut + nZmmjetsOut + nZemjetsOut + nZmejetsOut;
  float DYTotMC_err = quadrSum(nZeejetsOut_err,nZmmjetsOut_err,nZemjetsOut_err,nZmejetsOut_err);
  float bkgTotMC = nWjetsOut + nTopOut + DYTotMC + nDiBosonsOut;
  float bkgTotMC_err = quadrSum(nWjetsOut_err,nTopOut_err,DYTotMC_err,nDiBosonsOut_err);

  ///// SUMMARY BKG ////
  std::cout << "---->  BACKGROUND SUMMARY  <-------" << std::endl;
  std::cout << "bkg\t\tMC\t\t\tdata" << std::endl;   
  std::cout.precision(3);
  std::cout << "W+jets =\t" << nWjetsOut << " +/- " << nWjetsOut_err << "\t\t" << nWjetsOutData << " +/- " << nWjetsOutData_err << std::endl;
  std::cout << "top =\t\t" << nTopOut << " +/- " << nTopOut_err << "\t\t" << nTopOutSoftMuVeto_data_tot << " +/-" << nTopOutSoftMuVeto_data_tot_err << std::endl;
  std::cout << "DY =\t\t" << DYTotMC << " +/- " << DYTotMC_err << "\t\t\t" << DYTot << " +/- " << DYTot_err << std::endl;
  std::cout << "WZ,ZZ =\t\t" << nDiBosonsOut << " +/- " << nDiBosonsOut_err << "\tn.a." << std::endl;
  std::cout << "TOTAL:\n\t\t" << bkgTotMC << " +/- " << bkgTotMC_err << "\t\t" << bkgTot << " +/-" << bkgTot_err << std::endl;
  std::cout << "-----------------------------------\n\n\n" << std::endl;
  /////////////////////

  // WW 
  float nDataOut[4],nDataOut_err[4];
  nDataOut[ee] = dataEEH->Integral();
  nDataOut[mm] = dataMMH->Integral();
  nDataOut[em] = dataEMH->Integral();
  nDataOut[me] = dataMEH->Integral();
  // since we extrapolate Out -> In, assign the stat error to Out
  nDataOut_err[ee] = yieldErrPoisson(nDataOut[ee],dataEEH->GetEntries());
  nDataOut_err[mm] = yieldErrPoisson(nDataOut[mm],dataMMH->GetEntries());
  nDataOut_err[em] = yieldErrPoisson(nDataOut[em],dataEMH->GetEntries());
  nDataOut_err[me] = yieldErrPoisson(nDataOut[me],dataMEH->GetEntries());

  float nWWOutData[4], nWWOutData_err[4];
  nWWOutData[ee] = nDataOut[ee] - bkgTotCha[ee];
  nWWOutData_err[ee] = quadrSum(nDataOut_err[ee],bkgTotCha_err[ee]);
  nWWOutData[mm] = nDataOut[mm] - bkgTotCha[mm];
  nWWOutData_err[mm] = quadrSum(nDataOut_err[mm],bkgTotCha_err[mm]);
  nWWOutData[em] = nDataOut[em] - bkgTotCha[em];
  nWWOutData_err[em] = quadrSum(nDataOut_err[em],bkgTotCha_err[em]);
  nWWOutData[me] = nDataOut[me] - bkgTotCha[me];
  nWWOutData_err[me] = quadrSum(nDataOut_err[me],bkgTotCha_err[me]);

  float nWWOutData_tot = nWWOutData[ee]+nWWOutData[mm]+nWWOutData[em]+nWWOutData[me];
  float nWWOutData_tot_err = quadrSum(nWWOutData_err[ee],nWWOutData_err[mm],nWWOutData_err[em],nWWOutData_err[me]);

  // MC estimation of WW
  float nWWOutMC[4], nWWOutMC_err[4];
  nWWOutMC[ee] = WWCEEH->Integral();
  nWWOutMC_err[ee] = yieldErrPoisson(nWWOutMC[ee],WWCEEH->GetEntries());
  nWWOutMC[mm] = WWCMMH->Integral();
  nWWOutMC_err[mm] = yieldErrPoisson(nWWOutMC[mm],WWCMMH->GetEntries());
  nWWOutMC[em] = WWCEMH->Integral();
  nWWOutMC_err[em] = yieldErrPoisson(nWWOutMC[em],WWCEMH->GetEntries());
  nWWOutMC[me] = WWCMEH->Integral();
  nWWOutMC_err[me] = yieldErrPoisson(nWWOutMC[me],WWCMEH->GetEntries());

  float nWWOutMC_tot = nWWOutMC[ee]+nWWOutMC[mm]+nWWOutMC[em]+nWWOutMC[me];
  float nWWOutMC_tot_err = quadrSum(nWWOutMC_err[ee],nWWOutMC_err[mm],nWWOutMC_err[em],nWWOutMC_err[me]);

  std::cout << "WW CONTROL REGION:" << std::endl;
  std::cout << "MC = " << nWWOutMC_tot << " +/- " << nWWOutMC_tot_err 
            << "\t\tData = " << nWWOutData_tot << " +/- " << nWWOutData_tot_err << std::endl;

  float dataOmc = nWWOutData_tot/nWWOutMC_tot;
  float dataOmc_err = dataOmc * quadrSum(nWWOutData_tot_err/nWWOutData_tot,nWWOutMC_tot_err/nWWOutMC_tot);

  std::cout << "Scale factor data / MC = " << dataOmc << " +/- " << dataOmc_err << std::endl;

  // Ratio Signal region / Control region
  float nWWMC[4], nWWMC_err[4];
  nWWMC[ee] = WWEEH->Integral();
  nWWMC_err[ee] = yieldErrPoisson(nWWMC[ee],WWEEH->GetEntries());
  nWWMC[mm] = WWMMH->Integral();
  nWWMC_err[mm] = yieldErrPoisson(nWWMC[mm],WWMMH->GetEntries());
  nWWMC[em] = WWEMH->Integral();
  nWWMC_err[em] = yieldErrPoisson(nWWMC[em],WWEMH->GetEntries());
  nWWMC[me] = WWMEH->Integral();
  nWWMC_err[me] = yieldErrPoisson(nWWMC[me],WWMEH->GetEntries());

  float nWWMC_tot = nWWMC[ee]+nWWMC[mm]+nWWMC[em]+nWWMC[me];
  float nWWMC_tot_err = quadrSum(nWWMC_err[ee],nWWMC_err[mm],nWWMC_err[em],nWWMC_err[me]);

  /// WW CHANNEL FRACTIONS FROM MC ///
  // they are used in case of low stat to use the summed WW estimation in the 4 sub-channels
  std::cout << "Using channel fractions from MC!" << std::endl;
  float frac[4];
  for(int icha=0; icha<4; icha++) {
    frac[icha] = nWWMC[icha]/nWWMC_tot;
    std::cout << "Fraction of WW in channel " << icha << " = " << frac[icha] << std::endl;
  }

  float RSC[4],RSC_err[4];
  for(int icha=0; icha<4;icha++) {
    RSC[icha] = nWWMC[icha]/nWWOutMC[icha];
    RSC_err[icha] = RSC[icha] * quadrSum(nWWMC_err[icha]/nWWMC[icha],nWWOutMC_err[icha]/nWWOutMC[icha]);
  }

  // The WW for the wole region estimated from data at the WW sel level
  float nWWData_WWSel[4], nWWData_WWSel_err[4];
  for(int icha=0; icha<4;icha++) {
    nWWData_WWSel[icha] = RSC[icha] * nWWOutData[icha];
    nWWData_WWSel_err[icha] = nWWData_WWSel[icha] * quadrSum(RSC_err[icha]/RSC[icha], nWWOutData_err[icha]/nWWOutData[icha]);
  }

  float nWWData_WWSel_tot = nWWData_WWSel[ee]+nWWData_WWSel[mm]+nWWData_WWSel[em]+nWWData_WWSel[me];
  float nWWData_WWSel_tot_err = quadrSum(nWWData_WWSel_err[ee],nWWData_WWSel_err[mm],nWWData_WWSel_err[em],nWWData_WWSel_err[me]);

  std::cout << "===> WW ESTIMATION AT THE WW SELECTION LEVEL <===" << std::endl;
  std::cout << "MC = " << nWWMC_tot << " +/- " << nWWMC_tot_err << std::endl;
  std::cout << "data = " << nWWData_WWSel_tot << " +/- " << nWWData_WWSel_tot_err << std::endl;
  std::cout << "=================================================" << std::endl; 

  ofstream textfile;
  textfile.open("WWYieldsData.txt", ios_base::trunc);
  textfile.precision(2);

  ofstream tablefile1;
  tablefile1.open("WWYieldsData_ForTable_0j.txt", ios_base::trunc);
  tablefile1.precision(2);

  ofstream tablefile2;
  tablefile2.open("WWYieldsData_ForTable_1j.txt", ios_base::trunc);
  tablefile2.precision(2);

  ofstream tablefile3;
  tablefile3.open("WWYieldsMC_ForTable_0j.txt", ios_base::trunc);
  tablefile3.precision(2);

  ofstream tablefile4;
  tablefile4.open("WWYieldsMC_ForTable_1j.txt", ios_base::trunc);
  tablefile4.precision(2);

  int masses[17] = {120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600};
  // -------------------------------------------------------------------
  // now considering all masses to estimate the number of events at the end of the HWW selection
  for (int i=0; i<17; i++) {
    
    int mass = masses[i];

    countEvents(mass,"EE");
    countEvents(mass,"MM");
    countEvents(mass,"EM");
    countEvents(mass,"ME");

    float nWWData_HiggsSel_0j[4], nWWData_HiggsSel_0j_err[4];
    float nWWMC_HiggsSel_0j[4], nWWMC_HiggsSel_0j_err[4];

    float nWWData_HiggsSel_1j[4], nWWData_HiggsSel_1j_err[4];
    float nWWMC_HiggsSel_1j[4], nWWMC_HiggsSel_1j_err[4];

    for(int icha=0;icha<4;icha++) {
      float eff_0j = (nEv_endWW[icha]==0) ? 0. : nEv_end0j[icha]/nEv_endWW[icha];
      float eff_0j_err = (nEv_endWW[icha]==0) ? 0. : sqrt(eff_0j*(1-eff_0j)/nEv_endWW[icha]);
      
      // this is the correct esztimation for when we have sufficient stat
      // nWWData_HiggsSel_0j[icha] = nWWData_WWSel[icha] * eff_0j;
      // nWWData_HiggsSel_0j_err[icha] = nWWData_HiggsSel_0j[icha] * quadrSum(nWWData_WWSel_err[icha]/nWWData_WWSel[icha],eff_0j_err/eff_0j);
      // and this is using fraction of the subchannel from MC
      nWWData_HiggsSel_0j[icha] = nWWData_WWSel_tot * frac[icha] * eff_0j;
      nWWData_HiggsSel_0j_err[icha] = nWWData_HiggsSel_0j[icha] * quadrSum(nWWData_WWSel_tot_err/nWWData_WWSel_tot,eff_0j_err/eff_0j);

      nWWMC_HiggsSel_0j[icha] = nWWMC[icha] * eff_0j;
      nWWMC_HiggsSel_0j_err[icha] = nWWMC_HiggsSel_0j[icha] * quadrSum(nWWMC_err[icha]/nWWMC[icha],eff_0j_err/eff_0j);

      float eff_1j = nEv_end1j[icha]/nEv_endWW[icha];
      float eff_1j_err = sqrt(eff_1j*(1-eff_1j)/nEv_endWW[icha]);

      // this is the correct esztimation for when we have sufficient stat
      // nWWData_HiggsSel_1j[icha] = nWWData_WWSel[icha] * eff_1j;
      // nWWData_HiggsSel_1j_err[icha] = nWWData_HiggsSel_1j[icha] * quadrSum(nWWData_WWSel_err[icha]/nWWData_WWSel[icha],eff_1j_err/eff_1j);
      nWWData_HiggsSel_1j[icha] = nWWData_WWSel_tot * frac[icha] * eff_1j;
      nWWData_HiggsSel_1j_err[icha] =  nWWData_HiggsSel_1j[icha] * quadrSum(nWWData_WWSel_tot_err/nWWData_WWSel_tot,eff_1j_err/eff_1j);      

      nWWMC_HiggsSel_1j[icha] = nWWMC[icha] * eff_1j;
      nWWMC_HiggsSel_1j_err[icha] = nWWMC_HiggsSel_1j[icha] * quadrSum(nWWMC_err[icha]/nWWMC[icha],eff_1j_err/eff_1j);

      char channelName[2];
      if(icha==ee) sprintf(channelName,"EE");
      if(icha==mm) sprintf(channelName,"MM");
      if(icha==em) sprintf(channelName,"EM");
      if(icha==me) sprintf(channelName,"ME");

      textfile << channelName << ": Higgs Mass = " << mass
               << "\tdata 0 jet = " << nWWData_HiggsSel_0j[icha] << " +/- " << nWWData_HiggsSel_0j_err[icha]
               << "\tMC 0 jet = " << nWWMC_HiggsSel_0j[icha] << " +/- " << nWWMC_HiggsSel_0j_err[icha]
               << "\tdata 1 jet = " << nWWData_HiggsSel_1j[icha] << " +/- " << nWWData_HiggsSel_1j_err[icha]
               << "\tMC 1 jet = " << nWWMC_HiggsSel_1j[icha] << " +/- " << nWWMC_HiggsSel_1j_err[icha]
               << std::endl;
    }


    // summary table for limits                                                                                          
    if (i==0) {
      tablefile1 << "zero jets bin data" << endl;
      tablefile1 << "\t mumu \t mue \t emu \t ee" << endl;
    }
    tablefile1 << mass
               << " " << "\t" << 150./126. * nWWData_HiggsSel_0j[1] << " +/- " << nWWData_HiggsSel_0j_err[1]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_0j[3] << " +/- " << nWWData_HiggsSel_0j_err[3]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_0j[2] << " +/- " << nWWData_HiggsSel_0j_err[2]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_0j[0] << " +/- " << nWWData_HiggsSel_0j_err[0]
               << std::endl;

    if (i==0) {
      tablefile2 << "one jet bin data" << endl;
      tablefile2 << "\t mumu \t mue \t emu \t ee" << endl;
    }
    tablefile2 << mass
               << " " << "\t" << 150./126. * nWWData_HiggsSel_1j[1] << " +/- " << nWWData_HiggsSel_1j_err[1]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_1j[3] << " +/- " << nWWData_HiggsSel_1j_err[3]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_1j[2] << " +/- " << nWWData_HiggsSel_1j_err[2]
               << " " << "\t" << 150./126. * nWWData_HiggsSel_1j[0] << " +/- " << nWWData_HiggsSel_1j_err[0]
               << std::endl;

    if (i==0) {
      tablefile3 << "zero jets bin MC" << endl;
      tablefile3 << "\t mumu \t mue \t emu \t ee" << endl;
    }
    tablefile3 << mass
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_0j[1] << " +/- " << nWWMC_HiggsSel_0j_err[1]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_0j[3] << " +/- " << nWWMC_HiggsSel_0j_err[3]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_0j[2] << " +/- " << nWWMC_HiggsSel_0j_err[2]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_0j[0] << " +/- " << nWWMC_HiggsSel_0j_err[0]
               << std::endl;

    if (i==0) {
      tablefile4 << "one jet bin MC" << endl;
      tablefile4 << "\t mumu \t mue \t emu \t ee" << endl;
    }
    tablefile4 << mass
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_1j[1] << " +/- " << nWWMC_HiggsSel_1j_err[1]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_1j[3] << " +/- " << nWWMC_HiggsSel_1j_err[3]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_1j[2] << " +/- " << nWWMC_HiggsSel_1j_err[2]
               << " " << "\t" << 150./126. * nWWMC_HiggsSel_1j[0] << " +/- " << nWWMC_HiggsSel_1j_err[0]
               << std::endl;


    float nWWData_HiggsSel_0j_Tot = nWWData_HiggsSel_0j[ee] + nWWData_HiggsSel_0j[mm] + nWWData_HiggsSel_0j[em] + nWWData_HiggsSel_0j[me];
    float nWWData_HiggsSel_0j_Tot_err = quadrSum(nWWData_HiggsSel_0j_err[ee],nWWData_HiggsSel_0j_err[mm],nWWData_HiggsSel_0j_err[em],nWWData_HiggsSel_0j_err[me]);

    float nWWMC_HiggsSel_0j_Tot = nWWMC_HiggsSel_0j[ee] + nWWMC_HiggsSel_0j[mm] + nWWMC_HiggsSel_0j[em] + nWWMC_HiggsSel_0j[me];
    float nWWMC_HiggsSel_0j_Tot_err = quadrSum(nWWMC_HiggsSel_0j_err[ee],nWWMC_HiggsSel_0j_err[mm],nWWMC_HiggsSel_0j_err[em],nWWMC_HiggsSel_0j_err[me]);

    float nWWData_HiggsSel_1j_Tot = nWWData_HiggsSel_1j[ee] + nWWData_HiggsSel_1j[mm] + nWWData_HiggsSel_1j[em] + nWWData_HiggsSel_1j[me];
    float nWWData_HiggsSel_1j_Tot_err = quadrSum(nWWData_HiggsSel_1j_err[ee],nWWData_HiggsSel_1j_err[mm],nWWData_HiggsSel_1j_err[em],nWWData_HiggsSel_1j_err[me]);

    float nWWMC_HiggsSel_1j_Tot = nWWMC_HiggsSel_1j[ee] + nWWMC_HiggsSel_1j[mm] + nWWMC_HiggsSel_1j[em] + nWWMC_HiggsSel_1j[me];
    float nWWMC_HiggsSel_1j_Tot_err = quadrSum(nWWMC_HiggsSel_1j_err[ee],nWWMC_HiggsSel_1j_err[mm],nWWMC_HiggsSel_1j_err[em],nWWMC_HiggsSel_1j_err[me]);
    
    
    textfile << "\t===>> TOTAL: Higgs Mass = " << mass 
             << "\tdata 0 jet = " << nWWData_HiggsSel_0j_Tot << " +/- " << nWWData_HiggsSel_0j_Tot_err 
             << "\tdata 1 jet = " << nWWData_HiggsSel_1j_Tot << " +/- " << nWWData_HiggsSel_1j_Tot_err
             << "\tMC 0 jet = " << nWWMC_HiggsSel_0j_Tot << " +/- " << nWWMC_HiggsSel_0j_Tot_err 
             << "\tMC 1 jet = " << nWWMC_HiggsSel_1j_Tot << " +/- " << nWWMC_HiggsSel_1j_Tot_err
             << std::endl;

  }

  std::cout << "Full WW yields in data in:  WWYieldsData.txt " << std::endl;

}

float quadrSum(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8) {
  return sqrt(x1*x1 + x2*x2 + x3*x3 + x4*x4 + x5*x5 + x6*x6 + x7*x7 + x8*x8);
}

std::pair<float,float> nVeto(float ntag, float eff2b, float eff2berr) {
  float val = ntag * (1-eff2b) / eff2b;
  float err = ntag * eff2berr / pow(eff2b,2);
  return std::make_pair(val,err);
}

std::pair<float,float> nDYout(float nDYin, float nemu, float R, float sigmaR, float K, float sigmaK) {
  float val = R * (nDYin - 0.5 * nemu * K);
  float err = sqrt(pow(sigmaR,2) * pow(nDYin - 0.5 * nemu * K , 2) + 
                   1./4. * pow(nemu,2) * R*R * sigmaK*sigmaK + 
                   nDYin * R*R +
                   nemu * 1./4. * K*K * R*R); 
  return std::make_pair(val,err);
}

float yieldErrPoisson(float nEst1, float n1, float nEst2, float n2, float nEst3, float n3, float nEst4, float n4, float nEst5, float n5, float nEst6, float n6) {

  float sum=0;
  if(n1>0) sum += pow(nEst1,2)/n1;
  if(n2>0) sum += pow(nEst2,2)/n2;
  if(n3>0) sum += pow(nEst3,2)/n3;
  if(n4>0) sum += pow(nEst4,2)/n4;
  if(n5>0) sum += pow(nEst5,2)/n5;
  if(n6>0) sum += pow(nEst6,2)/n6;
  
  return sqrt(sum);
}

void countEvents(int mass, const char *channel) {

  // taking the EE or ME trees for the wanted mass
  char nametree[200];
  sprintf(nametree,"FULL_SELECTION_EVENT_COUNTER_%s",channel);  
  TChain *theChain = new TChain(nametree);
  
  char file_mc[1000];
  sprintf(file_mc,"/cmsrm/pc24_2/emanuele/data/Higgs4.1.X/MC2011_LHLoose_V13/OptimMH%d/Spring11_V2/WWTo2L2Nu_TuneZ2_7TeV-pythia6/*Counters.root",mass);  
  theChain->Add(file_mc);
  //  cout << "reading tree " << nametree << " from file " << file_mc << endl;    
  
  int theCha=-1;
  if(TString(channel).Contains("EE")) theCha=ee;
  if(TString(channel).Contains("MM")) theCha=mm;
  if(TString(channel).Contains("EM")) theCha=em;
  if(TString(channel).Contains("ME")) theCha=me;

  // number of events at the wanted step of the selection
  nEv_endWW[theCha] = 0.0;
  nEv_end0j[theCha] = 0.0;
  nEv_end1j[theCha] = 0.0;

  // reading the tree
  Int_t    nCuts;
  Float_t  nSel[25];   //[nCuts]                                      
  TBranch  *b_nCuts;   
  TBranch  *b_nSel;    
  theChain->SetBranchAddress("nCuts", &nCuts, &b_nCuts);
  theChain->SetBranchAddress("nSel", nSel, &b_nSel);
  
  Long64_t nentries = theChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    nb = theChain->GetEntry(jentry);   
    nbytes    += nb;
    nEv_endWW[theCha] += nSel[16];
    nEv_end0j[theCha] += nSel[22];
    nEv_end1j[theCha] += nSel[23];
  }
}

void estimateRWWRegion() {
  TFile *fileZjets = TFile::Open("results/datasets_trees/Zjets_ll.root");
  TTree *treeZjets = (TTree*)fileZjets->Get("T1");
  TH1F *ZeejetsH_IN = new TH1F("ZeejetsH_IN","",50,0,180);
  TH1F *ZmmjetsH_IN = new TH1F("ZmmjetsH_IN","",50,0,180);
  TH1F *ZeejetsH_OUT = new TH1F("ZeejetsH_OUT","",50,0,180);
  TH1F *ZmmjetsH_OUT = new TH1F("ZmmjetsH_OUT","",50,0,180);

  treeZjets->Project("ZeejetsH_IN","deltaPhi","(finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==0)*weight*puweight");
  treeZjets->Project("ZmmjetsH_IN","deltaPhi","(finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && abs(eleInvMass-91.1876)<15 && finalstate==1)*weight*puweight");
  treeZjets->Project("ZeejetsH_OUT","deltaPhi","(finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && eleInvMass>100 && finalstate==0)*weight*puweight");
  treeZjets->Project("ZmmjetsH_OUT","deltaPhi","(finalLeptons && pfMet>30 && projMet>35 && njets==0 && bTagTrackCount<2.1 && eleInvMass>100 && finalstate==1)*weight*puweight");

  float nZmmjetsH_OUT = ZmmjetsH_OUT->Integral();
  float nZmmjetsH_OUT_err = yieldErrPoisson(nZmmjetsH_OUT,ZmmjetsH_OUT->GetEntries());
  float nZeejetsH_OUT = ZeejetsH_OUT->Integral();
  float nZeejetsH_OUT_err = yieldErrPoisson(nZeejetsH_OUT,ZeejetsH_OUT->GetEntries());

  float nZmmjetsH_IN = ZmmjetsH_IN->Integral();
  float nZmmjetsH_IN_err = yieldErrPoisson(nZmmjetsH_IN,ZmmjetsH_IN->GetEntries());
  float nZeejetsH_IN = ZeejetsH_IN->Integral();
  float nZeejetsH_IN_err = yieldErrPoisson(nZeejetsH_IN,ZeejetsH_IN->GetEntries());

  Rmm = nZmmjetsH_OUT/nZmmjetsH_IN;
  Rmm_err = Rmm * quadrSum(nZmmjetsH_OUT_err/nZmmjetsH_OUT, nZmmjetsH_IN_err/nZmmjetsH_IN);

  Ree = nZeejetsH_OUT/nZeejetsH_IN;
  Ree_err = Ree * quadrSum(nZeejetsH_OUT_err/nZeejetsH_OUT, nZeejetsH_IN_err/nZeejetsH_IN);

}
