#ifndef ClassEfficiencyStudy_h
#define ClassEfficiencyStudy_h

#include <vector>
#include "Monitor.hh"
#include "HiggsBase.h"

#include <TVector3.h>
#include <TLorentzVector.h>

class ClassEfficiencyStudy : public HiggsBase {
 public:
    ClassEfficiencyStudy(TTree *tree=0,const char* ECALSection="EB");
    virtual ~ClassEfficiencyStudy();
    void Loop();
    void displayEfficiencies();
    
private:

  bool isClass(int eleInd, const char* gsfclass);
  float Fisher(int eleInd);
  void setKinematics(int,int);  
  void addVariables();

  // private members
  Counters *_goldenCounter, *_bigbremCounter, *_narrowCounter, *_showeringCounter, *_globalCounter;
  Selection* _goldenSelection, *_bigbremSelection, *_narrowSelection, *_showeringSelection;
  bool _verbose;
  const char* _ECALSection;
  
};
#endif
