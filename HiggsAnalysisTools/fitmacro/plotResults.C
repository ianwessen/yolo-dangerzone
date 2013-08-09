#include <RooConstVar.h>
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
#include <RooAddPdf.h>
#include <RooWorkspace.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TH1F.h>
#include <sstream>
#include <TStyle.h>

#include "YieldMaker.h"
#include "DatacardParser.hh"

using namespace RooFit;
using namespace std;
//           0      1     2     3        4        5      6     7      8
enum samp { iData, iHWW, iWZ, iZJets, iHWWOnly, iWJets, iTop, iWW, nSamples };

float xPos[nSamples] = {0.22,0.22,0.22,0.22,0.39,0.39,0.39,0.39}; 
float yOff[nSamples] = {0,1,2,3,0,1,2,3};

const Float_t _tsize   = 0.03;
const Float_t _xoffset = 0.20;
const Float_t _yoffset = 0.05;

//------------------------------------------------------------------------------
// DrawLegend
//------------------------------------------------------------------------------
void DrawLegend(Float_t x1,
                Float_t y1,
                TH1F*   hist,
                TString label,
                TString option)
{
  TLegend* legend = new TLegend(x1,
                                y1,
                                x1 + _xoffset,
                                y1 + _yoffset);
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (_tsize);

  legend->AddEntry(hist, label.Data(), option.Data());

  legend->Draw();
}


void plotMass(string resultsfile, string workspace, int ch, bool do7TeV, int nbins=100) {

  TFile *wspfile = TFile::Open(workspace.c_str());
  RooWorkspace *w = (RooWorkspace*)wspfile->Get("w");

  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  RooPlot *mrplot = mr->frame(RooFit::Bins(nbins));

  // take the pdfs from the workspace
  RooAbsPdf *qqww = (RooAbsPdf*)w->pdf("bkg_qqww");
  RooAbsPdf *ggww = (RooAbsPdf*)w->pdf("bkg_ggww");
  RooAbsPdf *top = (RooAbsPdf*)w->pdf("bkg_top");
  RooAbsPdf *dy = (RooAbsPdf*)w->pdf("bkg_dy");
  RooAbsPdf *wj = (RooAbsPdf*)w->pdf("bkg_wj");
  RooAbsPdf *wgstar = (RooAbsPdf*)w->pdf("bkg_wgstar");
  RooAbsPdf *others = (RooAbsPdf*)w->pdf("bkg_others");
  RooAbsPdf *ggH = (RooAbsPdf*)w->pdf("ggH");
  RooAbsPdf *vbfH = (RooAbsPdf*)w->pdf("vbfH");
  RooAbsPdf *wzttH = (RooAbsPdf*)w->pdf("wzttH");

  RooDataSet *data_obs = (RooDataSet*)w->data("data_obs");
  data_obs->plotOn(mrplot);

  vector<RooAbsPdf*> pdfs;
  pdfs.push_back(qqww);
  pdfs.push_back(ggww);
  pdfs.push_back(top);
  pdfs.push_back(dy);
  pdfs.push_back(wj);
  pdfs.push_back(wgstar);
  pdfs.push_back(others);
  pdfs.push_back(ggH);
  pdfs.push_back(vbfH);
  pdfs.push_back(wzttH);

  cout << pdfs.size() << "PDFs taken from workspace " << workspace << endl;

  vector<float> norms, fracs;
  norms.resize(pdfs.size());
  fracs.resize(pdfs.size());

  TFile *mlfitfile = TFile::Open(resultsfile.c_str());
  RooArgSet *norm_fit_s = (RooArgSet*)mlfitfile->Get("norm_fit_s");
  TIterator* iter(norm_fit_s->createIterator());  

  std::string chstr;
  if (ch == of0j) chstr = "of_0j";
  if (ch == of1j) chstr = "of_1j";
  
  float norm=0.;

  for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
    RooConstVar *rcv = dynamic_cast<RooConstVar *>(a);
    std::string name = rcv->GetName();
    if(name.find("bkg_qqww")!=string::npos && name.find(chstr)!=string::npos) norms[0]=rcv->getVal();
    if(name.find("bkg_ggww")!=string::npos && name.find(chstr)!=string::npos) norms[1]=rcv->getVal();
    if(name.find("bkg_top")!=string::npos && name.find(chstr)!=string::npos) norms[2]=rcv->getVal();
    if(name.find("bkg_dy")!=string::npos && name.find(chstr)!=string::npos) norms[3]=rcv->getVal();
    if(name.find("bkg_wj")!=string::npos && name.find(chstr)!=string::npos) norms[4]=rcv->getVal();
    if(name.find("bkg_wgstar")!=string::npos && name.find(chstr)!=string::npos) norms[5]=rcv->getVal();
    if(name.find("bkg_others")!=string::npos && name.find(chstr)!=string::npos) norms[6]=rcv->getVal();
    if(name.find("ggH")!=string::npos && name.find(chstr)!=string::npos) norms[7]=rcv->getVal();
    if(name.find("vbfH")!=string::npos && name.find(chstr)!=string::npos) norms[8]=rcv->getVal();
    if(name.find("wzttH")!=string::npos && name.find(chstr)!=string::npos) norms[9]=rcv->getVal();
    if(name.find(chstr)!=string::npos) norm += rcv->getVal();
  }
  delete iter;

  RooArgList list_pdfs("list_pdfs"); 
  RooArgList list_fracs("list_fracs");
  for(int i=0;i<(int)pdfs.size();++i) {
    list_pdfs.add(*pdfs[i]);
    if(i!=(int)pdfs.size()-1) {
      stringstream fss;
      fss << pdfs[i]->GetName() << "_Frac";
      RooRealVar *frac = new RooRealVar(fss.str().c_str(),"",0.5,0.,1.);
      frac->setVal(norms[i]/norm);
      cout << pdfs[i]->GetName() << " has frac = " << norms[i]/norm << endl;
      list_fracs.add(*frac);
    }
  }


  RooAddPdf *lh = new RooAddPdf("lh","",list_pdfs,list_fracs);
  lh->plotOn(mrplot, RooFit::LineColor(kRed+2),   RooFit::FillColor(kRed+1),   RooFit::FillStyle(1001), RooFit::DrawOption("F") );
  lh->plotOn(mrplot, RooFit::LineColor(kGray+2),  RooFit::FillColor(kGray+1),  RooFit::FillStyle(1001), RooFit::DrawOption("F"), RooFit::Components("bkg*"));
  lh->plotOn(mrplot, RooFit::LineColor(kAzure-1), RooFit::FillColor(kAzure-2), RooFit::FillStyle(1001), RooFit::DrawOption("F"), RooFit::Components("bkg_qqww,bkg_ggww,bkg_dy,bkg_top,bkg_others"));
  lh->plotOn(mrplot, RooFit::LineColor(kYellow+1),  RooFit::FillColor(kYellow),  RooFit::FillStyle(1001), RooFit::DrawOption("F"), RooFit::Components("bkg_qqww,bkg_ggww,bkg_dy,bkg_top"));
  lh->plotOn(mrplot, RooFit::LineColor(kGreen+2), RooFit::FillColor(kGreen+1), RooFit::FillStyle(1001), RooFit::DrawOption("F"), RooFit::Components("bkg_qqww,bkg_ggww,bkg_dy"));
  lh->plotOn(mrplot, RooFit::LineColor(kAzure-8), RooFit::FillColor(kAzure-9), RooFit::FillStyle(1001), RooFit::DrawOption("F"), RooFit::Components("bkg_qqww,bkg_ggww"));
  lh->plotOn(mrplot, RooFit::LineColor(kRed+2), RooFit::Components("sig*"));
  data_obs->plotOn(mrplot);


  // for the legend
  TH1F *hdata = new TH1F("hdata","",1,0,1);
  TH1F *hqqww = new TH1F("hqqww","",1,0,1);
  TH1F *htop = new TH1F("htop","",1,0,1);
  TH1F *hdy = new TH1F("hdy","",1,0,1);
  TH1F *hwj = new TH1F("hwj","",1,0,1);
  TH1F *hothers = new TH1F("hothers","",1,0,1);
  TH1F *hggH = new TH1F("hggH","",1,0,1);

  hdata->SetMarkerStyle(8);
  hqqww->SetFillColor(kAzure-9);
  htop->SetFillColor(kYellow);
  hdy->SetFillColor(kGreen+1);
  hwj->SetFillColor(kGray+1);
  hothers->SetFillColor(kAzure-2);
  hggH->SetFillColor(kRed+1);

  hqqww->SetLineColor(kAzure-9);
  htop->SetLineColor(kYellow);
  hdy->SetLineColor(kGreen+1);
  hwj->SetLineColor(kGray+1);
  hothers->SetLineColor(kAzure-2);
  hggH->SetLineColor(kRed+1);
  hggH->SetLineColor(kRed+1);

  TCanvas *c1 = new TCanvas("c1","c1",700,700);
  c1->cd();
  mrplot->Draw();
  DrawLegend(0.30+xPos[iData],  0.84 - yOff[iData]*_yoffset, hdata,   " data",   "lp");
  DrawLegend(0.30+xPos[iHWW],   0.84 - yOff[iHWW]*_yoffset,     hggH,    " H125",   "f" );
  DrawLegend(0.30+xPos[iWZ],    0.84 - yOff[iWZ]*_yoffset,      hothers, " VV",  "f" );
  DrawLegend(0.30+xPos[iZJets], 0.84 - yOff[iZJets]*_yoffset,   hdy,     " Z/#gamma^{*}", "f" );
  DrawLegend(0.30+xPos[iWJets], 0.84 - yOff[iWJets]*_yoffset,   hwj,     " W+jets", "f" );
  DrawLegend(0.30+xPos[iTop],   0.84 - yOff[iTop]*_yoffset,     htop,    " top",    "f" );
  DrawLegend(0.30+xPos[iWW],    0.84 - yOff[iWW]*_yoffset,      hqqww,   " WW",     "f" );
  DrawLegend(0.30+xPos[iHWWOnly], 0.84 - yOff[iHWWOnly]*_yoffset,   hggH,     " m_{H}=125 GeV", "l" );

  stringstream prelLabel;
  prelLabel << "CMS Preliminary                                        #sqrt{s} = " << (do7TeV ? "7" : "8") << " TeV, L = " << (do7TeV ? "4.9" : "19.6") << " fb^{-1}";
  TLatex* CP = new TLatex(0.15,0.96, prelLabel.str().c_str());
  CP->SetNDC(kTRUE);
  CP->SetTextSize(0.030);
  CP->Draw();

  std::string chlbl;
  if (ch == of0j) chlbl = "e#mu, 0-jet";
  if (ch == of1j) chlbl = "e#mu, 1-jet";
  TLatex* CH = new TLatex(0.30+xPos[iHWWOnly], 0.84 - 4*_yoffset, chlbl.c_str());
  CH->SetNDC(kTRUE);
  CH->SetTextSize(0.030);
  CH->Draw();
  

  stringstream figname, png, pdf;
  figname << "mr_postfit" << (do7TeV ? "_7TeV_" : "_8TeV_") << chstr;
  png << figname.str() << ".png";
  pdf << figname.str() << ".pdf";
  c1->SaveAs(png.str().c_str());
  c1->SaveAs(pdf.str().c_str());

}

float getSigWeight(float mr, float dphi, TH2F *histo_sig, TH2F *histo_bkg) {
  int bin = histo_sig->FindBin(mr,dphi);
  float s = histo_sig->GetBinContent(bin);
  float b = histo_bkg->GetBinContent(bin);
  return (s+b==0) ? 0.0 : s/(s+b);
}

TH1F* plotBkgSubMass(string resultsfile, string workspace, int ch, bool do7TeV, int nbins=100) {

  TFile *wspfile = TFile::Open(workspace.c_str());
  RooWorkspace *w = (RooWorkspace*)wspfile->Get("w");

  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  RooRealVar *dphi = w->var("CMS_ww2l_dphi");
  RooPlot *mrplot = mr->frame(RooFit::Bins(nbins));

  // take the pdfs from the workspace
  RooAbsPdf *qqww = (RooAbsPdf*)w->pdf("bkg_qqww");
  RooAbsPdf *ggww = (RooAbsPdf*)w->pdf("bkg_ggww");
  RooAbsPdf *top = (RooAbsPdf*)w->pdf("bkg_top");
  RooAbsPdf *dy = (RooAbsPdf*)w->pdf("bkg_dy");
  RooAbsPdf *wj = (RooAbsPdf*)w->pdf("bkg_wj");
  RooAbsPdf *wgstar = (RooAbsPdf*)w->pdf("bkg_wgstar");
  RooAbsPdf *others = (RooAbsPdf*)w->pdf("bkg_others");
  RooAbsPdf *ggH = (RooAbsPdf*)w->pdf("ggH");
  RooAbsPdf *vbfH = (RooAbsPdf*)w->pdf("vbfH");
  RooAbsPdf *wzttH = (RooAbsPdf*)w->pdf("wzttH");

  RooDataSet *data_obs = (RooDataSet*)w->data("data_obs");
  data_obs->plotOn(mrplot);

  vector<RooAbsPdf*> pdfs;
  pdfs.push_back(qqww);
  pdfs.push_back(ggww);
  pdfs.push_back(top);
  pdfs.push_back(dy);
  pdfs.push_back(wj);
  pdfs.push_back(wgstar);
  pdfs.push_back(others);
  pdfs.push_back(ggH);
  pdfs.push_back(vbfH);
  pdfs.push_back(wzttH);


  cout << pdfs.size() << "PDFs taken from workspace " << workspace << endl;

  vector<float> norms, fracs;
  norms.resize(pdfs.size());
  fracs.resize(pdfs.size());

  TFile *mlfitfile = TFile::Open(resultsfile.c_str());
  RooArgSet *norm_fit_s = (RooArgSet*)mlfitfile->Get("norm_fit_s");
  TIterator* iter(norm_fit_s->createIterator());  

  std::string chstr;
  if (ch == of0j) chstr = "of_0j";
  if (ch == of1j) chstr = "of_1j";
  
  float norm=0.;
  float normbkg=0.;

  for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
    RooConstVar *rcv = dynamic_cast<RooConstVar *>(a);
    std::string name = rcv->GetName();
    if(name.find("bkg_qqww")!=string::npos && name.find(chstr)!=string::npos) norms[0]=rcv->getVal();
    if(name.find("bkg_ggww")!=string::npos && name.find(chstr)!=string::npos) norms[1]=rcv->getVal();
    if(name.find("bkg_top")!=string::npos && name.find(chstr)!=string::npos) norms[2]=rcv->getVal();
    if(name.find("bkg_dy")!=string::npos && name.find(chstr)!=string::npos) norms[3]=rcv->getVal();
    if(name.find("bkg_wj")!=string::npos && name.find(chstr)!=string::npos) norms[4]=rcv->getVal();
    if(name.find("bkg_wgstar")!=string::npos && name.find(chstr)!=string::npos) norms[5]=rcv->getVal();
    if(name.find("bkg_others")!=string::npos && name.find(chstr)!=string::npos) norms[6]=rcv->getVal();
    if(name.find("ggH")!=string::npos && name.find(chstr)!=string::npos) norms[7]=rcv->getVal();
    if(name.find("vbfH")!=string::npos && name.find(chstr)!=string::npos) norms[8]=rcv->getVal();
    if(name.find("wzttH")!=string::npos && name.find(chstr)!=string::npos) norms[9]=rcv->getVal();
    if(name.find(chstr)!=string::npos) norm += rcv->getVal();
    if(name.find(chstr)!=string::npos && name.find("H")==string::npos) normbkg += rcv->getVal();
  }
  delete iter;

  RooArgList list_pdfs("list_pdfs"); RooArgList list_pdfs_bkg("list_pdfs_bkg");
  RooArgList list_fracs("list_fracs"); RooArgList list_fracs_bkg("list_fracs_bkg");
  for(int i=0;i<(int)pdfs.size();++i) {
    list_pdfs.add(*pdfs[i]);
    if(i<7) list_pdfs_bkg.add(*pdfs[i]);
    if(i!=(int)pdfs.size()-1) {
      stringstream fss;
      fss << pdfs[i]->GetName() << "_Frac";
      RooRealVar *frac = new RooRealVar(fss.str().c_str(),"",0.5,0.,1.);
      frac->setVal(norms[i]/norm);
      cout << pdfs[i]->GetName() << " has frac = " << norms[i]/norm << endl;
      list_fracs.add(*frac);
      if(i<6) list_fracs_bkg.add(*frac);
    }
  }

  int ntoy=100;
  RooAddPdf *lhbkg = new RooAddPdf("lhbkg","",list_pdfs_bkg,list_fracs_bkg);
  RooDataSet *bkgonly = lhbkg->generate(RooArgSet(*mr,*dphi),normbkg*ntoy);
  RooDataSet *sigonly = ggH->generate(RooArgSet(*mr,*dphi),(norms[7]+norms[8]+norms[9])*ntoy);
  TH2F *histo_data = new TH2F("histo_data","",nbins,50,250,6,0,TMath::Pi());
  TH2F *histo_bkg = (TH2F*)histo_data->Clone("histo_bkg");
  TH2F *histo_sig = (TH2F*)histo_data->Clone("histo_sig");

  bkgonly->fillHistogram(histo_bkg,RooArgList(*mr,*dphi));
  sigonly->fillHistogram(histo_sig,RooArgList(*mr,*dphi));
  data_obs->fillHistogram(histo_data,RooArgList(*mr,*dphi));

  stringstream nameh;
  nameh << "histo_data_weight_" << (ch==of0j ? "of0j_" : "of1j_") << (do7TeV ? "7TeV" : "8TeV");

  TH1F *histo_data_weight = new TH1F(nameh.str().c_str(),"",nbins,50,250);
  TH1F *histo_bkg_weight = (TH1F*)histo_data_weight->Clone("histo_bkg_weight");
  histo_data_weight->Sumw2();
  histo_bkg_weight->Sumw2();

  for (int i = 0; i < data_obs->numEntries(); i++) {
    float mrval = data_obs->get(i)->getRealValue("CMS_ww2l_mr_1D");
    float dphival = data_obs->get(i)->getRealValue("CMS_ww2l_dphi");
    float weight = getSigWeight(mrval,dphival,histo_sig,histo_bkg);
    //    cout << "mr = " << mrval << "\tdphi = " << dphival << "\twgt = " << weight << endl;
    histo_data_weight->Fill(mrval,weight);
  }

  for (int i = 0; i < bkgonly->numEntries(); i++) {
    float mrval = bkgonly->get(i)->getRealValue("CMS_ww2l_mr_1D");
    float dphival = bkgonly->get(i)->getRealValue("CMS_ww2l_dphi");
    float weight = getSigWeight(mrval,dphival,histo_sig,histo_bkg);
    histo_bkg_weight->Fill(mrval,weight);
  }


  histo_data_weight->Add(histo_bkg_weight,-1./float(ntoy));
  //  histo_data_weight->Scale((norms[7]+norms[8]+norms[9])/histo_data_weight->Integral());
  return histo_data_weight;

//   TFile *file = TFile::Open("stocazzo.root","recreate");
//   data_obs_weight->Write();
//   histo_data->Write();
//   histo_bkg->Write();
//   histo_data_weight->Write();
//   histo_bkg_weight->Write();
//   file->Close();

}

void plotAll() {
  plotMass("unblinding/mlfits/mlfitTEST8TeV_comb_0j1j_2D.root","datacards/hww-19.47fb.mH125.of_0j_shape_2D_8TeV_workspace.root",0,false,25);
  plotMass("unblinding/mlfits/mlfitTEST8TeV_comb_0j1j_2D.root","datacards/hww-19.47fb.mH125.of_1j_shape_2D_8TeV_workspace.root",1,false,22);
  plotMass("unblinding/mlfits/mlfitTEST7TeV_comb_0j1j_2D.root","datacards/hww-4.94fb.mH125.of_0j_shape_2D_7TeV_workspace.root",0,true,25);
  plotMass("unblinding/mlfits/mlfitTEST7TeV_comb_0j1j_2D.root","datacards/hww-4.94fb.mH125.of_0j_shape_2D_7TeV_workspace.root",1,true,22);
}

void plotAllWgt() {
  int nbins = 10;
  TH1F *h_of0j_8TeV = plotBkgSubMass("unblinding/mlfits/mlfitTEST8TeV_comb_0j1j_2D.root","datacards/hww-19.47fb.mH125.of_0j_shape_2D_8TeV_workspace.root",0,false,nbins);
  TH1F *h_of1j_8TeV = plotBkgSubMass("unblinding/mlfits/mlfitTEST8TeV_comb_0j1j_2D.root","datacards/hww-19.47fb.mH125.of_1j_shape_2D_8TeV_workspace.root",1,false,nbins);
  TH1F *h_of0j_7TeV = plotBkgSubMass("unblinding/mlfits/mlfitTEST7TeV_comb_0j1j_2D.root","datacards/hww-4.94fb.mH125.of_0j_shape_2D_7TeV_workspace.root",0,true,nbins);
  TH1F *h_of1j_7TeV = plotBkgSubMass("unblinding/mlfits/mlfitTEST7TeV_comb_0j1j_2D.root","datacards/hww-4.94fb.mH125.of_0j_shape_2D_7TeV_workspace.root",1,true,nbins);
  
  TH1F *h_of = (TH1F*)h_of0j_8TeV->Clone("h_of");
  h_of->Sumw2();
  h_of->Add(h_of1j_8TeV);
  h_of->Add(h_of0j_7TeV);
  h_of->Add(h_of1j_7TeV);

  float normH = 0.;
  TFile *mlfitfile8TeV = TFile::Open("unblinding/mlfits/mlfitTEST8TeV_comb_0j1j_2D.root");
  RooArgSet *norm_fit_s = (RooArgSet*)mlfitfile8TeV->Get("norm_fit_s");
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_ggH"))->getVal();
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_vbfH"))->getVal();
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_wzttH"))->getVal();
  mlfitfile8TeV->Close();

  TFile *mlfitfile7TeV = TFile::Open("unblinding/mlfits/mlfitTEST7TeV_comb_0j1j_2D.root");
  norm_fit_s = (RooArgSet*)mlfitfile7TeV->Get("norm_fit_s");
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_ggH"))->getVal();
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_vbfH"))->getVal();
  normH += ((RooConstVar*)norm_fit_s->find("n_exp_final_binof_0j_proc_wzttH"))->getVal();
  mlfitfile7TeV->Close();

  cout << "signal fitted = " << normH << endl;
  h_of->Sumw2();
  h_of->Scale(normH/h_of->Integral());

  TFile *wspfile = TFile::Open("datacards/hww-19.47fb.mH125.of_0j_shape_2D_8TeV_workspace.root");
  RooWorkspace *w = (RooWorkspace*)wspfile->Get("w");
  RooRealVar *mr = w->var("CMS_ww2l_mr_1D");
  RooAbsPdf *ggH = (RooAbsPdf*)w->pdf("ggH");
  
  RooDataHist *dh = new RooDataHist("dh","",RooArgList(*mr),h_of);
  RooPlot *mrplot = mr->frame(RooFit::Bins(nbins));
  mrplot->SetName("mrplot");
  dh->plotOn(mrplot);
  ggH->plotOn(mrplot);

  TFile *file = TFile::Open("stocazzo.root","recreate");
  h_of->Write();
  mrplot->Write();
  file->Close();
}
