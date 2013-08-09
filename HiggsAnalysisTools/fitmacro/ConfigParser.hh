#include <sstream>
#include <iostream>
#include <istream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>

#include "findAndReplace.h"

using namespace std;

class ConfigParser {

public:
  ConfigParser(string configfile);
  ~ConfigParser() {}
  vector<float> getLandau(string channel, string process, string syst);
  vector<float> getGaussian(string channel, string process, string syst);
  vector<float> getDoubleGaussian(string channel, string process, string syst);
  float getRelUncertainty(string channel, string process, string param, string syst);
  float getRelUncertainty(string channel, string process1, string process2, string param, string syst1, string syst2);
  std::string updateCardShapeUncertainty(string card, string templateSyst, float relunc);

private:
  vector<string> _pars;
  vector<float> _values, _errors;

};

ConfigParser::ConfigParser(string configfile) {
  std::ifstream file(configfile.c_str());
    if(!file.good()) {
    std::cout << "Selection::Error!   Unable to open the file " << configfile << std::endl;
  }
  else {
    std::string par; 
    float val, err;
    string line;
    while (file.good()) {
      getline (file,line);
      string ini = line.substr(0,1);
      if(ini.find("#")!=string::npos) continue;
      stringstream strstr(line);
      istream_iterator<string> it(strstr);
      istream_iterator<string> end;
      vector<string> results(it, end);
      if(results.size()<5) continue;
      par = results[0];
      val = atof(results[2].c_str());
      err = atof(results[4].c_str());
      // cout << "### Configuration: Parameter " << par << " = " << val << " +/- " << err << endl;
      _pars.push_back(par); _values.push_back(val); _errors.push_back(err);
    }
  }
}

vector<float> ConfigParser::getGaussian(string channel, string process, string syst) {
  vector<float> pars;
  pars.reserve(2);
  int size=0;
  stringstream me,si;
  me << channel << "_" << process << "_me_" << syst;
  si << channel << "_" << process << "_si_" << syst;
  for(int i=0;i<(int)_pars.size();++i) {
    if(_pars[i].find(me.str())!=string::npos) {
      pars[0]=_values[i];
      ++size;
    }
    if(_pars[i].find(si.str())!=string::npos) {
      pars[1]=_values[i];
      ++size;
    }
  }
  if(size!=2) cout << " ??? ===> CONFIGURATION PROBLEM <=== ??? " << endl;
  return pars;
}

vector<float> ConfigParser::getLandau(string channel, string process, string syst) {
  return getGaussian(channel,process,syst);
}

vector<float> ConfigParser::getDoubleGaussian(string channel, string process, string syst) {
  vector<float> pars;
  pars.reserve(5);
  int size=0;
  stringstream me1,si1,me2,si2,f;
  me1 << channel << "_" << process << "_me1_" << syst;
  si1 << channel << "_" << process << "_si1_" << syst;
  me2 << channel << "_" << process << "_me2_" << syst;
  si2 << channel << "_" << process << "_si2_" << syst;
  f << channel << "_" << process << "_f_" << syst;
  for(int i=0;i<(int)_pars.size();++i) {
    if(_pars[i].find(me1.str())!=string::npos) {
      pars[0]=_values[i];
      ++size;
    }
    if(_pars[i].find(si1.str())!=string::npos) {
      pars[1]=_values[i];
      ++size;
    }
    if(_pars[i].find(me2.str())!=string::npos) {
      pars[2]=_values[i];
      ++size;
    }
    if(_pars[i].find(si2.str())!=string::npos) {
      pars[3]=_values[i];
      ++size;
    }
    if(_pars[i].find(f.str())!=string::npos) {
      pars[4]=_values[i];
      ++size;
    }

  }
  if(size!=5) cout << " ??? ===> CONFIGURATION PROBLEM <=== ??? " << endl;
  return pars;
}

float ConfigParser::getRelUncertainty(string channel, string process, string param, string syst) {
  float nominal=-999.; 
  float alt=-999.;
  stringstream param_nom, param_alt;
  param_nom << channel << "_" << process << "_" << param << "_nominals";
  param_alt << channel << "_" << process << "_" << param << "_" << syst;

  for(int i=0; i<(int)_pars.size(); ++i) {
    if(_pars[i].find(param_nom.str())!=string::npos) nominal=_values[i];
    if(_pars[i].find(param_alt.str())!=string::npos) alt=_values[i];
    if(nominal!=-999 && alt!=-999) break;
  }
  if(nominal==-999 || alt==-999) {
    cout << "!!! SYSTEMATICS. CONFIGURATION PROBLEM: nominal value for " << param
         << "  or systematics one: " << param_alt.str() << " not found in config file!!!" << endl;
    return 0.;
  }
  return floor(fabs(nominal-alt)/nominal * 1e+4)/1e+4;
}

float ConfigParser::getRelUncertainty(string channel, string process1, string process2, string param, string syst1, string syst2) {
  float nominal=-999.; 
  float alt=-999.;
  stringstream param_nom, param_alt;
  param_nom << channel << "_" << process1 << "_" << param << "_" << syst1;
  param_alt << channel << "_" << process2 << "_" << param << "_" << syst2;

  for(int i=0; i<(int)_pars.size(); ++i) {
    if(_pars[i].find(param_nom.str())!=string::npos) nominal=_values[i];
    if(_pars[i].find(param_alt.str())!=string::npos) alt=_values[i];
    if(nominal!=-999 && alt!=-999) break;
  }
  if(nominal==-999 || alt==-999) {
    cout << "!!! SYSTEMATICS. CONFIGURATION PROBLEM: nominal value for " << param
         << "  or systematics one: " << param_alt.str() << " not found in config file!!!" << endl;
    return 0.;
  }
  return floor(fabs(nominal-alt)/nominal * 1e+4)/1e+4;
}

std::string ConfigParser::updateCardShapeUncertainty(string card, string templateSyst, float relunc) {
  if(relunc>0) {
    if(relunc<0.15) return findAndReplace(card, templateSyst, relunc);
    else {
      cout << "CONFIG WARNING! PARAMETER " << templateSyst << " has > 50% uncertanty. Check the fit. Automatically switching it off... " << endl;
      return findAndReplace(card, templateSyst, 0.0001);
    }
  }
  else return findAndReplace(card, templateSyst, 0.0001);
}
