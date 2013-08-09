#ifndef FINDANDREPLACE_H
#define FINDANDREPLACE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

int findAndReplace(string infilename, string outfilename, string oldStr, string newStr) {
  string text = "";    
  string line;
  ifstream infile (infilename.c_str());
  if (infile.is_open())
  {
    while ( infile.good() )
    {
      getline (infile,line);
      text += line;
      text += "\n";
    }
    infile.close();
  }

  else {
    cout << "Unable to open file"; 
    return 0;
  }    

  size_t pos = 0;
  while((pos = text.find(oldStr, pos)) != std::string::npos)
  {
     text.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
  
  ofstream outfile;
  outfile.open (outfilename.c_str());
  outfile << text;
  outfile.close();

  return 1;
}

int findAndReplace(string infilename, string outfilename, string oldStr, float val) {

  stringstream ss (stringstream::in | stringstream::out);
  ss << val; 
  string newStr = ss.str();

  return findAndReplace(infilename, outfilename, oldStr, newStr);

}

std::string findAndReplace(string text, string oldStr, string newStr) {
  string outstring = text;
  size_t pos = 0;
  while((pos = outstring.find(oldStr, pos)) != std::string::npos)
  {
     outstring.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
  return outstring;
}

std::string findAndReplace(string text, string oldStr, float val) {

  stringstream ss (stringstream::in | stringstream::out);
  ss << val;
  string newStr = ss.str();

  return findAndReplace(text, oldStr, newStr);

}

std::string findAndReplace(string text, string oldStr, float val1, float val2) {

  stringstream ss (stringstream::in | stringstream::out);
  ss << val1 << "/" << val2;
  string newStr = ss.str();

  return findAndReplace(text, oldStr, newStr);

}

#endif
