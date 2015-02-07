/*
 * Open Airbus Cockpit - Arduino Encoder library
 * Copyright (c) 2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef OAC_ENC_H
#define OAC_ENC_H

#include <arduino.h>
#include <limits.h>

namespace OAC {

/** 
 * This class wraps a long number and ensures it is maintained within a
 * given range. 
 */
class RangedNumber {
public:

   RangedNumber(long value = 0, long min_value = 0, long max_value = LONG_MAX) 
      : _min(min_value), _max(max_value) {
      setValue(value);
   }
  
   long value() const { return _value; }

   void setMax(long max) {
      _max = max;
      setValue(_value);
   }
  
   void setMin(long min) {
      _min = min;
      setValue(_value);
   }

   void setRange(long min, long max) {
      _min = min;
      _max = max;
      setValue(_value);
   }

   void setup(long value, long min, long max) {
      _min = min;
      _max = max;
      setValue(value);
   }
  
   void setValue(long value) {
      if (value < _min) { value = _min; }
      else if (_max < value) { value = _max; }
      _value = value;
   }
  
   long inc(long v) {
      long max_inc = (v < 0) ? _min - _value : _max - _value;
      if (v > 0 && v > max_inc) { v = max_inc; }
      if (v < 0 && v < max_inc) { v = max_inc; }
      _value += v;
      return v;
   }
  
   void swap(RangedNumber& other) {
      long mine = _value;
      long their = other._value;
      setValue(their);
      other.setValue(mine);
   }

private:

   long _value;
   long _min;
   long _max;
};

/**
 * A rotary encoder. 
 * 
 * The rotary encoder sends quadratic signals across two digital lines. 
 * The shift between such signals can be used to determine the movement
 * of the rotary encoder.
 */
class RotaryEncoder {
public:
  
   RotaryEncoder() : _lastA(LOW) {}
  
   void setPins(byte pinA, byte pinB) {
      _pinA = pinA;
      _pinB = pinB;

      pinMode(_pinA, INPUT);
      pinMode(_pinB, INPUT);
   }
  
   /** Read the increment of the encoder, or 0 if none. */
   int read() {
      byte a = digitalRead(_pinA);
      byte b = digitalRead(_pinB);
      int result = 0;
      if (!a && _lastA) {
         result = b ? 1 : -1;
      }
      _lastA = a;
      return result;
   }
  
   /**
    * Read the increment of the encoder and apply it to the given ranged
    * number. The `mult` argument can be used to indicate the multiplier
    * to be applied to the read increment before applying it to the
    * ranged number. 
    */
   long read(RangedNumber& num, int mult = 1) {
      long inc = read() * mult;
      return (inc != 0) ? num.inc(inc) : 0;
   }
  
private:

   byte _pinA;
   byte _pinB;
   byte _lastA;
};

}

#endif
