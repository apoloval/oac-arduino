# Open Airbus Cockpit - Button Library

## Introduction

This library provides a set of classes to handle buttons, switches and other
forms of passive elements to open and close the current of a circuit. They wrap
the state of the button so you can think only on what to do when the button is
pressed and what to do when it is released. It also implements a software-based
mechanism to avoid bounces.

You can use the library by including a `#include <oacbtn.h>` directive in your
sketch.

## Using OAC::Button class

The `Button` class implements the logic to manage a single button or switch. It
maintains the last known state of the button and implements a bounce detection
algorithm so you only have to worry on what to do when its pressed, released or
toggled. Let’s see it with an example:

```c++
#include <oacbtn.h>

#define BTN_PIN 4

OAC::Button<LOW> button(BTN_PIN);

void onButtonPressed() {
  // Put your on-button pressed code here
}

void setup() {
  button.setOnPressed(onButtonPressed);
}

void loop() {
  button.check();
}
```

In this code, the button is declared as `OAC::Button<LOW>`` type. This
declaration means a button that is considered to be pressed on low voltage. In
other words, it is configured in a [pull down circuit][1]. We must pass a
integer value to the button object constructor (BTN_PIN), which indicates the
Arduino pin the button is connected to. In the `setup()`` function, we only have
to indicate what is the function handler that would be executed when the button
is pressed. This function must have void as return type and must have no
arguments. In the `loop()`` function, we invoke check() method on the button
object to read the digital input and test its state.

This is the typical use of Button to manage buttons. If we are interested, we
can set a function handler also for the on-released event, as in:

```c++
void onButtonReleased() {
  // Put your on-button released code here
}

void setup() {
  button.setOnReleased(onButtonReleased);
}
```

This time, the `onButtonReleased()`` function will be invoked when the button is
released. We can also have an unique function to detect both events, button
pressed and released, as in:

```c++
void onButtonToggled(int state) {
  if (state == ON) {
    // Put your on-button pressed code here
  } else {
    // Put your on-button released code here
  }
}

void setup() {
  button.setOnToggled(onButtonToggled);
}
```

## Using OAC::RotarySwitch class

The `OAC::RotarySwitch` class can be used to manage rotary switches. It works in
a similar way to `Button` class:

```c++
#include <oacbtn.h>

#define ROTARY_PIN_BEGIN 4

OAC::RotarySwitch<3, HIGH> rotary(ROTARY_PIN_BEGIN);

void onModeSelected(int mode) {
  switch (mode) {
    case 0:
      // Put your code when pos 0 is selected
      break;
    case 1:
      // Put your code when pos 1 is selected
      break;
    case 2:
      // Put your code when pos 2 is selected
      break;
  }
}

void setup() {
  rotary.setOnSelect(onModeSelected);
}

void loop() {
  rotary.check();
}
```

Again, we are dealing with a generic class. This time it takes two parameters:
the first one is the number of positions of our rotary switch (3). The second
one is the logic value that is considered as active (HIGH). The constructor
requires one argument, which indicates the first pin of the rotary. Each rotary
position must be connected sequentially from that pin. In this example, we are
setting pin 4 as the begin of rotary pins. That means rotary position 0 would be
connected to pin 4, position 1 to pin 5 and position 2 to pin 6.

We can set the function handler in the same manner as we did for `Button` class.
This time, it requires a function that returns void and requires one parameter
of int type. This parameter would indicate the mode selected in the rotary. This
function handler is invoked each time a new position of the rotary is selected.

[1]: http://en.wikipedia.org/wiki/Pull-up_resistor
