### Build opencv from sources:
- Clone
  git clone https://github.com/opencv/opencv.git
  cd opencv
  git checkout 4.10.0  # Or the version you need
  git clone https://github.com/opencv/opencv_contrib.git

#### Build with cmake

mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -D WITH_CUDA=ON -D WITH_CUDNN=ON -D OPENCV_DNN_CUDA=ON ../

#### Compile and build
make -j$(nproc) or cmake --build .
sudo make install
sudo ldconfig


### Ubuntu set up:

- sudo apt-get install -y build-essential cmake libopenblas-dev liblapack-dev libx11-dev libgtk-3-dev

### DLIB lib installation:

git clone https://github.com/davisking/dlib.git
cd dlib && mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ../

####Compile and build
cmake --build .
sudo make install
sudo ldconfig
