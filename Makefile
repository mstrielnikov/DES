CXX = g++
CXXFILES = des.cpp 
CXXFLAGS = -O3 -o des -Wall -g -rdynamic -L/lib/

all:
	$(CXX) $(CXXFILES) $(CXXFLAGS)

clean: rm -f des *.o