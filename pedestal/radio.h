/*
 * Open Airbus Cockpit - Arduino Pedestal Sketch
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define RADIO_NAV_IND    0x0040
#define RADIO_VOR_IND    0x0020
#define RADIO_ILS_IND    0x0001
#define RADIO_MLS_IND    0x0004
#define RADIO_ADF_IND    0x0008
#define RADIO_BFO_IND    0x0010
#define RADIO_HF1_IND    0x0400
#define RADIO_HF2_IND    0x0800
#define RADIO_AM_IND     0x1000
#define RADIO_VHF1_IND   0x4000
#define RADIO_VHF2_IND   0x2000
#define RADIO_VHF3_IND   0x0100

#define RADIO_KEY_VHF1     0x00
#define RADIO_KEY_VHF2     0x01
#define RADIO_KEY_VHF3     0x02
#define RADIO_KEY_SWAP     0x03
#define RADIO_KEY_HF1      0x04
#define RADIO_KEY_SEL      0x05
#define RADIO_KEY_HF2      0x06
#define RADIO_KEY_AM       0x07
#define RADIO_KEY_NAV      0x08
#define RADIO_KEY_VOR      0x09
#define RADIO_KEY_ILS      0x0a
#define RADIO_KEY_MLS      0x0b
#define RADIO_KEY_ADF      0x0c
#define RADIO_KEY_BFO      0x0d

#define RADIO_DISPLAY_MASTER  0
#define RADIO_DISPLAY_SLAVE   1

#define RADIO_COM1_ACTIVE_OFFSET   0x034e
#define RADIO_COM1_STANDBY_OFFSET  0x311a
#define RADIO_COM2_ACTIVE_OFFSET   0x3118
#define RADIO_COM2_STANDBY_OFFSET  0x311c

struct RadioPanel {
   
   enum HfMode { HF_MODE_SSB, HF_MODE_AM };
   
   /**
    * 7-segment displays of radio panel. 
    * 
    * This struct provides the state and the operations representing 
    * the management of the 7-segment displays of the radio panel and
    * its indicators. It is often passed to the `DisplayData` objects 
    * to operate on the displays (print data, set display modes, etc). 
    */
   struct Displays {
      
      /** An enumeration of the available displays (left & right). */
      enum Display { 
         DISPLAY_RIGHT = RADIO_DISPLAY_MASTER, 
         DISPLAY_LEFT = RADIO_DISPLAY_SLAVE 
      };
      
      void setup() {
         displays.setPins(
            RMP1_DISPLAY_DIN_PIN, 
            RMP1_DISPLAY_LOAD_PIN, 
            RMP1_DISPLAY_CLK_PIN);
       
         // Variable `modes` store the known modes for each display.
         // We set it to default mode set by oacled library. 
         modes[DISPLAY_LEFT] = modes[DISPLAY_RIGHT] = MAX7219_CODE_B;
       
         // Slave display manages the pilot LEDs in digits 6 & 7.
         // These must be in no-decode mode to write independent segments.
         displays.get(RADIO_DISPLAY_SLAVE).setDecodeMode(6, MAX7219_NO_DECODE);
         displays.get(RADIO_DISPLAY_SLAVE).setDecodeMode(7, MAX7219_NO_DECODE);
       
         displays.get(RADIO_DISPLAY_MASTER).setIntensity(0.5f);
         displays.get(RADIO_DISPLAY_SLAVE).setIntensity(0.8f);       
      }
      
      /** Set given display to print numbers instead of text. */
      void setNumeric(Display display) {         
         setDisplayMode(display, MAX7219_CODE_B);
      }
      
      /** Set given display to print text instead of numbers. */
      void setText(Display display) {
         setDisplayMode(display, MAX7219_NO_DECODE);
      }
      
      /** 
       * Print the given number as a frequency in the given display. 
       * 
       * The number is assumed to be a x1000 magnitude. I.e., its last 3
       * digits are interpreted as the decimal part, and the rest is
       * considered the integer part. 
       * 
       * The display should be set to print numbers before. Otherwise none
       * is printed. 
       */
      void printFrequency(const OAC::RangedNumber& num, Display display) {
         if (modes[display] == MAX7219_CODE_B) {
            long int_part = num.value() / 1000L;
            long dec_part = num.value() % 1000L;
            displays.get(display).writeDec(int_part, dec_part, 0, 3, 3); 
         }
      }
      
      /** 
       * Print the text "ACARS" in the given display. 
       * 
       * The display should be set to print text before. Otherwise none
       * is printed. 
       */
      void printAcars(Display display) {
         if (modes[display] == MAX7219_NO_DECODE) {
            byte digits[] = { 0x00, 0x77, 0x4e, 0x77, 0x46, 0x5b };
            for (int i = 0; i < 6; i++) {
               displays.get(display).setDigit(i, digits[i]);
            }
         }
      }
      
      /** Print (or activate) the given indicators. */
      void printIndicators(short ind) {
         displays.get(RADIO_DISPLAY_SLAVE).setDigit(6, ind);
         displays.get(RADIO_DISPLAY_SLAVE).setDigit(7, ind >> 8);
      }
      
      void powerOn() {
         displays.get(RADIO_DISPLAY_MASTER).start();
         displays.get(RADIO_DISPLAY_SLAVE).start();
      }
     
      void powerOff() {
         displays.get(RADIO_DISPLAY_MASTER).shutdown();
         displays.get(RADIO_DISPLAY_SLAVE).shutdown();
      }
      
   private:
   
      OAC::Max7219<2> displays;
      byte modes[2];

      void setDisplayMode(Display display, byte mode) {
         if (modes[display] != mode) {
            // While decode mode is changed, we shutdown the chip 
            // and display blanks to avoid flickering
            displays.get(display).shutdown();
            displays.get(display).setDecodeMode(0, 5, mode);
            blank(display, mode);
            displays.get(display).start();
            
            modes[display] = mode;
         }
      }
      
      /** Display blanks in the given display on given mode. */
      void blank(Display display, byte mode) {
         // In no-decode, blank is 0 in D7-D0; in mode-b, blank is 1's in D3-D0
         byte blankSymbol = (mode == MAX7219_NO_DECODE) ? 0x00 : 0x0f;
         for (int i = 0; i < 6; i++) {
            displays.get(display).setDigit(i, blankSymbol);
         }
      }

   };
  
   struct DisplayData {      
      short indicator;
      virtual void onSelected(Displays& displays) = 0;
      virtual boolean onInnerIncrement(int inc) = 0;
      virtual boolean onOuterIncrement(int inc) = 0;
      virtual void display(Displays& displays) = 0;
      virtual void swap(Displays& displays) = 0;
   };
  
   struct FrequencyPair : DisplayData {
      OAC::RangedNumber active;
      OAC::RangedNumber standby;
      short activeOffset;
      short standbyOffset;
      
      void setup(long value, long min, long max, short ind, short actOffset, short stdbyOffset) {
         active.setup(value, min, max);
         standby.setup(value, min, max);
         indicator = ind;
         activeOffset = actOffset;
         standbyOffset = stdbyOffset;
      }
      
      void setActive(long value, Displays& displays) {
         active.setValue(value);
         displays.printFrequency(active, Displays::DISPLAY_LEFT);
      }
      
      void setStandby(long value, Displays& displays) {
         standby.setValue(value);
         displays.printFrequency(standby, Displays::DISPLAY_RIGHT);
      }
      
      virtual void onSelected(Displays& displays) {
         displays.setNumeric(Displays::DISPLAY_LEFT);
         displays.setNumeric(Displays::DISPLAY_RIGHT);
         display(displays);
      }

      virtual boolean onInnerIncrement(int inc) {
         return standbyIncrement(inc, 25);
      }

      virtual boolean onOuterIncrement(int inc) {
         return standbyIncrement(inc, 1000);
      }

      virtual void display(Displays& displays) {
         displays.printFrequency(active, Displays::DISPLAY_LEFT);
         displays.printFrequency(standby, Displays::DISPLAY_RIGHT);
      }
      
      virtual void swap(Displays& displays) {
         active.swap(standby);
         display(displays);
         OACSP.writeOffset(standbyOffset, freqToBcd(standby.value()));
         OACSP.writeOffset(activeOffset, freqToBcd(active.value()));
      }

   private:
   
      short freqToBcd(long freq) {
         long result = 0;
         result |= ((freq / 10L) % 10) << 0;
         result |= ((freq / 100L) % 10) << 4;
         result |= ((freq / 1000L) % 10) << 8;
         result |= ((freq / 10000L) % 10) << 12;
         return result;
      }
      
      boolean standbyIncrement(int inc, int mult) {
         boolean wasIncremented = standby.inc(inc * mult);
         if (wasIncremented && standbyOffset) {
            OACSP.writeOffset(standbyOffset, freqToBcd(standby.value()));
         }
         return wasIncremented;
      }
   };
   
   struct AcarsFrequency : DisplayData {
      
      enum PortSelect { PORT_SEL_GND, PORT_SEL_OPEN };
   
      OAC::RangedNumber freq;
      PortSelect portSelect;
      
      void setup(long value, long min, long max, short ind) {
         freq.setup(value, min, max);
         indicator = ind;
      }
      
      virtual boolean onInnerIncrement(int inc) {
         return (portSelect == PORT_SEL_GND) ? 
            freq.inc(inc * 25) : false;
      }

      virtual boolean onOuterIncrement(int inc) {
         return (portSelect == PORT_SEL_GND) ? 
            freq.inc(inc * 1000) : false;
      }
      
      virtual void onSelected(Displays& displays) {
         configDisplays(displays);
         display(displays);
      }

      virtual void display(Displays& displays) {
         displays.printFrequency(freq, freqDisplay());
         displays.printAcars(acarsDisplay());
      }
      
      virtual void swap(Displays& displays) {
         switch (portSelect) {
            case PORT_SEL_GND:
               portSelect = PORT_SEL_OPEN;
               break;
            case PORT_SEL_OPEN:
               portSelect = PORT_SEL_GND;
               break;
         }
         configDisplays(displays);
         display(displays);
      }
   
   private:
   
      void configDisplays(Displays& displays) {
         displays.setNumeric(freqDisplay());
         displays.setText(acarsDisplay());
      }
      
      Displays::Display freqDisplay() {
         return (portSelect == PORT_SEL_GND) ? 
            Displays::DISPLAY_RIGHT : Displays::DISPLAY_LEFT;
      }
   
      Displays::Display acarsDisplay() {
         return (portSelect == PORT_SEL_GND) ? 
            Displays::DISPLAY_LEFT : Displays::DISPLAY_RIGHT;
      }
   };
   
   Displays displays;
   OAC::RotaryEncoder innerEncoder;
   OAC::RotaryEncoder outerEncoder;
   OAC::Keyboard kb;
   OAC::Button<> powerBtn;
  
   FrequencyPair vhf1;
   FrequencyPair vhf2;
   AcarsFrequency vhf3;
   FrequencyPair hf1;
   FrequencyPair hf2;
   DisplayData* currentDisplayData;
   HfMode hfMode;
  
   void setCurrentDisplayData(DisplayData& freq) {
      currentDisplayData = &freq;
      currentDisplayData->onSelected(displays);
      displayIndicators();
   }
      
   boolean hfIsSelected() {
      return currentDisplayData == &hf1 || currentDisplayData == &hf2;
   }

   short hfModeIndicator() {
      boolean hfModeIsAm = hfMode == HF_MODE_AM;
      return (hfIsSelected() && hfModeIsAm) ? RADIO_AM_IND : 0;
   }
   
   void toggleHfMode() {
      if (hfIsSelected()) {
         switch (hfMode) {
            case HF_MODE_SSB:
               hfMode = HF_MODE_AM;
               break;
            case HF_MODE_AM:
               hfMode = HF_MODE_SSB;
               break;
         }
         displayIndicators();
      }
   }
   
   void displayIndicators() {
      short ind = currentDisplayData->indicator | hfModeIndicator();
      displays.printIndicators(ind);
   }   
   
   void onKey(short key) {
      if (key >= 0) {
         switch (key) {
            case RADIO_KEY_SWAP:
               currentDisplayData->swap(displays);
               break;                            
            case RADIO_KEY_HF1: 
               setCurrentDisplayData(hf1);
               break;
            case RADIO_KEY_HF2: 
               setCurrentDisplayData(hf2);
               break;
            case RADIO_KEY_VHF1: 
               setCurrentDisplayData(vhf1);
               break;
            case RADIO_KEY_VHF2: 
               setCurrentDisplayData(vhf2);
               break;
            case RADIO_KEY_VHF3: 
               setCurrentDisplayData(vhf3);
               break;
            case RADIO_KEY_AM: 
               toggleHfMode();
               break;
       }
     }
   }
  
   void setupFrequencies() {
      vhf1.setup(122800, 118000, 136975, RADIO_VHF1_IND, 
                 RADIO_COM1_ACTIVE_OFFSET, RADIO_COM1_STANDBY_OFFSET);
      vhf2.setup(122800, 118000, 136975, RADIO_VHF2_IND, 
                 RADIO_COM2_ACTIVE_OFFSET, RADIO_COM2_STANDBY_OFFSET);
      vhf3.setup(122800, 118000, 136975, RADIO_VHF3_IND);
      hf1.setup(10000, 2000, 29999, RADIO_HF1_IND, 0, 0);
      hf2.setup(10000, 2000, 29999, RADIO_HF2_IND, 0, 0);
      
      setCurrentDisplayData(vhf1);
   }     
  
   void setupEncoders() {
      innerEncoder.setPins(RMP1_ENC_IA_PIN, RMP1_ENC_IB_PIN);
      outerEncoder.setPins(RMP1_ENC_OA_PIN, RMP1_ENC_OB_PIN);
   }
  
   void setupPowerButton() {
      powerBtn.setPin(RMP1_POWER_PIN);
   }
  
   void setupKeypad() {
      kb.configBus(
         RMP1_KEYPAD_DATAA_PIN,
         RMP1_KEYPAD_DATAB_PIN,
         RMP1_KEYPAD_DATAC_PIN,
         RMP1_KEYPAD_DATAD_PIN);
      kb.configController(
         OAC::Keypad::CONTROLLER_0, RMP1_KEYPAD_DAV_PIN, -1);
   }
   
   /** Setup the FSUIPC offsets (observe the tuned frequencies in the sim). */
   void setupOffsets() {
      OACSP.observeOffset(RADIO_COM1_ACTIVE_OFFSET, OAC::OFFSET_UINT16);
      OACSP.observeOffset(RADIO_COM1_STANDBY_OFFSET, OAC::OFFSET_UINT16);
      OACSP.observeOffset(RADIO_COM2_ACTIVE_OFFSET, OAC::OFFSET_UINT16);
      OACSP.observeOffset(RADIO_COM2_STANDBY_OFFSET, OAC::OFFSET_UINT16);
   }
  
   void setup() {
      displays.setup();
      setupEncoders();
      setupPowerButton();
      setupKeypad();
      setupFrequencies();
      setupOffsets();
   }
  
   void loopPowerButton() {
      int last = powerBtn.state();
      int current = powerBtn.check();
      if (current != last) {
         if (current == ON) { displays.powerOn(); }
         else { displays.powerOff(); }
      }      
   }
   
   void loopEncoders() {
      boolean innerChanges = currentDisplayData->onInnerIncrement(innerEncoder.read());
      boolean outerChanges = currentDisplayData->onOuterIncrement(outerEncoder.read());
      if (innerChanges || outerChanges) {
         currentDisplayData->display(displays);
      }
   }
   
   void loopOffsets() {
      OAC::Event* event = OACSP.event();
      if (event && event->type == OAC::OFFSET_UPDATE) {
         const OAC::OffsetUpdateEvent& offsetUpdated = event->offset;
         switch (offsetUpdated.address) {
            case RADIO_COM1_ACTIVE_OFFSET:
               vhf1.setActive(freqFromBcd(offsetUpdated.value), displays);
               break;
            case RADIO_COM1_STANDBY_OFFSET:
               vhf1.setStandby(freqFromBcd(offsetUpdated.value), displays);
               break;
            case RADIO_COM2_ACTIVE_OFFSET:
               vhf2.setActive(freqFromBcd(offsetUpdated.value), displays);
               break;
            case RADIO_COM2_STANDBY_OFFSET:
               vhf2.setStandby(freqFromBcd(offsetUpdated.value), displays);
               break;
         }
      }
   }
  
   void loop() {
      loopPowerButton();
      if (powerBtn.state() == ON) {
         loopEncoders();
         onKey(kb.readKeyType());
         loopOffsets();
      }
   }
   
   long freqFromBcd(short bcd) {
      long result = 100000L;
      result += ((bcd & 0x000f) >> 0) * 10L;
      result += ((bcd & 0x00f0) >> 4) * 100L;
      result += ((bcd & 0x0f00) >> 8) * 1000L;
      result += ((bcd & 0xf000) >> 12) * 10000L;
      if (result % 100 == 70 || result % 100 == 20) { result += 5; }
      return result;
   }
   
   short freqToBcd(long freq) {
      long result = 0;
      result |= ((freq % 10L) / 10) << 0;
      result |= ((freq % 100L) / 10) << 4;
      result |= ((freq % 1000L) / 10) << 8;
      result |= ((freq % 10000L) / 10) << 12;
      return result;
   }
} rmp1;

