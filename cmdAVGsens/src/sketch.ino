/*
  cmdAVGsens sketch

  Measuring and self-averaging

*/


/*
    $$$ ATMEGA silicon temperature measurement
*/
#include <avr/pgmspace.h>
#include "ChipTemp.h"
#define ATSamples 9                 // number of samples to be averaged
ChipTemp chipTemp(ATSamples);       // Initialise internal temperature measurement


/*
    $$$ Voltage measurement using a voltage-divider circuit with resistors
*/
#include <VBAT.h>
#define VbatPin A0                  // Connected to analog pin 0
#define VbatSamples 9               // number of samples to be averaged
// Initialise VBAT library: measurement pin, number of samples to average,
//        +V reference [V], R1 [Ohm], R2 [Ohm]
VBAT vbat(VbatPin, VbatSamples, 5.0, 99300.0,  9870.0);


/*
    $$$ Temperature measurement (on-board) using a TMP36 sensor
*/
#include <TMP36.h>
#define Tmp36Pin A1                 // Connected to analog pin 1
#define Tmp36Samples 9              // number of samples to be averaged
// Initialise TMP36 library: measurement pin, number of samples to average, +V reference
TMP36 tmp36(Tmp36Pin, Tmp36Samples, 5.0);


/*
    $$$ RH/T measurement using a DHT22 sensor
*/
#include <DHT.h>
#define Dht22Pin 4                  // Connected to digital pin 4
#define DhtType DHT22               // Sensor-type used
#define Dht22Samples 9              // number of samples to be averaged (not used!)
DHT dht(Dht22Pin, DhtType);         // Initialise DHT sensor: measurement pin, sensortype
float h;                            // used for calculating dewpoint
float t;                            // used for calculating dewpoint


/*
    $$$ Temperature measurement using a DS18B20 sensor
*/
#include <OneWire.h>
#define DS18Pin 9                   // Connected to pin 9
#define DS18Samples 9               // number of samples to be averaged
byte ds1w_addr[8];                  // claim storage space for sensor id
byte ds1w_data[12];                 // claim storage space for data
OneWire ds1w(DS18Pin);              // Initialise DS18B20 library


/*
    $$$ p/T measurement using a BMP183 sensor
*/
#include <BMP183.h>
// Pins used for BMP183 via SPI
#define BMP183_CLK  13              // Clock
#define BMP183_SDO  12              // SPI Digital Out aka MISO
#define BMP183_SDI  11              // SPI Digital In aka MOSI
#define BMP183_CS   10              // ChipSelect pin
// initialize with hardware SPI
//BMP183 bmp = BMP183(BMP183_CS);
// or initialize with software SPI and use any 4 pins
BMP183 bmp(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);

/*
    Miscelaneous
*/
#define ActivityLED 3               // An LED is connected to pin 3
int SampleCounter;                  // For debugging purposes

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
  Serial.println(" cmdAVGsens ready !");   // Print banner
  SampleCounter = 0;                // For debugging purposes
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
  ds1w.write(0x44, 1);              // start conversion, with parasite power on at the end

  delay(1000);                      // maybe 750ms is enough, maybe not
  byte present = ds1w.reset();
  ds1w.select(ds1w_addr);
  ds1w.write(0xBE);                 // Read Scratchpad

  for ( int i = 0; i < 9; i++) {    // we need 9 bytes
    ds1w_data[i] = ds1w.read();
  }

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
    digitalWrite(ActivityLED, HIGH);  // signal activity detected
    SampleCounter += 1;               // For debugging purposes

    ActionRequest = serialRX();       // check the input

    Serial.print((char)ActionRequest);// start of telegram
    Serial.print(" ");                // and delimiter
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
          Serial.print(SampleCounter);
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
        Serial.print(SampleCounter);
        break;
      default:
        Serial.print("NaN");       // Invalid ActionRequest returns `NaN`
        break;
    }
    Serial.println(" !");          // Signal end of telegram

    digitalWrite(ActivityLED, LOW);// end of activity
  }
}
