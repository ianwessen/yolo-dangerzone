#! /bin/sh 
# ./mergeTrees.sh expects results of the selection in subdirectories such as results/H120, results/bkg1, results/bkg2...
# it creates a merged root file in the self-created results/merged/
mkdir -p results_data/merged

echo "Now merging EE datasets for mass $Hmass ..."
hadd results_data/merged/Wjets_ee.root results_closure_ee/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root

echo "Now merging ME datasets for mass $Hmass ..."
hadd results_data/merged/Wjets_me.root results_closure_me/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root


