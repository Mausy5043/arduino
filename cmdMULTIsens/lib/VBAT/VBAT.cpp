/*
  VBAT.h - Library for reading the voltage of a battery or solarcell.
  See sensV for circuitry.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/

#include "VBAT.h"

VBAT::VBAT(int pin, int samples, float ref5v, float r1, float r2)
{
  _pin = pin;
  _samples = samples;
  _ref5v = ref5v;
  _r1 = r1;
  _r2 = r2;
}

void VBAT::begin(void)
{
  // set up the pin!
  pinMode(_pin, INPUT);
  _r12 = ((_r1 + _r2)/_r2);
  _invsamples =  1.0/(float)_samples;
  //_lastreadtime = 0;
}

float VBAT::readVoltage(void)
{
  //int cntSamples = 0;
  int sumSamples;
  //float measurement = 0.0;

  // add up the pre-defined number of _samples for Sample Averaging
  for (int cntSamples = 0; cntSamples < _samples; cntSamples++)
  {
    sumSamples += analogRead(_pin);
    // minimum delay on analog pins is 100ms
    delay(110);
  }
  float measurement = (float)sumSamples * _invsamples; // Calculate avg raw value.
  measurement = map(measurement * 10, 0, 10230, 0, _ref5v * 10000) * 0.0001;
  measurement *= _r12;
  return measurement;
}
