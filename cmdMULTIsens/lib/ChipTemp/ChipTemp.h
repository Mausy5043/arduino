#ifndef ChipTemp_H
#define ChipTemp_H
// ATmega328 temperature sensor interface
// Rev 1.0 Albert van Dalen www.avdweb.nl
// Based on "InternalTemp"
// Requires 166 ... 204 bytes program memory
// Resolution 0.1 degree
16.5 = 17.68
// Calibration values, set in decimals
static const float offset = 319.561; // 335.2; // change this by calibration result
static const float gain = 1.06154;

static const int samples = 1000; // must be >= 1000, else the gain setting has no effect

// Compile time calculations
static const long offsetFactor = offset * samples;
static const float divideFactor = gain * samples/10; // deci = 1/10

class ChipTemp
{
public:
  ChipTemp();
  float deciCelsius();
  float celsius();

private:
  inline void initialize();
  inline int readAdc();
};

#endif
