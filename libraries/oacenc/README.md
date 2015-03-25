# Open Airbus Cockpit - Rotary Encoder Library

## Introduction

This library provides classes to easily manage rotary encoders. 

## Reference

### Simple usage of `RotaryEncoder`

The `RotaryEncoder` class represents a rotary encoder device. It should be
configured from the `setup()` function to set its pin layout.

```c++

#define ROTARY_PIN_A 4
#define ROTARY_PIN_B 5

OAC::RotaryEncoder rot;

void setup() {
	rot.setPins(
		ROTARY_PIN_A, 
		ROTARY_PIN_B);
}
```

After that, it could be used in the `loop()` function to read the increment
detected on each iteration of the loop. In the following example we asume 
`Serial` device was configured in `setup()`. 

```c++
void loop() {
	int inc = rot.read();
	if (inc > 0) {
		Serial.println("Rotary is incremented");
	} else if (inc < 0) {
		Serial.println("Rotary is decremented");
	} else {
		// `inc` is 0, no increment is detected
	}
}
```


The library also provides a mechanism to bind the rotary encoder to a range 
of numbers. That covers the typical use case of a rotary encoder: select
a number from a given range. The following example shows how the rotary
can be used to select a number in the 1..100 range. 

```c++
RangedNumber number(50, 1, 100); // start at 50, min is 1 and max is 100

void loop() {
	long inc = rot.read(number);
	if (inc != 0) {
		Serial.print("The number has changed to ");
		Serial.println(number.value());
	}
}

```

If the range is too large respect the speed of the rotary encoder, you 
can provide a multiplier as an argument to `read()` function to be applied
to the number managed by the `RangedNumber` instance. Let's say you want
the rotary to manage the value in the range 1..1000, but in steps of 10.

```c++
RangedNumber number(50, 1, 100); // start at 50, min is 1 and max is 100

void loop() {
	long inc = rot.read(number, 10); // use a 10x multiplier
	if (inc != 0) {
		Serial.print("The number has changed to ");
		Serial.println(number.value());
	}
}

```
