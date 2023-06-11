# `auto-watering` command

The command is responsible to activate, deactivate and control the flow of the automatic irrigation of the greenhouse.

**Note:** in the following documentation, **AWS** will be used as a short name for *automatic watering system*. The automatic watering system is the main entity that will handle the plants watering flow inside the application. When the AWS is started, water will be given to the plants and when it's deactivated no water will reach them.

## Command description

Please, note that the command uses an **active-low** hardware configuration on all the flow. This was introduced to fix the support for relays. This will properly change in future versions of the application to make it a configurable choice.

### Name

    auto-watering - manages the automatic irrigation of the greenhouse

### Synopsis

    auto-watering [OPTION]...

### Options

All the options listed below can be processed even if the irrigation flow is active. This means that if you want to change some parameter of the AWS, you can do that even if it's running.

Based on the timing, the changes will take effect after the entire flow cycle or after the current step completion (see below the irrigation flow for the steps).

```ps
    -h, --help                          Displays the help page.
    -S, --start                         Starts the automatic watering system in Daily-Cycle mode.
    -s, --stop                          Stops the automatic watering system waiting for resources to be released.
    -A, --activation-time arg (=6000)   Sets the automatic watering system activation time (expressed in ms).
    -D, --deactivation-time arg (=600000)
                                        Sets the automatic watering system deactivation time (expressed in ms).
    -P, --pumpvalve-deactsep-time arg (=600)
                                        (CAREFUL) Sets the separation time (espressed in ms) between the pump deactivation and the valve one. WARNING: PLAYING WITH THIS CONFIGURATION MAY LEAD TO HARDWARE FAILURE. USE WITH CAUTION.
    -V, --valve-pin-id arg (=26)        Sets the ID of the pin which will receive the output jumpers that will power up the water valve.
    -U, --pump-pin-id arg (=23)         Sets the ID of the pin which will receive the output jumpers that will power up the water pump.
    -o, --disable-pump                  Disables the pump in the automatic watering system cycles.
    -n, --disable-valve                 Disables the valve in the automatic watering system cycles.
    -E, --enable-pump                   Enables the water pump in the automatic watering system cycles.
    -G, --enable-valve                  Enables the water valve in the automatic watering system cycles.

```

**NOTE: Use the -o, -n, -E and -G options with care. They are experimental and should be used only BEFORE the automatic watering system is started or when it's disabled. Using them during the execution of the flow may result in undefined behaviors and hardware can be damaged in the process.**

#### `-S` or `--start` option

This will *activate* the automatic watering system flow and it will begin with the **activation step**. If another instance of the AWS was already running, **the command won't be executed**.

#### `-s` or `--stop` option

This will *deactivate* the automatic watering system flow, after the step that is currently executed in the flow. If the AWS is active, it will wait the step to complete and then it will disable all the output hardware, resetting the state. If the AWS isn't running, the command won't do anything.

#### `-A` or `--activation-time` option

This will set the activation time of the irrigation. This means that the irrigation system will bring water to the plants for the specified time.

The time parameter is an integer measured in **milliseconds**. The default activation time is 6 seconds.

**Note**: the name `Daily-cycle` simply means that the irrigation flow is performed using a set of steps that is repeated accross the day, even multiple times. This is the current implementation of the irrigation flow.

##### Examples

Using the long name:

```ps
    auto-watering --activation-time 7000 # This will set the activation time to 7 seconds.
```

Using the short name:

```ps
    auto-watering -A 63000 # This will set the activation time to 1 minute and 3 seconds.
```

#### `-D` or `--deactivation-time` option

This will set the deactivation time of the irrigation. After the activation time step is elapsed, the AWS will be deactivated and all the hardware devices turned off. These remain turned off for the specified time.

The time parameter is an integer measured in **milliseconds**. The default activation time is 10 minutes (600 seconds = 600'000 milliseconds).

##### Examples

Using the long name:

```ps
    auto-watering --deactivation-time 50000 # This will set the deactivation time to 50 seconds.
```

Using the short name:

```ps
    auto-watering -D 1200000 # This will set the activation time to 20 minutes.
```

#### `-P` or `--pumpvalve-deactsep-time` option

**WARNING: PLAYING WITH THIS CONFIGURATION MAY LEAD TO HARDWARE FAILURE. USE WITH CAUTION.**

This will set the time that the *deactivation step* will wait between the deactivation of the valve and the pump. This may be required to restore the water pressure inside the pipes so that the pump doesn't get damaged during the deactivation step.

You have to be careful using this option because you may damage hardware components. Do your homework beforehand and compute the correct deactivation time. The guaranteed tolerance for this time is `1e-4` seconds (`0.1` milliseconds).

The time parameter is an integer measured in **milliseconds**. The default value is 600 milliseconds.

## The automatic watering flow

The flow of the automatic watering system (**AWS**) can be described with the following diagram:

![Alt text](aws-flow.png)

As you can see, during the irrigation deactivation there is a wait time that is needed to restore the pressure of the water flow so that the pump doesn't get damaged during the process.

The flow can be interrupted after each step or during a `wait`: in this case, it will stop the wait and it will begin the teardown of the flow, putting the AWS state to "not running".
