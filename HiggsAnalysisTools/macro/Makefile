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

INCLUDEDIR       = ./
INCLUDEDIRCOMMON = ../../
CXX	         += -I$(INCLUDEDIR) -I$(INCLUDEDIRCOMMON) -I.
OUTLIB	         = $(INCLUDEDIR)/lib/

.SUFFIXES: .cc,.C,.hh,.h
.PREFIXES: ./lib/

$(OUTLIB)addWeightsToTree.o: $(INCLUDEDIR)addWeightsToTree.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)addWeightsToTree.o $<

$(OUTLIB)addWeightsToTreeLooseLoose.o: $(INCLUDEDIR)addWeightsToTreeLooseLoose.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)addWeightsToTreeLooseLoose.o $<

$(OUTLIB)HWWKinematics.o: $(INCLUDEDIRCOMMON)HiggsAnalysisTools/src/HWWKinematics.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIRCOMMON) -o $(OUTLIB)HWWKinematics.o $<

LatinosAnalyzer: $(INCLUDEDIR)/LatinosAnalyzer.cc \
	$(OUTLIB)addWeightsToTree.o \
	$(OUTLIB)HWWKinematics.o
	$(CXX) $(CXXFLAGS) -o LatinosAnalyzer $(OUTLIB)/*o $(GLIBS) $ $<

LatinosLooseLooseAnalyzer: $(INCLUDEDIR)/LatinosLooseLooseAnalyzer.cc \
	$(OUTLIB)addWeightsToTreeLooseLoose.o \
	$(OUTLIB)HWWKinematics.o
	$(CXX) $(CXXFLAGS) -o LatinosLooseLooseAnalyzer $(OUTLIB)/*o $(GLIBS) $ $<

LatinosEmbeddedTauAnalyzer: $(INCLUDEDIR)/LatinosEmbeddedTauAnalyzer.cc \
	$(OUTLIB)addWeightsToTreeLooseLoose.o \
	$(OUTLIB)HWWKinematics.o
	$(CXX) $(CXXFLAGS) -o LatinosEmbeddedTauAnalyzer $(OUTLIB)/*o $(GLIBS) $ $<
