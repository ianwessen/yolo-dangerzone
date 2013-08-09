#! /bin/sh
# ./mergeFinalStates.sh merges the trees for ee/emu/mm final states in one for a combined fit
# the variable "finalstate in the tree will distinguish the three"

hadd -f results_data/datasets_trees/dataset_ll.root results_data/datasets_trees/dataset_ee.root results_data/datasets_trees/dataset_mm.root results_data/datasets_trees/dataset_em.root results_data/datasets_trees/dataset_me.root
