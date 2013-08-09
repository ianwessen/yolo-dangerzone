#! /bin/bash

cat DYCard_ll_0j.txt | awk '{print $1 "\t" $2 "\t" 0.5*$3 "\t" 0.5*$4}' > DYCard_ee_0j.txt
cat DYCard_ll_0j.txt | awk '{print $1 "\t" $2 "\t" 0.5*$3 "\t" 0.5*$4}' > DYCard_mm_0j.txt
cat DYCard_ll_1j.txt | awk '{print $1 "\t" $2 "\t" 0.5*$3 "\t" 0.5*$4}' > DYCard_ee_1j.txt
cat DYCard_ll_1j.txt | awk '{print $1 "\t" $2 "\t" 0.5*$3 "\t" 0.5*$4}' > DYCard_mm_1j.txt

echo "DONE SPLITTING ee + mm."

