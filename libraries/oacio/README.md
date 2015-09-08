# Open Airbus Cockpit - IO Library

## Introduction

This library provides a very simple extension to IO capabilities as provided by
Arduino libraries.

## AnalogInput class

This class may be used to perform common tasks while managing analog input, as
we can see in the following example:

```c++
#include <oacio.h>

#define ROTARY_PIN 4

OAC::AnalogInput rotary(ROTARY_PIN);

void loop() {
  if (rotary.isChanged()) {
     long value = rotary.map(0, 20);
     // Do something with the value
  }
}
```

First of all, we declare our analog input variable using `OAC::AnalogInput`
type. In the constructor, we pass the analog pin our potentiometer is connected
to. In the `loop()`` function, we may invoke `isChanged()`` method on the
`AnalogInput` object to know whether it has changed its value. If so, we can map
the current value by invoking the `map()`` method. Its arguments indicate the
lower and upper limit of the range the analog value is mapped. In this example,
a minimum value in the potentiometer would be mapped to 0 while the maximum
value would be mapped to 20. After that, we can use the read value to perform
some action (typically send a command to the simulator).

The `isChanged()`` method accepts an optional argument of long type indicating
the tolerance while detecting changes in the analog input. Some potentiometers
are not very precise, and when they are stopped in some positions the value read
by Arduino is not constant, dancing some units above and below. By default, this
tolerance value is set to 16. If you still experiment that `isChanged()` returns
true without moving the potentiometer, you should increase this value.

`AnalogInput` class also provides a `read()`` method that returns the raw value
as read from the analog pin. Nevertheless, you would likely find `map()`` method
more useful for your purposes.
