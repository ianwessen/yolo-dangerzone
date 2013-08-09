#include "HiggsAnalysisTools/include/RedHiggsTree.h"

// C++
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

// Root
#include "TFile.h"
#include "TTree.h"

RedHiggsTree::RedHiggsTree(const char * filename) {

  myFile = new TFile(filename,"RECREATE");
  myTree = new TTree("latino","latino tree");

  // GENERAL block
  myTree->Branch("puweight",            &myPUWeight,            "puweight/F");
  myTree->Branch("hlt",                 &myHLT,                 "hlt/O");
  myTree->Branch("met",                 &myMet,                 "met/F");  
  myTree->Branch("pfMet",               &myPFMet,               "pfMet/F");  
  myTree->Branch("caloMet",             &myCaloMet,             "caloMet/F");  
  myTree->Branch("projMet",             &myProjectedMet,        "projMet/F");
  myTree->Branch("deltaPhi",            &myDeltaPhi,            "deltaPhi/F");  
  myTree->Branch("deltaR",              &myDeltaR,              "deltaR/F");  
  myTree->Branch("transvMass",          &myTransvMass,          "transvMass/F");  
  myTree->Branch("eleInvMass",          &myEleInvMass,          "eleInvMass/F");  
  myTree->Branch("maxPtEle",            &maxPtEle,              "maxPtEle/F");  
  myTree->Branch("minPtEle",            &minPtEle,              "minPtEle/F");  
  myTree->Branch("maxEtaEle",           &maxEtaEle,             "maxEtaEle/F");  
  myTree->Branch("minEtaEle",           &minEtaEle,             "minEtaEle/F");  
  myTree->Branch("detaLeptons",         &myDetaLeptons,         "detaLeptons/F");  
  myTree->Branch("nVtx",                &myNVtx,                "nVtx/I");
  myTree->Branch("finalLeptons",        &myFinalLeptons,        "finalLeptons/O");
  myTree->Branch("jetVeto",             &myJetVeto,             "jetVeto/O");
  myTree->Branch("uncorrJetVeto",       &myUncorrJetVeto,       "uncorrJetVeto/O");
  myTree->Branch("preDeltaPhi",         &myPreDeltaPhi,         "preDeltaPhi/O");
  myTree->Branch("finalSelection",      &myFinalSelection,      "finalSelection/O");

}

RedHiggsTree::~RedHiggsTree() 
{
  delete myFile;
}

void RedHiggsTree::addMLVars() {

  myTree->Branch("njets",               &myNjets,               "njets/I");
  myTree->Branch("nuncorrjets",         &myNuncorrjets,         "nuncorrjets/I");
  myTree->Branch("dxyEVT",              &myDxyEVT,              "dxyEVT/F");
  myTree->Branch("dszEVT",              &myDszEVT,              "dszEVT/F");
  myTree->Branch("softtche",            &mySoftBdisc,           "softtcche/F");
  myTree->Branch("hardbjpb",            &myHardBdisc,           "hardbjbp/F");
  myTree->Branch("bTagSecVertex",       &myBTagSecVertex,       "bTagSecVertex/F");
  myTree->Branch("nSoftMu",             &myNSoftMu,             "nSoftMu/I");
  myTree->Branch("nSoftMuNoJets",       &myNSoftMuNoJets,       "nSoftMuNoJets/I");
  myTree->Branch("leadingJetBTagTrackCount", &myLeadingJetBTagTrackCount,    "leadingJetBTagTrackCount/F");
  myTree->Branch("subleadingJetBTagTrackCount", &mySubleadingJetBTagTrackCount,    "subleadingJetBTagTrackCount/F");    
  myTree->Branch("subleadingJetsMaxBTagTrackCount", &mySubleadingJetsMaxBTagTrackCount,    "subleadingJetsMaxBTagTrackCount/F");    
  myTree->Branch("leadingJetBTagJetBProb", &myLeadingJetBTagJetBProb,    "leadingJetBTagJetBProb/F");
  myTree->Branch("subleadingJetBTagJetBProb", &mySubleadingJetBTagJetBProb,    "subleadingJetBTagJetBProb/F");    
  myTree->Branch("subleadingJetsMaxBTagJetBProb", &mySubleadingJetsMaxBTagJetBProb,    "subleadingJetsMaxBTagJetBProb/F");    
  myTree->Branch("numExtraLep", &myNumExtraLep, "numExtraLep/I");   
  myTree->Branch("nsoftjet", &myNumSoftJets, "nsoftjet/I");
  myTree->Branch("nsoftbjet", &myNumSoftBJets, "nsoftbjet/I");
}

void RedHiggsTree::addJetsVars() {

  myTree->Branch("leadJetPt",          &myLeadJetPt,          "leadJetPt/F");
  myTree->Branch("leadJetEta",         &myLeadJetEta,         "leadJetEta/F");
  myTree->Branch("leadJetLooseId",     &myLeadJetLooseId,     "leadJetLooseId/I");
  myTree->Branch("leadJetGenMatch",    &myLeadJetGenMatch,    "leadJetGenMatch/I");
  myTree->Branch("jetmva1",            &myLeadJetIdMva,       "jetmva1/F");
  myTree->Branch("jetid1",             &myLeadJetPassLooseId, "jetid1/I");
  myTree->Branch("subleadJetPt",       &mySubleadJetPt,       "subleadJetPt/F");
  myTree->Branch("subleadJetEta",      &mySubleadJetEta,      "subleadJetEta/F");
  myTree->Branch("subleadJetLooseId",  &mySubleadJetLooseId,  "subleadJetLooseId/I");
  myTree->Branch("subleadJetGenMatch", &mySubleadJetGenMatch, "subleadJetGenMatch/I");
  myTree->Branch("jetmva2",            &mySubleadJetIdMva,    "jetmva2/F");
  myTree->Branch("jetid2",             &mySubleadJetPassLooseId,  "jetid2/I");
}

void RedHiggsTree::addSystematics() {
  
  myTree->Branch("scEnergy", myScEnergy, "scEnergy[2]/F");
  myTree->Branch("R9", myR9, "R9[2]/F");
  myTree->Branch("eneL1",  &myEneL1,  "eneL1/F");
  myTree->Branch("eneL2",  &myEneL2, " eneL2/F");
  myTree->Branch("typeL1", &myTypeL1, "typeL1/I");
  myTree->Branch("typeL2", &myTypeL2, "typeL2/I");
  myMetFromJets = 0;
  myPfMetUp = 0;
  myPfMetDown = 0;
  myTree->Branch("metFromJets", "TVector3", &myMetFromJets);
  myTree->Branch("pfMetUp", "TVector3", &myPfMetUp);
  myTree->Branch("pfMetDown", "TVector3", &myPfMetDown);
  myTree->Branch("transvMassUp", &myMtUp, "transvMassUp/F");
  myTree->Branch("transvMassDown", &myMtDown, "transvMassDown/F");

}

void RedHiggsTree::addLeptonInfos() {
  myTree->Branch("pt",      myPt,      "pt[2]/F");
  myTree->Branch("eta",     myEta,     "eta[2]/F");
  myTree->Branch("phi",     myPhi,     "phi[2]/F");
  myTree->Branch("lepid",   myLepId,   "lepid[2]/F"); 
  myTree->Branch("lepiso",  myLepIso,  "lepiso[2]/F"); 
  myTree->Branch("lepconv", myLepConv, "lepconv[2]/F"); 
}

void RedHiggsTree::addCSA07Infos() {

  myTree->Branch("CSA07weight",              &myWeight,              "CSA07weight/D");
  myTree->Branch("CSA07processId",           &myProcesId,            "CSA07processId/D");
  myTree->Branch("CSA07lumi",                &myLumi,                "CSA07lumi/F");
}

void RedHiggsTree::addLatinos() {

  myTree->Branch("step",              mySteps,              "step[29]/O"); 
}

void RedHiggsTree::addPDFs() {
  myTree->Branch("cteq66W", myCTEQ66, "cteq66W[45]/D");
  myTree->Branch("mstwW",   myMSTW,   "mstwW[31]/D");
  myTree->Branch("nnpdfW",  myNNPDF,  "nnpdfW[101]/D");
}

void RedHiggsTree::addRazor() {

  myTree->Branch("mtr",  &myMTR,  "mtr/F");
  myTree->Branch("mr",  &myMR,  "mr/F");
  myTree->Branch("gammamr", &myGammaMR, "gammamr/F");
}

void RedHiggsTree::addFake() {
  
  myTree->Branch("tight",        &myTight,        "tight/I");
  myTree->Branch("weightFP",     &myWeightFP,     "weightFP/F");
  myTree->Branch("weightStatFP", &myWeightStatFP, "weightStatFP/F");

  myTree->Branch("weightFP15",     &myWeightFP15,     "weightFP15/F");
  myTree->Branch("weightStatFP15", &myWeightStatFP15, "weightStatFP15/F");
  myTree->Branch("weightFF15",     &myWeightFF15,     "weightFF15/F");
  myTree->Branch("weightStatFF15", &myWeightStatFF15, "weightStatFF15/F");
  myTree->Branch("weightPP15",     &myWeightPP15,     "weightPP15/F");
  myTree->Branch("weightStatPP15", &myWeightStatPP15, "weightStatPP15/F");

  myTree->Branch("weightFP35",     &myWeightFP35,     "weightFP35/F");
  myTree->Branch("weightStatFP35", &myWeightStatFP35, "weightStatFP35/F");
  myTree->Branch("weightFF35",     &myWeightFF35,     "weightFF35/F");
  myTree->Branch("weightStatFF35", &myWeightStatFF35, "weightStatFF35/F");
  myTree->Branch("weightPP35",     &myWeightPP35,     "weightPP35/F");
  myTree->Branch("weightStatPP35", &myWeightStatPP35, "weightStatPP35/F");

  myTree->Branch("weightFP50",     &myWeightFP50,     "weightFP50/F");
  myTree->Branch("weightStatFP50", &myWeightStatFP50, "weightStatFP50/F");
  myTree->Branch("weightFF50",     &myWeightFF50,     "weightFF50/F");
  myTree->Branch("weightStatFF50", &myWeightStatFF50, "weightStatFF50/F");
  myTree->Branch("weightPP50",     &myWeightPP50,     "weightPP50/F");
  myTree->Branch("weightStatPP50", &myWeightStatPP50, "weightStatPP50/F");

  myTree->Branch("weightFPQCD",     &myWeightFPQCD,     "weightFPQCD/F");
  myTree->Branch("weightStatFPQCD", &myWeightStatFPQCD, "weightStatFPQCD/F");
  myTree->Branch("weightFFQCD",     &myWeightFFQCD,     "weightFFQCD/F");
  myTree->Branch("weightStatFFQCD", &myWeightStatFFQCD, "weightStatFFQCD/F");
  myTree->Branch("weightPPQCD",     &myWeightPPQCD,     "weightPPQCD/F");
  myTree->Branch("weightStatPPQCD", &myWeightStatPPQCD, "weightStatPPQCD/F");
}


void RedHiggsTree::addKinematics() {

  myTree->Branch("pxChMet", &myPxTkMet, "pxChMet/F");
  myTree->Branch("pyChMet", &myPyTkMet, "pyChMet/F");
  myTree->Branch("pzChMet", &myPzTkMet, "pzChMet/F");
  myTree->Branch("pxLeadJet", myPxLeadJet, "pxLeadJet[3]/F"); // 0th is nominal JEC, 1st=+1sigma JES, 2nd=-1sigma JES   
  myTree->Branch("pyLeadJet", myPyLeadJet, "pyLeadJet[3]/F");
  myTree->Branch("pzLeadJet", myPzLeadJet, "pzLeadJet[3]/F");
  myTree->Branch("pxSecondJet", myPxSecondJet, "pxSecondJet[3]/F");
  myTree->Branch("pySecondJet", myPySecondJet, "pySecondJet[3]/F");
  myTree->Branch("pzSecondJet", myPzSecondJet, "pzSecondJet[3]/F");
  myTree->Branch("pxL1", &myPxL1, "pxL1/F");
  myTree->Branch("pyL1", &myPyL1, "pyL1/F");
  myTree->Branch("pzL1", &myPzL1, "pzL1/F");
  myTree->Branch("pxL2", &myPxL2, "pxL2/F");
  myTree->Branch("pyL2", &myPyL2, "pyL2/F");
  myTree->Branch("pzL2", &myPzL2, "pzL2/F");
  myTree->Branch("ch", myLepCharge, "ch[2]/I");

  myJetsSum = 0;
  myUncorrJetsSum = 0;
  myPfMet = 0;
  myTree->Branch("sumJetsV4", "TLorentzVector", &myJetsSum);
  myTree->Branch("uncorrSumJetsV4", "TLorentzVector", &myUncorrJetsSum);
  myTree->Branch("pfmetV", "TVector3", &myPfMet);
}

void RedHiggsTree::addKFactor() {
  
  myTree->Branch("KFactor",       &myKFactor,      "KFactor/F");
  myTree->Branch("GenHPt",        &myGenHPt,       "GenHPt/F");
  myTree->Branch("leadingJetPt",  &myLeadingJetPt, "leadingJetPt/F");
}

void RedHiggsTree::addMcTruthInfos() {
  myTree->Branch("promptDecay",         &myPromptDecay,         "promptDecay/O");
  myTree->Branch("genmll",              &myGenmll,              "genmll/F");
  myTree->Branch("genptll",             &myGenptll,             "genptll/F");
  myTree->Branch("genyll",              &myGenyll,              "genyll/F");
}

void RedHiggsTree::addHLTElectronsInfos() {

  myTree->Branch("HLTSingleElectron",        &myHLTSingleElectron,        "HLTSingleElectron/O");
  myTree->Branch("HLTSingleElectronRelaxed", &myHLTSingleElectronRelaxed, "HLTSingleElectronRelaxed/O");
  myTree->Branch("HLTSingleElectronOR",      &myHLTSingleElectronOR,      "HLTSingleElectronOR/O");
}

void RedHiggsTree::addHLTMuonsInfos() {

  myTree->Branch("HLTSingleMuon",        &myHLTSingleMuon,        "HLTSingleMuon/O");
  myTree->Branch("HLTSingleMuonRelaxed", &myHLTSingleMuonRelaxed, "HLTSingleMuonRelaxed/O");
  myTree->Branch("HLTSingleMuonOR",      &myHLTSingleMuonOR,      "HLTSingleMuonOR/O");
}

void RedHiggsTree::addRunInfos() {
  myTree->Branch("run", &myRun,     "run/I");
  myTree->Branch("lumi", &myLS,     "lumi/I");
  myTree->Branch("event", &myEvent, "event/l");
  myTree->Branch("npu", myNpu, "npu[3]/F");
}

void RedHiggsTree::addMetStudies() {

  myTree->Branch("projPFMet",        &myProjPFMet,        "projPFMet/F");
  myTree->Branch("projPFChargedMet", &myProjPFChargedMet, "projPFChargedMet/F");
  myTree->Branch("signPFMet",        &mySignPFMet,        "signPFMet/F");
  myTree->Branch("signPFChargedMet", &mySignPFChargedMet, "signPFChargedMet/F");
  myTree->Branch("mtrchargedMet",    &myMTRchargedMet,    "mtrchargedMet/F");
  myTree->Branch("dymva1",           &myDYMVA,            "dymva1/F");
  myTree->Branch("rho",              &myRho,              "rho/F"); 
  myTree->Branch("rhoJet",           &myRhoJet,           "rhoJet/F"); 
}

void RedHiggsTree::store()
{
  myTree->Fill();
}


void RedHiggsTree::save() 
{
  myFile->cd();
  myTree->Write();
  myFile->Close();
}


void RedHiggsTree::fillAll(float met, float pfmet, float cmet, float projmet, 
			   float dphi, float derre, float tmass, float mee, float max, float min, float deta, int nvtx,
			   bool finalLeptons, bool jetVeto, bool uncorrjetVeto, bool preDeltaPhi, bool finalSelection)
{

  myMet         = met;
  myPFMet       = pfmet;
  myCaloMet     = cmet;
  myProjectedMet = projmet;
  myDeltaPhi    = dphi;
  myDeltaR      = derre;
  myTransvMass  = tmass;
  myEleInvMass  = mee;
  maxPtEle      = max;
  minPtEle      = min;
  myDetaLeptons = deta;
  myNVtx        = nvtx;
  myFinalLeptons = finalLeptons;
  myJetVeto       = jetVeto;
  myUncorrJetVeto = uncorrjetVeto;
  myPreDeltaPhi = preDeltaPhi;
  myFinalSelection = finalSelection;
}

void RedHiggsTree::fillAll(float met, float pfmet, float cmet, float projmet, 
			   float dphi, float derre, float tmass, float mee, 
			   float max, float min, float maxEta, float minEta, float deta, int nvtx,
			   bool finalLeptons, bool jetVeto, bool uncorrjetVeto, bool preDeltaPhi, bool finalSelection)
{

  myMet         = met;
  myPFMet       = pfmet;
  myCaloMet     = cmet;
  myProjectedMet = projmet;
  myDeltaPhi    = dphi;
  myDeltaR      = derre;
  myTransvMass  = tmass;
  myEleInvMass  = mee;
  maxPtEle      = max;
  minPtEle      = min;
  maxEtaEle     = maxEta;
  minEtaEle     = minEta;
  myDetaLeptons = deta;
  myNVtx        = nvtx;
  myFinalLeptons = finalLeptons;
  myJetVeto       = jetVeto;
  myUncorrJetVeto = uncorrjetVeto;
  myPreDeltaPhi = preDeltaPhi;
  myFinalSelection = finalSelection;
}
 
 void RedHiggsTree::fillMLVars(int njets, int nuncorrjets, float dxyEVT, float dszEVT, 
                              float softbdisc, float hardbdisc, float bTagSecVertex, int nsoftmu, 
                              float leadJetBTagTrackCount, float subleadJetBTagTrackCount, float subleadJetsMaxBTagTrackCount,
                              float leadJetBTagJetBProb, float subleadJetBTagJetBProb, float subleadJetsMaxBTagJetBProb,
                              int numExtraLep, int nsoftmunojets, int nSoftBJets, int nSoftJets) {
 
  myNjets   = njets;
  myNuncorrjets = nuncorrjets;
  myDxyEVT = dxyEVT;
  myDszEVT = dszEVT;
  mySoftBdisc = softbdisc;
  myHardBdisc = hardbdisc;
  myBTagSecVertex = bTagSecVertex;
  myNSoftMu = nsoftmu;
  myNSoftMuNoJets = nsoftmunojets;
  myLeadingJetBTagTrackCount = leadJetBTagTrackCount;
  mySubleadingJetBTagTrackCount = subleadJetBTagTrackCount;
  mySubleadingJetsMaxBTagTrackCount = subleadJetsMaxBTagTrackCount;
  myLeadingJetBTagJetBProb = leadJetBTagJetBProb;
  mySubleadingJetBTagJetBProb = subleadJetBTagJetBProb;
  mySubleadingJetsMaxBTagJetBProb = subleadJetsMaxBTagJetBProb;
  myNumExtraLep = numExtraLep;
  myNumSoftBJets = nSoftBJets;
  myNumSoftJets = nSoftJets;
 }

void RedHiggsTree::fillJetsVars(float ljpt, float ljeta, int ljpfid, int ljmatch, float ljmva, int ljl, float sljpt, float sljeta, int sljpfid, int sljmatch, float sljmva, int sljl) {

  myLeadJetPt             = ljpt;
  myLeadJetEta            = ljeta;
  myLeadJetLooseId        = ljpfid;
  myLeadJetGenMatch       = ljmatch;
  myLeadJetIdMva          = ljmva;
  myLeadJetPassLooseId    = ljl;
  mySubleadJetPt          = sljpt;
  mySubleadJetEta         = sljeta;
  mySubleadJetLooseId     = sljpfid;
  mySubleadJetGenMatch    = sljmatch;
  mySubleadJetIdMva       = sljmva;
  mySubleadJetPassLooseId = sljl;
}

void RedHiggsTree::fillLatinos(bool s0, bool s1, bool s2, bool s3, bool s4, bool s5, bool s6, bool s7, bool s8, bool s9, bool s10, bool s11, bool s12, bool s13, bool s14, bool s15, bool s16, bool s17,
                               bool s18, bool s19, bool s20, bool s21, bool s22, bool s23, bool s24, bool s25, bool s26) {
  mySteps[0]  = s0;
  mySteps[1]  = s1;
  mySteps[2]  = s2;
  mySteps[3]  = s3;
  mySteps[4]  = s4;
  mySteps[5]  = s5;
  mySteps[6]  = s6;
  mySteps[7]  = s7;
  mySteps[8]  = s8;
  mySteps[9]  = s9;
  mySteps[10] = s10;
  mySteps[11] = s11;
  mySteps[12] = s12;
  mySteps[13] = s13;
  mySteps[14] = s14;
  mySteps[15] = s15;
  mySteps[16] = s16;
  mySteps[17] = s17;
  mySteps[18] = s18;
  mySteps[19] = s19;
  mySteps[20] = s20;
  mySteps[21] = s21;
  mySteps[22] = s22;
  mySteps[23] = s23;
  mySteps[24] = s24;
  mySteps[25] = s25;
  mySteps[26] = s26;
}

void RedHiggsTree::fillPDFs(double cteq66[45], double mstw[31], double nnpdf[101]) {
  for(int i=0;i<45;++i) myCTEQ66[i] = cteq66[i];
  for(int i=0;i<31;++i) myMSTW[i] = mstw[i];
  for(int i=0;i<101;++i) myNNPDF[i] = nnpdf[i];
}

void RedHiggsTree::fillRazor(float MTR, float mR, float gammaMR) {

  myMTR = MTR;
  myMR = mR;
  myGammaMR = gammaMR;
}

void RedHiggsTree::fillFake(int ntigh, float wfp, float wsfp, 
			    float wfp15, float wsfp15, float wff15, float wsff15, float wpp15, float wspp15,
			    float wfp35, float wsfp35, float wff35, float wsff35, float wpp35, float wspp35,
			    float wfp50, float wsfp50, float wff50, float wsff50, float wpp50, float wspp50,
			    float wfpQCD, float wsfpQCD, float wffQCD, float wsffQCD, float wppQCD, float wsppQCD) {
  
  myTight        = ntigh;
  myWeightFP     = wfp;
  myWeightStatFP = wsfp;
  //
  myWeightFP15     = wfp15;
  myWeightStatFP15 = wsfp15;
  myWeightFF15     = wff15;
  myWeightStatFF15 = wsff15;
  myWeightPP15     = wpp15;
  myWeightStatPP15 = wspp15;
  //
  myWeightFP35     = wfp35;
  myWeightStatFP35 = wsfp35;
  myWeightFF35     = wff35;
  myWeightStatFF35 = wsff35;
  myWeightPP35     = wpp35;
  myWeightStatPP35 = wspp35;
  //
  myWeightFP50     = wfp50;
  myWeightStatFP50 = wsfp50;
  myWeightFF50     = wff50;
  myWeightStatFF50 = wsff50;
  myWeightPP50     = wpp50;
  myWeightStatPP50 = wspp50;
  // 
  myWeightFPQCD     = wfpQCD;
  myWeightStatFPQCD = wsfpQCD;
  myWeightFFQCD     = wffQCD;
  myWeightStatFFQCD = wsffQCD;
  myWeightPPQCD     = wppQCD;
  myWeightStatPPQCD = wsppQCD;
}


void RedHiggsTree::fillKinematics(float pxTkMet, float pyTkMet, float pzTkMet,
                                  float pxLeadJet[3], float pyLeadJet[3], float pzLeadJet[3], 
                                  float pxSecJet[3], float pySecJet[3], float pzSecJet[3],    
                                  float pxL1, float pyL1, float pzL1,
                                  float pxL2, float pyL2, float pzL2,
                                  int ch[2], 
                                  TLorentzVector *jetSum, TLorentzVector *uncorrJetSum, TVector3 *pfmet) {

  myPxTkMet = pxTkMet;
  myPyTkMet = pyTkMet;
  myPzTkMet = pzTkMet;

  for(int jes=0; jes<3; jes++) {
    myPxLeadJet[jes] = pxLeadJet[jes];
    myPyLeadJet[jes] = pyLeadJet[jes];
    myPzLeadJet[jes] = pzLeadJet[jes];
    myPxSecondJet[jes] = pxSecJet[jes];
    myPySecondJet[jes] = pySecJet[jes];
    myPzSecondJet[jes] = pzSecJet[jes];
  }

  myPxL1 = pxL1;
  myPyL1 = pyL1;
  myPzL1 = pzL1;
  myPxL2 = pxL2;
  myPyL2 = pyL2;
  myPzL2 = pzL2;

  myJetsSum = jetSum;
  myUncorrJetsSum = uncorrJetSum;
  myPfMet = pfmet;
  for(int i=0; i<2; i++) {
    myLepCharge[i] = ch[i];
  }
}

void RedHiggsTree::fillSystematics(float scE[2], float r9[2], float ene1, float ene2, int ty1, int ty2, 
                                   TVector3 *metFromJets, TVector3 *pfMetUp, TVector3 *pfMetDown, float mtUp, float mtDown) {

  for(int i=0; i<2; i++) {
    myScEnergy[i] = scE[i];
    myR9[i] = r9[i];
  }

  myEneL1  = ene1;
  myEneL2  = ene2;
  myTypeL1 = ty1;
  myTypeL2 = ty2;
  myMetFromJets = metFromJets;
  myPfMetUp = pfMetUp;
  myPfMetDown = pfMetDown;  
  myMtUp = mtUp;
  myMtDown = mtDown;
}


void RedHiggsTree::fillLeptons(float pt[2], float eta[2], float phi[2], float lepid[2], float lepiso[2], float lepconv[2]) {
  for(int i=0; i<2; i++) {
    myPt[i] = pt[i];
    myEta[i] = eta[i];
    myPhi[i] = phi[i];
    myLepId[i] = lepid[i];
    myLepIso[i] = lepiso[i];
    myLepConv[i] = lepconv[i];
  }
}

void RedHiggsTree::fillCSA07(double weight, double processId, float lumi) 
{

  myWeight = weight;
  myProcesId = processId;
  myLumi = lumi;
}

void RedHiggsTree::fillKFactor(float kfactor, float genh, float ptlj ) {

  myKFactor      = kfactor;
  myGenHPt       = genh;
  myLeadingJetPt = ptlj;
}

void RedHiggsTree::fillMcTruth(bool prompt, float genmll, float genptll, float genyll) {
  myPromptDecay = prompt;
  myGenptll = genptll;
  myGenyll = genyll;
  myGenmll = genmll;
}

void RedHiggsTree::fillHLTElectrons(bool singleEle, bool singleEleRelaxed, bool singleEleOR) {

  myHLTSingleElectron = singleEle;
  myHLTSingleElectronRelaxed = singleEleRelaxed;
  myHLTSingleElectronOR = singleEleOR;
}

void RedHiggsTree::fillHLTMuons(bool singleMuon, bool singleMuonRelaxed, bool singleMuonOR) {

  myHLTSingleMuon = singleMuon;
  myHLTSingleMuonRelaxed = singleMuonRelaxed;
  myHLTSingleMuonOR = singleMuonOR;
}

void RedHiggsTree::fillRunInfos(int run, int lumi, long event, float puweight, bool HLT, float npu[3]) {

  myRun = run;
  myLS = lumi;
  myEvent = event;
  myPUWeight = puweight;
  myHLT = HLT;
  for(int i=0; i<3; i++) myNpu[i] = npu[i];
}

void RedHiggsTree::fillMetStudies(float projPF, float projTk, float signPFMet, float signChMet, float m_MTRcha, float DYMVA, float rho, float rhojet) {
  
  myProjPFMet        = projPF;  
  myProjPFChargedMet = projTk;  
  mySignPFMet        = signPFMet;
  mySignPFChargedMet = signChMet;
  myMTRchargedMet    = m_MTRcha;
  myDYMVA  = DYMVA;
  myRho    = rho; 
  myRhoJet = rhojet; 
}
