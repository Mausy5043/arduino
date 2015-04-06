#ifndef ChipTemp_H
#define ChipTemp_H
// ATmega328 temperature sensor interface
// Rev 1.0 Albert van Dalen www.avdweb.nl
// Based on "InternalTemp"
// Resolution 0.1 degree

static const float CT_gain = 0.8748936170;
static const float CT_offset = -292.5870;

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
