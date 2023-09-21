#!/bin/bash

# Download the latest version of the spdlog library
wget https://github.com/gabime/spdlog/archive/refs/tags/v1.12.0.tar.gz

# Extract the downloaded archive to a directory of your choice
tar -xvf v1.12.0.tar.gz

# Copy the header files to the system-wide include directory
sudo cp -r spdlog-1.12.0/include/spdlog /usr/local/include

# Clean up
rm -rf spdlog-1.12.0
rm v1.12.0.tar.gz
