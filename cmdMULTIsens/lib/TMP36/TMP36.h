/*
  TMP36.h - Library for reading the temperature of a TMP36.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/
#ifndef TMP36_h
#define TMP36_h

#include "Arduino.h"

class TMP36
{
  public:
    TMP36(int pin, int samples);
    void begin(void);
    float readTemperature(void);

  private:
    int _pin;       // which pin the TMP36 is connected to
    int _samples;   // number of samples used for averaging
};

#endif
