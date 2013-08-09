#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLegend.h>
#include <vector>
#include <iostream.h>

void drawPdfs(int isendcap) {

  // Lumi
  float usedLumi = 437.0 ; // pb-1
  float wantedLumi = 100.0 ;  // pb-1

  TFile file("Plots/ZTaP/pdfs.root");  
  
  const char* var;
  std::vector<TH1F> histo;

  for (int ii=0; ii<5; ii++){
    if(ii==0){ var = "dPhiVtx";}
    if(ii==1){ var = "dEtaVtx";}
    if(ii==2){ var = "EoPout";}
    if(ii==3){ var = "HoE";}
    if(ii==4){ var = "shapeFisher";}

    std::cout << var << std::endl;
    std::string baseName(var);
    
    std::string isEE;
    if(isendcap==1) isEE = "[1]";
    else isEE = "[0]";
    std::string nameEle0 = baseName+"0"+isEE;
    std::string nameEle1 = baseName+"1"+isEE;
    std::string nameEle2 = baseName+"2"+isEE;
    std::string nameEle3 = baseName+"3"+isEE;

    std::cout << "getting " << nameEle0 << std::endl;

    TH1F* ele0 = (TH1F*)file.Get(nameEle0.c_str());
    TH1F* ele1 = (TH1F*)file.Get(nameEle1.c_str());
    TH1F* ele2 = (TH1F*)file.Get(nameEle2.c_str());
    TH1F* ele3 = (TH1F*)file.Get(nameEle3.c_str());

    int pCol4[10];
    for ( int i = 0; i < 10; i++ ) pCol4[i] = 401+i;
    
    TCanvas c0;
    c0.SetLogy(1);
    gStyle->SetOptStat(" ");
    gStyle->SetPalette(10, pCol4);
    
    ele0->SetLineColor(1);
    ele1->SetLineColor(2);
    ele2->SetLineColor(3);
    ele3->SetLineColor(4);
    //had ->SetLineColor(6);
    ele0->GetXaxis()->SetTitle(var);
    ele1->GetXaxis()->SetTitle(var);
    ele2->GetXaxis()->SetTitle(var);
    ele3->GetXaxis()->SetTitle(var);
    //had ->GetXaxis()->SetTitle(var);
    ele0->SetTitle("");
    ele1->SetTitle("");
    ele2->SetTitle("");
    ele3->SetTitle("");
    //had->SetTitle("");

    //double scaleH = 1./had->GetSum();
    ele0->Scale(usedLumi/wantedLumi);
    ele1->Scale(usedLumi/wantedLumi);
    ele2->Scale(usedLumi/wantedLumi);
    ele3->Scale(usedLumi/wantedLumi);
    //had->Scale(scaleH);

    double max = TMath::Max(ele0->GetMaximum(),ele1->GetMaximum());
    max = TMath::Max(max,ele2->GetMaximum());
    max = TMath::Max(max,ele3->GetMaximum());

    ele0->SetMaximum(max+sqrt(max));
    ele0->Draw();
    ele1->Draw("same");
    ele2->Draw("same");
    ele3->Draw("same");
    //had ->Draw("same");

    TLegend* leg = new TLegend(0.15,0.75,0.40,0.90);
    leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.03); 
    leg->SetFillColor(0); 
    leg->AddEntry(ele0,"golden electrons","l");
    leg->AddEntry(ele1,"narrow electrons","l");
    leg->AddEntry(ele2,"big brem electrons","l");
    leg->AddEntry(ele3,"showering electrons","l");
    //leg->AddEntry(had,"qcd jets","l");
    leg->Draw();
    
    std::string psname(var);
    psname+=(isendcap) ? "_EE.ps" : "_EB.ps";
    c0.SaveAs(psname.c_str());
  }
}

