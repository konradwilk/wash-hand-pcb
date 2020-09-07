/*
  Copyright 2020, Konrad Rzeszutek Wilk
  SPDX-License-Identifier: CC-BY-4.0
*/

#include "alpha.h"

#include "v1_3.h"

const uint16_t char_to_port[] = {
    pinA,
    pinB,
    pinC,
    pinD,
    pinE,
    pinF,
    pinG,
    pinDP,
};

void code(char c) {
  unsigned int i;
  unsigned int idx = (unsigned int)c - (unsigned int)' ';
  unsigned int val = SevenSegmentASCII[idx];

  for (i = 0; i < 7; i++) {
    if ((val & 1 <<i)) {
      digitalWrite(char_to_port[i], on);
    }
    else
      digitalWrite(char_to_port[i], off);
  }
}

void setupPorts() {

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinE, OUTPUT);
  pinMode(pinF, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

}

void writeD1(char d1) {
  digitalWrite(D1, off);
  digitalWrite(D2, on);
  digitalWrite(D3, on);
  digitalWrite(D4, on);
  code(d1);
}

void writeD2(char d2) {
  digitalWrite(D1, on);
  digitalWrite(D2, off);
  digitalWrite(D3, on);
  digitalWrite(D4, on);
  code(d2);
}

void writeD3(char d3) {
  digitalWrite(D1, on);
  digitalWrite(D2, on);
  digitalWrite(D3, off);
  digitalWrite(D4, on);
  code(d3);
}

void writeD4(char d4) {
  digitalWrite(D1, on);
  digitalWrite(D2, on);
  digitalWrite(D3, on);
  digitalWrite(D4, off);
  code(d4);
}

int readSensor(void) {

  return analogRead(BUTTON);
}

#ifdef DEBUG
static inline int debug_loop() { return 1; }
#else
static inline int debug_loop() { return 0; }
#endif

void debug_begin(unsigned int) {}
void debug_end(void) { }
void debug_print(unsigned int) { }
void debug_error(const char *) { }
