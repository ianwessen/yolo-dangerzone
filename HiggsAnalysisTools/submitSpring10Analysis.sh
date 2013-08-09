#!/bin/sh

prefix=$1

echo "submitting signals..."
python cmst3_submit_manyfilesperjob.py HiggsWW H120_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H130_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H140_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H150_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H155_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H160_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H165_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H170_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H175_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H180_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py HiggsWW H190_2W_2lnu_gluonfusion_7TeV 5 HiggsApp 8nh $prefix
echo "done with signals."

echo "submitting top..."
python cmst3_submit_manyfilesperjob.py SingleTop SingleTop_sChannel-madgraph 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py SingleTop SingleTop_tChannel-madgraph 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py SingleTop SingleTop_tWChannel-madgraph 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py TTbar TTbarJets-madgraph 5 HiggsApp 8nh $prefix
echo "done with top."

echo "submitting V+jets..."
python cmst3_submit_manyfilesperjob.py WJetsMADGRAPH WJets-madgraph 10 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py ZJetsMADGRAPH ZJets-madgraph 5 HiggsApp 8nh $prefix
echo "done with V+jets."

echo "submitting W+1jet alpgen..."
python cmst3_submit_manyfilesperjob.py WJetsALPGEN W1Jets_Pt0to100-alpgen 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py WJetsALPGEN W1Jets_Pt100to300-alpgen 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py WJetsALPGEN W1Jets_Pt300to800-alpgen 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py WJetsALPGEN W1Jets_Pt800to1600-alpgen 5 HiggsApp 8nh $prefix
echo "done with W+1jet alpgen"

echo "submitting dibosons..."
python cmst3_submit_manyfilesperjob.py DiBosons WW_2l_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py DiBosons WZ_3l_7TeV 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py DiBosons Wgamma 5 HiggsApp 8nh $prefix
python cmst3_submit_manyfilesperjob.py DiBosons ZZ_2l2nu 5 HiggsApp 8nh $prefix
echo "done with dibosons."
echo "VERY DONE."

