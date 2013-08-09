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
#include <RooHistFunc.h>
#include <RooPlot.h>
#include <RooAbsPdf.h>
#include <RooWorkspace.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TH1F.h>
#include <sstream>
#include <TStyle.h>

#include "YieldMaker.h"
#include "FitSelection.hh"
#include "DatacardParser.hh"

using namespace RooFit;


void plotWsp1D(const char *inputfile, const char *figfile) {

  TFile *hww2l2nu = TFile::Open(inputfile);
  RooWorkspace *w = (RooWorkspace*)hww2l2nu->Get("w");

  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  RooPlot *mrplot = mr->frame();

  // take the pdfs from the workspace
  RooAbsPdf *qqww = (RooAbsPdf*)w->pdf("bkg_qqww");
  RooAbsPdf *ggww = (RooAbsPdf*)w->pdf("bkg_ggww");
  RooAbsPdf *top = (RooAbsPdf*)w->pdf("bkg_top");
  RooAbsPdf *dy = (RooAbsPdf*)w->pdf("bkg_dy");
  RooAbsPdf *wj = (RooAbsPdf*)w->pdf("bkg_wj");
  RooAbsPdf *wgstar = (RooAbsPdf*)w->pdf("bkg_wgstar");
  RooAbsPdf *others = (RooAbsPdf*)w->pdf("bkg_others");
  RooAbsPdf *ggH = (RooAbsPdf*)w->pdf("ggH");
  RooAbsPdf *qqH = (RooAbsPdf*)w->pdf("vbfH");

  qqww->plotOn(mrplot,LineColor(kAzure+1));
  ggww->plotOn(mrplot,LineColor(kAzure+2));
  top->plotOn(mrplot,LineColor(kYellow+2));
  dy->plotOn(mrplot,LineColor(kGreen+2));
  wj->plotOn(mrplot,LineColor(kGray+2));
  wgstar->plotOn(mrplot,LineColor(kOrange+7));
  others->plotOn(mrplot,LineColor(kOrange+2));
  ggH->plotOn(mrplot,LineColor(kRed));
  qqH->plotOn(mrplot,LineColor(kRed));

  // for the legend
  TH1F *hqqww = new TH1F("hqqww","",1,0,1);
  TH1F *hggww = new TH1F("hggww","",1,0,1);
  TH1F *htop = new TH1F("htop","",1,0,1);
  TH1F *hdy = new TH1F("hdy","",1,0,1);
  TH1F *hwj = new TH1F("hwj","",1,0,1);
  TH1F *hwgstar = new TH1F("hwgstar","",1,0,1);
  TH1F *hothers = new TH1F("hothers","",1,0,1);
  TH1F *hggH = new TH1F("hggH","",1,0,1);

  hqqww->SetLineColor(kAzure+1);
  hggww->SetLineColor(kAzure+2);
  htop->SetLineColor(kYellow+2);
  hdy->SetLineColor(kGreen+2);
  hwj->SetLineColor(kGray+2);
  hwgstar->SetLineColor(kOrange+7);
  hothers->SetLineColor(kOrange+2);
  hggH->SetLineColor(kRed);

  hqqww->SetLineWidth(2);
  hggww->SetLineWidth(2);
  htop->SetLineWidth(2);
  hdy->SetLineWidth(2);
  hwj->SetLineWidth(2);
  hwgstar->SetLineWidth(2);
  hothers->SetLineWidth(2);
  hggH->SetLineWidth(2);

  TLegend* legend = new TLegend(0.64, 0.24, 0.87, 0.90);
    
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  ( 0.05);

  legend->AddEntry(hqqww,"qq #rightarrow WW","l");
  legend->AddEntry(hggww,"gg #rightarrow WW","l");
  legend->AddEntry(htop,"top","l");
  legend->AddEntry(hdy,"DY","l");
  legend->AddEntry(hwj,"W+jets","l");
  legend->AddEntry(hothers,"WZ,ZZ","l");
  legend->AddEntry(hwgstar,"W#gamma^{(*)}","l");
  legend->AddEntry(hggH,"qq,gg #rightarrow H","l");

  TCanvas *c1 = new TCanvas("c1","c1");
  mrplot->Draw();
  legend->Draw();

  c1->SaveAs(figfile);

}

void plotWsp2D(const char *inputfile, int nj, bool do7TeV) {

  gStyle->SetPaintTextFormat("1.2f");

  TFile *hww2l2nu = TFile::Open(inputfile);
  RooWorkspace *w = (RooWorkspace*)hww2l2nu->Get("w");

  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  RooRealVar *dphi = w->var("CMS_ww2l_dphi");

  // take the pdfs from the workspace
  RooDataHist *rhist_qqww = (RooDataHist*) w->data(Form("rhist_qqww_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_qqww("pdf_qqww","pdf_qqww",RooArgSet(*mr,*dphi),*rhist_qqww);
  TH2F *h_qqww = (TH2F*)pdf_qqww.createHistogram("h_qqww",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_ggww = (RooDataHist*) w->data(Form("rhist_ggww_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_ggww("pdf_ggww","pdf_ggww",RooArgSet(*mr,*dphi),*rhist_ggww);
  TH2F *h_ggww = (TH2F*)pdf_ggww.createHistogram("h_ggww",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_top = (RooDataHist*) w->data(Form("rhist_top_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_top("pdf_top","pdf_top",RooArgSet(*mr,*dphi),*rhist_top);
  TH2F *h_top = (TH2F*)pdf_top.createHistogram("h_top",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_dy = (RooDataHist*) w->data(Form("rhist_dy_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_dy("pdf_dy","pdf_dy",RooArgSet(*mr,*dphi),*rhist_dy);
  TH2F *h_dy = (TH2F*)pdf_dy.createHistogram("h_dy",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_wj = (RooDataHist*) w->data(Form("rhist_wj_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_wj("pdf_wj","pdf_wj",RooArgSet(*mr,*dphi),*rhist_wj);
  TH2F *h_wj = (TH2F*)pdf_wj.createHistogram("h_wj",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_wgstar = (RooDataHist*) w->data(Form("rhist_wgstar_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_wgstar("pdf_wgstar","pdf_wgstar",RooArgSet(*mr,*dphi),*rhist_wgstar);
  TH2F *h_wgstar = (TH2F*)pdf_wgstar.createHistogram("h_wgstar",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_others = (RooDataHist*) w->data(Form("rhist_others_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_others("pdf_others","pdf_others",RooArgSet(*mr,*dphi),*rhist_others);
  TH2F *h_others = (TH2F*)pdf_others.createHistogram("h_others",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_ggH = (RooDataHist*) w->data(Form("rhist_ggH_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_ggH("pdf_ggH","pdf_ggH",RooArgSet(*mr,*dphi),*rhist_ggH);
  TH2F *h_ggH = (TH2F*)pdf_ggH.createHistogram("h_ggH",*mr,RooFit::YVar(*dphi));

  RooDataHist *rhist_vbfH = (RooDataHist*) w->data(Form("rhist_vbfH_of_%dj_%dTeV",nj,(do7TeV ? 7 : 8)));
  RooHistPdf pdf_vbfH("pdf_vbfH","pdf_vbfH",RooArgSet(*mr,*dphi),*rhist_vbfH);
  TH2F *h_vbfH = (TH2F*)pdf_vbfH.createHistogram("h_vbfH",*mr,RooFit::YVar(*dphi));

  RooDataSet *data = (RooDataSet*)w->data("data_obs");
  RooDataHist *rhist_data = data->binnedClone("data_obs_hist","binned data_obs");
  RooHistPdf pdf_data("pdf_data","pdf_data",RooArgSet(*mr,*dphi),*rhist_data);
  TH2F *h_data = (TH2F*)pdf_data.createHistogram("h_data",*mr,RooFit::YVar(*dphi));

  vector<TH2F*> histos;
  histos.push_back(h_qqww);
  histos.push_back(h_ggww);
  histos.push_back(h_top);
  histos.push_back(h_dy);
  histos.push_back(h_wj);
  histos.push_back(h_wgstar);
  histos.push_back(h_others);
  histos.push_back(h_ggH);
  histos.push_back(h_vbfH);
  histos.push_back(h_data);

  TCanvas *c1 = new TCanvas("c1","c1");
  for(int i=0;i<(int)histos.size();++i) {
    histos[i]->Draw("colz");
    c1->SaveAs(Form("%s_of_%dj.png",histos[i]->GetName(),nj)); 
    c1->SaveAs(Form("%s_of_%dj.pdf",histos[i]->GetName(),nj));
  }


  /*
  h_qqww->Draw("colz"); h_qqww->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_qqww->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_qqww->GetName(),nj));
  h_ggww->Draw("colz"); h_ggww->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_ggww->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_ggww->GetName(),nj));
  h_top->Draw("colz"); h_top->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_top->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_top->GetName(),nj));
  h_dy->Draw("colz"); h_dy->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_dy->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_dy->GetName(),nj));
  h_wj->Draw("colz"); h_wj->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_wj->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_wj->GetName(),nj));
  h_wgstar->Draw("colz"); h_wgstar->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_wgstar->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_wgstar->GetName(),nj));
  h_others->Draw("colz"); h_others->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_others->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_others->GetName(),nj));
  h_ggH->Draw("colz"); h_ggH->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_ggH->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_ggH->GetName(),nj));
  h_vbfH->Draw("colz"); h_vbfH->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_vbfH->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_vbfH->GetName(),nj));
  h_data->Draw("colz"); h_data->Draw("text same"); c1->SaveAs(Form("%s_of_%dj.png",h_data->GetName(),nj)); c1->SaveAs(Form("%s_of_%dj.pdf",h_data->GetName(),nj));
  */
  

}

void plotWsp2DFromTH2D(const char *inputfile, int nj, bool do7TeV) {

  gStyle->SetPaintTextFormat("1.2f");
  gStyle->SetTextFont(62);
  gStyle->SetTextSize(5);

  TFile *hww2l2nu = TFile::Open(inputfile);
  TH2F *h_qqww = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_qqww_of_%dj",nj));
  TH2F *h_ggww = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_ggww_of_%dj",nj));
  TH2F *h_top = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_top_of_%dj",nj));
  TH2F *h_dy = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_dy_of_%dj",nj));
  TH2F *h_wj = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_wj_of_%dj",nj));
  TH2F *h_wgstar = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_wgstar_of_%dj",nj));
  TH2F *h_others = (TH2F*)hww2l2nu->Get(Form("hist2D_bkg_others_of_%dj",nj));
  TH2F *h_ggH = (TH2F*)hww2l2nu->Get(Form("hist2D_sig_of_%dj",nj));

  vector<TH2F*> histos;
  histos.push_back(h_qqww);
  histos.push_back(h_ggww);
  histos.push_back(h_top);
  histos.push_back(h_dy);
  histos.push_back(h_wj);
  histos.push_back(h_wgstar);
  histos.push_back(h_others);
  histos.push_back(h_ggH);

  TH2F* h_bkgsum = (TH2F*)h_qqww->Clone(Form("hist2D_bkg_sumall_of_%dj",nj));
  for(int i=1;i<(int)histos.size();++i) h_qqww->Add(histos[i]);
  histos.push_back(h_bkgsum);

  TCanvas *c1 = new TCanvas("c1","c1");
  for(int i=0;i<(int)histos.size();++i) {
    for(int x=1;x<(int)histos[i]->GetNbinsX()+1;x++) {
      for(int y=1;y<(int)histos[i]->GetNbinsY()+1;y++) {
        if(histos[i]->GetBinContent(x,y)>0.99) histos[i]->SetBinContent(x,y,0.0); // means one bin with few entries
        if(histos[i]->GetBinContent(x,y)<0.005) histos[i]->SetBinContent(x,y,0.005); 
      }
    }
    histos[i]->GetXaxis()->SetTitle("M_{R} (GeV/c^{2})");
    histos[i]->GetYaxis()->SetTitle("#Delta#phi_{R}");
    histos[i]->Draw("colz");
    histos[i]->Draw("text same");
    c1->SaveAs(Form("%s_%s.png",histos[i]->GetName(),(do7TeV ? "7TeV" : "8TeV"),nj)); 
    c1->SaveAs(Form("%s_%s.pdf",histos[i]->GetName(),(do7TeV ? "7TeV" : "8TeV"),nj)); 
    c1->SaveAs(Form("%s_%s.C",histos[i]->GetName(),(do7TeV ? "7TeV" : "8TeV"),nj)); 
  }

}

void plotMRAllSignals(bool do7TeV) {

  stringstream fss0;
  if(do7TeV) fss0 << "datacards/hww-4.94fb.mH115.of_0j_shape_7TeV_workspace.root";
  else fss0 << "datacards/hww-19.47fb.mH115.of_0j_shape_8TeV_workspace.root";
  
  TFile *hww2l2nu0 = TFile::Open(fss0.str().c_str());
  RooWorkspace *w = (RooWorkspace*)hww2l2nu0->Get("w");

  TCanvas *c1 = new TCanvas("c1","c1");
  
  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  mr->setRange(50,250);
  RooPlot *mrplot = mr->frame();
  mrplot->Draw();

  TLegend* legend = new TLegend(0.60, 0.64, 0.85, 0.90);
    
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  ( 0.05);

  vector<int> cols;
  cols.push_back(kAzure+4);
  cols.push_back(kRed+1);
  cols.push_back(kMagenta+2);
  cols.push_back(kGreen+3);

  int j=0;
  for (float i = 115.; i <= 150.; i += 10.) {
    j++;

    stringstream fss;
    if(do7TeV) fss << "datacards/hww-4.94fb.mH" << i << ".of_0j_shape_7TeV_workspace.root";
    else fss << "datacards/hww-19.47fb.mH" << i << ".of_0j_shape_8TeV_workspace.root";

    cout << "Opening file " << fss.str() << endl;

    TFile *hww2l2nu = TFile::Open(fss.str().c_str());
    w = (RooWorkspace*)hww2l2nu->Get("w");
    
    // take the pdfs from the workspace
    RooAbsPdf *ggH = (RooAbsPdf*)w->pdf("ggH");
    ggH->plotOn(mrplot,LineColor(cols[j-1]));  

    TH1F *h = new TH1F("h","h",0,0,1);
    h->SetLineColor(cols[j-1]);

    stringstream lab;
    lab << "m_{H}=" << i << " GeV";
    legend->AddEntry(h,lab.str().c_str(),"l");

  }

  mrplot->Draw();
  legend->Draw();
  c1->SaveAs("severalHiggsesMR.C");
  c1->SaveAs("severalHiggsesMR.pdf");
  c1->SaveAs("severalHiggsesMR.png");
}

void plotOneShapeSyst(string process, string syst, int ch, bool do7TeV) {

  cout << "==>plotting syst " << syst << " for " << process << " for channel " << ch << endl;

  std::string chstr;
  if (ch == of0j) chstr = "of_0j";
  if (ch == of1j) chstr = "of_1j";
  if (ch == sf0j) chstr = "sf_0j";
  if (ch == sf1j) chstr = "sf_1j";

  stringstream fss;
  if(do7TeV) fss << "datacards/hww-4.94fb.mH125." << chstr << "_shape_7TeV_workspace.root";
  else fss << "datacards/hww-19.47fb.mH125." << chstr << "_shape_8TeV_workspace.root";

  TFile *hww2l2nu = TFile::Open(fss.str().c_str());

  RooWorkspace *w = (RooWorkspace*)hww2l2nu->Get("w");

  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  mr->setRange(50,500);
  RooRealVar *MH = w->var("MH");
  MH->setVal(125);

  RooPlot *mrplot = mr->frame();

  // take the pdfs from the workspace
  RooAbsPdf *pdfnom = (RooAbsPdf*)w->pdf(process.c_str());
  if(pdfnom==0) {
    cout << "PDF for process: " << process << " not found." << endl;
    return;
  }
  pdfnom->plotOn(mrplot,LineColor(kBlack));

  string tevstr = "_8TeV";

  // only special case where name in DC!=name in the workspace. A bit error prone... to be improved 
  string systvar=syst;
  if(syst.compare("scaleup_qcd")!=string::npos && syst.length()==11) systvar="scaleup";
  if(syst.compare("scaledn_qcd")!=string::npos && syst.length()==11) systvar="scaledn";

  RooRealVar *WW_mean_err, *WW_sigma_err, *WW_alpha_err, *WW_n_err;
  if(process.compare("ggH")==0) {
    WW_mean_err = (RooRealVar*)w->var(("sig_"+chstr+tevstr+"_mean_err_"+systvar).c_str());
    WW_sigma_err = (RooRealVar*)w->var(("sig_"+chstr+tevstr+"_sigma_err_"+systvar).c_str());
    WW_alpha_err = (RooRealVar*)w->var(("sig_"+chstr+tevstr+"_alpha_err_"+systvar).c_str()); 
    WW_n_err = (RooRealVar*)w->var(("sig_"+chstr+tevstr+"_n_err_"+systvar).c_str()); 
  } else {
    WW_mean_err = (RooRealVar*)w->var((process+"_"+chstr+tevstr+"_mean_err_"+systvar).c_str());
    WW_sigma_err = (RooRealVar*)w->var((process+"_"+chstr+tevstr+"_sigma_err_"+systvar).c_str());
  }

  if(WW_mean_err==0 || WW_sigma_err==0) {
    cout << WW_mean_err->GetName() << " not found " << endl;
    cout << WW_sigma_err->GetName() << " not found " << endl;
    return;
  }

  if(process.compare("ggH")==0 && (WW_alpha_err==0 || WW_sigma_err==0) ) {  
    cout << WW_alpha_err->GetName() << " not found " << endl;
    cout << WW_n_err->GetName() << " not found " << endl;
    return;
  }

  stringstream fdc;
  if(do7TeV) fdc << "datacards/hww-4.94fb.mH125." << chstr << "_shape_7TeV.txt";
  else fdc << "datacards/hww-19.47fb.mH125." << chstr << "_shape_8TeV.txt";
  DatacardParser dcp(fdc.str());

  string dcm(WW_mean_err->GetName());
  string dcs(WW_sigma_err->GetName());
  if(syst.compare("scaleup_qcd")==0 || syst.compare("scaledn_qcd")==0) { dcm+="_qcd"; dcs+="_qcd"; }
  string dca,dcn;
  if(process.compare("ggH")==0) {
    dca = WW_alpha_err->GetName();
    dcn = WW_n_err->GetName();
  }

  float meanShift = dcp.getRelUncertainty(dcm);
  float sigmaShift = dcp.getRelUncertainty(dcs);
  float alphaShift, nShift;
  if(process.compare("ggH")==0) {
    alphaShift = dcp.getRelUncertainty(dca);
    nShift = dcp.getRelUncertainty(dcn);
  }
  cout << "Landau error on mean/sigma =  " << meanShift << " / " << sigmaShift << endl;
  if(meanShift<-100 || sigmaShift<-100) {
    cout << dcm << " or " << dcs << "  not found in the datacard txt file" << endl;
    return;
  }

  WW_mean_err->setVal(meanShift);
  WW_sigma_err->setVal(sigmaShift);
  if(process.compare("ggH")==0) {  
    WW_alpha_err->setVal(alphaShift);
    WW_n_err->setVal(nShift);
  }

  pdfnom->plotOn(mrplot,LineColor(kRed+3),LineStyle(kDashed),LineWidth(2));

  WW_mean_err->setVal(-1*meanShift);
  WW_sigma_err->setVal(-1*sigmaShift);
  if(process.compare("ggH")==0) {  
    WW_alpha_err->setVal(-1*alphaShift);
    WW_n_err->setVal(-1*nShift);
  }

  pdfnom->plotOn(mrplot,LineColor(kRed+1),LineStyle(kDashed),LineWidth(2));


  TH1F *hNom = new TH1F("h1","",0,0,1);
  TH1F *hP1S = new TH1F("h2","",0,0,1);
  TH1F *hM1S = new TH1F("h3","",0,0,1);
  hNom->SetLineColor(kBlack);
  hP1S->SetLineColor(kRed+3);
  hM1S->SetLineColor(kRed+1);
  hP1S->SetLineStyle(kDashed);
  hM1S->SetLineStyle(kDashed);
  hNom->SetLineWidth(2);
  hP1S->SetLineWidth(2);
  hM1S->SetLineWidth(2);

  //float ymin = (process.compare("ggH")==0 ? 0.65 : 0.20);
  float ymin = (process.compare("ggH")==0 ? 0.65 : 0.65);
  TLegend* legend = new TLegend(0.55, ymin, 0.90, ymin+0.25);
  legend->SetBorderSize(    0);
  legend->SetFillColor (    0);
  legend->SetTextAlign (   12);
  legend->SetTextFont  (   42);
  legend->SetTextSize  ( 0.05);
  legend->AddEntry(hNom, "nominal PDF","l");
  legend->AddEntry(hP1S, "+1 #sigma PDF","l");
  legend->AddEntry(hM1S, "-1 #sigma PDF","l");

  TCanvas *c1 = new TCanvas("c1","c1");
  mrplot->Draw();
  legend->Draw();

  stringstream fileout;
  fileout << "syst_mr_" << process << "_" << syst << "_" << chstr << (do7TeV ? "_7TeV" : "_8TeV");
  stringstream pdffile,pngfile;
  pdffile << fileout.str() << ".pdf";
  pngfile << fileout.str() << ".png";
  c1->SaveAs(pdffile.str().c_str());
  c1->SaveAs(pngfile.str().c_str());

}


void plotAll(bool do7TeV) {
  
  gStyle->SetPalette(1);

  if(do7TeV) {
    plotWsp1D("datacards/hww-4.94fb.mH125.of_0j_shape_7TeV_workspace.root","pdfs_of_0j_7TeV.pdf");
    plotWsp1D("datacards/hww-4.94fb.mH125.of_1j_shape_7TeV_workspace.root","pdfs_of_1j_7TeV.pdf");
  } else {
    plotWsp1D("datacards/hww-19.47fb.mH125.of_0j_shape_8TeV_workspace.root","pdfs_of_0j_8TeV.pdf");
    plotWsp1D("datacards/hww-19.47fb.mH125.of_1j_shape_8TeV_workspace.root","pdfs_of_1j_8TeV.pdf");
  }
  plotMRAllSignals(do7TeV);

  if(do7TeV) {
    //    plotWsp2D("datacards/hww-4.94fb.mH125.of_0j_shape_2D_7TeV_workspace.root",0,do7TeV);
    //    plotWsp2D("datacards/hww-4.94fb.mH125.of_1j_shape_2D_7TeV_workspace.root",1,do7TeV);
    plotWsp2DFromTH2D("config/hww2DShapes_8TeV.root",0,do7TeV);
    plotWsp2DFromTH2D("config/hww2DShapes_8TeV.root",1,do7TeV);
  } else {
    plotWsp2DFromTH2D("config/hww2DShapes_7TeV.root",0,do7TeV);
    plotWsp2DFromTH2D("config/hww2DShapes_7TeV.root",1,do7TeV);    
    //    plotWsp2D("datacards/hww-19.47fb.mH125.of_0j_shape_2D_8TeV_workspace.root",0,do7TeV);
    //    plotWsp2D("datacards/hww-19.47fb.mH125.of_1j_shape_2D_8TeV_workspace.root",1,do7TeV);
  }

}

void plotAllSyst(bool do7TeV) {
  
  vector<string> processes; 
  processes.push_back("ggH");
  processes.push_back("bkg_qqww");
  processes.push_back("bkg_ggww");
  processes.push_back("bkg_top");
  //processes.push_back("bkg_dy");
  processes.push_back("bkg_wgstar");
  processes.push_back("bkg_others");

  vector<string> systematics;
  systematics.push_back("scaleup_qcd");
  systematics.push_back("scaledn_qcd");
  systematics.push_back("MC");
  systematics.push_back("res_met");
  systematics.push_back("res_e");
  systematics.push_back("scaleup_e");
  systematics.push_back("scaledn_e");
  systematics.push_back("scaleup_m");
  systematics.push_back("scaledn_m");
  systematics.push_back("scaleup_j");
  systematics.push_back("scaledn_j");

  for(int ch=0;ch<2;++ch) {
    for(int proc=0;proc<(int)processes.size();++proc) {
      for(int syst=0;syst<(int)systematics.size();++syst) {
        if(syst<3 && proc!=1) continue;
        if(systematics[syst].compare("res_met")==0 && processes[proc].compare("bkg_dy")==0) continue;
        plotOneShapeSyst(processes[proc], systematics[syst], ch, do7TeV);
      }
    }
  }

  vector<string> systematics_fakerate;
  systematics_fakerate.push_back("fakerateup");
  systematics_fakerate.push_back("fakeratedn");
  for(int ch=0;ch<2;++ch) {
    for(int syst=0;syst<(int)systematics_fakerate.size();++syst) { 
      plotOneShapeSyst("bkg_wj", systematics_fakerate[syst], ch, do7TeV);
    }
  }

}
