#! /bin/sh


mkdir -p results/merged_skim
lumiEE=$1
lumiMM=$2
lumiEM=$3
echo "Adding weights for ee datasets for " $lumiEE " pb-1..."
root -l -b <<EOF
.L addWeightsToTreeWWres.cc+
addWeights("results/merged/ggH6002L2Nu_ee.root", 7.75308e-08*$lumiEE, 1600 ,1, 1);
addWeights("results/merged/ggH600LNuTauNu_ee.root", 7.75273e-08*$lumiEE, 2600 ,1, 1);
addWeights("results/merged/qqH6002L2Nu_ee.root", 1.39956e-08*$lumiEE, 4600 ,1, 1);
addWeights("results/merged/qqH600LNuTauNu_ee.root", 1.38389e-08*$lumiEE, 5600 ,1, 1);
addWeights("results/merged/Wjets_ee.root", 0.000397171*$lumiEE, 80 ,1, 1);
addWeights("results/merged/Zee_Lo_ee.root", 0.000347794*$lumiEE, 33 ,1, 1);
addWeights("results/merged/Zmm_Lo_ee.root", 0.000338449*$lumiEE, 34 ,1, 1);
addWeights("results/merged/Ztautau_Lo_ee.root", 0.00150895*$lumiEE, 35 ,1, 1);
addWeights("results/merged/Zee_Hi_ee.root", 5.6585e-05*$lumiEE, 30 ,1, 1);
addWeights("results/merged/Zmm_Hi_ee.root", 5.64953e-05*$lumiEE, 31 ,1, 1);
addWeights("results/merged/Ztautau_Hi_ee.root", 8.37186e-05*$lumiEE, 32 ,1, 1);
addWeights("results/merged/SingleT_sChannel_ee.root", 1.22895e-05*$lumiEE, 15 ,1, 1);
addWeights("results/merged/SingleTbar_sChannel_ee.root", 1.04615e-05*$lumiEE, 16 ,1, 1);
addWeights("results/merged/SingleT_tChannel_ee.root", 1.07722e-05*$lumiEE, 13 ,1, 1);
addWeights("results/merged/SingleTbar_tChannel_ee.root", 1.16785e-05*$lumiEE, 14 ,1, 1);
addWeights("results/merged/SingleT_tWChannel_ee.root", 9.68563e-06*$lumiEE, 19 ,1, 1);
addWeights("results/merged/SingleTbar_tWChannel_ee.root", 2.43927e-05*$lumiEE, 20 ,1, 1);
addWeights("results/merged/TTbar_ee.root", 1.66017e-06*$lumiEE, 10 ,1, 1);
addWeights("results/merged/WGToENuG_ee.root", 2.84102e-05*$lumiEE, 82 ,1, 1);
addWeights("results/merged/WGToMuNuG_ee.root", 2.85541e-05*$lumiEE, 83 ,1, 1);
addWeights("results/merged/WGToTauNuG_ee.root", 2.87513e-05*$lumiEE, 84 ,1, 1);
addWeights("results/merged/ZGToEEG_ee.root", 4.30425e-06*$lumiEE, 38 ,1, 1);
addWeights("results/merged/ZGToMuMuG_ee.root", 4.33545e-06*$lumiEE, 39 ,1, 1);
addWeights("results/merged/ZGToTauTauG_ee.root", 4.27188e-06*$lumiEE, 40 ,1, 1);
addWeights("results/merged/ZGToNuNuG_ee.root", 0*$lumiEE, 41 ,1, 1);
addWeights("results/merged/WW_ee.root", 4.01907e-06*$lumiEE, 0 ,1, 1);
addWeights("results/merged/ggWW_ee.root", 1.37201e-06*$lumiEE, 1 ,1, 1);
addWeights("results/merged/WZ_ee.root", 4.89342e-07*$lumiEE, 74 ,1, 1);
addWeights("results/merged/ZZ_ee.root", 1.3604e-06*$lumiEE, 71 ,1, 1);
.q

EOF

echo "Adding weights for mm datasets for " $lumiMM " pb-1..."
root -l -b <<EOF
.L addWeightsToTreeWWres.cc+
addWeights("results/merged/ggH6002L2Nu_mm.root", 7.75308e-08*$lumiMM, 1600 ,0, 1);
addWeights("results/merged/ggH600LNuTauNu_mm.root", 7.75273e-08*$lumiMM, 2600 ,0, 1);
addWeights("results/merged/qqH6002L2Nu_mm.root", 1.39956e-08*$lumiMM, 4600 ,0, 1);
addWeights("results/merged/qqH600LNuTauNu_mm.root", 1.38389e-08*$lumiMM, 5600 ,0, 1);
addWeights("results/merged/Wjets_mm.root", 0.000397171*$lumiMM, 80 ,0, 1);
addWeights("results/merged/Zee_Lo_mm.root", 0.000347794*$lumiMM, 33 ,0, 1);
addWeights("results/merged/Zmm_Lo_mm.root", 0.000338449*$lumiMM, 34 ,0, 1);
addWeights("results/merged/Ztautau_Lo_mm.root", 0.00150895*$lumiMM, 35 ,0, 1);
addWeights("results/merged/Zee_Hi_mm.root", 5.6585e-05*$lumiMM, 30 ,0, 1);
addWeights("results/merged/Zmm_Hi_mm.root", 5.64953e-05*$lumiMM, 31 ,0, 1);
addWeights("results/merged/Ztautau_Hi_mm.root", 8.37186e-05*$lumiMM, 32 ,0, 1);
addWeights("results/merged/SingleT_sChannel_mm.root", 1.22895e-05*$lumiMM, 15 ,0, 1);
addWeights("results/merged/SingleTbar_sChannel_mm.root", 1.04615e-05*$lumiMM, 16 ,0, 1);
addWeights("results/merged/SingleT_tChannel_mm.root", 1.07722e-05*$lumiMM, 13 ,0, 1);
addWeights("results/merged/SingleTbar_tChannel_mm.root", 1.16785e-05*$lumiMM, 14 ,0, 1);
addWeights("results/merged/SingleT_tWChannel_mm.root", 9.68563e-06*$lumiMM, 19 ,0, 1);
addWeights("results/merged/SingleTbar_tWChannel_mm.root", 2.43927e-05*$lumiMM, 20 ,0, 1);
addWeights("results/merged/TTbar_mm.root", 1.66017e-06*$lumiMM, 10 ,0, 1);
addWeights("results/merged/WGToENuG_mm.root", 2.84102e-05*$lumiMM, 82 ,0, 1);
addWeights("results/merged/WGToMuNuG_mm.root", 2.85541e-05*$lumiMM, 83 ,0, 1);
addWeights("results/merged/WGToTauNuG_mm.root", 2.87513e-05*$lumiMM, 84 ,0, 1);
addWeights("results/merged/ZGToEEG_mm.root", 4.30425e-06*$lumiMM, 38 ,0, 1);
addWeights("results/merged/ZGToMuMuG_mm.root", 4.33545e-06*$lumiMM, 39 ,0, 1);
addWeights("results/merged/ZGToTauTauG_mm.root", 4.27188e-06*$lumiMM, 40 ,0, 1);
addWeights("results/merged/ZGToNuNuG_mm.root", 0*$lumiMM, 41 ,0, 1);
addWeights("results/merged/WW_mm.root", 4.01907e-06*$lumiMM, 0 ,0, 1);
addWeights("results/merged/ggWW_mm.root", 1.37201e-06*$lumiMM, 1 ,0, 1);
addWeights("results/merged/WZ_mm.root", 4.89342e-07*$lumiMM, 74 ,0, 1);
addWeights("results/merged/ZZ_mm.root", 1.3604e-06*$lumiMM, 71 ,0, 1);
.q

EOF

echo "Adding weights for em datasets for " $lumiEM " pb-1..."
root -l -b <<EOF
.L addWeightsToTreeWWres.cc+
addWeights("results/merged/ggH6002L2Nu_em.root", 7.75308e-08*$lumiEM, 1600 ,2, 1);
addWeights("results/merged/ggH600LNuTauNu_em.root", 7.75273e-08*$lumiEM, 2600 ,2, 1);
addWeights("results/merged/qqH6002L2Nu_em.root", 1.39956e-08*$lumiEM, 4600 ,2, 1);
addWeights("results/merged/qqH600LNuTauNu_em.root", 1.38389e-08*$lumiEM, 5600 ,2, 1);
addWeights("results/merged/Wjets_em.root", 0.000397171*$lumiEM, 80 ,2, 1);
addWeights("results/merged/Zee_Lo_em.root", 0.000347794*$lumiEM, 33 ,2, 1);
addWeights("results/merged/Zmm_Lo_em.root", 0.000338449*$lumiEM, 34 ,2, 1);
addWeights("results/merged/Ztautau_Lo_em.root", 0.00150895*$lumiEM, 35 ,2, 1);
addWeights("results/merged/Zee_Hi_em.root", 5.6585e-05*$lumiEM, 30 ,2, 1);
addWeights("results/merged/Zmm_Hi_em.root", 5.64953e-05*$lumiEM, 31 ,2, 1);
addWeights("results/merged/Ztautau_Hi_em.root", 8.37186e-05*$lumiEM, 32 ,2, 1);
addWeights("results/merged/SingleT_sChannel_em.root", 1.22895e-05*$lumiEM, 15 ,2, 1);
addWeights("results/merged/SingleTbar_sChannel_em.root", 1.04615e-05*$lumiEM, 16 ,2, 1);
addWeights("results/merged/SingleT_tChannel_em.root", 1.07722e-05*$lumiEM, 13 ,2, 1);
addWeights("results/merged/SingleTbar_tChannel_em.root", 1.16785e-05*$lumiEM, 14 ,2, 1);
addWeights("results/merged/SingleT_tWChannel_em.root", 9.68563e-06*$lumiEM, 19 ,2, 1);
addWeights("results/merged/SingleTbar_tWChannel_em.root", 2.43927e-05*$lumiEM, 20 ,2, 1);
addWeights("results/merged/TTbar_em.root", 1.66017e-06*$lumiEM, 10 ,2, 1);
addWeights("results/merged/WGToENuG_em.root", 2.84102e-05*$lumiEM, 82 ,2, 1);
addWeights("results/merged/WGToMuNuG_em.root", 2.85541e-05*$lumiEM, 83 ,2, 1);
addWeights("results/merged/WGToTauNuG_em.root", 2.87513e-05*$lumiEM, 84 ,2, 1);
addWeights("results/merged/ZGToEEG_em.root", 4.30425e-06*$lumiEM, 38 ,2, 1);
addWeights("results/merged/ZGToMuMuG_em.root", 4.33545e-06*$lumiEM, 39 ,2, 1);
addWeights("results/merged/ZGToTauTauG_em.root", 4.27188e-06*$lumiEM, 40 ,2, 1);
addWeights("results/merged/ZGToNuNuG_em.root", 0*$lumiEM, 41 ,2, 1);
addWeights("results/merged/WW_em.root", 4.01907e-06*$lumiEM, 0 ,2, 1);
addWeights("results/merged/ggWW_em.root", 1.37201e-06*$lumiEM, 1 ,2, 1);
addWeights("results/merged/WZ_em.root", 4.89342e-07*$lumiEM, 74 ,2, 1);
addWeights("results/merged/ZZ_em.root", 1.3604e-06*$lumiEM, 71 ,2, 1);
.q

EOF

echo "Adding weights for me datasets for " $lumiEM " pb-1..."
root -l -b <<EOF
.L addWeightsToTreeWWres.cc+
addWeights("results/merged/ggH6002L2Nu_me.root", 7.75308e-08*$lumiEM, 1600 ,3, 1);
addWeights("results/merged/ggH600LNuTauNu_me.root", 7.75273e-08*$lumiEM, 2600 ,3, 1);
addWeights("results/merged/qqH6002L2Nu_me.root", 1.39956e-08*$lumiEM, 4600 ,3, 1);
addWeights("results/merged/qqH600LNuTauNu_me.root", 1.38389e-08*$lumiEM, 5600 ,3, 1);
addWeights("results/merged/Wjets_me.root", 0.000397171*$lumiEM, 80 ,3, 1);
addWeights("results/merged/Zee_Lo_me.root", 0.000347794*$lumiEM, 33 ,3, 1);
addWeights("results/merged/Zmm_Lo_me.root", 0.000338449*$lumiEM, 34 ,3, 1);
addWeights("results/merged/Ztautau_Lo_me.root", 0.00150895*$lumiEM, 35 ,3, 1);
addWeights("results/merged/Zee_Hi_me.root", 5.6585e-05*$lumiEM, 30 ,3, 1);
addWeights("results/merged/Zmm_Hi_me.root", 5.64953e-05*$lumiEM, 31 ,3, 1);
addWeights("results/merged/Ztautau_Hi_me.root", 8.37186e-05*$lumiEM, 32 ,3, 1);
addWeights("results/merged/SingleT_sChannel_me.root", 1.22895e-05*$lumiEM, 15 ,3, 1);
addWeights("results/merged/SingleTbar_sChannel_me.root", 1.04615e-05*$lumiEM, 16 ,3, 1);
addWeights("results/merged/SingleT_tChannel_me.root", 1.07722e-05*$lumiEM, 13 ,3, 1);
addWeights("results/merged/SingleTbar_tChannel_me.root", 1.16785e-05*$lumiEM, 14 ,3, 1);
addWeights("results/merged/SingleT_tWChannel_me.root", 9.68563e-06*$lumiEM, 19 ,3, 1);
addWeights("results/merged/SingleTbar_tWChannel_me.root", 2.43927e-05*$lumiEM, 20 ,3, 1);
addWeights("results/merged/TTbar_me.root", 1.66017e-06*$lumiEM, 10 ,3, 1);
addWeights("results/merged/WGToENuG_me.root", 2.84102e-05*$lumiEM, 82 ,3, 1);
addWeights("results/merged/WGToMuNuG_me.root", 2.85541e-05*$lumiEM, 83 ,3, 1);
addWeights("results/merged/WGToTauNuG_me.root", 2.87513e-05*$lumiEM, 84 ,3, 1);
addWeights("results/merged/ZGToEEG_me.root", 4.30425e-06*$lumiEM, 38 ,3, 1);
addWeights("results/merged/ZGToMuMuG_me.root", 4.33545e-06*$lumiEM, 39 ,3, 1);
addWeights("results/merged/ZGToTauTauG_me.root", 4.27188e-06*$lumiEM, 40 ,3, 1);
addWeights("results/merged/ZGToNuNuG_me.root", 0*$lumiEM, 41 ,3, 1);
addWeights("results/merged/WW_me.root", 4.01907e-06*$lumiEM, 0 ,3, 1);
addWeights("results/merged/ggWW_me.root", 1.37201e-06*$lumiEM, 1 ,3, 1);
addWeights("results/merged/WZ_me.root", 4.89342e-07*$lumiEM, 74 ,3, 1);
addWeights("results/merged/ZZ_me.root", 1.3604e-06*$lumiEM, 71 ,3, 1);
.q

EOF

echo "done weighting."
