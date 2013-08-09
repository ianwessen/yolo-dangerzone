#! /bin/sh 
# ./fromSingleJobResToRooDataSetWWres.sh:
# it runs several scripts that bring from the results of the selection stored in the directory "results/"
# to the merged RooDataSets of the species used for the fit

lumi=$1

echo "COUNTING EVENTS PROCESSED IN MC..."
root -b -l <<EOF
.L wwResEventCounter.cc+
countEvents();
.q
EOF
echo "COUNTING EVENTS PROCESSED IN MC DONE."

echo "MERGING TREES STEP..."
./wwResMergeTrees.sh
echo "MERGING TREES STEP DONE."

echo "WEIGHTING TREES STEP..."
chmod a+x wwResWeightTrees.sh
./wwResWeightTrees.sh $lumi $lumi $lumi
echo "MERGING TREES STEP DONE."

echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION..."
./wwResMergeMultiSamples.sh
./wwResMergeMultiSamplesSkim.sh
echo "MERGING WEIGHTED TREES ACCORDING FIT SPECIES DEFINITION DONE."

echo "MERGING FINAL STATES..."
./wwResMergeFinalStates.sh
./wwResMergeFinalStatesSkim.sh
echo "MERGING FINAL STATES DONE."

#echo "CREATING FIT ROODATASETS STEP..."
#chmod u+x ./createFitDatasets.sh
#./createFitDatasets.sh $Hmass
#echo "CREATING FIT ROODATASETS DONE."
