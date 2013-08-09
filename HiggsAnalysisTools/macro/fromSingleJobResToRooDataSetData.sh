#! /bin/sh 
# ./fromSingleJobResToRooDataSetData.sh:

echo "===> STARTING... <==="

echo "MERGING TREES STEP..."
./mergeTreesData.sh 2012
echo "MERGING TREES STEP DONE."

echo "WEIGHTING TREES STEP..."
./weightTreesData.sh
echo "MERGING TREES STEP DONE."

echo "MERGING MULTIPLE DATASETS IN THE SAME FINAL STATE..."
./mergeMultiSamplesData.sh
echo "MERGING MULTIPLE DATASETS IN THE SAME FINAL STATE DONE."

echo "MERGING FINAL STATES..."
./mergeFinalStatesData.sh
echo "MERGING FINAL STATES DONE."

echo "===> VERY DONE. <==="
