Build opencv from sources:
- Clone
  git clone https://github.com/opencv/opencv.git
  cd opencv
  git checkout 4.10.0  # Or the version you need
  git clone https://github.com/opencv/opencv_contrib.git

- Build with cmake

cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib/modules -D WITH_CUDA=ON -D WITH_CUDNN=ON -D OPENCV_DNN_CUDA=ON ../

- Compile and build 
  make -j$(nproc)
  sudo make install
  sudo ldconfig
