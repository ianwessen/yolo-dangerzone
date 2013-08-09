#! /bin/sh 
# ./mergeTrees.sh expects results of single sample files in results/merged_skim/
# it creates a merged_skim root file for each single specie of the fit in results/datasets

mkdir -p results/datasets_trees_skim

echo "Now merging species for ee..."
# signal is always a species per se
for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
do
  hadd results/datasets_trees_skim/H$i\_ee.root results/merged_skim/ggH$i\2L2Nu_ee.root results/merged_skim/ggH$i\LNuTauNu_ee.root results/merged_skim/qqH$i\2L2Nu_ee.root results/merged_skim/qqH$i\LNuTauNu_ee.root
done
for i in 110 115 118 122 124 126 128 135
do
  hadd results/datasets_trees_skim/H$i\_ee.root results/merged_skim/ggH$i\2LAndTau2Nu_ee.root results/merged_skim/qqH$i\2LAndTau2Nu_ee.root
done

# WW is a species per se
hadd results/datasets_trees_skim/WW_ee.root results/merged_skim/WW_ee.root results/merged_skim/ggWW_ee.root

# Wjets is a species per se
cp results/merged_skim/Wjets_ee.root results/datasets_trees_skim/Wjets_ee.root

# Zjets is a species per se
hadd results/datasets_trees_skim/Zjets_ee.root results/merged_skim/Zee_*_ee.root results/merged_skim/Zmm_*_ee.root results/merged_skim/Ztautau_*_ee.root

# merging ttbar and single t in a species
hadd results/datasets_trees_skim/top_ee.root results/merged_skim/TTbar_ee.root results/merged_skim/SingleT_tChannel_ee.root   results/merged_skim/SingleTbar_sChannel_ee.root \
    results/merged_skim/SingleTbar_tWChannel_ee.root results/merged_skim/SingleT_tWChannel_ee.root  results/merged_skim/SingleTbar_tChannel_ee.root

# merging all other backgrounds
hadd results/datasets_trees_skim/others_ee.root results/merged_skim/WZ_ee.root results/merged_skim/ZZ_ee.root


echo "Now merging species for mm..."
# signal is always a species per se
for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
do
  hadd results/datasets_trees_skim/H$i\_mm.root results/merged_skim/ggH$i\2L2Nu_mm.root results/merged_skim/ggH$i\LNuTauNu_mm.root results/merged_skim/qqH$i\2L2Nu_mm.root results/merged_skim/qqH$i\LNuTauNu_mm.root
done
for i in 110 115 118 122 124 126 128 135
do
  hadd results/datasets_trees_skim/H$i\_mm.root results/merged_skim/ggH$i\2LAndTau2Nu_mm.root results/merged_skim/qqH$i\2LAndTau2Nu_mm.root
done

# WW is a species per se
hadd results/datasets_trees_skim/WW_mm.root results/merged_skim/WW_mm.root results/merged_skim/ggWW_mm.root

# Wjets is a species per se
cp results/merged_skim/Wjets_mm.root results/datasets_trees_skim/Wjets_mm.root

# Zjets is a species per se
hadd results/datasets_trees_skim/Zjets_mm.root results/merged_skim/Zee_*_mm.root results/merged_skim/Zmm_*_mm.root results/merged_skim/Ztautau_*_mm.root

# merging ttbar and single t in a species
hadd results/datasets_trees_skim/top_mm.root results/merged_skim/TTbar_mm.root results/merged_skim/SingleT_tChannel_mm.root   results/merged_skim/SingleTbar_sChannel_mm.root \
    results/merged_skim/SingleTbar_tWChannel_mm.root results/merged_skim/SingleT_tWChannel_mm.root  results/merged_skim/SingleTbar_tChannel_mm.root

# merging all other backgrounds
hadd results/datasets_trees_skim/others_mm.root results/merged_skim/WZ_mm.root results/merged_skim/ZZ_mm.root


echo "Now merging species for em..."
# signal is always a species per se
for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
do
  hadd results/datasets_trees_skim/H$i\_em.root results/merged_skim/ggH$i\2L2Nu_em.root results/merged_skim/ggH$i\LNuTauNu_em.root results/merged_skim/qqH$i\2L2Nu_em.root results/merged_skim/qqH$i\LNuTauNu_em.root
done
for i in 110 115 118 122 124 126 128 135
do
  hadd results/datasets_trees_skim/H$i\_em.root results/merged_skim/ggH$i\2LAndTau2Nu_em.root results/merged_skim/qqH$i\2LAndTau2Nu_em.root
done

# WW is a species per se
hadd results/datasets_trees_skim/WW_em.root results/merged_skim/WW_em.root results/merged_skim/ggWW_em.root

# Wjets is a species per se
cp results/merged_skim/Wjets_em.root results/datasets_trees_skim/Wjets_em.root

# Zjets is a species per se
hadd results/datasets_trees_skim/Zjets_em.root results/merged_skim/Zee_*_em.root results/merged_skim/Zmm_*_em.root results/merged_skim/Ztautau_*_em.root

# merging ttbar and single t in a species
hadd results/datasets_trees_skim/top_em.root results/merged_skim/TTbar_em.root results/merged_skim/SingleT_tChannel_em.root   results/merged_skim/SingleTbar_sChannel_em.root \
    results/merged_skim/SingleTbar_tWChannel_em.root results/merged_skim/SingleT_tWChannel_em.root  results/merged_skim/SingleTbar_tChannel_em.root

# merging all other backgrounds
hadd results/datasets_trees_skim/others_em.root results/merged_skim/WZ_em.root results/merged_skim/ZZ_em.root



echo "Now merging species for me..."
# signal is always a species per se
for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
do
  hadd results/datasets_trees_skim/H$i\_me.root results/merged_skim/ggH$i\2L2Nu_me.root results/merged_skim/ggH$i\LNuTauNu_me.root results/merged_skim/qqH$i\2L2Nu_me.root results/merged_skim/qqH$i\LNuTauNu_me.root
done
for i in 110 115 118 122 124 126 128 135
do
  hadd results/datasets_trees_skim/H$i\_me.root results/merged_skim/ggH$i\2LAndTau2Nu_me.root results/merged_skim/qqH$i\2LAndTau2Nu_me.root
done

# WW is a species per se
hadd results/datasets_trees_skim/WW_me.root results/merged_skim/WW_me.root results/merged_skim/ggWW_me.root

# Wjets is a species per se
cp results/merged_skim/Wjets_me.root results/datasets_trees_skim/Wjets_me.root

# Zjets is a species per se
hadd results/datasets_trees_skim/Zjets_me.root results/merged_skim/Zee_*_me.root results/merged_skim/Zmm_*_me.root results/merged_skim/Ztautau_*_me.root

# merging ttbar and single t in a species
hadd results/datasets_trees_skim/top_me.root results/merged_skim/TTbar_me.root results/merged_skim/SingleT_tChannel_me.root   results/merged_skim/SingleTbar_sChannel_me.root \
    results/merged_skim/SingleTbar_tWChannel_me.root results/merged_skim/SingleT_tWChannel_me.root  results/merged_skim/SingleTbar_tChannel_me.root

# merging all other backgrounds
hadd results/datasets_trees_skim/others_me.root results/merged_skim/WZ_me.root results/merged_skim/ZZ_me.root


