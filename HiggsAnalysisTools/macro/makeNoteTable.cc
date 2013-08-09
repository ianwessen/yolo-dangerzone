// g++ makeNoteTable.cc -o makeTable `root-config --libs --cflags --glibs` -Wno-deprecated

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>
using namespace std;

int main() {

  string spacing;

  // --------------------------------------------------------------------------
  // taking data top
  ifstream inFileTopData;
  inFileTopData.open("TopYieldsData_ForTable_0j.txt");
  if (!inFileTopData) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massTopData[30];
  float mumuTopData[30], errmumuTopData[30];
  float mueTopData[30],  errmueTopData[30];
  float emuTopData[30],  erremuTopData[30];
  float eeTopData[30],   erreeTopData[30];

  int ifcTopData = 0;
  while (!inFileTopData.eof()) {
    inFileTopData >> massTopData[ifcTopData] 
		  >> mumuTopData[ifcTopData] >> spacing >> errmumuTopData[ifcTopData] 
		  >> mueTopData[ifcTopData]  >> spacing >> errmueTopData[ifcTopData] 
		  >> emuTopData[ifcTopData]  >> spacing >> erremuTopData[ifcTopData] 
		  >> eeTopData[ifcTopData]   >> spacing >> erreeTopData[ifcTopData];
    ifcTopData++;
  }

  // --------------------------------------------------------------------------
  // taking MC top
  ifstream inFileTopMC;
  inFileTopMC.open("TopYieldsMC_ForTable_0j.txt");
  if (!inFileTopMC) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massTopMC[30];
  float mumuTopMC[30], errmumuTopMC[30];
  float mueTopMC[30],  errmueTopMC[30];
  float emuTopMC[30],  erremuTopMC[30];
  float eeTopMC[30],   erreeTopMC[30];

  int ifcTopMC = 0;
  while (!inFileTopMC.eof()) {
    inFileTopMC >> massTopMC[ifcTopMC] 
		>> mumuTopMC[ifcTopMC] >> spacing >> errmumuTopMC[ifcTopMC] 
		>> mueTopMC[ifcTopMC]  >> spacing >> errmueTopMC[ifcTopMC] 
		>> emuTopMC[ifcTopMC]  >> spacing >> erremuTopMC[ifcTopMC] 
		>> eeTopMC[ifcTopMC]   >> spacing >> erreeTopMC[ifcTopMC];
    ifcTopMC++;
  }



  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // taking data wjets
  ifstream inFileWjetsData;
  inFileWjetsData.open("WjetsYieldsData_ForTable_0j.txt");
  if (!inFileWjetsData) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massWjetsData[30];
  float mumuWjetsData[30], errmumuWjetsData[30];
  float mueWjetsData[30],  errmueWjetsData[30];
  float emuWjetsData[30],  erremuWjetsData[30];
  float eeWjetsData[30],   erreeWjetsData[30];

  int ifcWjetsData = 0;
  while (!inFileWjetsData.eof()) {
    inFileWjetsData >> massWjetsData[ifcWjetsData] 
		    >> mumuWjetsData[ifcWjetsData] >> spacing >> errmumuWjetsData[ifcWjetsData] 
		    >> mueWjetsData[ifcWjetsData]  >> spacing >> errmueWjetsData[ifcWjetsData] 
		    >> emuWjetsData[ifcWjetsData]  >> spacing >> erremuWjetsData[ifcWjetsData] 
		    >> eeWjetsData[ifcWjetsData]   >> spacing >> erreeWjetsData[ifcWjetsData];
    ifcWjetsData++;
  }

  // --------------------------------------------------------------------------
  // taking MC wjets
  ifstream inFileWjetsMC;
  inFileWjetsMC.open("WjetsYieldsMC_ForTable_0j.txt");
  if (!inFileWjetsMC) {
    cout << "Unable to open top MC file";
    return(0); 
  }
  
  int massWjetsMC[30];
  float mumuWjetsMC[30], errmumuWjetsMC[30];
  float mueWjetsMC[30],  errmueWjetsMC[30];
  float emuWjetsMC[30],  erremuWjetsMC[30];
  float eeWjetsMC[30],   erreeWjetsMC[30];

  int ifcWjetsMC = 0;
  while (!inFileWjetsMC.eof()) {
    inFileWjetsMC >> massWjetsMC[ifcWjetsMC] 
		  >> mumuWjetsMC[ifcWjetsMC] >> spacing >> errmumuWjetsMC[ifcWjetsMC] 
		  >> mueWjetsMC[ifcWjetsMC]  >> spacing >> errmueWjetsMC[ifcWjetsMC] 
		  >> emuWjetsMC[ifcWjetsMC]  >> spacing >> erremuWjetsMC[ifcWjetsMC] 
		  >> eeWjetsMC[ifcWjetsMC]   >> spacing >> erreeWjetsMC[ifcWjetsMC];
    ifcWjetsMC++;
  }



  // --------------------------------------------------------------------------
  // --------------------------------------------------------------------------
  // taking data dy
  ifstream inFileDYData;
  inFileDYData.open("DYYieldsData_ForTable_0j.txt");
  if (!inFileDYData) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massDYData[30];
  float mumuDYData[30], errmumuDYData[30];
  float mueDYData[30],  errmueDYData[30];
  float emuDYData[30],  erremuDYData[30];
  float eeDYData[30],   erreeDYData[30];
  
  int ifcDYData = 0;
  while (!inFileDYData.eof()) {
    inFileDYData >> massDYData[ifcDYData] 
		 >> mumuDYData[ifcDYData] >> spacing >> errmumuDYData[ifcDYData] 
		 >> mueDYData[ifcDYData]  >> spacing >> errmueDYData[ifcDYData] 
		 >> emuDYData[ifcDYData]  >> spacing >> erremuDYData[ifcDYData] 
		 >> eeDYData[ifcDYData]   >> spacing >> erreeDYData[ifcDYData];
    ifcDYData++;
  }
  
  // --------------------------------------------------------------------------
  // taking MC dy
  ifstream inFileDYMC;
  inFileDYMC.open("DYYieldsMC_ForTable_0j.txt");
  if (!inFileDYMC) {
    cout << "Unable to open top MC file";
    return(0); 
  }
  
  int massDYMC[30];
  float mumuDYMC[30], errmumuDYMC[30];
  float mueDYMC[30],  errmueDYMC[30];
  float emuDYMC[30],  erremuDYMC[30];
  float eeDYMC[30],   erreeDYMC[30];
  
  int ifcDYMC = 0;
  while (!inFileDYMC.eof()) {
    inFileDYMC >> massDYMC[ifcDYMC] 
	       >> mumuDYMC[ifcDYMC] >> spacing >> errmumuDYMC[ifcDYMC] 
	       >> mueDYMC[ifcDYMC]  >> spacing >> errmueDYMC[ifcDYMC] 
	       >> emuDYMC[ifcDYMC]  >> spacing >> erremuDYMC[ifcDYMC] 
	       >> eeDYMC[ifcDYMC]   >> spacing >> erreeDYMC[ifcDYMC];
    ifcDYMC++;
  }
  
  
  
  // --------------------------------------------------------------------------  
  // --------------------------------------------------------------------------
  // taking data WW
  ifstream inFileWWData;
  inFileWWData.open("WWYieldsData_ForTable_0j.txt");
  if (!inFileWWData) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massWWData[30];
  float mumuWWData[30], errmumuWWData[30];
  float mueWWData[30],  errmueWWData[30];
  float emuWWData[30],  erremuWWData[30];
  float eeWWData[30],   erreeWWData[30];
  
  int ifcWWData = 0;
  while (!inFileWWData.eof()) {
    inFileWWData >> massWWData[ifcWWData] 
		 >> mumuWWData[ifcWWData] >> spacing >> errmumuWWData[ifcWWData] 
		 >> mueWWData[ifcWWData]  >> spacing >> errmueWWData[ifcWWData] 
		 >> emuWWData[ifcWWData]  >> spacing >> erremuWWData[ifcWWData] 
		 >> eeWWData[ifcWWData]   >> spacing >> erreeWWData[ifcWWData];
    ifcWWData++;
  }
  
  // --------------------------------------------------------------------------
  // taking MC WW
  ifstream inFileWWMC;
  inFileWWMC.open("WWYieldsMC_ForTable_0j.txt");
  if (!inFileWWMC) {
    cout << "Unable to open top data file";
    return(0); 
  }
  
  int massWWMC[30];
  float mumuWWMC[30], errmumuWWMC[30];
  float mueWWMC[30],  errmueWWMC[30];
  float emuWWMC[30],  erremuWWMC[30];
  float eeWWMC[30],   erreeWWMC[30];

  int ifcWWMC = 0;
  while (!inFileWWMC.eof()) {
    inFileWWMC >> massWWMC[ifcWWMC] 
	       >> mumuWWMC[ifcWWMC] >> spacing >> errmumuWWMC[ifcWWMC] 
	       >> mueWWMC[ifcWWMC]  >> spacing >> errmueWWMC[ifcWWMC] 
	       >> emuWWMC[ifcWWMC]  >> spacing >> erremuWWMC[ifcWWMC] 
	       >> eeWWMC[ifcWWMC]   >> spacing >> erreeWWMC[ifcWWMC];
    ifcWWMC++;
  }
  

  // --------------------------------------------------------------------------
  // looking for possible problems with files
  if ( ifcTopMC   != ifcTopData )   cout << "problem in top:   data and mc # of mass are different" << endl;
  if ( ifcWjetsMC != ifcWjetsData ) cout << "problem in Wjets: data and mc # of mass are different" << endl;
  if ( ifcDYMC    != ifcDYData )    cout << "problem in DY:    data and mc # of mass are different" << endl;
  if ( ifcWWMC    != ifcWWData )    cout << "problem in WW:    data and mc # of mass are different" << endl;
  if ( ifcTopMC   != ifcWjetsMC )   cout << "problem: different samples have different # of masses" << endl;
  if ( ifcTopMC   != ifcDYMC )      cout << "problem: different samples have different # of masses" << endl;
  if ( ifcTopMC   != ifcWWMC )      cout << "problem: different samples have different # of masses" << endl;




  // --------------------------------------------------------------------------
  // summing up all channels
  float allTopData[30],   errAllTopData[30];
  float allWjetsData[30], errAllWjetsData[30];
  float allDYData[30],    errAllDYData[30];
  float allWWData[30],    errAllWWData[30];
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {

    allTopData[iMass]   = mumuTopData[iMass]   + mueTopData[iMass]   + emuTopData[iMass]   + eeTopData[iMass];
    allWjetsData[iMass] = mumuWjetsData[iMass] + mueWjetsData[iMass] + emuWjetsData[iMass] + eeWjetsData[iMass];
    allDYData[iMass]    = mumuDYData[iMass]    + mueDYData[iMass]    + emuDYData[iMass]    + eeDYData[iMass];
    allWWData[iMass]    = mumuWWData[iMass]    + mueWWData[iMass]    + emuWWData[iMass]    + eeWWData[iMass];

    errAllTopData[iMass]   = sqrt ( pow(errmumuTopData[iMass],2)   + pow(errmueTopData[iMass],2)   + pow(erremuTopData[iMass],2)   + pow(erreeTopData[iMass],2) );
    errAllWjetsData[iMass] = sqrt ( pow(errmumuWjetsData[iMass],2) + pow(errmueWjetsData[iMass],2) + pow(erremuWjetsData[iMass],2) + pow(erreeWjetsData[iMass],2) );
    errAllDYData[iMass]    = sqrt ( pow(errmumuDYData[iMass],2)    + pow(errmueDYData[iMass],2)    + pow(erremuDYData[iMass],2)    + pow(erreeDYData[iMass],2) );
    errAllWWData[iMass]    = sqrt ( pow(errmumuWWData[iMass],2)    + pow(errmueWWData[iMass],2)    + pow(erremuWWData[iMass],2)    + pow(erreeWWData[iMass],2) );
  }

  float allTopMC[30],   errAllTopMC[30];
  float allWjetsMC[30], errAllWjetsMC[30];
  float allDYMC[30],    errAllDYMC[30];
  float allWWMC[30],    errAllWWMC[30];
  for (int iMass=0; iMass<(ifcTopMC-1); iMass++) {

    allTopMC[iMass]   = mumuTopMC[iMass]   + mueTopMC[iMass]   + emuTopMC[iMass]   + eeTopMC[iMass];
    allWjetsMC[iMass] = mumuWjetsMC[iMass] + mueWjetsMC[iMass] + emuWjetsMC[iMass] + eeWjetsMC[iMass];
    allDYMC[iMass]    = mumuDYMC[iMass]    + mueDYMC[iMass]    + emuDYMC[iMass]    + eeDYMC[iMass];
    allWWMC[iMass]    = mumuWWMC[iMass]    + mueWWMC[iMass]    + emuWWMC[iMass]    + eeWWMC[iMass];

    errAllTopMC[iMass]   = sqrt ( pow(errmumuTopMC[iMass],2)   + pow(errmueTopMC[iMass],2)   + pow(erremuTopMC[iMass],2)   + pow(erreeTopMC[iMass],2) );
    errAllWjetsMC[iMass] = sqrt ( pow(errmumuWjetsMC[iMass],2) + pow(errmueWjetsMC[iMass],2) + pow(erremuWjetsMC[iMass],2) + pow(erreeWjetsMC[iMass],2) );
    errAllDYMC[iMass]    = sqrt ( pow(errmumuDYMC[iMass],2)    + pow(errmueDYMC[iMass],2)    + pow(erremuDYMC[iMass],2)    + pow(erreeDYMC[iMass],2) );
    errAllWWMC[iMass]    = sqrt ( pow(errmumuWWMC[iMass],2)    + pow(errmueWWMC[iMass],2)    + pow(erremuWWMC[iMass],2)    + pow(erreeWWMC[iMass],2) );
  }


  // --------------------------------------------------------------------------
  // preparing summary tables:

  char namefile[200];
  sprintf(namefile,"yields_byCut.tex");
  ofstream textfile;
  textfile.open(namefile, ios_base::app);
  textfile.precision(2);

  textfile << "\\documentclass{article}" << endl;
  textfile << "\\setlength\\textheight{9.8in}" << endl;
  textfile << "\\usepackage{rotating}" << endl;
  textfile << "\\begin{document}" << endl;


  // mu-mu -------------------------------
  textfile << "\\begin{sidewaystable}[p]" << endl
	   << "\\begin{tiny}" << endl
	   << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c c|c c|c c|c c|}" << endl;
  textfile << "\\hline" << endl;
  
  textfile << "mass & Top MC & Top Data & Wjets MC & Wjets Data & DY MC & DY Data & WW MC & WW Data \t\\\\" << endl;
  textfile << "\\hline" << endl;

  textfile << "\t\\\\" << endl;
  
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {
    textfile << massTopMC[iMass] 
	     << "\t&\t" <<  mumuTopMC[iMass]     << "$\\pm$" << errmumuTopMC[iMass] 
	     << "\t&\t" <<  mumuTopData[iMass]   << "$\\pm$" << errmumuTopData[iMass] 
	     << "\t&\t" <<  mumuWjetsMC[iMass]   << "$\\pm$" << errmumuWjetsMC[iMass] 
	     << "\t&\t" <<  mumuWjetsData[iMass] << "$\\pm$" << errmumuWjetsData[iMass] 
	     << "\t&\t" <<  mumuDYMC[iMass]      << "$\\pm$" << errmumuDYMC[iMass] 
	     << "\t&\t" <<  mumuDYData[iMass]    << "$\\pm$" << errmumuDYData[iMass] 
	     << "\t&\t" <<  mumuWWMC[iMass]      << "$\\pm$" << errmumuWWMC[iMass] 
	     << "\t&\t" <<  mumuWWData[iMass]    << "$\\pm$" << errmumuWWData[iMass] 
	     << "\t\\\\" << endl;
  } 
  textfile << "\t\\\\" << endl;
  
  textfile << "\\hline"              << endl
	   << "\\end{tabular}"       << endl
	   << "\\end{center}"        << endl
	   << "\\end{tiny}"          << endl
	   << "\\caption{MuMu} "     << endl
	   << "\\end{sidewaystable}" << endl;

  // e-e -------------------------------
  textfile << "\\begin{sidewaystable}[p]" << endl
	   << "\\begin{tiny}" << endl
	   << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c c|c c|c c| c c|}" << endl;
  textfile << "\\hline" << endl;
  
  textfile << "mass & Top MC & Top Data & Wjets MC & Wjets Data & DY MC & DY Data & WW MC & WW Data \t\\\\" << endl;
  textfile << "\\hline" << endl;
  
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {
    textfile << massTopMC[iMass] 
	     << "\t&\t" <<  eeTopMC[iMass]     << "$\\pm$" << erreeTopMC[iMass] 
	     << "\t&\t" <<  eeTopData[iMass]   << "$\\pm$" << erreeTopData[iMass] 
	     << "\t&\t" <<  eeWjetsMC[iMass]   << "$\\pm$" << erreeWjetsMC[iMass] 
	     << "\t&\t" <<  eeWjetsData[iMass] << "$\\pm$" << erreeWjetsData[iMass] 
	     << "\t&\t" <<  eeDYMC[iMass]      << "$\\pm$" << erreeDYMC[iMass] 
	     << "\t&\t" <<  eeDYData[iMass]    << "$\\pm$" << erreeDYData[iMass] 
	     << "\t&\t" <<  eeWWMC[iMass]      << "$\\pm$" << erreeWWMC[iMass] 
	     << "\t&\t" <<  eeWWData[iMass]    << "$\\pm$" << erreeWWData[iMass] 
	     << "\t\\\\" << endl;
  } 
  textfile << "\t\\\\" << endl;
  
  textfile << "\\hline"              << endl
	   << "\\end{tabular}"       << endl
	   << "\\end{center}"        << endl
	   << "\\end{tiny}"          << endl
	   << "\\caption{e-e} "       << endl
	   << "\\end{sidewaystable}" << endl;
  
  
  // mu-e -------------------------------
  textfile << "\\begin{sidewaystable}[p]" << endl
	   << "\\begin{tiny}" << endl
	   << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c c|c c|c c| c c|}" << endl;
  textfile << "\\hline" << endl;
  
  textfile << "mass & Top MC & Top Data & Wjets MC & Wjets Data & DY MC & DY Data & WW MC & WW Data \t\\\\" << endl;
  textfile << "\\hline" << endl;
  
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {
    textfile << massTopMC[iMass] 
	     << "\t&\t" <<  mueTopMC[iMass]     << "$\\pm$" << errmueTopMC[iMass] 
	     << "\t&\t" <<  mueTopData[iMass]   << "$\\pm$" << errmueTopData[iMass] 
	     << "\t&\t" <<  mueWjetsMC[iMass]   << "$\\pm$" << errmueWjetsMC[iMass] 
	     << "\t&\t" <<  mueWjetsData[iMass] << "$\\pm$" << errmueWjetsData[iMass] 
	     << "\t&\t" <<  mueDYMC[iMass]      << "$\\pm$" << errmueDYMC[iMass] 
	     << "\t&\t" <<  mueDYData[iMass]    << "$\\pm$" << errmueDYData[iMass] 
	     << "\t&\t" <<  mueWWMC[iMass]      << "$\\pm$" << errmueWWMC[iMass] 
	     << "\t&\t" <<  mueWWData[iMass]    << "$\\pm$" << errmueWWData[iMass] 
	     << "\t\\\\" << endl;
  } 
  textfile << "\t\\\\" << endl;
  
  textfile << "\\hline"              << endl
	   << "\\end{tabular}"       << endl
	   << "\\end{center}"        << endl
	   << "\\end{tiny}"          << endl
	   << "\\caption{Mue} "     << endl
	   << "\\end{sidewaystable}" << endl;
  
  // e-mu -------------------------------
  textfile << "\\begin{sidewaystable}[p]" << endl
	   << "\\begin{tiny}" << endl
	   << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c c|c c|c c| c c|}" << endl;
  textfile << "\\hline" << endl;
  
  textfile << "mass & Top MC & Top Data & Wjets MC & Wjets Data & DY MC & DY Data & WW MC & WW Data \t\\\\" << endl;
  textfile << "\\hline" << endl;
  
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {
    textfile << massTopMC[iMass] 
	     << "\t&\t" <<  emuTopMC[iMass]     << "$\\pm$" << erremuTopMC[iMass] 
	     << "\t&\t" <<  emuTopData[iMass]   << "$\\pm$" << erremuTopData[iMass] 
	     << "\t&\t" <<  emuWjetsMC[iMass]   << "$\\pm$" << erremuWjetsMC[iMass] 
	     << "\t&\t" <<  emuWjetsData[iMass] << "$\\pm$" << erremuWjetsData[iMass] 
	     << "\t&\t" <<  emuDYMC[iMass]      << "$\\pm$" << erremuDYMC[iMass] 
	     << "\t&\t" <<  emuDYData[iMass]    << "$\\pm$" << erremuDYData[iMass] 
	     << "\t&\t" <<  emuWWMC[iMass]      << "$\\pm$" << erremuWWMC[iMass] 
	     << "\t&\t" <<  emuWWData[iMass]    << "$\\pm$" << erremuWWData[iMass] 
	     << "\t\\\\" << endl;
  } 
  textfile << "\t\\\\" << endl;
  
  textfile << "\\hline"              << endl
	   << "\\end{tabular}"       << endl
	   << "\\end{center}"        << endl
	   << "\\end{tiny}"          << endl
	   << "\\caption{Emu} "     << endl
	   << "\\end{sidewaystable}" << endl;


  // all -------------------------------
  textfile << "\\begin{sidewaystable}[p]" << endl
	   << "\\begin{tiny}" << endl
	   << "\\begin{center}" << endl;
  textfile << "\\begin{tabular}{|c|c c|c c|c c| c c|}" << endl;
  textfile << "\\hline" << endl;
  
  textfile << "mass & Top MC & Top Data & Wjets MC & Wjets Data & DY MC & DY Data & WW MC & WW Data \t\\\\" << endl;
  textfile << "\\hline" << endl;
  
  for (int iMass=0; iMass<(ifcTopData-1); iMass++) {
    textfile << massTopMC[iMass] 
	     << "\t&\t" <<  allTopMC[iMass]     << "$\\pm$" << errAllTopMC[iMass] 
	     << "\t&\t" <<  allTopData[iMass]   << "$\\pm$" << errAllTopData[iMass] 
	     << "\t&\t" <<  allWjetsMC[iMass]   << "$\\pm$" << errAllWjetsMC[iMass] 
	     << "\t&\t" <<  allWjetsData[iMass] << "$\\pm$" << errAllWjetsData[iMass] 
	     << "\t&\t" <<  allDYMC[iMass]      << "$\\pm$" << errAllDYMC[iMass] 
	     << "\t&\t" <<  allDYData[iMass]    << "$\\pm$" << errAllDYData[iMass] 
	     << "\t&\t" <<  allWWMC[iMass]      << "$\\pm$" << errAllWWMC[iMass] 
	     << "\t&\t" <<  allWWData[iMass]    << "$\\pm$" << errAllWWData[iMass] 
	     << "\t\\\\" << endl;
  } 
  textfile << "\t\\\\" << endl;
  
  textfile << "\\hline"              << endl
	   << "\\end{tabular}"       << endl
	   << "\\end{center}"        << endl
	   << "\\end{tiny}"          << endl
	   << "\\caption{All} "     << endl
	   << "\\end{sidewaystable}" << endl;

  textfile << "\\end{document}"      << endl;
}
