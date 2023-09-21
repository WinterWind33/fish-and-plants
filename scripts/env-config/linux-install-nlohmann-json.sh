#!/bin/bash

# Download the latest version of the nlohmann json library
wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp

# Copy the header file to the system-wide include directory
sudo mkdir /usr/local/include/nlohmann
sudo cp json.hpp /usr/local/include/nlohmann/json.hpp

# Clean up
rm json.hpp
