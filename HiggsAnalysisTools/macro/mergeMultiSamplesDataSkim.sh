#! /bin/sh                                                                                                                                                                                                      
mkdir -p results_data/datasets_trees_skim

echo "now merging ee..."
cp results_data/merged_skim/dataset_DoubleElectron_ee.root results_data/datasets_trees_skim/dataset_ee.root
echo "done with ee."

echo "now merging mm..."
hadd results_data/datasets_trees_skim/dataset_mm.root results_data/merged_skim/dataset_DoubleMu_mm.root results_data/merged_skim/dataset_SingleMu_mm.root
echo "merged_skim mm."

echo "now merging em..."
hadd results_data/datasets_trees_skim/dataset_em.root results_data/merged_skim/dataset_MuEG_em.root results_data/merged_skim/dataset_SingleMu_em.root results_data/merged_skim/dataset_SingleElectron_em.root
echo "merged_skim em."

echo "now merging me..."
hadd results_data/datasets_trees_skim/dataset_me.root results_data/merged_skim/dataset_MuEG_me.root results_data/merged_skim/dataset_SingleMu_me.root results_data/merged_skim/dataset_SingleElectron_me.root
echo "merged_skim me."
