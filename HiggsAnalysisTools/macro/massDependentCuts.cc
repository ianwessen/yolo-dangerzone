#include <TString.h>
#include <map>

TString higgsCuts(int mH, bool out, int njets) {
  
  TString dymvacut = (njets==0) ? TString("(dymva1>0.6 || !sameflav) && njet==0") : TString("(dymva1>0.3 || !sameflav) && njet==1");
  TString finalmetcut = (njets==0) ? TString("((mpmet>45 && dphilljet<165) || !sameflav) && njet==0") : TString("((mpmet>45 && dphilljet<165) || !sameflav) && njet==1");

  std::map<int,TString> cuts;
  cuts.insert(std::make_pair(110,dymvacut + TString(" && pt1>20 && dphill*180./TMath::Pi()<115 &&  mth>80  &&  mth<110")));
  cuts.insert(std::make_pair(115,dymvacut + TString(" && pt1>20 && dphill*180./TMath::Pi()<115 &&  mth>80  &&  mth<110")));
  cuts.insert(std::make_pair(118,dymvacut + TString(" && pt1>20 && dphill*180./TMath::Pi()<115 &&  mth>80  &&  mth<115")));
  cuts.insert(std::make_pair(120,dymvacut + TString(" && pt1>20 && dphill*180./TMath::Pi()<115 &&  mth>80  &&  mth<120")));
  cuts.insert(std::make_pair(122,dymvacut + TString(" && pt1>21 && dphill*180./TMath::Pi()<110 &&  mth>80  &&  mth<121")));
  cuts.insert(std::make_pair(124,dymvacut + TString(" && pt1>22 && dphill*180./TMath::Pi()<105 &&  mth>80  &&  mth<122")));
  cuts.insert(std::make_pair(125,dymvacut + TString(" && pt1>22 && dphill*180./TMath::Pi()<105 &&  mth>80  &&  mth<122")));
  cuts.insert(std::make_pair(126,dymvacut + TString(" && pt1>23 && dphill*180./TMath::Pi()<100 &&  mth>80  &&  mth<123")));
  cuts.insert(std::make_pair(128,dymvacut + TString(" && pt1>24 && dphill*180./TMath::Pi()<95  &&  mth>80  &&  mth<124")));
  cuts.insert(std::make_pair(130,dymvacut + TString(" && pt1>25 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<125")));
  cuts.insert(std::make_pair(135,dymvacut + TString(" && pt1>25 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<128")));
  cuts.insert(std::make_pair(140,dymvacut + TString(" && pt1>25 && pt2>15 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<130")));
  cuts.insert(std::make_pair(145,finalmetcut + TString(" && pt1>25 && pt2>15 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<130")));
  cuts.insert(std::make_pair(150,finalmetcut + TString(" && pt1>27 && pt2>25 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<150")));
  cuts.insert(std::make_pair(155,finalmetcut + TString(" && pt1>29 && pt2>25 && dphill*180./TMath::Pi()<90  &&  mth>80  &&  mth<155")));
  cuts.insert(std::make_pair(160,finalmetcut + TString(" && pt1>30 && pt2>25 && dphill*180./TMath::Pi()<60  &&  mth>90  &&  mth<160")));
  cuts.insert(std::make_pair(170,finalmetcut + TString(" && pt1>34 && pt2>25 && dphill*180./TMath::Pi()<60  &&  mth>110 &&  mth<170")));
  cuts.insert(std::make_pair(180,finalmetcut + TString(" && pt1>36 && pt2>25 && dphill*180./TMath::Pi()<70  &&  mth>120 &&  mth<180")));
  cuts.insert(std::make_pair(190,finalmetcut + TString(" && pt1>38 && pt2>25 && dphill*180./TMath::Pi()<90  &&  mth>120 &&  mth<190")));
  cuts.insert(std::make_pair(200,finalmetcut + TString(" && pt1>40 && pt2>25 && dphill*180./TMath::Pi()<100 &&  mth>120 &&  mth<200")));
  cuts.insert(std::make_pair(250,finalmetcut + TString(" && pt1>55 && pt2>25 && dphill*180./TMath::Pi()<140 &&  mth>120 &&  mth<250")));
  cuts.insert(std::make_pair(300,finalmetcut + TString(" && pt1>70 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<300")));
  cuts.insert(std::make_pair(350,finalmetcut + TString(" && pt1>80 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<350")));
  cuts.insert(std::make_pair(400,finalmetcut + TString(" && pt1>90 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<400")));
  cuts.insert(std::make_pair(450,finalmetcut + TString(" && pt1>110 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<450")));
  cuts.insert(std::make_pair(500,finalmetcut + TString(" && pt1>120 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<500")));
  cuts.insert(std::make_pair(550,finalmetcut + TString(" && pt1>130 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<550")));
  cuts.insert(std::make_pair(600,finalmetcut + TString(" && pt1>140 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<600")));
  cuts.insert(std::make_pair(700,finalmetcut + TString(" && pt1>150 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<700")));
  cuts.insert(std::make_pair(800,finalmetcut + TString(" && pt1>160 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<800")));
  cuts.insert(std::make_pair(900,finalmetcut + TString(" && pt1>170 && pt2>25 && dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<900")));
  cuts.insert(std::make_pair(1000,finalmetcut + TString(" && pt1>180 && pt2>25 &&  dphill*180./TMath::Pi()<175 &&  mth>120 &&  mth<1000")));

  std::map<int,TString> cutsOut;
  cutsOut.insert(std::make_pair(110,cuts[110]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(115,cuts[115]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(118,cuts[118]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(120,cuts[120]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(122,cuts[122]+TString(" && mll<41")));
  cutsOut.insert(std::make_pair(124,cuts[124]+TString(" && mll<42")));
  cutsOut.insert(std::make_pair(125,cuts[125]+TString(" && mll<42")));
  cutsOut.insert(std::make_pair(126,cuts[126]+TString(" && mll<43")));
  cutsOut.insert(std::make_pair(128,cuts[128]+TString(" && mll<44")));
  cutsOut.insert(std::make_pair(130,cuts[130]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(135,cuts[135]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(140,cuts[140]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(145,cuts[145]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(150,cuts[150]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(155,cuts[155]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(160,cuts[160]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(170,cuts[170]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(180,cuts[180]+TString(" && mll<60")));
  cutsOut.insert(std::make_pair(190,cuts[190]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(200,cuts[200]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(250,cuts[250]+TString(" && mll<150")));
  cutsOut.insert(std::make_pair(300,cuts[300]+TString(" && mll<200")));
  cutsOut.insert(std::make_pair(350,cuts[350]+TString(" && mll<250")));
  cutsOut.insert(std::make_pair(400,cuts[400]+TString(" && mll<300")));
  cutsOut.insert(std::make_pair(450,cuts[450]+TString(" && mll<350")));
  cutsOut.insert(std::make_pair(500,cuts[500]+TString(" && mll<400")));
  cutsOut.insert(std::make_pair(550,cuts[550]+TString(" && mll<450")));
  cutsOut.insert(std::make_pair(600,cuts[600]+TString(" && mll<500")));
  cutsOut.insert(std::make_pair(700,cuts[600]+TString(" && mll<600")));
  cutsOut.insert(std::make_pair(800,cuts[600]+TString(" && mll<700")));
  cutsOut.insert(std::make_pair(900,cuts[600]+TString(" && mll<800")));
  cutsOut.insert(std::make_pair(1000,cuts[600]+TString(" && mll<900")));

  if(out) return cutsOut[mH];
  else return cuts[mH];

}

TString higgsCutsNoMT(int mH, bool out) {
  
  std::map<int,TString> cuts;
  cuts.insert(std::make_pair(110,TString("pt1>20 && dphill*180./TMath::Pi()<115")));
  cuts.insert(std::make_pair(115,TString("pt1>20 && dphill*180./TMath::Pi()<115")));
  cuts.insert(std::make_pair(118,TString("pt1>20 && dphill*180./TMath::Pi()<115")));
  cuts.insert(std::make_pair(120,TString("pt1>20 && dphill*180./TMath::Pi()<115")));
  cuts.insert(std::make_pair(122,TString("pt1>21 && dphill*180./TMath::Pi()<110")));
  cuts.insert(std::make_pair(124,TString("pt1>22 && dphill*180./TMath::Pi()<105")));
  cuts.insert(std::make_pair(125,TString("pt1>22 && dphill*180./TMath::Pi()<105")));
  cuts.insert(std::make_pair(126,TString("pt1>23 && dphill*180./TMath::Pi()<100")));
  cuts.insert(std::make_pair(128,TString("pt1>24 && dphill*180./TMath::Pi()<95")));
  cuts.insert(std::make_pair(130,TString("pt1>25 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(135,TString("pt1>25 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(140,TString("pt1>25 && pt2>15 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(145,TString("pt1>25 && pt2>15 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(150,TString("pt1>27 && pt2>25 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(155,TString("pt1>29 && pt2>25 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(160,TString("pt1>30 && pt2>25 && dphill*180./TMath::Pi()<60")));
  cuts.insert(std::make_pair(170,TString("pt1>34 && pt2>25 && dphill*180./TMath::Pi()<60")));
  cuts.insert(std::make_pair(180,TString("pt1>36 && pt2>25 && dphill*180./TMath::Pi()<70")));
  cuts.insert(std::make_pair(190,TString("pt1>38 && pt2>25 && dphill*180./TMath::Pi()<90")));
  cuts.insert(std::make_pair(200,TString("pt1>40 && pt2>25 && dphill*180./TMath::Pi()<100")));
  cuts.insert(std::make_pair(250,TString("pt1>55 && pt2>25 && dphill*180./TMath::Pi()<140")));
  cuts.insert(std::make_pair(300,TString("pt1>70 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(350,TString("pt1>80 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(400,TString("pt1>90 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(450,TString("pt1>110 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(500,TString("pt1>120 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(550,TString("pt1>130 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(600,TString("pt1>140 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(700,TString("pt1>150 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(800,TString("pt1>160 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(900,TString("pt1>170 && pt2>25 && dphill*180./TMath::Pi()<175")));
  cuts.insert(std::make_pair(1000,TString("pt1>180 && pt2>25 &&  dphill*180./TMath::Pi()<175")));

  std::map<int,TString> cutsOut;
  cutsOut.insert(std::make_pair(110,cuts[110]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(115,cuts[115]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(118,cuts[118]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(120,cuts[120]+TString(" && mll<40")));
  cutsOut.insert(std::make_pair(122,cuts[122]+TString(" && mll<41")));
  cutsOut.insert(std::make_pair(124,cuts[124]+TString(" && mll<42")));
  cutsOut.insert(std::make_pair(125,cuts[125]+TString(" && mll<42")));
  cutsOut.insert(std::make_pair(126,cuts[126]+TString(" && mll<43")));
  cutsOut.insert(std::make_pair(128,cuts[128]+TString(" && mll<44")));
  cutsOut.insert(std::make_pair(130,cuts[130]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(135,cuts[135]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(140,cuts[140]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(145,cuts[145]+TString(" && mll<45")));
  cutsOut.insert(std::make_pair(150,cuts[150]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(155,cuts[155]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(160,cuts[160]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(170,cuts[170]+TString(" && mll<50")));
  cutsOut.insert(std::make_pair(180,cuts[180]+TString(" && mll<60")));
  cutsOut.insert(std::make_pair(190,cuts[190]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(200,cuts[200]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(250,cuts[250]+TString(" && mll<150")));
  cutsOut.insert(std::make_pair(300,cuts[300]+TString(" && mll<200")));
  cutsOut.insert(std::make_pair(350,cuts[350]+TString(" && mll<250")));
  cutsOut.insert(std::make_pair(400,cuts[400]+TString(" && mll<300")));
  cutsOut.insert(std::make_pair(450,cuts[450]+TString(" && mll<350")));
  cutsOut.insert(std::make_pair(500,cuts[500]+TString(" && mll<400")));
  cutsOut.insert(std::make_pair(550,cuts[550]+TString(" && mll<450")));
  cutsOut.insert(std::make_pair(600,cuts[600]+TString(" && mll<500")));
  cutsOut.insert(std::make_pair(700,cuts[600]+TString(" && mll<600")));
  cutsOut.insert(std::make_pair(800,cuts[600]+TString(" && mll<700")));
  cutsOut.insert(std::make_pair(900,cuts[600]+TString(" && mll<800")));
  cutsOut.insert(std::make_pair(1000,cuts[600]+TString(" && mll<900")));

  if(out) return cutsOut[mH];
  else return cuts[mH];

}

TString higgsCutsBDT(int mH, bool out, int njets) {

  TString dymvacut = (njets==0) ? TString("dymva1>0.6 && njet==0") : TString("dymva1>0.3 && njet==1");
  TString finalmetcut = (njets==0) ? TString("(mpmet>45 && dphilljet<165) && njet==0") : TString("(mpmet>45 && dphilljet<165) && njet==1");
  
  std::map<int,TString> cuts;
  cuts.insert(std::make_pair(110,dymvacut + TString(" &&  mth>80 && mth<110")));
  cuts.insert(std::make_pair(115,dymvacut + TString(" &&  mth>80 && mth<115")));
  cuts.insert(std::make_pair(118,dymvacut + TString(" &&  mth>80 && mth<118")));
  cuts.insert(std::make_pair(120,dymvacut + TString(" &&  mth>80 && mth<120")));
  cuts.insert(std::make_pair(122,dymvacut + TString(" &&  mth>80 && mth<122")));
  cuts.insert(std::make_pair(124,dymvacut + TString(" &&  mth>80 && mth<124")));
  cuts.insert(std::make_pair(125,dymvacut + TString(" &&  mth>80 && mth<125")));
  cuts.insert(std::make_pair(126,dymvacut + TString(" &&  mth>80 && mth<126")));
  cuts.insert(std::make_pair(128,dymvacut + TString(" &&  mth>80 && mth<128")));
  cuts.insert(std::make_pair(130,dymvacut + TString(" &&  mth>80 && mth<130")));
  cuts.insert(std::make_pair(135,dymvacut + TString(" &&  mth>80 && mth<135")));
  cuts.insert(std::make_pair(140,dymvacut + TString(" &&  mth>80 && mth<140")));
  cuts.insert(std::make_pair(145,finalmetcut + TString(" &&  mth>80 && mth<145")));
  cuts.insert(std::make_pair(150,finalmetcut + TString("  &&  mth>80 && mth<150")));
  cuts.insert(std::make_pair(155,finalmetcut + TString("  &&  mth>80 && mth<155")));
  cuts.insert(std::make_pair(160,finalmetcut + TString("  &&  mth>80 && mth<160")));
  cuts.insert(std::make_pair(170,finalmetcut + TString("  &&  mth>80 && mth<170")));
  cuts.insert(std::make_pair(180,finalmetcut + TString("  &&  mth>80 && mth<180")));
  cuts.insert(std::make_pair(190,finalmetcut + TString("  &&  mth>80 && mth<190")));
  cuts.insert(std::make_pair(200,finalmetcut + TString("  &&  mth>80 && mth<200")));
  cuts.insert(std::make_pair(250,finalmetcut + TString("  &&  mth>80 && mth<250")));
  cuts.insert(std::make_pair(300,finalmetcut + TString("  &&  mth>80 && mth<300")));
  cuts.insert(std::make_pair(350,finalmetcut + TString("  &&  mth>80 && mth<350")));
  cuts.insert(std::make_pair(400,finalmetcut + TString("  &&  mth>80 && mth<400")));
  cuts.insert(std::make_pair(450,finalmetcut + TString("  &&  mth>80 && mth<450")));
  cuts.insert(std::make_pair(500,finalmetcut + TString("  &&  mth>80 && mth<500")));
  cuts.insert(std::make_pair(550,finalmetcut + TString("  &&  mth>80 && mth<550")));
  cuts.insert(std::make_pair(600,finalmetcut + TString("  &&  mth>80 && mth<600")));
  cuts.insert(std::make_pair(700,finalmetcut + TString("  &&  mth>80 && mth<700")));
  cuts.insert(std::make_pair(800,finalmetcut + TString("  &&  mth>80 && mth<800")));
  cuts.insert(std::make_pair(900,finalmetcut + TString("  &&  mth>80 && mth<900")));
  cuts.insert(std::make_pair(1000,finalmetcut + TString("  &&  mth>80 && mth<1000")));

  std::map<int,TString> cutsOut;
  cutsOut.insert(std::make_pair(110,cuts[110]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(115,cuts[115]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(118,cuts[118]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(120,cuts[120]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(122,cuts[122]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(124,cuts[124]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(125,cuts[125]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(126,cuts[126]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(128,cuts[128]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(130,cuts[130]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(135,cuts[135]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(140,cuts[140]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(145,cuts[145]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(150,cuts[150]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(155,cuts[155]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(160,cuts[160]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(170,cuts[170]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(180,cuts[180]+TString(" && mll<110")));
  cutsOut.insert(std::make_pair(190,cuts[190]+TString(" && mll<120")));
  cutsOut.insert(std::make_pair(200,cuts[200]+TString(" && mll<130")));
  cutsOut.insert(std::make_pair(250,cuts[250]+TString(" && mll<250")));
  cutsOut.insert(std::make_pair(300,cuts[300]+TString(" && mll<300")));
  cutsOut.insert(std::make_pair(350,cuts[350]+TString(" && mll<350")));
  cutsOut.insert(std::make_pair(400,cuts[400]+TString(" && mll<400")));
  cutsOut.insert(std::make_pair(450,cuts[450]+TString(" && mll<450")));
  cutsOut.insert(std::make_pair(500,cuts[500]+TString(" && mll<500")));
  cutsOut.insert(std::make_pair(550,cuts[550]+TString(" && mll<550")));
  cutsOut.insert(std::make_pair(600,cuts[600]+TString(" && mll<600")));
  cutsOut.insert(std::make_pair(700,cuts[600]+TString(" && mll<700")));
  cutsOut.insert(std::make_pair(800,cuts[600]+TString(" && mll<800")));
  cutsOut.insert(std::make_pair(900,cuts[600]+TString(" && mll<900")));
  cutsOut.insert(std::make_pair(1000,cuts[600]+TString(" && mll<1000")));

  if(out) return cutsOut[mH];
  else return cuts[mH];

}

TString higgsCutsBDTNoMT(int mH, bool out) {
  
  std::map<int,TString> cuts;
  cuts.insert(std::make_pair(110,"1"));
  cuts.insert(std::make_pair(115,"1"));
  cuts.insert(std::make_pair(118,"1"));
  cuts.insert(std::make_pair(120,"1"));
  cuts.insert(std::make_pair(122,"1"));
  cuts.insert(std::make_pair(124,"1"));
  cuts.insert(std::make_pair(125,"1"));
  cuts.insert(std::make_pair(126,"1"));
  cuts.insert(std::make_pair(128,"1"));
  cuts.insert(std::make_pair(130,"1"));
  cuts.insert(std::make_pair(135,"1"));
  cuts.insert(std::make_pair(140,"1"));
  cuts.insert(std::make_pair(145,"1"));
  cuts.insert(std::make_pair(150,"1"));
  cuts.insert(std::make_pair(155,"1"));
  cuts.insert(std::make_pair(160,"1"));
  cuts.insert(std::make_pair(170,"1"));
  cuts.insert(std::make_pair(180,"1"));
  cuts.insert(std::make_pair(190,"1"));
  cuts.insert(std::make_pair(200,"1"));
  cuts.insert(std::make_pair(250,"1"));
  cuts.insert(std::make_pair(300,"1"));
  cuts.insert(std::make_pair(350,"1"));
  cuts.insert(std::make_pair(400,"1"));
  cuts.insert(std::make_pair(450,"1"));
  cuts.insert(std::make_pair(500,"1"));
  cuts.insert(std::make_pair(550,"1"));
  cuts.insert(std::make_pair(600,"1"));
  cuts.insert(std::make_pair(700,"1"));
  cuts.insert(std::make_pair(800,"1"));
  cuts.insert(std::make_pair(900,"1"));
  cuts.insert(std::make_pair(1000,"1"));

  std::map<int,TString> cutsOut;
  cutsOut.insert(std::make_pair(110,cuts[110]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(115,cuts[115]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(118,cuts[118]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(120,cuts[120]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(122,cuts[122]+TString(" && mll<70")));
  cutsOut.insert(std::make_pair(124,cuts[124]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(125,cuts[125]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(126,cuts[126]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(128,cuts[128]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(130,cuts[130]+TString(" && mll<80")));
  cutsOut.insert(std::make_pair(135,cuts[135]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(140,cuts[140]+TString(" && mll<90")));
  cutsOut.insert(std::make_pair(145,cuts[145]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(150,cuts[150]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(155,cuts[155]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(160,cuts[160]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(170,cuts[170]+TString(" && mll<100")));
  cutsOut.insert(std::make_pair(180,cuts[180]+TString(" && mll<110")));
  cutsOut.insert(std::make_pair(190,cuts[190]+TString(" && mll<120")));
  cutsOut.insert(std::make_pair(200,cuts[200]+TString(" && mll<130")));
  cutsOut.insert(std::make_pair(250,cuts[250]+TString(" && mll<250")));
  cutsOut.insert(std::make_pair(300,cuts[300]+TString(" && mll<300")));
  cutsOut.insert(std::make_pair(350,cuts[350]+TString(" && mll<350")));
  cutsOut.insert(std::make_pair(400,cuts[400]+TString(" && mll<400")));
  cutsOut.insert(std::make_pair(450,cuts[450]+TString(" && mll<450")));
  cutsOut.insert(std::make_pair(500,cuts[500]+TString(" && mll<500")));
  cutsOut.insert(std::make_pair(550,cuts[550]+TString(" && mll<550")));
  cutsOut.insert(std::make_pair(600,cuts[600]+TString(" && mll<600")));
  cutsOut.insert(std::make_pair(700,cuts[600]+TString(" && mll<700")));
  cutsOut.insert(std::make_pair(800,cuts[600]+TString(" && mll<800")));
  cutsOut.insert(std::make_pair(900,cuts[600]+TString(" && mll<900")));
  cutsOut.insert(std::make_pair(1000,cuts[600]+TString(" && mll<1000")));

  if(out) return cutsOut[mH];
  else return cuts[mH];

}

