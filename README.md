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

*Please, note that the application runs correctly only on arm64 Linux architectures. For other architectures and all macOS and Windows operating systems, the
hardware interface is simulated.*

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

### Developers

If you want to contribute to the prject, before you commit anything you need to configure [pre-commits](https://pre-commit.com/): in the main repo folder, type `pre-commit install`.

## Application reference

[Here](./docs/reference/rpi_gc.md) you can find the main application reference.

## License

MIT License. See [LICENSE](./LICENSE).

## Credits

These are the third party libraries used for the application.

### Production code

- [spdlog](https://github.com/gabime/spdlog)
- [popl](https://github.com/badaix/popl)
- [libgpiod](https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/)
- [semver](https://github.com/Neargye/semver)
- [nlohmann json](https://github.com/nlohmann/json)
- [PlatformFolders](https://github.com/sago007/PlatformFolders)

### Testing code

- [Catch2](https://github.com/catchorg/Catch2)
- [GoogleTest](https://github.com/google/googletest)
