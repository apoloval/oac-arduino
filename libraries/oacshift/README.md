# Open Airbus Cockpit - Shift Library

## Introduction

This library provides classes to manage shift-in and shift-out circuits
based on 74HC595N & CD4021BE ICs. 

## Reference

### Shift4021

Instances of the class `Shift4021` represents an array of CD4021BE's. They can
be initialized in the `setup()` function to designate the pin layout. In the
following example, we use a 4021 IC to manage 8 switches connected to the 
parallel input of the 4021. 

```c++

#define SWITCHES_CLOCK_PIN 4
#define SWITCHES_LATCH_PIN 5
#define SWITCHES_DATA_PIN 6

OAC::Shift4021 switches;

void setup() {
	switches.setPins(
		SWITCHES_CLOCK_PIN, 
		SWITCHES_LATCH_PIN, 
		SWITCHES_DATA_PIN);
}
```

After that, in `loop()` function we may shift-in the input data as follows.

```c++
void loop() {
	// Load the state of parallel lines into the 4021
	switches.parallelIn();

	// Shift in the input data
	byte data = switches.shiftByteIn();
}
```

If you have a chain of several 4021's, you can shift more than one byte. Let's
say we have a chain of two 4021's. 

```c++
void loop() {
	// Load the state of parallel lines into each 4021
	switches.parallelIn();

	// Shift in the input data of both 4021's: 2 bytes, 1 word
	word data = switches.shiftWordIn();
}
```

### Shift595

Instances of `Shift595` class represents an array of 74HC595N ICs. As with 
4021, we have to configure the pin layout in the `setup()` function. In the
following example, we use one 595 manage the output for 8 LEDs.

```c++

#define LEDS_CLOCK_PIN 4
#define LEDS_LATCH_PIN 5
#define LEDS_DATA_PIN 6

OAC::Shift595 leds;

void setup() {
	leds.setPins(
		LEDS_CLOCK_PIN, 
		LEDS_LATCH_PIN, 
		LEDS_DATA_PIN);
}
```

Now we can use the object to shift-out data as follows.

```c++
byte ledState = 0;

void loop() {
	// We obtain the new state of the LEDs from a custom source
	byte newLedState = getNewLedStates();

	if (ledState != newLedState) {
		ledState = newLedState;

		// Shift the byte out
		leds.shiftByteOut(ledsState);
	}
}
```

Now let's say we have 16 LEDs instead of 8, and a chain of two 595's. We can
use a word to maintain the state of the LEDs and use `shiftWordOut()` method
instead.

```c++
word ledState = 0;

void loop() {
	// We obtain the new state of the LEDs from a custom source
	word newLedState = getNewLedStates();

	if (ledState != newLedState) {
		ledState = newLedState;

		// Shift two bytes (one word) out
		leds.shiftWordOut(ledsState);
	}
}
```
