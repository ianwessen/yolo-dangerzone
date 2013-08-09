#include <sstream>
#include <iostream>
#include <istream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>

using namespace std;

class DatacardParser {

public:

  DatacardParser(string datacardfile) { _datacardfile=datacardfile; }
  ~DatacardParser() {}

  float getRelUncertainty(string param);

private:
  string _datacardfile;

};

float DatacardParser::getRelUncertainty(string param) {

  float unc=-1000.;

  std::ifstream file(_datacardfile.c_str());
  
  if(!file.good()) {
    std::cout << "Error!   Unable to open the file " << _datacardfile << std::endl;
    return unc;
  }
  else {
    std::string par, paramstr, interval; 
    float bias, sigma;
    string line;
    while (!file.eof()) {
      getline(file,line);
      istringstream iss(line);
      iss >> par >> paramstr >> bias >> sigma >> interval;
      if(par.compare(param)==0 && par.length()==param.length()) {
        unc=sigma;
        break;
      }
    }
  }
  return unc;
}
