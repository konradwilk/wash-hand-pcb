/*
# Copyright 2020, Konrad Rzeszutek Wilk
# SPDX-License-Identifier: CC-BY-4.0
*/
#include "func_gpio.h"

/* For DEBUG - use this. */
//#include "func_usb.h"

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

}

#define INIT  0

#define SOAP        (INIT+1)
#define SOAP_STR    'S','O','A','P'

#define WASH        (SOAP+1)
#define WASH_STR    'G','o','G','o'

#define COUNT       (WASH+1)
#define COUNT_MAX   (COUNT+20)

#define YAY         (COUNT_MAX+1)
#define YAY_STR     'Y','A','Y',' '

#define DONE        (YAY+1)
#define DONE_STR    'D','O','N','E'

#define CLEAR       (DONE+1)
#define CLEAR_STR   ' ',' ',' ',' '

#define  DELAY 5
#define STR_DELAY 5000
#define SEC_DELAY 1000

#define DEBUG_STR   '8','8','8','8'

unsigned int state = INIT;
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

  if (debug_loop()) {
    setupPorts();
    write(DEBUG_STR, DELAY);
    (void)waitFor(STR_DELAY, INIT);
  }
  sensor = readSensor();
  if (sensor < 500) {
    if (state == INIT) {
        setupPorts();
        /* All of them cleared. */
        state = SOAP;
        s = millis();
    }
  }
  debug_begin(state);
  switch (state) {
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
      state = waitFor(STR_DELAY, CLEAR);
      break;
    case CLEAR:
      write(CLEAR_STR, 0);
      state = waitFor(10, INIT);
      break;
    case INIT:
      break;
    default:
      if ((state >= COUNT) && (state <= COUNT_MAX)) {
        unsigned int n = state - (COUNT);

        debug_print(n);

        if (n == 0) {
          state = state + 1;
        } else if (n <= 9) {
          writeD4('0' + n);
          state = waitFor(SEC_DELAY, state + 1);
        } else if ((n >= 10) && (n < 20)) {
          writeD3('1');
          delay(DELAY);
          writeD4('0' + n - 10);
          delay(DELAY);
          state=waitFor(SEC_DELAY, state + 1);
        } else if (n >= 20) {
          writeD3('2');
          delay(DELAY);
          writeD4('0'+ n - 20);
          delay(DELAY);
          state = waitFor(SEC_DELAY, state + 1);
        } else
          debug_error(__LINE__);

      } else if (state > COUNT_MAX) {
          state = YAY;
      } else {
          state = CLEAR;
      }
      break;
  }
  debug_end();
}
