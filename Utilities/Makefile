# Makefile
LIB=lib
TreeReaderDir=TreeReader

CXXFLAGS=-std=c++1z -fPIC -I$(shell root-config --incdir)
CXXFLAGS+=-I$(OPENHF2020TOP)/Utilities
LDFLAGS=$(shell root-config --libs)
CXX=g++

.PHONY: all
all: $(LIB) $(LIB)/libMyTreeReader.so

$(LIB)/libMyTreeReader.so: \
	$(TreeReaderDir)/ParticleTreeMC2.cxx \
	$(TreeReaderDir)/ParticleTreeMC.cxx \
	$(TreeReaderDir)/ParticleTree.cxx \
	Ana/TreeHelpers.cc \
	Ana/Common.cc \
	$(LIB)/MyTreeReaderDict.cxx
	$(CXX) -shared -o $@ $(CXXFLAGS) $(LDFLAGS) $^

$(LIB)/MyTreeReaderDict.cxx: \
	$(TreeReaderDir)/ParticleTreeMC2.hxx \
	$(TreeReaderDir)/ParticleTreeMC.hxx \
	$(TreeReaderDir)/ParticleTree.hxx \
	Ana/TreeHelpers.h \
	Ana/Common.h \
	Linkdef.h
	rootcling -f $@ -rmf $(LIB)/MyTreeReaderDict.rootmap -rml $(LIB)/libMyTreeReader.so $^

lib:
	mkdir lib

.PHONY: clean
clean:
	rm -r $(LIB)
