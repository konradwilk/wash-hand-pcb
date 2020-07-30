/*
# Copyright 2020, Konrad Rzeszutek Wilk
# SPDX-License-Identifier: CC-BY-4.0
*/

unsigned int _shadow_state = 0;
unsigned int _d1 = 'Z';
unsigned int _d2 = 'Z';
unsigned int _d3 = 'Z';
unsigned int _d4 = 'Z';
unsigned int _tickle = 0;

void setupPorts() {

  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  Serial.println("DEBUG MODE");
  _shadow_state = 0;
  _tickle = 0;
}

void writeD1(char d1) {
  _d1 = d1;
  _tickle = 1;
}

void writeD2(char d2) {
  _d2 = d2;
  _tickle = 1;
}

void writeD3(char d3) {
  _d3 = d3;
  _tickle = 1;
}

void writeD4(char d4) {
  _d4 = d4;
  _tickle = 1;
}

int readSensor(void) {

  if (Serial.available() > 0) {
    Serial.read();
    return 0;
  }

  return 1024;
}

void debug_print(unsigned int v) {
  Serial.println(v, DEC);
}
void debug_begin(unsigned int state) {

  _d4 = _d3 = _d2 = _d1 = ' ';

  if (state == _shadow_state)
    return;

  Serial.print("STATE=");
  Serial.println(state);

  _shadow_state = state;
}

void debug_end(void) {

  if (!_tickle)
    return;

  Serial.write(_d1);
  Serial.write(_d2);
  Serial.write(_d3);
  Serial.write(_d4);
  Serial.println("\n");
  _tickle = 0;
}

