#include "cajun/json/reader.h"
#include "cajun/json/elements.h"

#include <fstream>
#include <sstream>

#include "TString.h"
#include "CommonTools/include/Utils.hh"
#include "CommonTools/include/LeptonIdBits.h"
#include "CommonTools/include/ElectronEffectiveArea.h"
#include "CommonTools/include/MuonEffectiveArea.h"
#include "EgammaAnalysisTools/include/ElectronLikelihood.h"
#include "HiggsAnalysisTools/include/Higgs.hh"

using namespace bits;

Higgs::Higgs(TTree *tree) : HiggsBase(tree)
{
  jsonFile = "";
  lastFile = "";

  //initialize the JES objects for calo and PF
  jecUnc_calo = 
    (JetCorrectionUncertainty*) new JetCorrectionUncertainty("data/JES/GR_R_42_V19_AK5Calo_Uncertainty.txt");
  jecUnc_PF = 
    (JetCorrectionUncertainty*) new JetCorrectionUncertainty("data/JES/GR_R_42_V19_AK5PF_Uncertainty.txt");

}

Higgs::~Higgs()
{
  // By this time, the destructor of HiggsBase has not yet been called.
  // This means that the tree has not yet been deleted.
  // So, we do nothing here.
}

void Higgs::setRequiredTriggers(const std::vector<std::string>& reqTriggers) {
  requiredTriggers=reqTriggers;
}

bool Higgs::hasPassedHLT() {
  Utils anaUtils;
  return anaUtils.getTriggersOR(m_requiredTriggers, firedTrg);
}

void Higgs::setJsonGoodRunList(const std::string& jsonFilePath)
{
  jsonFile=jsonFilePath;
}

void Higgs::fillRunLSMap()
{
  
  if (jsonFile == "")
    {
      std::cout << "Cannot fill RunLSMap. json file not configured" << std::endl;
      return;
    }

  std::ifstream jsonFileStream;
  jsonFileStream.open(jsonFile.c_str());
  if (!jsonFileStream.is_open())
    {
      std::cout << "Unable to open file " << jsonFile << std::endl;
      return;
    }

  json::Object elemRootFile;
  json::Reader::Read(elemRootFile, jsonFileStream);

  for (json::Object::const_iterator itRun=elemRootFile.Begin();itRun!=elemRootFile.End();++itRun)
    {
      const json::Array& lsSegment = (*itRun).element;
      LSSegments thisRunSegments; 
      for (json::Array::const_iterator lsIterator=lsSegment.Begin();lsIterator!=lsSegment.End();++lsIterator)
	{
	  json::Array lsSegment=(*lsIterator);
	  json::Number lsStart=lsSegment[0];	   
	  json::Number lsEnd=lsSegment[1];
	  aLSSegment thisSegment;
	  thisSegment.first=lsStart.Value();
	  thisSegment.second=lsEnd.Value();
	  thisRunSegments.push_back(thisSegment);
	  //	   std::pair<int, int> lsSegment=std::pair<int, int>(atoi(,lsIterator[1]); 
	}
      goodRunLS.insert(aRunsLSSegmentsMapElement(atoi((*itRun).name.c_str()),thisRunSegments));
    }


  std::cout << "[GoodRunLSMap]::Good Run LS map filled with " << goodRunLS.size() << " runs" << std::endl;
  for (runsLSSegmentsMap::const_iterator itR=goodRunLS.begin(); itR!=goodRunLS.end(); ++itR)
    {
      std::cout << "[GoodRunLSMap]::Run " << (*itR).first <<  " LS ranges are: ";
      for (LSSegments::const_iterator iSeg=(*itR).second.begin();iSeg!=(*itR).second.end();++iSeg)
	std::cout << "[" << (*iSeg).first << "," << (*iSeg).second << "] "; 
      std::cout << std::endl;
    }
}

bool Higgs::isGoodRunLS()
{
  runsLSSegmentsMap::const_iterator thisRun=goodRunLS.find(runNumber);
  if (thisRun == goodRunLS.end())
    return false;
  //  std::cout << runNumber << " found in the good run map" << std::endl;
  for (LSSegments::const_iterator iSeg=goodRunLS[runNumber].begin();iSeg!=goodRunLS[runNumber].end();++iSeg)
    {
      //      std::cout << "Range is [" << (*iSeg).first << "," << (*iSeg).second << "]" << std::endl;
      if ( lumiBlock >= (*iSeg).first && lumiBlock <= (*iSeg).second)
	return true;
    }
  return false;
}

bool Higgs::reloadTriggerMask(bool newVersion)
{
  if(newVersion) {
    std::vector<int> triggerMask;
    for (std::vector< std::string >::const_iterator fIter=requiredTriggers.begin();fIter!=requiredTriggers.end();++fIter)
      {   
        for(unsigned int i=0; i<nameHLT->size(); i++)
          {
            if( !strcmp ((*fIter).c_str(), nameHLT->at(i).c_str() ) )
              {
                triggerMask.push_back( indexHLT[i] ) ;
                break;
              }
          }
      }
    m_requiredTriggers = triggerMask;
  } else {
    TString fileName=((TChain*)fChain)->GetFile()->GetName();
    if ( TString(lastFile) != fileName )
      {

        std::cout << "[ReloadTriggerMask]::File has changed reloading trigger mask" << std::endl;
        lastFile = fileName;
        TTree *treeCond;
        std::cout << "[ReloadTriggerMask]::Opening " << fileName << std::endl;
        treeCond = (TTree*)((TChain*)fChain)->GetFile()->Get("Conditions");
        int           nHLT_;
        std::vector<std::string>  *nameHLT_;
        std::vector<unsigned int> *indexHLT_;

        //To get the pointers for the vectors
        nameHLT_=0;
        indexHLT_=0;

        treeCond->SetBranchAddress("nHLT", &nHLT_);
        treeCond->SetBranchAddress("nameHLT", &nameHLT_);
        treeCond->SetBranchAddress("indexHLT", &indexHLT_);
        treeCond->GetEntry(0);

        std::vector<int> triggerMask;
        for (std::vector< std::string >::const_iterator fIter=requiredTriggers.begin();fIter!=requiredTriggers.end();++fIter)
          {
            for(unsigned int i=0; i<nameHLT_->size(); i++) 
              {
                if( !strcmp ((*fIter).c_str(), nameHLT_->at(i).c_str() ) ) 
                  {
                    triggerMask.push_back( indexHLT_->at(i) ) ;
                    break;
                  }
              }
          }
        m_requiredTriggers = triggerMask;
        for (int i=0;i<m_requiredTriggers.size();++i)
          std::cout << "[ReloadTriggerMask]::Requiring bit " << m_requiredTriggers[i] << " " << requiredTriggers[i] << std::endl;
      }
  }
}

float Higgs::mT3(TLorentzVector pl1, TLorentzVector pl2, TVector3 met) {
  float pTll = (pl1.Vect() + pl2.Vect()).Pt();
  float mll = (pl1 + pl2).M();
  float El = sqrt(pTll*pTll + mll*mll);
  float pTnu = met.Pt();
  float Enu = sqrt(pTnu*pTnu + mll*mll);
  float Ex = (pl1+pl2).X() + met.X();
  float Ey = (pl1+pl2).Y() + met.Y();
  float mnu = mll;

  return sqrt(mll*mll + mnu*mnu + 2*(El*Enu-Ex*Ex-Ey*Ey));
}

float Higgs::likelihoodRatio(int eleIndex, ElectronLikelihood &lh) {
  LikelihoodMeasurements measurements;
  Utils anaUtils;
  bool inEB=anaUtils.fiducialFlagECAL(fiducialFlagsEle[eleIndex],isEB);
  measurements.pt = GetPt(pxEle[eleIndex],pyEle[eleIndex]);
  if(inEB && fabs(etaEle[eleIndex])<1.0) measurements.subdet = 0;
  else if (inEB && fabs(etaEle[eleIndex])>=1.0) measurements.subdet = 1;
  else measurements.subdet = 2;
  measurements.deltaPhi = deltaPhiAtVtxEle[eleIndex];
  measurements.deltaEta = deltaEtaAtVtxEle[eleIndex];
  measurements.eSeedClusterOverPout = eSeedOverPoutEle[eleIndex];
  measurements.eSuperClusterOverP = eSuperClusterOverPEle[eleIndex];
  measurements.hadronicOverEm = hOverEEle[eleIndex];
  measurements.sigmaIEtaIEta = SigmaiEiE(eleIndex);
  measurements.sigmaIPhiIPhi = SigmaiPiP(eleIndex);
  measurements.fBrem = fbremEle[eleIndex];
  measurements.nBremClusters = nbremsEle[eleIndex];
  int gsftrack = gsfTrackIndexEle[eleIndex];
  TVector3 pIn(pxGsfTrack[gsftrack],pyGsfTrack[gsftrack],pzGsfTrack[gsftrack]);
  measurements.OneOverEMinusOneOverP = 1./(eSuperClusterOverPEle[eleIndex]*pIn.Mag()) - 1./pIn.Mag();
  return lh.resultLog(measurements);
}

/// sigma ieta ieta of the seed cluster (ecal-driven/tracker-driven)
float Higgs::SigmaiEiE(int electron) {
  float see;
  Utils anaUtils;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[electron], isEcalDriven);
  if(ecaldriven) {
    int sc = superClusterIndexEle[electron];
    see = sqrt(covIEtaIEtaSC[sc]);
  } else {
    int sc = PFsuperClusterIndexEle[electron];
    if(sc>-1) {
      see = sqrt(covIEtaIEtaPFSC[sc]);
    } else {
      see = 999.;
    }
  }
  return see;
}

/// sigma iphi iphi of the seed cluster (ecal-driven/tracker-driven)
float Higgs::SigmaiPiP(int electron) {
  float spp;
  Utils anaUtils;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[electron], isEcalDriven);
  if(ecaldriven) {
    int sc = superClusterIndexEle[electron];
    spp = sqrt(covIPhiIPhiSC[sc]);
  } else {
    int sc = PFsuperClusterIndexEle[electron];
    if(sc>-1) {
      spp = sqrt(covIPhiIPhiPFSC[sc]);
    } else {
      spp = 999.;
    }
  }
  return spp;
}

bool Higgs::isPFJetID(float eta, float neutralHadFrac, float neutralEmFraction, int nConstituents, float chargedHadFraction, 
                      float chargedMultiplicity, float chargedEmFraction, int WP) {
  switch(WP) {
  case none:
    return true;
    break;
  case loose:
    if(neutralHadFrac>=0.99 || neutralEmFraction>=0.99 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (chargedHadFraction==0 || chargedMultiplicity==0 || chargedEmFraction>=0.99) ) return false;
    break;
  case medium:
    if(neutralHadFrac>=0.95 || neutralEmFraction>=0.95 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (chargedHadFraction==0 || chargedMultiplicity==0 || chargedEmFraction>=0.99) ) return false;
    break;
  case tight:
    if(neutralHadFrac>=0.90 || neutralEmFraction>=0.90 || nConstituents<=1) return false;
    if(fabs(eta)<2.4 && (chargedHadFraction==0 || chargedMultiplicity==0 || chargedEmFraction>=0.99) ) return false;
    break;
  default:
    std::cout << "Jet::isPFJetID(nt WP). Requested wrong Working point. Available are loose, medium, tight." << std::endl;
    return false;
  }
  return true;
}

/// *****************************************
/// From C. Rogan, M. Pierini, M. Spiropulu 
/// *****************************************

//
// this is the 'new' MRstar
//
double Higgs::CalcMRstar(TLorentzVector ja, TLorentzVector jb){
  double A = ja.P();
  double B = jb.P();
  double az = ja.Pz();
  double bz = jb.Pz();
  TVector3 jaT, jbT;
  jaT.SetXYZ(ja.Px(),ja.Py(),0.0);
  jbT.SetXYZ(jb.Px(),jb.Py(),0.0);

  double temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-
                     (jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());

  return temp;
}



//
// this is the 'new' MRstar, times 'gamma_{R*}' - I would recommend making 'R*' with this as 
// the denominator and 'M_{T}^{R}' as the numerator (the next function in this file)
//
double Higgs::CalcGammaMRstar(TLorentzVector ja, TLorentzVector jb){
  double A = ja.P();
  double B = jb.P();
  double az = ja.Pz();
  double bz = jb.Pz();
  TVector3 jaT, jbT;
  jaT.SetXYZ(ja.Px(),ja.Py(),0.0);
  jbT.SetXYZ(jb.Px(),jb.Py(),0.0);
  double ATBT = (jaT+jbT).Mag2();

  double temp = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-
                     (jbT.Dot(jbT)-jaT.Dot(jaT))*(jbT.Dot(jbT)-jaT.Dot(jaT))/(jaT+jbT).Mag2());

  double mybeta = (jbT.Dot(jbT)-jaT.Dot(jaT))/
    sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));

  double mygamma = 1./sqrt(1.-mybeta*mybeta);

  //gamma times MRstar
  temp *= mygamma;

  return temp;
}


//
// this is MR including the ISR recoil
//
double Higgs::CalcMRNEW(TLorentzVector P, TLorentzVector Q, TVector3 MET){

  double PpQ = sqrt((P.P()+Q.P())*(P.P()+Q.P())-(P.Pz()+Q.Pz())*(P.Pz()+Q.Pz())); //old GammaMRstar function

  double vptx = (P+Q).Px();
  double vpty = (P+Q).Py();
  TVector3 vpt;
  vpt.SetXYZ(vptx,vpty,0.0);

  TVector3 vI = vpt+MET;

  vI.SetZ(0.0); //just in case someone forgot....
    
  float MR2 = 0.5*(PpQ*PpQ-vpt.Dot(vI)+PpQ*sqrt(PpQ*PpQ+vI.Dot(vI)-2.*vI.Dot(vpt)));
  
  return sqrt(MR2);
}

//
// This is 'M_{T}^{R}', the guy that should be used in the numerator of 'R' or 'R*'
//
double Higgs::CalcMTR(TLorentzVector ja, TLorentzVector jb, TVector3 met){

  double temp = met.Mag()*(ja.Pt()+jb.Pt()) - met.Dot(ja.Vect()+jb.Vect());
  temp /= 2.;

  temp = sqrt(temp);

  return temp;
}


TVector3 Higgs::XYCorrectedMet(TVector3 MET, int sample){
  float dmetx,dmety;
  if(sample == kDATA2011) {
    dmetx =  3.87339e-1 + 2.58294e-1 * nPV;
    dmety = -7.83502e-1 - 2.88899e-1 * nPV;
  } else if(sample==kMC2011) {
    dmetx = -1.94451e-2 - 4.38986e-3 * nPV;
    dmety = -4.31368e-1 - 1.90753e-1 * nPV;
  } else if(sample==kDATA2012) {
    dmetx =  3.54233e-01 + 2.65299e-01 * nPV;
    dmety =  1.88923e-01 - 1.66425e-01 * nPV;
  } else if(sample==kMC2012) {
    dmetx = -2.99576e-02 - 6.61932e-02 * nPV;
    dmety =  3.70819e-01 - 1.48617e-01 * nPV;
  } else { // nochange
    dmetx = 0;
    dmety = 0;
  }

  TVector3 newmet(MET.x()-dmetx,MET.y()-dmety,0.);
  return newmet;
}

std::vector<int> Higgs::sortElectronsByPt(std::vector<int> electrons) {
  int tmp;
  int max;
  for(int i=0;i<(int)electrons.size();i++) {
      max = i;
      float maxelePt = GetPt(pxEle[i],pyEle[i]);
      for(int x=i; x<(int)electrons.size(); x++) {
        float xelePt = GetPt(pxEle[x],pyEle[x]);
        if(xelePt > maxelePt) {
          max = x;
        }
      }
      tmp = electrons[i];
      electrons[i] = electrons[max];
      electrons[max] = tmp;
  }
  return electrons;
}

std::vector<int> Higgs::sortMuonsByPt(std::vector<int> muons) {
  int tmp;
  int max;
  for(int i=0;i<(int)muons.size();i++) {
      max = i;
      float maxmuonPt = GetPt(pxMuon[i],pyMuon[i]);
      for(int x=i; x<(int)muons.size(); x++) {
        float xmuonPt = GetPt(pxMuon[x],pyMuon[x]);
        if(xmuonPt > maxmuonPt) {
          max = x;
        }
      }
      tmp = muons[i];
      muons[i] = muons[max];
      muons[max] = tmp;
  }
  return muons;
}

TLorentzVector Higgs::GetJESCorrected(TLorentzVector p4jet, const char *ScaleDirection) {

  float mass = p4jet.M();
  float ptUnscaled = p4jet.Pt();
  
  // estimate the uncertainty
  jecUnc_PF->setJetEta(p4jet.Eta());
  jecUnc_PF->setJetPt(ptUnscaled);

  int scaleEnergy = 0;
  if(TString(ScaleDirection).Contains("Up")) scaleEnergy = 1.0;
  if(TString(ScaleDirection).Contains("Down")) scaleEnergy = -1.0;

  // apply the uncertainty
  float pt = ptUnscaled + scaleEnergy*jecUnc_PF->getUncertainty(true)*ptUnscaled;
  float p = pt/fabs(sin(p4jet.Theta()));
  float energy = sqrt(p*p+mass*mass);

  TLorentzVector p4Scaled;
  p4Scaled.SetPtEtaPhiE(pt,p4jet.Eta(),p4jet.Phi(),energy);

  return p4Scaled;
}

TVector3 Higgs::pfChargedMet(TVector3 lep1, TVector3 lep2) {

  float chMetP3x = pxPFChMet[0];
  float chMetP3y = pyPFChMet[0];

  TVector3 firstPV(PVxPV[0],PVyPV[0],PVzPV[0]);

  // charged PF MET has been computed with all the PF cands (inverted -p)
  // first remove the contribution in dR = 0.1 to avoid double counting
  for(int i=0; i<nReducedPFCand; i++) {
    if(chargeReducedPFCand[i]==0) continue;
    TVector3 pfCandP3(pxReducedPFCand[i],pyReducedPFCand[i],pzReducedPFCand[i]);
    TVector3 pfCandVPos(vertexXReducedPFCand[i],vertexYReducedPFCand[i],vertexZReducedPFCand[i]);
    if(fabs(trackDzPV(firstPV,pfCandVPos,pfCandP3))>0.1) continue;
    if(pfCandP3.DeltaR(lep1)<=0.1 || pfCandP3.DeltaR(lep2)<=0.1) {
      chMetP3x += pxReducedPFCand[i];
      chMetP3y += pyReducedPFCand[i];
    }
  }

  // then add back the RECO leptons
  chMetP3x -= (lep1.Px() + lep2.Px());
  chMetP3y -= (lep1.Py() + lep2.Py());
  
  return TVector3(chMetP3x,chMetP3y,0.0);

}

std::string Higgs::getHLTPathForRun(int runN, std::string fullname) {
  TString fullName = TString(fullname.c_str());
  TObjArray* selectionTokens = fullName.Tokenize(":");
  if (selectionTokens->GetEntries()!=2) {
    std::cout << "Wrong trigger strings " << selectionTokens->GetEntries() << std::endl;
    return std::string("NOPATH");
  }
  TString RunRange =((TObjString*)(*selectionTokens)[0])->GetString();
  TString HLTPathName =((TObjString*)(*selectionTokens)[1])->GetString();
  
  TObjArray* runs = RunRange.Tokenize("-");
  if (runs->GetEntries()!=2) {
    std::cout << "Wrong trigger run range strings " << runs->GetEntries() << std::endl;
    return std::string("NOPATH");    
  }
  
  const char *minStr = (((TObjString*)(*runs)[0])->GetString()).Data();
  const char *maxStr = (((TObjString*)(*runs)[1])->GetString()).Data();

  int min = atoi(minStr);
  int max = atoi(maxStr);

  if(runN>=min && runN<=max) return std::string(HLTPathName.Data());
  else return std::string("NOPATH");
}

// this is the smurfs tight denominator
bool Higgs::isEleDenomFake(int theEle, bool *isDenomEleID, bool *isDenomEleIso) {
  
  Utils anaUtils;
  bool isGoodDenom = true;
  bool isGoodDenomID, isGoodDenomIso;
  isGoodDenomID = isGoodDenomIso = true;
  TVector3 p3Ele(pxEle[theEle], pyEle[theEle], pzEle[theEle]);
  
  // acceptance
  if( fabs(p3Ele.Eta()) > 2.5 ) { isGoodDenom = false; isGoodDenomID = false; isGoodDenomIso = false; }
  if( p3Ele.Pt() < 10.  )       { isGoodDenom = false; isGoodDenomID = false; isGoodDenomIso = false; }

  // taking shower shape
  int sc;
  bool ecalDriven = anaUtils.electronRecoType(recoFlagsEle[theEle], bits::isEcalDriven);
  float thisSigmaIeIe = -1.;
  float scEta = -1.;
  if ( ecalDriven) {
    sc = superClusterIndexEle[theEle];
    thisSigmaIeIe = sqrt(covIEtaIEtaSC[sc]);
    scEta = etaSC[sc];
  }
  if (!ecalDriven) {
    sc = PFsuperClusterIndexEle[theEle];
    thisSigmaIeIe = sqrt(covIEtaIEtaPFSC[sc]);
    scEta = etaPFSC[sc];
  }
  if ( sc < 0 ) { isGoodDenom = false; isGoodDenomID = false; isGoodDenomIso = false; }

  // barrel or endcap
  bool isEleEB = false;
  if (fabs(scEta)<1.479) isEleEB = true;

  // sigmaIetaIeta
  if ( isEleEB && thisSigmaIeIe>0.01) { isGoodDenom = false; isGoodDenomID = false; }
  if (!isEleEB && thisSigmaIeIe>0.03) { isGoodDenom = false; isGoodDenomID = false; }

  // isolation
  float ecalIsolAbs = 0.0;
  if ( isEleEB ) ecalIsolAbs = max(0.0,dr03EcalRecHitSumEtEle[theEle]-1.0);
  else ecalIsolAbs = dr03EcalRecHitSumEtEle[theEle];
  float ecalIsol = ecalIsolAbs/p3Ele.Pt();
  float hcalIsol = (dr03HcalTowerSumEtEle[theEle])/p3Ele.Pt();
  float trackerIsol = (dr03TkSumPtEle[theEle])/p3Ele.Pt();

  if (ecalIsol>0.2) { isGoodDenom = false; isGoodDenomIso = false; }
  if (hcalIsol>0.2) { isGoodDenom = false; isGoodDenomIso = false; }
  if (trackerIsol>0.2) { isGoodDenom = false; isGoodDenomIso = false; }

  // H/E
  if ( isEleEB && hOverEEle[theEle]>0.12) { isGoodDenom = false; isGoodDenomID = false; }
  if (!isEleEB && hOverEEle[theEle]>0.10) { isGoodDenom = false; isGoodDenomID = false; }

  // deltaEta
  if ( isEleEB && (fabs(deltaEtaAtVtxEle[theEle])>0.007) ) { isGoodDenom = false; isGoodDenomID = false; }
  if (!isEleEB && (fabs(deltaEtaAtVtxEle[theEle])>0.009) ) { isGoodDenom = false; isGoodDenomID = false; }

  // deltaPhi
  if ( isEleEB && (fabs(deltaPhiAtVtxEle[theEle])>0.15) ) { isGoodDenom = false; isGoodDenomID = false; }
  if (!isEleEB && (fabs(deltaPhiAtVtxEle[theEle])>0.10) ) { isGoodDenom = false; isGoodDenomID = false; }

  // full conversion rejection
  int gsf = gsfTrackIndexEle[theEle];
  int missHits = expInnerLayersGsfTrack[gsf];
  bool matchConv = hasMatchedConversionEle[theEle];
  if (missHits>0 || matchConv) isGoodDenom = false;

  // impact parameter cuts
  float dxyEle = transvImpactParGsfTrack[gsf];
  float dzEle  = eleDzPV(theEle,0);
  if (fabs(dxyEle)>0.02) isGoodDenom = false;
  if (fabs(dzEle)>0.10)  isGoodDenom = false;

  *isDenomEleID = isGoodDenomID;
  *isDenomEleIso = isGoodDenomIso;

  return isGoodDenom;
}

bool Higgs::isMuonDenomFake(int theMuon, bool *isDenomMuonID, bool *isDenomMuonIso) {

  bool isGoodDenom = true;
  bool isGoodDenomID, isGoodDenomIso;
  isGoodDenomID = isGoodDenomIso = true;
  TVector3 p3Muon(pxMuon[theMuon], pyMuon[theMuon], pzMuon[theMuon]);
  
  // acceptance	   
  if( fabs(p3Muon.Eta()) > 2.5 ) { isGoodDenom = false; isGoodDenomID = false; isGoodDenomIso = false; }
  if( p3Muon.Pt() < 10. )        { isGoodDenom = false; isGoodDenomID = false; isGoodDenomIso = false; }

  // muonID
  bool isTight = true;
  isMuonID(theMuon, &isTight);
  if (!isTight) { isGoodDenom = false; isGoodDenomID = false; }
  
  // isolation
  float thePFMuonIso = pfCombinedIsoMuon[theMuon]/p3Muon.Pt();
  if ( thePFMuonIso > 0.4 ) { isGoodDenom = false; isGoodDenomIso = false; }
  
  // IP
  int ctfMuon   = trackIndexMuon[theMuon]; 
  float dxyMuon = transvImpactParTrack[ctfMuon];
  float dzMuon  = muonDzPV(theMuon,0);
  if (fabs(dxyMuon)>0.2 ) { isGoodDenom = false; isGoodDenomID = false; }   
  if (fabs(dzMuon)>0.1  ) { isGoodDenom = false; isGoodDenomID = false; }   
  
  *isDenomMuonID  = isGoodDenomID;
  *isDenomMuonIso = isGoodDenomIso;

  return isGoodDenom;
}

void Higgs::isMuonID(int muonIndex, bool *muonIdOutput) {

  *muonIdOutput = true;

  Utils anaUtils; 
  bool flagGlobalMu = false;
  if(anaUtils.muonIdVal(muonIdMuon[muonIndex],AllGlobalMuons)) {
    int globalMuonTrack = combinedTrackIndexMuon[muonIndex];
    if(trackNormalizedChi2GlobalMuonTrack[globalMuonTrack] < 10 && 
       trackValidHitsGlobalMuonTrack[globalMuonTrack] > 0 &&
       numberOfMatchesMuon[muonIndex] > 1 ) flagGlobalMu = true; // to be used when new trees are available
  }

  bool flagTrackerMu = false;
  if( (anaUtils.muonIdVal(muonIdMuon[muonIndex],AllTrackerMuons) &&
       anaUtils.muonIdVal(muonIdMuon[muonIndex],TMLastStationTight)) ) flagTrackerMu  = true;

  if(!(flagGlobalMu || flagTrackerMu)) {
    *muonIdOutput = false;
    return;
  }
    
  int track = trackIndexMuon[muonIndex];

  if(trackValidHitsTrack[track]<=10) *muonIdOutput = false;

  if( (numberOfValidPixelBarrelHitsTrack[track]+numberOfValidPixelEndcapHitsTrack[track])<1 ) *muonIdOutput = false; 

  if(kinkMuon[muonIndex]>=20) *muonIdOutput = false;

  float ptTrack = sqrt( pxTrack[track]*pxTrack[track] + pyTrack[track]*pyTrack[track] );
  float sign = fabs(ptErrorTrack[track]/ptTrack);
  if (sign>=0.1) *muonIdOutput = false;
}

void Higgs::isMuonID2012(int muonIndex, bool *muonIdOutput) {

  *muonIdOutput = true;

  Utils anaUtils; 
  bool flagGlobalMu = false;
  if(anaUtils.muonIdVal(muonIdMuon[muonIndex],AllGlobalMuons)) {
    int globalMuonTrack = combinedTrackIndexMuon[muonIndex];
    if(trackNormalizedChi2GlobalMuonTrack[globalMuonTrack] < 10 && 
       numberOfValidMuonHitsGlobalMuonTrack[globalMuonTrack] > 0 &&
       numberOfMatchesMuon[muonIndex] > 1 ) flagGlobalMu = true; // to be used when new trees are available
  }

  bool flagTrackerMu = false;
  if( (anaUtils.muonIdVal(muonIdMuon[muonIndex],AllTrackerMuons) &&
       anaUtils.muonIdVal(muonIdMuon[muonIndex],TMLastStationTight)) ) flagTrackerMu  = true;

  if(!(flagGlobalMu || flagTrackerMu)) {
    *muonIdOutput = false;
    return;
  }
    
  int track = trackIndexMuon[muonIndex];

  int trackerLayersWithMeasurement = trackerLayersWithMeasurementTrack[track];
  if(trackerLayersWithMeasurement<6) *muonIdOutput = false;

  if( (numberOfValidPixelBarrelHitsTrack[track]+numberOfValidPixelEndcapHitsTrack[track])<1 ) *muonIdOutput = false; 

  if(kinkMuon[muonIndex]>=20) *muonIdOutput = false;

  if(pfmuonIdMuon[muonIndex]==0) *muonIdOutput = false;

  float ptTrack = sqrt( pxTrack[track]*pxTrack[track] + pyTrack[track]*pyTrack[track] );
  float sign = fabs(ptErrorTrack[track]/ptTrack);
  if (sign>=0.1) *muonIdOutput = false;

}

bool Higgs::isPFIsolatedMuon(int muonIndex) {
  float eta = etaMuon[muonIndex];
  float pt = GetPt(pxMuon[muonIndex],pyMuon[muonIndex]);
  float iso = pfCombinedIsoMuon[muonIndex]/pt;
  if( pt>=20. && fabs(eta)<1.479 ) return (iso < 0.13);
  if( pt>=20. && fabs(eta)>=1.479 ) return (iso < 0.09);
  if( pt<20. && fabs(eta)<1.479 ) return (iso < 0.06);
  if( pt<20. && fabs(eta)>=1.479 ) return (iso < 0.05);
  return true;
}

bool Higgs::isPFIsolatedMuon2012(int muonIndex) {

  float abseta = fabs(etaMuon[muonIndex]);
  float pt = GetPt(pxMuon[muonIndex],pyMuon[muonIndex]);
  float bdtiso = mvaisoMuon[muonIndex];

  bool MUON_ISO_ICHEP2012 = (
                             ( pt <= 20 && abseta >= 0.000 && abseta < 1.479 && bdtiso > 0.86 ) ||
                             ( pt <= 20 && abseta >= 1.479 && abseta < 2.400 && bdtiso > 0.82 ) ||
                             ( pt >  20 && abseta >= 0.000 && abseta < 1.479 && bdtiso > 0.82 ) ||
                             ( pt >  20 && abseta >= 1.479 && abseta < 2.400 && bdtiso > 0.86 )
                             );

  return MUON_ISO_ICHEP2012;
}

void Higgs::isEleID(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput, CutBasedEleIDSelector *thisCutBasedID, bool applyBDTIdNotCutbased) {
  
  *eleIdOutput = *isolOutput = *convRejOutput = false;

  Utils anaUtils;
  int gsf = gsfTrackIndexEle[eleIndex];
  float pt = GetPt(pxEle[eleIndex],pyEle[eleIndex]);

  // if is ECAL driven, take the electron ID variables from the standard electron
  // above all, take the ECAL supercluster instead of PF super cluster
  float HoE, s9s25, deta, dphiin, dphiout, fbrem, see, spp, eopout, eop;
  float e1, e4SwissCross, fidFlagSC, seedRecHitFlag, seedTime, seedChi2;
  float sceta;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[eleIndex], isEcalDriven);
  HoE = hOverEEle[eleIndex];
  deta = deltaEtaAtVtxEle[eleIndex];
  dphiin = deltaPhiAtVtxEle[eleIndex];
  dphiout = deltaPhiAtCaloEle[eleIndex];
  fbrem = fbremEle[eleIndex];
  eopout = eSeedOverPoutEle[eleIndex];
  eop = eSuperClusterOverPEle[eleIndex];
  if(ecaldriven) {
    int sc = superClusterIndexEle[eleIndex];
    s9s25 = e3x3SC[sc]/e5x5SC[sc];
    see = sqrt(covIEtaIEtaSC[sc]);
    spp = sqrt(covIPhiIPhiSC[sc]);
    e1 = eMaxSC[sc];
    e4SwissCross = e4SwissCrossSC[sc];
    fidFlagSC = fiducialFlagsEle[eleIndex];
    seedRecHitFlag = recoFlagSC[sc];
    seedTime = timeSC[sc];
    seedChi2 = chi2SC[sc];
    sceta = etaSC[sc];
  } else {
    int sc = PFsuperClusterIndexEle[eleIndex];
    if(sc>-1) {
      s9s25 = e3x3PFSC[sc]/e5x5PFSC[sc];
      see = sqrt(covIEtaIEtaPFSC[sc]);
      spp = sqrt(covIPhiIPhiPFSC[sc]);
      e1 = eMaxSC[sc];
      e4SwissCross = e4SwissCrossPFSC[sc];
      fidFlagSC = fiducialFlagsEle[eleIndex];
      seedRecHitFlag = recoFlagPFSC[sc];
      seedTime = timePFSC[sc];
      seedChi2 = chi2PFSC[sc];
      sceta = etaPFSC[sc];
    } else {
      s9s25 = 999.;
      see = 999.;
      spp = 999.;
      sceta = 999.;
    }
  }

  
  thisCutBasedID->SetEcalFiducialRegion( fiducialFlagsEle[eleIndex] );
  thisCutBasedID->SetRecoFlag(recoFlagsEle[eleIndex]);
  thisCutBasedID->applyElectronIDOnPFlowElectrons(true);
  thisCutBasedID->SetHOverE( HoE );
  thisCutBasedID->SetS9S25( s9s25 );
  thisCutBasedID->SetDEta( deta );
  thisCutBasedID->SetDPhiIn( dphiin );
  thisCutBasedID->SetDPhiOut( dphiout );
  thisCutBasedID->SetBremFraction( fbrem );
  thisCutBasedID->SetSigmaEtaEta( see );
  thisCutBasedID->SetSigmaPhiPhi( spp );
  thisCutBasedID->SetEOverPout( eopout );
  thisCutBasedID->SetEOverPin( eop );
  thisCutBasedID->SetElectronClass ( classificationEle[eleIndex] );
  //  thisCutBasedID->SetLikelihood( likelihoodRatio(eleIndex,*LH) );
  thisCutBasedID->SetLikelihood( eleIdLikelihoodEle[eleIndex] );
  thisCutBasedID->SetNBrem( nbremsEle[eleIndex] );
  thisCutBasedID->SetEcalIsolation( (dr03EcalRecHitSumEtEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );                
  thisCutBasedID->SetTrkIsolation ( (dr03TkSumPtEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );                        
  thisCutBasedID->SetHcalIsolation( (dr03HcalTowerSumEtFullConeEle[eleIndex] - rhoFastjet*TMath::Pi()*0.3*0.3)/pt );         
  float iso = 0.0;
  if ( anaUtils.fiducialFlagECAL(fiducialFlagsEle[eleIndex],isEB) ) iso = dr03TkSumPtEle[eleIndex] + max(0.0,dr03EcalRecHitSumEtEle[eleIndex]-1.0) + dr03HcalTowerSumEtFullConeEle[eleIndex];
  else iso = dr03TkSumPtEle[eleIndex] + dr03EcalRecHitSumEtEle[eleIndex] + dr03HcalTowerSumEtFullConeEle[eleIndex];
  thisCutBasedID->SetCombinedIsolation( (iso - rhoFastjet*TMath::Pi()*0.3*0.3) / pt );
  thisCutBasedID->SetCombinedPFIsolation( (pfCombinedIsoEle[eleIndex]) / pt );
  thisCutBasedID->SetMissingHits( expInnerLayersGsfTrack[gsf] );
  thisCutBasedID->SetConvDist( fabs(convDistEle[eleIndex]) );
  thisCutBasedID->SetConvDcot( fabs(convDcotEle[eleIndex]) );
  thisCutBasedID->SetHasMatchedConversion ( hasMatchedConversionEle[eleIndex] );

  // ECAL cleaning variables
  thisCutBasedID->m_cleaner->SetE1(e1);
  thisCutBasedID->m_cleaner->SetE4SwissCross(e4SwissCross);
  thisCutBasedID->m_cleaner->SetFiducialFlag(fidFlagSC);
  thisCutBasedID->m_cleaner->SetSeedFlag(seedRecHitFlag);
  thisCutBasedID->m_cleaner->SetSeedTime(seedTime);
  thisCutBasedID->m_cleaner->SetSeedChi2(seedChi2);

  //  return egammaCutBasedID.output(); // class dependent result
  if(!applyBDTIdNotCutbased) *eleIdOutput = thisCutBasedID->outputNoClassEleId();
  else {
    float bdt = eleBDT(fMVA,eleIndex);
    *eleIdOutput = passEleBDT(pt,sceta,bdt);
  }
  *isolOutput = thisCutBasedID->outputNoClassIso();
  *convRejOutput = thisCutBasedID->outputNoClassConv();
}

float Higgs::corrEleIso2012(int eleIndex) {

  float abseta=-100.;
  Utils anaUtils;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[eleIndex], isEcalDriven);
  if(ecaldriven) {
    int sc = superClusterIndexEle[eleIndex];
    abseta=fabs(etaSC[sc]);
  } else {
    int sc = PFsuperClusterIndexEle[eleIndex];
    abseta=fabs(etaPFSC[sc]);
  }

  // calculate the PU subtracted isolation
  ElectronEffectiveArea::ElectronEffectiveAreaTarget effAreaTarget_ = ElectronEffectiveArea::kEleEAData2012;
  ElectronEffectiveArea::ElectronEffectiveAreaType effAreaGamma_   = ElectronEffectiveArea::kEleGammaIso04;
  ElectronEffectiveArea::ElectronEffectiveAreaType effAreaNeutralHad_ = ElectronEffectiveArea::kEleNeutralHadronIso04;
  ElectronEffectiveArea::ElectronEffectiveAreaType effAreaGammaAndNeutralHad_ = ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04;

  float eff_area_ga  = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaGamma_, abseta, effAreaTarget_);
  float eff_area_nh  = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaNeutralHad_, abseta, effAreaTarget_);
  float eff_area_ganh = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaGammaAndNeutralHad_, abseta, effAreaTarget_);

  float iso = pfCandChargedIso04Ele[eleIndex];
  //  iso += max<float>(0.,pfCandNeutralIso04Ele[eleIndex]-eff_area_nh*rhoJetsFastJet + pfCandPhotonIso04Ele[eleIndex]-eff_area_ga*rhoJetsFastJet);
  iso += max<float>(0.,pfCandNeutralIso04Ele[eleIndex]+pfCandPhotonIso04Ele[eleIndex]-eff_area_ganh*rhoJetsFastJet);

  return iso;

}

void Higgs::isEleID2012(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput) {
  
  *eleIdOutput = *isolOutput = *convRejOutput = false;

  float bdt = mvaidtrigEle[eleIndex];

  float abseta=-100.;
  Utils anaUtils;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[eleIndex], isEcalDriven);
  if(ecaldriven) {
    int sc = superClusterIndexEle[eleIndex];
    abseta=fabs(etaSC[sc]);
  } else {
    int sc = PFsuperClusterIndexEle[eleIndex];
    abseta=fabs(etaPFSC[sc]);
  }

  // calculate the PU subtracted isolation
  float iso = corrEleIso2012(eleIndex);
//   ElectronEffectiveArea::ElectronEffectiveAreaTarget effAreaTarget_ = ElectronEffectiveArea::kEleEAData2012;
//   ElectronEffectiveArea::ElectronEffectiveAreaType effAreaGamma_   = ElectronEffectiveArea::kEleGammaIso04;
//   ElectronEffectiveArea::ElectronEffectiveAreaType effAreaNeutralHad_ = ElectronEffectiveArea::kEleNeutralHadronIso04;
//   ElectronEffectiveArea::ElectronEffectiveAreaType effAreaGammaAndNeutralHad_ = ElectronEffectiveArea::kEleGammaAndNeutralHadronIso04;

//   float eff_area_ga  = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaGamma_, abseta, effAreaTarget_);
//   float eff_area_nh  = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaNeutralHad_, abseta, effAreaTarget_);
//   float eff_area_ganh = ElectronEffectiveArea::GetElectronEffectiveArea(effAreaGammaAndNeutralHad_, abseta, effAreaTarget_);

//   float iso = pfCandChargedIso04Ele[eleIndex];
//   //  iso += max<float>(0.,pfCandNeutralIso04Ele[eleIndex]-eff_area_nh*rhoJetsFastJet + pfCandPhotonIso04Ele[eleIndex]-eff_area_ga*rhoJetsFastJet);
//   iso += max<float>(0.,pfCandNeutralIso04Ele[eleIndex]+pfCandPhotonIso04Ele[eleIndex]-eff_area_ganh*rhoJetsFastJet);

  float pt = GetPt(pxEle[eleIndex],pyEle[eleIndex]);

  bool ELE_ID_EGAMMA_2012 = (
                             ( pt <= 20 && abseta >= 0.000 && abseta < 0.800 && bdt > 0.00 ) ||
                             ( pt <= 20 && abseta >= 0.800 && abseta < 1.479 && bdt > 0.10 ) ||
                             ( pt <= 20 && abseta >= 1.479 && abseta < 2.500 && bdt > 0.62 ) ||
                             ( pt >  20 && abseta >= 0.000 && abseta < 0.800 && bdt > 0.94 ) ||
                             ( pt >  20 && abseta >= 0.800 && abseta < 1.479 && bdt > 0.85 ) ||
                             ( pt >  20 && abseta >= 1.479 && abseta < 2.500 && bdt > 0.92 )
                             );
  bool ELE_ISO_EGAMMA_2012 = (iso/pt < 0.15);

  int gsf = gsfTrackIndexEle[eleIndex];
  bool ELE_CONV_2012 = (!hasMatchedConversionEle[eleIndex] && expInnerLayersGsfTrack[gsf]==0);

  *eleIdOutput = ELE_ID_EGAMMA_2012;
  *isolOutput = ELE_ISO_EGAMMA_2012;
  *convRejOutput = ELE_CONV_2012;
}

void Higgs::isEleIDAndDenom(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput, CutBasedEleIDSelector *thisCutBasedID, bool applyBDTIdNotCutbased) {

  bool tightId, tightIso, tightConvRej;
  tightId = tightIso = tightConvRej = true;
  isEleID(eleIndex,&tightId,&tightIso,&tightConvRej,thisCutBasedID,applyBDTIdNotCutbased);

  bool denomId, denomIso;
  denomId = denomIso = true;
  bool fullDenom = isEleDenomFake(eleIndex,&denomId,&denomIso);

  *eleIdOutput = (tightId && denomId && denomIso); // apply the full denom on top of the ID to be consistent with the others for synch reasons
  *isolOutput = (tightIso);
  *convRejOutput = tightConvRej; // num and denom conv rej are the same

}

void Higgs::isEleID2012AndDenom(int eleIndex, bool *eleIdOutput, bool *isolOutput, bool *convRejOutput) {

  bool tightId, tightIso, tightConvRej;
  tightId = tightIso = tightConvRej = true;
  isEleID2012(eleIndex,&tightId,&tightIso,&tightConvRej);

  bool denomId, denomIso;
  denomId = denomIso = true;
  bool fullDenom = isEleDenomFake(eleIndex,&denomId,&denomIso);

  *eleIdOutput = (tightId && denomId && denomIso); // apply the full denom on top of the ID to be consistent with the others for synch reasons
  *isolOutput = (tightIso);
  *convRejOutput = tightConvRej; // num and denom conv rej are the same

}

// dxy parameter with respect to PV for tracks
double Higgs::trackDxyPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom) {
  return ( - (trackVPos.X()-PVPos.X())*trackMom.Y() + (trackVPos.Y()-PVPos.Y())*trackMom.X() ) / trackMom.Pt(); 
}

/// dz parameter with respect to PV for tracks
double Higgs::trackDzPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom) {
  float trackPt = trackMom.Pt();
  return (trackVPos.Z()-PVPos.Z()) - ((trackVPos.X()-PVPos.X())*trackMom.X()+(trackVPos.Y()-PVPos.Y())*trackMom.Y())/trackPt *trackMom.Pz()/trackPt; 
}

/// dsz parameter with respect to PV for tracks
double Higgs::trackDszPV(TVector3 PVPos, TVector3 trackVPos, TVector3 trackMom) {
  float trackPt = trackMom.Pt();
  float trackP  = trackMom.Mag();
  return (trackVPos.Z()-PVPos.Z())*trackPt/trackP - ((trackVPos.X()-PVPos.X())*trackMom.X()+(trackVPos.Y()-PVPos.Y())*trackMom.Y())/trackPt *trackMom.Pz()/trackP; 
}

/// dxy, dz and dsz parameters with respect to PV for electrons
double Higgs::eleDxyPV(int iele, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int gsfTrack = gsfTrackIndexEle[iele];
  TVector3 lepVPos(trackVxGsfTrack[gsfTrack],trackVyGsfTrack[gsfTrack],trackVzGsfTrack[gsfTrack]);
  TVector3 lepMom(pxEle[iele],pyEle[iele],pzEle[iele]);
  return trackDxyPV(PVPos,lepVPos,lepMom);
}

double Higgs::eleDzPV(int iele, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int gsfTrack = gsfTrackIndexEle[iele];
  TVector3 lepVPos(trackVxGsfTrack[gsfTrack],trackVyGsfTrack[gsfTrack],trackVzGsfTrack[gsfTrack]);
  TVector3 lepMom(pxEle[iele],pyEle[iele],pzEle[iele]);
  return trackDzPV(PVPos,lepVPos,lepMom);
}

double Higgs::eleDszPV(int iele, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int gsfTrack = gsfTrackIndexEle[iele];
  TVector3 lepVPos(trackVxGsfTrack[gsfTrack],trackVyGsfTrack[gsfTrack],trackVzGsfTrack[gsfTrack]);
  TVector3 lepMom(pxEle[iele],pyEle[iele],pzEle[iele]);
  return trackDszPV(PVPos,lepVPos,lepMom);
}

/// dxy, dz and dsz parameters with respect to PV for muons
double Higgs::muonDxyPV(int imu, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int ctfMuon   = trackIndexMuon[imu];
  TVector3 lepVPos(trackVxTrack[ctfMuon],trackVyTrack[ctfMuon],trackVzTrack[ctfMuon]);
  TVector3 lepMom(pxMuon[imu],pyMuon[imu],pzMuon[imu]);
  return trackDxyPV(PVPos,lepVPos,lepMom);
}

double Higgs::muonDzPV(int imu, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int ctfMuon   = trackIndexMuon[imu];
  TVector3 lepVPos(trackVxTrack[ctfMuon],trackVyTrack[ctfMuon],trackVzTrack[ctfMuon]);
  TVector3 lepMom(pxMuon[imu],pyMuon[imu],pzMuon[imu]);
  return trackDzPV(PVPos,lepVPos,lepMom);
}

double Higgs::muonDszPV(int imu, int iPV) {
  TVector3 PVPos(PVxPV[iPV],PVyPV[iPV],PVzPV[iPV]);
  int ctfMuon   = trackIndexMuon[imu];
  TVector3 lepVPos(trackVxTrack[ctfMuon],trackVyTrack[ctfMuon],trackVzTrack[ctfMuon]);
  TVector3 lepMom(pxMuon[imu],pyMuon[imu],pzMuon[imu]);
  return trackDszPV(PVPos,lepVPos,lepMom);
}

int Higgs::eleChargeMajority(int iele) {
  int gsfTrk = gsfTrackIndexEle[iele];
  int gsfCharge = chargeGsfTrack[gsfTrk];
  int pixelsCharge = scPixChargeEle[iele];

  int ctfTrk = trackIndexEle[iele];
  if(ctfTrk<0) return (gsfCharge==pixelsCharge);

  int ctfCharge = chargeTrack[ctfTrk];
  return ((ctfCharge==pixelsCharge) && (ctfCharge==gsfCharge));
}

float Higgs::eleBDT(ElectronIDMVA *mva, int eleIndex) {
  
  if(mva==0) {
    std::cout << "electron BDT not created/initialized. BIG PROBLEM. Returning false output -999!" << std::endl; 
    return -999.;
  }
  
  int gsfTrack = gsfTrackIndexEle[eleIndex]; 
  double gsfsign   = (-eleDxyPV(eleIndex,0) >=0 ) ? 1. : -1.;

  float ElePt = GetPt(pxEle[eleIndex],pyEle[eleIndex]);

  float EleDEtaIn = deltaEtaAtVtxEle[eleIndex];
  float EleDPhiIn = deltaPhiAtVtxEle[eleIndex];
  float EleHoverE = hOverEEle[eleIndex];
  float EleD0 = gsfsign * transvImpactParGsfTrack[gsfTrack];
  float EleFBrem = fbremEle[eleIndex];
  float EleEOverP = eSuperClusterOverPEle[eleIndex];
  float EleESeedClusterOverPout = eSeedOverPoutEle[eleIndex];
  float EleNBrem = nbremsEle[eleIndex];
  TVector3 pInGsf(pxGsfTrack[gsfTrack],pyGsfTrack[gsfTrack],pzGsfTrack[gsfTrack]);

  float EleIP3d = gsfsign * impactPar3DGsfTrack[gsfTrack];
  float EleIP3dSig = EleIP3d/impactPar3DErrorGsfTrack[gsfTrack];

  // we have not pout and seed cluster energy in the trees. Gymnastics...
  float Pout = pInGsf.Mag() - fbremEle[eleIndex] * pInGsf.Mag();
  float SCSeedEnergy = EleESeedClusterOverPout * Pout;
  float EleESeedClusterOverPIn = SCSeedEnergy/pInGsf.Mag();      

  float EleSigmaIEtaIEta, EleSigmaIPhiIPhi, EleOneOverEMinusOneOverP, EleSCEta;

  Utils anaUtils;
  bool ecaldriven = anaUtils.electronRecoType(recoFlagsEle[eleIndex], isEcalDriven);
  if(ecaldriven) {
    int sc = superClusterIndexEle[eleIndex];
    EleSigmaIEtaIEta = sqrt(covIEtaIEtaSC[sc]);
    EleSigmaIPhiIPhi = sqrt(covIPhiIPhiSC[sc]);
    EleOneOverEMinusOneOverP = 1./energySC[sc]  - 1./pInGsf.Mag();
    EleSCEta = etaSC[sc];
  } else {
    int sc = PFsuperClusterIndexEle[eleIndex];
    if(sc>-1) {
      EleSigmaIEtaIEta = sqrt(covIEtaIEtaPFSC[sc]);
      EleSigmaIPhiIPhi = sqrt(covIPhiIPhiPFSC[sc]);
      EleOneOverEMinusOneOverP = 1./energyPFSC[sc]  - 1./pInGsf.Mag();
      EleSCEta = etaPFSC[sc];
    } else {
      EleSigmaIEtaIEta = 999.;
      EleSigmaIPhiIPhi = 999.;
      EleOneOverEMinusOneOverP = 999.;
      EleESeedClusterOverPIn = 999.;
      EleSCEta = 0.;
    }
  }

  return mva->MVAValue(ElePt, EleSCEta,
                       EleSigmaIEtaIEta,
                       EleDEtaIn,
                       EleDPhiIn,
                       EleHoverE,
                       EleD0,
                       EleFBrem,
                       EleEOverP,
                       EleESeedClusterOverPout,
                       EleSigmaIPhiIPhi,
                       EleNBrem,
                       EleOneOverEMinusOneOverP,
                       EleESeedClusterOverPIn,
                       EleIP3d,
                       EleIP3dSig );

}

bool Higgs::passEleBDT(float pt, float eta, float bdt) {

  if(pt < 20 && fabs(eta) < 1.0) return (bdt > 0.139);
  if(pt < 20 && fabs(eta) >= 1.0 && fabs(eta) < 1.479) return (bdt > 0.525);
  if(pt < 20 && fabs(eta) >= 1.479 && fabs(eta) < 2.500) return (bdt > 0.543);
  if(pt >= 20 && fabs(eta) < 1.0) return (bdt > 0.947);
  if(pt >= 20 && fabs(eta) >= 1.0 && fabs(eta) < 1.479) return (bdt > 0.950);
  if(pt >= 20 && fabs(eta) >= 1.479 && fabs(eta) < 2.500) return (bdt > 0.884);

  // here we are cutting the events with |SC eta|>2.5. If the acceptance is done with |ele eta|<2.5 then will cut some event more. Fine. Synch with this.
  return false;

}

bool Higgs::goodPV(int v) {
  bool isGoodVertex = true;
  if (nPV<1) isGoodVertex = false;
  float rhoVtx = sqrt(PVxPV[v]*PVxPV[v] + PVyPV[v]*PVyPV[v]);
  if ( isFakePV[v] )       isGoodVertex = false;
  if ( ndofPV[v]<=4 )      isGoodVertex = false;
  if ( fabs(PVzPV[v])>24.) isGoodVertex = false;
  if ( rhoVtx>2 )          isGoodVertex = false; 
  return isGoodVertex;
}
