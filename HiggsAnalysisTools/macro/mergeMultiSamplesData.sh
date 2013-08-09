#! /bin/sh                                                                                                                                                                                                      
mkdir -p results_data/datasets_trees

echo "now merging ee..."
hadd results_data/datasets_trees/dataset_ee.root results_data/merged/dataset_DoubleElectron_ee.root results_data/merged/dataset_SingleElectron_ee.root 
echo "done with ee."

echo "now merging mm..."
hadd results_data/datasets_trees/dataset_mm.root results_data/merged/dataset_DoubleMu_mm.root results_data/merged/dataset_SingleMu_mm.root
echo "merged mm."

echo "now merging em..."
hadd results_data/datasets_trees/dataset_em.root results_data/merged/dataset_MuEG_em.root results_data/merged/dataset_SingleMu_em.root results_data/merged/dataset_SingleElectron_em.root
echo "merged em."

echo "now merging me..."
hadd results_data/datasets_trees/dataset_me.root results_data/merged/dataset_MuEG_me.root results_data/merged/dataset_SingleMu_me.root results_data/merged/dataset_SingleElectron_me.root
echo "merged me."
