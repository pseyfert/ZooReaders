LD = gcc
CXXFLAGS = $(shell root-config --cflags) -fPIC
LDFLAGS = $(shell root-config --libs) -Lhelpers -lhelpers
###TARGET: todo

