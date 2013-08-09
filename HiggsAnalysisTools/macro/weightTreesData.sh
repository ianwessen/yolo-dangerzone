#! /bin/sh 
# this script replaces every merged tree with the same tree with one more branch, containing the event weight for that sample
# the event weight is evaluated with the total number of generated events, cross section and eventual prescale and the wanted luminosity
# the values can be evaluated with the program weights.cc

# usage: ./weightTreesData.sh

mkdir -p results_data/merged_skim

echo "Adding weights to tight x tight..."
./LatinosAnalyzer results_data/merged/dataset_DoubleElectron_ee.root 1.0 102 1 0
./LatinosAnalyzer results_data/merged/dataset_SingleElectron_ee.root 1.0 100 1 0

./LatinosAnalyzer results_data/merged/dataset_DoubleMu_mm.root 1.0 103 0 0
./LatinosAnalyzer results_data/merged/dataset_SingleMu_mm.root 1.0 101 0 0

./LatinosAnalyzer results_data/merged/dataset_MuEG_em.root 1.0 104 2 0
./LatinosAnalyzer results_data/merged/dataset_SingleMu_em.root 1.0 101 2 0
./LatinosAnalyzer results_data/merged/dataset_SingleElectron_em.root 1.0 100 2 0

./LatinosAnalyzer results_data/merged/dataset_MuEG_me.root 1.0 104 3 0
./LatinosAnalyzer results_data/merged/dataset_SingleMu_me.root 1.0 101 3 0
./LatinosAnalyzer results_data/merged/dataset_SingleElectron_me.root 1.0 100 3 0

make LatinosLooseLooseAnalyzer
./LatinosLooseLooseAnalyzer results_data/datasets_trees/dataset_looseloose.root

make LatinosEmbeddedTauAnalyzer
./LatinosEmbeddedTauAnalyzer results_data/datasets_trees/dataset_embeddedtt_ll.root

echo "done weighting."
