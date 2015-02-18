/*
  DS18B20.cpp - Library ...
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/

#include "DS18B20.h"

OneWire _1wire(10);

DS18B20::DS18B20(int pin, int samples)
{
  _pin = pin;
  _samples = samples;

}

void DS18B20::begin(void)
{
  int i;
  // set up the pin!
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
  int i = 0;
  int type_s = 0;

  _1wire.reset();
  _1wire.select(_addr);
  _1wire.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  byte present = _1wire.reset();
  _1wire.select(_addr);
  _1wire.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    _data[i] = _1wire.read();
    //Serial.print(_data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(_data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (_data[1] << 8) | _data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (_data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - _data[6];
    }
  } else {
    byte cfg = (_data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  float celsius = (float)raw / 16.0;
  //Serial.print("  Temperature = ");
  //Serial.print(celsius);
  //Serial.println(" Celsius, ");

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
  return celsius;
}
