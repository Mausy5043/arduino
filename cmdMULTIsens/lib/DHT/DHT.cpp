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

float DHT::computeHeatIndex(float tempCelsius, float percentHumidity)
{
  float t = convertCtoF(tempCelsius);
  float rh = percentHumidity;
  float hi = -100.0;
  float adj = 0.0;

  /*  Source: http://www.hpc.ncep.noaa.gov/html/heatindex_equation.shtml
  The Heat Index Equation

  The computation of the heat index is a refinement of a result obtained by
  multiple regression analysis carried out by Lans P. Rothfusz and described in a
  1990 National Weather Service (NWS) Technical Attachment (SR 90-23).
  */
  /*
  The Rothfusz regression is not appropriate when conditions of temperature and
  humidity warrant a heat index value below about 80 degrees F. In those cases, a
  simpler formula is applied to calculate values consistent with Steadman's
  results:

  HI = 0.5 * {T + 61.0 + [(T-68.0)*1.2] + (RH*0.094)}
  */

  hi = rh * 0.094;
  hi += ((t - 68.0) * 1.2);
  hi += 61.0;
  hi +=  t;
  hi *= 0.5;

  /*
  In practice, the simple formula is computed first and the result averaged with
  the temperature. If this heat index value is 80 degrees F or higher, the full
  regression equation along with any adjustment as described below is applied. The
  Rothfusz regression is not valid for extreme temperature and relative humidity
  conditions beyond the range of data considered by Steadman.
  */

  hi = ((hi + t) * 0.5);
  if (hi >= 80.0)
  {
    /*
    The regression equation of Rothfusz is:

    HI = -42.379 + 2.04901523*T + 10.14333127*RH - .22475541*T*RH - .00683783*T*T
    - .05481717*RH*RH + .00122874*T*T*RH + .00085282*T*RH*RH - .00000199*T*T*RH*RH

    where T is temperature in degrees F and RH is relative humidity in percent.  HI
    is the heat index expressed as an apparent temperature in degrees F.
    */

    hi = -42.379;
    hi += 2.04901523 * t;
    hi += 10.14333127 * rh;
    hi -= 0.22475541 * t * rh;
    hi -= 0.00683783 * t * t;
    hi -= 0.05481717 * rh * rh;
    hi += 0.00122874 * t * t * rh;
    hi += 0.00085282 * t * rh * rh;
    hi -= 0.00000199 * t * t * rh * rh;

    /*
    If the RH is less than 13% and the temperature is between 80 and 112 degrees F,
    then the following adjustment is subtracted from HI:

    ADJUSTMENT = [(13-RH)/4]*SQRT{[17-ABS(T-95.)]/17}

    where ABS and SQRT are the absolute value and square root functions,
    respectively.
    */

    if ((rh < 13.0) && (t >= 80.0) && (t <= 112.0))
    {
      adj = abs(t - 95.0);
      adj = (17.0 - adj);
      adj *= 0.05882352941; // 1/17
      adj = sqrt(adj);
      adj *= ((13.0 - rh) * 0.25);
    }

    /*
    On the other hand, if the RH is greater than 85% and the
    temperature is between 80 and 87 degrees F, then the following adjustment is
    added to HI:

    ADJUSTMENT = [(RH-85)/10] * [(87-T)/5]
    */

    if ((rh > 85.0) && (t >= 80.0) && (t <= 87.0))
    {
      adj = rh - 85.0;
      adj *= 0.1;
      adj *= ((87.0 - t) * 0.2);
    }

    hi += adj;
  }
  
  return = convertFtoC(hi);
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
