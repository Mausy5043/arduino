/*
  VBAT.h - Library for reading the voltage of a battery or solarcell.
  See sensV for circuitry.
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/
#ifndef VBAT_h
#define VBAT_h

#include "Arduino.h"

class VBAT
{
  public:
    VBAT(int pin, int samples, float ref5v, float r1, float r2);
    void begin(void);
    float readVoltage(void);

  private:
    int _pin;       // which pin the +V is connected to
    int _samples;   // number of samples used for averaging
    float _ref5v;   // 5V reference value used for VBAT
    float _r1;      // Large resistor value in Ohm
    float _r2;      // Small resistor value in Ohm
    float _r12;     // reductionfactor
    float _invsamples; // (1/_samples) for faster multiplications
};

#endif
