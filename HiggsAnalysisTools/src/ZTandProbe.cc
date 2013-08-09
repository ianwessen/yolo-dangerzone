#include <iostream>
#include <string>

#include <TTree.h>

#include "CommonTools/include/Counters.hh"
#include "CommonTools/include/Selection.hh"
#include "CommonTools/include/EfficiencyEvaluator.hh"
#include "HiggsAnalysisTools/include/ZTandProbe.hh"

ZTandProbe::ZTandProbe(TTree *tree) 
  : HiggsBase(tree) {
  
  // Initialize kine selection
  std::string fileCuts("config/zee/zee_T&P_Cuts.txt");
  std::string fileSwitches("config/zee/zee_T&P_Switches.txt");

  _selection = new Selection(fileCuts,fileSwitches);
  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addCut("TagPtInf");           
  _selection->addCut("TagTrackerPtSum");           
  _selection->addCut("ProbePtInf");           
  _selection->addCut("ProbeTrackerPtSum");           
  _selection->addCut("invMass");           

  // cut based EleID: class dependent or not (true to use likelihood)
  bool classDepID = true;    

  // Initialize eleID selections
  Selection *_goldenSelectionEB, *_bigbremSelectionEB, *_narrowSelectionEB, *_showeringSelectionEB; 
  Selection *_goldenSelectionEE, *_bigbremSelectionEE, *_narrowSelectionEE, *_showeringSelectionEE; 

  if (classDepID){
    _goldenSelectionEB    = new Selection("config/zee/eleID_T&P_GoldenCuts.txt",     "config/zee/eleID_T&P_Switches.txt");
    _bigbremSelectionEB   = new Selection("config/zee/eleID_T&P_BigBremCuts.txt",    "config/zee/eleID_T&P_Switches.txt");
    _narrowSelectionEB    = new Selection("config/zee/eleID_T&P_NarrowCuts.txt",     "config/zee/eleID_T&P_Switches.txt");
    _showeringSelectionEB = new Selection("config/zee/eleID_T&P_ShoweringCuts.txt",  "config/zee/eleID_T&P_Switches.txt");
    _goldenSelectionEE    = new Selection("config/zee/eleID_T&P_GoldenCutsEE.txt",   "config/zee/eleID_T&P_Switches.txt");
    _bigbremSelectionEE   = new Selection("config/zee/eleID_T&P_BigBremCutsEE.txt",  "config/zee/eleID_T&P_Switches.txt");
    _narrowSelectionEE    = new Selection("config/zee/eleID_T&P_NarrowCutsEE.txt",   "config/zee/eleID_T&P_Switches.txt");
    _showeringSelectionEE = new Selection("config/zee/eleID_T&P_ShoweringCutsEE.txt","config/zee/eleID_T&P_Switches.txt");
  }
  if (!classDepID){
    _goldenSelectionEB    = new Selection("config/zee/eleID_T&P_noClassCutsEB.txt",  "config/zee/eleID_T&P_Switches.txt");
    _bigbremSelectionEB   = new Selection("config/zee/eleID_T&P_noClassCutsEB.txt",  "config/zee/eleID_T&P_Switches.txt");
    _narrowSelectionEB    = new Selection("config/zee/eleID_T&P_noClassCutsEB.txt",  "config/zee/eleID_T&P_Switches.txt");
    _showeringSelectionEB = new Selection("config/zee/eleID_T&P_noClassCutsEB.txt",  "config/zee/eleID_T&P_Switches.txt");
    _goldenSelectionEE    = new Selection("config/zee/eleID_T&P_noClassCutsEE.txt",  "config/zee/eleID_T&P_Switches.txt");
    _bigbremSelectionEE   = new Selection("config/zee/eleID_T&P_noClassCutsEE.txt",  "config/zee/eleID_T&P_Switches.txt");
    _narrowSelectionEE    = new Selection("config/zee/eleID_T&P_noClassCutsEE.txt",  "config/zee/eleID_T&P_Switches.txt");
    _showeringSelectionEE = new Selection("config/zee/eleID_T&P_noClassCutsEE.txt",  "config/zee/eleID_T&P_Switches.txt");
  }

  _electronSelection.push_back(_goldenSelectionEB);
  _electronSelection.push_back(_bigbremSelectionEB);
  _electronSelection.push_back(_narrowSelectionEB);
  _electronSelection.push_back(_showeringSelectionEB);
  _electronSelection.push_back(_goldenSelectionEE);
  _electronSelection.push_back(_bigbremSelectionEE);
  _electronSelection.push_back(_narrowSelectionEE);
  _electronSelection.push_back(_showeringSelectionEE);


  std::vector<Selection*>::iterator selectionItr;
  for(selectionItr=_electronSelection.begin();selectionItr!=_electronSelection.end();selectionItr++) {
    Selection *eleSelection = *selectionItr;

    // tag eleID selections
    eleSelection->addCut("Tag_hOverE");
    eleSelection->addCut("Tag_s9s25");
    eleSelection->addCut("Tag_deta");
    eleSelection->addCut("Tag_dphiIn");
    eleSelection->addCut("Tag_dphiOut");
    eleSelection->addCut("Tag_invEMinusInvP");
    eleSelection->addCut("Tag_bremFraction");
    eleSelection->addCut("Tag_covEtaEta");
    eleSelection->addCut("Tag_eOverPout");
    eleSelection->addCut("Tag_eOverPin");
    eleSelection->addCut("Tag_Fisher");
    eleSelection->addCut("Tag_Likelihood");

    // probe eleID selections
    eleSelection->addCut("Probe_hOverE");
    eleSelection->addCut("Probe_s9s25");
    eleSelection->addCut("Probe_deta");
    eleSelection->addCut("Probe_dphiIn");
    eleSelection->addCut("Probe_dphiOut");
    eleSelection->addCut("Probe_invEMinusInvP");
    eleSelection->addCut("Probe_bremFraction");
    eleSelection->addCut("Probe_covEtaEta");
    eleSelection->addCut("Probe_eOverPout");
    eleSelection->addCut("Probe_eOverPin");
    eleSelection->addCut("Probe_Fisher");
    eleSelection->addCut("Probe_Likelihood");

    eleSelection->summary();
  }


  std::map<std::string,std::pair<float,float> > selectionMap = _selection->getSelection();
  _selection->summary();

  // Initialize additional counters  
  _counter.SetTitle("EVENT COUNTER");
  _counter.AddVar("event");
  _counter.AddVar("MCtruth");
  _counter.AddVar("trigger");

  _eleCounter.SetTitle("SINGLE ELECTRONS (TAG & PROBE) COUNTER");
  _eleCounter.AddVar("Probe_electrons");
  _eleCounter.AddVar("Probe_hOverE");
  _eleCounter.AddVar("Probe_s9s25");
  _eleCounter.AddVar("Probe_deta");
  _eleCounter.AddVar("Probe_dphiIn");
  _eleCounter.AddVar("Probe_dphiOut");
  _eleCounter.AddVar("Probe_invEMinusInvP");
  _eleCounter.AddVar("Probe_bremFraction");
  _eleCounter.AddVar("Probe_covEtaEta");
  _eleCounter.AddVar("Probe_eOverPout");
  _eleCounter.AddVar("Probe_eOverPin");
  _eleCounter.AddVar("Probe_Fisher");
  _eleCounter.AddVar("Probe_Likelihood");
  _eleCounter.AddVar("Probe_finalEleID");

  _eleCounter.AddVar("Tag_electrons");
  _eleCounter.AddVar("Tag_hOverE");
  _eleCounter.AddVar("Tag_s9s25");
  _eleCounter.AddVar("Tag_deta");
  _eleCounter.AddVar("Tag_dphiIn");
  _eleCounter.AddVar("Tag_dphiOut");
  _eleCounter.AddVar("Tag_invEMinusInvP");
  _eleCounter.AddVar("Tag_bremFraction");
  _eleCounter.AddVar("Tag_covEtaEta");
  _eleCounter.AddVar("Tag_eOverPout");
  _eleCounter.AddVar("Tag_eOverPin");
  _eleCounter.AddVar("Tag_Fisher");
  _eleCounter.AddVar("Tag_Likelihood");
  _eleCounter.AddVar("Tag_finalEleID");


  // the kinematic vectors
  _p3Tag   = new TVector3(0.,0.,0.);
  _p3Probe = new TVector3(0.,0.,0.);  
  _p4Tag   = new TLorentzVector(0.,0.,0.,0.);
  _p4Probe = new TLorentzVector(0.,0.,0.,0.);
}

ZTandProbe::~ZTandProbe(){

  delete _p3Tag;
  delete _p4Tag;
  delete _p3Probe;
  delete _p4Probe;
}

bool ZTandProbe::findMcTree(const char* processType) {

  // basic trigger studies preselections - see HLT note
  //if( (pMc[7]*fabs(sin(thetaMc[7])) < 5) || (fabs(etaMc[7])>2.5) ) { return false; }
  //if( (pMc[8]*fabs(sin(thetaMc[8])) < 5) || (fabs(etaMc[8])>2.5) ) { return false; }

  if(strcmp(processType,"Ztoee")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0)
	    );
  if(strcmp(processType,"ZtoeeBarrel")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0 &&
	     fabs(etaMc[7])<1.47 && fabs(etaMc[8])<1.47)
	    );
  if(strcmp(processType,"ZtoeeEndcap")==0)
    return (
	    (abs(idMc[6])==23) && abs(idMc[7])==11 && abs(idMc[8])==11 &&
	    (idMc[7]*idMc[8]<0 &&
	     fabs(etaMc[7])>1.47 && fabs(etaMc[8])>1.47)
	    );
  else {
    std::cout << "This processType: " << processType << "is not expected, you should put MTtruth switch off" << std::endl;
    return false;
  }
}
  
void ZTandProbe::Loop() {
  _verbose=true;
  if(fChain == 0) return;

  bookHistos();
  TFile *file = new TFile("histograms.root","RECREATE");

  Long64_t nbytes = 0, nb = 0;
  Long64_t nentries = fChain->GetEntries();
  if(_verbose) std::cout << "Number of entries = " << nentries << std::endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    _counter.IncrVar("event");
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (_verbose && jentry%1000 == 0) std::cout << ">>> Processing event # " << jentry << std::endl;


    // filling histos with MC distributions - all events
    Gene_eta->Fill(etaMc[7]);   
    Gene_eta->Fill(etaMc[8]);
    Gene_pt->Fill(pMc[7]*fabs(sin(thetaMc[7])));
    Gene_pt->Fill(pMc[8]*fabs(sin(thetaMc[8])));

    // mc info
    if(_selection->getSwitch("MCtruth") && !findMcTree("Ztoee") ) { continue; }             
    _counter.IncrVar("MCtruth");
    Hmc_eta -> Fill(etaMc[7]);   
    Hmc_eta -> Fill(etaMc[8]);   
    Hmc_pt -> Fill(pMc[7]*fabs(sin(thetaMc[7])));   
    Hmc_pt -> Fill(pMc[8]*fabs(sin(thetaMc[8])));   

    // trigger info
    if(_selection->getSwitch("trigger") && !(singleElePassedTrg || doubleElePassedTrg) ) continue;    
    _counter.IncrVar("trigger"); 
    Htrigger_eta -> Fill(etaMc[7]);   
    Htrigger_eta -> Fill(etaMc[8]);   
    Htrigger_pt -> Fill(pMc[7]*fabs(sin(thetaMc[7])));
    Htrigger_pt -> Fill(pMc[8]*fabs(sin(thetaMc[8])));

    // looking for a tag electron
    bool isoFill[2], eleIDFill[2], ptFill[2];
    for (int ii=0; ii<2; ii++){
      isoFill[ii]   = false;
      eleIDFill[ii] = false;
      ptFill[ii]    = false;
    }

    // loop over ALL reconstructed electrons
    for(int theTag=0; theTag<nEle; theTag++) {
      
      double thisTagPt = sqrt(pxEle[theTag]*pxEle[theTag] + pyEle[theTag]*pyEle[theTag]); 

      // matching with mc
      int mcBin = 1000;
      float mcEta, mcPt, deltaR;
      TVector3 tagPAtInner(pxAtInnerEle[theTag],pyAtInnerEle[theTag],pzAtInnerEle[theTag]);
      TVector3 trueEleP7, trueEleP8;
      trueEleP7.SetPtThetaPhi(pMc[7]*fabs(sin(thetaMc[7])),thetaMc[7],phiMc[7]);
      trueEleP8.SetPtThetaPhi(pMc[8]*fabs(sin(thetaMc[8])),thetaMc[8],phiMc[8]);
      float deltaR7 = trueEleP7.DeltaR(tagPAtInner);
      float deltaR8 = trueEleP8.DeltaR(tagPAtInner);
      if (deltaR7 < deltaR8){ deltaR = deltaR7; mcEta = etaMc[7]; mcPt = pMc[7]*fabs(sin(thetaMc[7])); mcBin = 0;}
      if (deltaR8 < deltaR7){ deltaR = deltaR8; mcEta = etaMc[8]; mcPt = pMc[8]*fabs(sin(thetaMc[8])); mcBin = 1;}


      // efficiency histos step by step
      if (_selection->passCut("TagTrackerPtSum",eleSumPt04Ele[theTag]) && (!isoFill[mcBin]) ){ 
	if (deltaR<0.5){
	  Hiso_eta -> Fill(mcEta);   
	  Hiso_pt -> Fill(mcPt);
	  isoFill[mcBin] = true;
	}}
      
      if (_selection->passCut("TagTrackerPtSum",eleSumPt04Ele[theTag])){ 
	if (isTagEleID(theTag) && !eleIDFill[mcBin] ){                                                  
	  if (deltaR<0.5){
	    HeleID_eta -> Fill(mcEta);   
	    HeleID_pt -> Fill(mcPt);
	    eleIDFill[mcBin] = true;
	  }}}
      
      if (_selection->passCut("TagTrackerPtSum",eleSumPt04Ele[theTag])){ 
	if (isTagEleID(theTag)){
	  if (_selection->passCut("TagPtInf", thisTagPt) && (!ptFill[mcBin])){ 
	    if (deltaR<0.5){
	      Hpt_eta -> Fill(mcEta);   
	      Hpt_pt -> Fill(mcPt);
	      ptFill[mcBin] = true;
	    }}}}
      
      
      // is this a tag?
      if( !(isATag(theTag)) ){ continue; }  
      
      // this is a tag; now, search for a probe electron
      int bestProbe=-999;
      double ptBestProbe=-999.;
      for(int theProbe=0; theProbe<nEle; theProbe++) {

	if (theTag == theProbe){ continue; }
	
	setKinematics(theTag,theProbe);
	float invMass = (*_p4Tag+*_p4Probe).M();

	if(!(isAProbe(theProbe, theTag, invMass))){ continue; }  
	
	// this is a probe (noEleID requirement). Is it the best one?
	double thisPt = sqrt(pxEle[theProbe]*pxEle[theProbe] + pyEle[theProbe]*pyEle[theProbe]); 
	if (thisPt > ptBestProbe){ ptBestProbe = thisPt; bestProbe = theProbe; }
      
      } // loop over possible probes
      

      // to study efficiency of eleID on probes: all tags
      H_tagEta -> Fill(etaEle[theTag]);

      // only if I reconstruct the probe
      if (bestProbe > -100){
	
	// to study efficiency of eleID on probes: all probes
	H_probeAllEta -> Fill(etaEle[bestProbe]);
	H_probeAllPt -> Fill(etEle[bestProbe]);

	// to determine pdfs for eleID
	bool isEndcap = true;
	if (fabs(etaEle[bestProbe]) < 1.476)  { isEndcap = false; }
	int probeClass = classificationEle[bestProbe];
	
	// all probe candidates without eleID requirements
	if(probeClass == 0 || probeClass == 100) {
	  dPhiVtx0[isEndcap]  -> Fill(deltaPhiAtVtxEle[bestProbe]);
	  dEtaCalo0[isEndcap] -> Fill(eleDeltaEtaAtCaloEle[bestProbe]);
	  dEtaVtx0[isEndcap] -> Fill(deltaEtaAtVtxEle[bestProbe]);
	  EoPout0[isEndcap]   -> Fill(eSeedOverPoutEle[bestProbe]);
	  HoE0[isEndcap]      -> Fill(hOverEEle[bestProbe]);
	  if(isEndcap==0)
	    shapeFisher0[isEndcap]->Fill(42.0238-3.38943*s9s25Ele[bestProbe]-794.092*sqrt(covEtaEtaEle[bestProbe])-15.3449*latEle[bestProbe]-31.1032*a20Ele[bestProbe]);
	  if(isEndcap==1)
	    shapeFisher0[isEndcap]->Fill(27.2967+2.97453*s9s25Ele[bestProbe]-169.219*sqrt(covEtaEtaEle[bestProbe])-17.0445*latEle[bestProbe]-24.8542*a20Ele[bestProbe]);
	}
	else if(probeClass == 10 || probeClass == 110) {
	  dPhiVtx1[isEndcap]  -> Fill(deltaPhiAtVtxEle[bestProbe]);
	  dEtaCalo1[isEndcap] -> Fill(eleDeltaEtaAtCaloEle[bestProbe]);
	  dEtaVtx1[isEndcap] -> Fill(deltaEtaAtVtxEle[bestProbe]);
	  EoPout1[isEndcap]   -> Fill(eSeedOverPoutEle[bestProbe]);
	  if(isEndcap==0)
	    shapeFisher1[isEndcap]->Fill(42.0238-3.38943*s9s25Ele[bestProbe]-794.092*sqrt(covEtaEtaEle[bestProbe])-15.3449*latEle[bestProbe]-31.1032*a20Ele[bestProbe]);
	  if(isEndcap==1)
	    shapeFisher1[isEndcap]->Fill(27.2967+2.97453*s9s25Ele[bestProbe]-169.219*sqrt(covEtaEtaEle[bestProbe])-17.0445*latEle[bestProbe]-24.8542*a20Ele[bestProbe]);
	}
	else if(probeClass == 20 || probeClass == 120) {
	  dPhiVtx2[isEndcap]  -> Fill(deltaPhiAtVtxEle[bestProbe]);
	  dEtaCalo2[isEndcap] -> Fill(eleDeltaEtaAtCaloEle[bestProbe]);
	  dEtaVtx2[isEndcap] -> Fill(deltaEtaAtVtxEle[bestProbe]);
  	  EoPout2[isEndcap]   -> Fill(eSeedOverPoutEle[bestProbe]);
	  if(isEndcap==0)
	    shapeFisher2[isEndcap]->Fill(42.0238-3.38943*s9s25Ele[bestProbe]-794.092*sqrt(covEtaEtaEle[bestProbe])-15.3449*latEle[bestProbe]-31.1032*a20Ele[bestProbe]);
	  if(isEndcap==1)
	    shapeFisher2[isEndcap]->Fill(27.2967+2.97453*s9s25Ele[bestProbe]-169.219*sqrt(covEtaEtaEle[bestProbe])-17.0445*latEle[bestProbe]-24.8542*a20Ele[bestProbe]);
	}
	else if(probeClass == 30 || probeClass == 31 || probeClass == 32 || probeClass == 33 || probeClass == 34 || probeClass == 130 || probeClass == 131 || probeClass == 132 || probeClass == 133 || probeClass == 134) { 
	  dPhiVtx3[isEndcap]  -> Fill(deltaPhiAtVtxEle[bestProbe]);
	  dEtaCalo3[isEndcap] -> Fill(eleDeltaEtaAtCaloEle[bestProbe]);
	  dEtaVtx3[isEndcap] -> Fill(deltaEtaAtVtxEle[bestProbe]);
	  EoPout3[isEndcap]   -> Fill(eSeedOverPoutEle[bestProbe]);
	  HoE3[isEndcap]      -> Fill(hOverEEle[bestProbe]);
	  if(isEndcap==0)
	    shapeFisher3[isEndcap]->Fill(42.0238-3.38943*s9s25Ele[bestProbe]-794.092*sqrt(covEtaEtaEle[bestProbe])-15.3449*latEle[bestProbe]-31.1032*a20Ele[bestProbe]);
	  if(isEndcap==1)
	    shapeFisher3[isEndcap]->Fill(27.2967+2.97453*s9s25Ele[bestProbe]-169.219*sqrt(covEtaEtaEle[bestProbe])-17.0445*latEle[bestProbe]-24.8542*a20Ele[bestProbe]);
	}
      
	// eleID efficiency on probes             
	if( isProbeEleID(bestProbe)){ 
	  H_probeOkEta -> Fill(etaEle[bestProbe]);
	  H_probeOkPt -> Fill(etEle[bestProbe]);
	}
      
      } // found the probe electron
      

    } // loop over possible tags
    
  } // loop over entries
}


bool ZTandProbe::isATag(int theTag) {

  bool isThisOk = true;
  
  double thisPt = sqrt(pxEle[theTag]*pxEle[theTag] + pyEle[theTag]*pyEle[theTag]); 
  if ( !_selection->passCut("TagTrackerPtSum",eleSumPt04Ele[theTag])){ isThisOk = false; } 
  if ( !isTagEleID(theTag) )                                                  { isThisOk = false; }  
  if ( (fabs(etaEle[theTag]) > 1.444) && (fabs(etaEle[theTag]) < 1.526) )     { isThisOk = false; } 
  if ( (fabs(etaEle[theTag]) > 2.5) )                                         { isThisOk = false; }   
  if ( !_selection->passCut("TagPtInf",       thisPt) )                       { isThisOk = false; } 

  return isThisOk;
}

// requirements on eta acceptance, pt, isolation and matching with the tag
// no requirement at this level on eleID - put in the loop to study efficiency
bool ZTandProbe::isAProbe(int theProbe, int theTag, float mll) {

  bool isThisOk = true;
  
  double probePt = sqrt(pxEle[theProbe]*pxEle[theProbe] + pyEle[theProbe]*pyEle[theProbe]); 

  // default eta acceptance, not to be changed
  if ( (fabs(etaEle[theProbe]) > 1.444) && (fabs(etaEle[theProbe]) < 1.526) )     { isThisOk = false; } 
  if ( (fabs(etaEle[theProbe]) > 2.5) )                                           { isThisOk = false; } 

  // opposite charge
  if ( chargeEle[theProbe]*chargeEle[theTag]>0 )                                  { isThisOk = false; } 

  // to be set via cfg file
  //float thisDeltaLip = fabs(vertexZ[theProbe] - vertexZ[theTag]);
  if ( !_selection->passCut("ProbePtInf",       probePt) )                        { isThisOk = false; } 
  if ( !_selection->passCut("ProbeTrackerPtSum",eleSumPt04Ele[theProbe])){ isThisOk = false; } 
  //if ( !_selection->passCut("deltaLip", thisDeltaLip) )                           { isThisOk = false; } 
  if ( !_selection->passCut("invMass",  mll) )                                    { isThisOk = false; } 
  
  return isThisOk;
}

void ZTandProbe::displayEfficiencies() {

  std::cout << std::endl;
  std::cout << "Tag electron: eleID selections" << std::endl; 
  _eleCounter.Draw("Tag_hOverE",       "Tag_electrons");
  _eleCounter.Draw("Tag_s9s25",        "Tag_hOverE");
  _eleCounter.Draw("Tag_deta",         "Tag_s9s25");
  _eleCounter.Draw("Tag_dphiIn",       "Tag_deta");
  _eleCounter.Draw("Tag_dphiOut",      "Tag_dphiIn");
  _eleCounter.Draw("Tag_invEMinusInvP","Tag_dphiOut");
  _eleCounter.Draw("Tag_bremFraction", "Tag_invEMinusInvP");
  _eleCounter.Draw("Tag_covEtaEta",    "Tag_bremFraction");
  _eleCounter.Draw("Tag_eOverPout",    "Tag_covEtaEta");
  _eleCounter.Draw("Tag_eOverPin",     "Tag_eOverPout");
  _eleCounter.Draw("Tag_Fisher",       "Tag_eOverPin");
  _eleCounter.Draw("Tag_Likelihood",   "Tag_Fisher");
  _eleCounter.Draw("Tag_finalEleID",   "Tag_electrons");

  std::cout << std::endl;
  std::cout << "Probe electron: eleID selections" << std::endl; 
  _eleCounter.Draw("Probe_hOverE",       "Probe_electrons");
  _eleCounter.Draw("Probe_s9s25",        "Probe_hOverE");
  _eleCounter.Draw("Probe_deta",         "Probe_s9s25");
  _eleCounter.Draw("Probe_dphiIn",       "Probe_deta");
  _eleCounter.Draw("Probe_dphiOut",      "Probe_dphiIn");
  _eleCounter.Draw("Probe_invEMinusInvP","Probe_dphiOut");
  _eleCounter.Draw("Probe_bremFraction", "Probe_invEMinusInvP");
  _eleCounter.Draw("Probe_covEtaEta",    "Probe_bremFraction");
  _eleCounter.Draw("Probe_eOverPout",    "Probe_covEtaEta");
  _eleCounter.Draw("Probe_eOverPin",     "Probe_eOverPout");
  _eleCounter.Draw("Probe_Fisher",       "Probe_eOverPin");
  _eleCounter.Draw("Probe_Likelihood",   "Probe_Fisher");
  _eleCounter.Draw("Probe_finalEleID",   "Probe_electrons");

  std::cout << std::endl;
  std::cout << "event selections" << std::endl; 
  _counter.Draw("MCtruth",       "event");
  _counter.Draw("trigger",       "MCtruth");


  // tag reconstruction step by step
  EfficiencyEvaluator recoEleEta("recoEleTaPEta.root");
  recoEleEta.AddNumerator(Gene_eta);
  recoEleEta.AddNumerator(Htrigger_eta);
  recoEleEta.AddNumerator(Hmc_eta);
  recoEleEta.AddNumerator(Hiso_eta);
  recoEleEta.AddNumerator(HeleID_eta);
  recoEleEta.AddNumerator(Hpt_eta);
  recoEleEta.SetDenominator(Gene_eta);
  recoEleEta.ComputeEfficiencies();
  recoEleEta.Write();

  EfficiencyEvaluator recoElePt("recoEleTaPPt.root");
  recoElePt.AddNumerator(Gene_pt);
  recoElePt.AddNumerator(Htrigger_pt);
  recoElePt.AddNumerator(Hmc_pt);
  recoElePt.AddNumerator(Hiso_pt);
  recoElePt.AddNumerator(HeleID_pt);
  recoElePt.AddNumerator(Hpt_pt);
  recoElePt.SetDenominator(Gene_pt);
  recoElePt.ComputeEfficiencies();
  recoElePt.Write();



  // electron ID details on probe
  EfficiencyEvaluator eleIDEta("eleIdTaPEta.root");
  eleIDEta.AddNumerator(H_probeAllEta);
  eleIDEta.AddNumerator(H_probeGoldenEta);
  eleIDEta.AddNumerator(H_probeBigBremEta);
  eleIDEta.AddNumerator(H_probeNarrowEta);
  eleIDEta.AddNumerator(H_probeShoweringEta);
  eleIDEta.AddNumerator(H_probeCracksEta);
  eleIDEta.AddNumerator(H_probeHoEOkEta);
  eleIDEta.AddNumerator(H_probeS9S25OkEta);
  eleIDEta.AddNumerator(H_probedEtaOkEta);
  eleIDEta.AddNumerator(H_probedPhiOkEta);
  eleIDEta.AddNumerator(H_probeCovEtaEtaOkEta);
  eleIDEta.AddNumerator(H_probeEoPoutOkEta);
  eleIDEta.AddNumerator(H_probeOkEta);
  eleIDEta.SetDenominator(H_probeAllEta);
  eleIDEta.ComputeEfficiencies();
  eleIDEta.Write();

  EfficiencyEvaluator eleIDPt("eleIdTaPPt.root");
  eleIDPt.AddNumerator(H_probeAllPt);
  eleIDPt.AddNumerator(H_probeGoldenPt);
  eleIDPt.AddNumerator(H_probeBigBremPt);
  eleIDPt.AddNumerator(H_probeNarrowPt);
  eleIDPt.AddNumerator(H_probeShoweringPt);
  eleIDPt.AddNumerator(H_probeCracksPt);
  eleIDPt.AddNumerator(H_probeHoEOkPt);
  eleIDPt.AddNumerator(H_probeS9S25OkPt);
  eleIDPt.AddNumerator(H_probedEtaOkPt);
  eleIDPt.AddNumerator(H_probedPhiOkPt);
  eleIDPt.AddNumerator(H_probeCovEtaEtaOkPt);
  eleIDPt.AddNumerator(H_probeEoPoutOkPt);
  eleIDPt.AddNumerator(H_probeOkPt);
  eleIDPt.SetDenominator(H_probeAllPt);
  eleIDPt.ComputeEfficiencies();
  eleIDPt.Write();

}


void ZTandProbe::writeHistos() {

  TFile pdfFile("pdfs.root","RECREATE");
  for (int ii=0; ii<2; ii++){
    dPhiVtx0[ii]       -> Write(); 
    dEtaCalo0[ii]      -> Write(); 
    dEtaVtx0[ii]      -> Write(); 
    EoPout0[ii]        -> Write(); 
    HoE0[ii]           -> Write(); 
    shapeFisher0[ii]   -> Write(); 

    dPhiVtx1[ii]       -> Write(); 
    dEtaCalo1[ii]      -> Write(); 
    dEtaVtx1[ii]      -> Write(); 
    EoPout1[ii]        -> Write(); 
    HoE1[ii]           -> Write(); 
    shapeFisher1[ii]   -> Write(); 

    dPhiVtx2[ii]       -> Write(); 
    dEtaCalo2[ii]      -> Write(); 
    dEtaVtx2[ii]      -> Write(); 
    EoPout2[ii]        -> Write(); 
    HoE2[ii]           -> Write(); 
    shapeFisher2[ii]   -> Write(); 

    dPhiVtx3[ii]       -> Write(); 
    dEtaCalo3[ii]      -> Write(); 
    dEtaVtx3[ii]      -> Write(); 
    EoPout3[ii]        -> Write(); 
    HoE3[ii]           -> Write(); 
    shapeFisher3[ii]   -> Write(); 
  }

  pdfFile.Close();
}


bool ZTandProbe::isProbeEleID(int eleIndex) {

  int GsfClass = classificationEle[eleIndex];
  int offset=0;
  if(GsfClass>=100) { GsfClass-=100; offset=4; }

  Selection *selection;
  if(GsfClass==0) {
    selection=_electronSelection[offset];
    H_probeGoldenEta->Fill(etaEle[eleIndex]);
    H_probeGoldenPt->Fill(etEle[eleIndex]);
  }
  else if(GsfClass==10) {
    selection=_electronSelection[offset+1];
    H_probeBigBremEta->Fill(etaEle[eleIndex]);
    H_probeBigBremPt->Fill(etEle[eleIndex]);
  }
  else if(GsfClass==20) {
    selection=_electronSelection[offset+2];
    H_probeNarrowEta->Fill(etaEle[eleIndex]);
    H_probeNarrowPt->Fill(etEle[eleIndex]);
  }
  else if(GsfClass>=30 && GsfClass<=40) {
    selection=_electronSelection[offset+3];
    if(GsfClass<40) {
      H_probeShoweringEta->Fill(etaEle[eleIndex]);
      H_probeShoweringPt->Fill(etEle[eleIndex]);
    }
    if(GsfClass==40) {
      H_probeCracksEta->Fill(etaEle[eleIndex]);
      H_probeCracksPt->Fill(etEle[eleIndex]);
    }
  }

  // for the electron class fraction


  TVector3 pTrkAtInner(pxAtInnerEle[eleIndex],pyAtInnerEle[eleIndex],pzAtInnerEle[eleIndex]);
  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  _eleCounter.IncrVar("Probe_electrons");

  if(selection->getSwitch("Probe_hOverE")        && !selection->passCut("Probe_hOverE",hOverEEle[eleIndex]))             return false;  
  _eleCounter.IncrVar("Probe_hOverE");
  H_probeHoEOkEta->Fill(etaEle[eleIndex]);
  H_probeHoEOkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_s9s25")         && !selection->passCut("Probe_s9s25",s9s25Ele[eleIndex]))               return false; 
  _eleCounter.IncrVar("Probe_s9s25");
  H_probeS9S25OkEta->Fill(etaEle[eleIndex]);
  H_probeS9S25OkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_deta")          && !selection->passCut("Probe_deta",deltaEtaAtVtxEle[eleIndex]))     return false; 
  _eleCounter.IncrVar("Probe_deta");
  H_probedEtaOkEta->Fill(etaEle[eleIndex]);
  H_probedEtaOkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_dphiIn")        && !selection->passCut("Probe_dphiIn",deltaPhiAtVtxEle[eleIndex]))   return false; 
  _eleCounter.IncrVar("Probe_dphiIn");
  H_probedPhiOkEta->Fill(etaEle[eleIndex]);
  H_probedPhiOkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_dphiOut")        && !selection->passCut("Probe_dphiOut",deltaPhiAtCaloEle[eleIndex])) return false; 
  _eleCounter.IncrVar("Probe_dphiOut");

  if(selection->getSwitch("Probe_invEMinusInvP") && !selection->passCut("Probe_invEMinusInvP",1./ecalEle[eleIndex]-1./pTrkAtInner.Mag()))          return false; 
  _eleCounter.IncrVar("Probe_invEMinusInvP");

  if(selection->getSwitch("Probe_bremFraction")  && !selection->passCut("Probe_bremFraction",fabs(pTrkAtInner.Mag()-pTrkAtOuter.Mag())/pTrkAtInner.Mag())) return false; 
  _eleCounter.IncrVar("Probe_bremFraction");

  if(selection->getSwitch("Probe_covEtaEta")     && !selection->passCut("Probe_covEtaEta",covEtaEtaEle[eleIndex])) return false; 
  _eleCounter.IncrVar("Probe_covEtaEta");
  H_probeCovEtaEtaOkEta->Fill(etaEle[eleIndex]);
  H_probeCovEtaEtaOkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_eOverPout")     && !selection->passCut("Probe_eOverPout",eSeedOverPoutEle[eleIndex]))  return false; 
  _eleCounter.IncrVar("Probe_eOverPout");
  H_probeEoPoutOkEta->Fill(etaEle[eleIndex]);
  H_probeEoPoutOkPt->Fill(etEle[eleIndex]);

  if(selection->getSwitch("Probe_eOverPin")      && !selection->passCut("Probe_eOverPin",eSuperClusterOverPEle[eleIndex]))      return false; 
  _eleCounter.IncrVar("Probe_eOverPin");

  if(selection->getSwitch("Probe_Fisher")        && !selection->passCut("Probe_Fisher",Fisher(eleIndex)))               return false; 
  _eleCounter.IncrVar("Probe_Fisher");


  if(selection->getSwitch("Probe_Likelihood")    && !selection->passCut("Probe_Likelihood",eleIdLikelihoodEle[eleIndex])) return false; 
  _eleCounter.IncrVar("Probe_Likelihood");

  _eleCounter.IncrVar("Probe_finalEleID");

  return true;
}


bool ZTandProbe::isTagEleID(int eleIndex) {

  int GsfClass = classificationEle[eleIndex];
  int offset=0;
  if(GsfClass>=100) { GsfClass-=100; offset=4; }

  Selection *selection;
  if(GsfClass==0) 
    selection=_electronSelection[offset];
  else if(GsfClass==10)
    selection=_electronSelection[offset+1];
  else if(GsfClass==20)
    selection=_electronSelection[offset+2];
  else if(GsfClass>=30 && GsfClass<=40) {
    selection=_electronSelection[offset+3];
  }

  TVector3 pTrkAtInner(pxAtInnerEle[eleIndex],pyAtInnerEle[eleIndex],pzAtInnerEle[eleIndex]);
  TVector3 pTrkAtOuter(pxAtOuterEle[eleIndex],pyAtOuterEle[eleIndex],pzAtOuterEle[eleIndex]);

  _eleCounter.IncrVar("Tag_electrons");

  if(selection->getSwitch("Tag_hOverE")        && !selection->passCut("Tag_hOverE",hOverEEle[eleIndex]))             return false;  
  _eleCounter.IncrVar("Tag_hOverE");
  if(selection->getSwitch("Tag_s9s25")         && !selection->passCut("Tag_s9s25",s9s25Ele[eleIndex]))               return false; 
  _eleCounter.IncrVar("Tag_s9s25");
  if(selection->getSwitch("Tag_deta")          && !selection->passCut("Tag_deta",deltaEtaAtVtxEle[eleIndex]))     return false; 
  _eleCounter.IncrVar("Tag_deta");
  if(selection->getSwitch("Tag_dphiIn")        && !selection->passCut("Tag_dphiIn",deltaPhiAtVtxEle[eleIndex]))   return false; 
  _eleCounter.IncrVar("Tag_dphiIn");
  if(selection->getSwitch("Tag_dphiOut")       && !selection->passCut("Tag_dphiOut",deltaPhiAtCaloEle[eleIndex])) return false; 
  _eleCounter.IncrVar("Tag_dphiOut");
  if(selection->getSwitch("Tag_invEMinusInvP") && !selection->passCut("Tag_invEMinusInvP",1./ecalEle[eleIndex]-1./pTrkAtInner.Mag()))          return false; 
  _eleCounter.IncrVar("Tag_invEMinusInvP");
  if(selection->getSwitch("Tag_bremFraction")  && !selection->passCut("Tag_bremFraction",fabs(pTrkAtInner.Mag()-pTrkAtOuter.Mag())/pTrkAtInner.Mag())) return false; 
  _eleCounter.IncrVar("Tag_bremFraction");
  if(selection->getSwitch("Tag_covEtaEta")     && !selection->passCut("Tag_covEtaEta",covEtaEtaEle[eleIndex]))      return false; 
  _eleCounter.IncrVar("Tag_covEtaEta");
  if(selection->getSwitch("Tag_eOverPout")     && !selection->passCut("Tag_eOverPout",eSeedOverPoutEle[eleIndex]))  return false; 
  _eleCounter.IncrVar("Tag_eOverPout");
  if(selection->getSwitch("Tag_eOverPin")      && !selection->passCut("Tag_eOverPin",eSuperClusterOverPEle[eleIndex]))      return false; 
  _eleCounter.IncrVar("Tag_eOverPin");
  if(selection->getSwitch("Tag_Fisher")        && !selection->passCut("Tag_Fisher",Fisher(eleIndex)))               return false; 
  _eleCounter.IncrVar("Tag_Fisher");
  if(selection->getSwitch("Tag_Likelihood")    && !selection->passCut("Tag_Likelihood",eleIdLikelihoodEle[eleIndex])) return false; 
  _eleCounter.IncrVar("Tag_Likelihood");

  _eleCounter.IncrVar("Tag_finalEleID");

  return true;
}

void ZTandProbe::setKinematics(int theTag, int theProbe) {
  // three-vectors
  _p3Tag  ->SetXYZ(pxEle[theTag],pyEle[theTag],pzEle[theTag]);
  _p3Probe->SetXYZ(pxEle[theProbe],pyEle[theProbe],pzEle[theProbe]);

  // four-vectors
  _p4Tag  ->SetXYZT(pxEle[theTag],pyEle[theTag],pzEle[theTag],energyEle[theTag]);
  _p4Probe->SetXYZT(pxEle[theProbe],pyEle[theProbe],pzEle[theProbe],energyEle[theProbe]);
}

float ZTandProbe::Fisher(int eleIndex) {
  float fisher;
  if(classificationEle[eleIndex]<100)
    fisher = 42.0238-3.38943*s9s25Ele[eleIndex]-794.092*sqrt(covEtaEtaEle[eleIndex])-15.3449*latEle[eleIndex]-31.1032*a20Ele[eleIndex];
  else
    fisher = 27.2967+2.97453*s9s25Ele[eleIndex]-169.219*sqrt(covEtaEtaEle[eleIndex])-17.0445*latEle[eleIndex]-24.8542*a20Ele[eleIndex];
  return fisher;
}

void ZTandProbe::bookHistos() {

  // Efficiencies
  Gene_eta      = new TH1F("Gene_eta",      "generated #eta",             40, -2.5,2.5);
  H_tagEta      = new TH1F("H_tagEta",      "tag electron #eta",          40, -2.5,2.5);
  H_probeAllEta = new TH1F("H_probeAllEta", "probe (all) electron #eta",  40, -2.5,2.5);
  H_probeOkEta  = new TH1F("H_probeOkEta",  "probe (ok)  electron #eta",  40, -2.5,2.5);
  Gene_eta      -> GetXaxis()->SetTitle("electron #eta");
  H_tagEta      -> GetXaxis()->SetTitle("electron #eta");
  H_probeAllEta -> GetXaxis()->SetTitle("electron #eta");
  H_probeOkEta  -> GetXaxis()->SetTitle("electron #eta");

  Gene_pt       = new TH1F("Gene_pt",      "generated p_{T}",             40, 15., 100.);
  H_tagPt       = new TH1F("H_tagPt",      "tag electron p_{T}",          40, 15., 100.);
  H_probeAllPt  = new TH1F("H_probeAllPt", "probe (all) electron p_{T}",  40, 15., 100.);
  H_probeOkPt   = new TH1F("H_probeOkPt",  "probe (ok)  electron p_{T}",  40, 15., 100.);
  Gene_pt       -> GetXaxis()->SetTitle("electron p_{T}");
  H_tagPt       -> GetXaxis()->SetTitle("electron p_{T}");
  H_probeAllPt  -> GetXaxis()->SetTitle("electron p_{T}");
  H_probeOkPt   -> GetXaxis()->SetTitle("electron p_{T}");

  // step by step efficiency - using generated electrons
  Htrigger_eta = new TH1F("Htrigger_eta", "#eta - after trigger",             40, -2.5,2.5);
  Hmc_eta      = new TH1F("Hmc_eta",      "#eta - after mc info selection",   40, -2.5,2.5);
  Hiso_eta     = new TH1F("Hiso_eta",     "#eta - after isolation",           40, -2.5,2.5);
  HeleID_eta   = new TH1F("HeleID_eta",   "#eta - after eleID",               40, -2.5,2.5);
  Hpt_eta      = new TH1F("Hpt_eta",      "#eta - after pt cut",              40, -2.5,2.5);

  Htrigger_pt = new TH1F("Htrigger_pt", "p_{T} - after trigger",             40, 15., 100.);
  Hmc_pt      = new TH1F("Hmc_pt",      "p_{T} - after mc info selection",   40, 15., 100.);
  Hiso_pt     = new TH1F("Hiso_pt",     "p_{T} - after isolation",           40, 15., 100.);
  HeleID_pt   = new TH1F("HeleID_pt",   "p_{T} - after eleID",               40, 15., 100.);
  Hpt_pt      = new TH1F("Hpt_pt",      "p_{T} - after pt cut",              40, 15., 100.);

  // step by step electron ID efficiency
  H_probeGoldenEta      = new TH1F("H_probeGoldenEta",      "H_probeGoldenEta",      40, -2.5,2.5);
  H_probeBigBremEta     = new TH1F("H_probeBigBremEta",     "H_probeBigBremEta",     40, -2.5,2.5);
  H_probeNarrowEta      = new TH1F("H_probeNarrowEta",      "H_probeNarrowEta",      40, -2.5,2.5);
  H_probeShoweringEta   = new TH1F("H_probeShoweringEta",   "H_probeShoweringEta",   40, -2.5,2.5);
  H_probeCracksEta      = new TH1F("H_probeCracksEta",      "H_probeCracksEta",      40, -2.5,2.5);
  H_probeEoPoutOkEta    = new TH1F("H_probeEoPoutOkEta",    "H_probeEoPoutOkEta",    40, -2.5,2.5);
  H_probeHoEOkEta       = new TH1F("H_probeHoEOkEta",       "H_probeHoEOkEta",       40, -2.5,2.5);
  H_probedPhiOkEta      = new TH1F("H_probedPhiOkEta",      "H_probedPhiOkEta",      40, -2.5,2.5);
  H_probedEtaOkEta      = new TH1F("H_probedEtaOkEta",      "H_probedEtaOkEta",      40, -2.5,2.5);
  H_probeCovEtaEtaOkEta = new TH1F("H_probeCovEtaEtaOkEta", "H_probeCovEtaEtaOkEta", 40, -2.5,2.5);
  H_probeS9S25OkEta     = new TH1F("H_probeS9S25OkEta",     "H_probeS9S25OkEta",     40, -2.5,2.5);

  H_probeGoldenPt      = new TH1F("H_probeGoldenPt",      "H_probeGoldenPt",      40, 15., 100.);
  H_probeBigBremPt     = new TH1F("H_probeBigBremPt",     "H_probeBigBremPt",     40, 15., 100.);
  H_probeNarrowPt      = new TH1F("H_probeNarrowPt",      "H_probeNarrowPt",      40, 15., 100.);
  H_probeShoweringPt   = new TH1F("H_probeShoweringPt",   "H_probeShoweringPt",   40, 15., 100.);
  H_probeCracksPt      = new TH1F("H_probeCracksPt",      "H_probeCracksPt",      40, 15., 100.);
  H_probeEoPoutOkPt    = new TH1F("H_probeEoPoutOkPt",    "H_probeEoPoutOkPt",    40, 15., 100.);
  H_probeHoEOkPt       = new TH1F("H_probeHoEOkPt",       "H_probeHoEOkPt",       40, 15., 100.);
  H_probedPhiOkPt      = new TH1F("H_probedPhiOkPt",      "H_probedPhiOkPt",      40, 15., 100.);
  H_probedEtaOkPt      = new TH1F("H_probedEtaOkPt",      "H_probedEtaOkPt",      40, 15., 100.);
  H_probeCovEtaEtaOkPt = new TH1F("H_probeCovEtaEtaOkPt", "H_probeCovEtaEtaOkPt", 40, 15., 100.);
  H_probeS9S25OkPt     = new TH1F("H_probeS9S25OkPt",     "H_probeS9S25OkPt",     40, 15., 100.);

  // pdfs
  char title[200];
  for (int ii=0; ii<2; ii++){ 

    sprintf (title, "dPhiVtx0[%d]",ii);
    dPhiVtx0[ii]     = new TH1F(title, "#Delta #phi @ vtx",    100,-0.1,0.1);
    sprintf (title, "dEtaCalo0[%d]",ii);
    dEtaCalo0[ii]    = new TH1F(title, "#Delta #eta @ Calo",   100,-0.02,0.02); 
    sprintf (title, "dEtaVtx0[%d]",ii);
    dEtaVtx0[ii]    = new TH1F(title, "#Delta #eta @ Vtx",   100,-0.02,0.02); 
    sprintf (title, "EoPout0[%d]",ii);
    EoPout0[ii]      = new TH1F(title, "EoPout",               100, 0.0, 15.0);  
    sprintf (title, "HoE0[%d]",ii);
    HoE0[ii]         = new TH1F(title, "HoE",                   50,-0.02, 0.1);  
    sprintf (title, "shapeFisher0[%d]",ii);
    shapeFisher0[ii] = new TH1F(title, "shapeFisher",          100,-15., 15.);

    sprintf (title, "dPhiVtx1[%d]",ii);
    dPhiVtx1[ii]     = new TH1F(title, "#Delta #phi @ vtx",    100,-0.1,0.1);
    sprintf (title, "dEtaCalo1[%d]",ii);
    dEtaCalo1[ii]    = new TH1F(title, "#Delta #eta @ Calo",   100,-0.02,0.02); 
    sprintf (title, "dEtaVtx1[%d]",ii);
    dEtaVtx1[ii]    = new TH1F(title, "#Delta #eta @ Vtx",   100,-0.02,0.02); 
    sprintf (title, "EoPout1[%d]",ii);
    EoPout1[ii]      = new TH1F(title, "EoPout",               100, 0.0, 15.0);  
    sprintf (title, "HoE1[%d]",ii);
    HoE1[ii]         = new TH1F(title, "HoE",                   50,-0.02, 0.1);  
    sprintf (title, "shapeFisher1[%d]",ii);
    shapeFisher1[ii] = new TH1F(title, "shapeFisher",          100,-15., 15.);

    sprintf (title, "dPhiVtx2[%d]",ii);
    dPhiVtx2[ii]     = new TH1F(title, "#Delta #phi @ vtx",    100,-0.1,0.1);
    sprintf (title, "dEtaCalo2[%d]",ii);
    dEtaCalo2[ii]    = new TH1F(title, "#Delta #eta @ Calo",   100,-0.02,0.02); 
    sprintf (title, "dEtaVtx2[%d]",ii);
    dEtaVtx2[ii]    = new TH1F(title, "#Delta #eta @ Vtx",   100,-0.02,0.02); 
    sprintf (title, "EoPout2[%d]",ii);
    EoPout2[ii]      = new TH1F(title, "EoPout",               100, 0.0, 15.0);  
    sprintf (title, "HoE2[%d]",ii);
    HoE2[ii]         = new TH1F(title, "HoE",                   50,-0.02, 0.1);  
    sprintf (title, "shapeFisher2[%d]",ii);
    shapeFisher2[ii] = new TH1F(title, "shapeFisher",          100,-15., 15.);

    sprintf (title, "dPhiVtx3[%d]",ii);
    dPhiVtx3[ii]     = new TH1F(title, "#Delta #phi @ vtx",    100,-0.1,0.1);
    sprintf (title, "dEtaCalo3[%d]",ii);
    dEtaCalo3[ii]    = new TH1F(title, "#Delta #eta @ Calo",   100,-0.02,0.02); 
    sprintf (title, "dEtaVtx3[%d]",ii);
    dEtaVtx3[ii]    = new TH1F(title, "#Delta #eta @ Calo",   100,-0.02,0.02); 
    sprintf (title, "EoPout3[%d]",ii);
    EoPout3[ii]      = new TH1F(title, "EoPout",               100, 0.0, 15.0);  
    sprintf (title, "HoE3[%d]",ii);
    HoE3[ii]         = new TH1F(title, "HoE",                   50,-0.02, 0.1);  
    sprintf (title, "shapeFisher3[%d]",ii);
    shapeFisher3[ii] = new TH1F(title, "shapeFisher",          100,-15., 15.);
  }
 
}
