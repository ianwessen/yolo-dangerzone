#! /bin/sh

mkdir results_data/merged_skim/

lumiEE=$1
lumiME=$2
echo "Adding weights for ee and me datasets for " $lumiEE " pb-1..."
root -l -b <<EOF
.L addWeightsToTree.cc+
addFRWeights()
addWeights("results_data/merged/Wjets_ee.root", 0.00218538*$lumiEE, 1 ,1, 0);
addWeights("results_data/merged/Wjets_me.root", 0.00218538*$lumiME, 1 ,1, 0);
.q

EOF

echo "done weighting."
