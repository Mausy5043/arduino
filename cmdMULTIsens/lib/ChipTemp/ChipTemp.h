#ifndef ChipTemp_H
#define ChipTemp_H
// ATmega328 temperature sensor interface
// Rev 1.0 Albert van Dalen www.avdweb.nl
// Based on "InternalTemp"
// Requires 166 ... 204 bytes program memory
// Resolution 0.1 degree

// Calibration values, set in decimals

/*
  Calibration date 20150222:
  offset = 335.2 (original)
  gain = 1.06154 (original)
  offset’ = T(chip) * gain + offset – gain * T(room)

  T(room) = 16.5
  T(chip) = 20.54

  offset' = 20.54 * 1.06154 + 335.2 - 1.06154 * 16.5  =  339.4886216

  01MAR2015:
  offset' = 16.29 * 1.06154 + 339.4886216 - 1.06154 * 19.25  =  336.3464632
*/

static const float CT_offset = 336.3464632; // 339.4886216; // 335.2; // change this by calibration result
static const float CT_gain = 1.06154;

// Compile time calculations
static const float CT_invgain = 1 / CT_gain;

class ChipTemp
{
public:
  ChipTemp(int samples);
  float readTemperature();

private:
  inline void initialize();
  inline int readAdc();
  int _samples;      // number of samples used for averaging
  float _invsamples; // (1/_samples) for faster multiplications
};

#endif
