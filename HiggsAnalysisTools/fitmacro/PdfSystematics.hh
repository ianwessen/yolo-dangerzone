#include <string>
#include <sstream>

#include <RooRealVar.h>
#include <RooArgList.h>

using namespace std;

class WWSystematics {
public:
  WWSystematics(string process) { _process=process; }
  ~WWSystematics() {}

  string getFormulaSyst() {
    stringstream fss;
    fss << "@0*(1+@1+@2+@3+@4+@5+@6+@7+@8+@9";
    if(_process.find("qqww")!=string::npos) fss << "+@10+@11+@12";
    fss << ")";
    return fss.str();
  }

  RooArgList getParSystematics(string par, string chstr, string tevstr, float centralval) {
    RooRealVar *WW_par = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_nominal").c_str(), "", 0., -10000., 10000.);
    RooRealVar *WW_par_err_MC = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_MC").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleup = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaledn = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_resmet = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_met").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_rese = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_resmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_mu").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupe = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaledne = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_m").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scalednmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_m").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupj = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_j").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scalednj = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_j").c_str(), "", 0., -10., 10.);

    WW_par->setVal(centralval);
    WW_par->setConstant(kTRUE);

    // parm nuisances will be made floating by combine, if used. 
    // if they are not used, they have to stay fixed
    WW_par_err_MC->setConstant(kTRUE);
    WW_par_err_scaleup->setConstant(kTRUE);
    WW_par_err_scaledn->setConstant(kTRUE);
    WW_par_err_resmet->setConstant(kTRUE);
    WW_par_err_rese->setConstant(kTRUE);
    WW_par_err_resmu->setConstant(kTRUE);
    WW_par_err_scaleupe->setConstant(kTRUE);
    WW_par_err_scaledne->setConstant(kTRUE);
    WW_par_err_scaleupmu->setConstant(kTRUE);
    WW_par_err_scalednmu->setConstant(kTRUE);
    WW_par_err_scaleupj->setConstant(kTRUE);
    WW_par_err_scalednj->setConstant(kTRUE);

    RooArgList WW_par_err(*WW_par,
                          *WW_par_err_resmet,    *WW_par_err_rese, *WW_par_err_resmu, 
                          *WW_par_err_scaleupe,  *WW_par_err_scaledne,
                          *WW_par_err_scaleupmu, *WW_par_err_scalednmu);
    WW_par_err.add(*WW_par_err_scaleupj); 
    WW_par_err.add(*WW_par_err_scalednj);

    if(_process.find("qqww")!=string::npos) {
      WW_par_err.add(*WW_par_err_MC);
      WW_par_err.add(*WW_par_err_scaleup);
      WW_par_err.add(*WW_par_err_scaledn);
    }
    return WW_par_err;
  }

private:
  string _process;
};


class WJetsSystematics {
public:
  WJetsSystematics(string process) { _process=process; }
  ~WJetsSystematics() {}

  string getFormulaSyst() {
    stringstream fss;
    fss << "@0*(1+@1+@2)";
    return fss.str();
  }

  RooArgList getParSystematics(string par, string chstr, string tevstr, float centralval) {
    RooRealVar *WJets_par = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_nominal").c_str(), "", 0., -10000., 100000.);
    RooRealVar *WJets_par_err_fakerateup = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_fakerateup").c_str(), "", 0., -10., 10.);
    RooRealVar *WJets_par_err_fakeratedn = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_fakeratedn").c_str(), "", 0., -10., 10.);

    WJets_par->setVal(centralval);
    WJets_par->setConstant(kTRUE);

    // parm nuisances will be made floating by combine, if used. 
    // if they are not used, they have to stay fixed
    WJets_par_err_fakerateup->setConstant(kTRUE);
    WJets_par_err_fakeratedn->setConstant(kTRUE);

    RooArgList WJets_par_err(*WJets_par,
                             *WJets_par_err_fakerateup, *WJets_par_err_fakeratedn);
    return WJets_par_err;
  }

private:
  string _process;
};

class HWWSystematics {
public:
  HWWSystematics(string process, int mH) { _process=process; _mH=mH; }
  ~HWWSystematics() {}

  string getFormulaSyst() {
    stringstream fss;
    fss << "*(1+@1+@2+@3+@4+@5+@6+@7+@8+@9)";
    return fss.str();
  }

  RooArgList getParSystematics(string par, string chstr, string tevstr) {
    RooRealVar *WW_par_err_resmet = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_met").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_rese = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_resmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_res_mu").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupe = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaledne = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_e").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_m").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scalednmu = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_m").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scaleupj = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaleup_j").c_str(), "", 0., -10., 10.);
    RooRealVar *WW_par_err_scalednj = new RooRealVar((_process+"_"+chstr+tevstr+"_"+par+"_err_scaledn_j").c_str(), "", 0., -10., 10.);
    
    // parm nuisances will be made floating by combine, if used. 
    // if they are not used, they have to stay fixed
    WW_par_err_resmet->setConstant(kTRUE);
    WW_par_err_rese->setConstant(kTRUE);
    WW_par_err_resmu->setConstant(kTRUE);
    WW_par_err_scaleupe->setConstant(kTRUE);
    WW_par_err_scaledne->setConstant(kTRUE);
    WW_par_err_scaleupmu->setConstant(kTRUE);
    WW_par_err_scalednmu->setConstant(kTRUE);
    WW_par_err_scaleupj->setConstant(kTRUE);
    WW_par_err_scalednj->setConstant(kTRUE);

    RooArgList WW_par_err(*WW_par_err_resmet,    *WW_par_err_rese, *WW_par_err_resmu, 
                          *WW_par_err_scaleupe,  *WW_par_err_scaledne,
                          *WW_par_err_scaleupmu, *WW_par_err_scalednmu);
    WW_par_err.add(*WW_par_err_scaleupj); 
    WW_par_err.add(*WW_par_err_scalednj);

    return WW_par_err;
  }

private:
  string _process;
  int _mH;
};
