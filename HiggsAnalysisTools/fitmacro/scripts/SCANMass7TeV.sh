#!/bin/bash 
cd /afs/cern.ch/work/e/emanuele/hww2l2nu/CMSSW_6_1_1/src/
eval `scram ru -sh`
cd -
combine -M MultiDimFit /afs/cern.ch/work/e/emanuele/hww2l2nu/HiggsAnalysisTools/fitmacro/FloatMass_comb_7TeV_hww.root  -m 125.8 -P MH --floatOtherPOI=0 --algo=grid --points=200 -n SCAN2D_7TeV_FAST
mv higgsCombine*root /afs/cern.ch/work/e/emanuele/hww2l2nu/HiggsAnalysisTools/fitmacro/