#/bin/sh

echo "DOING FIRST 3 SETS OF MASSES..."
root -b -l <<EOF
.L HiggsYieldsSteps.cc+
printSuperSummary(35,0)
EOF

echo "DOING SECOND 3 SETS OF MASSES..."
root -b -l <<EOF
.L HiggsYieldsSteps.cc+
printSuperSummary(35,1)
EOF

echo "DOING THIRD 3 SETS OF MASSES..."
root -b -l <<EOF
.L HiggsYieldsSteps.cc+
printSuperSummary(35,2)
EOF

echo "DONE."


