#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include "massDependentCuts.cc"

float quadrSum(float x1, float x2, float x3=0, float x4=0, float x5=0, float x6=0, float x7=0, float x8=0);
float yieldErrPoisson(float nEst1, float n1, float nEst2=0, float n2=0, float nEst3=0, float n3=0, float nEst4=0, float n4=0, float nEst5=0, float n5=0, float nEst6=0, float n6=0);
void estimateWjets(int njets, TString addCut);

void estimateWjetsMassDependent(int njets) {

  int mH[18] = {115,120,130,140,150,160,170,180,190,200,250,300,350,400,450,500,550,600};
  for(int i=0; i<18;i++) {
    std::cout << "mH = " << mH[i] << "\t";
    TString addCut = higgsCuts(mH[i],true);
    estimateWjets(njets,addCut);
  }
  
}

void estimateWjets(int njets, TString addCut) {

  float ee_syst_rel[2] = {0.50, 0.50};
  float mm_syst_rel[2] = {0.31, 0.30};
  float em_syst_rel[2] = {0.37, 0.26};
  float me_syst_rel[2] = {0.50, 0.50};
  
  char njcut[30];
  sprintf(njcut, "njets==%d", njets);
  char wwselcut[30];
  if(njets==0) sprintf(wwselcut,"WWSel");
  else if(njets==1) sprintf(wwselcut,"WWSel1j");
  else {
    std::cout << "Jet bin must be 0/1" << std::endl;
    return;
  }

  TFile *fileData  = TFile::Open("results_data/merged/dataset_fake_ll.root");
  TTree *treeData  = (TTree*)fileData->Get("T1");
  TH1F *WjetsEEH = new TH1F("WjetsEEH","",50,0,180);
  TH1F *WjetsMMH = new TH1F("WjetsMMH","",50,0,180);
  TH1F *WjetsEMH = new TH1F("WjetsEMH","",50,0,180);
  TH1F *WjetsMEH = new TH1F("WjetsMEH","",50,0,180);

  // weight with the fake rate probability (prompt-subtracted)
  treeData->Project("WjetsEEH","deltaPhi",(TString("(")+TString(wwselcut)+TString(" && ")+addCut+TString(" && finalstate==0)*weight*puweight")).Data());
  treeData->Project("WjetsMEH","deltaPhi",(TString("(")+TString(wwselcut)+TString(" && ")+addCut+TString(" && finalstate==3)*weight*puweight")).Data());
  treeData->Project("WjetsMMH","deltaPhi",(TString("(")+TString(wwselcut)+TString(" && ")+addCut+TString(" && finalstate==1)*weight*puweight")).Data());
  treeData->Project("WjetsEMH","deltaPhi",(TString("(")+TString(wwselcut)+TString(" && ")+addCut+TString(" && finalstate==2)*weight*puweight")).Data());

  float nEE = WjetsEEH->Integral();
  float nEE_err_stat = yieldErrPoisson(nEE,WjetsEEH->GetEntries());
  float nEE_err_syst = nEE * ee_syst_rel[njets];
  float nME = WjetsMEH->Integral();
  float nME_err_stat = yieldErrPoisson(nME,WjetsMEH->GetEntries());
  float nME_err_syst = nME * me_syst_rel[njets];
  //  float nEM, nMM, 

  std::cout.precision(2);
//   std::cout << "\t" << nEE << " +/- " << nEE_err_stat << " (stat) " << " +/- " << nEE_err_syst << " (syst)    [ +/- " << quadrSum(nEE_err_stat,nEE_err_syst) << " (tot) ]";
//   std::cout << "\t" << nME << " +/- " << nME_err_stat << " (stat) " << " +/- " << nME_err_syst << " (syst)    [ +/- " << quadrSum(nME_err_stat,nME_err_syst) << " (tot) ]";
  //  std::cout << "em:\t" << nEM << " +/- " << nEM_err_stat << " (stat) " << " +/- " << nEM_err_syst << " (syst)    [ +/- " << quadrSum(nEM_err_stat,nEM_err_syst) << " (tot) ]" << std::endl;
  //  std::cout << "mm:\t" << nMM << " +/- " << nMM_err_stat << " (stat) " << " +/- " << nMM_err_syst << " (syst)    [ +/- " << quadrSum(nMM_err_stat,nMM_err_syst) << " (tot) ]" << std::endl;

  std::cout << "\t" << nEE << " +/- " << quadrSum(nEE_err_stat,nEE_err_syst) << " (tot)";
  std::cout << "\t" << nME << " +/- " << quadrSum(nME_err_stat,nME_err_syst) << " (tot)";

  std::cout << std::endl;
}  

float quadrSum(float x1, float x2, float x3, float x4, float x5, float x6, float x7, float x8) {
  return sqrt(x1*x1 + x2*x2 + x3*x3 + x4*x4 + x5*x5 + x6*x6 + x7*x7 + x8*x8);
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
