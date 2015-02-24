#ifndef DHT_H
#define DHT_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/* DHT library

MIT license
written by Adafruit Industries

Modifications by M. Hendrix (Mausy5043) FEB2015:
- Added function computeDewPoint
- Added function computeDewPoint2

*/

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21

class DHT {
 private:
  uint8_t data[6];
  uint8_t _pin, _type, _count;
  unsigned long _lastreadtime;
  boolean firstreading;

 public:
  DHT(uint8_t pin, uint8_t type, uint8_t count=6);
  void begin(void);
  float readTemperature(void);
  float convertCtoF(float);
  float convertFtoC(float);
  float computeHeatIndex(float tempCelsius, float percentHumidity);
  float computeDewPoint(float tempCelsius, float percentHumidity);
  float computeDewPoint2(float tempCelsius, float percentHumidity);
  float readHumidity(void);
  boolean read(void);

};

#endif
