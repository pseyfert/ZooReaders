LD = gcc
CXXFLAGS = $(shell root-config --cflags) -fPIC
LDFLAGS = $(shell root-config --glibs) -lGenVector -Lhelpers -lhelpers -LZoo -lZooROOT -LZooFunctors -lZooFunctorROOT
SUBDIRS = Zoo ZooFunctors helpers
###TARGET: todo

include Rules.mk

ZooGuiDict.cpp: ZooGui.h
ZooGui: ZooGui.o funs.o ZooGuiDict.o

ZooReader_options.o: CXXFLAGS += -std=c++11
ZooReader_glob_options.o: CXXFLAGS += -std=c++11
