/*
  Vsens sketch
  measures the voltage on analog pin and prints the result to the serial port
  Do not connect more than 5 volts directly to an Arduino pin!!
*/

// +V from battery is connected to analog pin 0
#define measurePin A0
#define activityLED 13

// *** declare constants
const float ref5V = 5.14;   // reference: 5.0V on measurePin == 1023.0
const float R1 = 100000.0;  // Resistor value used for R1
const float R2 = 10000.0;   // Resistor value used for R2

// (numSamples ^ 0.5) is the S/N ratio that can be achieved.
// To get a S/N-ratio of 3 requires 9 samples. S/N=4 requires 16 samples
// More samples means longer measurement times.
// Beware, numSamples * 1024 should not exceed the positive maximum for the
// `int` data-type!
// Therefore the maximum numSamples = (32767 / 1023 = ) 32
const int numSamples = 16;  // number of measurements used for one result

// *** declare calculated constants
const float ratioRes = (R2/(R1 + R2));
const float scaleRaw2Volts = ref5V / (ratioRes * 1023.0);

// *** declare variables
int sumSamples = 0;         // sum of samples
int cntSamples = 0;         // sample counter
float voltage = 0.0;
unsigned long startTime;
unsigned long elapsedTime;

void setup()
{
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  Serial.begin(9600);             // Initialise serial port
}

void loop()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read()
    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
}
