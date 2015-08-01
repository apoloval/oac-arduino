/*
 * Open Airbus Cockpit - Arduino South Pedestal Sketch
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define ENGINE_MASTER1_LVAR      "AB_PDS_Eng1Master"
#define ENGINE_MASTER2_LVAR      "AB_PDS_Eng2Master"
#define ENGINE_MODE_LVAR         "AB_PDS_ignition"
#define ENGINE_FIRE1_LVAR        "AB_FIRE_ENG1"
#define ENGINE_FIRE2_LVAR        "AB_FIRE_ENG2"

#define ENGINE_MASTER1_MASK(b)   ((b & 0x01) >> 0)
#define ENGINE_MASTER2_MASK(b)   ((b & 0x02) >> 1)
#define ENGINE_MODE_MASK(b)      (((b & 0x0c) >> 2) - 1)

#define ENGINE_MASK_NEQ(l, r, m) (m(l) != m(r)) 

#define ENGINE_FIRE1_MASK        0x01
#define ENGINE_FIRE2_MASK        0x04

struct Engine {
   OAC::Shift4021 in;
   OAC::Shift595 out;
   byte prevInput;
   byte output;

   void setup() {
      setupPins();
      setupInitialState();
      setupObservedLVars();
   }
  
   void loop() {
      inputLoop();
      outputLoop();
   }

private:

   void setupPins() {
      pinMode(ENGINE_DAV_PIN, INPUT);
      in.setPins(
         ENGINE_CLOCK_PIN, 
         ENGINE_IN_LATCH_PIN, 
         ENGINE_IN_DATA_PIN);
      out.setPins(
         ENGINE_CLOCK_PIN,
         ENGINE_OUT_LATCH_PIN,
         ENGINE_OUT_DATA_PIN);
   }

   void setupInitialState() {
      prevInput = 0;
      processInput();

      output = 0;
      processOutput();
   }

   void setupObservedLVars() {
      OACSP.observeLVar(ENGINE_FIRE1_LVAR);
      OACSP.observeLVar(ENGINE_FIRE2_LVAR);
   }

   void inputLoop() {
      int dav = digitalRead(ENGINE_DAV_PIN);
      if (dav) {
         // Have to wait some millis to give time for data to be ready
         delay(20);
         processInput();
      }
   }

   void outputLoop() {
      OAC::LVarUpdateEvent* ev;
      if (ev = OACSP.lvarUpdateEvent(ENGINE_FIRE1_LVAR)) {
         if (ev->value == 0) output &= ~ENGINE_FIRE1_MASK;
         else output |= ENGINE_FIRE1_MASK;
         processOutput();
      }
      if (ev = OACSP.lvarUpdateEvent(ENGINE_FIRE2_LVAR)) {
         if (ev->value == 0) output &= ~ENGINE_FIRE2_MASK;
         else output |= ENGINE_FIRE2_MASK;
         processOutput();
      }
   }

   void processInput() {
      in.parallelIn();
      byte data = in.shiftByteIn();

      setMasterOne(data);
      setMasterTwo(data);
      setEngineMode(data);

      prevInput = data;
   }

   void processOutput() {
      out.shiftByteOut(output);
   }

   void setMasterOne(byte data) {
      if (!prevInput || ENGINE_MASK_NEQ(data, prevInput, ENGINE_MASTER1_MASK)) {
         OACSP.writeLVar(ENGINE_MASTER1_LVAR, ENGINE_MASTER1_MASK(data));
      }
   }

   void setMasterTwo(byte data) {
      if (!prevInput || ENGINE_MASK_NEQ(data, prevInput, ENGINE_MASTER2_MASK)) {
         OACSP.writeLVar(ENGINE_MASTER2_LVAR, ENGINE_MASTER2_MASK(data));
      }
   }

   void setEngineMode(byte data) {
      if (!prevInput || ENGINE_MASK_NEQ(data, prevInput, ENGINE_MODE_MASK)) {
         OACSP.writeLVar(ENGINE_MODE_LVAR, ENGINE_MODE_MASK(data));
      }
   }

} engine;
