
# IDIR=./

CXX = g++

.PHONY: build

build:
	echo "build step"

build_deps:
	$(CXX) ./src/check_dlib.cpp --std c++17 \
	-o ./bin/check_dlib -Wno-deprecated-gpu-targets \
	-I/usr/local/cuda/include \
	-I/usr/local/cuda/targets/x86_64-linux/include \
	-lcuda -ldlib

dep_test: build_deps
	./bin/check_dlib

run:
	./bin/check_dlib $(ARGS)

clean:
	rm -f build/* bin/* && echo "project is cleaned"

all: clean build