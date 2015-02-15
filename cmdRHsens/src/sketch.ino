// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain


// Dewpoint:  http://www.dpcalc.org

#include <DHT.h>

#define DHTPIN 4     // what pin we're connected to
#define activityLED 13

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302) purchased JAN2015
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);


const int NUM_SAMPLES = 5;  // number of measurements used for one result

// *** declare variables
float ReturnedValue = 0.0;
byte ActionRequest;

void setup() {
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  digitalWrite(activityLED, HIGH);
  delay(2000);
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  digitalWrite(activityLED, LOW);
}

void loop() {
  if (Serial.available() > 0)
  {
    digitalWrite(ActivityLED, HIGH);  // signal activity detected

    ActionRequest = serialRX();                 // See what the input is

    Serial.print((char)ActionRequest);          // Signal start of telegram
    Serial.print(" ");                // Delimiter
    switch (ActionRequest)
    {
      case 'H':
      case 'h':
        ReturnedValue = dht.readHumidity();
        Serial.print(ReturnedValue);        // AverageValue
        break;
      case 'T':
      case 't':
        ReturnedValue = dht.readTemperature();
        Serial.print(ReturnedValue);        // AverageValue
        break;
      case 'D':
      case 'd':
        ReturnedValue = computeDewPoint( dht.readTemperature(), dht.readHumidity() );
        Serial.print(ReturnedValue);        // AverageValue
        break;
      default:
        Serial.print("NaN");          // Invalid ActionRequest returns `NaN`
    }
    Serial.println(" !");             // Signal end of telegram

    digitalWrite(ActivityLED, LOW);   // end of activity
  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  // float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute dewpoint
  float dpc = dht.computeDewPoint(t, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" % \t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C \t");
  Serial.print("Dewpoint: ");
  Serial.print(dpc);
  Serial.println(" *C");
}
