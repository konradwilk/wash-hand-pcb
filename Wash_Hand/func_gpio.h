/*
# Copyright 2020, Konrad Rzeszutek Wilk
# SPDX-License-Identifier: CC-BY-4.0
*/

#include "alpha.h"

const int pinA = 0;
const int pinB = 1;
const int pinC = 9; // PB1 b/c 2 we want for interrupt if MCP23017 is added
const int pinD = 3;
const int pinE = 4;
const int pinF = 5;
const int pinG = 6;
const int pinDP = 7;
const int D4 = A3; //PB0
const int D3 = A2;
const int D2 = A1;
const int D1 = 8;

unsigned int on = LOW;
unsigned int off = HIGH;

const byte BUTTON = A0;

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

void debug_begin(unsigned int val) {}
void debug_end(void) { }
void debug_print(unsigned int val) { }
void debug_error(const char s[]) { }
