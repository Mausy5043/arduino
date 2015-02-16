/*
  sensTMP36 sketch
  measures the voltage on analog pin
  converts the result to a temperature
  and prints the results to the serial port
*/

// measurement pin from TMP36 is connected to analog pin 1
#define measurePin A1
#define activityLED 3

// *** declare constants
const float ref5V = 5.06;   // reference: 5.0V on measurePin == 1023.0


// (numSamples ^ 0.5) is the S/N ratio that can be achieved.
// To get a S/N-ratio of 3 requires 9 samples. S/N=4 requires 16 samples
// More samples means longer measurement times.
// Beware, numSamples * 1024 should not exceed the positive maximum for the
// `int` data-type!
// Therefore the maximum numSamples = (32767 / 1023 = ) 32
const int numSamples = 10;  // number of measurements used for one result

// *** declare variables
int sumSamples = 0;         // sum of samples
int cntSamples = 0;         // sample counter
float voltage = 0.0;
float temperature = 0.0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup()
{
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  digitalWrite(activityLED, HIGH);
  Serial.begin(9600);             // Initialise serial port
  delay(2000);
  Serial.println("sensTMP36");
  digitalWrite(activityLED, LOW);
}

void loop()
{
  startTime = millis();
  digitalWrite(activityLED, HIGH);
  sumSamples = 0;

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (cntSamples = 0; cntSamples < numSamples; cntSamples++) {
    sumSamples += analogRead(measurePin);
    // minimum delay on anaolg pins is 100ms
    delay(200);
  }

  // *** Determine the source voltage:
  voltage = (float)sumSamples / (float)cntSamples; // Calculate avg raw value.
  voltage = map(voltage * 10, 0, 10230, 0, ref5V * 10000) * 0.0001;
  temperature = 100 * voltage - 50.0;

  Serial.print(voltage,5);
  Serial.print(" V ; ");
  Serial.print(temperature);
  Serial.println(" degC");
  digitalWrite(activityLED, LOW);
  elapsedTime = millis() - startTime;

  // *** Wait until 5sec have passed.
  delay(abs(5000 - elapsedTime));
}
