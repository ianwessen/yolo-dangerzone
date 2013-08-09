#ifndef VertexTree_h
#define VertexTree_h
class TFile;
class TTree;

class G3EventProxy;

class VertexTree {
public:
   VertexTree(const char * filename = "vertexing.root");
  ~VertexTree();

  //! event by event final dataset fill
  void fillAll(float dz, float dxy, float dxyz);
  //! effectively store the events in the tree
  void store();
  //! save in the ROOT file
  void save();

private:
  float myDeltaZ; 
  float myDeltaXY; 
  float myDeltaXYZ; 

  TFile* myFile;
  TTree* myTree;
};

#endif // VertexTree_h
