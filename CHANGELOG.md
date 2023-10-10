# RPI Greenhouse Controller Changelog

This file contains the changelog of the RPI Greenhouse Controller project. The changelog is divided into sections, each section
contains the changes made in a specific version of the project.

This file follows the [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format and the [Semantic Versioning](https://semver.org/spec/v2.0.0.html) specification.

## [Unreleased]

### Added

- (**New**) Added support for multiple hardware activation states. Now the user can choose between `active-high` and `active-low` activation states for the hardware. To do so he
    must specify the activation state in the project's JSON file. The default activation state is `active-low`. Old projects will be automatically updated to the new activation state considering
    the old activation state as `active-low`;
- (**New**) Added new optional argument to the `auto-watering --start` option that takes the name of the flow to be executed. This is the first step to enable multiple flows execution at the same time.
    Hope to implement this feature in the next release. The name is serialized in the project's JSON file.
- Added new workflows module that will replace the automatic watering system and will be able to perform multiple flows at the same time and new tasks as well;

### Fixed

- (**Bug**) Fixed bug that caused the project data to be lost when the application tried to update the project data to a new version;
- Static analysis not working with the `gsl` library;

## [1.1.0]

### New features and improvements

- (**New**) Added new `status` command to show the current status of the controller. This will print every system flow in a
 formatted output;
- (**New**) Added new `project` command! With this command you can create, save and load a project that contains the configuration of the controller.
    This is useful if you want to save the current configuration of the controller and load it later. When the application starts, it will load the last saved project while
    when the application is closing it will save the current project and will update the application config so that the next time the application starts it will load the last saved project;
- (**New**) Every project is serialized into JSON files. When the project is deserialized and loaded again into the application, the application will check if the project is valid or not.
    If the project is not valid, the application will try to update data to valid values. This is useful if you want to manually edit the project files and you want to be sure that the project is valid;
- Added `nlohmann/json` third-party library as project dependency and as the main JSON engine;
- Added `PlatformFolders` third-party library as project dependency. This library is used to get the preferred OS application data path;
- Applied new `clang-format` configuration to all files;
- Updated log messages;
- Greatly improved error handling to avoid UB and crashes and added errors feedback to the user;
- Overall code stability and UB fixing;

### Bug fixes

- (**Crash**) Fixed crash when the user entered an invalid option;

### (Dev) Tests

- Updated `Catch2` version to `v3.4.0`;
- Extensively added unit and integration tests along the TDD methodology;

### (Dev) Dev Ops

- Updated Linux toolchains: now for ARM64 we target gcc-12 and g++-12 while for x86_64 we target gcc-13 and g++-13;
- Updated `clang-tidy` checks to remove some warnings as errors;
- Updated the mainstream `build-and-tests` job to build with `-j8` command during cmake build.
- Updated `pre-commit` hook to run `clang-format` on every staged file;
- Removed `qemu` build and test job after PR merge;
