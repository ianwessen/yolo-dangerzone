#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/Utils.hh"
#include "HiggsAnalysisTools/include/kFactorEvaluator.hh"
#include "HiggsAnalysisTools/include/HiggsSelection.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"

#include <iostream>
#include <string>

#include <TTree.h>

HiggsSelection::HiggsSelection(TTree *tree) 
  : HiggsBase(tree) {

  // choose the Higgs Mass
  std::string higgsConfigDir;
  std::string higgsConfigDirMass;
  std::ifstream setfile("config/higgs/higgsMass.txt");
  higgsConfigDir="config/higgs/";
  if(!setfile.good()) {
    std::cout << "Cannot read the higgsMass file to choose the selection: config/higgs/higgsMass.txt" << std::endl
	      << "using default (160 GeV)" << std::endl;
    higgsConfigDirMass="config/higgs/h160/";
  }
  else {
    std::string var, massVal; 
    bool found=false;
    while (1) {
      setfile >> var >> massVal;
      _massVal = atoi(massVal.c_str());
      if(!setfile.good()) break;
      if(var.compare("HiggsMass")==0) { 
	found=true;
	higgsConfigDirMass="config/higgs/h" + massVal + "/";
	std::cout << "Reading configuration for Higgs mass = " << massVal << " GeV/c^2" << std::endl;
	break;
      }
    }
  }


  std::string fileCutsPreselection     = higgsConfigDir + "2l2nuCutsPreselection.txt";
  std::string fileSwitchesPreselection = higgsConfigDir + "2l2nuSwitchesPreselection.txt";

  // preselection efficiencies
  CommonHiggsPreselection.Configure(fileCutsPreselection.c_str(), fileSwitchesPreselection.c_str()); 
  _preselection = CommonHiggsPreselection.GetSelection();

  // extra preselection efficiencies  - to be put here not to pass the full list of leptons to the preselection class
  //  _addedPres = new Selection(fileCutsPreselection,fileSwitchesPreselection);
  _preselection->addSwitch("apply_looseIdBeforePreselection");   
  _preselection->addSwitch("asymmetricLeptons");
  _preselection->addSwitch("apply_kFactor");   
  _preselection->addCut("etaElectronAcc");
  _preselection->addCut("ptElectronAcc");
  _preselection->addCut("etaMuonAcc");
  _preselection->addCut("ptMuonAcc");

  // selection efficiencies
  std::string fileCutsEE     = higgsConfigDirMass + "2e2nuCuts.txt";
  std::string fileSwitchesEE = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionEE.Configure(fileCutsEE.c_str(),fileSwitchesEE.c_str(),"FULL SELECTION EVENT COUNTER EE"); 
  CutBasedHiggsSelectionEE.AppendPreselection(_preselection);
  _selectionEE = CutBasedHiggsSelectionEE.GetSelection();  

  std::string fileCutsMM     = higgsConfigDirMass + "2mu2nuCuts.txt";
  std::string fileSwitchesMM = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionMM.Configure(fileCutsMM.c_str(),fileSwitchesMM.c_str(),"FULL SELECTION EVENT COUNTER MM"); 
  CutBasedHiggsSelectionMM.AppendPreselection(_preselection);
  _selectionMM = CutBasedHiggsSelectionMM.GetSelection();

  std::string fileCutsEM     = higgsConfigDirMass + "emu2nuCuts.txt";
  std::string fileSwitchesEM = higgsConfigDir + "2l2nuSwitches.txt";
  CutBasedHiggsSelectionEM.Configure(fileCutsEM.c_str(),fileSwitchesEM.c_str(),"FULL SELECTION EVENT COUNTER EM"); 
  CutBasedHiggsSelectionEM.AppendPreselection(_preselection);
  _selectionEM = CutBasedHiggsSelectionEM.GetSelection();
  
  // single electron efficiency
  EgammaCutBasedID.Configure("config/higgs/");

  // kinematics
  m_p4ElectronPlus = new TLorentzVector(0.,0.,0.,0.);
  m_p4ElectronMinus = new TLorentzVector(0.,0.,0.,0.);
  m_p4MuonPlus = new TLorentzVector(0.,0.,0.,0.);
  m_p4MuonMinus = new TLorentzVector(0.,0.,0.,0.);
  m_p4MET = new TLorentzVector(0.,0.,0.,0.);

  _bestElectrons = new std::vector<int>;
  _bestMuons = new std::vector<int>;
  _bestJets = new std::vector<int>;
  _excludedJets = new std::vector<int>;
  _bestGenJets = new std::vector<int>;
}

HiggsSelection::~HiggsSelection(){

  delete m_p4ElectronPlus;
  delete m_p4ElectronMinus;
  delete m_p4MuonPlus;
  delete m_p4MuonMinus;
  delete m_p4MET;
  delete _bestElectrons;
  delete _bestMuons;
  delete _bestJets;
  delete _excludedJets;
  delete _bestGenJets;
  delete _preselection;
  delete _selectionEE;
  delete _selectionMM;
  delete _selectionEM;
  myOutTreeEE -> save();
  myOutTreeMM -> save();
  myOutTreeEM -> save();
  myTriggerTree -> save();

}

bool HiggsSelection::findMcTree(const char* processType) {

  _process = "UNDEFINED";
  _theGenEle = -1;
  _theGenPos = -1;
  
  // now we look for ee || mumu || emu
  // in the acceptance and with a loose pT threshold

  float etaEleAcc_ = 2.5;
  float ptEleAcc_ = 5.0; // GeV
  float etaMuonAcc_ = 2.4;
  float ptMuonAcc_ = 0.0; // GeV
  
  // signal: 2e2nu
  if(strcmp(processType,"HtoWWto2e2nu")==0) {
    int indeminus=999, indeplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc] == -11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc] ==  11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeminus<25 && indeplus<25 ) {
      _theGenPos = indeplus;
      _theGenEle = indeminus;
    }
    return ( indeplus < 25 && indeminus < 25 );
  }

  // signal: 2m2nu
  if(strcmp(processType,"HtoWWto2m2nu")==0) {
    int indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc] ==  13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
    }
    if( indmuminus<25 && indmuplus<25 ) {
      _theGenMuPlus = indmuplus;
      _theGenMuMinus = indmuminus;
    }
    return ( indmuplus < 25 && indmuminus < 25 );
  }

  // signal: em2nu
  if(strcmp(processType,"HtoWWtoem2nu")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 11 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos = indeplus;
      _theGenMuMinus = indmuminus;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
    }
    return ( (indeplus<25 && indmuminus<25) || (indeminus<25 && indmuplus<25) );
  }

  // signal ee excluding taus
  if(strcmp(processType,"HtoWWto2e2nu_prompt")==0) {
    int indeminus=999, indeplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 11 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeminus<25 && indeplus<25 ) {
      _theGenPos = indeplus;
      _theGenEle = indeminus;
    }
    return ( indeplus < 25 && indeminus < 25 );
  }

  // signal mm excluding taus
  if(strcmp(processType,"HtoWWto2m2nu_prompt")==0) {
    int indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -13 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 13 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = 11;
    }
    if( indmuminus<25 && indmuplus<25 ) {
      _theGenMuPlus = indmuplus;
      _theGenMuMinus = indmuminus;
    }
    return ( indmuplus < 25 && indmuminus < 25 );
  }

  // signal em excluding taus
  if(strcmp(processType,"HtoWWtoem2nu_prompt")==0) {
    int indeminus=999, indeplus=999, indmuminus=999, indmuplus=999;
    for(int imc=6;imc<25;imc++) {
      float ptMc = pMc[imc]*fabs(sin(thetaMc[imc]));
      if( idMc[imc]  == -11 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeplus = imc;
      if( idMc[imc]  == 13 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuminus = imc;
      if( idMc[imc]  == -13 && idMc[mothMc[imc]]==24 && fabs(etaMc[imc]) < etaMuonAcc_ && ptMc > ptMuonAcc_ ) indmuplus = imc;
      if( idMc[imc]  == 11 && idMc[mothMc[imc]]==-24 && fabs(etaMc[imc]) < etaEleAcc_ && ptMc > ptEleAcc_ ) indeminus = imc;
    }
    if( indeplus<25 && indmuminus<25 ) {
      _theGenPos = indeplus;
      _theGenMuMinus = indmuminus;
    } else if( indeminus<25 && indmuplus<25 ) {
      _theGenEle = indeminus;
      _theGenMuPlus = indmuplus;
    }
    return ( (indeplus<25 && indmuminus<25) || (indeminus<25 && indmuplus<25) );
  }

  // signal: 2l2nu
  if(strcmp(processType,"HtoWWto2l2nu")==0) {
    int indlminus=999, indlplus=999;
    for(int imc=6;imc<25;imc++) {
      if(idMc[imc]>10 && idMc[imc]<19 && idMc[mothMc[imc]]==-24)  indlminus=imc;
      if(idMc[imc]<-10 && idMc[imc]>-19 && idMc[mothMc[imc]]==24) indlplus=imc;
    }
    if(indlminus<25 && indlplus<25) {
      if( (idMc[indlminus]==-11) || (idMc[indlminus]==-13) || (idMc[indlminus]==-15))  
	_theGenEle = indlminus;
      if( (idMc[indlplus]==11) || (idMc[indlplus]==13) || (idMc[indlplus]==15) )
	_theGenPos = indlplus;
    }
    return (indlminus<25 && indlplus<25);
  }
  

  // WW: e / mu / tau
  else if(strcmp(processType,"WW")==0) {
    _process = "WW";
    TVector3 WminusP, WplusP;
    WminusP.SetMagThetaPhi(pMc[6],thetaMc[6],phiMc[6]);
    WplusP.SetMagThetaPhi(pMc[7],thetaMc[7],phiMc[7]);
    float pT = (WminusP+WplusP).Pt();
    _theGenEle = 6;
    _theGenPos = 7;
    _genHiggsPt[0] = pT;
    return (
	    (abs(idMc[6])==24) && (abs(idMc[7])==24) &&
	    (abs(idMc[8])>10 && abs(idMc[8])<19 && abs(idMc[mothMc[8]])==24) &&
	    (abs(idMc[10])>10 && abs(idMc[10])<19 && abs(idMc[mothMc[10]])==24)
	    );
  }
  // w+jets: e / mu / tau
  else if(strcmp(processType,"Wjets")==0) {
    _process = "Wjets";
    return ( ((abs(idMc[8])==11) && abs(idMc[9])==12) || ((abs(idMc[8])==13) && abs(idMc[9])==14) || ((abs(idMc[8])==15) && abs(idMc[9])==16));
  }
  // ttbar: e / mu / tau
  else if(strcmp(processType,"ttbar")==0) {
    _process = "ttbar";
    _theGenEle = 13;
    _theGenPos = 15;
    return ( 
	    abs(idMc[9])==24 && abs(idMc[15])>10 && abs(idMc[15])<19 &&
	    abs(idMc[11])==24 && abs(idMc[13])>10 && abs(idMc[13])<19 &&
	    (idMc[13]*idMc[15]<0)
	    );
  }
  else if(strcmp(processType,"ZZleptonic")==0) {
    _process = "ZZleptonic";
    // 8,9; 10,11 are the daughters of the Z;Z
    return (fabs(idMc[8])>10 && fabs(idMc[8])<19 &&
	    fabs(idMc[9])>10 && fabs(idMc[9])<19 &&
	    fabs(idMc[10])>10 && fabs(idMc[10])<19 &&
	    fabs(idMc[11])>10 && fabs(idMc[11])<19);
  }
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}

float HiggsSelection::getkFactor(std::string process) {

  float weight = 1.;
  if((process.compare("Higgs")==0)) {
    if(evtKfactor==1.) weight = 0.5; // this is to correct the kFactor of VBF. in the ntuple k=1 means VBF, which has ~costant k=0.5
    else weight = evtKfactor;
  }
  else if(process.compare("WW")==0) {
    weight = 1.0; // we used MC @ NLO weight in 16X   
  }
  return weight;
}

void HiggsSelection::Loop() {

  _verbose=false;
  if(fChain == 0) return;
  
  
  std::string::size_type loc = _datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    _datasetName.erase(loc);
  }
  
  // kinematics reduced tree
  std::string reducedTreeNameEE = _datasetName+"-datasetEE.root";
  std::string reducedTreeNameMM = _datasetName+"-datasetMM.root";
  std::string reducedTreeNameEM = _datasetName+"-datasetEM.root";
  myOutTreeEE = new RedHiggsTree(reducedTreeNameEE.c_str());
  myOutTreeMM = new RedHiggsTree(reducedTreeNameMM.c_str());
  myOutTreeEM = new RedHiggsTree(reducedTreeNameEM.c_str());

  myOutTreeEE->addHLTElectronsInfos();
  myOutTreeMM->addHLTMuonsInfos();
  myOutTreeEM->addHLTElectronsInfos(); myOutTreeEM->addHLTMuonsInfos();

  if ( _preselection->getSwitch("apply_kFactor") ) {

    myOutTreeEE->addKFactor();
    myOutTreeMM->addKFactor();
    myOutTreeEM->addKFactor();
  }

  myOutTreeEE->addMcTruthInfos();
  myOutTreeMM->addMcTruthInfos();
  myOutTreeEM->addMcTruthInfos();

  // trigger reduced tree
  std::string reducedTriggerTreeName = _datasetName+"-trigger.root";
  myTriggerTree = new RedTriggerTree(reducedTriggerTreeName.c_str());

  float met, deltaPhi, transvMass; 
  float dileptonInvMass, maxPtEle, minPtEle, detaLeptons;

  if ( _preselection->getSwitch("asymmetricLeptons") )
    cout << "---> Running with asymmetric leptons" << endl;
  
  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  std::cout << "Number of entries = " << nentries << std::endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;

    resetKinematics();

    // get the kFactor of the event (for signal)
    float weight = 1;
    if (_preselection->getSwitch("apply_kFactor")) weight = getkFactor("Higgs");
    
    // look to the MC truth decay tree
    bool foundMcTree = findMcTree("HtoWWto2e2nu");

    // trigger
    Utils anaUtils;
    bool passedHLT = anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);

    bool decayEE = findMcTree("HtoWWto2e2nu");
    bool decayMM = findMcTree("HtoWWto2m2nu");
    bool decayEM = findMcTree("HtoWWtoem2nu");

    bool promptEE = findMcTree("HtoWWto2e2nu_prompt");
    bool promptMM = findMcTree("HtoWWto2m2nu_prompt");
    bool promptEM = findMcTree("HtoWWtoem2nu_prompt");

    myTriggerTree->fillMcTruth(decayEE,decayMM,decayEM,promptEE,promptMM,promptEM);
    myTriggerTree->fillHLTElectrons( firedTrg[m_requiredTriggers[0]], 
				     firedTrg[m_requiredTriggers[1]],
				     (firedTrg[m_requiredTriggers[0]] || firedTrg[m_requiredTriggers[1]]) );
    myTriggerTree->fillHLTMuons( firedTrg[m_requiredTriggers[2]], 
				 firedTrg[m_requiredTriggers[3]],
				 (firedTrg[m_requiredTriggers[2]] || firedTrg[m_requiredTriggers[3]]) );
    myTriggerTree->store();

    // get the best electrons, best muons  
    std::pair<int,int> theElectrons = getBestElectronPair();
    std::pair<int,int> theMuons = getBestMuonPair();
    int tbElectron(theElectrons.second), tbPositron(theElectrons.first);    
    int tbMuonPlus(theMuons.first),      tbMuonMinus(theMuons.second);
    theElectron  = tbElectron;
    thePositron  = tbPositron;
    theMuonPlus  = tbMuonPlus;
    theMuonMinus = tbMuonMinus;

    // how many electrons after preselection eleID and isolation   
    // (in case we want to reproduce CSA07 conditions)
    int nPreselEle = nEle;
    if(_preselection->getSwitch("apply_looseIdBeforePreselection")){       
      nPreselEle = 0;
      for(int i=0;i<nEle;i++) {
	if (!eleIdCutBasedEle[i]) continue;
	if (eleSumPtPreselectionEle[i]>0.05) continue;  
	nPreselEle++;
      }
    }
    
    // reconstructed channel
    m_channel[ee] = false;     
    m_channel[mm] = false;
    m_channel[em] = false;
    if (theElectron > -1 && thePositron > -1)  m_channel[ee] = true;
    if (theMuonPlus > -1 && theMuonMinus > -1) m_channel[mm] = true;
    if ((theElectron > -1 && theMuonPlus > -1) || (thePositron > -1 && theMuonMinus > -1)) m_channel[em] = true;
    if (_verbose) {
      std::cout << "nEle = " << nEle << "\tnMuon = " << nMuon << std::endl;
      std::cout << "indices: " << theElectron << " " << thePositron << " " << theMuonMinus << " " << theMuonPlus << std::endl;
      std::cout << "chargeEle = " << chargeEle[theElectron] << "\tchargePos = " << chargeEle[thePositron] 
		<< "\tchargeMuonMinus = " << chargeMuon[theMuonMinus] << "\tchargeMuonPlus = " << chargeMuon[theMuonPlus] << std::endl;
      std::cout << "ee = " << m_channel[ee] << "\tmm = " << m_channel[mm] << "\temu = " << m_channel[em] << std::endl; 
    }

    // kinematics at preselection level 
    // only few variables since here we still do not know if two good leptons are reconstructed
    setPreselKinematics();
    
    // setting preselections
    CommonHiggsPreselection.SetWeight(weight);
    CommonHiggsPreselection.SetMcTruth(foundMcTree);
    CommonHiggsPreselection.SetHLT(passedHLT);
    CommonHiggsPreselection.SetNele(nPreselEle);   
    CommonHiggsPreselection.SetNmuon(nMuon);
    CommonHiggsPreselection.SetIsEE(m_channel[ee]);
    CommonHiggsPreselection.SetIsEM(m_channel[em]);
    CommonHiggsPreselection.SetIsMM(m_channel[mm]);
    CommonHiggsPreselection.SetHighElePt(hardestElectronPt);
    CommonHiggsPreselection.SetLowElePt(slowestElectronPt);
    CommonHiggsPreselection.SetHighMuonPt(hardestMuonPt);
    CommonHiggsPreselection.SetLowMuonPt(slowestMuonPt);
    CommonHiggsPreselection.SetMet(etMet[0]);
    CommonHiggsPreselection.SetMllEE(m_mll[ee]);
    CommonHiggsPreselection.SetMllEM(m_mll[em]);
    CommonHiggsPreselection.SetMllMM(m_mll[mm]);
    CommonHiggsPreselection.SetCommonPres(evtPresel);

    // did we pass preselections?
    bool isPreselections = CommonHiggsPreselection.output();    
    //    if( !isPreselections ) continue;

    // kinematics after preselections
    // now we know we have two good reconstructed leptons
    setKinematics();

    // ----------------------- selection ----------------------------
    bool asymmetricLeptons = _preselection->getSwitch("asymmetricLeptons");
    // electron ID (true by default - studied only if ee or emu channel)
    bool theElectronID = true;
    bool thePositronID = true;
    // custom electron ID (tight symmetric for ee, tight for emu)
    if (theElectron > -1) theElectronID = isEleID(theElectron);
    if (thePositron > -1) thePositronID = isEleID(thePositron);

    // custom electron ID (tight + loose)
    if(asymmetricLeptons) {
      if ( theElectron > -1 && thePositron > -1 ) {
        int theHardest=-1;
        int theSlowest=-1;
        if(etEle[theElectron] > etEle[thePositron]) {
          theHardest = theElectron;
          theSlowest = thePositron;
        } else {
          theHardest = thePositron;
          theSlowest = theElectron;
        }
        // don't look at names
        if(fabs(etaEle[theSlowest])<1.475) {
          // tight x loose
          theElectronID = eleIdCutBasedEle[theHardest];
          thePositronID = isEleID(theSlowest);
        } // else tight x tight: already done
      }
    }

    // loose egamma electron ID (loose symmetric)
    // if (theElectron > -1) theElectronID = eleIdCutBasedEle[theElectron];
    // if (thePositron > -1) thePositronID = eleIdCutBasedEle[thePositron];

    // likelihood electron ID: asymmetric on two electrons
    // if ( theElectron > -1 && thePositron > -1 ) {
    //   if(etEle[theElectron] > etEle[thePositron]) {
    // 	theElectronID = eleIdLikelihoodEle[theElectron] > 0.75;
    // 	thePositronID = eleIdLikelihoodEle[thePositron] > 0.40;
    //   }
    //  else {
    // 	theElectronID = eleIdLikelihoodEle[theElectron] > 0.40;
    // 	thePositronID = eleIdLikelihoodEle[thePositron] > 0.75;
    //  }
    // }

    // extra tracker isolation for electrons
    float theEleTrackerPtSum = 0.;
    float thePosTrackerPtSum = 0.;
    if (theElectron > -1 && thePositron > -1) { 
      float ptEle = sqrt(pxEle[theElectron]*pxEle[theElectron] + pyEle[theElectron]*pyEle[theElectron]);
      theEleTrackerPtSum = (eleSumPt04Ele[theElectron]*ptEle - getSecondEleTkPt(theElectron,thePositron,0.4))/ptEle;
      float ptPos = sqrt(pxEle[thePositron]*pxEle[thePositron] + pyEle[thePositron]*pyEle[thePositron]);
      thePosTrackerPtSum = (eleSumPtPreselectionEle[thePositron]*ptPos - getSecondEleTkPt(thePositron,theElectron,0.4))/ptPos;      
    }
    if (theElectron > -1 && thePositron < 0)  theEleTrackerPtSum = eleSumPtPreselectionEle[theElectron];
    if (thePositron > -1 && theElectron < 0)  thePosTrackerPtSum = eleSumPtPreselectionEle[thePositron];

    // hcal isolation for electrons
    float theEleHcalPtSum = 0.;
    float thePosHcalPtSum = 0.;
    if (theElectron > -1) theEleHcalPtSum = eleSumHadEt04Ele[theElectron];
    if (thePositron > -1) thePosHcalPtSum = eleSumHadEt04Ele[thePositron];

    // ecal isolation for electrons
    float theEleEcalPtSum = 0.;
    float thePosEcalPtSum = 0.;
    if (theElectron > -1 && thePositron > -1) { 
      float ptEle = sqrt(pxEle[theElectron]*pxEle[theElectron] + pyEle[theElectron]*pyEle[theElectron]);
      theEleEcalPtSum = (eleSumEmEt04Ele[theElectron]*ptEle - getSecondEleEmEt(theElectron,thePositron,0.4))/ptEle;      
      float ptPos = sqrt(pxEle[thePositron]*pxEle[thePositron] + pyEle[thePositron]*pyEle[thePositron]);
      thePosEcalPtSum = (eleSumEmEt04Ele[thePositron]*ptPos - getSecondEleEmEt(thePositron,theElectron,0.4))/ptPos;
    }
    if (theElectron > -1 && thePositron < 0 ) theEleEcalPtSum = eleSumEmEt04Ele[theElectron];
    if (thePositron > -1 && theElectron < 0 ) thePosEcalPtSum = eleSumEmEt04Ele[thePositron];

    // jet veto: method gives true if good jet is found
    bool passedJetVeto = !goodJetFound();

    // kine variables
    float theDeltaPhiEE, theInvMassEE, theTransvMassEE, theDetaLeptonsEE = 0.;
    float theDeltaPhiMM, theInvMassMM, theTransvMassMM, theDetaLeptonsMM = 0.;
    float theDeltaPhiEM, theInvMassEM, theTransvMassEM, theDetaLeptonsEM = 0.;


    // ---------------------------------------
    // ee final state
    if (m_channel[ee]){

      float theEleHardTrackerPtSum = (etEle[theElectron] > etEle[thePositron]) ? theEleTrackerPtSum : thePosTrackerPtSum;
      float theEleSlowTrackerPtSum = (etEle[theElectron] > etEle[thePositron]) ? thePosTrackerPtSum : theEleTrackerPtSum;
      float theEleHardHcalPtSum = (etEle[theElectron] > etEle[thePositron]) ? theEleHcalPtSum : thePosHcalPtSum;
      float theEleSlowHcalPtSum = (etEle[theElectron] > etEle[thePositron]) ? thePosHcalPtSum : theEleHcalPtSum;
      float theEleHardEcalPtSum = (etEle[theElectron] > etEle[thePositron]) ? theEleEcalPtSum : thePosEcalPtSum;
      float theEleSlowEcalPtSum = (etEle[theElectron] > etEle[thePositron]) ? thePosEcalPtSum : theEleEcalPtSum;

      theDeltaPhiEE    = m_deltaPhi[ee];
      theInvMassEE     = m_mll[ee];
      theDetaLeptonsEE = etaEle[theElectron]-etaEle[thePositron];
      theTransvMassEE  = m_transvMass[ee];

      // selections
      CutBasedHiggsSelectionEE.SetWeight(weight);
      CutBasedHiggsSelectionEE.SetHighElePt(hardestElectronPt);
      CutBasedHiggsSelectionEE.SetLowElePt(slowestElectronPt);
      CutBasedHiggsSelectionEE.SetAsymmetricLeptons(asymmetricLeptons);
      CutBasedHiggsSelectionEE.SetElectronId(theElectronID);
      CutBasedHiggsSelectionEE.SetPositronId(thePositronID);
      CutBasedHiggsSelectionEE.SetEleHardTrackerPtSum(theEleHardTrackerPtSum);
      CutBasedHiggsSelectionEE.SetEleSlowTrackerPtSum(theEleSlowTrackerPtSum);
      CutBasedHiggsSelectionEE.SetEleHardHcalPtSum(theEleHardHcalPtSum);
      CutBasedHiggsSelectionEE.SetEleSlowHcalPtSum(theEleSlowHcalPtSum);
      CutBasedHiggsSelectionEE.SetEleHardEcalPtSum(theEleHardEcalPtSum);
      CutBasedHiggsSelectionEE.SetEleSlowEcalPtSum(theEleSlowEcalPtSum);
      CutBasedHiggsSelectionEE.SetJetVeto(passedJetVeto);
      CutBasedHiggsSelectionEE.SetMet(etMet[0]);					
      CutBasedHiggsSelectionEE.SetDeltaPhi(theDeltaPhiEE);
      CutBasedHiggsSelectionEE.SetInvMass(theInvMassEE);
      CutBasedHiggsSelectionEE.SetDetaLeptons(theDetaLeptonsEE);
      bool isSelectedEE = CutBasedHiggsSelectionEE.output();    
      bool selUpToFinalLeptonsEE = CutBasedHiggsSelectionEE.outputUpToFinalLeptons();
      bool selUpToJetVetoEE = CutBasedHiggsSelectionEE.outputUpToJetVeto();
      bool selPreDeltaPhiEE = CutBasedHiggsSelectionEE.outputPreDeltaPhi();

      myOutTreeEE -> fillMcTruth(promptEE);
      
      myOutTreeEE -> fillHLTElectrons( firedTrg[m_requiredTriggers[0]], 
				       firedTrg[m_requiredTriggers[1]],
				       (firedTrg[m_requiredTriggers[0]] || firedTrg[m_requiredTriggers[1]]) );

      myOutTreeEE -> fillAll(etMet[0], 
			     theDeltaPhiEE, 
			     theTransvMassEE, 
			     theInvMassEE, 
			     hardestElectronPt, 
			     slowestElectronPt, 
			     theDetaLeptonsEE,
			     selUpToFinalLeptonsEE,
			     selUpToJetVetoEE,
			     selPreDeltaPhiEE,
			     isSelectedEE);

      if ( _preselection->getSwitch("apply_kFactor") ) {
	myOutTreeEE->fillKFactor(evtKfactor);
      }

      // dumping final tree
      myOutTreeEE -> store();
      
    }

    // ---------------------------------------
    // mm final state
    if (m_channel[mm]){
      theDeltaPhiMM    = m_deltaPhi[mm];
      theInvMassMM     = m_mll[mm];
      theDetaLeptonsMM = etaEle[theMuonMinus]-etaEle[theMuonPlus];
      theTransvMassMM  = m_transvMass[mm];
      
      // selections
      CutBasedHiggsSelectionMM.SetWeight(weight);
      CutBasedHiggsSelectionMM.SetHighElePt(hardestMuonPt);
      CutBasedHiggsSelectionMM.SetLowElePt(slowestMuonPt);
      CutBasedHiggsSelectionEE.SetAsymmetricLeptons(false);
      CutBasedHiggsSelectionMM.SetElectronId(true);
      CutBasedHiggsSelectionMM.SetPositronId(true);
      CutBasedHiggsSelectionMM.SetEleHardTrackerPtSum(0);
      CutBasedHiggsSelectionMM.SetEleSlowTrackerPtSum(0);
      CutBasedHiggsSelectionMM.SetEleHardHcalPtSum(0);
      CutBasedHiggsSelectionMM.SetEleSlowHcalPtSum(0);
      CutBasedHiggsSelectionMM.SetEleHardEcalPtSum(0);
      CutBasedHiggsSelectionMM.SetEleSlowEcalPtSum(0);
      CutBasedHiggsSelectionMM.SetJetVeto(passedJetVeto);
      CutBasedHiggsSelectionMM.SetMet(etMet[0]);					
      CutBasedHiggsSelectionMM.SetDeltaPhi(theDeltaPhiMM);
      CutBasedHiggsSelectionMM.SetInvMass(theInvMassMM);
      CutBasedHiggsSelectionMM.SetDetaLeptons(theDetaLeptonsMM);
      bool isSelectedMM = CutBasedHiggsSelectionMM.output();    
      bool selUpToFinalLeptonsMM = CutBasedHiggsSelectionMM.outputUpToFinalLeptons();
      bool selUpToJetVetoMM = CutBasedHiggsSelectionMM.outputUpToJetVeto();
      bool selPreDeltaPhiMM = CutBasedHiggsSelectionMM.outputPreDeltaPhi();

      myOutTreeMM -> fillMcTruth(promptMM);
      
      myOutTreeMM -> fillHLTMuons( firedTrg[m_requiredTriggers[2]], 
				   firedTrg[m_requiredTriggers[3]],
				   (firedTrg[m_requiredTriggers[2]] || firedTrg[m_requiredTriggers[3]]) );
      
      myOutTreeMM -> fillAll(etMet[0], 
			     theDeltaPhiMM, 
			     theTransvMassMM, 
			     theInvMassMM, 
			     hardestMuonPt, 
			     slowestMuonPt, 
			     theDetaLeptonsMM,
			     selUpToFinalLeptonsMM,
			     selUpToJetVetoMM,
			     selPreDeltaPhiMM,
			     isSelectedMM);
      
      if ( _preselection->getSwitch("apply_kFactor") ) {
	myOutTreeMM->fillKFactor(evtKfactor);
      }

      // dumping final tree
      myOutTreeMM -> store();
      
    }
    
    // ancora da finire
    if (m_channel[em]){
      
      // electron ID, isolations for the only electron / positron
      bool theEleIDEM = false;
      float theEleTrackerPtSumEM = 0.0;
      float theEleHcalPtSumEM = 0.0;
      float theEleEcalPtSumEM = 0.0;

      theDeltaPhiEM    = m_deltaPhi[em];
      theInvMassEM     = m_mll[em];
      theTransvMassEM  = m_transvMass[em];
      if(theElectron>-1 && theMuonPlus>-1) {
	theDetaLeptonsEM = etaEle[theElectron]-etaEle[theMuonPlus];
	theEleIDEM = theElectronID;
	theEleTrackerPtSumEM = theEleTrackerPtSum;
	theEleHcalPtSumEM = theEleHcalPtSum;
	theEleEcalPtSumEM = theEleEcalPtSum;
      }
      if(thePositron>-1 && theMuonMinus>-1 ) {
	theDetaLeptonsEM = etaEle[thePositron]-etaEle[theMuonMinus];
	theEleIDEM = thePositronID;
	theEleTrackerPtSumEM = thePosTrackerPtSum;
	theEleHcalPtSumEM = thePosHcalPtSum;
	theEleEcalPtSumEM = thePosEcalPtSum;
      }


      // selections
      CutBasedHiggsSelectionEM.SetWeight(weight);
      CutBasedHiggsSelectionEM.SetHighElePt(hardestElectronPt);
      CutBasedHiggsSelectionEM.SetLowElePt(slowestMuonPt);
      CutBasedHiggsSelectionEE.SetAsymmetricLeptons(false);
      CutBasedHiggsSelectionEM.SetElectronId(true);
      CutBasedHiggsSelectionEM.SetPositronId(theEleIDEM);
      CutBasedHiggsSelectionEM.SetEleHardTrackerPtSum(0);
      CutBasedHiggsSelectionEM.SetEleSlowTrackerPtSum(theEleTrackerPtSumEM);
      CutBasedHiggsSelectionEM.SetEleHardHcalPtSum(0);
      CutBasedHiggsSelectionEM.SetEleSlowHcalPtSum(theEleHcalPtSumEM);
      CutBasedHiggsSelectionEM.SetEleHardEcalPtSum(0);
      CutBasedHiggsSelectionEM.SetEleSlowEcalPtSum(theEleEcalPtSumEM);
      CutBasedHiggsSelectionEM.SetJetVeto(passedJetVeto);
      CutBasedHiggsSelectionEM.SetMet(etMet[0]);					
      CutBasedHiggsSelectionEM.SetDeltaPhi(theDeltaPhiMM);
      CutBasedHiggsSelectionEM.SetInvMass(theInvMassMM);
      CutBasedHiggsSelectionEM.SetDetaLeptons(theDetaLeptonsMM);
      bool isSelectedEM = CutBasedHiggsSelectionEM.output();    
      bool selUpToFinalLeptonsEM = CutBasedHiggsSelectionEM.outputUpToFinalLeptons();
      bool selUpToJetVetoEM = CutBasedHiggsSelectionEM.outputUpToJetVeto();
      bool selPreDeltaPhiEM = CutBasedHiggsSelectionEM.outputPreDeltaPhi();

      myOutTreeEM -> fillMcTruth(promptEM);
      
      myOutTreeEM -> fillHLTElectrons( firedTrg[m_requiredTriggers[0]], 
				       firedTrg[m_requiredTriggers[1]],
				       (firedTrg[m_requiredTriggers[0]] || firedTrg[m_requiredTriggers[1]]) );

      myOutTreeEM -> fillHLTMuons( firedTrg[m_requiredTriggers[2]], 
				   firedTrg[m_requiredTriggers[3]],
				   (firedTrg[m_requiredTriggers[2]] || firedTrg[m_requiredTriggers[3]]) );

      myOutTreeEM -> fillAll(etMet[0], 
			     theDeltaPhiEM, 
			     theTransvMassEM, 
			     theInvMassEM, 
			     hardestMuonPt, 
			     slowestMuonPt, 
			     theDetaLeptonsEM,
			     selUpToFinalLeptonsEM,
			     selUpToJetVetoEM,
			     selPreDeltaPhiEM,
			     isSelectedEM);
      
      if ( _preselection->getSwitch("apply_kFactor") ) {
	myOutTreeEM->fillKFactor(evtKfactor);
      }

      // dumping final tree
      myOutTreeEM -> store();

    }

  }

}

void HiggsSelection::displayEfficiencies(std::string datasetName) {

  std::string::size_type loc = datasetName.find_first_of(".",0);
  if( loc != std::string::npos ) {
    datasetName.erase(loc);
  }

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Common preselections: " << std::endl;
  CommonHiggsPreselection.diplayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full EE selections: " << std::endl;
  CutBasedHiggsSelectionEE.diplayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full MM selections: " << std::endl;
  CutBasedHiggsSelectionMM.diplayEfficiencies(datasetName);

  std::cout << "--------------------------------" << std::endl;
  std::cout << "Full EM selections: " << std::endl;
  CutBasedHiggsSelectionEM.diplayEfficiencies(datasetName);

  EgammaCutBasedID.diplayEfficiencies();


}

std::pair<int,int> HiggsSelection::getBestElectronPair() {

  int theLep1=-1;
  int theLep2=-1;
  float maxPtLep1=-1000.;
  float maxPtLep2=-1000.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nEle;i++) {
    
  // if ambiguity resolution is not applied... @$#%@^@!
//   vector<int> _resolvedElectrons = resolvedElectrons();
//   vector<int>::const_iterator it; 

//   for(it=_resolvedElectrons.begin(); it!=_resolvedElectrons.end(); it++) {
//     int i = *it;

    // to be in the same situation as for CSA07 analysis   
    if(_preselection->getSwitch("apply_looseIdBeforePreselection")){       
      if (!eleIdCutBasedEle[i]) continue;
      if (eleSumPtPreselectionEle[i]>0.05) continue;  
    }

    if(_preselection->getSwitch("etaElectronAcc") && !_preselection->passCut("etaElectronAcc",etaEle[i]) ) continue;
    TVector3 pLepton(pxEle[i],pyEle[i],pzEle[i]);
    float thisPt=pLepton.Pt();
    if(_preselection->getSwitch("ptElectronAcc") && !_preselection->passCut("ptElectronAcc",thisPt) ) continue;
    // fixme: (in the future) put here full electron ID not to loose efficiency I think
    float thisCharge = chargeEle[i];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
  }
  _bestElectrons->clear();
  _bestElectrons->push_back(theLep1);  _bestElectrons->push_back(theLep2); 
  return make_pair(theLep1,theLep2);
}

std::pair<int,int> HiggsSelection::getBestMuonPair() {
  int theLep1=-1;
  int theLep2=-1;
  float maxPtLep1=-1000.;
  float maxPtLep2=-1000.;
  std::vector<int> goodRecoLeptons;
  for(int i=0;i<nMuon;i++) {
    if(_preselection->getSwitch("etaMuonAcc") && !_preselection->passCut("etaMuonAcc",etaMuon[i]) ) continue;
    TVector3 pLepton(pxMuon[i],pyMuon[i],pzMuon[i]);
    float thisPt=pLepton.Pt();
    if(_preselection->getSwitch("ptMuonAcc") && !_preselection->passCut("ptMuonAcc",thisPt) ) continue;
    // fixme: (in the future) put here full electron ID not to loose efficiency I think
    float thisCharge = chargeMuon[i];
    if (thisCharge > 0 && thisPt> maxPtLep1){ maxPtLep1 = thisPt; theLep1 = i; }
    if (thisCharge < 0 && thisPt> maxPtLep2){ maxPtLep2 = thisPt; theLep2 = i; }
  }
  _bestMuons->clear();
  _bestMuons->push_back(theLep1);  _bestMuons->push_back(theLep2); 
  return make_pair(theLep1,theLep2);
}

bool HiggsSelection::isEleID(int eleIndex) {

  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  EgammaCutBasedID.SetHOverE( hOverEEle[eleIndex] );
  EgammaCutBasedID.SetS9S25( s9s25Ele[eleIndex] );
  EgammaCutBasedID.SetDEta( deltaEtaAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiIn( deltaPhiAtVtxEle[eleIndex] );
  EgammaCutBasedID.SetDPhiOut( deltaPhiAtCaloEle[eleIndex] );
  EgammaCutBasedID.SetInvEminusInvP( 1./ecalEle[eleIndex]-1./momentumEle[eleIndex] );
  EgammaCutBasedID.SetBremFraction( fabs(momentumEle[eleIndex]-pTrkAtOuter.Mag())/momentumEle[eleIndex] );
  EgammaCutBasedID.SetSigmaEtaEta( sqrt(covEtaEtaEle[eleIndex]) );
  EgammaCutBasedID.SetSigmaPhiPhi( sqrt(covPhiPhiEle[eleIndex]) );
  EgammaCutBasedID.SetEOverPout( eSeedOverPoutEle[eleIndex] );
  EgammaCutBasedID.SetEOverPin( eSuperClusterOverPEle[eleIndex] );
  EgammaCutBasedID.SetElectronClass ( classificationEle[eleIndex] );
  EgammaCutBasedID.SetEgammaCutBasedID ( eleIdCutBasedEle[eleIndex] );
  EgammaCutBasedID.SetLikelihood( eleIdLikelihoodEle[eleIndex] );

  bool isIdentified = EgammaCutBasedID.output();

  return isIdentified;
}



void HiggsSelection::setPreselKinematics() {

  // highest and lowest pt for electrons
  // + mll if 2 good electrons are reconstructed
  if (thePositron > -1 && theElectron > -1) {
    hardestElectronPt = TMath::Max(etEle[theElectron],etEle[thePositron]);
    slowestElectronPt = TMath::Min(etEle[theElectron],etEle[thePositron]);
    m_p4ElectronMinus -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
    m_p4ElectronPlus  -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);      
    m_mll[ee] = (*m_p4ElectronMinus + *m_p4ElectronPlus).M();
  }
  if (thePositron <= -1 && theElectron > -1) {
    hardestElectronPt = etEle[theElectron];
    slowestElectronPt = etEle[theElectron];
    m_p4ElectronMinus -> SetXYZT(pxEle[theElectron],pyEle[theElectron],pzEle[theElectron],energyEle[theElectron]);
    m_mll[ee] = -999.;
  }
  if (thePositron > -1 && theElectron <= -1) {
    hardestElectronPt = etEle[thePositron];
    slowestElectronPt = etEle[thePositron];
    m_p4ElectronPlus  -> SetXYZT(pxEle[thePositron],pyEle[thePositron],pzEle[thePositron],energyEle[thePositron]);      
    m_mll[ee] = -999.;
  }
  if (theElectron <= -1 && thePositron <= -1) {
    hardestElectronPt = -999.;
    slowestElectronPt = -999.;
    m_mll[ee] = -999.;
  }

  // highest and lowest pt for muons
  // + mll if 2 good muons are reconstructed  
  if (theMuonPlus > -1 && theMuonMinus > -1){ 
    hardestMuonPt = TMath::Max(etMuon[theMuonPlus],etMuon[theMuonMinus]);
    slowestMuonPt = TMath::Min(etMuon[theMuonPlus],etMuon[theMuonMinus]);
    m_p4MuonMinus -> SetXYZT(pxMuon[theMuonMinus],pyMuon[theMuonMinus],pzMuon[theMuonMinus],energyMuon[theMuonMinus]);
    m_p4MuonPlus  -> SetXYZT(pxMuon[theMuonPlus],pyMuon[theMuonPlus],pzMuon[theMuonPlus],energyMuon[theMuonPlus]);      
    m_mll[mm] = (*m_p4MuonMinus + *m_p4MuonPlus).M();
  }
  if (theMuonMinus > -1 && theMuonPlus <= -1) {
    hardestMuonPt = etMuon[theMuonMinus];
    slowestMuonPt = etMuon[theMuonMinus];
    m_p4MuonMinus -> SetXYZT(pxMuon[theMuonMinus],pyMuon[theMuonMinus],pzMuon[theMuonMinus],energyMuon[theMuonMinus]);
    m_mll[mm]     = -999.;
  }
  if (theMuonPlus > -1 && theMuonMinus <= -1) {
    hardestMuonPt = etMuon[theMuonPlus];
    slowestMuonPt = etMuon[theMuonPlus];
    m_p4MuonPlus  -> SetXYZT(pxMuon[theMuonPlus],pyMuon[theMuonPlus],pzMuon[theMuonPlus],energyMuon[theMuonPlus]);      
    m_mll[mm]     = -999.;
  }
  if (theMuonMinus <= -1 && theMuonPlus <= -1) {
    hardestMuonPt = -999.;
    slowestMuonPt = -999.;
    m_mll[mm]     = -999.;
  }
  
  // mixed channel mll --> always passed
  m_mll[em]       = 50.;
  
  // MET
  m_p4MET->SetXYZT(pxMet[0],pyMet[0],pzMet[0],energyMet[0]); 
}



void HiggsSelection::setKinematics( ) {

  // electron variables used for ele quality in jet veto 
  m_HoEElectronMinus     = hOverEEle[theElectron];
  m_HoEElectronPlus      = hOverEEle[thePositron];
  m_CaloEneElectronMinus = ecalEle[theElectron];
  m_CaloEneElectronPlus  = ecalEle[thePositron];

  // compute delta Phi in degrees, di-lepton invariant mass, transverse mass
  TVector3 dilepPt;
  if ( m_channel[ee] ) {
    m_deltaPhi[ee] = fabs(180./TMath::Pi() * m_p4ElectronMinus->Vect().DeltaPhi(m_p4ElectronPlus->Vect()));
    dilepPt.SetXYZ( m_p4ElectronMinus->Vect().X()+m_p4ElectronPlus->Vect().X(),
		    m_p4ElectronMinus->Vect().Y()+m_p4ElectronPlus->Vect().Y(),
		    0.0 );
    m_transvMass[ee]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect())) ) );
  }
  else {    
    m_deltaPhi[ee]   = -1.;
    m_transvMass[ee] = -1.;
  }

  if ( m_channel[mm] ) {    
    m_deltaPhi[mm] = fabs(180./TMath::Pi() * m_p4MuonMinus->Vect().DeltaPhi(m_p4MuonPlus->Vect()));
    dilepPt.SetXYZ( m_p4MuonMinus->Vect().X()+m_p4MuonPlus->Vect().X(),
		    m_p4MuonMinus->Vect().Y()+m_p4MuonPlus->Vect().Y(),
		    0.0 );
    m_transvMass[mm]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect()))));
  }
  else { 
    m_deltaPhi[mm]   = -1.;
    m_transvMass[mm] = -1.;
  }
  
  if ( m_channel[em] ) {
    float deltaPhiEPlusMuMinus = -1.0;
    float deltaPhiEMinusMuPlus = -1.0;
    float dilepPtEPlusMuMinus  = -1.0;
    float dilepPtEMinusMuPlus  = -1.0;

    if ( thePositron > -1 && theMuonMinus > -1 ) {
      deltaPhiEPlusMuMinus = fabs(180./TMath::Pi() * m_p4ElectronPlus->Vect().DeltaPhi(m_p4MuonMinus->Vect()));
      m_deltaPhi[em] = deltaPhiEPlusMuMinus;
      dilepPt.SetXYZ( m_p4ElectronPlus->Vect().X()+m_p4MuonMinus->Vect().X(),
		      m_p4ElectronPlus->Vect().Y()+m_p4MuonMinus->Vect().Y(),
		      0.0 );
      dilepPtEPlusMuMinus = dilepPt.Mag();
      m_transvMass[em]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect()))));
      hardestLeptonPt = TMath::Max(etEle[thePositron],etMuon[theMuonMinus]);
      slowestLeptonPt = TMath::Min(etEle[thePositron],etMuon[theMuonMinus]);
    }
    if ( theElectron > -1 && theMuonPlus > -1 ) {
      deltaPhiEMinusMuPlus = fabs( 180./TMath::Pi() * m_p4ElectronMinus->Vect().DeltaPhi(m_p4MuonPlus->Vect()));
      m_deltaPhi[em] = deltaPhiEMinusMuPlus;
      dilepPt.SetXYZ( m_p4ElectronMinus->Vect().X()+m_p4MuonPlus->Vect().X(),
		      m_p4ElectronMinus->Vect().Y()+m_p4MuonPlus->Vect().Y(),
		      0.0 );
      dilepPtEMinusMuPlus = dilepPt.Mag();
      m_transvMass[em]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect())) ) );
      hardestLeptonPt = TMath::Max(etEle[theElectron],etMuon[theMuonPlus]);
      slowestLeptonPt = TMath::Min(etEle[theElectron],etMuon[theMuonPlus]);
    }
    if ( thePositron > -1 && theMuonMinus > -1 &&
	 theElectron > -1 && theMuonPlus > -1) {
      
      // if two pairs are built we choose the one with highest di-lepton pt
      if ( dilepPtEPlusMuMinus > dilepPtEMinusMuPlus ) {
	m_deltaPhi[em] = deltaPhiEPlusMuMinus;
	dilepPt.SetXYZ( m_p4ElectronPlus->Vect().X()+m_p4MuonMinus->Vect().X(),
			m_p4ElectronPlus->Vect().Y()+m_p4MuonMinus->Vect().Y(),
			0.0 );
	m_transvMass[em]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect())) ) );
	hardestLeptonPt = TMath::Max(etEle[thePositron],etMuon[theMuonMinus]);
	slowestLeptonPt = TMath::Min(etEle[thePositron],etMuon[theMuonMinus]);
      }
      else {
	m_deltaPhi[em] = deltaPhiEMinusMuPlus;
	dilepPt.SetXYZ( m_p4ElectronMinus->Vect().X()+m_p4MuonPlus->Vect().X(),
			m_p4ElectronMinus->Vect().Y()+m_p4MuonPlus->Vect().Y(),
			0.0 );
	m_transvMass[em]=sqrt(2*dilepPt.Mag() * m_p4MET->Vect().Mag() * (1-cos(dilepPt.Angle(m_p4MET->Vect())) ) );
	hardestLeptonPt = TMath::Max(etEle[theElectron],etMuon[theMuonPlus]);
	slowestLeptonPt = TMath::Min(etEle[theElectron],etMuon[theMuonPlus]);
      }
    }
  }
  else {
    m_deltaPhi[em] = -1.;
    m_transvMass[em] = -1.;
  }
  
  // --- Higgs and electron generator level ---
  if(_theGenEle>0 && _theGenPos>0) {
    if(pMc[_theGenEle]>=pMc[_theGenPos]) {
      _highestPtGen[0]=pMc[_theGenEle]*fabs(sin(thetaMc[_theGenEle]));
      _lowestPtGen[0]=pMc[_theGenPos]*fabs(sin(thetaMc[_theGenPos]));
    }
    else {
      _highestPtGen[0]=pMc[_theGenPos]*fabs(sin(thetaMc[_theGenPos]));
      _lowestPtGen[0]=pMc[_theGenEle]*fabs(sin(thetaMc[_theGenEle]));
    }
  }
}


bool HiggsSelection::goodJetFound() {

  // first check that kinematics has been set
  bool foundJet=false;
  float maxPtJet=0.;
  for(int j=0;j<nSisConeJet;j++) {

    // check if the electron or the positron falls into the jet
    // common cleaning class
    TVector3 p3Jet(pxSisConeJet[j],pySisConeJet[j],pzSisConeJet[j]);
    if ( m_p4ElectronMinus->Vect().Mag() != 0 ) {
      float deltaR =  p3Jet.DeltaR( m_p4ElectronMinus->Vect() );
      // taking from ee config file, but jets veto is the same for all the channels
      if(_selectionEE->getSwitch("jetConeWidth") && 
	 _selectionEE->passCut("jetConeWidth",deltaR)
	// && (m_HoEElectronMinus < 0.2)  
	// && (m_CaloEneElectronMinus/energyJet[j] > 0.9)
	 ) continue;
    }
    if ( m_p4ElectronPlus->Vect().Mag() != 0 ) {
      float deltaR =  p3Jet.DeltaR( m_p4ElectronPlus->Vect() );
      if(_selectionEE->getSwitch("jetConeWidth") && 
	 _selectionEE->passCut("jetConeWidth",deltaR)
	 // && (m_HoEElectronPlus < 0.2)
	 // && (m_CaloEneElectronPlus/energyJet[j] > 0.9)
	 ) continue;
    }

    if(etSisConeJet[j]>maxPtJet) maxPtJet=etSisConeJet[j];
    if(_selectionEE->getSwitch("etaJetAcc") && !_selectionEE->passCut("etaJetAcc",etaSisConeJet[j])) continue;
    if(_selectionEE->getSwitch("etJetLowAcc") && !_selectionEE->passCut("etJetLowAcc",etSisConeJet[j]) ) continue;

    if( (_selectionEE->getSwitch("etJetHighAcc") && _selectionEE->passCut("etJetHighAcc",etSisConeJet[j])) &&
 	(_selectionEE->getSwitch("alphaJet") && !_selectionEE->passCut("alphaJet",alphaSisConeJet[j])) 
	) continue;
    foundJet=true;
    break;
  }

  return foundJet;

}


float HiggsSelection::Fisher(int eleIndex) {
  float fisher=0;
  // CMSSW_1_3_1 coefficients, obsolete!
//   if(classificationEle[eleIndex]<100)
//     fisher = 42.0238-3.38943*s9s25Ele[eleIndex]-794.092*sqrt(covEtaEtaEle[eleIndex])-15.3449*latEle[eleIndex]-31.1032*a20Ele[eleIndex];
//   else
//     fisher = 27.2967+2.97453*s9s25Ele[eleIndex]-169.219*sqrt(covEtaEtaEle[eleIndex])-17.0445*latEle[eleIndex]-24.8542*a20Ele[eleIndex];
  return fisher;
}

void HiggsSelection::resetKinematics() {

  theElectron = -1;
  thePositron = -1;
  theMuonMinus = -1;
  theMuonPlus = -1;
  m_p4ElectronMinus->SetXYZT(0,0,0,0);
  m_p4ElectronPlus->SetXYZT(0,0,0,0);
  m_p4MuonMinus->SetXYZT(0,0,0,0);
  m_p4MuonPlus->SetXYZT(0,0,0,0);
  m_p4MET->SetXYZT(0,0,0,0);
  m_HoEElectronMinus = 0;
  m_HoEElectronPlus = 0;
  m_CaloEneElectronMinus = 0;
  m_CaloEneElectronPlus = 0;

  for(int ichan=0;ichan<3;ichan++) {
    m_deltaPhi[ichan] = 0;
    m_mll[ichan] = 0;
    m_transvMass[ichan] = 0;
  }
  
  hardestElectronPt = 0;
  hardestMuonPt = 0;
  slowestElectronPt = 0;
  slowestMuonPt = 0;

}


float HiggsSelection::getSecondEleTkPt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleTrackPt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  if( dr < deltaR ) { 
    secondEleTrackPt = momentumEle[second] * fabs( sin(thetaEle[second]) );
  }

  return secondEleTrackPt;

}

float HiggsSelection::getSecondEleEmEt(int first, int second, float deltaR) {

  TVector3 firstEle(pxEle[first],pyEle[first],pzEle[first]);
  TVector3 secondEle(pxEle[second],pyEle[second],pzEle[second]);

  float secondEleEmEt = 0.0;
  float dr = firstEle.DeltaR(secondEle);

  // remove part of the supercluster energy assuming that dR supercluster ~ 0.1
  // dR(1cryEB) = 0.0175 => dR(10x10) = 10*0.0175/sqrt(2)
  float dR10x10 = 10*0.0175/sqrt(2);

  if( dr < deltaR+dR10x10 ) { 
    // assume that the Sclu is a 10x10 circle. Formula is exact only if
    // the isolation circle and SC circle are equal (not true: 0.4 vs ~ 0.1)
    float alpha = acos(dr/(2*deltaR));
    float overlappingArea = deltaR*deltaR*(4*alpha - sin(2*alpha));
    float fraction = overlappingArea/(deltaR*deltaR);
    secondEleEmEt = fraction * energyEle[second] * fabs( sin(thetaEle[second]) );
  }

  return secondEleEmEt;

}


vector<int> HiggsSelection::resolvedElectrons() {

  float drmin=0.1;
  vector<int> amb_index;
  vector<int> resolvedEles;
  vector<std::pair<int, int> > ambEle;

  ambEle.clear();

  if(nEle==1) {
    resolvedEles.push_back(0);
    return resolvedEles;
  }

  // first electron
  for(int i=0;i<nEle;i++) {
    TVector3 pEle1(pxEle[i],pyEle[i],pzEle[i]);
    int no_ambiguity=0;
    // second electron
    for(int j=i+1;j<nEle;j++) {
      TVector3 pEle2(pxEle[j],pyEle[j],pzEle[j]);
      float dr = pEle1.DeltaR(pEle2);
      if(dr<drmin) {
        amb_index.push_back(j);
        ambEle.push_back(std::make_pair(i,j));
      } else no_ambiguity++;
    }
    bool test=true;
    for (int t=0; t<amb_index.size(); t++){
      if (amb_index[t] == i ) test=false;
    }

    if(no_ambiguity == (nEle-1-i) && test == true) { 
      resolvedEles.push_back(i);
    }
  }

  // resolve ambiguities
  std::vector< pair<int,int> >::const_iterator it;
  for(it=ambEle.begin(); it<ambEle.end(); it++) {
    int bestEleId=(int)it->first;
    int multiAmbEleId=(int)it->first;
    while((int)it->first==multiAmbEleId && it<ambEle.end()) {
      int bestEle = bestEleId;
      int compEle = it->second;
      if(fabs(eSuperClusterOverPEle[compEle]-1) <= fabs(eSuperClusterOverPEle[bestEle]-1)) bestEleId=it->second;
      it++;
    }
    resolvedEles.push_back(bestEleId);
  }

  return resolvedEles;

}
