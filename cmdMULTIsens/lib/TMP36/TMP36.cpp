/*
  TMP36.h - Library for reading the temperature of a TMP36.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/

//#include "Arduino.h"
#include "TMP36.h"

TMP36::TMP36(int pin, int samples)
{
  _pin = pin;
  _samples = samples;
}

void TMP36::begin()
{
  // set up the pin!
  pinMode(_pin, INPUT);
  //_lastreadtime = 0;
}

float TMP36::readTemperature(void)
{
  return -0.01;
}
