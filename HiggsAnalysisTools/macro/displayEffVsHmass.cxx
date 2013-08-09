#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>

void displayEffVsHiggsMass() {

  // the data: heavily hardcoded.......
  
  // --- signal ---
  std::vector< std::vector<std::pair<std::string,float> > > selEvents;

  std::vector<std::pair<std::string,float> > h130;
  h130.push_back(std::make_pair("event", 46491));
  h130.push_back(std::make_pair("MCtruth",      5234));
  h130.push_back(std::make_pair("trigger",      3536));
  h130.push_back(std::make_pair("nRecoEle",     2334));
  h130.push_back(std::make_pair("twoGoodRec",   1900));
  h130.push_back(std::make_pair("jetVeto",      1153));
  h130.push_back(std::make_pair("eleID",        624));
  h130.push_back(std::make_pair("trackerIsol",  547));
  h130.push_back(std::make_pair("hcalIsol",     483));
  h130.push_back(std::make_pair("MET",  269));
  h130.push_back(std::make_pair("deltaPhi",     230));
  h130.push_back(std::make_pair("eleInvMass",   162));
  h130.push_back(std::make_pair("maxPtEle",     139));
  h130.push_back(std::make_pair("minPtEle",     139));
  h130.push_back(std::make_pair("detaLeptons",  139));
  h130.push_back(std::make_pair("final",        139));

  std::vector<std::pair<std::string,float> > h140;
  h140.push_back(std::make_pair("event",       18000));
  h140.push_back(std::make_pair("MCtruth",     2007));
  h140.push_back(std::make_pair("trigger",     1448));
  h140.push_back(std::make_pair("nRecoEle",    926));
  h140.push_back(std::make_pair("twoGoodRec",  791));
  h140.push_back(std::make_pair("jetVeto",     463));
  h140.push_back(std::make_pair("eleID",       256));
  h140.push_back(std::make_pair("trackerIsol", 165));
  h140.push_back(std::make_pair("hcalIsol",    156));
  h140.push_back(std::make_pair("MET", 95));
  h140.push_back(std::make_pair("deltaPhi",    66));
  h140.push_back(std::make_pair("eleInvMass",  50));
  h140.push_back(std::make_pair("maxPtEle",    47));
  h140.push_back(std::make_pair("minPtEle",    47));
  h140.push_back(std::make_pair("detaLeptons", 47));
  h140.push_back(std::make_pair("final",       47));

  
  std::vector<std::pair<std::string,float> > h150;
  h150.push_back(std::make_pair("event",     19000));
  h150.push_back(std::make_pair("MCtruth",     2079));
  h150.push_back(std::make_pair("trigger",     1515));
  h150.push_back(std::make_pair("nRecoEle",    1041));
  h150.push_back(std::make_pair("twoGoodRec",  912));
  h150.push_back(std::make_pair("jetVeto",     533));
  h150.push_back(std::make_pair("eleID",       297));
  h150.push_back(std::make_pair("trackerIsol", 198));
  h150.push_back(std::make_pair("hcalIsol",    181));
  h150.push_back(std::make_pair("MET", 128));
  h150.push_back(std::make_pair("deltaPhi",    77));
  h150.push_back(std::make_pair("eleInvMass",  60));
  h150.push_back(std::make_pair("maxPtEle",    46));
  h150.push_back(std::make_pair("minPtEle",    45));
  h150.push_back(std::make_pair("detaLeptons", 45));
  h150.push_back(std::make_pair("final",       45));

  std::vector<std::pair<std::string,float> > h160;
  h160.push_back(std::make_pair("event",       40375));
  h160.push_back(std::make_pair("MCtruth",     4479));
  h160.push_back(std::make_pair("trigger",     3286));
  h160.push_back(std::make_pair("nRecoEle",    2162));
  h160.push_back(std::make_pair("twoGoodRec",  1940));
  h160.push_back(std::make_pair("jetVeto",     1119));
  h160.push_back(std::make_pair("eleID",       681));
  h160.push_back(std::make_pair("trackerIsol", 610));
  h160.push_back(std::make_pair("hcalIsol",    569));
  h160.push_back(std::make_pair("MET", 337));
  h160.push_back(std::make_pair("deltaPhi",    215));
  h160.push_back(std::make_pair("eleInvMass",  162));
  h160.push_back(std::make_pair("maxPtEle",    131));
  h160.push_back(std::make_pair("minPtEle",    114));
  h160.push_back(std::make_pair("detaLeptons", 114));
  h160.push_back(std::make_pair("final",       114));
  
  std::vector<std::pair<std::string,float> > h170;
  h170.push_back(std::make_pair("event",       47140));
  h170.push_back(std::make_pair("MCtruth",     5207));
  h170.push_back(std::make_pair("trigger",     3940));
  h170.push_back(std::make_pair("nRecoEle",    2581));
  h170.push_back(std::make_pair("twoGoodRec",  2292));
  h170.push_back(std::make_pair("jetVeto",     1362));
  h170.push_back(std::make_pair("eleID",       837));
  h170.push_back(std::make_pair("trackerIsol", 758));
  h170.push_back(std::make_pair("hcalIsol",    706));
  h170.push_back(std::make_pair("MET", 448));
  h170.push_back(std::make_pair("deltaPhi",    254));
  h170.push_back(std::make_pair("eleInvMass",  182));
  h170.push_back(std::make_pair("maxPtEle",    151));
  h170.push_back(std::make_pair("minPtEle",    109));
  h170.push_back(std::make_pair("detaLeptons", 109));
  h170.push_back(std::make_pair("final",       109));

  std::vector<std::pair<std::string,float> > h180;
  h180.push_back(std::make_pair("event",       20000)); 
  h180.push_back(std::make_pair("MCtruth",     2234));  
  h180.push_back(std::make_pair("trigger",     1714));  
  h180.push_back(std::make_pair("nRecoEle",    1167));  
  h180.push_back(std::make_pair("twoGoodRec",  1042));  
  h180.push_back(std::make_pair("jetVeto",     592));   
  h180.push_back(std::make_pair("eleID",       369));   
  h180.push_back(std::make_pair("trackerIsol", 335));   
  h180.push_back(std::make_pair("hcalIsol",    308));   
  h180.push_back(std::make_pair("MET", 195));           
  h180.push_back(std::make_pair("deltaPhi",    87));    
  h180.push_back(std::make_pair("eleInvMass",  51));    
  h180.push_back(std::make_pair("maxPtEle",    32));    
  h180.push_back(std::make_pair("minPtEle",    24));    
  h180.push_back(std::make_pair("detaLeptons", 24));    
  h180.push_back(std::make_pair("final",       24));    

  selEvents.push_back(h130);
  selEvents.push_back(h140);
  selEvents.push_back(h150);
  selEvents.push_back(h160);
  selEvents.push_back(h170);
  selEvents.push_back(h180);




  // --- now start the job ---
  // ---------- efficiency(Higgs mass)  -----------

  std::vector<TGraph*> effGraph;
  // create a graph for each cut
  int color=1;
  std::vector<std::pair<std::string,float> >::const_iterator cutItr;
  for(cutItr=h180.begin()+1;cutItr!=h180.end();++cutItr) {
    TGraph *graph = new TGraph( selEvents.size() );
    graph->SetName((cutItr->first).c_str());
    if(color==5) color++;
    if(color==10) color=30;
    graph->SetLineColor(color);
    graph->SetMarkerColor(color);
    graph->SetMarkerStyle(8);
    graph->SetMarkerSize(1);
    effGraph.push_back(graph);
    if(color<10) ++color;
    else color+=2;
  }


  // now fill the graphs with the single cut efficiency
  int i=0;
  float higgsMass=130;
  std::vector< std::vector<std::pair<std::string,float> > >::const_iterator higgsMassItr;
  for(higgsMassItr=selEvents.begin();higgsMassItr!=selEvents.end();higgsMassItr++) {
    std::cout << " %%%%%%  Higgs mass = " << higgsMass << " GeV/c^2" << std::endl;
    std::vector<TGraph*>::const_iterator graphItr=effGraph.begin();
    
    std::vector<std::pair<std::string,float> >::const_iterator cutItr=higgsMassItr->begin();
    std::vector<std::pair<std::string,float> >::const_iterator cutItrPrevious=higgsMassItr->begin();
    cutItr++;
    for(;cutItr!=higgsMassItr->end();++cutItr) {
      float eff = cutItr->second/cutItrPrevious->second;
      std::cout << "cut = " << cutItr->first 
		<< " nSel = " << cutItr->second << "   nPrevious = " << cutItrPrevious->second
		<< " eff = " << eff << std::endl;
      (*graphItr)->SetPoint(i,higgsMass,eff);
      ++cutItrPrevious;
      ++graphItr;
    }
    i++;
    higgsMass+=10;
  }


  TCanvas cX;
  TLegend* leg = new TLegend(0.1,0.1,0.60,0.40);
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.02); 
  leg->SetFillColor(0); 


  // now draw the graphs
  std::vector<TGraph*>::const_iterator graphItr=effGraph.begin()+1; // +1 skip the MCtruth
  
  (*graphItr)->SetMaximum(1.1);
  (*graphItr)->SetMinimum(0.0); 
  (*graphItr)->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  (*graphItr)->GetYaxis()->SetTitle("efficiency w.r.t. previous cut");
  (*graphItr)->SetTitle("");
  int graphCounter=0;
  for(;graphItr!=effGraph.end();++graphItr) {
    (*graphItr)->SetMaximum(1.1);
    (*graphItr)->SetMinimum(0.0);
    if(strcmp((*graphItr)->GetName(),"MCtruth")!=0 && 
       strcmp((*graphItr)->GetName(),"detaLeptons")!=0 &&
       strcmp((*graphItr)->GetName(),"final")!=0) {
      std::cout << "now plotting " << (*graphItr)->GetName() << std::endl;
      if(graphCounter==0) (*graphItr)->Draw("APC");
      else (*graphItr)->Draw("PC");
      leg->AddEntry((*graphItr),(*graphItr)->GetName(),"pl");
      graphCounter++;
    }
  }
  leg->Draw();

  cX.SaveAs("boh.eps");



}

void displayStatisticalSignificance(float lumi=100.) {

  // --- Higgs final eff ---
  std::vector<float> HiggsEff; 
  HiggsEff.push_back(0.027);
  HiggsEff.push_back(0.023);
  HiggsEff.push_back(0.022);
  HiggsEff.push_back(0.025);
  HiggsEff.push_back(0.021);
  HiggsEff.push_back(0.011);

  // --- WW background ---
  std::vector<float> wwEff;
  wwEff.push_back(0.007);
  wwEff.push_back(0.007);
  wwEff.push_back(0.005);
  wwEff.push_back(0.001);
  wwEff.push_back(0.001);
  wwEff.push_back(0.001);

  // --- ttbar background ---
  std::vector<float> ttEff;
  ttEff.push_back(0.00015);
  ttEff.push_back(0.00015);
  ttEff.push_back(0.00015);
  ttEff.push_back(0.00015);
  ttEff.push_back(0.00015);
  ttEff.push_back(0.00015);
  
  // --- Xsec ---
  std::vector<float> HiggsXSecBR;
  HiggsXSecBR.push_back(0.118); // 130 
  HiggsXSecBR.push_back(0.176); // 140 
  HiggsXSecBR.push_back(0.220); // 150
  HiggsXSecBR.push_back(0.260); // 160 
  HiggsXSecBR.push_back(0.251); // 170
  HiggsXSecBR.push_back(0.221); // 180

  float xSecBRWW = 1.350 ;
  float xSecBRtt = 9.700 ;


  // ---- statistical significance ------
  TGraph *statSignificance = new TGraph( HiggsXSecBR.size() );
  TGraph *SoverB = new TGraph( HiggsXSecBR.size() );

  float higgsMass=130.;
  for(int i=0; i<(int)HiggsXSecBR.size();++i) {
    float nHiggs = lumi * HiggsXSecBR[i] * HiggsEff[i];
    float nWW = lumi * xSecBRWW * wwEff[i];
    float ntt = lumi * xSecBRtt * ttEff[i];
    // assume 10% syst uncertainty on background: nb -> nb + 1/10. nb
    // stat significance
    float nSigma = nHiggs / sqrt(11./10.* (nWW+ntt));
    statSignificance->SetPoint(i,higgsMass,nSigma);

    float SoverBratio = nHiggs / (11./10.* (nWW+ntt));
    SoverB->SetPoint(i,higgsMass,SoverBratio);
    higgsMass+=10;
  }

  char title[100];
  sprintf(title,"H->WW->2e 2 #nu statistical significance for L=%i pb^{-1}",(int)lumi);

  TCanvas cX;

  // draw statistical significance
  statSignificance->SetLineColor(34);
  statSignificance->SetLineWidth(2);
  statSignificance->SetMarkerColor(34);
  statSignificance->SetMarkerStyle(8);
  statSignificance->SetMarkerSize(2);
  statSignificance->SetMinimum(0);
  statSignificance->SetTitle(title);
  statSignificance->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  statSignificance->GetYaxis()->SetTitle("n_{Higgs}/(n_{Bkg} + 0.1 n_{Bkg})^{0.5}");

  statSignificance->Draw("ACP");
  cX.SaveAs("statSign.eps");


  // draw S/B ratio
  TCanvas cY;
  SoverB->SetLineColor(34);
  SoverB->SetLineWidth(2);
  SoverB->SetMarkerColor(34);
  SoverB->SetMarkerStyle(8);
  SoverB->SetMarkerSize(2);
  SoverB->SetMinimum(0);
  SoverB->SetTitle("N(Higgs)/N(WW non resonant + tt)");
  SoverB->GetXaxis()->SetTitle("Higgs mass (GeV/c^{2})");
  SoverB->GetYaxis()->SetTitle("n_{Higgs}/(n_{Bkg} + 0.1 n_{Bkg})");

  SoverB->Draw("ACP");
  cY.SaveAs("SoverB.eps");


}

void all() {
  
  displayEffVsHiggsMass();
  displayStatisticalSignificance(100); // pb-1

}
