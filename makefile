# vim: set ts=4:

CXX=g++
CXXFLAGS=-I./ $(shell pkg-config --cflags --libs Magick++)


# our programm
gpx2imgseq: main.cpp file.o parse.o image.o option.o
	$(CXX) $(CXXFLAGS) -o $@ $?

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


# a little helper to test Magick++ related stuff
im: im.cpp
	$(CXX) $(CXXFLAGS) -o $@ $?


# a little helper to test getopt related stuff
op: op.cpp
	$(CXX) -o $@ $?


clean:
	@rm *.o


.PHONY: clean
# EOF
