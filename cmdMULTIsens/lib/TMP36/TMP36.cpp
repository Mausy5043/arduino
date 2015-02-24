/*
  TMP36.h - Library for reading the temperature of a TMP36.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/

#include "TMP36.h"

TMP36::TMP36(int pin, int samples, float ref5v)
{
  _pin = pin;
  _samples = samples;
  _ref5v = ref5v;
}

void TMP36::begin()
{
  // set up the pin!
  pinMode(_pin, INPUT);
  _invsamples =  1.0/(float)_samples;
  //_lastreadtime = 0;
}

float TMP36::readTemperature(void)
{
  // int cntSamples = 0;
  int sumSamples;
  //float measurement = 0.0;

  //analogReference(EXTERNAL);
  delay(200);
  // add up the pre-defined number of _samples for Sample Averaging
  for (int cntSamples = 0; cntSamples < _samples; cntSamples++)
  {
    sumSamples += analogRead(_pin);
    // minimum delay on analog pins is 100ms
    //Serial.println(sumSamples);
    delay(200);
  }
  //measurement = (float)sumSamples / (float)cntSamples; // Calculate avg raw value.
  float measurement = (float)sumSamples * _invsamples; // Calculate avg raw value.
  measurement = map(measurement * 10, 0, 10230, 0, _ref5v * 10000) * 0.0001;

  //analogReference(DEFAULT);
  return 100.0 * measurement - 50.0;
}
