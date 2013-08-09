#ifndef KFACTOREVALUATOR_H
#define KFACTOREVALUATOR_H

#include <vector>

struct range {
  float min, max;
};

class kFactorEvaluator {
public:
  kFactorEvaluator(int mH);
  virtual ~kFactorEvaluator() {}
  float evaluate(float pT);

private:
  int HiggsMass_;
  std::vector<range> ranges; 
  std::vector<float> kfactors;
};

#endif
