# `auto-watering` command

The command is responsible to activate, deactivate and control the flow of the automatic irrigation of the greenhouse.

## Synopsis

## The automatic watering flow

The flow of the automatic watering system (**AWS**) can be described

```plantuml

start

repeat
    partition "**Step** AWS Activation" {
        :Activate valve;
        :Activate pump;
    }
    :**Wait** ACTIVATION_TIME;

    partition "**Step** AWS Deactivation" {
        :Deactivate valve;
        :**Wait** DEACT_SEP_TIME;
        :Deactivate pump;
    }
    :**Wait** DEACTIVATION_TIME;

repeat while (Stop requested?) is no

stop

```
