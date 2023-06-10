# Fish And Plants

`Fish and Plants` is project that aims to automate an aeroponic greenhouse. The project touches every aspect, from the software
that will automate it to the hardware that is needed to grow the plants inside the greenhouse.

This repository holds the main software that runs inside the Raspberry PI that will control inputs and outputs.

If you want to know more about the project, you can read a summary [here](docs/project/summary.md).

## Features

- Automatic irrigation support for a simple circuit with a valve and a pump;
- The automatic irrigation is implemented as cycles, with an activation step and a deactivation step;
- Configurable activation and deactivations times;
- Emergency abort support;
- Logging support;

## Download

- [Download the stable release](https://github.com/WinterWind33/fish-and-plants/releases) for GNU/Linux, macOS or Windows.

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

## License

MIT License. See [LICENSE](./LICENSE).

## Credits

### Production code

- [spdlog](https://github.com/gabime/spdlog)
- [popl](https://github.com/badaix/popl)
- [libgpiod](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/)

### Testing code

- [Catch2](https://github.com/catchorg/Catch2)
- [GoogleTest](https://github.com/google/googletest)
