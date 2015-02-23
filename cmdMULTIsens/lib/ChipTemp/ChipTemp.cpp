#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// http://www.avdweb.nl/arduino/hardware-interfacing/temperature-measurement.html

#include "ChipTemp.h"

ChipTemp::ChipTemp(int samples)
{
  _samples = samples;
  _invsamples = 1 / samples;
}

inline void ChipTemp::initialize()
{ ADMUX = 0xC8; // select reference, select temp sensor
  delay(10); // wait for the analog reference to stabilize
  readAdc(); // discard first sample (never hurts to be safe)
}

inline int ChipTemp::readAdc()
{ ADCSRA |= _BV(ADSC); // start the conversion
  while (bit_is_set(ADCSRA, ADSC)); // ADSC is cleared when the conversion finishes
  return (ADCL | (ADCH << 8)); // combine bytes
}

float ChipTemp::readTemperature()
{ float averageTemp=0.0;
  initialize(); // must be done everytime
  for (int i=0; i<_samples; i++)
  {
    averageTemp += readAdc();
  }
  averageTemp *= _invsamples;
  averageTemp -= CT_offset;
  averageTemp * CT_invgain;
  return averageTemp;
}
