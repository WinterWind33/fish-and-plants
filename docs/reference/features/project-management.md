# Project Management

The project management is a feature that allows the user to manage multiple projects, each one with its own configurations.

Every project is serialized using a JSON file, and the application can load and save them. To handle the projects, the application uses the [`project`](../commands/project-command.md) command.

## Structure of a project

In this section, we'll see how the project is structured and represented. The project is a JSON file that contains all the configurations of the greenhouse.

### Basic project data

The basic data of a project is a set of these values:

- `version` : this represents the version of the `rpi_gc` application that last loaded the project. The version is automatically upgraded if a newer version of `rpi_gc` is launched with the project that was generated from an older version;
- `creation time` : this represents the time and date of the creation of the project. This field will be useful for statistical analysis;
- `title` : the name of the project;

### Automatic Watering System configuration

The automatic watering system configuration is a set of values that are used to configure the AWS flow. The AWS configuration is a set of these values:

- `mode`: the mode of the AWS flow. The mode can only be `cycled`, i.e. the automatic watering system runs automatically alternating active and inactive periods. The inactive periods are used to let the soil dry;
- `flow`: the flow of the AWS flow. The flow is the structure of every cycles of the AWS. The cycle is repeated at the end of every inactive period. The flow is a set of these values:
  - `activationTime` **[milliseconds]**: the duration of the active period of the cycle. During this period, the pump and the valve will be activated and the water will be pumped into the greenhouse;
  - `deactivationTime` **[milliseconds]**: the interval of time between the end of the active period and the start of the next active period. During this period, the pump and the valve will be deactivated and the soil will dry;
  - `deactivationSepTime` **[milliseconds]**: the interval of time between the deactivation of the valve and the deactivation of the valve to let the water pressure inside the pipes to stabilize to not damage the pump;
  - `isWaterValveEnabled` **[boolean]**: a boolean value that indicates if the water valve will be used during the active period of the cycle;
  - `isWaterPumpEnabled` **[boolean]**: a boolean value that indicates if the water pump will be used during the active period of the cycle.
  - `pumpPinID`: the ID of the board that will be attached to the pump that will be used during the active period of the cycle. This is only used if the `isWaterValveEnabled` is set to `true`;
  - `valvePinID`: the ID of the board that will be attached to the valve that will be used during the active period of the cycle. This is only used if the `isWaterPumpEnabled` is set to `true`;

## Project automatic loading

The application automatically loads the last project that was used. If the application is launched for the first time, it will create a new project with the default configurations.

### Last project data

The last project information is stored inside the `rpi_gc_config.json` file. This file is located inside the `AppData` folder, which is specified by the operating system. Tipically, under `Windows`, the folder resides inside the `%appdata%` folder while under Ubuntu the folder is inside the `/home/<user>/.local/share` folder. The `rpi_gc_config.json` file is a JSON file that contains the path of the last project that was used. Inside this folder, the application will search the application config file that is named `rpi_gc_project.json` under the `/path/to/appdata/FishAndPlants/rpi_gc` folder.

If inside the `rpi_gc_config.json` file there is a path that doesn't exist, the application will remove the entry from it. If the file doesn't exist, the application will create it.

The last project information is automatically saved every time a new project is loaded, created or saved, as specified in the `project` command reference.
