#!/bin/bash
# combcards.sh 8 (7 for 7TeV)

source "$CMSSW_BASE/src/HWWAnalysis/ShapeAnalysis/test/env.sh"

tailsf="_shape"
tailof="_shape"
cwd=$PWD
masses="115 120 125 130 135 140 150 160 170 180"


function usage() {
	echo "$( basename $0) -p <prefix> -t (to use 2D OF) -s (7 or 8 for 7TeV or 8TeV)"
}

sqrts=
prefix=
suffix2D=
twod=
while getopts "hp:ts:" OPTION
do
    case $OPTION in
        h)
            usage
            exit 1
            ;;
        p)
            prefix=$OPTARG
            ;;
        t)
            twod="_2D"
            ;;
        s) 
            sqrts=$OPTARG
            ;;
    esac
done

lumi=
if [[ $sqrts == 7 ]]
    then
    lumi=4.94
    else 
    lumi=19.47
fi

if [[ $sqrts == 78 ]]
    then
    lumi=24.41
fi

tailof="_shape"$twod"_"$sqrts"TeV.txt"
tailsf="_shape_"$sqrts"TeV.txt"

echo `shape-config.py`
eval `shape-config.py $lumi`

if [[ $prefix ]]; then
    echo "Running in $prefix"
    cd $prefix
fi

echo "Lumi $lumi"
head="hww-${lumi}fb.mH"
head7TeV="hww-4.94fb.mH"
head8TeV="hww-19.47fb.mH"
head2j="hww-${lumi}fb.mH"
hline=$(printf '%.0s-' {1..80})
combCmd='combineCards.py -S'
echo $PWD
for mass in $masses
  do
  
  if [[ $sqrts -ne 78 ]]
      then
      
      echo "combining datacards for sqrt s = $sqrts"

      for jet in 0j 1j
	do
        echo $hline
        echo " Combining cards $mass $jet"
        echo $hline
        if [ -e $head$mass".of_"$jet$tailof ] && [ -e $head$mass".sf_"$jet$tailsf ] 
            then
            echo " ${mass}: of_${jet} sf_${jet} => comb_${jet}"
            $combCmd "of_"$jet"="$head$mass".of_"$jet$tailof  "sf_"$jet"="$head$mass".sf_"$jet$tailsf > $head$mass".comb_"$jet$tailof 
        fi
      done
      
      for fl in of sf
	do
        tailfl=
        if [[ $fl == "of" ]]; then
            tailfl=$tailof
        else
            tailfl=$tailsf
        fi
        file0="${head}${mass}.${fl}_0j${tailfl}"
        file1="${head}${mass}.${fl}_1j${tailfl}"
        cmb="${head}${mass}.comb_${fl}${tailfl}"
        
        echo $hline
        echo " Combining cards $mass $fl ( ${file0} + ${file1} )"
        echo $hline
        if [ -e "${file0}" ] && [ -e "${file1}" ] 
            then
            echo " ${mass}: ${fl}_0j ${fl}_1j => comb_${fl}"
            $combCmd "${fl}_0j=${file0}"  "${fl}_1j=${file1}"  > "${cmb}"
        fi
      done
      
      if [ -e $head$mass".of_0j"$tailof ] && [ -e $head$mass".sf_0j"$tailsf ] && [ -e $head$mass".of_1j"$tailof ] && [ -e $head$mass".sf_1j"$tailsf ]
          then
          echo " $mass: sf_0j of_0j of_1j sf_1j => comb_0j1j "
          $combCmd "of_0j="$head$mass".of_0j"$tailof  "sf_0j="$head$mass".sf_0j"$tailsf "of_1j="$head$mass".of_1j"$tailof  "sf_1j="$head$mass".sf_1j"$tailsf > $head$mass".comb_0j1j"$tailof
      fi
      
      if [ -e $head$mass".comb_0j"$tailof ] && [ -e $head$mass".comb_1j"$tailof ] && [ -e $head$mass".of_2j"$tailsf ] && [ -e $head$mass".sf_2j"$tailsf ]
          then
          echo " $mass: comb_0j comb_1j of_2j sf_2j => comb_0j1j2j "
          $combCmd HWW_0j=$head$mass".comb_0j"$tailof HWW_1j=$head$mass".comb_1j"$tailof HWW_2j_of=$head$mass".of_2j"$tailsf HWW_2j_sf=$head$mass".sf_2j"$tailsf > $head$mass.comb_0j1j2j$tailof
      fi
  else

      echo "combining datacards for sqrt s = 7 TeV with sqrt s = 8 TeV"

      if [ -e $head7TeV$mass".of_0j_shape"$twod"_7TeV.txt" ] && [ -e $head7TeV$mass".sf_0j_shape_7TeV.txt" ] && [ -e $head7TeV$mass".of_1j_shape"$twod"_7TeV.txt" ] && [ -e $head7TeV$mass".sf_1j_shape_7TeV.txt" ] && \
          [ -e $head8TeV$mass".of_0j_shape"$twod"_8TeV.txt" ] && [ -e $head8TeV$mass".sf_0j_shape_8TeV.txt" ] && [ -e $head8TeV$mass".of_1j_shape"$twod"_8TeV.txt" ] && [ -e $head8TeV$mass".sf_1j_shape_8TeV.txt" ]
          then
          echo " $mass: comb_0j1j_7TeV comb_0j1j_8TeV => comb_0j1j_78TeV "
          $combCmd "of_0j_7TeV="$head7TeV$mass".of_0j_shape"$twod"_7TeV.txt" "of_1j_7TeV="$head7TeV$mass".of_1j_shape"$twod"_7TeV.txt" "sf_0j_7TeV="$head7TeV$mass".sf_0j_shape_7TeV.txt" "sf_1j_7TeV="$head7TeV$mass".sf_1j_shape_7TeV.txt" \
"of_0j_8TeV="$head8TeV$mass".of_0j_shape"$twod"_8TeV.txt" "of_1j_8TeV="$head8TeV$mass".of_1j_shape"$twod"_8TeV.txt" "sf_0j_8TeV="$head8TeV$mass".sf_0j_shape_8TeV.txt" "sf_1j_8TeV="$head8TeV$mass".sf_1j_shape_8TeV.txt" \
              > $head$mass".comb_0j1j_shape"$twod"_78TeV.txt"
      fi
  fi
done
echo "...Done"


