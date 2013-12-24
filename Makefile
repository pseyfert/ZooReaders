LD = gcc
CXXFLAGS = $(shell root-config --cflags) -fPIC
LDFLAGS = $(shell root-config --libs) -Lhelpers -lhelpers -LZoo -lZooROOT
###TARGET: todo

ZooReader_options.o: CXXFLAGS += -std=c++11
ZooReader_glob_options.o: CXXFLAGS += -std=c++11
