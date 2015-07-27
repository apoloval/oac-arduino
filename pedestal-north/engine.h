/*
 * Open Airbus Cockpit - Arduino North Pedestal Sketch
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

struct Engine {
  OAC::Button<LOW> masterOneSw;
  OAC::Button<LOW> masterTwoSw;
  OAC::RotarySwitch<3, LOW> modeSel;
  
  Engine() :
    masterOneSw(ENGINE_MASTER1_SW_PIN),
    masterTwoSw(ENGINE_MASTER2_SW_PIN),
    modeSel(ENGINE_MODE_SEL_PIN) {}
    
  static void onMasterOneToggle(int isOn) {
    OACSP.writeLVar("AB_PDS_Eng1Master", isOn);
  }
  
  static void onMasterTwoToggle(int isOn) {
    OACSP.writeLVar("AB_PDS_Eng2Master", isOn);
  }
  
  static void onModeSelected(int pos) {
    OACSP.writeLVar("AB_PDS_ignition", pos);
  }
  
  void setup() {
    masterOneSw.setOnToggled(onMasterOneToggle);
    masterTwoSw.setOnToggled(onMasterTwoToggle);
    modeSel.setOnSelect(onModeSelected);
  }
  
  void loop() {
    masterOneSw.check();
    masterTwoSw.check();
    modeSel.check();
  }
} engine;

