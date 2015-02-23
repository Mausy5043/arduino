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

static const float offset = 339.489; // 335.2; // change this by calibration result
static const float gain = 1.06154;

static const int samples = 1000; // must be >= 1000, else the gain setting has no effect

// Compile time calculations
static const long offsetFactor = offset * samples;
static const float divideFactor = gain * samples/10; // deci = 1/10

class ChipTemp
{
public:
  ChipTemp();
  float readTemperature();
  //float celsius();

private:
  inline void initialize();
  inline int readAdc();
};

#endif
