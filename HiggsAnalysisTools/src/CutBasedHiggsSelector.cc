#include "HiggsAnalysisTools/include/CutBasedHiggsSelector.hh"
#include <iostream>
#include <stdio.h>
#include <math.h>

CutBasedHiggsSelector::CutBasedHiggsSelector() {

  m_finalLeptons = false;
  m_jetVeto = false;
  m_preDeltaPhi = false;
  m_extraSlowLeptonPTMin = 0.;

  // latinos
  m_step0 = false;
  m_step1 = false;
  m_step2 = false;
  m_step3 = false;
  m_step4 = false;
  m_step5 = false;
  m_step6 = false;
  m_step7 = false;
  m_step8 = false;
  m_step9 = false;
  m_step10 = false;
  m_step11 = false;
  m_step12 = false;
  m_step13 = false;
  m_step14 = false;
  m_step15 = false;
  m_step16 = false;
  m_step17 = false;
  m_step18 = false;
  m_step19 = false;
  m_step20 = false;
  m_step21 = false;
  m_step22 = false;
  m_step23 = false;
  m_step24 = false;
  m_step25 = false;
  m_step26 = false;

  m_processID = -1;
}

CutBasedHiggsSelector::CutBasedHiggsSelector( const CutBasedHiggsSelector& selector ) {

  m_weight = selector.m_weight;
  m_foundMcTree = selector.m_foundMcTree;
  m_passedHLT = selector.m_passedHLT;
  m_isThisChannel = selector.m_isThisChannel;
  m_highPt = selector.m_highPt;
  m_isElectronId = selector.m_isElectronId;
  m_isPositronId = selector.m_isPositronId;
  m_isElectronIsol = selector.m_isElectronIsol;
  m_isPositronIsol = selector.m_isPositronIsol;
  m_isElectronConvRej = selector.m_isElectronConvRej;
  m_isPositronConvRej = selector.m_isPositronConvRej;
  m_isElectronIp = selector.m_isElectronIp;
  m_isPositronIp = selector.m_isPositronIp;
  m_invMass = selector.m_invMass;
  m_nJets            = selector.m_nJets;
  m_nUncorrJets      = selector.m_nUncorrJets;
  m_btagJets         = selector.m_btagJets;
  m_nSoftMuons       = selector.m_nSoftMuons;
  m_nExtraLeptons    = selector.m_nExtraLeptons;
  m_met = selector.m_met;
  m_projectedMet = selector.m_projectedMet;
  m_metOverPtLL = selector.m_metOverPtLL;
  m_dymva = selector.m_dymva;
  m_deltaPhiLLJet = selector.m_deltaPhiLLJet;
  m_deltaPhi = selector.m_deltaPhi;
  m_detaLeptons = selector.m_detaLeptons;
  m_maxPtElectron = selector.m_maxPtElectron;
  m_minPtElectron = selector.m_minPtElectron;
  m_ptll = selector.m_ptll;
  m_WWInvMass = selector.m_WWInvMass;
  m_nVtx = selector.m_nVtx;
  m_extraSlowLeptonPTMin = selector.m_extraSlowLeptonPTMin;
  m_processID = selector.m_processID;
  *_selection = *selector._selection;
  *globalCounter = *selector.globalCounter;
  m_finalLeptons = selector.m_finalLeptons;
  m_jetVeto = selector.m_jetVeto;
  m_preDeltaPhi = selector.m_preDeltaPhi;
  multiProcessCounter = selector.multiProcessCounter;

  // latinos
  m_step0  = selector.m_step0;
  m_step1  = selector.m_step1;
  m_step2  = selector.m_step2;
  m_step3  = selector.m_step3;
  m_step4  = selector.m_step4;
  m_step5  = selector.m_step5;
  m_step6  = selector.m_step6;
  m_step7  = selector.m_step7;
  m_step8  = selector.m_step8;
  m_step9  = selector.m_step9;
  m_step10 = selector.m_step10;
  m_step11 = selector.m_step11;
  m_step12 = selector.m_step12;
  m_step13 = selector.m_step13;
  m_step14 = selector.m_step14;
  m_step15 = selector.m_step15;
  m_step16 = selector.m_step16;
  m_step17 = selector.m_step17;
  m_step18 = selector.m_step18;
  m_step19 = selector.m_step19;
  m_step20 = selector.m_step20;
  m_step21 = selector.m_step21;
  m_step22 = selector.m_step22;
  m_step23 = selector.m_step23;
  m_step24 = selector.m_step24;
  m_step25 = selector.m_step25;
  m_step26 = selector.m_step26;
}

CutBasedHiggsSelector::~CutBasedHiggsSelector() {}

void CutBasedHiggsSelector::Configure(const char *fileCuts, const char* fileSwitches, const char *theTitle) {

  _selection = new Selection(std::string(fileCuts),std::string(fileSwitches));

  // tehse cuts are applied in the HiggsSelection class, but are configured here
  _selection->addSwitch("MCtruth");
  _selection->addSwitch("trigger");
  _selection->addSwitch("leptonId");
  _selection->addSwitch("leptonIso");
  _selection->addSwitch("leptonD0"); 
  _selection->addSwitch("convRej");
  _selection->addCut("muGlobalIso");
  _selection->addCut("electronIP");
  _selection->addCut("electronDz");
  _selection->addCut("muonIPhighPT");
  _selection->addCut("muonIPlowPT");
  _selection->addCut("muonDz");
  _selection->addCut("nExtraLeptons");
  _selection->addCut("looseMET");
  _selection->addCut("mll");
  _selection->addCut("mllZPeak");
  _selection->addCut("tightMET");
  _selection->addCut("projectedMET");
  _selection->addCut("metOverPtLL");
  _selection->addCut("etaJetAcc");
  _selection->addCut("etJetAcc");
  _selection->addCut("jetConeWidth");
  _selection->addCut("nSoftMuons");
  _selection->addCut("bTagVeto");
  _selection->addCut("ptll");
  _selection->addCut("minPtLeptonSameFlav");
  _selection->addCut("dymva0j");
  _selection->addCut("dymva1j");
  _selection->addCut("mll2");
  _selection->addCut("maxPtLepton");
  _selection->addCut("minPtLepton");
  _selection->addCut("deltaPhiLLJet");
  _selection->addCut("deltaPhi");
  _selection->addCut("higgsMassRel0j");
  _selection->addCut("higgsMassRel1j");

  _selection->summary();

  globalCounter = new Counters();
  globalCounter->SetTitle(theTitle);
  globalCounter->AddVar("event"); 
  globalCounter->AddVar("MCtruth"); 
  globalCounter->AddVar("trigger"); // 0
  globalCounter->AddVar("preselected"); // 1
  globalCounter->AddVar("leptonId"); // 2
  globalCounter->AddVar("leptonIso"); // 3
  globalCounter->AddVar("convRej"); // 4
  globalCounter->AddVar("leptonD0"); // 5   
  globalCounter->AddVar("looseMET"); // 6
  globalCounter->AddVar("mll"); // 7
  globalCounter->AddVar("mllZPeak"); // 8
  globalCounter->AddVar("mpmet"); // 9
  //  globalCounter->AddVar("metOverPtLL");
  globalCounter->AddVar("deltaPhiLLJet"); // 10
  globalCounter->AddVar("nSoftMuons"); // 11
  globalCounter->AddVar("nExtraLeptons"); // 12
  globalCounter->AddVar("bTagVeto"); // 13
  globalCounter->AddVar("ptll"); // 14
  globalCounter->AddVar("minPtLeptonSameFlav"); 
  globalCounter->AddVar("dymva"); // 15
  globalCounter->AddVar("zeroJets"); // 16
  globalCounter->AddVar("maxPtLepton"); // 17
  globalCounter->AddVar("minPtLepton"); // 18
  globalCounter->AddVar("mll2"); // 19
  globalCounter->AddVar("higgsMassRel0j"); // 20
  globalCounter->AddVar("deltaPhi"); // 21
  globalCounter->AddVar("final"); // 22
  globalCounter->AddVar("oneJet"); // 23
  globalCounter->AddVar("gt1Jets"); // 24
  globalCounter->AddVar("WW1jet"); // 25 -> WW level for 1jet bin
}

bool CutBasedHiggsSelector::output() {

  Counters *theCounter=0;

  if( m_processID > -1 ) {

    std::map<int, Counters*>::const_iterator iter = multiProcessCounter.find(m_processID);

    if ( iter == multiProcessCounter.end() ) {
      
      std::cout << "First time I get process " << m_processID 
		<< ": adding a counter" << std::endl;

      char buffer[200];
      sprintf(buffer,"Event counter for process %d", m_processID);
      
      Counters *processCounter = new Counters();
      processCounter->SetTitle(buffer);
      processCounter->AddVar("event");
      processCounter->AddVar("MCtruth");
      processCounter->AddVar("trigger");
      processCounter->AddVar("preselected");
      processCounter->AddVar("leptonId");
      processCounter->AddVar("leptonIso");      
      processCounter->AddVar("convRej");
      processCounter->AddVar("leptonD0");
      processCounter->AddVar("looseMET");
      processCounter->AddVar("mll");
      processCounter->AddVar("mllZPeak");
      processCounter->AddVar("mpmet");
      //      processCounter->AddVar("metOverPtLL");
      processCounter->AddVar("zeroJets");
      processCounter->AddVar("deltaPhiLLJet");
      processCounter->AddVar("nSoftMuons");
      processCounter->AddVar("nExtraLeptons");
      processCounter->AddVar("bTagVeto");
      processCounter->AddVar("ptll");
      processCounter->AddVar("minPtLeptonSameFlav");
      processCounter->AddVar("dymva");
      processCounter->AddVar("maxPtLepton");
      processCounter->AddVar("minPtLepton");
      processCounter->AddVar("mll2");
      processCounter->AddVar("higgsMassRel0j");
      processCounter->AddVar("deltaPhi");
      processCounter->AddVar("final");
      processCounter->AddVar("oneJet");
      processCounter->AddVar("gt1Jets");
      processCounter->AddVar("WW1jet"); // 23 -> WW level for 1jet bin
      multiProcessCounter.insert( std::make_pair(m_processID,processCounter) );      
    }

    theCounter = multiProcessCounter[m_processID];

  }
  
  else theCounter = globalCounter;

  m_finalLeptons = false;
  m_jetVeto = false;
  m_preDeltaPhi = false;
  
  // latinos
  m_step0 = false;
  m_step1 = false;
  m_step2 = false;
  m_step3 = false;
  m_step4 = false;
  m_step5 = false;
  m_step6 = false;
  m_step7 = false;
  m_step8 = false;
  m_step9 = false;
  m_step10 = false;
  m_step11 = false;
  m_step12 = false;
  m_step13 = false;
  m_step14 = false;
  m_step15 = false;
  m_step16 = false;
  m_step17 = false;
  m_step18 = false;
  m_step19 = false;
  m_step20 = false;
  m_step21 = false;
  m_step22 = false;
  m_step23 = false;
  m_step24 = false;
  m_step25 = false;
  m_step26 = false;

  theCounter->IncrVar("event",m_weight);
  
  if (_selection->getSwitch("MCtruth") && !m_foundMcTree) return false;
  theCounter->IncrVar("MCtruth",m_weight);

  if(_selection->getSwitch("trigger") && !m_passedHLT ) return false;
  theCounter->IncrVar("trigger",m_weight); 
  m_step0 = true;

  if(!m_isThisChannel) return false;
  theCounter->IncrVar("preselected",m_weight);
  m_step1 = true;

  if (_selection->getSwitch("leptonId") && (m_isElectronId<0 || m_isPositronId<0) ) return false; 
  theCounter->IncrVar("leptonId",m_weight);
  m_step2 = true;

  if (_selection->getSwitch("leptonIso") && (m_isElectronIsol<0 || m_isPositronIsol<0)) return false; 
  theCounter->IncrVar("leptonIso",m_weight);
  m_step3 = true;

  if (_selection->getSwitch("convRej") && (m_isElectronConvRej<0 || m_isPositronConvRej<0) ) return false; 
  theCounter->IncrVar("convRej",m_weight);
  m_step4 = true;

  if (_selection->getSwitch("leptonD0") && (m_isElectronIp<0 || m_isPositronIp<0) ) return false; 
  theCounter->IncrVar("leptonD0",m_weight);
  m_step5 = true;
  
  m_finalLeptons = true;

  if (_selection->getSwitch("looseMET") && !_selection->passCut("looseMET",m_met)) return false; 
  theCounter->IncrVar("looseMET",m_weight);
  m_step6 = true;

  if (_selection->getSwitch("mll") && !_selection->passCut("mll", m_invMass)) return false;
  theCounter->IncrVar("mll",m_weight);
  m_step7 = true;  

  if (_selection->getSwitch("mllZPeak") && !_selection->passCut("mllZPeak", fabs(m_invMass-91.1876))) return false;
  theCounter->IncrVar("mllZPeak",m_weight);
  m_step8 = true;

  if (_selection->getSwitch("projectedMET") && !_selection->passCut("projectedMET",m_projectedMet)) return false;
  theCounter->IncrVar("mpmet",m_weight);
  m_step9 = true;

  //   if (_selection->getSwitch("metOverPtLL") && !_selection->passCut("metOverPtLL",m_metOverPtLL)) return false;
  //   theCounter->IncrVar("metOverPtLL",m_weight);
  //   m_step9 = true;

  if(!_selection->getSwitch("deltaPhiLLJet") || m_nJets < 2 || 
     (_selection->getSwitch("deltaPhiLLJet") && m_nJets >=2 && _selection->passCut("deltaPhiLLJet",m_deltaPhiLLJet))) {
    theCounter->IncrVar("deltaPhiLLJet",m_weight);
    m_step10 = true;
      
    if(!_selection->getSwitch("nSoftMuons") ||
       (_selection->getSwitch("nSoftMuons") && _selection->passCut("nSoftMuons",m_nSoftMuons))) {
      theCounter->IncrVar("nSoftMuons",m_weight);
      m_step11 = true;
        
      if (!_selection->getSwitch("nExtraLeptons") || 
          (_selection->getSwitch("nExtraLeptons") && _selection->passCut("nExtraLeptons",m_nExtraLeptons))) {
        theCounter->IncrVar("nExtraLeptons",m_weight);
        m_step12 = true;
          
        if(!_selection->getSwitch("bTagVeto") ||
           (_selection->getSwitch("bTagVeto") && _selection->passCut("bTagVeto",m_btagJets))) {
          theCounter->IncrVar("bTagVeto",m_weight);
          m_step13 = true;
            
          if(!_selection->getSwitch("ptll") ||
             (_selection->getSwitch("ptll") && _selection->passCut("ptll",m_ptll))) {
            theCounter->IncrVar("ptll",m_weight);
            m_step14 = true;

            if(!_selection->getSwitch("minPtLeptonSameFlav") ||
               (_selection->getSwitch("minPtLeptonSameFlav") && _selection->passCut("minPtLeptonSameFlav",m_lowPt))) {
              theCounter->IncrVar("minPtLeptonSameFlav",m_weight);
              // minPtLeptonSameFlav is no more used after introducing the dy mva
              
              bool passFinalMET = true;
              if(m_nJets==0) passFinalMET = _selection->passCut("dymva0j",m_dymva);
              else if(m_nJets==1) passFinalMET = _selection->passCut("dymva1j",m_dymva);
              else passFinalMET = _selection->passCut("tightMET",m_met-(float(m_nVtx))/2.0);

              if(!_selection->getSwitch("dymva0j") || !_selection->getSwitch("dymva1j") || 
                 (_selection->getSwitch("dymva0j") && _selection->getSwitch("dymva1j") && passFinalMET)) {
                theCounter->IncrVar("dymva",m_weight);
                m_step15 = true;
                
                if(m_nJets==0) {
                  theCounter->IncrVar("zeroJets",m_weight);
                  m_jetVeto = true;
                  m_step16 = true;
                  
                  if (!_selection->getSwitch("maxPtLepton") || 
                      (_selection->getSwitch("maxPtLepton") && _selection->passCut("maxPtLepton", m_highPt))) {
                    theCounter->IncrVar("maxPtLepton",m_weight);
                    m_step17 = true;
                    
                    if (!_selection->getSwitch("minPtLepton") || 
                        (_selection->getSwitch("minPtLepton") && _selection->passCut("minPtLepton", m_lowPt))
                        && m_lowPt >= m_extraSlowLeptonPTMin ) {
                      theCounter->IncrVar("minPtLepton",m_weight);
                      m_step18 = true;
                      
                      if (!_selection->getSwitch("mll2") ||
                          (_selection->getSwitch("mll2") && _selection->passCut("mll2", m_invMass))) {
                        theCounter->IncrVar("mll2",m_weight);
                        m_step19 = true;
                        
                        if (!_selection->getSwitch("higgsMassRel0j") || 
                            (_selection->getSwitch("higgsMassRel0j") && _selection->passCut("higgsMassRel0j", m_WWInvMass))) {
                          theCounter->IncrVar("higgsMassRel0j",m_weight);
                          m_step20 = true;                    
                          
                          m_preDeltaPhi = true;
                          if (!_selection->getSwitch("deltaPhi") ||
                              _selection->getSwitch("deltaPhi") && _selection->passCut("deltaPhi", m_deltaPhi)) {
                            theCounter->IncrVar("deltaPhi",m_weight); 
                            theCounter->IncrVar("final",m_weight);
                            m_step21 = true;
                            
                            return true;
                          }
                        }
                      }
                    }
                  }
                } else {
                  
                  // for nJets>0 we do not need cut by cut: just final counter
                  if(m_nJets==1) theCounter->IncrVar("WW1jet",m_weight);
                  
                  if(_selection->getSwitch("maxPtLepton") && !_selection->passCut("maxPtLepton", m_highPt)) return false;
                  m_step22 = true;
                  
                  if (_selection->getSwitch("minPtLepton") && !_selection->passCut("minPtLepton", m_lowPt)) return false;
                  if (_selection->getSwitch("minPtLepton") && m_lowPt < m_extraSlowLeptonPTMin) return false;
                  m_step23 = true;
                  
                  if(_selection->getSwitch("mll2") && !_selection->passCut("mll2", m_invMass)) return false;
                  m_step24 = true;
                  
                  if (_selection->getSwitch("higgsMassRel1j") && !_selection->passCut("higgsMassRel1j", m_WWInvMass)) return false;
                  theCounter->IncrVar("higgsMassRel1j",m_weight);
                  m_step25 = true;
                  
                  if (_selection->getSwitch("deltaPhi") && !_selection->passCut("deltaPhi", m_deltaPhi)) return false;
                  m_step26 = true;
                  
                  if (m_nJets==1) theCounter->IncrVar("oneJet",m_weight);
                  
                  if (m_nJets>1)  theCounter->IncrVar("gt1Jets",m_weight);
                  
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
    
  return false;
}


void CutBasedHiggsSelector::displayEfficiencies(std::string datasetName) {

  if( m_processID > -1 ) {

    std::map<int, Counters*>::const_iterator iter;
    for( iter=multiProcessCounter.begin(); iter!=multiProcessCounter.end(); ++iter ) {

      Counters *theCounter = iter->second;

      theCounter->Draw();
      theCounter->Draw("trigger","MCtruth");
      theCounter->Draw("preselected","trigger");
      theCounter->Draw("leptonId","preselected");     
      theCounter->Draw("leptonIso","leptonId");
      theCounter->Draw("convRej","leptonIso");
      theCounter->Draw("leptonD0","convRej");
      theCounter->Draw("looseMET","leptonD0");
      theCounter->Draw("mll","looseMET");
      theCounter->Draw("mllZPeak","mll");
      theCounter->Draw("mpmet","mllZPeak");
      theCounter->Draw("deltaPhiLLJet","mpmet");
      theCounter->Draw("nSoftMuons","deltaPhiLLJet");
      theCounter->Draw("nExtraLeptons","nSoftMuons");
      theCounter->Draw("bTagVeto","nExtraLeptons");
      theCounter->Draw("ptll","bTagVeto");
      theCounter->Draw("minPtLeptonSameFlav","ptll");
      theCounter->Draw("dymva","minPtLeptonSameFlav");
      theCounter->Draw("zeroJets","dymva");
      theCounter->Draw("maxPtLepton","bTagVeto");
      theCounter->Draw("minPtLepton","maxPtLepton");
      theCounter->Draw("mll2","minPtLepton");
      theCounter->Draw("higgsMassRel0j","mll2");
      theCounter->Draw("deltaPhi","higgsMassRel0j");      
      theCounter->Draw("final","preselected");
      theCounter->Draw("oneJet","preselected");
      theCounter->Draw("gt1Jets","preselected");
    }
  }

  else {

    char namefile[500];
    sprintf(namefile,"%s-Counters.root",datasetName.c_str());
    
    globalCounter->Draw();
    globalCounter->Draw("trigger","MCtruth");
    globalCounter->Draw("preselected","trigger");
    globalCounter->Draw("leptonId","preselected");     
    globalCounter->Draw("leptonIso","leptonId");
    globalCounter->Draw("convRej","leptonIso");
    globalCounter->Draw("leptonD0","convRej");
    globalCounter->Draw("looseMET","leptonD0");
    globalCounter->Draw("mll","looseMET");
    globalCounter->Draw("mllZPeak","mll");
    globalCounter->Draw("mpmet","mllZPeak");
    globalCounter->Draw("deltaPhiLLJet","mpmet");
    globalCounter->Draw("nSoftMuons","deltaPhiLLJet");
    globalCounter->Draw("nExtraLeptons","nSoftMuons");
    globalCounter->Draw("bTagVeto","nExtraLeptons");
    globalCounter->Draw("ptll","bTagVeto");
    globalCounter->Draw("minPtLeptonSameFlav","ptll");
    globalCounter->Draw("dymva","minPtLeptonSameFlav");
    globalCounter->Draw("zeroJets","dymva");
    globalCounter->Draw("maxPtLepton","bTagVeto");
    globalCounter->Draw("minPtLepton","maxPtLepton");
    globalCounter->Draw("mll2","minPtLepton");
    globalCounter->Draw("higgsMassRel0j","mll2");
    globalCounter->Draw("deltaPhi","higgsMassRel0j");
    globalCounter->Draw("final","preselected");
    globalCounter->Draw("oneJet","preselected");
    globalCounter->Draw("gt1Jets","preselected");
    
    globalCounter->Save(namefile,"update");
  }

}
