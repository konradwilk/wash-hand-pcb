/*
# Copyright 2020, Konrad Rzeszutek Wilk
# SPDX-License-Identifier: CC-BY-4.0
*/

#define DEBUG 1
#ifndef ARDUINO_AVR_NG
#define DEBUG_TIME 1
#endif
#define NOSD 1

#define STRINGIFY(x) #x
#ifndef USB
#include "func_gpio.h"
#else
#include "func_usb.h"
#endif

#ifndef NOSD
#include <SPI.h>
#include <SD.h>
#else
#endif

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

#include <avr/sleep.h>
#include <avr/wdt.h>

#ifdef DEBUG_TIME
volatile bool wdtFired;
unsigned long startTime = 0;
unsigned long endTime = 0;

ISR (WDT_vect)
{
   wdt_disable();  // disable watchdog
   wdtFired = true;
}  // end of WDT_vect

void setup ()
{
  noInterrupts ();           // timed sequence follows
  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = bit (WDCE) | bit (WDE);
  // set interrupt mode and an interval
  WDTCSR = bit (WDIE);    // set WDIE, and 16 ms seconds delay
  wdt_reset();  // pat the dog
  wdtFired = false;
  interrupts ();

  startTime = micros ();
  while (!wdtFired)
    { }  // wait for watchdog
  endTime = micros ();

}
#else
void setup (void) {

}
#endif

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

#ifndef DEBUG
#define  DELAY 5
#define STR_DELAY 5000
#define SEC_DELAY 1000
#else
#define  DELAY 1
#define STR_DELAY 500
#define SEC_DELAY 100
#endif
#define DEBUG_STR   '1','2','3','8'

unsigned int state = INIT;
unsigned long s;

#ifndef NOSD
Sd2Card card;
SdVolume volume;
SdFile root;

const int CS = 10;
unsigned int done_sd = 0;
#endif
unsigned int waitFor(unsigned long timeout, unsigned new_state)
{
  unsigned long e = millis();

  if ((e - s) > timeout) {
    s = e;
    return new_state;
  }
  return state;
}


void printk(const char *str) {

  char *s;
  unsigned int i = 0;
  char buf[4] = {};

  s = (char *)str;
  do {
    if (*s) {
      buf[i] = *s;
      i++;
      s++;
    }
    if (i == 4 || *s == 0) {
      unsigned long _e, _s;

      _e = _s = millis();
      do {
        write(buf[0], buf[1], buf[2], buf[3], DELAY);
        debug_end();
        _e = millis();
      } while ( (_e - _s ) < STR_DELAY);

      buf[0] = buf[1] = buf[2] = buf[3] = ' ';
      i = 0;
    }
  } while (*s != 0);

}

void printk(unsigned long val) {

   char buf[8] = {};
   char dst[8] = {};
   unsigned int i = 0, j;
   unsigned int v;

   while (val > 0) {
     v = val % 10;
     buf[i]= '0' + v;
     val /= 10;
     if (val > 0)
        i++;
   }
   for (j = 0; j <= i; j++) {
     dst[j] = buf[i-j];
   }
   j++;
   dst[j++]='\0';
   printk(dst);
}

#define STR(x)  STRINGIFY(x)
#define ERR()   printk("ERR " STR(__LINE__));

#ifndef printf
#define printf(x) ;
#endif

unsigned int done = 0;
void loop() {
  int sensor = 0;
#ifdef DEBUG_TIME
  unsigned long delta;
#endif

  if (debug_loop() && !done) {
    setupPorts();
    printk("DBG");
#ifdef DEBUG_TIME
    printk("TIME");
    delta = (endTime - startTime);
    printk(delta);
#endif
    done = 1;
  }

#ifndef NOSD
  if (!done_sd) {
    setupPorts();

    if (!card.init(SPI_HALF_SPEED, CS)) {
      ERR();
    } else {
      switch (card.type()) {
        case SD_CARD_TYPE_SD1:
          printk("SD01");
          break;
        case SD_CARD_TYPE_SD2:
          printk("SD02");
          break;
        case SD_CARD_TYPE_SDHC:
          printk("SDHC");
          break;
        default:
          ERR();
          break;
      }
      if (!volume.init(card)) {
        ERR();
      } else {
        printk("OK!");
      }
    }
    done_sd = 1;
  }
#endif
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
          debug_error(STRINGIFY(__LINE__));

      } else if (state > COUNT_MAX) {
          state = YAY;
      } else {
          state = CLEAR;
      }
      break;
  }
  debug_end();
}
