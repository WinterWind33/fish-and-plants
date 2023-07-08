# RPI_GC Application reference

RPI_GC (Raspberry PI Greenhouse Controller) is an application with a text-based user interface that runs on a Raspberry PI 3 B+ inside our project's greenhouse
to automate it.

## Features

In this paragraph are listed all the features' summaries that the application has to offer. If you want to deepen each aspect, you can follow their linked reference page.

### Commands

Being the application based on a TUI, you can use the application, and thus control the greenhouse automation, through the implemented commands:

- help : displays a page with a brief description and a list of all implemented commands with a brief summary;
- exit : stops every execution flow, waiting for every resource to be released (soft stop) and exits the application;
- version : displays the name and the current version of the application;
- [abort](./commands/abort.md) : aborts every execution flow and, for now, waiting every resource to be released (soft stop) but doesn't exit the application;
- [auto-watering](./commands/auto-watering.md) : the main command used to control the irrigation automation;
- [status](./commands/status-command.md) : retrieves the data regarding the flows currently running in the application;
- [project](./commands/project-command.md) : manages a project that holds the configurations of your greenhouse;

Every implemented command inside the application, apart from `help` and `exit`, have an help page: to show it, you can simply type the name of the command followed by `-h` or `--help`.

### Logging

The application creates daily logs in the format `daily-log_YYYY-MM-DD.log`. Every day at 00:00 the application creates a new file with the log of the current day.

All the logs are saved inside the folder `logs`.
