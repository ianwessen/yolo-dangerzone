#! /bin/sh  
mkdir -p results_fake_ee/datasets_trees
mkdir -p results_fake_me/datasets_trees

echo "now merging ee..."
hadd results_fake_ee/datasets_trees/dataset_ee.root results_fake_ee/merged/dataset_DoubleElectron_ee.root results_fake_ee/merged/dataset_DoubleElectron_Run2011B_ee.root results_fake_ee/merged/dataset_SingleElectron_ee.root results_fake_ee/merged/dataset_SingleElectron_Run2011B_ee.root  
echo "done with ee."

echo "now merging me..."
hadd results_fake_me/datasets_trees/dataset_me.root results_fake_me/merged/dataset_MuEG_me.root results_fake_me/merged/dataset_MuEG_Run2011B_me.root results_fake_me/merged/dataset_SingleMu_me.root results_fake_me/merged/dataset_SingleMu_Run2011B_me.root results_fake_me/merged/dataset_SingleElectron_me.root results_fake_me/merged/dataset_SingleElectron_Run2011B_me.root
echo "done with me."

