#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// http://www.avdweb.nl/arduino/hardware-interfacing/temperature-measurement.html

#include "ChipTemp.h"

ChipTemp::ChipTemp()
{
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

float ChipTemp::deciCelsius()
{ long averageTemp=0;
  initialize(); // must be done everytime
  for (int i=0; i<samples; i++) averageTemp += readAdc();
  averageTemp -= offsetFactor;
  return (float)averageTemp / (float)divideFactor; // return deci degree Celsius
}

float ChipTemp::celsius()
{ return deciCelsius()*0.1;
}
