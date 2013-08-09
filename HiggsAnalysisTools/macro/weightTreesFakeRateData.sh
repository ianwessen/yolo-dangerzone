#! /bin/sh 
# this script replaces every merged tree with the same tree with one more branch, containing the event weight for that sample
# the event weight is evaluated with the total number of generated events, cross section and eventual prescale and the wanted luminosity
# the values can be evaluated with the program weights.cc

# usage: ./weightTreesData.sh

mkdir results_fake_ee/merged_skim
mkdir results_fake_me/merged_skim

echo "Adding weights..."
root -l -b <<EOF

.L addWeightsToTree.cc+

addFRWeights();

addWeights("results_fake_ee/merged/dataset_DoubleElectron_ee.root",          1.0, 102, 1, 1); 
addWeights("results_fake_ee/merged/dataset_DoubleElectron_Run2011B_ee.root", 1.0, 102, 1, 1);            
                                                                 
addWeights("results_fake_ee/merged/dataset_SingleElectron_ee.root",          1.0, 100, 1, 1);          
addWeights("results_fake_ee/merged/dataset_SingleElectron_Run2011B_ee.root", 1.0, 100, 1, 1);          

addWeights("results_fake_me/merged/dataset_MuEG_me.root",          1.0, 104, 3, 1); 
addWeights("results_fake_me/merged/dataset_MuEG_Run2011B_me.root", 1.0, 104, 3, 1); 

addWeights("results_fake_me/merged/dataset_SingleMu_me.root",           1.0, 101, 3, 1); 
addWeights("results_fake_me/merged/dataset_SingleMu_Run2011B_me.root",  1.0, 101, 3, 1); 

addWeights("results_fake_me/merged/dataset_SingleElectron_me.root", 1.0, 100, 3, 1);  
addWeights("results_fake_me/merged/dataset_SingleElectron_me.root", 1.0, 100, 3, 1);  

.q

EOF

echo "done weighting."
