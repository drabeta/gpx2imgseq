# vim: set ts=4:

CXX=g++
CXXFLAGS=-I./ $(shell pkg-config --cflags --libs Magick++)


# our programm
gpx2imgseq: main.cpp image.o
	$(CXX) $(CXXFLAGS) -o $@ $?

image.o: image.cpp image.h
	$(CXX) $(CXXFLAGS) -c -o $@ image.cpp


# a little helper to test Magick++ related stuff
im: im.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?


# a little helper to test getopt related stuff
op: op.cpp
	$(CXX) -o $@ $?


# EOF
