#!/bin/bash
LUMI=$1; if [[ "$LUMI" == "" ]]; then echo "$0 lumi"; exit 1; fi;
for m in $(cat ./masses); do
    test -f dataCard.$m.txt || echo "No 2-jets datacard for m(H) = $m (dataCard.$m.txt)";
    test -f dataCard.$m.txt || break;
    echo "formatting dataCard.$m.txt"
    sed -i 's/ll2j/ll_2j/g' dataCard.$m.txt
    sed -i 's/W-jet/WJet/g' dataCard.$m.txt
    sed -i 's/qqH/vbfH/g' dataCard.$m.txt
    sed -i 's/top/Top/g' dataCard.$m.txt
    sed -i 's/process                        -1           0 /process                        -9          -8/g' dataCard.$m.txt
    mv dataCard.$m.txt hww-$LUMI.mH$m.comb_2j.txt 
done
