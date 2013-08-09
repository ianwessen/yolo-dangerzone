#include <iostream>
#include <string>

using namespace std;

double weight(string sample, double ngen, double xsec, double filtereff, double lumi = 10, double prescale = 1);

int main(int argc, char* argv[]) {
  
  double wantLumi = 1.0; //pb-1  
  double w;

  std::cout << "sample" << "\t" << "xsec" << "\t" << "weight" << std:: endl;
  
  w = weight("H120", 57100, 0.247143 * 4./9., 1.0, wantLumi);
  w = weight("H130", 109995, 0.452859 * 4./9., 1.0, wantLumi);
  w = weight("H140", 46007, 0.649260 * 4./9., 1.0, wantLumi);
  w = weight("H150", 38472, 0.787871 * 4./9., 1.0, wantLumi);
  w = weight("H160", 105540, 0.897043 * 4./9., 1.0, wantLumi);
  w = weight("H170", 47627, 0.808914 * 4./9., 1.0, wantLumi);
  w = weight("H200", 109988, 0.422487 * 4./9., 1.0, wantLumi);
  w = weight("H210", 65993, 0.370845 * 4./9., 1.0, wantLumi);
  w = weight("H220", 15992, 0.331814 * 4./9., 1.0, wantLumi);
  w = weight("H230", 65998, 0.300268 * 4./9., 1.0, wantLumi);
  w = weight("H250", 57509, 0.251763 * 4./9., 1.0, wantLumi);
  w = weight("H300", 2243, 0.181931 * 4./9., 1.0, wantLumi);
  w = weight("H400", 31989, 0.125106 * 4./9., 1.0, wantLumi);
  w = weight("H500", 21910, 0.047826 * 4./9., 1.0, wantLumi);
  w = weight("H600", 44940, 0.018751 * 4./9., 1.0, wantLumi);

  // MCFM NLO
  //  w = weight("Wgamma", 107050, 23.2*1.8, 1.0, wantLumi);
  w = weight("WW", 110000, 4.50347, 1.0, wantLumi);
  w = weight("ggWW", 9994, 0.1538, 1.0, wantLumi);
  w = weight("WZ", 110000, 0.599442, 1.0, wantLumi);
  w = weight("ZZ", 2.11337e+06, 5.9, 1.0, wantLumi);

  // MCFM NLO
  w = weight("Wenu", 1.63475e+06, 31314./3. * 0.742, 1.0, wantLumi);
  w = weight("Wmunu", 5.33094e+06, 31314./3. * 0.742, 1.0, wantLumi);
  w = weight("Wtaunu", 3.91925e+06, 31314./3. * 0.742, 1.0, wantLumi);

  w = weight("Zee_Hi",  1.99899e+06,  4998/3., 1.0, wantLumi);
  w = weight("Zmm_Hi",  1.89893e+06,  4998/3., 1.0, wantLumi);
  w = weight("Ztautau_Hi",  1.84822e+06, 4998./3., 1.0, wantLumi);

  w = weight("Zee_Lo",  1952,  2659./3., 1.0, wantLumi);
  w = weight("Zmm_Lo",  49368,  2659./3., 1.0, wantLumi);
  w = weight("Ztautau_Lo", 1.94143e+06, 2659./3., 1.0, wantLumi);

  w = weight("TTbar", 1.19875e+06,   157.5, 1.0, wantLumi);

  w = weight("SingleTop_sChannel", 444967, 4.21 * (0.1080*3), 1.0, wantLumi);
  w = weight("SingleTop_tChannel", 484060, 64.6*(3*0.1080), 1.0, wantLumi);
  w = weight("SingleTop_tWChannel", 494961, 10.6 * (0.1080*3), 1.0, wantLumi);

  return 0;

}

double weight(string sample, double ngen, double xsec, double filtereff, 
	   double lumi, double prescale) {

  float W = xsec * filtereff * lumi * prescale / ngen;

  char line[1000];
  sprintf(line,"addWeights(\"results/merged/%s_ee.root\", %e);", sample.c_str(), W); 

  //  std::cout << sample << "\t" << xsec << "\t" << W << std:: endl;
  std::cout << line << endl;

  return W;

}
