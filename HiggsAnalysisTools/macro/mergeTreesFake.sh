#! /bin/sh 
# ./mergeTrees.sh expects results of the selection in subdirectories such as results/H120, results/bkg1, results/bkg2...
# it creates a merged root file in the self-created results/merged/

mkdir -p results_fakemc_ee/merged
mkdir -p results_fakemc_me/merged

hadd results_fakemc_ee/merged/Wjets_ee.root results_fakemc_ee/Summer11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
hadd results_fakemc_me/merged/Wjets_me.root results_fakemc_me/Summer11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
