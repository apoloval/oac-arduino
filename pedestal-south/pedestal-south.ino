/*
 * Open Airbus Cockpit - Arduino South Pedestal Sketch
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

#define DEVICE_NAME "PedestalSouth"

#include "pins.h"

#include "engine.h"

void setup() {
  OACSP.begin(DEVICE_NAME);
  engine.setup();
}

void loop() {
  OACSP.pollEvent();
  engine.loop();
}
