# Open Airbus Cockpit - Arduino Libraries & Sketches

## Introduction

This repository contains Arduino code implementing the logic of Open Airbus
Cockpit panels. 

It is separated into Arduino libraries and sketches. 

The following libraries are used by the sketches, but they can be used 
separately.

* `oacbtn.h`. This library provides a set of classes to handle buttons, 
switches and other forms of passive elements to open and close the current of 
a circuit.

* `oacio.h`. This library provides a very simple extension to IO capabilities 
as provided by Arduino libraries.

* `oacsp.h`. This library provides a wrapper to send and receive messages 
according to OAC Serial Protocol. It can be used to communicate the Arduino 
board with the flight simulator through OAC Command Gateway.

* `oacexp.h`. This library provides classes and functions to manage Open 
Airbus Cockpit expansion cards. These cards enable the access to common ICs 
as digital shift registers, keyboard controllers, LED drivers, etc.

* `oackb.h`. This library provides the means to manage a Open Airbus Cockpit 
Keypad Expansion Card, which is able to control a matrix of keys saving many 
digital pins of your Arduino board.

Along this libraries, the following sketches are provided.

* `pedestal`. This sketch provides the code to manage the A320 pedestal
using an Arduino Mega board. 

## Installation

Just copy each directory in this folder to your Arduino personal folder 
(e.g., `%HOMEPATH\My Documents\Arduino\libraries`). 

Further information in http://arduino.cc/en/Reference/Libraries
