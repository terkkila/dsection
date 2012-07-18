COMPILER = g++44
CFLAGS = -std=c++0x -O2
DFLAGS = -g -pg
TFLAGS = -pthread
SOURCES = src/dsection.cpp src/utils.cpp src/distributions.cpp src/datadefs.cpp

all: dsection

dsection: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(SOURCES) $(TFLAGS) -o bin/dsection 

gprof: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(DFLAGS) $(SOURCES) $(TFLAGS) -o bin/dsection-gprof