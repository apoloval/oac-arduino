/*
 * Open Airbus Cockpit - Arduino North Pedestal Sketch
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <oacbtn.h>
#include <oacenc.h>
#include <oacexp.h>
#include <oacio.h>
#include <oackb.h>
#include <oacled.h>
#include <oacsp.h>
#include <oacshift.h>

#define DEVICE_NAME "PedestalNorth"

#include "pins.h"

#include "ecam.h"
#include "mcdu.h"
#include "radio.h"

void setup() {
  OACSP.begin(DEVICE_NAME);
  ecam.setup();
  mcdu.setup();
  rmp1.setup();
}

void loop() {
  OACSP.pollEvent();
  ecam.loop();
  mcdu.loop();
  rmp1.loop();
}
