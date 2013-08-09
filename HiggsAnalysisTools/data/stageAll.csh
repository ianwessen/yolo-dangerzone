echo "staging signal..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H120 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H120/" $9}' | xargs -i stager_get -U HiggsH120 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H130 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H130/" $9}' | xargs -i stager_get -U HiggsH130 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H140 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H140/" $9}' | xargs -i stager_get -U HiggsH140 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H150 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H150/" $9}' | xargs -i stager_get -U HiggsH150 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H160 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H160/" $9}' | xargs -i stager_get -U HiggsH160 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H165 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H165/" $9}' | xargs -i stager_get -U HiggsH165 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H170 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H170/" $9}' | xargs -i stager_get -U HiggsH170 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H180 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H180/" $9}' | xargs -i stager_get -U HiggsH180 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H190 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H190/" $9}' | xargs -i stager_get -U HiggsH190 -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H200 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Higgs/H200/" $9}' | xargs -i stager_get -U HiggsH200 -M {}

echo "staging WW MC at NLO..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWee | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWee/" $9}' | xargs -i stager_get -U WW_MCatNLOWWee -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWem | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWem/" $9}' | xargs -i stager_get -U WW_MCatNLOWWem -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWet | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWet/" $9}' | xargs -i stager_get -U WW_MCatNLOWWet -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWme | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWme/" $9}' | xargs -i stager_get -U WW_MCatNLOWWme -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWmm | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWmm/" $9}' | xargs -i stager_get -U WW_MCatNLOWWmm -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWmt | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWmt/" $9}' | xargs -i stager_get -U WW_MCatNLOWWmt -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWte | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWte/" $9}' | xargs -i stager_get -U WW_MCatNLOWWte -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWtm | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWtm/" $9}' | xargs -i stager_get -U WW_MCatNLOWWtm -M {}
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWtt | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_MCatNLO/WWtt/" $9}' | xargs -i stager_get -U WW_MCatNLOWWtt -M {}

echo "staging WW incl..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WW_incl | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WW_incl/" $9}' | xargs -i stager_get -U WW_incl -M {}

echo "staging Wgamma..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Wgamma | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Wgamma/" $9}' | xargs -i stager_get -U Wgamma -M {}

echo "staging ZZ_incl..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/ZZ_incl | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/ZZ_incl/" $9}' | xargs -i stager_get -U ZZ_incl -M {}

echo "staging WZ..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/WZ | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/WZ/" $9}' | xargs -i stager_get -U WZ -M {}

echo "staging tW_incl..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/tW_incl | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/tW_incl/" $9}' | xargs -i stager_get -U tW_incl -M {}

echo "staging Drell Yan..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/DrellYan_ll_10-40 | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/DrellYan_ll_10-40/" $9}' | xargs -i stager_get -U DrellYan_ll_10-40 -M {}

echo "staging Chowder PDElectron..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Chowder/PDElectron/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Chowder/PDElectron/Skim/" $9}' | xargs -i stager_get -U ChowderPDElectronSkim -M {}

echo "staging Chowder PDMuon..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Chowder/PDMuon/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Chowder/PDMuon/Skim/" $9}' | xargs -i stager_get -U ChowderPDMuonSkim -M {}

echo "staging Stew PDElectron..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Stew/PDElectron/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Stew/PDElectron/Skim/" $9}' | xargs -i stager_get -U StewPDElectronSkim -M {}

echo "staging Stew PDMuon..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Stew/PDMuon/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Stew/PDMuon/Skim/" $9}' | xargs -i stager_get -U StewPDMuonSkim -M {}

echo "staging Gumbo PDElectron..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Gumbo/PDElectron/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Gumbo/PDElectron/Skim/" $9}' | xargs -i stager_get -U GumboPDElectronSkim -M {}

echo "staging Gumbo PDMuon..."
rfdir /castor/cern.ch/user/e/emanuele/Higgs169_new/Gumbo/PDMuon/Skim | awk '{print "/castor/cern.ch/user/e/emanuele/Higgs169_new/Gumbo/PDMuon/Skim/" $9}' | xargs -i stager_get -U GumboPDMuonSkim -M {}


echo "reporting all...."
stager_qry -U HiggsH120
stager_qry -U HiggsH130
stager_qry -U HiggsH140
stager_qry -U HiggsH150
stager_qry -U HiggsH160
stager_qry -U HiggsH165
stager_qry -U HiggsH170
stager_qry -U HiggsH180
stager_qry -U HiggsH190
stager_qry -U HiggsH200
stager_qry -U WW_MCatNLOWWee
stager_qry -U WW_MCatNLOWWem
stager_qry -U WW_MCatNLOWWet
stager_qry -U WW_MCatNLOWWme
stager_qry -U WW_MCatNLOWWmm
stager_qry -U WW_MCatNLOWWmt
stager_qry -U WW_MCatNLOWWte
stager_qry -U WW_MCatNLOWWtm
stager_qry -U WW_MCatNLOWWtt
stager_qry -U WW_incl
stager_qry -U Wgamma
stager_qry -U ZZ_incl
stager_qry -U WZ
stager_qry -U tW_incl
stager_qry -U DrellYan_ll_10
stager_qry -U ChowderPDElectronSkim
stager_qry -U ChowderPDMuonSkim
stager_qry -U StewPDElectronSkim
stager_qry -U StewPDMuonSkim
stager_qry -U GumboPDElectronSkim
stager_qry -U GumboPDMuonSkim

echo "finished staging..."



