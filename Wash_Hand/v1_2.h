/*
  Copyright 2020, Konrad Rzeszutek Wilk
  SPDX-License-Identifier: CC-BY-4.0
*/

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
