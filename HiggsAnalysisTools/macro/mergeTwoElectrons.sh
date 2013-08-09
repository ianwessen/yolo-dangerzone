#! /bin/sh 
# this script replaces every merged tree with the same tree with one more branch, containing the event weight for that sample
# the event weight is evaluated with the total number of generated events, cross section and eventual prescale and the wanted luminosity
# the values can be evaluated with the program weights.cc

# usage: ./mergeTwoElectrons.sh

root -l -b <<EOF

.L mergeTwoElectrons.cc+

merge("results/datasets_trees/WjetsMadgraph_ee");

merge("results/datasets_trees/WW_ee");

merge("results/datasets_trees/WjetsAlpgen_ee");

merge("results/datasets_trees/H130_ee");

merge("results/datasets_trees/H160_ee");

merge("results/datasets_trees/H190_ee");


.q

EOF

