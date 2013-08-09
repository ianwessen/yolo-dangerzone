#! /bin/sh 
# ./fromSingleJobResToRooDataSet.sh:
# it runs several scripts that bring from the results of the selection stored in the directory "results/"
# to the merged RooDataSets of the species used for the fit

echo "COUNTING EVENTS PROCESSED IN MC..."
root -b -l <<EOF
.L HiggsEventCounter8TeV.cc+
countEvents();
.q
EOF
echo "COUNTING EVENTS PROCESSED IN MC DONE."

echo "MERGING TREES STEP..."
./mergeTrees.sh 2012
echo "MERGING TREES STEP DONE."

echo "WEIGHTING TREES STEP..."
./weightTrees.sh 1000 1000 1000
echo "MERGING TREES STEP DONE."

echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION..."
./mergeMultiSamples.sh 2012
echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION DONE."

echo "MERGING FINAL STATES..."
./mergeFinalStates.sh 2012
echo "MERGING FINAL STATES DONE."

