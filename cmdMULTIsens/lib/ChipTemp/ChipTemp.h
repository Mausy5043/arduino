#ifndef ChipTemp_H
#define ChipTemp_H
// ATmega328 temperature sensor interface
// Rev 1.0 Albert van Dalen www.avdweb.nl
// Based on "InternalTemp"
// Requires 166 ... 204 bytes program memory
// Resolution 0.1 degree

// Calibration values, set in decimals
static const float offset = 335.2; // change this by calibration result
static const float gain = 1.06154;

static const int samples = 1000; // must be >= 1000, else the gain setting has no effect

// Compile time calculations
static const long offsetFactor = offset * samples;
static const int divideFactor = gain * samples/10; // deci = 1/10

class ChipTemp
{
public:
  ChipTemp();
  int deciCelsius();
  int celsius();

private:
  inline void initialize();
  inline int readAdc();
};

#endif
