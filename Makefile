COMPILER = g++44
CFLAGS = -std=c++0x
TFLAGS = -pthread
SOURCES = src/dsection.cpp src/utils.cpp src/distributions.cpp

all: dsection

dsection: $(SOURCES)
	$(COMPILER) $(CFLAGS) $(SOURCES) $(TFLAGS) -o bin/dsection 