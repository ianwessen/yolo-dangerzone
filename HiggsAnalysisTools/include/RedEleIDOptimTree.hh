#ifndef RedEleIDOptimTree_h
#define RedEleIDOptimTree_h
class TFile;
class TTree;

class G3EventProxy;

class RedEleIDOptimTree {
public:
   RedEleIDOptimTree(const char * filename = "eleID.root");
  ~RedEleIDOptimTree();

  //! event by event final dataset fill
  void fillAll(float fhdeta, float fhdphi, float fhhoe, float fhs9s25, float fhsee, float fheopout, int fhclass, float fldeta, float fldphi, float flhoe, float fls9s25, float flsee,  float fleopout, int flclass);
  //! effectively store the events in the tree
  void store();
  //! save in the ROOT file
  void save();

private:
  float myH_DeltaEta; 
  float myH_DeltaPhi; 
  float myH_Hoe;      
  float myH_S9s25;    
  float myH_See;      
  float myH_EopOut;   
  float myL_DeltaEta; 
  float myL_DeltaPhi; 
  float myL_Hoe;      
  float myL_S9s25;    
  float myL_See;      
  float myL_EopOut;   
  int myH_Class;
  int myL_Class;

  TFile* myFile;
  TTree* myTree;
};

#endif // RedEleIDOptimTree_h
