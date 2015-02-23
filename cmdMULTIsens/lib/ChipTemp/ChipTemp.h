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
  offset = 335.2
  gain = 1.06154
  offset’ = Celsius * gain / 10 + offset – gain * T

  RT=16.5
  Celsius = 20.54

  offset' = 20.54 * 1.06154 /10 + 335.2 -1.06154*16.5  =  339.4886216
*/

static const float CT_offset = 339.489; // 335.2; // change this by calibration result
static const float CT_gain = 1.06154;

//static const int CT_samples = 1000; // must be >= 1000, else the gain setting has no effect

// Compile time calculations
//static const long offsetFactor = CT_offset * CT_samples;
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
