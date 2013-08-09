#ifndef RedEWKTree_h
#define RedEWKTree_h
class TFile;
class TTree;

class G3EventProxy;

class RedEWKTree {
public:
   RedEWKTree(const char * filename = "eleID.root");
  ~RedEWKTree();

  void fillAll(int ne, float iwe, float mt, float mtx, float mty );
  void fillEleId(float is, float tp, float lp, float see, float de, float dp, float eop, float hoe, int qe, float pt, float eta, float phi);

  void store();
  void save();

private:

  // general
  int myNEle;
  float myWeight;       
  float myMet;       
  float myMetx;       
  float myMety;       

  // eleID
  int myNumberOfEle;
  int myCharge[2];
  float myIso[2];
  float myTip[2], myLip[2];
  float mySigmaEE[2]; 
  float myDeltaEtaIn[2], myDeltaPhiIn[2];
  float myEoverPin[2],   myHoverE[2];
  float myPt[2], myEta[2], myPhi[2];  

  TFile* myFile;
  TTree* myTree;
 
};

#endif // RedEWKTree_h
