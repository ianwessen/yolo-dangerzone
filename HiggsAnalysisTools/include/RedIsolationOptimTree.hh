#ifndef RedIsolationOptimTree_h
#define RedIsolationOptimTree_h
class TFile;
class TTree;

class G3EventProxy;

class RedIsolationOptimTree {
public:
   RedIsolationOptimTree(const char * filename = "eleID.root");
  ~RedIsolationOptimTree();

  //! event by event final dataset fill
  void fillAll(float fhtracker, float fhhcal, float fhecal, float fltracker, float flhcal, float flecal);
  //! effectively store the events in the tree
  void store();
  //! save in the ROOT file
  void save();

private:
  float myH_trackerIsol; 
  float myH_hcalIsol;
  float myH_ecalIsol;
  float myL_trackerIsol; 
  float myL_hcalIsol;
  float myL_ecalIsol;

  TFile* myFile;
  TTree* myTree;
};

#endif // RedIsolationOptimTree_h
