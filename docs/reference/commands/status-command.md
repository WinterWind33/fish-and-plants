# `status` command

This command is responsible to retrieve the status of all the active systems that are currently running in the application.
It presents the data in a structured way: every system has its own section.

## Examples

Here you can see an example of a running AWS flow:

```text
<Diagnostic section start>

 [Diagnostic]:  Automatic watering system (AWS)
 [AWS Mode]:    Cycled
 [Status]:      Irrigating
 [Thread ID]:   6328
 {AWS Flow}
         [Completed cycles]: 2
         [Water valve status]: Enabled
         [Water pump status]: Enabled
         [Water valve output PIN]: 26
         [Water pump output PIN]: 23
         [Activation time]:     5000ms
         [Deactivation time]:   10000ms
         [Pump-valve sep time]: 600ms

<section end>
```

As you can see, in this example the AWS is operating in a cycled mode and is dispensing the water. Along with general data about the AWS, there is also data regarding the flow, completed cycles, timings and the hardware devices that are currently enabled and to which PIN they're connected.
