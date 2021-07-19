# vim: set ts=4:

CXX=g++
CXXFLAGS=-fdiagnostics-color -std=c++11 -I./ $(shell pkg-config --cflags --libs Magick++)


# our programm
gpx2imgseq: main.cpp file.o parse.o image.o option.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


# a little helper to test Magick++ related stuff
im: im.o image.o
	$(CXX) $(CXXFLAGS) -o $@ $^


# a little helper to test getopt related stuff
op: op.cpp option.o
	$(CXX) -std=c++11 -o $@ $^


clean:
	@rm *.o


.PHONY: clean
# EOF
