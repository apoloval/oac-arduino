/*
 * Open Airbus Cockpit - Arduino OACSP library
 * Copyright (c) 2012-2015 Alvaro Polo
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef OAC_OACSP_H
#define OAC_OACSP_H

#include "Arduino.h"

#define OACSP_PROTOCOL_VERSION 0x01
#define OACSP_BUFFER_LEN 256
#define OACSP_MAX_NAME_LEN 64

namespace OAC {

enum OffsetLength {
  OFFSET_UINT8,
  OFFSET_SINT8,
  OFFSET_UINT16,
  OFFSET_SINT16,
  OFFSET_UINT32,
  OFFSET_SINT32,
};

const char* OffsetLengthCode[]  = { "UB", "SB", "UW", "SW", "UD", "SD" };

enum EventType {
  NO_EVENT,
  LVAR_UPDATE,
  OFFSET_UPDATE,
};

struct LVarUpdateEvent {
  EventType type;
  char name[OACSP_MAX_NAME_LEN];
  long value;
};

struct OffsetUpdateEvent {
  EventType type;
  word address;
  long value;
};

union Event {
  EventType type;
  LVarUpdateEvent lvar;
  OffsetUpdateEvent offset;
};

class SerialProtocol {
public:

  SerialProtocol() {
    polledEvent.type = NO_EVENT;
  }

  void begin(const char* clientName, int baudRate = 9600) {
    buffer.reserve(512);
    Serial.begin(baudRate);
    while (!Serial) {} // Wait for port to be open in Leonardo and Due
    Serial.print("BEGIN ");
    Serial.print(OACSP_PROTOCOL_VERSION, HEX);
    Serial.print(" ");
    Serial.print(clientName);
    Serial.print('\n');
  }

  void end() {
    Serial.print("END\n");
  }

  template <typename T> 
  void writeLVarAs(const char* lvar, T value) {
    Serial.print("WRITE_LVAR ");
    Serial.print(lvar);
    Serial.print(" ");
    Serial.print(value, DEC);
    Serial.print('\n');
  }

  void writeLVar(const char* lvar, int value) {
    writeLVarAs<int>(lvar, value);
  }

  void writeOffset(word offset, unsigned char value) {
    writeOffset(offset, OFFSET_UINT8, value);
  }

  void writeOffset(word offset, char value) {
    writeOffset(offset, OFFSET_SINT8, value);
  }

  void writeOffset(word offset, unsigned int value) {
    writeOffset(offset, OFFSET_UINT16, value);
  }

  void writeOffset(word offset, int value) {
    writeOffset(offset, OFFSET_SINT16, value);
  }

  void writeOffset(word offset, unsigned long value) {
    writeOffset(offset, OFFSET_UINT32, value);
  }

  void writeOffset(word offset, long value) {
    writeOffset(offset, OFFSET_SINT32, value);
  }

  template <typename T>
  void writeOffset(word offset, OffsetLength len, T value) {
    Serial.print("WRITE_OFFSET ");
    Serial.print(offset, HEX);
    Serial.print(":");
    Serial.print(OffsetLengthCode[len]);
    Serial.print(" ");
    Serial.print(value, DEC);
    Serial.print('\n');
  }

  void observeLVar(const char* lvar) {
    Serial.print("OBS_LVAR ");
    Serial.print(lvar);
    Serial.print('\n');
  }

  void observeOffset(word offset, OffsetLength len) {
    Serial.print("OBS_OFFSET ");
    Serial.print(offset, HEX);
    Serial.print(":");
    Serial.print(OffsetLengthCode[len]);
    Serial.print('\n');
  }

  Event* pollEvent() {
    polledEvent.type = NO_EVENT;
    Event* result = NULL;
    while (Serial.available()) {
      char c = char(Serial.read());
      buffer += c;
      if (c == '\n') {
        if (buffer.startsWith("EVENT_LVAR")) {
          result = pollLVarEvent(buffer);
        } else if (buffer.startsWith("EVENT_OFFSET")) {
          result = pollOffsetEvent(buffer);
        }
        buffer = "";
        break;
      }
      return result;
    }
  }

  Event* event() {
    return (polledEvent.type != NO_EVENT) ? &polledEvent : NULL;
  }

  LVarUpdateEvent* lvarUpdateEvent(const char* lvar) {
    return (
      (polledEvent.type == LVAR_UPDATE) && 
      strcmp(polledEvent.lvar.name, lvar) == 0) ? &(polledEvent.lvar) : NULL;
  }

  OffsetUpdateEvent* offsetUpdateEvent(word address) {
    return (
      (polledEvent.type == OFFSET_UPDATE) && 
      (polledEvent.offset.address == address)) ? &(polledEvent.offset) : NULL;
  }

private:

  Event* pollLVarEvent(String& line) {
    line.replace("EVENT_LVAR", "");
    line.trim();
    String lvar, value;
    if (!parseTuple2(line, lvar, value))
      return NULL;
    polledEvent.type = LVAR_UPDATE;
    lvar.toCharArray(polledEvent.lvar.name, 64);
    polledEvent.lvar.value = value.toInt();
    return &polledEvent;
  }

  Event* pollOffsetEvent(String& line) {
    line.replace("EVENT_OFFSET", "");
    line.trim();
    String offset, value;
    if (!parseTuple2(line, offset, value))
      return NULL;
    polledEvent.type = OFFSET_UPDATE;
    polledEvent.offset.address = hexToLong(offset);
    polledEvent.offset.value = value.toInt();
    return &polledEvent;
  }

  bool parseTuple2(const String& line, String& tk1, String& tk2) {
    int sep = line.indexOf(' ');
    if (sep == -1)
      return false;
    tk1 = line.substring(0, sep);
    tk2 = line.substring(sep + 1, line.length());
    return true;
  }

  word hexToLong(const String& hex) {
    char buf[9];
    hex.toCharArray(buf, 8);
    return strtol(buf, 0, 16);
  }

  Event polledEvent;
  String buffer;
};

}

OAC::SerialProtocol OACSP;

#endif
