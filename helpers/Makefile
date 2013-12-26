LD = gcc
CXXFLAGS = $(shell root-config --cflags) -fPIC
LDFLAGS = $(shell root-config --libs)
TARGET: libhelpers.so

option.o: CXXFLAGS+=-std=c++11

libhelpers.so: option.o logstream.o treefunctions.o glob.o
	$(LD) -o $@ -shared $^
