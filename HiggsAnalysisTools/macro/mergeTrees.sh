#! /bin/sh 
# ./mergeTrees.sh expects results of the selection in subdirectories such as results/H120, results/bkg1, results/bkg2...
# it creates a merged root file in the self-created results/merged/

year=$1

mkdir -p results/merged

if [ $year -eq "2012" ] ; then
    echo "Now merging EE datasets..."
    for i in 110 115 120 125 130 135 140 145 150 155 160 170 180 190 200 250 300 350 400 450 500 550 600 700 800 900 1000
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_ee.root results/Summer12_V14_52X/GluGluToHToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetEE.root
      hadd results/merged/qqH$i\2LAndTau2Nu_ee.root results/Summer12_V14_52X/VBF_HToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetEE.root
    done

    #hadd results/merged/TTbar_ee.root results/Summer12_V14_52X/TTJets_TuneZ2star_8TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/TTbar_ee.root results/Summer12_V14_52X/TTTo2L2Nu2B_8TeV-powheg-pythia6/*datasetEE.root 
    hadd results/merged/SingleT_tWChannel_ee.root results/Summer12_V14_52X/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetEE.root
    hadd results/merged/SingleTbar_tWChannel_ee.root results/Summer12_V14_52X/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetEE.root
    hadd results/merged/Wjets_ee.root results/Summer12_V14_52X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/*datasetEE.root
    hadd results/merged/Zll_Hi_ee.root results/Summer12_V14_52X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/*datasetEE.root
    hadd results/merged/Zll_Lo_ee.root results/Summer12_V14_52X/DYJetsToLL_M-10To50filter_8TeV-madgraph/*datasetEE.root
    hadd results/merged/WW_ee.root results/Summer12_V14_52X/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/*datasetEE.root 
    hadd results/merged/ggWW_ee.root results/Summer12_V14_52X/GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6/*datasetEE.root
    hadd results/merged/WZ_ee.root results/Summer12_V14_52X/WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola/*datasetEE.root
    #hadd results/merged/WZ_ee.root results/Summer12_V14_52X/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/ZZ_ee.root results/Summer12_V14_52X/ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetEE.root
    hadd results/merged/WGToLNuG_ee.root results/Summer12_V14_52X/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/WGstarToLNu2E_ee.root results/Summer12_V14_52X/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/WGstarToLNu2Mu_ee.root results/Summer12_V14_52X/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/WGstarToLNu2Tau_ee.root results/Summer12_V14_52X/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/*datasetEE.root
    #hadd results/merged/ZGToEEG_ee.root results/Summer12_V14_52X/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEE.root
    #hadd results/merged/ZGToMuMuG_ee.root results/Summer12_V14_52X/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    #hadd results/merged/ZGToTauTauG_ee.root results/Summer12_V14_52X/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
    #hadd results/merged/ZGToNuNuG_ee.root results/Summer12_V14_52X/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    #hadd results/merged/WW_mcatnlo_ee.root results/Summer12_V14_52X/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetEE.root
    #hadd results/merged/WW_mcatnlo_scaledown_ee.root results/Summer12_V14_52X/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetEE.root
    #hadd results/merged/WW_mcatnlo_scaleup_ee.root results/Summer12_V14_52X/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetEE.root
    hadd results/merged/WW_pythia_ee.root results/Summer12_V14_52X/WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetEE.root

    echo "Now merging MM datasets..."
    for i in 110 115 120 125 130 135 140 145 150 155 160 170 180 190 200 250 300 350 400 450 500 550 600 700 800 900 1000
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_mm.root results/Summer12_V14_52X/GluGluToHToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetMM.root
      hadd results/merged/qqH$i\2LAndTau2Nu_mm.root results/Summer12_V14_52X/VBF_HToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetMM.root
    done

    #hadd results/merged/TTbar_mm.root results/Summer12_V14_52X/TTJets_TuneZ2star_8TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/TTbar_mm.root results/Summer12_V14_52X/TTTo2L2Nu2B_8TeV-powheg-pythia6/*datasetMM.root 
    hadd results/merged/SingleT_tWChannel_mm.root results/Summer12_V14_52X/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetMM.root
    hadd results/merged/SingleTbar_tWChannel_mm.root results/Summer12_V14_52X/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetMM.root
    hadd results/merged/Wjets_mm.root results/Summer12_V14_52X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/*datasetMM.root
    hadd results/merged/Zll_Hi_mm.root results/Summer12_V14_52X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/*datasetMM.root
    hadd results/merged/Zll_Lo_mm.root results/Summer12_V14_52X/DYJetsToLL_M-10To50filter_8TeV-madgraph/*datasetMM.root
    hadd results/merged/WW_mm.root results/Summer12_V14_52X/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/*datasetMM.root 
    hadd results/merged/ggWW_mm.root results/Summer12_V14_52X/GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6/*datasetMM.root
    hadd results/merged/WZ_mm.root results/Summer12_V14_52X/WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola/*datasetMM.root
    #hadd results/merged/WZ_mm.root results/Summer12_V14_52X/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/ZZ_mm.root results/Summer12_V14_52X/ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetMM.root
    hadd results/merged/WGToLNuG_mm.root results/Summer12_V14_52X/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/WGstarToLNu2E_mm.root results/Summer12_V14_52X/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/WGstarToLNu2Mu_mm.root results/Summer12_V14_52X/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/WGstarToLNu2Tau_mm.root results/Summer12_V14_52X/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/*datasetMM.root
    #hadd results/merged/ZGToEEG_mm.root results/Summer12_V14_52X/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetMM.root
    #hadd results/merged/ZGToMuMuG_mm.root results/Summer12_V14_52X/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    #hadd results/merged/ZGToTauTauG_mm.root results/Summer12_V14_52X/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
    #hadd results/merged/ZGToNuNuG_mm.root results/Summer12_V14_52X/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    #hadd results/merged/WW_mcatnlo_mm.root results/Summer12_V14_52X/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetMM.root
    #hadd results/merged/WW_mcatnlo_scaledown_mm.root results/Summer12_V14_52X/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetMM.root
    #hadd results/merged/WW_mcatnlo_scaleup_mm.root results/Summer12_V14_52X/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetMM.root
    hadd results/merged/WW_pythia_mm.root results/Summer12_V14_52X/WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetMM.root

    echo "Now merging EM datasets..."
    for i in 110 115 120 125 130 135 140 145 150 155 160 170 180 190 200 250 300 350 400 450 500 550 600 700 800 900 1000
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_em.root results/Summer12_V14_52X/GluGluToHToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetEM.root
      hadd results/merged/qqH$i\2LAndTau2Nu_em.root results/Summer12_V14_52X/VBF_HToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetEM.root
    done

    #hadd results/merged/TTbar_em.root results/Summer12_V14_52X/TTJets_TuneZ2star_8TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/TTbar_em.root results/Summer12_V14_52X/TTTo2L2Nu2B_8TeV-powheg-pythia6/*datasetEM.root 
    hadd results/merged/SingleT_tWChannel_em.root results/Summer12_V14_52X/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetEM.root
    hadd results/merged/SingleTbar_tWChannel_em.root results/Summer12_V14_52X/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetEM.root
    hadd results/merged/Wjets_em.root results/Summer12_V14_52X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/*datasetEM.root
    hadd results/merged/Zll_Hi_em.root results/Summer12_V14_52X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/*datasetEM.root
    hadd results/merged/Zll_Lo_em.root results/Summer12_V14_52X/DYJetsToLL_M-10To50filter_8TeV-madgraph/*datasetEM.root
    hadd results/merged/WW_em.root results/Summer12_V14_52X/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/*datasetEM.root 
    hadd results/merged/ggWW_em.root results/Summer12_V14_52X/GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6/*datasetEM.root
    hadd results/merged/WZ_em.root results/Summer12_V14_52X/WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola/*datasetEM.root
    #hadd results/merged/WZ_em.root results/Summer12_V14_52X/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/ZZ_em.root results/Summer12_V14_52X/ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetEM.root
    hadd results/merged/WGToLNuG_em.root results/Summer12_V14_52X/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/WGstarToLNu2E_em.root results/Summer12_V14_52X/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/WGstarToLNu2Mu_em.root results/Summer12_V14_52X/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/WGstarToLNu2Tau_em.root results/Summer12_V14_52X/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/*datasetEM.root
    #hadd results/merged/ZGToEEG_em.root results/Summer12_V14_52X/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEM.root
    #hadd results/merged/ZGToMuMuG_em.root results/Summer12_V14_52X/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    #hadd results/merged/ZGToTauTauG_em.root results/Summer12_V14_52X/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
    #hadd results/merged/ZGToNuNuG_em.root results/Summer12_V14_52X/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    #hadd results/merged/WW_mcatnlo_em.root results/Summer12_V14_52X/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetEM.root
    #hadd results/merged/WW_mcatnlo_scaledown_em.root results/Summer12_V14_52X/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetEM.root
    #hadd results/merged/WW_mcatnlo_scaleup_em.root results/Summer12_V14_52X/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetEM.root
    hadd results/merged/WW_pythia_em.root results/Summer12_V14_52X/WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetEM.root

    echo "Now merging ME datasets..."
    for i in 110 115 120 125 130 135 140 145 150 155 160 170 180 190 200 250 300 350 400 450 500 550 600 700 800 900 1000
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_me.root results/Summer12_V14_52X/GluGluToHToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetME.root
      hadd results/merged/qqH$i\2LAndTau2Nu_me.root results/Summer12_V14_52X/VBF_HToWWTo2LAndTau2Nu_M-$i\_8TeV-powheg-pythia6/*datasetME.root
    done

    #hadd results/merged/TTbar_me.root results/Summer12_V14_52X/TTJets_TuneZ2star_8TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/TTbar_me.root results/Summer12_V14_52X/TTTo2L2Nu2B_8TeV-powheg-pythia6/*datasetME.root 
    hadd results/merged/SingleT_tWChannel_me.root results/Summer12_V14_52X/T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetME.root
    hadd results/merged/SingleTbar_tWChannel_me.root results/Summer12_V14_52X/Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola/*datasetME.root
    hadd results/merged/Wjets_me.root results/Summer12_V14_52X/WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball/*datasetME.root
    hadd results/merged/Zll_Hi_me.root results/Summer12_V14_52X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/*datasetME.root
    hadd results/merged/Zll_Lo_me.root results/Summer12_V14_52X/DYJetsToLL_M-10To50filter_8TeV-madgraph/*datasetME.root
    hadd results/merged/WW_me.root results/Summer12_V14_52X/WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola/*datasetME.root 
    hadd results/merged/ggWW_me.root results/Summer12_V14_52X/GluGluToWWTo4L_TuneZ2star_8TeV-gg2ww-pythia6/*datasetME.root
    hadd results/merged/WZ_me.root results/Summer12_V14_52X/WZTo3LNu_TuneZ2star_8TeV_pythia6_tauola/*datasetME.root
    #hadd results/merged/WZ_me.root results/Summer12_V14_52X/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/ZZ_me.root results/Summer12_V14_52X/ZZTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetME.root
    hadd results/merged/WGToLNuG_me.root results/Summer12_V14_52X/WGToLNuG_TuneZ2star_8TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/WGstarToLNu2E_me.root results/Summer12_V14_52X/WGstarToLNu2E_TuneZ2star_8TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/WGstarToLNu2Mu_me.root results/Summer12_V14_52X/WGstarToLNu2Mu_TuneZ2star_7TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/WGstarToLNu2Tau_me.root results/Summer12_V14_52X/WGstarToLNu2Tau_TuneZ2star_7TeV-madgraph-tauola/*datasetME.root
    #hadd results/merged/ZGToEEG_me.root results/Summer12_V14_52X/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetME.root
    #hadd results/merged/ZGToMuMuG_me.root results/Summer12_V14_52X/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetME.root
    #hadd results/merged/ZGToTauTauG_me.root results/Summer12_V14_52X/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
    #hadd results/merged/ZGToNuNuG_me.root results/Summer12_V14_52X/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetME.root
    #hadd results/merged/WW_mcatnlo_me.root results/Summer12_V14_52X/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetME.root
    #hadd results/merged/WW_mcatnlo_scaledown_me.root results/Summer12_V14_52X/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetME.root
    #hadd results/merged/WW_mcatnlo_scaleup_me.root results/Summer12_V14_52X/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetME.root
    hadd results/merged/WW_pythia_me.root results/Summer12_V14_52X/WWTo2L2Nu_TuneZ2star_8TeV_pythia6_tauola/*datasetME.root


fi



if [ $year -eq "2011" ] ; then
    echo "Now merging EE datasets..."
    for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
      do
      hadd results/merged/ggH$i\2L2Nu_ee.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
      hadd results/merged/ggH$i\LNuTauNu_ee.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
      hadd results/merged/qqH$i\2L2Nu_ee.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
      hadd results/merged/qqH$i\LNuTauNu_ee.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
    done
    for i in 110 115 118 122 124 126 128 135
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_ee.root results/Fall11_V1/GluGluToHToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
      hadd results/merged/qqH$i\2LAndTau2Nu_ee.root results/Fall11_V1/VBF_HToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEE.root
    done

    hadd results/merged/TTbar_ee.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetEE.root 
#hadd results/merged/TTbar_ee.root results/Fall11_V1/TTJets_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
#hadd results/merged/SingleTop_sChannel_ee.root results/Fall11_V1/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*datasetEE.root
#hadd results/merged/SingleTop_tChannel_ee.root results/Fall11_V1/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*datasetEE.root
#hadd results/merged/SingleTop_tWChannel_ee.root results/Fall11_V1/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*datasetEE.root
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
#hadd results/merged/WZ_ee.root results/Fall11_V1/WZTo3LNu_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root
#hadd results/merged/ZZ_ee.root results/Fall11_V1/ZZ_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root
    hadd results/merged/WZ_ee.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/ZZ_ee.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root
    hadd results/merged/WGToENuG_ee.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    hadd results/merged/WGToMuNuG_ee.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    hadd results/merged/WGToTauNuG_ee.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/ZGToEEG_ee.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEE.root
    hadd results/merged/ZGToMuMuG_ee.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    hadd results/merged/ZGToTauTauG_ee.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEE.root
    hadd results/merged/ZGToNuNuG_ee.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEE.root
    hadd results/merged/WW_mcatnlo_ee.root results/Fall11_V1/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetEE.root
    hadd results/merged/WW_mcatnlo_scaledown_ee.root results/Fall11_V1/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetEE.root
    hadd results/merged/WW_mcatnlo_scaleup_ee.root results/Fall11_V1/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetEE.root
    hadd results/merged/WW_pythia_ee.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root
    hadd results/merged/Wgamma_inclusive_ee.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEE.root

    echo "Now merging MM datasets..."
    for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
      do
      hadd results/merged/ggH$i\2L2Nu_mm.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
      hadd results/merged/ggH$i\LNuTauNu_mm.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
      hadd results/merged/qqH$i\2L2Nu_mm.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
      hadd results/merged/qqH$i\LNuTauNu_mm.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
    done
    for i in 110 115 118 122 124 126 128 135
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_mm.root results/Fall11_V1/GluGluToHToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
      hadd results/merged/qqH$i\2LAndTau2Nu_mm.root results/Fall11_V1/VBF_HToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetMM.root
    done
    hadd results/merged/TTbar_mm.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetMM.root
#hadd results/merged/TTbar_mm.root results/Fall11_V1/TTJets_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
#hadd results/merged/SingleTop_sChannel_mm.root results/Fall11_V1/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*datasetMM.root
#hadd results/merged/SingleTop_tChannel_mm.root results/Fall11_V1/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*datasetMM.root
#hadd results/merged/SingleTop_tWChannel_mm.root results/Fall11_V1/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*datasetMM.root
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
#hadd results/merged/WZ_mm.root results/Fall11_V1/WZTo3LNu_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root
#hadd results/merged/ZZ_mm.root results/Fall11_V1/ZZ_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root
    hadd results/merged/WZ_mm.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/ZZ_mm.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root
    hadd results/merged/WGToENuG_mm.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    hadd results/merged/WGToMuNuG_mm.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    hadd results/merged/WGToTauNuG_mm.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/ZGToEEG_mm.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetMM.root
    hadd results/merged/ZGToMuMuG_mm.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    hadd results/merged/ZGToTauTauG_mm.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetMM.root
    hadd results/merged/ZGToNuNuG_mm.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetMM.root
    hadd results/merged/WW_mcatnlo_mm.root results/Fall11_V1/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetMM.root
    hadd results/merged/WW_mcatnlo_scaledown_mm.root results/Fall11_V1/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetMM.root
    hadd results/merged/WW_mcatnlo_scaleup_mm.root results/Fall11_V1/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetMM.root
    hadd results/merged/WW_pythia_mm.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root
    hadd results/merged/Wgamma_inclusive_mm.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetMM.root

    echo "Now merging EM datasets..."
    for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
      do
      hadd results/merged/ggH$i\2L2Nu_em.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
      hadd results/merged/ggH$i\LNuTauNu_em.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
      hadd results/merged/qqH$i\2L2Nu_em.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
      hadd results/merged/qqH$i\LNuTauNu_em.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
    done
    for i in 110 115 118 122 124 126 128 135
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_em.root results/Fall11_V1/GluGluToHToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
      hadd results/merged/qqH$i\2LAndTau2Nu_em.root results/Fall11_V1/VBF_HToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetEM.root
    done
    hadd results/merged/TTbar_em.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetEM.root
#hadd results/merged/TTbar_em.root results/Fall11_V1/TTJets_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
#hadd results/merged/SingleTop_sChannel_em.root results/Fall11_V1/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*datasetEM.root
#hadd results/merged/SingleTop_tChannel_em.root results/Fall11_V1/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*datasetEM.root
#hadd results/merged/SingleTop_tWChannel_em.root results/Fall11_V1/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*datasetEM.root
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
#hadd results/merged/WZ_em.root results/Fall11_V1/WZTo3LNu_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root
#hadd results/merged/ZZ_em.root results/Fall11_V1/ZZ_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root
    hadd results/merged/WZ_em.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/ZZ_em.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root
    hadd results/merged/WGToENuG_em.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    hadd results/merged/WGToMuNuG_em.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    hadd results/merged/WGToTauNuG_em.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/ZGToEEG_em.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetEM.root
    hadd results/merged/ZGToMuMuG_em.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    hadd results/merged/ZGToTauTauG_em.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetEM.root
    hadd results/merged/ZGToNuNuG_em.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetEM.root
    hadd results/merged/WW_mcatnlo_em.root results/Fall11_V1/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetEM.root
    hadd results/merged/WW_mcatnlo_scaledown_em.root results/Fall11_V1/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetEM.root
    hadd results/merged/WW_mcatnlo_scaleup_em.root results/Fall11_V1/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetEM.root
    hadd results/merged/WW_pythia_em.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root
    hadd results/merged/Wgamma_inclusive_em.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetEM.root

    echo "Now merging ME datasets..."
    for i in 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600
      do
      hadd results/merged/ggH$i\2L2Nu_me.root results/Fall11_V1/GluGluToHToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
      hadd results/merged/ggH$i\LNuTauNu_me.root results/Fall11_V1/GluGluToHToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
      hadd results/merged/qqH$i\2L2Nu_me.root results/Fall11_V1/VBF_HToWWTo2L2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
      hadd results/merged/qqH$i\LNuTauNu_me.root results/Fall11_V1/VBF_HToWWToLNuTauNu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
    done
    for i in 110 115 118 122 124 126 128 135
      do
      hadd results/merged/ggH$i\2LAndTau2Nu_me.root results/Fall11_V1/GluGluToHToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
      hadd results/merged/qqH$i\2LAndTau2Nu_me.root results/Fall11_V1/VBF_HToWWTo2LAndTau2Nu_M-$i\_7TeV-powheg-pythia6/*datasetME.root
    done
    hadd results/merged/TTbar_me.root results/Fall11_V1/TTTo2L2Nu2B_7TeV-powheg-pythia6/*datasetME.root
#hadd results/merged/TTbar_me.root results/Fall11_V1/TTJets_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
#hadd results/merged/SingleTop_sChannel_me.root results/Fall11_V1/TToBLNu_TuneZ2_s-channel_7TeV-madgraph/*datasetME.root
#hadd results/merged/SingleTop_tChannel_me.root results/Fall11_V1/TToBLNu_TuneZ2_t-channel_7TeV-madgraph/*datasetME.root
#hadd results/merged/SingleTop_tWChannel_me.root results/Fall11_V1/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/*datasetME.root
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
#hadd results/merged/WZ_me.root results/Fall11_V1/WZTo3LNu_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
#hadd results/merged/ZZ_me.root results/Fall11_V1/ZZ_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
    hadd results/merged/WZ_me.root results/Fall11_V1/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/ZZ_me.root results/Fall11_V1/ZZTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
    hadd results/merged/WGToENuG_me.root results/Fall11_V1/WGToENuG_TuneZ2_7TeV-madgraph/*datasetME.root
    hadd results/merged/WGToMuNuG_me.root results/Fall11_V1/WGToMuNuG_TuneZ2_7TeV-madgraph/*datasetME.root
    hadd results/merged/WGToTauNuG_me.root results/Fall11_V1/WGToTauNuG_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/ZGToEEG_me.root results/Fall11_V1/ZGToEEG_TuneZ2_7TeV-madgraph/*datasetME.root
    hadd results/merged/ZGToMuMuG_me.root results/Fall11_V1/ZGToMuMuG_TuneZ2_7TeV-madgraph/*datasetME.root
    hadd results/merged/ZGToTauTauG_me.root results/Fall11_V1/ZGToTauTauG_TuneZ2_7TeV-madgraph-tauola/*datasetME.root
    hadd results/merged/ZGToNuNuG_me.root results/Fall11_V1/ZGToNuNuG_TuneZ2_7TeV-madgraph/*datasetME.root
    hadd results/merged/WW_mcatnlo_me.root results/Fall11_V1/WWTo2L2Nu_CT10_7TeV-mcatnlo/*datasetME.root
    hadd results/merged/WW_mcatnlo_scaledown_me.root results/Fall11_V1/WWTo2L2Nu_scaledown_CT10_7TeV-mcatnlo/*datasetME.root
    hadd results/merged/WW_mcatnlo_scaleup_me.root results/Fall11_V1/WWTo2L2Nu_scaleup_CT10_7TeV-mcatnlo/*datasetME.root
    hadd results/merged/WW_pythia_me.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
    hadd results/merged/Wgamma_inclusive_me.root results/Fall11_V1/WWTo2L2Nu_TuneZ2_7TeV_pythia6_tauola/*datasetME.root
fi


