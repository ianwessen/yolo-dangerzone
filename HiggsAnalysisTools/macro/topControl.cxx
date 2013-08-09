#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "ReferencePlot.cc"
#include <iostream>

TH1F *truth =0;
TH1F *control =0;
TString additionalCutControl="*1";
TString additionalCutCommon="*1";

void getTopControl(const char *variable, int njet, float min, float max, int nbins=40, const char* units="") {

  TFile *fileTruth = TFile::Open("results/datasets_trees_skim/top_ll.root");
  TTree *treeTruth = (TTree*)fileTruth->Get("latino");

  TFile *fileControl = TFile::Open("results_data/datasets_trees/dataset_ll.root");
  TTree *treeControl = (TTree*)fileControl->Get("latino");

  TString nameTruth = TString(variable)+TString("_truth");
  TString nameControl = TString(variable)+TString("_control");
  truth = new TH1F(nameTruth.Data(),nameTruth.Data(),nbins,min,max);
  control = new TH1F(nameControl.Data(),nameControl.Data(),nbins,min,max);
  truth->Sumw2();
  control->Sumw2();

  TString cutTruth = (njet==0) ? TString("WWSel*baseW*puW*effW") : TString("WWSel1j*baseW*puW*effW");
  TString cutControl;
  if(TString(variable).Contains("nsoftbjet")) {
    if(njet==0) {
      cutControl = TString("(step[9] && njet>=1 && softbdisc>1.4 && ((mpmet>(37+nvtx/2) && abs(dphilljet)<165 && sameflav) || !sameflav))");
    } else {
      cutControl = TString("(step[9] && njet==2 && softbdisc>2.1 && ((mpmet>(37+nvtx/2) && abs(dphilljet)<165 && sameflav) || !sameflav))");    
    }
  } else {
    if(njet==0) {
      cutControl = TString("(step[9] && njet>=1 && softbdisc>1.4 && nSoftMuNoJets>0 && ((mpmet>(37+nvtx/2) && abs(dphilljet)<165 && sameflav) || !sameflav))");
    } else {
      cutControl = TString("(step[9] && njet==2 && softbdisc>2.1 && ((mpmet>(37+nvtx/2) && abs(dphilljet)<165 && sameflav) || !sameflav))");    
    }
  }

  cutControl += additionalCutControl;
  cutControl += additionalCutCommon;
  cutTruth += additionalCutCommon;

  TString variableControl;
  if(TString(variable).Contains("nsoftbjet")) 
    variableControl = TString("nsoftbjet-1");
  else variableControl = TString(variable);

  treeTruth->Project(nameTruth.Data(),variable,cutTruth.Data());
  treeControl->Project(nameControl.Data(),variableControl.Data(),cutControl.Data());

  TString axistitle = TString(variable)+TString(" ")+TString(units);
  truth->GetXaxis()->SetTitle(axistitle.Data());
  
  std::cout << "DONE filling distribution for TOP specie." << std::endl;

}

void Draw(const char *name="figure") {
  if(!truth || !control) {
    std::cout << "ERROR: before get a control sample, then issue Draw()..." << std::endl;
    return;
  }

  std::vector<TH1F*> hists;
  hists.push_back(truth);
  hists.push_back(control);

  std::vector<TString> labels;
  labels.push_back("MC");
  labels.push_back("data control");

  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  ReferencePlot refplot;
  refplot.setHists(hists,labels);
  refplot.Draw(c1);
  
  c1->SaveAs(name);

}

void addCutControl(const char *cut) {
  additionalCutControl = TString(cut);
}

void addCutCommon(const char *cut) {
  additionalCutCommon = TString(cut);
}


TString Filename(const char *prefix, const char* variable, int njet, const char* channel, const char* ext) {
  char njstr[5];
  sprintf(njstr,"_%dj_",njet);
  TString output = TString(prefix) + TString("_")
    + TString(variable) 
    + TString(njstr) 
    + TString(channel) 
    + TString(ext);
  return output;
}

void DrawControlSamples() {
  
  //------------------------------
  // top data control sample
  //------------------------------
  std::vector<TString> channel;
  channel.push_back("*sameflav");
  channel.push_back("*(!sameflav)");
  for(int j=0; j<2; j++) {
    for(int c=0; c<2; c++) {
      cout << "===> DOING: nj = " << j << " and channel = " << channel[c] << std::endl;
      addCutCommon(channel[c].Data());
  
      // MR
      getTopControl("gammaMRStar",j,0,500,15,"[GeV]");
      TString namefile = (c==0) ? Filename("topctr","gammaMRStar",j,"sf",".pdf") : Filename("topctr","gammaMRStar",j,"of",".pdf");
      Draw(namefile.Data());

      // nsoftbjet
      getTopControl("nsoftbjet",j,0,6,6);
      namefile = (c==0) ? Filename("topctr","nsoftbjet",j,"sf",".pdf") : Filename("topctr","nsoftbjet",j,"of",".pdf");
      Draw(namefile.Data());
    }
  }

}
