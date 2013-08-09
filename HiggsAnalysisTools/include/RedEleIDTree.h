class TFile;
class TTree;

class G3EventProxy;

class RedEleIDTree {
public:
   RedEleIDTree(const char * filename = "eleID.root");
  ~RedEleIDTree();

  void fillAll(int cl, float hoe, float eop, float eopo, float deta, float dphi, float see);
  void store();
  void save();

private:
  int myClass;
  float myHOverE;          
  float myEOverP, myEOverPout;
  float myDeltaEtaAtVtx, myDeltaPhiAtVtx;
  float mySigmaIEtaIEta;

  TFile* myFile;
  TTree* myTree;
 
};

