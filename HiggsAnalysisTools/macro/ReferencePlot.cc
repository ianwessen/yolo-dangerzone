#include <vector>

#include "TH1F.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TFrame.h"
#include "TLine.h"
#include "TStyle.h"
#include "TString.h"
#include <iostream>

class ReferencePlot {
public:
  ReferencePlot() { 
    _hist.clear(); 
    _refHist=0; 
    _labelFont        = 42;
    _labelOffset      = 0.015;
    _axisLabelSize    = 0.050;
    _titleOffset      = 1.6;
    _sf               = 1.0;  
  }
  void setHists(std::vector<TH1F*> hists, std::vector<TString> labels) { _hist = hists; _label = labels; }
  void setRefHist(int pos) { 
    if(pos>=(int)_hist.size())
      std::cout << "Warning: position for reference histogram larger than hist size. " 
		<< std::endl
		<< "Setting reference histogram the first one." << std::endl;
    else _refHist=pos;
  }

  void setColors() {
    // set some colors for the first histograms
    _color.push_back(1); // the reference black
    _color.push_back(kRed+1);
    _color.push_back(kMagenta+3);
    _color.push_back(kAzure-6);
    _color.push_back(kTeal+3);
    
    if(_hist.size()>_color.size()) {
      _color.resize(_hist.size());
      for(int i=4; i<(int)_hist.size(); i++) _color[i]=kOrange+i-3;
    }

  }
  
  //------------------------------------------------------------------------------
  // AxisFonts
  //------------------------------------------------------------------------------
  void AxisFonts(TAxis*  axis,
		 TString coordinate,
		 TString title)
  {
    axis->SetLabelFont  (_labelFont  );
    axis->SetLabelOffset(_labelOffset*_sf);
    axis->SetLabelSize  (_axisLabelSize*_sf);
    axis->SetNdivisions (  505);
    axis->SetTitleFont  (_labelFont);
    axis->SetTitleOffset(  1.5);
    axis->SetTitleSize  (_axisLabelSize*_sf);
    
    if (coordinate == "y") axis->SetTitleOffset(_titleOffset*_sf);
    
    axis->SetTitle(title);
  }

  void Draw(TCanvas *c1, bool doratio=true) {
    
    setColors();

    _sf = doratio?1.3:1.0;
    gStyle->SetOptStat(0);
    gPad->cd();
    gPad->Clear();
    
    TPad *pad1;
    if(doratio) {
      pad1 = new TPad("pad1","pad1",0,1-0.614609572,1,1);
      pad1->SetTopMargin(0.0983606557);
      pad1->SetBottomMargin(0.0163934426);
    } else {
      pad1 = new TPad("pad1","pad1",0,0,1,1);
    }
    pad1->Draw();
    pad1->cd();

    int nSamples=_hist.size();

    Float_t max=0;

    for(int i=0; i<nSamples; i++) {
      if(!_hist[i]) continue;
      _hist[i]->Scale(1./_hist[i]->Integral());

      _hist[i]->SetLineColor(_color[i]);
      _hist[i]->SetMarkerColor(_color[i]);

      _hist[i]->SetLineWidth(2);
      _hist[i]->SetMarkerStyle(24);

      AxisFonts(_hist[i]->GetXaxis(), "x", _hist[i]->GetXaxis()->GetTitle());

      Float_t thisMax = GetMaximumIncludingErrors(_hist[i]);
      if(thisMax > max) max = thisMax; 
    }

    for(int i=0; i<nSamples; i++) {
      if(!_hist[i]) continue;
      _hist[i]->SetMaximum(max);
      if(i==0) _hist[i]->Draw();
      else _hist[i]->Draw("same");
    }


    // draw the legend
    TLegend* legend = new TLegend(0.64, 0.74, 0.87, 0.90);
    
    legend->SetBorderSize(     0);
    legend->SetFillColor (     0);
    legend->SetTextAlign (    12);
    legend->SetTextFont  (_labelFont);
    legend->SetTextSize  (0.05);
    
    for(int i=0; i<nSamples; ++i) {
      if(!_hist[i]) continue;
      legend->AddEntry(_hist[i], _label[i].Data());
    }
    legend->Draw();

    gPad->Update();
    if(!doratio) return;

    c1->cd();
    TPad *pad2 = new TPad("pad2","pad2",0,0,1,1-0.614609572);
    pad2->SetTopMargin(0.0261437908);
    pad2->SetBottomMargin(0.392156863);
    pad2->Draw();
    pad2->cd();

    TH1F *rref = (TH1F*)_hist[_refHist]->Clone("rref"); 

    TLine *line = new TLine(rref->GetXaxis()->GetXmin(), 1.0, rref->GetXaxis()->GetXmax(), 1.0);
    line->SetLineColor(kRed);
    line->SetLineWidth(2);

    for(int i=0; i<nSamples; i++) {
    
      if(i==_refHist) continue;
      TH1F *rcur = (TH1F*)_hist[i]->Clone("rcur");   
      double absmax = 0;
      for (int b = 0, n = rcur->GetNbinsX(); b <= n+1; ++b) {
	double scale = rref->GetBinContent(b);
	if (scale == 0) {
	  rcur->SetBinContent(b, 0);
	  rref->SetBinContent(b, 0);
	  rcur->SetBinError(b, 0);
	  rref->SetBinError(b, 0);
	} else {
	  rcur->SetBinContent(b, rcur->GetBinContent(b)/scale);
	  rref->SetBinContent(b, rref->GetBinContent(b)/scale);
	  rcur->SetBinError(b, rcur->GetBinError(b)/scale);
	  rref->SetBinError(b, rref->GetBinError(b)/scale);
	  double mymax = TMath::Max(1.2*fabs(rcur->GetBinContent(b)-1)+1.4*rcur->GetBinError(b), 2.0*rref->GetBinError(b));
	  absmax = TMath::Max(mymax, absmax);
	}
      }

      rref->GetYaxis()->SetRangeUser(TMath::Max(0.,1.-absmax), absmax+1.);
      AxisFonts(rref->GetYaxis(), "y", "ratio");
      AxisFonts(rref->GetXaxis(), "x", rref->GetXaxis()->GetTitle());
      rref->SetLineWidth(0);
      rref->SetLineColor(0);
      rref->SetMarkerColor(0);
      rref->SetFillColor(kYellow);
      rref->GetYaxis()->SetTitle("data/mc");
      rref->SetLabelSize(0.06*_sf,"XY");
      rref->SetTitleSize(0.06*_sf,"XY");
      rref->SetTitle("");
      rref->Draw("E2");
      rcur->SetMarkerStyle(20);
      rcur->Draw("E SAME p");      
    }
    line->Draw();

  }

  //------------------------------------------------------------------------------
  // GetMaximumIncludingErrors
  //------------------------------------------------------------------------------
  Float_t GetMaximumIncludingErrors(TH1F* h) {
    Float_t maxWithErrors = 0;
    
    for (Int_t i=1; i<=h->GetNbinsX(); i++) {
      
      Float_t binHeight = h->GetBinContent(i) + h->GetBinError(i);
      
      if (binHeight > maxWithErrors) maxWithErrors = binHeight;
    }
    
    return maxWithErrors;
  }
  
private:
  std::vector<TH1F*> _hist;
  std::vector<TString> _label;
  std::vector<int> _color;
  int _refHist;
  Int_t   _labelFont      ;
  Float_t _labelOffset    ;
  Float_t _axisLabelSize  ;
  Float_t _titleOffset    ;
  Float_t _sf    ;
};
