#!/bin/bash
MERGE="./mergeByFlavour.py"
for J in 0j 1j; do
#    for X in DYCard TopCard WWCard ggWWCard WJet; do
for X in DYCard TopCard WWCard ggWWCard; do
OPTS="--type=gamma"; if [[ "$X" == "WJet" ]]; then OPTS=""; fi;
python $MERGE $OPTS ${X}_ee_$J.txt ${X}_mm_$J.txt > ${X}_sf_$J.txt && echo ${X}_sf_$J.txt
if [[ "$X" != "DYCard" ]]; then
python $MERGE $OPTS  ${X}_em_$J.txt ${X}_me_$J.txt > ${X}_of_$J.txt && echo ${X}_of_$J.txt
fi;
done;
done;
                                                    