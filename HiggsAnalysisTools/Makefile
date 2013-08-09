ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs) -lTMVA

CXX           = g++
CXXFLAGS      = -g -fPIC -Wno-deprecated -O -ansi -D_GNU_SOURCE -g -O2
LD            = g++
LDFLAGS       = -g
SOFLAGS       = -shared


ARCH         := $(shell root-config --arch)
PLATFORM     := $(shell root-config --platform)


CXXFLAGS      += $(ROOTCFLAGS)
#CXX           += -I./
LIBS           = $(ROOTLIBS) 

NGLIBS         = $(ROOTGLIBS) 
NGLIBS        += -lMinuit -lMinuit2
GLIBS          = $(filter-out -lNew, $(NGLIBS))

# For MT2:
# MT2LIB_INC_DIR := mT2
# MT2LIB_LIB_DIR := mT2/src
# MT2LIB_CPPFLAGS := -I $(MT2LIB_INC_DIR)
# MT2LIB_LDFLAGS  := $(MT2LIB_LIB_DIR)/libMt2.so
# CXXFLAGS += $(MT2LIB_CPPFLAGS)
# LDFLAGS  += $(MT2LIB_LDFLAGS)

INCLUDEDIR       = ./
INCLUDEDIRCOMMON = ../
CXX	         += -I$(INCLUDEDIR) -I$(INCLUDEDIRCOMMON) -I.
OUTLIB	         = $(INCLUDEDIR)/lib/
OUTLIBCOMMON     = $(INCLUDEDIRCOMMON)/CommonTools/lib/

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ./lib/


$(OUTLIB)HiggsBase.o: $(INCLUDEDIR)/src/HiggsBase.C \
	$(INCLUDEDIR)/src/Higgs.cc \
	$(INCLUDEDIR)/src/HiggsSelection.cc \
	$(INCLUDEDIR)/src/HiggsMLSelection.cc \
	$(INCLUDEDIR)/src/HiggsEleIdOptimToyMC.cc \
	$(INCLUDEDIR)/src/RedEleIDOptimTree.cc \
	$(INCLUDEDIR)/src/RedLikeOptimTree.cc \
	$(INCLUDEDIR)/src/HiggsIsolationOptimToyMC.cc \
	$(INCLUDEDIR)/src/RedIsolationOptimTree.cc \
	$(INCLUDEDIR)/src/RedEleIDTree.cc \
	$(INCLUDEDIR)/src/ZplusJetsSelection.cc \
	$(INCLUDEDIR)/src/LeptonPlusFakeSelection.cc \
	$(INCLUDEDIR)/src/LeptonPlusFakeMLSelection.cc \
	$(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_fullEE.cc \
	$(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_fullME.cc \
	$(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_ME.cc \
	$(INCLUDEDIR)/src/HiggsVertexing.cpp \
	$(INCLUDEDIR)/src/VertexTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsBase.o $<
$(OUTLIBCOMMON)Conditions.o: $(INCLUDEDIRCOMMON)/CommonTools/src/Conditions.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)Conditions.o $<
$(OUTLIBCOMMON)PUWeight.o: $(INCLUDEDIRCOMMON)/CommonTools/src/PUWeight.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)PUWeight.o $<
$(OUTLIBCOMMON)Utils.o: $(INCLUDEDIRCOMMON)/CommonTools/src/Utils.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIBCOMMON)Utils.o $<
$(OUTLIBCOMMON)Counters.o: $(INCLUDEDIRCOMMON)/CommonTools/src/Counters.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)Counters.o $<
$(OUTLIBCOMMON)Selection.o: $(INCLUDEDIRCOMMON)/CommonTools/src/Selection.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)Selection.o $<
$(OUTLIBCOMMON)EfficiencyEvaluator.o: $(INCLUDEDIRCOMMON)/CommonTools/src/EfficiencyEvaluator.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)EfficiencyEvaluator.o $<
$(OUTLIBCOMMON)Monitor.o: $(INCLUDEDIRCOMMON)/CommonTools/src/Monitor.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)Monitor.o $<
$(OUTLIBCOMMON)SprDataFiller.o: $(INCLUDEDIRCOMMON)/CommonTools/src/SprDataFiller.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)SprDataFiller.o $<
$(OUTLIBCOMMON)TriggerMask.o: $(INCLUDEDIRCOMMON)/CommonTools/src/TriggerMask.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIBCOMMON)TriggerMask.o $<
$(OUTLIB)EcalCleaner.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/EcalCleaner.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)EcalCleaner.o $<
$(OUTLIB)CutBasedEleIDSelector.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/CutBasedEleIDSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)CutBasedEleIDSelector.o $<
$(OUTLIB)CiCBasedEleSelector.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/CiCBasedEleSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)CiCBasedEleSelector.o $<
$(OUTLIB)Higgs.o: $(INCLUDEDIR)/src/Higgs.cc $(OUTLIB)JetCorrectionUncertainty.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)Higgs.o $<
$(OUTLIB)JetCorrectorParameters.o: $(INCLUDEDIR)/src/JetCorrectorParameters.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)JetCorrectorParameters.o $<
$(OUTLIB)SimpleJetCorrectionUncertainty.o: $(INCLUDEDIR)/src/SimpleJetCorrectionUncertainty.cc \
	$(OUTLIB)JetCorrectorParameters.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)SimpleJetCorrectionUncertainty.o $<
$(OUTLIB)JetCorrectionUncertainty.o: $(INCLUDEDIR)/src/JetCorrectionUncertainty.cc \
	$(OUTLIB)SimpleJetCorrectionUncertainty.o
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)JetCorrectionUncertainty.o $<
$(OUTLIB)HiggsSelection.o: $(INCLUDEDIR)/src/HiggsSelection.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsSelection.o $<
$(OUTLIB)HiggsMLSelection.o: $(INCLUDEDIR)/src/HiggsMLSelection.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsMLSelection.o $<
$(OUTLIB)HiggsEleIdOptimToyMC.o: $(INCLUDEDIR)/src/HiggsEleIdOptimToyMC.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsEleIdOptimToyMC.o $<
$(OUTLIB)RedEleIDOptimTree.o: $(INCLUDEDIR)/src/RedEleIDOptimTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedEleIDOptimTree.o $<
$(OUTLIB)RedLikeOptimTree.o: $(INCLUDEDIR)/src/RedLikeOptimTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedLikeOptimTree.o $<
$(OUTLIB)HiggsIsolationOptimToyMC.o: $(INCLUDEDIR)/src/HiggsIsolationOptimToyMC.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsIsolationOptimToyMC.o $<
$(OUTLIB)HiggsKinematicsOptimToyMC.o: $(INCLUDEDIR)/src/HiggsKinematicsOptimToyMC.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsKinematicsOptimToyMC.o $<
$(OUTLIB)RedIsolationOptimTree.o: $(INCLUDEDIR)/src/RedIsolationOptimTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedIsolationOptimTree.o $<
$(OUTLIB)RedEleIDTree.o: $(INCLUDEDIR)/src/RedEleIDTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedEleIDTree.o $<
$(OUTLIB)kFactorEvaluator.o: $(INCLUDEDIR)/src/kFactorEvaluator.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)kFactorEvaluator.o $<
$(OUTLIB)ElectronIDMVA.o: $(INCLUDEDIR)/src/ElectronIDMVA.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ElectronIDMVA.o $<
$(OUTLIB)IDForBsMVA.o: $(INCLUDEDIR)/src/IDForBsMVA.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)IDForBsMVA.o $<
$(OUTLIB)tH_leptonMcCorrections.o: $(INCLUDEDIR)/src/tH_leptonMcCorrections.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)tH_leptonMcCorrections.o $<
$(OUTLIB)GetDYMVA.o: $(INCLUDEDIR)/src/GetDYMVA.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)GetDYMVA.o $<
$(OUTLIB)RedHiggsTree.o: $(INCLUDEDIR)/src/RedHiggsTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedHiggsTree.o $<
$(OUTLIB)RedTriggerTree.o: $(INCLUDEDIR)/src/RedTriggerTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedTriggerTree.o $<
$(OUTLIB)CommonHiggsPreselector.o: $(INCLUDEDIR)/src/CommonHiggsPreselector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)CommonHiggsPreselector.o $<
$(OUTLIB)CutBasedHiggsSelector.o: $(INCLUDEDIR)/src/CutBasedHiggsSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)CutBasedHiggsSelector.o $<
$(OUTLIB)ZplusJetsSelection.o: $(INCLUDEDIR)/src/ZplusJetsSelection.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ZplusJetsSelection.o $<
$(OUTLIB)LeptonPlusFakeSelection.o: $(INCLUDEDIR)/src/LeptonPlusFakeSelection.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)LeptonPlusFakeSelection.o $<
$(OUTLIB)LeptonPlusFakeMLSelection.o: $(INCLUDEDIR)/src/LeptonPlusFakeMLSelection.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)LeptonPlusFakeMLSelection.o $<
$(OUTLIB)LeptonPlusFakeMLSelection_fullEE.o: $(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_fullEE.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)LeptonPlusFakeMLSelection_fullEE.o $<
$(OUTLIB)LeptonPlusFakeMLSelection_fullME.o: $(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_fullME.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)LeptonPlusFakeMLSelection_fullME.o $<
$(OUTLIB)LeptonPlusFakeMLSelection_ME.o: $(INCLUDEDIR)/src/LeptonPlusFakeMLSelection_ME.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)LeptonPlusFakeMLSelection_ME.o $<
$(OUTLIB)HiggsVertexing.o: $(INCLUDEDIR)/src/HiggsVertexing.cpp
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)HiggsVertexing.o $<
$(OUTLIB)VertexTree.o: $(INCLUDEDIR)/src/VertexTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)VertexTree.o $<
$(OUTLIB)ElectronBestCandidateSelector.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/ElectronBestCandidateSelector.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)ElectronBestCandidateSelector.o $<
$(OUTLIB)BestLeptonSelectorWjets.o: $(INCLUDEDIR)/src/BestLeptonSelectorWjets.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)BestLeptonSelectorWjets.o $<
$(OUTLIB)LikelihoodPdf.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/LikelihoodPdf.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)LikelihoodPdf.o $<
$(OUTLIB)LikelihoodSpecies.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/LikelihoodSpecies.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)LikelihoodSpecies.o $<
$(OUTLIB)LikelihoodPdfProduct.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/LikelihoodPdfProduct.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)LikelihoodPdfProduct.o $<
$(OUTLIB)ElectronLikelihood.o: $(INCLUDEDIRCOMMON)/EgammaAnalysisTools/src/ElectronLikelihood.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)ElectronLikelihood.o $<

# no more used ------------------------------------
#$(OUTLIB)ElectronID.o: $(INCLUDEDIR)/src/ElectronID.C
#	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)ElectronID.o $<
#$(OUTLIB)RedEWKTree.o: $(INCLUDEDIR)/src/RedEWKTree.cc
#	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedEWKTree.o $<
#$(OUTLIB)RedEleIDTree.o: $(INCLUDEDIR)/src/RedEleIDTree.cc
#	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)RedEleIDTree.o $<
#$(OUTLIB)eleID_Higgs_Studies.o: $(INCLUDEDIR)/src/eleID_Higgs_Studies.cc
#	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)eleID_Higgs_Studies.o $<

#----------------------------------------------------#

# ==================== HiggsApp =============================================
HiggsApp:  $(INCLUDEDIR)/src/HiggsApp.C \
	$(OUTLIB)HiggsBase.o \
	$(OUTLIB)Higgs.o \
	$(OUTLIBCOMMON)Conditions.o \
	$(OUTLIBCOMMON)PUWeight.o \
	$(OUTLIBCOMMON)Selection.o \
	$(OUTLIBCOMMON)EfficiencyEvaluator.o \
	$(OUTLIBCOMMON)Counters.o \
	$(OUTLIBCOMMON)Monitor.o \
	$(OUTLIBCOMMON)SprDataFiller.o \
	$(OUTLIBCOMMON)TriggerMask.o \
	$(OUTLIBCOMMON)Utils.o \
	$(OUTLIB)kFactorEvaluator.o \
	$(OUTLIB)ElectronIDMVA.o \
	$(OUTLIB)IDForBsMVA.o \
	$(OUTLIB)tH_leptonMcCorrections.o \
	$(OUTLIB)GetDYMVA.o \
	$(OUTLIB)RedHiggsTree.o \
	$(OUTLIB)RedTriggerTree.o \
	$(OUTLIB)RedEleIDOptimTree.o \
	$(OUTLIB)RedLikeOptimTree.o \
	$(OUTLIB)RedIsolationOptimTree.o \
	$(OUTLIB)RedEleIDTree.o \
	$(OUTLIB)CutBasedEleIDSelector.o \
	$(OUTLIB)CiCBasedEleSelector.o \
	$(OUTLIB)EcalCleaner.o \
	$(OUTLIB)CommonHiggsPreselector.o \
	$(OUTLIB)CutBasedHiggsSelector.o \
	$(OUTLIB)VertexTree.o \
	$(OUTLIB)LikelihoodPdf.o \
	$(OUTLIB)LikelihoodSpecies.o \
	$(OUTLIB)LikelihoodPdfProduct.o \
	$(OUTLIB)ElectronLikelihood.o
	$(CXX) $(CXXFLAGS) -ldl -o HiggsApp $(OUTLIB)/*.o $(OUTLIBCOMMON)/*o $(GLIBS) $(LDFLAGS) $ $<
HiggsApp.clean:
	rm -f HiggsApp

# ==================== eleID =============================================
eleID_Higgs_Studies:  $(INCLUDEDIR)eleID_Higgs_Studies.cpp $(OUTLIB)RedEleIDTree.o 
	$(CXX) $(CXXFLAGS) -o eleID_Higgs_Studies $(OUTLIB)/*.o $(GLIBS) $ $<
#eleID_Higgs_Studies.clean:
#	rm -f eleID_Higgs_Studies

eleIDtableToy:  $(INCLUDEDIR)/src/eleIDtableToy.cpp
	$(CXX) $(CXXFLAGS) -o eleIDtableToy $(GLIBS) $ $<
eleIDtoyPlot_input:  $(INCLUDEDIR)/src/eleIDtoyPlot_input.cpp
	$(CXX) $(CXXFLAGS) -o eleIDtoyPlot_input $(GLIBS) $ $<
eleIDtableToy.clean:
	rm -f eleIDtableToy
eleIDtoyPlot_input.clean:
	rm -f eleIDtoyPlot_input
eleIDtableLike:  $(INCLUDEDIR)/src/eleIDtableLike.cpp
	$(CXX) $(CXXFLAGS) -o eleIDtableLike $(GLIBS) $ $<
eleIDtableLike.clean:
	rm -f eleIDtableLike
isolationTableToy:  $(INCLUDEDIR)/src/isolationTableToy.cpp
	$(CXX) $(CXXFLAGS) -o isolationTableToy $(GLIBS) $ $<
isolationTableToy.clean:
	rm -f isolationTableToy
kinematicsTableToy:  $(INCLUDEDIR)/src/kinematicsTableToy.cpp
	$(CXX) $(CXXFLAGS) -o kinematicsTableToy $(GLIBS) $ $<
kinematicsTableToy.clean:
	rm -f kinematicsTableToy
vtxAndIsoOptim:  $(INCLUDEDIR)/src/vtxAndIsoOptim.cpp
	$(CXX) $(CXXFLAGS) -o vtxAndIsoOptim $(GLIBS) $ $<
vtxAndIsoOptim.clean:
	rm -f vtxAndIsoOptim

# ================= other ===================
BestLeptonApp: $(INCLUDEDIR)/src/BestLeptonApp.C \
	$(OUTLIBCOMMON)Conditions.o \
	$(OUTLIBCOMMON)TriggerMask.o \
	$(OUTLIBCOMMON)Utils.o \
	$(OUTLIB)HiggsBase.o \
	$(OUTLIB)ElectronBestCandidateSelector.o \
	$(OUTLIB)BestLeptonSelectorWjets.o
	$(CXX) $(CXXFLAGS) -o BestLeptonApp $(OUTLIB)/*o $(OUTLIBCOMMON)/*o $(GLIBS) $ $<
BestLeptonApp.clean:
	rm -f $(OUTLIB)BestLeptonSelectorWjets.o
	rm -f BestLeptonApp

# ==================== reduced trees =============================================
#ReducedTree_HwwEleId:  $(INCLUDEDIR)ReducedTree_HwwEleId.cpp $(OUTLIB)RedEleIDTree.o 
#	$(CXX) $(CXXFLAGS) -o ReducedTree_HwwEleId $(OUTLIB)/*.o $(GLIBS) $ $<
#ReducedTree_HwwEleId.clean:
#	rm -f ReducedTree_HwwEleId

clean:
	rm -f $(OUTLIB)*.o $(OUTLIBCOMMON)*.o
	rm -f HiggsApp
# rm -f ReducedTree_HwwEleId
# rm -f eleID_Higgs_Studies
	rm -f eleIDtableToy
	rm -f eleIDtoyPlot_input
	rm -f eleIDtableLike
	rm -f isolationTableToy
	rm -f kinematicsTableToy
	rm -f vtxAndIsoOptim

all:  HiggsApp
