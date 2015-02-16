/*
  sensTMP36 sketch
  measures the voltage on analog pin and prints the result to the serial port
  Do not connect more than 5 volts directly to an Arduino pin!!
*/

// +V from battery is connected to analog pin 0
#define measurePin A1
#define activityLED 13

// *** declare constants
const float ref5V = 5.14;   // reference: 5.0V on measurePin == 1023.0


// (numSamples ^ 0.5) is the S/N ratio that can be achieved.
// To get a S/N-ratio of 3 requires 9 samples. S/N=4 requires 16 samples
// More samples means longer measurement times.
// Beware, numSamples * 1024 should not exceed the positive maximum for the
// `int` data-type!
// Therefore the maximum numSamples = (32767 / 1023 = ) 32
const int numSamples = 16;  // number of measurements used for one result

// *** declare calculated constants
const float scaleRaw2Volts = ref5V / 1023.0;

// *** declare variables
int sumSamples = 0;         // sum of samples
int cntSamples = 0;         // sample counter
float voltage = 0.0;
float temperature = 0.0;
unsigned long startTime;
unsigned long elapsedTime;

void setup()
{
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  Serial.begin(9600);             // Initialise serial port
}

void loop()
{
  startTime = millis();
  digitalWrite(activityLED, HIGH);
  sumSamples = 0;

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (cntSamples = 0; cntSamples <= numSamples; cntSamples++) {
    sumSamples += analogRead(measurePin);
    delay(10);
  }

  // *** Determine the source voltage:
  voltage = (float)sumSamples / (float)numSamples; // Calculate avg raw value.
  voltage *= scaleRaw2Volts;      // Scale avg raw value to source voltage.
  temperature = 100 * voltage - 50.0

  Serial.print(voltage);
  Serial.print(" V ; ");
  Serial.print(temperature);
  Serial.println(" degC");
  digitalWrite(activityLED, LOW);
  elapsedTime = millis() - startTime;

  // *** Wait until 5sec have passed.
  delay(abs(5000 - elapsedTime));
}
