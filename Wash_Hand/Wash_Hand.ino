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


const byte BUTTON = A0;
const byte DELAY = 5;

unsigned int digit = 0;
// Common anode has off has off.

unsigned int on = LOW;
unsigned int off = HIGH;

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


void setup() {
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
  digit = 0;

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
void write(char d1, char d2, char d3, char d4, unsigned int sleep) {

  delay(sleep);
  writeD1(d1);

  delay(sleep);
  writeD2(d2);

  delay(sleep);
  writeD3(d3);

  delay(sleep);
  writeD4(d4);
}



#define NOBODY  1<<0
#define NOBODY_STR  ' ',' ',' ',' '

#define SOAP    1<<1
#define SOAP_STR    'S','O','A','P'

#define WASH_0  1<<2
#define WASH_0_STR  'G','o','G','o'

#define WASH_1  1<<3

#define WAIT_1  1<<4
#define WAIT_2  1<<5
#define WAIT_3  1<<6
#define WAIT_4  1<<7


#define WAIT_2_STR  'o','_','_','o'

#define YAY   1<<11

#define YAY_STR  'Y','A','Y',' '

#define DONE    1<<12
#define DONE_STR   'D','O','N','E'

#define TEST  1<<15
#define TEST_STR  '1','2','3','4'

#define STR_DELAY 5000
#define SEC_DELAY 100


unsigned int state = NOBODY;
unsigned long s;

unsigned int waitFor(unsigned long timeout, unsigned new_state)
{
  unsigned long e = millis();

  if ((e - s) > timeout) {
    s = e;
    return new_state;
  }
  return state;
}

void loop() {
  int sensor = 0;
  unsigned int i;

  sensor = analogRead(BUTTON);
  if (sensor < 500) {
    if (state & NOBODY) {
        /* All of them cleared. */
        state = SOAP;
        s = millis();
    }
  }

  switch (state) {
    case TEST:
      write(TEST_STR, 5);
      state = waitFor(STR_DELAY, NOBODY);
      break;
    case SOAP:
      write(SOAP_STR, DELAY);
      state = waitFor(STR_DELAY, WASH_0);
      break;
    case WASH_0:
      write(WASH_0_STR, DELAY);
      state = waitFor(STR_DELAY, WASH_1);
      break;
    case WASH_1:
      writeD4('1');
      state=waitFor(SEC_DELAY, WAIT_1);
      break;
    case WAIT_1:
      writeD4('2');
      state = waitFor(SEC_DELAY, WAIT_2);
      break;
    case WAIT_2:
      writeD4('3');
      state = waitFor(SEC_DELAY, WAIT_3);
      break;
    case WAIT_3:
      writeD4('4');
      state = waitFor(SEC_DELAY, YAY);
      break;
    case YAY:
      write(YAY_STR, DELAY);
      state = waitFor(STR_DELAY, DONE);
      break;
    case DONE:
      write(DONE_STR, DELAY);
      state = waitFor(STR_DELAY, NOBODY);
      break;
    case NOBODY:
    default:
      delay(DELAY);
      write(NOBODY_STR, 0);
      break;
  }
}
