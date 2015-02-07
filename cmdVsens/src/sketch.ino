/*
  cmdVsens sketch
  Waits for a request on the serialport.
  Then measures the voltage on measurePin and prints the result to the serialport

  *******
  Do not connect more than 5 volts directly to an Arduino pin!!
  *******
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
byte byteIn = 0;
byte iB = 0;

void setup()
{
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  Serial.begin(9600);             // Initialise serial port
}

int serialRX()
{
  // read a a byte from the serialbuffer
  iB = Serial.read();
}

void serialTX(byte oB)
{
  if (oB == 13)
  {
    Serial.write(11);
  }
  Serial.write(oB);
}

float sensV()
{
  sumSamples = 0;

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (cntSamples = 0; cntSamples <= numSamples; cntSamples++)
  {
    sumSamples += analogRead(measurePin);
    delay(10);
  }

  // *** Determine the source voltage:
  voltage = (float)sumSamples / (float)numSamples; // Calculate avg raw value.
  voltage *= scaleRaw2Volts;      // Scale avg raw value to source voltage.

}

void loop()
{
  if (Serial.available() > 0)
  {
    digitalWrite(activityLED, HIGH); // signal activity detected

    byteIn = serialRX(); // see what the input is
    voltage = sensV();
    Serial.println("/"); // Signal start of telegram
    Serial.print(voltage);
    Serial.println(" V");
    Serial.println("!"); // Signal end of telegram

    digitalWrite(activityLED, LOW); // end of activity
  }
}
