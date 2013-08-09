#include "HiggsAnalysisTools/include/CommonHiggsPreselector.hh"
#include <iostream>
#include <stdio.h>
#include <math.h>

CommonHiggsPreselector::CommonHiggsPreselector() {
  m_processID = -1;
}

CommonHiggsPreselector::~CommonHiggsPreselector() {}

void CommonHiggsPreselector::Configure(const char *fileCuts, const char *fileSwitches) {
  
  _selection = new Selection(std::string(fileCuts),std::string(fileSwitches));
  
  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addSwitch("preselection");
  _selection->addSwitch("leptonAcceptance");
  _selection->addCut("nRecoLeptons");
  _selection->addCut("leptonD0");
  _selection->addCut("muGlobalIso");
  _selection->addCut("hardLeptonThreshold");
  _selection->addCut("slowLeptonThreshold");
  _selection->addCut("METpreselection");
  _selection->addCut("dileptonInvMassMin");

  presCounter = new Counters();
  presCounter->SetTitle("PRESELECTION EVENT COUNTER");
  presCounter->AddVar("event");
  presCounter->AddVar("MCtruth");
  presCounter->AddVar("trigger");
  presCounter->AddVar("nRecoLeptons");
  presCounter->AddVar("twoGoodRec");
  presCounter->AddVar("hardLeptonThreshold");
  presCounter->AddVar("slowLeptonThreshold");
  presCounter->AddVar("METpreselection");
  presCounter->AddVar("dileptonInvMassMin");
  presCounter->AddVar("finalOURPreselection");
}



bool CommonHiggsPreselector::output() {
  
  Counters *theCounter=0;
  
  if( m_processID > -1 ) {
    
    std::map<int, Counters*>::const_iterator iter = multiProcessCounter.find(m_processID);

    if ( iter == multiProcessCounter.end() ) {
      
      std::cout << "First time I get process " << m_processID << ": adding a counter" << std::endl;

      char buffer[200];
      sprintf(buffer,"PRESELECTION EVENT COUNTER for process %d", m_processID);

      Counters *processCounter = new Counters();
      processCounter->SetTitle(buffer);
      processCounter->AddVar("event");
      processCounter->AddVar("MCtruth");
      processCounter->AddVar("trigger");
      processCounter->AddVar("nRecoLeptons");
      processCounter->AddVar("twoGoodRec");
      processCounter->AddVar("hardLeptonThreshold");
      processCounter->AddVar("slowLeptonThreshold");
      processCounter->AddVar("METpreselection");
      processCounter->AddVar("dileptonInvMassMin");
      processCounter->AddVar("finalOURPreselection");
      
      multiProcessCounter.insert( std::make_pair(m_processID,processCounter) );
    }

    theCounter = multiProcessCounter[m_processID];
  }
  
  else theCounter = presCounter;

  float m_weight = m_kFactor;
  theCounter->IncrVar("event",m_weight);


  bool mctruth = true;
  if (_selection->getSwitch("MCtruth")) mctruth = m_foundMcTree;

  // MC truth
  if(_selection->getSwitch("MCtruth") && !m_foundMcTree ) return false;
  theCounter->IncrVar("MCtruth",m_weight);
  
  // HLT
  if(_selection->getSwitch("trigger") && !m_passedHLT ) return false;
  theCounter->IncrVar("trigger",m_weight); 
  
  // nEle, nMuon
  int nLeptons = m_nEle + m_nMuon;
  if ( _selection->getSwitch("nRecoLeptons") && ( !_selection->passCut("nRecoLeptons", nLeptons) ) ) return false;
  theCounter->IncrVar("nRecoLeptons",m_weight);
  
  // leptons in the acceptance
  if( _selection->getSwitch("leptonAcceptance") && !m_isEE && !m_isEM && !m_isMM && !m_isME ) return false;
  theCounter->IncrVar("twoGoodRec",m_weight);
  
  // high pt lepton
  if ( _selection->getSwitch("hardLeptonThreshold") &&
       (!_selection->passCut("hardLeptonThreshold", m_highElePt) && 
	!_selection->passCut("hardLeptonThreshold", m_highMuonPt) )
       ) return false;
  theCounter->IncrVar("hardLeptonThreshold",m_weight);
  
  // low pt lepton
  if ( _selection->getSwitch("slowLeptonThreshold") &&
       (!_selection->passCut("slowLeptonThreshold", m_lowElePt) && 
	!_selection->passCut("slowLeptonThreshold", m_lowMuonPt) )
       ) return false;
  theCounter->IncrVar("slowLeptonThreshold",m_weight);

  // met cut
  if ( _selection->getSwitch("METpreselection") && 
       !_selection->passCut("METpreselection", m_met) ) return false;
  theCounter->IncrVar("METpreselection",m_weight);
  
  // m(ll) cut
  if ( _selection->getSwitch("dileptonInvMassMin")) {
    if ( ! ( (m_isEE && _selection->passCut("dileptonInvMassMin", m_mllEE)) ||
	     (m_isMM && _selection->passCut("dileptonInvMassMin", m_mllMM)) ||
	     (m_isEM && _selection->passCut("dileptonInvMassMin", m_mllEM)) ||
	     (m_isME && _selection->passCut("dileptonInvMassMin", m_mllME)) ) 
	 ) return false;
  }
  theCounter->IncrVar("dileptonInvMassMin",m_weight);

  theCounter->IncrVar("finalOURPreselection",m_weight);
    
  return true;
}


void CommonHiggsPreselector::displayEfficiencies(std::string datasetName) {

  if( m_processID > -1 ) {
    
    std::map<int, Counters*>::const_iterator iter;
    for( iter=multiProcessCounter.begin(); iter!=multiProcessCounter.end(); ++iter ) {
      
      Counters *theCounter = iter->second;
      theCounter->Draw();
      theCounter->Draw("MCtruth","event");
      theCounter->Draw("trigger","MCtruth");
      theCounter->Draw("nRecoLeptons","trigger");
      theCounter->Draw("twoGoodRec","nRecoLeptons");
      theCounter->Draw("hardLeptonThreshold","twoGoodRec");
      theCounter->Draw("slowLeptonThreshold","hardLeptonThreshold");
      theCounter->Draw("METpreselection","slowLeptonThreshold");
      theCounter->Draw("dileptonInvMassMin","METpreselection");
      theCounter->Draw("finalOURPreselection","MCtruth");
    }
  }

  else {
    
    char namefile[500];
    sprintf(namefile,"%s-Counters.root",datasetName.c_str());

    presCounter->Draw();
    presCounter->Draw("MCtruth","event");
    presCounter->Draw("trigger","MCtruth");
    presCounter->Draw("nRecoLeptons","trigger");
    presCounter->Draw("twoGoodRec","nRecoLeptons");
    presCounter->Draw("hardLeptonThreshold","twoGoodRec");
    presCounter->Draw("slowLeptonThreshold","hardLeptonThreshold");
    presCounter->Draw("METpreselection","slowLeptonThreshold");
    presCounter->Draw("dileptonInvMassMin","METpreselection");
    presCounter->Draw("finalOURPreselection","MCtruth");
  
    presCounter->Save(namefile,"recreate");
  }
}
