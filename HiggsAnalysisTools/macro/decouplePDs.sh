#! /bin/sh 

root -l -b <<EOF

.L decouplePDs.cc+
decouple("results_data/merged/dataset_mu_em.root","results_data/merged/dataset_eg_em.root","results_data/merged/dataset_muAndEg_em.root")
.q

EOF
