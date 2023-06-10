# Fish And Plants

`Fish and Plants` is project that aims to automate an aeroponic greenhouse. The project touches every aspect of this objective: from the software
that will automate it to the hardware that is needed to grow the plants inside the greenhouse.

This repository holds the main software that runs inside the Raspberry PI that will control inputs and outputs.

If you want to know more about the project, you can read a summary [here](docs/project/summary.md).

## Features

## Download

- [Download the stable release](https://github.com/WinterWind33/fish-and-plants/releases) for GNU/Linux, macOS or Windows

## Setup the Repo

If you want to clone the repository to your local machine and play with the code or build the executable yourself, you can follow these steps:

```bash

git clone https://github.com/WinterWind33/fish-and-plants.git
cd fish-and-plants && mkdir build
cd build
cmake .. -Wno-dev
cmake --build . --config Release

```

The repo uses CMake as the main build system, and it requires at least `CMake 3.19`.
