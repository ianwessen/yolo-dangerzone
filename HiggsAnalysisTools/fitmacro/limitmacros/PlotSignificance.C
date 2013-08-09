#include <TAxis.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TString.h>
#include <TSystem.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

Bool_t verbose = false;

//------------------------------------------------------------------------------
// DrawTLatex
//------------------------------------------------------------------------------
void DrawTLatex(Double_t x, Double_t y, Double_t tsize, const char* text)
{
  TLatex* tl = new TLatex(x, y, text);

  tl->SetNDC();
  tl->SetTextAlign(   32);
  tl->SetTextFont (   42);
  tl->SetTextSize (tsize);

  tl->Draw("same");
}

void PlotSignificance(string  limitFiles   = "inputs/ana_ICHEP_limits_nj_shape7teV_cut8TeV.txt",
                      string  outputPrefix = "combined",
                      string  luminosity   = "5.1 fb^{-1} (8 TeV) + 4.9 fb^{-1} (7 TeV)",
                      Float_t mhmin        = 110,
                      Float_t mhmax        = 160,
                      Int_t   setLogx      = 0,
                      Int_t   setLogy      = 1,
                      string  title        = "H #rightarrow WW #rightarrow 2l2#nu",
                      Bool_t  drawObserved = 1,
                      Int_t   ratio        = 0,
                      string  format       = "pdf")
{
/*   setTDRStyle(); */


/*   gSystem->mkdir(format.c_str(), kTRUE); */


  // Get input files
  //----------------------------------------------------------------------------
  stringstream fnames(limitFiles);
  string fname;
  vector<string> LimitFile;

  while (getline(fnames, fname, ',')) {
    if (verbose) printf(" Using file(s) %s\n", fname.c_str());
    LimitFile.push_back(fname);
  }


  // Read in the nominal limits
  //----------------------------------------------------------------------------
  vector<float> vMass;
  vector<float> vObsLimit; 
  vector<float> vMedianExpLimit; 
  vector<float> vMeanExpLimit; 

  float Mass;
  float ObsLimit; 
  float MedianExpLimit; 
  float MeanExpLimit; 

  ifstream indata;
  indata.open(LimitFile[0].c_str());
  if (!indata) {
    cerr << " Error: file could not be opened: " << LimitFile[0]<< endl;
    return;
  }

  while (indata
	 >> Mass
	 >> ObsLimit
	 >> MedianExpLimit
	 >> MeanExpLimit) {

    vMass          .push_back(Mass);
    vObsLimit      .push_back(ObsLimit); 
    vMedianExpLimit.push_back(MedianExpLimit); 
    vMeanExpLimit  .push_back(MeanExpLimit); 

  }

  UInt_t npoints = vMass.size();


  // Draw
  //----------------------------------------------------------------------------
  TString ctitle = Form("%s_from%.0fto%.0f_logx%d_logy%d_signif",
			outputPrefix.c_str(),
			mhmin,
			mhmax,
			setLogx,
			setLogy);

  TCanvas* canvas = new TCanvas(ctitle, ctitle);

  canvas->SetLeftMargin  (1.30 * canvas->GetLeftMargin());
  canvas->SetRightMargin (2.10 * canvas->GetRightMargin());
  canvas->SetBottomMargin(1.35 * canvas->GetBottomMargin());

  cout << "logy = " << setLogy << endl;

  canvas->SetLogx(setLogx);
  canvas->SetLogy(setLogy);

  float min = +9999;
  float max = -9999;


  // Expected Limit
  //----------------------------------------------------------------------------
  float x   [npoints];
  float y   [npoints];
  
  for (UInt_t i=0; i<npoints; ++i) {

    x [i] = vMass.at(i);

    y   [i] = vMeanExpLimit.at(i);

    if (ratio == 1) {
      y   [i] /= vMeanExpLimit.at(i);
    }
    else if (ratio == 2) {
      y   [i] = 0;
    }

    if (y[i] > max) max = y[i];
    if (y[i] < min) min = y[i];
  }


  TGraph*            ExpLim    = new TGraph           (npoints, x, y);


  // Cosmetics
  //----------------------------------------------------------------------------
  ExpLim->SetTitle("");

  ExpLim->GetXaxis()->SetTitle("Higgs mass [GeV]");
    
  if (ratio == 0) ExpLim->GetYaxis()->SetTitle("signal significance");
  if (ratio == 1) ExpLim->GetYaxis()->SetTitle("ratio to expected");
  if (ratio == 2) ExpLim->GetYaxis()->SetTitle("(observed - expected) / 1#sigma");

  ExpLim->GetXaxis()->SetTitleOffset(1.2);
  ExpLim->GetYaxis()->SetTitleOffset(1.0);
  ExpLim->GetYaxis()->SetNdivisions(505);

  //  ExpLim->SetLineStyle(kDotted);
  ExpLim->SetLineWidth(3);
  ExpLim->SetLineColor(kMagenta+3);

  ExpLim   ->Draw("ac");


  // Observed limit
  //----------------------------------------------------------------------------
  TGraph* ObsLim = NULL;

  if (drawObserved) {

    float yObs[npoints];    

    for (UInt_t i=0; i<npoints; ++i) {

      yObs[i] = vObsLimit.at(i);

      if (ratio == 1) {
	yObs[i] /= vMeanExpLimit.at(i);
      }
      if (yObs[i] > max) max = yObs[i];
      if (yObs[i] < min) min = yObs[i];
    }

    ObsLim = new TGraph(npoints, x, yObs);
    ObsLim->SetLineWidth(4);
    ObsLim->SetLineColor(kBlue+1);
    ObsLim->SetMarkerStyle(kFullCircle);
    ObsLim->Draw("l");
  }


  // y-axis
  //----------------------------------------------------------------------------
  if (canvas->GetLogy())
    ExpLim->GetYaxis()->SetRangeUser(0.5, max+150);
  else
    ExpLim->GetYaxis()->SetRangeUser(min-1.5, max+7);


  // canvas dimensions
  //----------------------------------------------------------------------------
  canvas->Update();

  Float_t uxmin = canvas->GetUxmin();
  Float_t uxmax = canvas->GetUxmax();
  Float_t uymin = canvas->GetUymin();
  Float_t uymax = canvas->GetUymax();


  // x-axis ticks
  //----------------------------------------------------------------------------
  if (canvas->GetLogx()) {

    ExpLim->GetXaxis()->SetNdivisions(0);

    TLine tick;

    tick.SetLineWidth(1);
    tick.SetLineColor(1);

    for (Int_t i=100; i<=600; i+=10) {
	
      if (i < mhmin || i > mhmax) continue;

      Float_t xx = i;

      if (canvas->GetLogy()) {
	//tick.DrawLine(xx, pow(10,uymin), xx, pow(10,uymin) + (i%100 == 0 ? 0.025 : 0.01));
	tick.DrawLine(xx, pow(10,uymin), xx, (i%100 == 0 ? pow(10,uymin+0.04*(uymax-uymin)) : pow(10,uymin+0.022*(uymax-uymin))));
      } else {
	tick.DrawLine(xx, uymin, xx, uymin + (i%100 == 0 ? 0.75 : 0.3));
      }
    }
    
    canvas->Update();

    // TLatex
    //--------------------------------------------------------------------------
    Float_t ylatex = (canvas->GetLogy()) ? pow(10,uymin) - 0.02 : uymin - 0.75;

    Float_t xbins[6] = {100, 200, 300, 400, 500, 600};

    while (mhmin > xbins[0]) xbins[0] += 10;

    for (Int_t i=0; i<6; i++) {
      
      if (xbins[i] < mhmin || xbins[i] > mhmax) continue;
    
      TLatex* latex = new TLatex(xbins[i], ylatex, Form("%.0f", xbins[i]));
      
      latex->SetTextAlign(  22);
      latex->SetTextFont (  42);
      latex->SetTextSize (0.05);
    
      latex->Draw("same");
    }
  }


  // Cosmetics
  //----------------------------------------------------------------------------
  DrawTLatex(0.92, 0.850, 0.050, "CMS preliminary");
  DrawTLatex(0.92, 0.795, 0.035, title.c_str());
  float lumif = atof(luminosity.c_str());
  if(lumif<10) DrawTLatex(0.92, 0.740, 0.035, TString("L = "+ luminosity+" (7 TeV)").Data());
  else if(lumif<20) DrawTLatex(0.92, 0.740, 0.035, TString("L = "+ luminosity+" (8 TeV)").Data());
  else DrawTLatex(0.92, 0.740, 0.035, TString("L = 4.9 fb^{-1} (7 TeV) + 19.6 fb^{-1} (8 TeV)").Data());

  TLegend* leg = new TLegend(0.18, 0.66, 0.355, 0.88, "");

  leg->SetBorderSize(    0);
  leg->SetFillColor (    0);
  leg->SetFillStyle (    0);
  leg->SetTextFont  (   42);
  leg->SetTextSize  (0.035);

  leg->AddEntry(ExpLim,    " expected",      "l");

  if ( drawObserved ) 
	  leg->AddEntry(ObsLim,    " observed",             "l");
  leg->Draw("same");


  // Unit line
  //----------------------------------------------------------------------------
  canvas->Update();

  if (!ratio) {

    TLine *lineDisc = NULL;
    TLine *lineObs = NULL;

    if (canvas->GetLogx()) {
      lineDisc = new TLine(pow(10,uxmin), 5, pow(10,uxmax), 5);
      lineObs = new TLine(pow(10,uxmin), 3, pow(10,uxmax), 3);
    }
    else {
      lineDisc = new TLine(uxmin, 5, uxmax, 5);
      lineObs = new TLine(uxmin, 3, uxmax, 3);
    }

    lineDisc->SetLineColor(kRed+1);
    lineObs->SetLineColor(kGreen+1);
    lineDisc->SetLineWidth(2);
    lineObs->SetLineWidth(2);
    //    lineDisc->Draw("same");
    //    lineObs->Draw("same");

    canvas->SetGridx(kTRUE);
    canvas->SetGridy(kTRUE);

    if ( drawObserved ) 
      ObsLim->Draw("same");
  }
    

  // Save
  //----------------------------------------------------------------------------
  canvas->GetFrame()->DrawClone();
  canvas->RedrawAxis();
  canvas->Update();

  canvas->SaveAs(Form("%s.%s", ctitle.Data(), format.c_str()));
}
