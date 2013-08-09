// to compile with
// g++ expectedHWWtt.cpp -o exp `root-config --libs --cflags --glibs` -Wno-deprecated

//! c++ includes              
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iostream.h>
#include <unistd.h>
#include <fstream>
#include <math.h>

// Main program
int main ( int argc, char **argv)
{ 
  int tausi = -200;
  if(argc==2){ tausi = atoi(argv[1]); }
  else { std::cout << "insert 0 (no tau)  or 1 (with tau)" << std::endl; }
  
  float lumi = 100.;   // pb

  // H production cross-section = gg + VBF  (pb)
  float xSecH[6];
  xSecH[0] = (31.7 + 4.14);    // 130
  xSecH[1] = (27.8 + 3.83);    // 140
  xSecH[2] = (24.6 + 3.56);    // 150
  xSecH[3] = (21.9 + 3.32);    // 160
  xSecH[4] = (19.7 + 3.09);    // 170
  xSecH[5] = (17.8 + 2.88);    // 180
  //xSecH[6] = (16.2 + 2.71);    // 190
  //xSecH[7] = (14.8 + 2.53);    // 200
  
  // H -> WW BR
  float brHWW[8];
  brHWW[0] = 0.2888;      // 130
  brHWW[1] = 0.4854;      // 140
  brHWW[2] = 0.6832;      // 150
  brHWW[3] = 0.9016;      // 160
  brHWW[4] = 0.9654;      // 170  
  brHWW[5] = 0.9346;      // 180
  // brHWW[6] = 0.7761;   // 190
  // brHWW[7] = 0.7347;   // 200

  // WW, ttbar production xsec (pb)
  float xSecWW = 114.3;
  float xSectt = 840.;

  // ZZ, DY cross sections
  float xSecZZ   = 100000.;  // fix: che decadimenti consideriamo qui??
  float xSecBRZZ = 100000.;  // fix: che decadimenti consideriamo qui??

  // W -> leptons BRs
  float brWenu   = 0.1075;     // BR(W->e nu)
  float brWtaunu = 0.1125;     // BR(W->tau nu)
  float brTaue   = 0.1784;     // BR(tau -> e nu nu)

  // xsec x BR vari
  float xSecBRH[6];
  for(int uu=0; uu<6; uu++){
    if(tausi)  xSecBRH[uu] = xSecH[uu]*brHWW[uu]*(brWenu*brWenu + brWenu*brWtaunu*brTaue + brWtaunu*brTaue*brWtaunu*brTaue);
    if(!tausi) xSecBRH[uu] = xSecH[uu]*brHWW[uu]*(brWenu*brWenu);
  }

  float xSecBRWW;
  if(tausi)  xSecBRWW = xSecWW*(brWenu*brWenu + brWenu*brWtaunu*brTaue + brWtaunu*brTaue*brWtaunu*brTaue);
  if(!tausi) xSecBRWW = xSecWW*(brWenu*brWenu);

  // float xSecBRZZ;
  // if(tausi)  xSecBRZZ = xSecZZ*(brWenu*brWenu + brWenu*brWtaunu*brTaue + brWtaunu*brTaue*brWtaunu*brTaue);
  // if(!tausi) xSecBRZZ = xSecZZ*(brWenu*brWenu);

  float xSecBRtt;
  if(tausi)  xSecBRtt = xSectt*(brWenu*brWenu + brWenu*brWtaunu*brTaue + brWtaunu*brTaue*brWtaunu*brTaue);
  if(!tausi) xSecBRtt = xSectt*(brWenu*brWenu);
  

  // efficiencies
  float eff[6], erreff[6];
  float effWW[6], erreffWW[6];
  float effZZ[6], erreffZZ[6];
  float efftt[6], errefftt[6];

  // Higgs eff
  eff[0] = 0.027;   erreff[0] = 0.002;
  eff[1] = 0.023;   erreff[1] = 0.003;
  eff[2] = 0.022;   erreff[2] = 0.003;
  eff[3] = 0.025;   erreff[3] = 0.002;
  eff[4] = 0.021;   erreff[4] = 0.002;
  eff[5] = 0.011;   erreff[5] = 0.002;

  // WW eff
  effWW[0] = 0.007;   erreffWW[0] = 0.002;
  effWW[1] = 0.007;   erreffWW[1] = 0.002;
  effWW[2] = 0.005;   erreffWW[2] = 0.001;
  effWW[3] = 0.001;   erreffWW[3] = 0.001;
  effWW[4] = 0.001;   erreffWW[4] = 0.001;
  effWW[5] = 0.001;   erreffWW[5] = 0.001;

  // ZZ eff  - fix: to be included
  effZZ[0] = 0.00015;    erreffZZ[0] = 0.001;
  effZZ[1] = 0.00015;    erreffZZ[1] = 0.001;
  effZZ[2] = 0.00015;    erreffZZ[2] = 0.001;
  effZZ[3] = 0.00015;    erreffZZ[3] = 0.001;
  effZZ[4] = 0.00015;    erreffZZ[4] = 0.001;
  effZZ[5] = 0.00015;    erreffZZ[5] = 0.001;

  // tt~ eff
  efftt[0] = 0.00015;    errefftt[0] = 0.001;
  efftt[1] = 0.00015;    errefftt[1] = 0.001;
  efftt[2] = 0.00015;    errefftt[2] = 0.001;
  efftt[3] = 0.00015;    errefftt[3] = 0.001;
  efftt[4] = 0.00015;    errefftt[4] = 0.001;
  efftt[5] = 0.00015;    errefftt[5] = 0.001;
  


  // number of expected events  
  for (int uu=0; uu<6; uu++){
    cout << "selection for higgs mass = " << uu << std::endl;
    float nExpHiggs = xSecBRH[uu] * eff[uu] * lumi;
    float nExpHiggsErr = xSecBRH[uu] * erreff[uu] * lumi;
    float nExpWW    = xSecBRWW * effWW[uu] * lumi;
    float nExpWWErr = xSecBRWW * erreffWW[uu] * lumi;
    float nExptt    = xSecBRtt * efftt[uu] * lumi;
    float nExpttErr = xSecBRtt * errefftt[uu] * lumi;
    float nExpZZ    = xSecBRZZ * effZZ[uu] * lumi;
    float nExpZZErr = xSecBRZZ * erreffZZ[uu] * lumi;
    cout <<  "number of expected events for lumi = " << lumi << " pb^-1 :" << endl;
    cout << "\tHiggs signal = " << nExpHiggs << " +/-" << nExpHiggsErr << endl;
    cout << "\tWW non res. = "  << nExpWW    << " +/-" << nExpWWErr << endl;
    cout << "\ttt~ = "          << nExptt    << " +/-" << nExpttErr << endl;
    cout << "ZZ = "             << nExpZZ    << " +/-" << nExpZZErr << endl;
    float statSign = nExpHiggs/sqrt(11./10.*(nExpWW+nExptt+nExpZZ));
    cout << "with a statistical significance of " << statSign << endl;
    cout << "W+jet background not considered.n\n\n\n" << endl;
  }

}
