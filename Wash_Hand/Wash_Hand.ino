/*
# Copyright 2020, Konrad Rzeszutek Wilk
# SPDX-License-Identifier: CC-BY-4.0
*/
#include "func_ports.h"

void write(char d1, char d2, char d3, char d4, unsigned int sleep) {

  writeD1(d1);
  delay(sleep);

  writeD2(d2);
  delay(sleep);

  writeD3(d3);
  delay(sleep);

  writeD4(d4);
  delay(sleep);
}

void setup(void) {

  setupPorts();
}

#define NOBODY  0
#define NOBODY_STR  ' ',' ',' ',' '

#define SOAP    NOBODY+1
#define SOAP_STR    'S','O','A','P'

#define WASH	SOAP+1
#define WASH_STR    'G','o','G','o'

#define COUNT   WASH+1
#define COUNT_MAX COUNT+20

#define YAY	COUNT_MAX+1
#define YAY_STR     'Y','A','Y',' '

#define DONE    YAY+1
#define DONE_STR    'D','O','N','E'

#define TEST	DONE+1
#define TEST_STR    '1','2','3','4'

#define  DELAY 5
#define STR_DELAY 5000
#define SEC_DELAY 100


unsigned int state = 0;
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

  sensor = analogRead(BUTTON);
  if (sensor < 500) {
    if (state == NOBODY) {
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
      state = waitFor(STR_DELAY, WASH);
      break;
    case WASH:
      write(WASH_STR, DELAY);
      /* This ends up jumping to the default case */
      state = waitFor(STR_DELAY, COUNT);
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
      write(NOBODY_STR, 0);
      break;
    default:
      if ((state >= COUNT) && (state < COUNT_MAX)) {
	if (state < COUNT+10) {
           writeD4('1' + state - COUNT);
           state=waitFor(STR_DELAY, state + 1);
        } else {
	   writeD3('0');
	   delay(DELAY);
           writeD4('1' + state - COUNT);
           state=waitFor(STR_DELAY, state + 1);
        }
      } else if (state == COUNT_MAX) {
           state = YAY;
      } else {
      	   state = TEST;
      }
      break;
  }
}
