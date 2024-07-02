## Description
This project is an example on how to use OpenCV lib together with CUDA support. It will take a video as an input find all faces in it and blur everything except faces, like zoom filters.

## Results
Videos can be found in data folder:
- result output_video.mp4
- input_video.mp4 the one that will be processed

Executables located in bin folder.

## Installation steps

### Ubuntu set up (check this link https://gist.github.com/raulqf/f42c718a658cddc16f9df07ecc627be7):
```
sudo apt-get install -y build-essential cmake libopenblas-dev liblapack-dev libx11-dev libgtk-3-dev
```

In case OS is Amazon Linux 2 you would need to build ffmpeg tool before compiling opencv


### Build opencv from sources:
- Clone opencv project

```
git clone https://github.com/opencv/opencv.git
cd opencv
git checkout 4.10.0  # Or the version you need
git clone https://github.com/opencv/opencv_contrib.git
```

#### Build with cmake and enable CUDA support together with FFMPEG as it is needed for video processing
```
sudo rm -rf build && mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -D WITH_CUDA=ON -D WITH_CUDNN=ON -D OPENCV_DNN_CUDA=ON WITH_FFMPEG=ON -D WITH_GSTREAMER=ON -D OPENCV_GENERATE_PKGCONFIG=ON -D OPENCV_PC_FILE_NAME=opencv.pc ..
```

#### Compile and build (can take up to 3-4 hours)

```
make -j$(nproc) or cmake --build . (will be slow)
sudo make install
sudo ldconfig
```

### DLIB lib installation:
git clone https://github.com/davisking/dlib.git
cd dlib && mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..

####Compile and build
cmake --build .
sudo make install
sudo ldconfig