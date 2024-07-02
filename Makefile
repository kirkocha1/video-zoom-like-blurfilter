
# IDIR=./

CXX = g++

.PHONY: build

# Compiler flags
CXXFLAGS = --std=c++17

# Include directories
INCLUDES = `pkg-config --cflags --libs opencv4` `pkg-config --cflags --libs dlib-1` -I/usr/local/cuda/include -I/usr/local/cuda/targets/x86_64-linux/include 

# Libraries
LIBS = -lcuda

SRC = ./src/face_blur.cpp

OUT = ./bin/face_blur

build:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(INCLUDES) $(LIBS)

build_check_dlib:
	$(CXX) ./src/check_dlib.cpp $(CXXFLAGS) -o ./bin/check_dlib $(INCLUDES) $(LIBS)

build_check_opencv:
	$(CXX) ./src/check_opencv.cpp $(CXXFLAGS) -o ./bin/check_opencv $(INCLUDES) $(LIBS)

test_opencv: build_check_opencv
	./bin/check_opencv

test_dlib: build_check_dlib
	./bin/check_dlib

run:
	./bin/check_dlib $(ARGS)

clean:
	rm -f build/* bin/* && echo "project is cleaned"

all: clean build
