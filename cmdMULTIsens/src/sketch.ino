/*
  cmdMULTI sketch

*/

#include <TMP36.h>
#include <DHT.h>

// An LED is connected to pin 3
#define ActivityLED 3
// measurement pin of DHT22 is connected to digital pin 4
#define Dht22Pin 4
#define DhtType DHT22
#define Dht22Samples 3

// measurement pin of TMP36 is connected to analog pin 1
#define Tmp36Pin A1
// number of samples to be averaged
#define Tmp36Samples 3

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(Dht22Pin, DhtType);
// Initialise TMP36 library
TMP36 tmp36(Tmp36Pin, Tmp36Samples);

void setup()
{
  pinMode(ActivityLED, OUTPUT);     // An LED to signal activity
  digitalWrite(ActivityLED, HIGH);  // Turn the LED on during setup()
  Serial.begin(9600);               // Initialise serialport
  Serial.println("cmdMULTIsens");   // Print banner
  tmp36.begin();                    // Initialise TMP36 sensor
  dht.begin();                      // Initialise DHT22 sensor
  delay(2000);                      // Wait 2s for all sensors to come online
  digitalWrite(ActivityLED, LOW);   // Turn off the LED at end of setup()
}

int serialRX()
{
  return Serial.read();             // read a byte from the serialbuffer
}

void loop()
{
  byte ActionRequest;
  float Value;

  if (Serial.available() > 0)
  {
    digitalWrite(ActivityLED, HIGH);      // signal activity detected

    ActionRequest = serialRX();           // check the input

    Serial.print((char)ActionRequest);    // start of telegram
    Serial.print(" ");                    // and delimiter
    switch (ActionRequest)
    {
      case '?':
        Serial.println(" ");
        Serial.println("cmdMULTIsens help is underway!");
        Serial.println(" ");
        Serial.println("H | h : DHT22 humidity");
        Serial.println("T | t : TMP36 temperature");
        break;
      case 'H':
      case 'h':
        // DHT22 humidity
        Value = dht.readHumidity();
        Serial.print(Value);
        break;
      case 'T':
      case 't':
        // TMP36 temperature
        Value = tmp36.readTemperature();
        Serial.print(Value);
        break;
      default:
        Serial.print("NaN");          // Invalid ActionRequest returns `NaN`
        break;
    }
    Serial.println(" !");             // Signal end of telegram

    digitalWrite(ActivityLED, LOW);   // end of activity
  }
}
