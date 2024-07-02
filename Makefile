
CXX = g++

.PHONY: build

CXXFLAGS = --std=c++17

INCLUDES = `pkg-config --cflags --libs opencv` `pkg-config --cflags --libs dlib-1` -I/usr/local/cuda/include -I/usr/local/cuda/targets/x86_64-linux/include 


LIBS = -lcuda

SRC = ./src/face_blur.cpp

OUT = ./bin/face_blur

INPUT_VIDEO = data/input_video.mp4
OUTPUT_VIDEO = data/output_video.mp4
METHOD = caffe

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
	$(OUT) $(METHOD) $(INPUT_VIDEO) $(OUTPUT_VIDEO)


clean:
	rm -f build/* && rm -f data/output_video.mp4 && echo "project is cleaned"

all: clean build
