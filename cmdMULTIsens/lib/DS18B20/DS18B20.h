/*
  DS18B20.h - ...
  Created by Maurice (Mausy5043) Hendrix, FEB2015

  MIT license
*/
#ifndef DS18B20_h
#define DS18B20_h

#include "Arduino.h"
#include "OneWire.h"

class DS18B20
{
  public:
    DS18B20(int pin, int samples);
    void begin(void);
    float readTemperature(void);

  private:
    int _pin;       // which pin the +V is connected to
    int _samples;   // number of samples used for averaging
    float _invsamples; // (1/_samples) for faster multiplications
    byte _addr[8];
    byte _data[12];
};

#endif
