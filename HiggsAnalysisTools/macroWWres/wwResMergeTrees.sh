#! /bin/sh 
# ./mergeTrees.sh expects results of the selection in subdirectories such as results/H120, results/bkg1, results/bkg2...
# it creates a merged root file in the self-created results/merged/

mkdir -p results/merged

echo "Now merging EE datasets for mass $Hmass ..."
for i in 600
do
  hadd results/merged/ggH$i\2L2Nu_ee.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
  hadd results/merged/ggH$i\LNuTauNu_ee.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
  hadd results/merged/qqH$i\2L2Nu_ee.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
  hadd results/merged/qqH$i\LNuTauNu_ee.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
done

hadd results/merged/TTbar_ee.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetEE.root 
hadd results/merged/SingleT_sChannel_ee.root results/Fall11_V1/T_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/SingleTbar_sChannel_ee.root results/Fall11_V1/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/SingleT_tChannel_ee.root results/Fall11_V1/T_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/SingleTbar_tChannel_ee.root results/Fall11_V1/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/SingleT_tWChannel_ee.root results/Fall11_V1/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/SingleTbar_tWChannel_ee.root results/Fall11_V1/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetEE.root
hadd results/merged/Wjets_ee.root results/Fall11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
hadd results/merged/Zee_Hi_ee.root results/Fall11_V1/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEE.root
hadd results/merged/Zmm_Hi_ee.root results/Fall11_V1/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEE.root
hadd results/merged/Ztautau_Hi_ee.root results/Fall11_V1/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*datasetEE.root
hadd results/merged/Zee_Lo_ee.root results/Fall11_V1/DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEE.root
hadd results/merged/Zmm_Lo_ee.root results/Fall11_V1/DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEE.root
hadd results/merged/Ztautau_Lo_ee.root results/Fall11_V1/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*datasetEE.root
hadd results/merged/WW_ee.root results/Fall11_V1/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root 
hadd results/merged/ggWW_ee.root results/Fall11_V1/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*datasetEE.root
hadd results/merged/WZ_ee.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
hadd results/merged/ZZ_ee.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root
hadd results/merged/WGToENuG_ee.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetEE.root
hadd results/merged/WGToMuNuG_ee.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetEE.root
hadd results/merged/WGToTauNuG_ee.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
hadd results/merged/ZGToEEG_ee.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEE.root
hadd results/merged/ZGToMuMuG_ee.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEE.root
hadd results/merged/ZGToTauTauG_ee.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
hadd results/merged/ZGToNuNuG_ee.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEE.root

echo "Now merging MM datasets..."
for i in 600
do
  hadd results/merged/ggH$i\2L2Nu_mm.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
  hadd results/merged/ggH$i\LNuTauNu_mm.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
  hadd results/merged/qqH$i\2L2Nu_mm.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
  hadd results/merged/qqH$i\LNuTauNu_mm.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
done
hadd results/merged/TTbar_mm.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetMM.root
hadd results/merged/SingleT_sChannel_mm.root results/Fall11_V1/T_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/SingleTbar_sChannel_mm.root results/Fall11_V1/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/SingleT_tChannel_mm.root results/Fall11_V1/T_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/SingleTbar_tChannel_mm.root results/Fall11_V1/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/SingleT_tWChannel_mm.root results/Fall11_V1/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/SingleTbar_tWChannel_mm.root results/Fall11_V1/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetMM.root
hadd results/merged/Wjets_mm.root results/Fall11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
hadd results/merged/Zee_Hi_mm.root results/Fall11_V1/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetMM.root
hadd results/merged/Zmm_Hi_mm.root results/Fall11_V1/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetMM.root
hadd results/merged/Ztautau_Hi_mm.root results/Fall11_V1/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*datasetMM.root
hadd results/merged/Zee_Lo_mm.root results/Fall11_V1/DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetMM.root
hadd results/merged/Zmm_Lo_mm.root results/Fall11_V1/DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetMM.root
hadd results/merged/Ztautau_Lo_mm.root results/Fall11_V1/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*datasetMM.root
hadd results/merged/WW_mm.root results/Fall11_V1/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
hadd results/merged/ggWW_mm.root results/Fall11_V1/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*datasetMM.root
hadd results/merged/WZ_mm.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
hadd results/merged/ZZ_mm.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root
hadd results/merged/WGToENuG_mm.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetMM.root
hadd results/merged/WGToMuNuG_mm.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetMM.root
hadd results/merged/WGToTauNuG_mm.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
hadd results/merged/ZGToEEG_mm.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetMM.root
hadd results/merged/ZGToMuMuG_mm.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetMM.root
hadd results/merged/ZGToTauTauG_mm.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
hadd results/merged/ZGToNuNuG_mm.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetMM.root

echo "Now merging EM datasets..."
for i in 600
do
  hadd results/merged/ggH$i\2L2Nu_em.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
  hadd results/merged/ggH$i\LNuTauNu_em.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
  hadd results/merged/qqH$i\2L2Nu_em.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
  hadd results/merged/qqH$i\LNuTauNu_em.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
done
hadd results/merged/TTbar_em.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetEM.root
hadd results/merged/SingleT_sChannel_em.root results/Fall11_V1/T_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/SingleTbar_sChannel_em.root results/Fall11_V1/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/SingleT_tChannel_em.root results/Fall11_V1/T_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/SingleTbar_tChannel_em.root results/Fall11_V1/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/SingleT_tWChannel_em.root results/Fall11_V1/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/SingleTbar_tWChannel_em.root results/Fall11_V1/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetEM.root
hadd results/merged/Wjets_em.root results/Fall11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
hadd results/merged/Zee_Hi_em.root results/Fall11_V1/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEM.root
hadd results/merged/Zmm_Hi_em.root results/Fall11_V1/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEM.root
hadd results/merged/Ztautau_Hi_em.root results/Fall11_V1/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*datasetEM.root
hadd results/merged/Zee_Lo_em.root results/Fall11_V1/DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEM.root
hadd results/merged/Zmm_Lo_em.root results/Fall11_V1/DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetEM.root
hadd results/merged/Ztautau_Lo_em.root results/Fall11_V1/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*datasetEM.root
hadd results/merged/WW_em.root results/Fall11_V1/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
hadd results/merged/ggWW_em.root results/Fall11_V1/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*datasetEM.root
hadd results/merged/WZ_em.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
hadd results/merged/ZZ_em.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root
hadd results/merged/WGToENuG_em.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetEM.root
hadd results/merged/WGToMuNuG_em.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetEM.root
hadd results/merged/WGToTauNuG_em.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
hadd results/merged/ZGToEEG_em.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEM.root
hadd results/merged/ZGToMuMuG_em.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEM.root
hadd results/merged/ZGToTauTauG_em.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
hadd results/merged/ZGToNuNuG_em.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEM.root

echo "Now merging ME datasets..."
for i in 600
do
  hadd results/merged/ggH$i\2L2Nu_me.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
  hadd results/merged/ggH$i\LNuTauNu_me.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
  hadd results/merged/qqH$i\2L2Nu_me.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
  hadd results/merged/qqH$i\LNuTauNu_me.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
done
hadd results/merged/TTbar_me.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetME.root
hadd results/merged/SingleT_sChannel_me.root results/Fall11_V1/T_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/SingleTbar_sChannel_me.root results/Fall11_V1/Tbar_TuneZ2_s-channel_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/SingleT_tChannel_me.root results/Fall11_V1/T_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/SingleTbar_tChannel_me.root results/Fall11_V1/Tbar_TuneZ2_t-channel_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/SingleT_tWChannel_me.root results/Fall11_V1/T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/SingleTbar_tWChannel_me.root results/Fall11_V1/Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola/*datasetME.root
hadd results/merged/Wjets_me.root results/Fall11_V1/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
hadd results/merged/Zee_Hi_me.root results/Fall11_V1/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetME.root
hadd results/merged/Zmm_Hi_me.root results/Fall11_V1/DYToMuMu_M-20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetME.root
hadd results/merged/Ztautau_Hi_me.root results/Fall11_V1/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/*datasetME.root
hadd results/merged/Zee_Lo_me.root results/Fall11_V1/DYToEE_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetME.root
hadd results/merged/Zmm_Lo_me.root results/Fall11_V1/DYToMuMu_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia/*datasetME.root
hadd results/merged/Ztautau_Lo_me.root results/Fall11_V1/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/*datasetME.root
hadd results/merged/WW_me.root results/Fall11_V1/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
hadd results/merged/ggWW_me.root results/Fall11_V1/GluGluToWWTo4L_TuneZ2_7TeV-gg2ww-pythia6/*datasetME.root
hadd results/merged/WZ_me.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
hadd results/merged/ZZ_me.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
hadd results/merged/WGToENuG_me.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetME.root
hadd results/merged/WGToMuNuG_me.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetME.root
hadd results/merged/WGToTauNuG_me.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
hadd results/merged/ZGToEEG_me.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetME.root
hadd results/merged/ZGToMuMuG_me.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetME.root
hadd results/merged/ZGToTauTauG_me.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
hadd results/merged/ZGToNuNuG_me.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetME.root



