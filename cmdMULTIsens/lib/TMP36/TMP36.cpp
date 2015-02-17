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
  int cntSamples;
  int sumSamples;
  float measurement;

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (cntSamples = 0; cntSamples < _samples; cntSamples++) {
    sumSamples += analogRead(_pin);
    // minimum delay on analog pins is 100ms
    delay(110);
  }

  measurement = (float)sumSamples / (float)cntSamples; // Calculate avg raw value.
  measurement = map(measurement * 10, 0, 10230, 0, ref5V * 10000) * 0.0001;
  return 100 * measurement - 50.0;
}
