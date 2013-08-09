#!/bin/bash
LUMI=$1; if [[ "$LUMI" == "" ]]; then echo "$0 lumi"; exit 1; fi;
for m in $(cat ./masses); do 
    for J in 0 1; do
        test -f hww-$LUMI.mH$m.mm_${J}j.txt || echo "No $J-jets datacards for m(H) = $m, lumi $LUMI (hww-$LUMI.mH$m.mm_${J}j.txt)";
        test -f hww-$LUMI.mH$m.mm_${J}j.txt || break;
        echo " make hww-$LUMI.mH$m.sf_${J}j.txt"
        combineCards.py mm_${J}j=hww-$LUMI.mH$m.mm_${J}j.txt \
                        ee_${J}j=hww-$LUMI.mH$m.ee_${J}j.txt \
                > hww-$LUMI.mH$m.sf_${J}j.txt; 
        echo " make hww-$LUMI.mH$m.of_${J}j.txt"
        combineCards.py me_${J}j=hww-$LUMI.mH$m.me_${J}j.txt \
                        em_${J}j=hww-$LUMI.mH$m.em_${J}j.txt \
                > hww-$LUMI.mH$m.of_${J}j.txt; 
        echo " make hww-$LUMI.mH$m.comb_${J}j.txt"
        combineCards.py mm_${J}j=hww-$LUMI.mH$m.mm_${J}j.txt \
                        me_${J}j=hww-$LUMI.mH$m.me_${J}j.txt \
                        em_${J}j=hww-$LUMI.mH$m.em_${J}j.txt \
                        ee_${J}j=hww-$LUMI.mH$m.ee_${J}j.txt \
                > hww-$LUMI.mH$m.comb_${J}j.txt; 
    done
    test -f hww-$LUMI.mH$m.comb_0j.txt || continue;
#    test -f hww-$LUMI.mH$m.comb_2j.txt || echo "No 2-jets datacards for m(H) = $m, lumi $LUMI (hww-$LUMI.mH$m.comb_2j.txt)";
#    test -f hww-$LUMI.mH$m.comb_2j.txt || break;
    echo " make hww-$LUMI.mH$m.comb.txt"
#    combineCards.py HWW_0j=hww-$LUMI.mH$m.comb_0j.txt HWW_1j=hww-$LUMI.mH$m.comb_1j.txt HWW_2j=hww-$LUMI.mH$m.comb_2j.txt > hww-$LUMI.mH$m.comb.txt
    combineCards.py HWW_0j=hww-$LUMI.mH$m.comb_0j.txt HWW_1j=hww-$LUMI.mH$m.comb_1j.txt > hww-$LUMI.mH$m.comb.txt
done
