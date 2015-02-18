/*
  DS18B20.h - Library for reading the voltage of a battery or solarcell.
  See sensV for circuitry.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/

#include "DS18B20.h"

DS18B20::DS18B20(int pin, int samples)
{
  _pin = pin;
  _samples = samples;
}

void DS18B20::begin(void)
{
  // set up the pin!
  OneWire _1wire(_pin);
  _invsamples =  1.0/(float)_samples;
  if ( !_1wire.search(_addr))
  {
    Serial.println("No sensor.");
  }
}

float DS18B20::readTemperature(void)
{
  int cntSamples = 0;
  int sumSamples = 0;
  float measurement = 0.0;

  // add up the pre-defined number of _samples for Sample Averaging
  for (cntSamples = 0; cntSamples < _samples; cntSamples++)
  {
    //sumSamples += analogRead(_pin);
    // minimum delay on analog pins is 100ms
    delay(110);
  }
  //measurement = (float)sumSamples * _invsamples; // Calculate avg raw value.
  //measurement = map(measurement * 10, 0, 10230, 0, _ref5v * 10000) * 0.0001;
  //measurement *= _r12;
  //return measurement;
  return -0.001;
}
