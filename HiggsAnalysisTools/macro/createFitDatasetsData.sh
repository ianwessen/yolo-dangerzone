#! /bin/sh
# ./createFitDatasets.sh assumes that merged trees (one per species as defined in createFitDatasets.cc) exist in results/datasets_trees/
# creates the fit RooDataSets in directory results/datasets

mkdir -p results_data/datasets

root -l -b <<EOF

.include /afs/cern.ch/cms/sw/slc5_ia32_gcc434/lcg/roofit/5.26.00-cms8/include/
.L createFitDatasets.cc+
createAllData();

.q

EOF
