LD = gcc
CXXFLAGS = $(shell root-config --cflags) -fPIC
LDFLAGS = $(shell root-config --libs)
TARGET: libhelpers.so

libhelpers.so: option.o logstream.o treefunctions.o
	$(LD) -o $@ -shared $^
