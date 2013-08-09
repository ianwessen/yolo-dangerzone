#!/bin/sh

prefix=$1

echo "submitting EG..."
python cmst3_submit_manyfilesperjob_data.py Data7TeV dataset_eg 8 HiggsApp 8nh $prefix
echo "done with EG."

echo "submitting MU..."
python cmst3_submit_manyfilesperjob_data.py Data7TeV dataset_mu 8 HiggsApp 8nh $prefix
echo "done with MU."


