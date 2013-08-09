#! /bin/sh
# ./mergeFinalStates.sh merges the trees for ee/emu/mm final states in one for a combined fit
# the variable "finalstate in the tree will distinguish the three"



# usage: ./mergeFinalStates.sh Hmass
for i in 110 115 118 120 122 124 126 128 130 135 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
do
  hadd -f results/datasets_trees_skim/H$i\_ll.root results/datasets_trees_skim/H$i\_ee.root results/datasets_trees_skim/H$i\_mm.root results/datasets_trees_skim/H$i\_em.root results/datasets_trees_skim/H$i\_me.root
done
hadd -f results/datasets_trees_skim/WW_ll.root results/datasets_trees_skim/WW_ee.root results/datasets_trees_skim/WW_mm.root results/datasets_trees_skim/WW_em.root results/datasets_trees_skim/WW_me.root
hadd -f results/datasets_trees_skim/top_ll.root results/datasets_trees_skim/top_ee.root results/datasets_trees_skim/top_mm.root results/datasets_trees_skim/top_em.root results/datasets_trees_skim/top_me.root
hadd -f results/datasets_trees_skim/Wjets_ll.root results/datasets_trees_skim/Wjets_ee.root results/datasets_trees_skim/Wjets_mm.root results/datasets_trees_skim/Wjets_em.root results/datasets_trees_skim/Wjets_me.root
hadd -f results/datasets_trees_skim/Zjets_ll.root results/datasets_trees_skim/Zjets_ee.root results/datasets_trees_skim/Zjets_mm.root results/datasets_trees_skim/Zjets_em.root results/datasets_trees_skim/Zjets_me.root
hadd -f results/datasets_trees_skim/others_ll.root results/datasets_trees_skim/others_ee.root results/datasets_trees_skim/others_mm.root results/datasets_trees_skim/others_em.root results/datasets_trees_skim/others_me.root
