#include <RooArgList.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooGenericPdf.h>
#include <RooPlot.h>
#include <RooRealVar.h>
#include <RooFormulaVar.h>
#include <RooWorkspace.h>
#include <RooGaussian.h>
#include <RooLandau.h>
#include <RooBreitWigner.h>
#include <RooCBShape.h>
#include <RooFFTConvPdf.h>
#include <RooProdPdf.h>
#include <RooAddPdf.h>
#include <RooHistFunc.h>
#include "HiggsAnalysis/CombinedLimit/interface/HZZ4LRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/VerticalInterpHistPdf.h"

#include "YieldMaker.h"
#include "CardTemplate.h"
#include "findAndReplace.h"
#include "yields.h"
#include "XSecProvider.h"
#include "FitSelection.hh"
#include "ConfigParser.hh"
#include "PdfSystematics.hh"

using namespace RooFit;
using namespace std;

class HiggsMassPointInfo {

public:
  HiggsMassPointInfo() {}
  ~HiggsMassPointInfo() {}

  float lumi;
  bool  do1D;
  bool  do7TeV;
  bool  doFFT;
  std::string treeFolder;
  std::string hww2DShapesfilename;

  DataYieldMaker  ymaker_data;
  YieldMaker      ymaker_hi;
  YieldMaker      ymaker_qqww;
  YieldMaker      ymaker_ggww;
  YieldMaker      ymaker_top;
  YieldMaker      ymaker_dysf;
  YieldMaker      ymaker_dyof;
  YieldMaker      ymaker_wgstar;
  YieldMaker      ymaker_others;
  WJetsYieldMaker ymaker_wj;
  XSecProvider xsecProvider;
  FitSelection sel;
  
  int getClosestMCMass(float mH) {
    int masses[10];
    int nMasses;
    if(do7TeV) {
      masses[0]=120; masses[1]=130; masses[2]=140; 
      masses[3]=150; masses[4]=160; masses[5]=170;
      masses[6]=180;
      nMasses=7;
    } else {
      masses[0]=115; masses[1]=120; masses[2]=125; 
      masses[3]=130; masses[4]=135; masses[5]=140;
      masses[6]=150; masses[7]=160; masses[8]=170;
      masses[9]=180; 
      nMasses=10;
    }
    
    if(mH<masses[0]) return masses[0];
    if(mH>=masses[nMasses-1]) return masses[nMasses-1];
    for(int i=0;i<nMasses-1;++i) {
      if(mH>=masses[i] && mH<masses[i+1]) {
        float sx=mH-masses[i];
        float dx=masses[i+1]-mH;
        return (sx<=dx) ? masses[i] : masses[i+1];
      }
    }
    return -999;
  }
  
  std::string getSignalCBMeanString(int ch) {
    HWWSystematics syst("sig",1);
    stringstream fss;
    fss << "( ";  
    if (!doFFT) fss << "@0 + ";
    if (ch == of0j) fss << (do7TeV ? "(23.5003) + (-0.229111*@0)" : "(27.252) + (-0.245363*@0)");
    if (ch == of1j) fss << (do7TeV ? "(14.9959) + (-0.162208*@0)" : "(28.1494) + (-0.246974*@0)");
    if (ch == sf0j) fss << (do7TeV ? "(27.3141) + (-0.241242*@0)" : "(32.2168) + (-0.265605*@0)");
    if (ch == sf1j) fss << (do7TeV ? "(25.3767) + (-0.223649*@0)" : "(32.6065) + (-0.266415*@0)");
    fss << " )";
    fss << syst.getFormulaSyst();
    return fss.str();
  }

  std::string getSignalCBSigmaString(int ch) {
    HWWSystematics syst("sig",1);
    stringstream fss;
    fss << "( ";  
    if (ch == of0j) fss << (do7TeV ? "( 2.52826) + (0.141593*@0)" : "(-1.08658) + (0.168527*@0)");
    if (ch == of1j) fss << (do7TeV ? "(-11.0486) + (0.238924*@0)" : "(-1.78201) + (0.186343*@0)");
    if (ch == sf0j) fss << (do7TeV ? "( 3.05721) + (0.115459*@0)" : "(-9.03439) + (0.213851*@0)");
    if (ch == sf1j) fss << (do7TeV ? "(-5.24940) + (0.189442*@0)" : "(3.73474) + (0.129752*@0)");
    fss << " )";
    fss << syst.getFormulaSyst();
    return fss.str();
  }

  std::string getSignalCBAlphaString(int ch) {
    HWWSystematics syst("sig",1);
    stringstream fss;
    if (ch == of0j) fss << (do7TeV ? "(1.11326)" : "(4.63328)");
    if (ch == of1j) fss << (do7TeV ? "(1.05557)" : "(5.49636)");
    if (ch == sf0j) fss << (do7TeV ? "(1.26936)" : "(2.13613)");
    if (ch == sf1j) fss << (do7TeV ? "(4.98803)" : "(4.21967)");
    fss << syst.getFormulaSyst();
    return fss.str();
  }

  std::string getSignalCBNString(int ch) {
    HWWSystematics syst("sig",1);
    stringstream fss;
    if (ch == of0j) fss << "5";
    if (ch == of1j) fss << "5";
    if (ch == sf0j) fss << "5";
    if (ch == sf1j) fss << "5";
    fss << syst.getFormulaSyst();
    return fss.str();
  }

  void createCard(float mass, float mrMin, float mrMax, int ch) {

    stringstream configfile;
    if(do7TeV) configfile << "config/mrfit_of_7TeV.txt";
    else configfile << "config/mrfit_of_8TeV.txt";
    ConfigParser cp(configfile.str().c_str());
    stringstream sigfilename;
    sigfilename << "config/sigParamsHWW_mH" << getClosestMCMass(mass) << (do7TeV ? "_7TeV" : "_8TeV") << ".txt";
    ConfigParser sigcp(sigfilename.str());

    std::string tevstr = do7TeV ? "_7TeV" : "_8TeV"; 

    std::string chstr;
    if (ch == of0j) chstr = "of_0j";
    if (ch == of1j) chstr = "of_1j";
    if (ch == sf0j) chstr = "sf_0j";
    if (ch == sf1j) chstr = "sf_1j";

    stringstream mass_str_ss;
    mass_str_ss << mass;
    std::string mass_str = mass_str_ss.str();
        
    std::cout << "Creating datacard for " << mass_str << " GeV mass point and channel " << chstr << " ... " << std::endl;
    
    stringstream card_name;
    card_name << "hww-" << lumi << "fb.mH" << mass_str << "." << chstr << "_shape";
    if(!do1D) card_name << "_2D"; 
    card_name << tevstr;
    std::string workspace = card_name.str()+"_workspace.root";

    ScaleFactors sf(ch,do7TeV);

    float yield_data   = ymaker_data   .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax);
    float yield_qqww   = ymaker_qqww   .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * sf.getWW() * lumi;
    float yield_ggww   = ymaker_ggww   .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * sf.getWW() * lumi;
    float yield_top    = ymaker_top    .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * sf.getTop() * lumi;
    float yield_dy     = 0.0; 
    if(ch==of0j || ch==of1j) yield_dy = ymaker_dyof .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax);
    else                     yield_dy = ymaker_dysf .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * sf.getDY() * lumi;
    float yield_wgstar = ymaker_wgstar .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * lumi;
    float yield_others = ymaker_others .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax) * lumi;
    float yield_wj     = ymaker_wj     .getYield(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax);

    std::string card   = createCardTemplate(do7TeV, ch, do1D, workspace.c_str());

    std::string binname;
    if (ch == of0j) binname = "of_0j";
    if (ch == of1j) binname = "of_1j";
    if (ch == sf0j) binname = "sf_0j";
    if (ch == sf1j) binname = "sf_1j";

    int jet = (ch == of0j || ch == sf0j) ? 0 : 1;
    if(do7TeV) {
      card = findAndReplace(card, "GGH_PDF"         , xsecProvider.get7TeVggHPDFDown(mass),            xsecProvider.get7TeVggHPDFUp(mass));
      card = findAndReplace(card, "VBF_PDF"         , xsecProvider.get7TeVVBFPDFDown(mass),            xsecProvider.get7TeVVBFPDFUp(mass));
      card = findAndReplace(card, "GGH_QCD"         , xsecProvider.get7TeVggHExclQCDDown(mass),        xsecProvider.get7TeVggHExclQCDUp(mass));
      card = findAndReplace(card, "GGH1_QCD"        , xsecProvider.get7TeVggH1inExclQCDDown(mass,jet), xsecProvider.get7TeVggH1inExclQCDUp(mass,jet));
      card = findAndReplace(card, "GGH2_QCD"        , xsecProvider.get7TeVggH2inExclQCDDown(mass,jet), xsecProvider.get7TeVggH2inExclQCDUp(mass,jet));
      card = findAndReplace(card, "VBF_QCD"         , xsecProvider.get7TeVVBFQCDDown(mass),            xsecProvider.get7TeVVBFQCDUp(mass));
    } else {
      card = findAndReplace(card, "GGH_PDF"         , xsecProvider.get8TeVggHPDFDown(mass),            xsecProvider.get8TeVggHPDFUp(mass));
      card = findAndReplace(card, "VBF_PDF"         , xsecProvider.get8TeVVBFPDFDown(mass),            xsecProvider.get8TeVVBFPDFUp(mass));
      card = findAndReplace(card, "GGH_QCD"         , xsecProvider.get8TeVggHExclQCDDown(mass),        xsecProvider.get8TeVggHExclQCDUp(mass));
      card = findAndReplace(card, "GGH1_QCD"        , xsecProvider.get8TeVggH1inExclQCDDown(mass,jet), xsecProvider.get8TeVggH1inExclQCDUp(mass,jet));
      card = findAndReplace(card, "GGH2_QCD"        , xsecProvider.get8TeVggH2inExclQCDDown(mass,jet), xsecProvider.get8TeVggH2inExclQCDUp(mass,jet));
      card = findAndReplace(card, "VBF_QCD"         , xsecProvider.get8TeVVBFQCDDown(mass),            xsecProvider.get8TeVVBFQCDUp(mass));
    }
    card = findAndReplace(card, "SIG_GGH_YIELD"   , 1);
    card = findAndReplace(card, "SIG_VBF_YIELD"   , 1);
    card = findAndReplace(card, "SIG_WZTT_YIELD"  , 1);
    card = findAndReplace(card, "BKG_qqWW_YIELD"  , yield_qqww);
    card = findAndReplace(card, "BKG_ggWW_YIELD"  , yield_ggww);
    card = findAndReplace(card, "BKG_TOP_YIELD"   , yield_top);
    card = findAndReplace(card, "BKG_DY_YIELD"    , yield_dy);
    card = findAndReplace(card, "BKG_WGSTAR_YIELD", yield_wgstar);
    card = findAndReplace(card, "BKG_OTHERS_YIELD", yield_others);
    card = findAndReplace(card, "BKG_WJETS_YIELD" , yield_wj);
    card = findAndReplace(card, "BIN"             , binname);
    card = findAndReplace(card, "OBS"             , yield_data);

    DataDrivenStatErrors ddGmN(ch,do7TeV);
    stringstream GmNchstr;
    GmNchstr << (ch==of0j ? "0J" : "1J") << (do7TeV ? "7T" : "8T");
    card = findAndReplace(card, ("NWW"+GmNchstr.str())  , ddGmN.getWW_stat());
    card = findAndReplace(card, ("QQWW"+GmNchstr.str()) , yield_qqww/ddGmN.getWW_stat());
    card = findAndReplace(card, ("GGWW"+GmNchstr.str()) , yield_ggww/ddGmN.getWW_stat());
    card = findAndReplace(card, ("NTO"+GmNchstr.str())  , ddGmN.getTop_stat());
    card = findAndReplace(card, ("TOP"+GmNchstr.str())  , yield_top/ddGmN.getTop_stat());

    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_res_met"),          sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","res_met"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_res_met"),         sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","res_met"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_res_met"),         sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","res_met"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_res_met"),             sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","res_met"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_res_e"),            sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","res_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_res_e"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","res_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_res_e"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","res_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_res_e"),               sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","res_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaleup_e"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaleup_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaleup_e"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaleup_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaleup_e"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaleup_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaleup_e"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaleup_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaledn_e"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaledn_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaledn_e"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaledn_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaledn_e"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaledn_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaledn_e"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaledn_e"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaleup_m"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaleup_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaleup_m"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaleup_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaleup_m"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaleup_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaleup_m"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaleup_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaledn_m"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaledn_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaledn_m"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaledn_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaledn_m"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaledn_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaledn_m"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaledn_m"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaleup_j"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaleup_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaleup_j"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaleup_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaleup_j"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaleup_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaleup_j"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaleup_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_mean_err_scaledn_j"),        sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","me","scaledn_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_sigma_err_scaledn_j"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","si","scaledn_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_alpha_err_scaledn_j"),       sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","alpha","scaledn_j"));
    card = sigcp.updateCardShapeUncertainty(card, ("SIG_"+chstr+tevstr+"_n_err_scaledn_j"),           sigcp.getRelUncertainty(getStringFitChannel(ch),"sig","n","scaledn_j"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_MC"),             cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","qqWWMCatNLONom","me","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_MC"),            cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","qqWWMCatNLONom","si","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaleup_qcd"),    cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMCatNLONom","qqWWMCatNLOUp","me","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaleup_qcd"),   cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMCatNLONom","qqWWMCatNLOUp","si","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaledn_qcd"),    cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMCatNLONom","qqWWMCatNLODown","me","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaledn_qcd"),   cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMCatNLONom","qqWWMCatNLODown","si","nominals","nominals"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_res_met"),        cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_res_met"),       cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_res_e"),          cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_res_e"),         cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaleup_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaleup_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaledn_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaledn_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaleup_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaleup_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaledn_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaledn_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaleup_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaleup_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_mean_err_scaledn_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","me","scaledn_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_QQWW_"+chstr+tevstr+"_sigma_err_scaledn_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"qqWWMadgraph","si","scaledn_j"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_res_met"),        cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_res_met"),       cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_res_e"),          cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_res_e"),         cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaleup_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaleup_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaledn_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaledn_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaleup_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaleup_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaledn_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaledn_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaleup_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaleup_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_mean_err_scaledn_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","me","scaledn_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_GGWW_"+chstr+tevstr+"_sigma_err_scaledn_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"ggWW","si","scaledn_j"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_res_met"),        cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_res_met"),       cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_res_e"),          cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_res_e"),         cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaleup_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaleup_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaledn_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaledn_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaleup_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaleup_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaledn_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaledn_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaleup_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaleup_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_mean_err_scaledn_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"Top","me","scaledn_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_TOP_"+chstr+tevstr+"_sigma_err_scaledn_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"Top","si","scaledn_j"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_WJ_"+chstr+tevstr+"_mean_err_fakerateup"),       cp.getRelUncertainty(getStringFitChannel(ch),"wjets","me","fakerateup"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WJ_"+chstr+tevstr+"_sigma_err_fakerateup"),      cp.getRelUncertainty(getStringFitChannel(ch),"wjets","si","fakerateup"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WJ_"+chstr+tevstr+"_mean_err_fakeratedn"),       cp.getRelUncertainty(getStringFitChannel(ch),"wjets","me","fakeratedn"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WJ_"+chstr+tevstr+"_sigma_err_fakeratedn"),      cp.getRelUncertainty(getStringFitChannel(ch),"wjets","si","fakeratedn"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_res_met"),        cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_res_met"),       cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_res_e"),          cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_res_e"),         cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaleup_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaleup_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaledn_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaledn_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaleup_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaleup_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaledn_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaledn_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaleup_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaleup_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_mean_err_scaledn_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","me","scaledn_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_WGSTAR_"+chstr+tevstr+"_sigma_err_scaledn_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"WGstar","si","scaledn_j"));

    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_res_met"),        cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_res_met"),       cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","res_met"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_res_e"),          cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_res_e"),         cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","res_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaleup_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaleup_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaleup_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaledn_e"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaledn_e"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaledn_e"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaleup_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaleup_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaleup_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaledn_m"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaledn_m"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaledn_m"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaleup_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaleup_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaleup_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_mean_err_scaledn_j"),      cp.getRelUncertainty(getStringFitChannel(ch),"Ot","me","scaledn_j"));
    card = cp.updateCardShapeUncertainty(card, ("BKG_OTHERS_"+chstr+tevstr+"_sigma_err_scaledn_j"),     cp.getRelUncertainty(getStringFitChannel(ch),"Ot","si","scaledn_j"));

    ofstream file;
    file.open ((card_name.str() +".txt").c_str());
    file << card;
    file.close();
    
    RooWorkspace w("w", "");
    
    RooRealVar CMS_ww2l_dphi ("CMS_ww2l_dphi" , "#Delta #Phi" ,   0,       TMath::Pi(), "");
    RooRealVar CMS_ww2l_mr_1D("CMS_ww2l_mr_1D", "M_{R}",          mrMin,   mrMax,       "GeV/c^{2}");
    RooRealVar weight("weight","weight",1.0,-1000.,1000.);
    if (doFFT) CMS_ww2l_mr_1D.setBins(100000, "fft");
        
    if (do1D) {
      RooArgSet argset_obs(CMS_ww2l_mr_1D, "argset_obs");
      RooDataSet data_obs("data_obs", "data_obs", argset_obs);
      
      ymaker_data.getDataSet1D(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, data_obs, CMS_ww2l_mr_1D, weight);
    
      w.import(data_obs);
    }

    else {
      RooArgSet argset_obs(CMS_ww2l_mr_1D, CMS_ww2l_dphi, "argset_obs");
      RooDataSet data_obs("data_obs", "data_obs", argset_obs);
            
      ymaker_data.getDataSet2D(ch, mrMin, mrMax, sel.dphimin, sel.dphimax, sel.mtmin, sel.mtmax, sel.ptllmin, sel.ptllmax, data_obs, CMS_ww2l_mr_1D, CMS_ww2l_dphi, weight);

      w.import(data_obs);
    }
    

    ///////////////////// Define parameters //////////////////////////////////

    float qqWWme = 0.;
    float qqWWsi = 0.;
    float ggWWme = 0.;
    float ggWWsi = 0.;
    float Topme = 0.;
    float Topsi = 0.;
    float DYofme1 = 0.;
    float DYofsi1 = 0.;
    float DYofme2 = 0.;
    float DYofsi2 = 0.;
    float DYoffrac = 0.;
    float DYsfa0  = 0.;
    float DYsfa1  = 0.;
    float DYsfa2  = 0.;
    float DYsfa3  = 0.;
    float DYsfa4  = 0.;
    float DYsfa5  = 0.;
    float DYsfa6  = 0.;
    float DYsfa7  = 0.;
    float DYsfa8  = 0.;
    float DYsfa9  = 0.;
    float DYsfa10 = 0.;
    float DYsfa11 = 0.;
    float DYsfa12 = 0.;
    float DYsfa13 = 0.;
    float WJme    = 0.;
    float WJsi    = 0.;
    float WGsme   = 0.;
    float WGssi   = 0.;
    float Otofme  = 0.;
    float Otofsi  = 0.;
    float Otsfa0  = 0.;
    float Otsfa1  = 0.;
    float Otsfa2  = 0.;
    float Otsfa3  = 0.;
    float Otsfa4  = 0.;
    float Otsfa5  = 0.;
    float Otsfa6  = 0.;
    float Otsfa7  = 0.;
    float Otsfa8  = 0.;
    float Otsfa9  = 0.;
    float Otsfa10 = 0.;
    float Otsfa11 = 0.;
    float Otsfa12 = 0.;
    float Otsfa13 = 0.;

    if(ch == of0j || ch == of1j) {
      qqWWme = cp.getLandau(getStringFitChannel(ch),"qqWWMadgraph","nominals")[0];
      qqWWsi = cp.getLandau(getStringFitChannel(ch),"qqWWMadgraph","nominals")[1];
      
      ggWWme = cp.getLandau(getStringFitChannel(ch),"ggWW","nominals")[0];
      ggWWsi = cp.getLandau(getStringFitChannel(ch),"ggWW","nominals")[1];
      
      Topme = cp.getLandau(getStringFitChannel(ch),"Top","nominals")[0];
      Topsi = cp.getLandau(getStringFitChannel(ch),"Top","nominals")[1];
      
      DYofme1 = cp.getDoubleGaussian("of0j","embeddedtt","nominals")[0];
      DYofsi1 = cp.getDoubleGaussian("of0j","embeddedtt","nominals")[1];
      DYofme2 = cp.getDoubleGaussian("of0j","embeddedtt","nominals")[2];
      DYofsi2 = cp.getDoubleGaussian("of0j","embeddedtt","nominals")[3];
      DYoffrac = cp.getDoubleGaussian("of0j","embeddedtt","nominals")[4];
      
      WJme = cp.getLandau(getStringFitChannel(ch),"wjets","nominals")[0];
      WJsi = cp.getLandau(getStringFitChannel(ch),"wjets","nominals")[1]; 

      WGsme = cp.getLandau(getStringFitChannel(ch),"WGstar","nominals")[0];
      WGssi = cp.getLandau(getStringFitChannel(ch),"WGstar","nominals")[1];
      
      Otofme = cp.getLandau(getStringFitChannel(ch),"Ot","nominals")[0];
      Otofsi = cp.getLandau(getStringFitChannel(ch),"Ot","nominals")[1];
    }

    else if(ch == sf0j) {
      // not updated !!!
      qqWWme = 166.184;
      qqWWsi = 31.4472;

      ggWWme = 165.034;
      ggWWsi = 26.5768;

      Topme = 198.501;
      Topsi = 43.3945;

      DYsfa0  = 84.7481;
      DYsfa1  = 13.0831;
      DYsfa2  = 56.1839;
      DYsfa3  = 0.0362868;
      DYsfa4  = 200.527;
      DYsfa5  = 17.7071;
      DYsfa6  = 13.9596;
      DYsfa7  = 0.0518413;
      DYsfa8  = 5.53392;
      DYsfa9  = 0.91199;
      DYsfa10  = 107.116;
      DYsfa11  = -8.36242;
      DYsfa12  = 4064.28;
      DYsfa13  = 0.00377819;

      WJme = 126.356;
      WJsi = 20.1992;

      Otsfa0  = 87.1441;
      Otsfa1  = 8.19755;
      Otsfa2  = 58.9242;
      Otsfa3  = 0.0224312;
      Otsfa4  = 207.433;
      Otsfa5  = 19.2934;
      Otsfa6  = 11.915;
      Otsfa7  = 0.166626;
      Otsfa8  = 65.7923;
      Otsfa9  = 0.0639726;
      Otsfa10  = 122.923;
      Otsfa11  = 10.95;
      Otsfa12  = 932.008;
      Otsfa13  = 1.43236e-08;
    }

    else if(ch == sf1j) {
      // not updated!!!
      qqWWme = 173.467;
      qqWWsi = 37.7352;
      
      ggWWme = 168.458;
      ggWWsi = 30.4483;

      Topme = 185.053;
      Topsi = 40.3746;

      DYsfa0  = 79.0588;
      DYsfa1  = 30.0091;
      DYsfa2  = 84.2549;
      DYsfa3  = 0.0155325;
      DYsfa4  = 197.92;
      DYsfa5  = 18.9961;
      DYsfa6  = 24.912;
      DYsfa7  = 0.098307;
      DYsfa8  = 11.9076;
      DYsfa9  = 0.199545;
      DYsfa10  = 21.3897;
      DYsfa11  = -18.2451;
      DYsfa12  = 754.799;
      DYsfa13  = 0.0958732;

      WJme = 135.239;
      WJsi = 26.0509;

      Otsfa0  = 192.072;
      Otsfa1  = 10.1597;
      Otsfa2  = 56.8007;
      Otsfa3  = 0.701599;
      Otsfa4  = 228.752;
      Otsfa5  = 87.2529;
      Otsfa6  = 58.8586;
      Otsfa7  = 0.18201;
      Otsfa8  = 5.15327;
      Otsfa9  = 0.318942;
      Otsfa10  = 86.3412;
      Otsfa11  = 19.9938;
      Otsfa12  = 213.419;
      Otsfa13  = 0.0378662;
    }


    stringstream lumiss;
    lumiss << lumi;
    std::string lumistr = lumiss.str(); 

    WWSystematics qqwwsyst("bkg_qqww");
    RooArgList qqww_mean_al  = qqwwsyst.getParSystematics("mean",chstr,tevstr,qqWWme);
    RooArgList qqww_sigma_al = qqwwsyst.getParSystematics("sigma",chstr,tevstr,qqWWsi); 
    RooFormulaVar qqww_mean (("bkg_qqww_"+chstr+tevstr+"_mean" ).c_str(), qqwwsyst.getFormulaSyst().c_str(), qqww_mean_al);
    RooFormulaVar qqww_sigma(("bkg_qqww_"+chstr+tevstr+"_sigma").c_str(), qqwwsyst.getFormulaSyst().c_str(), qqww_sigma_al);

    WWSystematics ggwwsyst("bkg_ggww");
    RooArgList ggww_mean_al  = ggwwsyst.getParSystematics("mean",chstr,tevstr,ggWWme);
    RooArgList ggww_sigma_al = ggwwsyst.getParSystematics("sigma",chstr,tevstr,ggWWsi);
    RooFormulaVar ggww_mean (("bkg_ggww_"+chstr+tevstr+"_mean" ).c_str(), ggwwsyst.getFormulaSyst().c_str(), ggww_mean_al);
    RooFormulaVar ggww_sigma(("bkg_ggww_"+chstr+tevstr+"_sigma").c_str(), ggwwsyst.getFormulaSyst().c_str(), ggww_sigma_al);

    WWSystematics topsyst("bkg_top");
    RooArgList top_mean_al  = topsyst.getParSystematics("mean",chstr,tevstr,Topme);
    RooArgList top_sigma_al = topsyst.getParSystematics("sigma",chstr,tevstr,Topsi);
    RooFormulaVar top_mean (("bkg_top_"+chstr+tevstr+"_mean" ).c_str(), topsyst.getFormulaSyst().c_str(), top_mean_al);
    RooFormulaVar top_sigma(("bkg_top_"+chstr+tevstr+"_sigma").c_str(), topsyst.getFormulaSyst().c_str(), top_sigma_al);

    RooRealVar dyof_mean1 (("bkg_dy_"+chstr+tevstr+"_mean1" ).c_str(), "", DYofme1);
    RooRealVar dyof_sigma1(("bkg_dy_"+chstr+tevstr+"_sigma1").c_str(), "", DYofsi1);
    RooRealVar dyof_mean2 (("bkg_dy_"+chstr+tevstr+"_mean2" ).c_str(), "", DYofme2);
    RooRealVar dyof_sigma2(("bkg_dy_"+chstr+tevstr+"_sigma2").c_str(), "", DYofsi2);
    RooRealVar dyof_frac  (("bkg_dy_"+chstr+tevstr+"_frac").c_str(), "", DYoffrac);

    WJetsSystematics wjsyst("bkg_wj");
    RooArgList wj_mean_al  = wjsyst.getParSystematics("mean",chstr,tevstr,WJme);
    RooArgList wj_sigma_al = wjsyst.getParSystematics("sigma",chstr,tevstr,WJsi);    
    RooFormulaVar wj_mean (("bkg_wj_"+chstr+tevstr+"_mean" ).c_str(), wjsyst.getFormulaSyst().c_str(), wj_mean_al);
    RooFormulaVar wj_sigma(("bkg_wj_"+chstr+tevstr+"_sigma").c_str(), wjsyst.getFormulaSyst().c_str(), wj_sigma_al);

    WWSystematics wgstarsyst("bkg_wgstar");
    RooArgList wgstar_mean_al  = wgstarsyst.getParSystematics("mean",chstr,tevstr,WGsme);
    RooArgList wgstar_sigma_al = wgstarsyst.getParSystematics("sigma",chstr,tevstr,WGssi);    
    RooFormulaVar wgstar_mean (("bkg_wgstar_"+chstr+tevstr+"_mean" ).c_str(), wgstarsyst.getFormulaSyst().c_str(), wgstar_mean_al);
    RooFormulaVar wgstar_sigma(("bkg_wgstar_"+chstr+tevstr+"_sigma").c_str(), wgstarsyst.getFormulaSyst().c_str(), wgstar_sigma_al);

    WWSystematics otherssyst("bkg_others");
    RooArgList others_mean_al  = otherssyst.getParSystematics("mean",chstr,tevstr,Otofme);
    RooArgList others_sigma_al = otherssyst.getParSystematics("sigma",chstr,tevstr,Otofsi);    
    RooFormulaVar othersof_mean (("bkg_others_"+chstr+tevstr+"_mean" ).c_str(), otherssyst.getFormulaSyst().c_str(), others_mean_al);
    RooFormulaVar othersof_sigma(("bkg_others_"+chstr+tevstr+"_sigma").c_str(), otherssyst.getFormulaSyst().c_str(), others_sigma_al);

    RooRealVar dysf_a0 (("bkg_dy_"+chstr+tevstr+"_a0" ).c_str(), "", DYsfa0);
    RooRealVar dysf_a1 (("bkg_dy_"+chstr+tevstr+"_a1" ).c_str(), "", DYsfa1);
    RooRealVar dysf_a2 (("bkg_dy_"+chstr+tevstr+"_a2" ).c_str(), "", DYsfa2);
    RooRealVar dysf_a3 (("bkg_dy_"+chstr+tevstr+"_a3" ).c_str(), "", DYsfa3);
    RooRealVar dysf_a4 (("bkg_dy_"+chstr+tevstr+"_a4" ).c_str(), "", DYsfa4);
    RooRealVar dysf_a5 (("bkg_dy_"+chstr+tevstr+"_a5" ).c_str(), "", DYsfa5);
    RooRealVar dysf_a6 (("bkg_dy_"+chstr+tevstr+"_a6" ).c_str(), "", DYsfa6);
    RooRealVar dysf_a7 (("bkg_dy_"+chstr+tevstr+"_a7" ).c_str(), "", DYsfa7);
    RooRealVar dysf_a8 (("bkg_dy_"+chstr+tevstr+"_a8" ).c_str(), "", DYsfa8);
    RooRealVar dysf_a9 (("bkg_dy_"+chstr+tevstr+"_a9" ).c_str(), "", DYsfa9);
    RooRealVar dysf_a10 (("bkg_dy_"+chstr+tevstr+"_a10" ).c_str(), "", DYsfa10);
    RooRealVar dysf_a11 (("bkg_dy_"+chstr+tevstr+"_a11" ).c_str(), "", DYsfa11);
    RooRealVar dysf_a12 (("bkg_dy_"+chstr+tevstr+"_a12" ).c_str(), "", DYsfa12);
    RooRealVar dysf_a13 (("bkg_dy_"+chstr+tevstr+"_a13" ).c_str(), "", DYsfa13);
	
    RooRealVar otherssf_a0 (("bkg_others_"+chstr+tevstr+"_a0" ).c_str(), "", Otsfa0);
    RooRealVar otherssf_a1 (("bkg_others_"+chstr+tevstr+"_a1" ).c_str(), "", Otsfa1);
    RooRealVar otherssf_a2 (("bkg_others_"+chstr+tevstr+"_a2" ).c_str(), "", Otsfa2);
    RooRealVar otherssf_a3 (("bkg_others_"+chstr+tevstr+"_a3" ).c_str(), "", Otsfa3);
    RooRealVar otherssf_a4 (("bkg_others_"+chstr+tevstr+"_a4" ).c_str(), "", Otsfa4);
    RooRealVar otherssf_a5 (("bkg_others_"+chstr+tevstr+"_a5" ).c_str(), "", Otsfa5);
    RooRealVar otherssf_a6 (("bkg_others_"+chstr+tevstr+"_a6" ).c_str(), "", Otsfa6);
    RooRealVar otherssf_a7 (("bkg_others_"+chstr+tevstr+"_a7" ).c_str(), "", Otsfa7);
    RooRealVar otherssf_a8 (("bkg_others_"+chstr+tevstr+"_a8" ).c_str(), "", Otsfa8);
    RooRealVar otherssf_a9 (("bkg_others_"+chstr+tevstr+"_a9" ).c_str(), "", Otsfa9);
    RooRealVar otherssf_a10 (("bkg_others_"+chstr+tevstr+"_a10" ).c_str(), "", Otsfa10);
    RooRealVar otherssf_a11 (("bkg_others_"+chstr+tevstr+"_a11" ).c_str(), "", Otsfa11);
    RooRealVar otherssf_a12 (("bkg_others_"+chstr+tevstr+"_a12" ).c_str(), "", Otsfa12);
    RooRealVar otherssf_a13 (("bkg_others_"+chstr+tevstr+"_a13" ).c_str(), "", Otsfa13);

    RooRealVar masshiggs       ("MH", "", mass);

    HWWSystematics sigsyst("sig",mass);
    RooArgList sig_mean_al(masshiggs); sig_mean_al.add(sigsyst.getParSystematics("mean",chstr,tevstr));
    RooArgList sig_sigma_al(masshiggs); sig_sigma_al.add(sigsyst.getParSystematics("sigma",chstr,tevstr));
    RooArgList sig_alpha_al(masshiggs); sig_alpha_al.add(sigsyst.getParSystematics("alpha",chstr,tevstr));
    RooArgList sig_n_al(masshiggs); sig_n_al.add(sigsyst.getParSystematics("n",chstr,tevstr));

    InterpolatedYields ggHy(ch,ggH,do7TeV);
    InterpolatedYields vbfHy(ch,vbfH,do7TeV);
    InterpolatedYields wzttHy(ch,wzttH,do7TeV);
    
    RooRealVar ggh_gamma_BW    (("sig_ggh_"+chstr+tevstr+"_gamma_BW" ).c_str(), "", 1.0);
    RooFormulaVar ggh_mean_CB  (("sig_ggh_"+chstr+tevstr+"_mean_CB"  ).c_str(), getSignalCBMeanString (ch).c_str() , sig_mean_al);
    RooFormulaVar ggh_sigma_CB (("sig_ggh_"+chstr+tevstr+"_sigma_CB" ).c_str(), getSignalCBSigmaString(ch).c_str() , sig_sigma_al);
    RooFormulaVar ggh_alpha    (("sig_ggh_"+chstr+tevstr+"_alpha"    ).c_str(), getSignalCBAlphaString(ch).c_str() , sig_alpha_al);
    RooFormulaVar ggh_n        (("sig_ggh_"+chstr+tevstr+"_n"        ).c_str(), getSignalCBNString(ch).c_str()     , sig_n_al);
    RooFormulaVar ggh_norm     ("ggH_norm"                                    , (ggHy.getInterpolatedYieldString(lumi)).c_str()          , RooArgList(masshiggs));
    
    RooRealVar vbf_gamma_BW    (("sig_vbf_"+chstr+tevstr+"_gamma_BW" ).c_str(), "", 1.0);
    RooFormulaVar vbf_mean_CB  (("sig_vbf_"+chstr+tevstr+"_mean_CB"  ).c_str(), getSignalCBMeanString (ch).c_str() , sig_mean_al);
    RooFormulaVar vbf_sigma_CB (("sig_vbf_"+chstr+tevstr+"_sigma_CB" ).c_str(), getSignalCBSigmaString(ch).c_str() , sig_sigma_al);
    RooFormulaVar vbf_alpha    (("sig_vbf_"+chstr+tevstr+"_alpha"    ).c_str(), getSignalCBAlphaString(ch).c_str() , sig_alpha_al);
    RooFormulaVar vbf_n        (("sig_vbf_"+chstr+tevstr+"_n"        ).c_str(), getSignalCBNString(ch).c_str()     , sig_n_al);
    RooFormulaVar vbf_norm     ("vbfH_norm"                                   , (vbfHy.getInterpolatedYieldString(lumi)).c_str()          , RooArgList(masshiggs));

    RooRealVar wztt_gamma_BW    (("sig_wztt_"+chstr+tevstr+"_gamma_BW" ).c_str(), "", 1.0);
    RooFormulaVar wztt_mean_CB  (("sig_wztt_"+chstr+tevstr+"_mean_CB"  ).c_str(), getSignalCBMeanString (ch).c_str() , sig_mean_al);
    RooFormulaVar wztt_sigma_CB (("sig_wztt_"+chstr+tevstr+"_sigma_CB" ).c_str(), getSignalCBSigmaString(ch).c_str() , sig_sigma_al);
    RooFormulaVar wztt_alpha    (("sig_wztt_"+chstr+tevstr+"_alpha"    ).c_str(), getSignalCBAlphaString(ch).c_str() , sig_alpha_al);
    RooFormulaVar wztt_n        (("sig_wztt_"+chstr+tevstr+"_n"        ).c_str(), getSignalCBNString(ch).c_str()     , sig_n_al);
    RooFormulaVar wztt_norm     ("wzttH_norm"                                   , (wzttHy.getInterpolatedYieldString(lumi)).c_str()       , RooArgList(masshiggs));
      
    /////////// Set parameters to constant //////////////////

    dyof_mean1  .setConstant(kTRUE);
    dyof_sigma1 .setConstant(kTRUE);
    dyof_mean2  .setConstant(kTRUE);
    dyof_sigma2 .setConstant(kTRUE);
    dyof_frac   .setConstant(kTRUE);

    dysf_a0  .setConstant(kTRUE);
    dysf_a1  .setConstant(kTRUE);
    dysf_a2  .setConstant(kTRUE);
    dysf_a3  .setConstant(kTRUE);
    dysf_a4  .setConstant(kTRUE);
    dysf_a5  .setConstant(kTRUE);
    dysf_a6  .setConstant(kTRUE);
    dysf_a7  .setConstant(kTRUE);
    dysf_a8  .setConstant(kTRUE);
    dysf_a9  .setConstant(kTRUE);
    dysf_a10 .setConstant(kTRUE);
    dysf_a11 .setConstant(kTRUE);
    dysf_a12 .setConstant(kTRUE);
    dysf_a13 .setConstant(kTRUE);
    
    otherssf_a0  .setConstant(kTRUE);
    otherssf_a1  .setConstant(kTRUE);
    otherssf_a2  .setConstant(kTRUE);
    otherssf_a3  .setConstant(kTRUE);
    otherssf_a4  .setConstant(kTRUE);
    otherssf_a5  .setConstant(kTRUE);
    otherssf_a6  .setConstant(kTRUE);
    otherssf_a7  .setConstant(kTRUE);
    otherssf_a8  .setConstant(kTRUE);
    otherssf_a9  .setConstant(kTRUE);
    otherssf_a10 .setConstant(kTRUE);
    otherssf_a11 .setConstant(kTRUE);
    otherssf_a12 .setConstant(kTRUE);
    otherssf_a13 .setConstant(kTRUE);

    masshiggs     .setConstant(kTRUE);
    ggh_gamma_BW  .setConstant(kTRUE);
    vbf_gamma_BW  .setConstant(kTRUE);
    wztt_gamma_BW .setConstant(kTRUE);
    
    ////////////////// Define the PDFs /////////////////////////////////

    const char* bkg_qqww_pdf_name   = do1D ? "bkg_qqww"   : "bkg_qqww_1D" ;
    const char* bkg_ggww_pdf_name   = do1D ? "bkg_ggww"   : "bkg_ggww_1D" ;
    const char* bkg_top_pdf_name    = do1D ? "bkg_top"    : "bkg_top_1D" ;
    const char* bkg_dy_pdf_name     = do1D ? "bkg_dy"     : "bkg_dy_1D" ;
    const char* bkg_wj_pdf_name     = do1D ? "bkg_wj"     : "bkg_wj_1D" ;
    const char* bkg_wgstar_pdf_name = do1D ? "bkg_wgstar" : "bkg_wgstar_1D" ;
    const char* bkg_others_pdf_name = do1D ? "bkg_others" : "bkg_others_1D" ;
    const char* sig_ggH_pdf_name    = do1D ? "ggH"        : "ggH_1D"  ;
    const char* sig_vbfH_pdf_name   = do1D ? "vbfH"       : "vbfH_1D"  ;
    const char* sig_wzttH_pdf_name  = do1D ? "wzttH"      : "wzttH_1D"  ;

    RooLandau *bkg_qqww_pdf = new RooLandau(bkg_qqww_pdf_name,"",CMS_ww2l_mr_1D,qqww_mean,qqww_sigma);

    RooLandau *bkg_ggww_pdf = new RooLandau(bkg_ggww_pdf_name,"",CMS_ww2l_mr_1D,ggww_mean,ggww_sigma);

    RooLandau *bkg_top_pdf = new RooLandau(bkg_top_pdf_name,"",CMS_ww2l_mr_1D,top_mean,top_sigma);

    RooLandau *bkg_wj_pdf = new RooLandau(bkg_wj_pdf_name,"",CMS_ww2l_mr_1D,wj_mean,wj_sigma);

    RooLandau *bkg_wgstar_pdf = new RooLandau(bkg_wgstar_pdf_name,"",CMS_ww2l_mr_1D,wgstar_mean,wgstar_sigma);

    RooAbsPdf *bkg_dy_pdf, *bkg_others_pdf;
    bkg_dy_pdf=bkg_others_pdf=0;
    if(ch == of0j || ch == of1j) {
      RooGaussian *dygauss1 = new RooGaussian("dygauss1","dygauss1",CMS_ww2l_mr_1D,dyof_mean1,dyof_sigma1);
      RooGaussian *dygauss2 = new RooGaussian("dygauss2","dygauss2",CMS_ww2l_mr_1D,dyof_mean2,dyof_sigma2);
      bkg_dy_pdf = new RooAddPdf(bkg_dy_pdf_name,"",*dygauss1,*dygauss2,dyof_frac);
      bkg_others_pdf = new RooLandau(bkg_others_pdf_name,"",CMS_ww2l_mr_1D,othersof_mean,othersof_sigma);
    }
    if(ch == sf0j || ch == sf1j) {
      bkg_dy_pdf = new RooqqZZPdf_v2(bkg_dy_pdf_name,"",CMS_ww2l_mr_1D,
				     dysf_a0,
				     dysf_a1,
				     dysf_a2,
				     dysf_a3,
				     dysf_a4,
				     dysf_a5,
				     dysf_a6,
				     dysf_a7,
				     dysf_a8,
				     dysf_a9,
				     dysf_a10,
				     dysf_a11,
				     dysf_a12,
				     dysf_a13);

      bkg_others_pdf = new RooqqZZPdf_v2(bkg_others_pdf_name,"",CMS_ww2l_mr_1D,
					 otherssf_a0,
					 otherssf_a1,
					 otherssf_a2,
					 otherssf_a3,
					 otherssf_a4,
					 otherssf_a5,
					 otherssf_a6,
					 otherssf_a7,
					 otherssf_a8,
					 otherssf_a9,
					 otherssf_a10,
					 otherssf_a11,
					 otherssf_a12,
					 otherssf_a13);
    }

    RooCBShape      signalCB_ggH  (doFFT ? "signalCB_ggH" : sig_ggH_pdf_name, "", CMS_ww2l_mr_1D, ggh_mean_CB,ggh_sigma_CB,ggh_alpha,ggh_n);
    RooBreitWigner  signalBW_ggH   ("signalBW_ggH", "", CMS_ww2l_mr_1D, masshiggs,ggh_gamma_BW);
    RooFFTConvPdf*  sig_ggH_pdf = new RooFFTConvPdf(sig_ggH_pdf_name, "", CMS_ww2l_mr_1D, signalBW_ggH,signalCB_ggH,2);
    sig_ggH_pdf->setBufferFraction(0.2);

    RooCBShape      signalCB_vbfH   (doFFT? "signalCB_vbfH" : sig_vbfH_pdf_name, "", CMS_ww2l_mr_1D, vbf_mean_CB,vbf_sigma_CB,vbf_alpha,vbf_n);
    RooBreitWigner  signalBW_vbfH   ("signalBW_vbfH", "", CMS_ww2l_mr_1D, masshiggs,vbf_gamma_BW);
    RooFFTConvPdf*  sig_vbfH_pdf = new RooFFTConvPdf(sig_vbfH_pdf_name, "", CMS_ww2l_mr_1D, signalBW_vbfH,signalCB_vbfH,2);
    sig_vbfH_pdf->setBufferFraction(0.2);

    RooCBShape      signalCB_wzttH   (doFFT? "signalCB_wzttH" : sig_wzttH_pdf_name, "", CMS_ww2l_mr_1D, wztt_mean_CB,wztt_sigma_CB,wztt_alpha,wztt_n);
    RooBreitWigner  signalBW_wzttH   ("signalBW_wzttH", "", CMS_ww2l_mr_1D, masshiggs,wztt_gamma_BW);
    RooFFTConvPdf*  sig_wzttH_pdf = new RooFFTConvPdf(sig_wzttH_pdf_name, "", CMS_ww2l_mr_1D, signalBW_wzttH,signalCB_wzttH,2);
    sig_wzttH_pdf->setBufferFraction(0.2);

    w.import(ggh_norm);
    w.import(vbf_norm);
    w.import(wztt_norm);

    if (do1D) { 
      w.import(*bkg_qqww_pdf);
      w.import(*bkg_ggww_pdf);
      w.import(*bkg_top_pdf);
      w.import(*bkg_dy_pdf);
      w.import(*bkg_wj_pdf);
      w.import(*bkg_wgstar_pdf);
      w.import(*bkg_others_pdf);
      if(doFFT) {
        w.import(*sig_ggH_pdf);
        w.import(*sig_vbfH_pdf);
        w.import(*sig_wzttH_pdf);
      } else {
        w.import(signalCB_ggH);
        w.import(signalCB_vbfH);
        w.import(signalCB_wzttH);
      }
    } 

    else {
      TFile *shapes2Dfile = TFile::Open(hww2DShapesfilename.c_str());
      RooArgList v2dList(CMS_ww2l_mr_1D, CMS_ww2l_dphi);
      RooArgSet  v2dSet (CMS_ww2l_mr_1D, CMS_ww2l_dphi);
      
      TH2F* dphishape_qqww = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_qqww_"+chstr).c_str()));
      TH2F* dphishape_ggww = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_ggww_"+chstr).c_str()));
      TH2F* dphishape_top = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_top_"+chstr).c_str()));
      TH2F* dphishape_dy = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_dy_"+chstr).c_str()));
      TH2F* dphishape_wj = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_wj_"+chstr).c_str()));
      TH2F* dphishape_wgstar = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_wgstar_"+chstr).c_str()));
      TH2F* dphishape_others = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_others_"+chstr).c_str()));
      TH2F* dphishape_sig = (TH2F*)(shapes2Dfile->Get(("hist2D_sig_"+chstr).c_str()));

      // 2D systematics
      TH2F* dphishape_qqww_up = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_qqww_"+chstr+"_Up").c_str()));
      TH2F* dphishape_qqww_dn = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_qqww_"+chstr+"_Dn").c_str()));
      TH2F* dphishape_wj_up = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_wj_"+chstr+"_Up").c_str()));
      TH2F* dphishape_wj_dn = (TH2F*)(shapes2Dfile->Get(("hist2D_bkg_wj_"+chstr+"_Dn").c_str()));

      RooDataHist rhist_qqww   (("rhist_qqww_" +chstr+tevstr).c_str(), "", v2dList, dphishape_qqww);
      RooDataHist rhist_ggww   (("rhist_ggww_" +chstr+tevstr).c_str(), "", v2dList, dphishape_ggww);
      RooDataHist rhist_top    (("rhist_top_" +chstr+tevstr).c_str(), "", v2dList, dphishape_top);
      RooDataHist rhist_dy     (("rhist_dy_" +chstr+tevstr).c_str(), "", v2dList, dphishape_dy);
      RooDataHist rhist_wj     (("rhist_wj_" +chstr+tevstr).c_str(), "", v2dList, dphishape_wj);
      RooDataHist rhist_wgstar (("rhist_wgstar_" +chstr+tevstr).c_str(), "", v2dList, dphishape_wgstar);
      RooDataHist rhist_others (("rhist_others_" +chstr+tevstr).c_str(), "", v2dList, dphishape_others);
      RooDataHist rhist_ggH    (("rhist_ggH_" +chstr+tevstr).c_str(), "", v2dList, dphishape_sig);
      RooDataHist rhist_vbfH   (("rhist_vbfH_" +chstr+tevstr).c_str(), "", v2dList, dphishape_sig);
      RooDataHist rhist_wzttH  (("rhist_wzttH_" +chstr+tevstr).c_str(), "", v2dList, dphishape_sig);
      RooDataHist rhist_qqwwup (("rhist_qqwwup_" +chstr+tevstr).c_str(), "", v2dList, dphishape_qqww_up);
      RooDataHist rhist_qqwwdn (("rhist_qqwwdn_" +chstr+tevstr).c_str(), "", v2dList, dphishape_qqww_dn);
      RooDataHist rhist_wjup   (("rhist_wjup_" +chstr+tevstr).c_str(), "", v2dList, dphishape_wj_up);
      RooDataHist rhist_wjdn   (("rhist_wjdn_" +chstr+tevstr).c_str(), "", v2dList, dphishape_wj_dn);
	    
      RooHistPdf rpdf_qqww   (("bkg_qqww_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_qqww);
      RooHistPdf rpdf_ggww   (("bkg_ggww_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_ggww);
      RooHistPdf rpdf_top    (("bkg_top_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_top);
      RooHistPdf rpdf_dy     (("bkg_dy_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_dy);
      RooHistPdf rpdf_wj     (("bkg_wj_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_wj);
      RooHistPdf rpdf_wgstar (("bkg_wgstar_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_wgstar);
      RooHistPdf rpdf_others (("bkg_others_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_others);
      RooHistPdf rpdf_ggH    (("bkg_ggH_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_ggH);
      RooHistPdf rpdf_vbfH   (("bkg_vbfH_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_vbfH);
      RooHistPdf rpdf_wzttH  (("bkg_wzttH_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_wzttH);
      RooHistPdf rpdf_qqwwup (("bkg_qqwwup_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_qqwwup);
      RooHistPdf rpdf_qqwwdn (("bkg_qqwwdn_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_qqwwdn);
      RooHistPdf rpdf_wjup   (("bkg_wjup_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_wjup);
      RooHistPdf rpdf_wjdn   (("bkg_wjdn_dphi2D_pdf_" +chstr+tevstr).c_str(), "", v2dSet , rhist_wjdn);

      RooArgList list_qqww;
      list_qqww.add(rpdf_qqww);
      list_qqww.add(rpdf_qqwwup);
      list_qqww.add(rpdf_qqwwdn);

      RooArgList list_wj;
      list_wj.add(rpdf_wj);
      list_wj.add(rpdf_wjup);
      list_wj.add(rpdf_wjdn);
	    
      RooRealVar CMS_ww2l_qqww2D("CMS_ww2l_qqww2D" ,"" ,0,-10,10); 
      RooRealVar CMS_ww2l_wj2D("CMS_ww2l_wj2D" ,"" ,0,-10,10); 
	
      // FastVerticalInterpHistPdf2D plpdf_qqww   (("bkg_qqww_FVIHP_" +chstr+tevstr).c_str(),   "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,list_qqww               ,RooArgList(CMS_ww2l_qqww2D) ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_qqww   (("bkg_qqww_FVIHP_" +chstr+tevstr).c_str(),   "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_qqww)   ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_ggww   (("bkg_ggww_FVIHP_" +chstr+tevstr).c_str(),   "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_ggww)   ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_top    (("bkg_top_FVIHP_" +chstr+tevstr).c_str(),    "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_top)    ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_dy     (("bkg_dy_FVIHP_" +chstr+tevstr).c_str(),     "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_dy)     ,RooArgList()                ,1.0,1);
      // FastVerticalInterpHistPdf2D plpdf_wj     (("bkg_wj_FVIHP_" +chstr+tevstr).c_str(),     "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,list_wj                 ,RooArgList(CMS_ww2l_wj2D)   ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_wj     (("bkg_wj_FVIHP_" +chstr+tevstr).c_str(),     "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_wj)     ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_wgstar (("bkg_wgstar_FVIHP_" +chstr+tevstr).c_str(), "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_wgstar) ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_others (("bkg_others_FVIHP_" +chstr+tevstr).c_str(), "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_others) ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_ggH    (("sig_ggH_FVIHP_"  +chstr+tevstr).c_str(),   "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_ggH)    ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_vbfH   (("sig_vbfH_FVIHP_" +chstr+tevstr).c_str(),   "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_vbfH)   ,RooArgList()                ,1.0,1);
      FastVerticalInterpHistPdf2D plpdf_wzttH  (("sig_wzttH_FVIHP_" +chstr+tevstr).c_str(),  "",CMS_ww2l_mr_1D,CMS_ww2l_dphi,true,RooArgList(rpdf_wzttH)  ,RooArgList()                ,1.0,1);

      RooProdPdf bkg_qqww_pdf_2D   ("bkg_qqww"   , "", *bkg_qqww_pdf   ,Conditional(plpdf_qqww   , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_ggww_pdf_2D   ("bkg_ggww"   , "", *bkg_ggww_pdf   ,Conditional(plpdf_ggww   , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_top_pdf_2D    ("bkg_top"    , "", *bkg_top_pdf    ,Conditional(plpdf_top    , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_dy_pdf_2D     ("bkg_dy"     , "", *bkg_dy_pdf     ,Conditional(plpdf_dy     , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_wj_pdf_2D     ("bkg_wj"     , "", *bkg_wj_pdf     ,Conditional(plpdf_wj     , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_wgstar_pdf_2D ("bkg_wgstar" , "", *bkg_wgstar_pdf ,Conditional(plpdf_wgstar , RooArgSet(CMS_ww2l_dphi))); 
      RooProdPdf bkg_others_pdf_2D ("bkg_others" , "", *bkg_others_pdf ,Conditional(plpdf_others , RooArgSet(CMS_ww2l_dphi))); 
	
      if(doFFT) {
        RooProdPdf sig_ggH_pdf_2D   ("ggH",   "", *sig_ggH_pdf    ,Conditional(plpdf_ggH   , RooArgSet(CMS_ww2l_dphi))); 
        RooProdPdf sig_vbfH_pdf_2D  ("vbfH",  "", *sig_vbfH_pdf   ,Conditional(plpdf_vbfH  , RooArgSet(CMS_ww2l_dphi))); 
        RooProdPdf sig_wzttH_pdf_2D ("wzttH", "", *sig_wzttH_pdf  ,Conditional(plpdf_wzttH , RooArgSet(CMS_ww2l_dphi))); 
        
        w.import(bkg_qqww_pdf_2D); 
        w.import(bkg_ggww_pdf_2D); 
        w.import(bkg_top_pdf_2D); 
        w.import(bkg_dy_pdf_2D); 
        w.import(bkg_wj_pdf_2D); 
        w.import(bkg_wgstar_pdf_2D); 
        w.import(bkg_others_pdf_2D); 
        w.import(sig_ggH_pdf_2D); 
        w.import(sig_vbfH_pdf_2D); 
        w.import(sig_wzttH_pdf_2D); 
      } else {
        RooProdPdf sig_ggH_pdf_2D   ("ggH",  "", signalCB_ggH   ,Conditional(plpdf_ggH   , RooArgSet(CMS_ww2l_dphi))); 
        RooProdPdf sig_vbfH_pdf_2D  ("vbfH", "", signalCB_vbfH  ,Conditional(plpdf_vbfH  , RooArgSet(CMS_ww2l_dphi))); 
        RooProdPdf sig_wzttH_pdf_2D ("wzttH","", signalCB_wzttH ,Conditional(plpdf_wzttH , RooArgSet(CMS_ww2l_dphi))); 
        
        w.import(bkg_qqww_pdf_2D); 
        w.import(bkg_ggww_pdf_2D); 
        w.import(bkg_top_pdf_2D); 
        w.import(bkg_dy_pdf_2D); 
        w.import(bkg_wj_pdf_2D); 
        w.import(bkg_wgstar_pdf_2D); 
        w.import(bkg_others_pdf_2D); 
        w.import(sig_ggH_pdf_2D); 
        w.import(sig_vbfH_pdf_2D); 
        w.import(sig_wzttH_pdf_2D); 
      }

    }

    w.writeToFile(workspace.c_str());

  }

};

void createWorkspace() {

  HiggsMassPointInfo hmpi7;
  hmpi7.lumi = 4.94;
  hmpi7.do1D = true;
  hmpi7.do7TeV = true;
  hmpi7.doFFT = false;
  hmpi7.treeFolder = "latinos_tree_skim_of_7TeV";
  hmpi7.hww2DShapesfilename = "config/hww2DShapes_7TeV.root";
  hmpi7.xsecProvider.initXsec();
  hmpi7.xsecProvider.initQCDScale();
  hmpi7.xsecProvider.initPDF();
  hmpi7.xsecProvider.initJetBinFracs();

  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_100_SingleElectron2011Av4.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_101_SingleMuon2011Av4.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_102_DoubleElectron2011Av4.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_103_DoubleMuon2011Av4.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_104_MuEG2011Av4.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_120_SingleElectron2011Av6.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_121_SingleMuon2011Av6.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_122_DoubleElectron2011Av6.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_123_DoubleMuon2011Av6.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_124_MuEG2011Av6.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_140_SingleElectron2011Bv1a.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_141_SingleMuon2011Bv1a.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_142_DoubleElectron2011Bv1a.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_143_DoubleMuon2011Bv1a.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_144_MuEG2011Bv1a.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_150_SingleElectron2011AMay10.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_151_SingleMuon2011AMay10.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_152_DoubleMuon2011AMay10.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_153_DoubleElectron2011AMay10.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_154_MuEG2011AMay10.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_160_SingleElectron2011AAug05.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_161_SingleMuon2011AAug05.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_162_DoubleElectron2011AAug05.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_163_DoubleMuon2011AAug05.root");
  hmpi7.ymaker_data   .fill(hmpi7.treeFolder+"/data/latino_164_MuEG2011AAug05.root");
  hmpi7.ymaker_qqww   .fill(hmpi7.treeFolder+"/nominals/latino_000_WWJets2LMad.root");
  hmpi7.ymaker_ggww   .fill(hmpi7.treeFolder+"/nominals/latino_001_GluGluToWWTo4L.root");
  hmpi7.ymaker_top    .fill(hmpi7.treeFolder+"/nominals/latino_019_TTTo2L2Nu2B.root");
  hmpi7.ymaker_top    .fill(hmpi7.treeFolder+"/nominals/latino_011_TtWFullDR.root");
  hmpi7.ymaker_top    .fill(hmpi7.treeFolder+"/nominals/latino_012_TbartWFullDR.root");
  hmpi7.ymaker_dysf   .fill(hmpi7.treeFolder+"/nominals/latino_037_DY50toLLMad.root");
  hmpi7.ymaker_dysf   .fill(hmpi7.treeFolder+"/nominals/latino_036_DY10toLLMad.root");
  hmpi7.ymaker_dyof   .fill(hmpi7.treeFolder+"/dyTemplate/latino_DYtt_2011_added.root");
  hmpi7.ymaker_wj     .fill(hmpi7.treeFolder+"/wjets/WJetsEstimated_Full2011_added.root");
  hmpi7.ymaker_wgstar .fill(hmpi7.treeFolder+"/nominals/latino_082_WGstarToElNuMad.root");
  hmpi7.ymaker_wgstar .fill(hmpi7.treeFolder+"/nominals/latino_083_WGstarToMuNuMad.root");
  hmpi7.ymaker_wgstar .fill(hmpi7.treeFolder+"/nominals/latino_084_WGstarToTauNuMad.root");
  hmpi7.ymaker_others .fill(hmpi7.treeFolder+"/nominals/latino_074_WZJetsMad.root");
  hmpi7.ymaker_others .fill(hmpi7.treeFolder+"/nominals/latino_075_ZZJetsMad.root");

  for (float i = 115.; i <= 180.; i += 5.) {
    // for (float i = 125.; i <= 125.; i += 1.) {  
    for(int j = 0; j < 2; ++j) hmpi7.createCard(i, 50, 500, j);
  }

  hmpi7.do1D = false;
  for (float i = 115.; i <= 180.; i += 5.) {
    // for (float i = 125.; i <= 125.; i += 1.) {  
    for(int j = 0; j < 2; ++j) hmpi7.createCard(i, 50, 500, j);
  }

  HiggsMassPointInfo hmpi8;
  hmpi8.lumi = 19.47;
  hmpi8.do1D = true;
  hmpi8.do7TeV = false;
  hmpi8.doFFT = false;
  hmpi8.treeFolder = "latinos_tree_skim_of_8TeV";
  hmpi8.hww2DShapesfilename = "config/hww2DShapes_8TeV.root";
  hmpi8.xsecProvider.initXsec();
  hmpi8.xsecProvider.initQCDScale();
  hmpi8.xsecProvider.initPDF();
  hmpi8.xsecProvider.initJetBinFracs();

  hmpi8.ymaker_data   .fill(hmpi8.treeFolder+"/data/latino_RunA_892pbinv.root");
  hmpi8.ymaker_data   .fill(hmpi8.treeFolder+"/data/latino_RunB_4404pbinv.root");
  hmpi8.ymaker_data   .fill(hmpi8.treeFolder+"/data/latino_RunC_7032pbinv.root");
  hmpi8.ymaker_data   .fill(hmpi8.treeFolder+"/data/latino_RunD_7274pbinv.root");
  hmpi8.ymaker_qqww   .fill(hmpi8.treeFolder+"/nominals/latino_000_WWJets2LMad.root");
  hmpi8.ymaker_ggww   .fill(hmpi8.treeFolder+"/nominals/latino_001_GluGluToWWTo4L.root");
  hmpi8.ymaker_top    .fill(hmpi8.treeFolder+"/nominals/latino_019_TTTo2L2Nu2B.root");
  hmpi8.ymaker_top    .fill(hmpi8.treeFolder+"/nominals/latino_011_TtWFullDR.root");
  hmpi8.ymaker_top    .fill(hmpi8.treeFolder+"/nominals/latino_012_TbartWFullDR.root");
  hmpi8.ymaker_dysf   .fill(hmpi8.treeFolder+"/nominals/latino_037_DY50toLLMad.root");
  hmpi8.ymaker_dysf   .fill(hmpi8.treeFolder+"/nominals/latino_036_DY10toLLMad.root");
  hmpi8.ymaker_dyof   .fill(hmpi8.treeFolder+"/nominals/latino_DYtt_19.5fb.root");
  hmpi8.ymaker_wj     .fill(hmpi8.treeFolder+"/wjets/latino_LooseLoose_19.5fb.root");
  hmpi8.ymaker_others .fill(hmpi8.treeFolder+"/nominals/latino_074_WZJetsMad.root");
  hmpi8.ymaker_others .fill(hmpi8.treeFolder+"/nominals/latino_075_ZZJetsMad.root");
  hmpi8.ymaker_wgstar .fill(hmpi8.treeFolder+"/nominals/latino_082_WGstarToElNuMad.root");
  hmpi8.ymaker_wgstar .fill(hmpi8.treeFolder+"/nominals/latino_083_WGstarToMuNuMad.root");
  hmpi8.ymaker_wgstar .fill(hmpi8.treeFolder+"/nominals/latino_084_WGstarToTauNuMad.root");

  for (float i = 115.; i <= 180.; i += 5.) {
    // for (float i = 125.; i <= 125.; i += 1.) {  
    for(int j = 0; j < 2; ++j) hmpi8.createCard(i, 50, 500, j);
  }

  hmpi8.do1D = false;
  for (float i = 115.; i <= 180.; i += 5.) {
    // for (float i = 125.; i <= 125.; i += 1.) {  
    for(int j = 0; j < 2; ++j) hmpi8.createCard(i, 50, 500, j);
  }

}
