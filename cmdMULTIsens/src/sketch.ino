/*
  cmdMULTI sketch

*/


#include <avr/pgmspace.h>
#include "ChipTemp.h"

#include <VBAT.h>
#include <TMP36.h>
#include <DHT.h>
#include <OneWire.h>
#include <BMP183.h>

// Pins used for BMP183 via SPI
#define BMP183_CLK  13  // Clock
#define BMP183_SDO  12  // SPI Digital Out aka MISO
#define BMP183_SDI  11  // SPI Digital In aka MOSI
#define BMP183_CS   10  // ChipSelect pin

// An LED is connected to pin 3
#define ActivityLED 3

// measurement pin of DHT22 is connected to digital pin 4
#define Dht22Pin 4
#define DhtType DHT22
// number of samples to be averaged (not used!)
#define Dht22Samples 9

// measurement pin of the voltage measurement circuit is connected to analog pin 0
#define VbatPin A0
// number of samples to be averaged
#define VbatSamples 9

// measurement pin of TMP36 is connected to analog pin 1
#define Tmp36Pin A1
// number of samples to be averaged
#define Tmp36Samples 9

// measurement pin of DS18B20 is connected to pin 9
#define DS18Pin 9
// number of samples to be averaged
#define DS18Samples 9

// ATMEGA Chip is internal
// number of samples to be averaged
#define ATSamples 9

// Initialise DHT sensor: measurement pin, sensortype
DHT dht(Dht22Pin, DhtType);

// Initialise VBAT library: measurement pin, number of samples to average,
//        +V reference [V], R1 [Ohm], R2 [Ohm]
VBAT vbat(VbatPin, VbatSamples, 5.0, 99300.0,  9870.0);

// Initialise TMP36 library: measurement pin, number of samples to average, +V reference
TMP36 tmp36(Tmp36Pin, Tmp36Samples, 5.0);

// Initialise DS18B20 library: measurement pin, number of samples to average
//DS18B20 ds1w(DS18Pin, DS18Samples);
OneWire ds1w(DS18Pin);

// Initialise internal temperature
ChipTemp chipTemp(ATSamples);

// initialize with hardware SPI
//BMP183 bmp = BMP183(BMP183_CS);
// or initialize with software SPI and use any 4 pins
BMP183 bmp = BMP183(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);

float h;  // used for calculating dewpoint
float t;  // used for calculating dewpoint
byte ds1w_addr[8];
byte ds1w_data[12];

int SerialRequestCounter;     // For debugging purposes

void setup()
{
  pinMode(ActivityLED, OUTPUT);     // An LED to signal activity
  digitalWrite(ActivityLED, HIGH);  // Turn the LED on during setup()
  Serial.begin(9600);               // Initialise serialport
  Serial.print("... ");

  tmp36.begin();                    // Initialise TMP36 sensor
  vbat.begin();                     // Initialise VBAT sensor
  dht.begin();                      // Initialise DHT22 sensor
  bmp.begin();                      // Initialise BMP183 sensor
  if ( !ds1w.search(ds1w_addr))
  {
    Serial.println("No sensor.");
  }

  delay(2000);                      // Wait 2s for all sensors to come online
  Serial.println(" cmdMULTIsens ready !");   // Print banner
  SerialRequestCounter = 0;         // For debugging purposes
  digitalWrite(ActivityLED, LOW);   // Turn off the LED at end of setup()
}

int serialRX()
{
  return Serial.read();             // read a byte from the serialbuffer
}

float readDSTemperature(void)
{
  //int cntSamples = 0;
  // int sumSamples = 0;
  //float measurement = 0.0;
  //int i = 0;
  int type_s = 0;

  ds1w.reset();
  ds1w.select(ds1w_addr);
  ds1w.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  byte present = ds1w.reset();
  ds1w.select(ds1w_addr);
  ds1w.write(0xBE);         // Read Scratchpad

  //Serial.print("  Data = ");
  //Serial.print(present, HEX);
  //Serial.print(" ");
  for ( int i = 0; i < 9; i++) {           // we need 9 bytes
    ds1w_data[i] = ds1w.read();
    //Serial.print(ds1w_data[i], HEX);
    //Serial.print(" ");
  }
  //Serial.print(" CRC=");
  //Serial.print(OneWire::crc8(ds1w_data, 8), HEX);
  //Serial.println();

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (ds1w_data[1] << 8) | ds1w_data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (ds1w_data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - ds1w_data[6];
    }
  } else {
    byte cfg = (ds1w_data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  float celsius = (float)raw / 16.0;
  //Serial.print("  Temperature = ");
  //Serial.print(celsius);
  //Serial.println(" Celsius, ");

/*
  // add up the pre-defined number of DS18Samples for Sample Averaging
  for (cntSamples = 0; cntSamples < DS18Samples; cntSamples++)
  {
    //sumSamples += analogRead(_pin);
    // minimum delay on analog pins is 100ms
    // delay(110);
  }
  //measurement = (float)sumSamples * _invsamples; // Calculate avg raw value.
  //measurement = map(measurement * 10, 0, 10230, 0, _ref5v * 10000) * 0.0001;
  //measurement *= _r12;
  //return measurement;
*/
  return celsius;
}

void loop()
{
  byte ActionRequest;
  float Value;

  if (Serial.available() > 0)
  {
    digitalWrite(ActivityLED, HIGH);      // signal activity detected
    SerialRequestCounter += 1;            // For debugging purposes

    ActionRequest = serialRX();           // check the input

    Serial.print((char)ActionRequest);    // start of telegram
    Serial.print(" ");                    // and delimiter
    switch (ActionRequest)
    {
      case '?':
        Serial.println(" ");
        Serial.println("cmdMULTIsens help is underway!");
        Serial.println(" ");
        Serial.println("A | a : All sensor and calculated values (C,W,S,H,D,E,I,V,T,P,Q)");
        Serial.println("C | c : ATMEGA chip temperature");
        Serial.println("D | d : DHT22 calculated Dewpoint");
        Serial.println("E | e : DHT22 calculated Dewpoint2");
        Serial.println("H | h : DHT22 humidity");
        Serial.println("I | i : DHT22 calculated Heat index");
        Serial.println("P | p : BMP183 barometric pressure");
        Serial.println("Q | q : BMP183 temperature");
        Serial.println("R | r : DHT22 all sensor data (S,H,D,E,I)");
        Serial.println("S | s : DHT22 temperature");
        Serial.println("T | t : TMP36 temperature");
        Serial.println("V | v : VBAT voltage");
        Serial.println("W | w : DS18B20 (1-wire) temperature");
        break;
      case 'C':
      case 'c':
        // ATMEGA temperature
        Value = chipTemp.readTemperature();
        Serial.print(Value);
        break;
      case 'D':
      case 'd':
        // DHT22 calculated dewpoint
        Value = dht.computeDewPoint2(readDSTemperature(), dht.readHumidity());
        Serial.print(Value);
        break;
      case 'E':
      case 'e':
        // DHT22 calculated dewpoint (method 2)
        Value = dht.computeDewPoint2(dht.readTemperature(), dht.readHumidity());
        Serial.print(Value);
        break;
      case 'H':
      case 'h':
        // DHT22 humidity
        Value = dht.readHumidity();
        Serial.print(Value);
        break;
      case 'I':
      case 'i':
        // DHT calculated HeatIndex
        Value = dht.computeHeatIndex(dht.readTemperature(), dht.readHumidity());
        Serial.print(Value);
        break;
      case 'A':
      case 'a':
      case 'R':
      case 'r':
        if ((ActionRequest == 'A') || (ActionRequest == 'a'))
        {
          // ATMEGA temperature
          Value = chipTemp.readTemperature();
          Serial.print(Value);
          Serial.print(", ");
          // DS18B20 temperature
          Value = readDSTemperature();
          Serial.print(Value);
          Serial.print(", ");
        }
        // All DHT22 data: Temperature, Humidity, DewPoint, DewPoint2, HeatIndex
        t = dht.readTemperature();
        Serial.print(t);
        Serial.print(", ");
        h = dht.readHumidity();
        Serial.print(h);
        Serial.print(", ");
        Value = dht.computeDewPoint(t, h);
        Serial.print(Value);
        Serial.print(", ");
        Value = dht.computeDewPoint2(t, h);
        Serial.print(Value);
        Serial.print(", ");
        Value = dht.computeHeatIndex(t, h);
        Serial.print(Value);
        if ((ActionRequest == 'A') || (ActionRequest == 'a'))
        {
          // VBAT voltage
          Serial.print(", ");
          Value = vbat.readVoltage();
          Serial.print(Value);
          // TMP36 temperature
          Value = tmp36.readTemperature();
          Serial.print(", ");
          Serial.print(Value);
          // BMP183 pressure, temperature
          Value = bmp.getPressure() * 0.01;
          Serial.print(", ");
          Serial.print(Value);
          Value = bmp.getTemperature();
          Serial.print(", ");
          Serial.print(Value);
          Serial.print(", ");
          Serial.print(SerialRequestCounter);
        }
        break;
      case 'P':
      case 'p':
        // BMP183 barometric pressure
        Value = bmp.getPressure() * 0.01;   // millibars
        Serial.print(Value);
        break;
      case 'Q':
      case 'q':
        // BMP183 temperature
        Value = bmp.getTemperature();
        Serial.print(Value);
        break;
      case 'S':
      case 's':
        // DHT22 temperature
        Value = dht.readTemperature();
        Serial.print(Value);
        break;
      case 'T':
      case 't':
        // TMP36 temperature
        Value = tmp36.readTemperature();
        Serial.print(Value);
        break;
      case 'V':
      case 'v':
        // VBAT voltage
        Value = vbat.readVoltage();
        Serial.print(Value);
        break;
      case 'W':
      case 'w':
        // DS18B20 temperature
        Value = readDSTemperature();
        Serial.print(Value);
        break;
      case 'Z':
      case 'z':
        // Number of serial requests
        // For debugging purposes
        Serial.print(SerialRequestCounter);
        break;
      default:
        Serial.print("NaN");          // Invalid ActionRequest returns `NaN`
        break;
    }
    Serial.println(" !");             // Signal end of telegram

    digitalWrite(ActivityLED, LOW);   // end of activity
  }
}
