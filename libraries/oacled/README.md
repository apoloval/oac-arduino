# Open Airbus Cockpit - LED Driver Library

## Introduction

This library provides classes to easily manage MAX7219 LED drivers.

## Reference

### Pin configuration

The class `Max7219` represents a chain of 7219 chips. You can configure
the pins used to communicate your Arduino board with the chain as follow:

```c++
#define DISPLAYS_DIN_PIN 10
#define DISPLAYS_LOAD_PIN 11
#define DISPLAYS_CLOCK_PIN 12

OAC::Max7219 displays;

void setup() {
    displays.setPins(
        DISPLAYS_DIN_PIN,
        DISPLAYS_LOAD_PIN,
        DISPLAYS_CLOCK_PIN);
}
```

### Selecting chips

By design, this library provides a different object to represent each chip in
the chain. You can select each chip using the `get()` method:

```c++
void setup() {
    // ...
    displays.get(0).start();
    displays.get(1).shutdown();
}
```

In this example, the first chip in the chain is selected with `get(0)` and it
is requested to start (turn on). Also the second chip in the chain is selected
and requested to shutdown (turn off).

### Setting decode mode

7219 accepts different decoding modes for each digit. You can select the mode
off all digits in a chip by invoking the `setAllDecodeMode()` method.

```c++
void setup() {
    // ...
    displays.get(0).setAllDecodeMode(MAX7219_CODE_B);
    displays.get(1).setAllDecodeMode(MAX7219_NO_DECODE);
}
```

You can also select the decode mode for independent displays using:

```c++
void setup() {
    // ...
    displays.get(0).setDecodeMode(0, MAX7219_CODE_B);    // display 0 in code-b
    displays.get(0).setDecodeMode(1, MAX7219_NO_DECODE); // display 1 in no decode
}
```

### Setting digits

This library provides convenience methods to set digits from integer or float
numbers. You can use `writeInt()` method to print an integer value in the
displays managed by a chip.

```c++
void loop() {
    displays.get(0).writeInt(1234);
}
```

You can also specify the range of displays the number would be printed to:

```c++
void loop() {
    // Write 4 digits starting from digit 2
    displays.get(0).writeInt(1234, 2, 4);
}
```

Also float numbers are supported with `writeFloat()`:

```c++
void loop() {
    // Write Pi with a precision of 4 decimal digits
    displays.get(0).writeFloat(3.1415, 4);
}
```

It also accepts a range of displays.

```c++
void loop() {
    // Write Pi with a precision of 4 decimal digits using 5 digits starting
    // from digit 2
    displays.get(0).writeFloat(3.1415, 4, 2, 5);
}
```

If you want to manage the integer and the decimal parts in different values,
you can use `writeDec()` method to display them as decimal values.

```c++
void loop() {
    // Write Pi with a precision of 4 integer digits and 4 decimal digits
    // starting from digit 0
    displays.get(0).writeDec(3, 1415);
    // Write Pi with a precision of 1 integer digit and 4 decimal digits
    // starting from digit 2
    displays.get(1).writeDec(3, 1415, 2, 1, 4);
}
```

Finally, if all this doesn't cover your needs you can specify the value of
each digit independently:

```c++
void loop() {
    // Display "3.1415"
    displays.get(0).setDigit(0, 3, true); // true here means dot point
    displays.get(1).setDigit(1, 1);
    displays.get(1).setDigit(2, 4);
    displays.get(1).setDigit(3, 1);
    displays.get(1).setDigit(4, 5);
}
```

### Other configuration parameters

You can select the intensity of the displays using `setIntensity()` method.

```c++
void setup() {
    // ...
    displays.get(0).setIntensity(0.3); // 30% of intensity
}
```

You can also set a scan limit, which makes the chip to ignore the digits
greater than such limit.

```c++
void setup() {
    // ...
    displays.get(0).setScanLimit(4); // Only display displays 0 to 4
}
```

Finally, as shown above, you can turn on and off all displays managed by a chip
by using `start()` and `shutdown()`.

```c++
void setup() {
    // ...
    displays.get(0).start();
    displays.get(1).shutdown();
}
```
