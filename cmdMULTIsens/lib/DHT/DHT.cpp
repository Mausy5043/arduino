/* DHT library

MIT license
written by Adafruit Industries

Modifications by M. Hendrix (Mausy5043) FEB2015:
- Added function computeDewPoint
- Added function computeDewPoint2
- Removed internal support of Fahrenheit. C2F and F2C conversions remain.
  Users is expected to convert Celsius to Fahrenheit when needed using
  the functions provided.
- placed return's outside switch{}

*/

#include "DHT.h"

DHT::DHT(uint8_t pin, uint8_t type, uint8_t count) {
  _pin = pin;
  _type = type;
  _count = count;
  firstreading = true;
}

void DHT::begin(void) {
  // set up the pins!
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  _lastreadtime = 0;
}

float DHT::readTemperature(void) {
  float f;

  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[2];
      break;
      //return f;
    case DHT22:
    case DHT21:
      f = data[2] & 0x7F;
      f *= 256;
      f += data[3];
      f /= 10;
      if (data[2] & 0x80)
      {
	       f *= -1;
      }
      break;
      //return f;
    }
    return f;
  }
  return NAN;
}

float DHT::convertCtoF(float c) {
	return c * 9 / 5 + 32;
}

float DHT::convertFtoC(float f) {
  return (f - 32) * 5 / 9;
}

float DHT::readHumidity(void) {
  float f;
  if (read()) {
    switch (_type) {
    case DHT11:
      f = data[0];
      break;
      //return f;
    case DHT22:
    case DHT21:
      f = data[0];
      f *= 256;
      f += data[1];
      f /= 10;
      break;
      //return f;
    }
    return f;
  }
  return NAN;
}

float DHT::computeHeatIndex(float tempCelsius, float percentHumidity) {
  return -8.784695 +
          1.61139411 * tempCelsius +
          2.33854900 * percentHumidity +
         -0.14611605 * tempCelsius*percentHumidity +
         -0.01230809 * pow(tempCelsius, 2) +
         -0.01642482 * pow(percentHumidity, 2) +
          0.00221173 * pow(tempCelsius, 2) * percentHumidity +
          0.00072546 * tempCelsius * pow(percentHumidity, 2) +
         -0.00000358 * pow(tempCelsius, 2) * pow(percentHumidity, 2);
}

float DHT::computeDewPoint(float tempCelsius, float percentHumidity) {
  // Source: http://nl.wikipedia.org/wiki/Dauwpunt
  float alpha = 17.271;
  float beta = 237.7; //degC
  float gamma = (alpha * tempCelsius) / (beta + tempCelsius) + log(percentHumidity * 0.01);
  return (beta * gamma) / (alpha - gamma);
}

float DHT::computeDewPoint2(float tempCelsius, float percentHumidity) {
  /*
  reference: http://wahiduddin.net/calc/density_algorithms.htm
  _
  USER_DEV:[GULIB.THERMOSRC]DWPT_TG.FOR;1
  _
        FUNCTION DWPT(T,RH)
  _
  C       INCLUDE 'LIB_DEV:[GUDOC]EDFVAXBOX.FOR/LIST'
  C       Baker, Schlatter  17-MAY-1982     Original version.
  _
  C   THIS FUNCTION RETURNS THE DEW POINT (CELSIUS) GIVEN THE TEMPERATURE
  C   (CELSIUS) AND RELATIVE HUMIDITY (%). THE FORMULA IS USED IN THE
  C   PROCESSING OF U.S. RAWINSONDE DATA AND IS REFERENCED IN PARRY, H.
  C   DEAN, 1969: "THE SEMIAUTOMATIC COMPUTATION OF RAWINSONDES,"
  C   TECHNICAL MEMORANDUM WBTM EDL 10, U.S. DEPARTMENT OF COMMERCE,
  C   ENVIRONMENTAL SCIENCE SERVICES ADMINISTRATION, WEATHER BUREAU,
  C   OFFICE OF SYSTEMS DEVELOPMENT, EQUIPMENT DEVELOPMENT LABORATORY,
  C   SILVER SPRING, MD (OCTOBER), PAGE 9 AND PAGE II-4, LINE 460.
  _
        X = 1.-0.01*RH
  _
  C   COMPUTE DEW POINT DEPRESSION.
  _
        DPD =(14.55+0.114*T)*X+((2.5+0.007*T)*X)**3+(15.9+0.117*T)*X**14
        DWPT = T-DPD
        RETURN
        END
  _
   TEMP  REL.HUM.   TD
   ----  -------- ------
    35    75.46   30.14
    25    38.77    9.93
     0    31.18  -15.19
    20    12.22  -10.16
    30    89.09   28.01
  */
  float inverseHumidity = 1 - 0.01 * percentHumidity;
  float intermediate = 0.0;
  intermediate = (14.55 + 0.114 * tempCelsius) * inverseHumidity;
  intermediate += pow(((2.5 + 0.007 * tempCelsius) * inverseHumidity), 3);
  intermediate += (15.9 + 0.117 * tempCelsius) * pow(inverseHumidity, 14);
  return (tempCelsius - intermediate);
}

boolean DHT::read(void) {
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  unsigned long currenttime;

  // Check if sensor was read less than two seconds ago and return early
  // to use last reading.
  currenttime = millis();
  if (currenttime < _lastreadtime) {
    // ie there was a rollover
    _lastreadtime = 0;
  }
  if (!firstreading && ((currenttime - _lastreadtime) < 2000)) {
    return true; // return last correct measurement
    //delay(2000 - (currenttime - _lastreadtime));
  }
  firstreading = false;
  /*
    Serial.print("Currtime: "); Serial.print(currenttime);
    Serial.print(" Lasttime: "); Serial.print(_lastreadtime);
  */
  _lastreadtime = millis();

  data[0] = data[1] = data[2] = data[3] = data[4] = 0;

  // pull the pin high and wait 250 milliseconds
  digitalWrite(_pin, HIGH);
  delay(250);

  // now pull it low for ~20 milliseconds
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);
  noInterrupts();
  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);

  // read in timings
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (digitalRead(_pin) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = digitalRead(_pin);

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      data[j/8] <<= 1;
      if (counter > _count)
        data[j/8] |= 1;
      j++;
    }

  }

  interrupts();

  /*
  Serial.println(j, DEC);
  Serial.print(data[0], HEX); Serial.print(", ");
  Serial.print(data[1], HEX); Serial.print(", ");
  Serial.print(data[2], HEX); Serial.print(", ");
  Serial.print(data[3], HEX); Serial.print(", ");
  Serial.print(data[4], HEX); Serial.print(" =? ");
  Serial.println(data[0] + data[1] + data[2] + data[3], HEX);
  */

  // check we read 40 bits and that the checksum matches
  if ((j >= 40) &&
      (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
    return true;
  }

  return false;

}
