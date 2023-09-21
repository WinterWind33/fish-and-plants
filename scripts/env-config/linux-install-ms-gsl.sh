#!/bin/bash

git clone --depth 1 --branch v4.0.0 https://github.com/microsoft/GSL.git
cd GSL
mkdir build
cd build
cmake ..
sudo make
sudo make install
rm -r -f ./GSL
