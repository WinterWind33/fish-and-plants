# RPI Greenhouse Controller Changelog

## v1.1.0

### New features and improvements

- (**New**) New `project` command is here! With this command you can create, save and load a project that contains the configuration of the controller.
- Added `nlohmann/json` third-party library as project dependency and as the main JSON engine;
- Overall code stability and UB fixing;

### Bug fixes

- (**crash**) Fixed crash when the user entered an invalid option;

### Tests

- Updated `Catch2` version to `v3.4.0`;
- Extensively added unit and integration tests along the TDD methodology;

### Dev Ops

- Removed `qemu` build and test job after PR merge;
- Updated `clang-tidy` checks to remove some warnings as errors;
- Updated the mainstream `build-and-tests` job to build with `-j8` command during cmake build.
