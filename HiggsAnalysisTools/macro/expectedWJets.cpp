// to compile with
// g++ expectedWJets.cpp -o exp `root-config --libs --cflags --glibs` -Wno-deprecated

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
  float lumi = 100.;   // pb

  // w+jets cross sections = sigma(pp->W+jet; W-> e nu) // pb
  float xSec[8];
  xSec[0] = 32190.;   // 0-15
  xSec[1] = 1722.;    // 15-20
  xSec[2] = 1914.;    // 20-30
  xSec[3] = 1541.;    // 30-50
  xSec[4] = 706.2;   // 50-80
  xSec[5] = 236.2;   // 80-120
  xSec[6] = 70.72;   // 120-170
  xSec[7] = 20.36;   // 170-230
  
  // nSelected events after eleID
  float nSelEleId[8];
  nSelEleId[0] = 0.;
  nSelEleId[1] = 1.;
  nSelEleId[2] = 0.;
  nSelEleId[3] = 0.;
  nSelEleId[4] = 1.;
  nSelEleId[5] = 0.;
  nSelEleId[6] = 0.;
  nSelEleId[7] = 1.;

  // nBeforeEleID events
  float nTotEleId[8];
  nTotEleId[0] = 15470.;
  nTotEleId[1] = 28742.;
  nTotEleId[2] = 56067.;
  nTotEleId[3] = 51365.;
  nTotEleId[4] = 29796.;
  nTotEleId[5] = 27117.;
  nTotEleId[6] = 24750.;
  nTotEleId[7] = 17250.;
  
  // nSelected events after kine
  float nSelKine[8]; 
  nSelKine[0] = 0 ;
  nSelKine[1] = 0 ;
  nSelKine[2] = 1 ;
  nSelKine[3] = 2 ;
  nSelKine[4] = 1 ;
  nSelKine[5] = 0 ;
  nSelKine[6] = 1 ;
  nSelKine[7] = 1 ;

  // nTot events before kine
  float nTotKine[8];
  nTotKine[0] = 23.;
  nTotKine[1] = 118.;
  nTotKine[2] = 408.;
  nTotKine[3] = 846.;
  nTotKine[4] = 794.;
  nTotKine[5] = 824.;
  nTotKine[6] = 705.;
  nTotKine[7] = 690.;

  float effEleId[8];
  float effKine[8];
  float deffEleId[8];
  float deffKine[8];
  float nExp[8];

  // 3 instead on 1 o 2
  for (int uu=0; uu<8; uu++){
    if( (nSelEleId[uu] > 0) && (nSelEleId[uu] < 3) ){ nSelEleId[uu] = 3.; }
    if( (nSelKine[uu] > 0)  && (nSelKine[uu] < 3) ) { nSelKine[uu]  = 3.; }
  }

  // efficiency
  for (int uu=0; uu<8; uu++){
    effEleId[uu]  = nSelEleId[uu]/nTotEleId[uu];  				
    deffEleId[uu] = pow(nSelEleId[uu]*(1- (nSelEleId[uu]/nTotEleId[uu]) ),0.5)/nTotEleId[uu];
    cout << "bin = " << uu << "   effEleId[uu] = " << effEleId[uu] << " +/- " << deffEleId[uu] << endl;   
    effKine[uu]   = nSelKine[uu]/nTotKine[uu];  				
    deffKine[uu]  = pow(nSelKine[uu]*(1- (nSelKine[uu]/nTotKine[uu]) ),0.5)/nTotKine[uu];
    cout << "bin = " << uu << "   effKine[uu] = " << effKine[uu] << " +/- " << deffKine[uu] << endl;   
  }
    
  // number of expected events  
  for (int uu=0; uu<8; uu++){
    nExp[uu] = (effEleId[uu]*effKine[uu])*xSec[uu]*lumi;
    if( (nSelEleId[uu] == 0) || ( nSelKine[uu] == 0) ){ nExp[uu] = (3./nTotEleId[uu])*xSec[uu]*lumi; }
  }

  cout << "expected events: " << endl;
  float tot = 0.;
  for (int uu=0; uu<8; uu++){ 

    if(nSelEleId[uu] <= 3 || nSelKine[uu] <= 3) {
      cout << "sample " << uu << ", nExp < " << nExp[uu] << endl;
    }
    else
      cout << "sample " << uu << ", nExp = " << nExp[uu] << endl; 
    tot = tot + nExp[uu];
  }
  cout << "totale: " << tot << endl;

}
