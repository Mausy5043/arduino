/*
  TMP36.h - Library for reading the temperature of a TMP36.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/
#ifndef TMP36_h
#define TMP36_h

#include "Arduino.h"

static const float T36_gain = 1;  //.02380395824;
static const float T36_offset = 0;  // 1.94747688944;

class TMP36
{
  public:
    TMP36(int pin, int samples, float ref5v);
    void begin(void);
    float readTemperature(void);

  private:
    int _pin;       // which pin the TMP36 is connected to
    int _samples;   // number of samples used for averaging
    float _ref5v;   // 5V reference value used for TMP36
    float _invsamples; // (1/_samples) for faster multiplications
};

#endif
