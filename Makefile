
# IDIR=./

CXX = g++

.PHONY: build

# Compiler flags
CXXFLAGS = --std=c++17

# Include directories
INCLUDES = -I/usr/local/include/opencv4 -I/usr/local/cuda/include -I/usr/local/cuda/targets/x86_64-linux/include

# Libraries
LIBS = -lopencv_core -lopencv_videoio -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_cudaimgproc -lcudafilters -ldlib -lcuda

SRC = ./src/face_blur.cpp

OUT = ./bin/face_blur

build:
	$(CXX) $(SRC) -o $(OUT) $(CXXFLAGS) $(INCLUDES) $(LIBS)

build_deps:
	$(CXX) ./src/check_dlib.cpp --std c++17 \
	-o ./bin/check_dlib -Wno-deprecated-gpu-targets \
	-I/usr/local/cuda/include \
	-I/usr/local/cuda/targets/x86_64-linux/include \
	-lcuda -ldlib -lopencv_core

dep_test: build_deps
	./bin/check_dlib

run:
	./bin/check_dlib $(ARGS)

clean:
	rm -f build/* bin/* && echo "project is cleaned"

all: clean build
