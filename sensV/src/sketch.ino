/*
  sensV sketch
  Measures the voltage on the analog pin and prints the result to
  the serialport

  *******
  Do not connect more than 5 volts directly to an Arduino pin!!
  *******
*/

// +V from battery is connected to analog pin 0
#define VoltMeasurePIN A0
#define activityLED 3

// *** declare constants
const float ref5V = 5.14;   // reference: 5.14V on measurePin == 1023.0
//const float R1 = 100000.0;  // Resistor value used for R1
const float R1 = 99300.0;  // Measured resistor value used for R1
//const float R2 = 10000.0;   // Resistor value used for R2
const float R2 = 9870.0;   // Measured resistor value used for R2

// (NUM_SAMPLES ^ 0.5) is the S/N ratio that can be achieved.
// To get a S/N-ratio of 3 requires 9 samples. S/N=4 requires 16 samples
// More samples means longer measurement times.
// Beware, NUM_SAMPLES * 1024 should not exceed the positive maximum for the
// `int` data-type!
// Therefore the maximum NUM_SAMPLES = (32767 / 1023 = ) 32
const int NUM_SAMPLES = 16;  // number of measurements used for one result

// *** declare calculated constants
const float RATIO_RESISTORS = ((R1 + R2)/R2) * 0.00001; // multiply by 1e-5 to get V

// *** declare variables
float voltage = 0.0;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup()
{
  pinMode(activityLED, OUTPUT);   // An LED to signal activity
  digitalWrite(activityLED, HIGH);
  Serial.begin(9600);             // Initialise serial port
  delay(2000);
  Serial.println("sensV");
  digitalWrite(activityLED, LOW);
}

float sensV()
{
  float AverageValue = 0.0;     // result
  int SumSamples = 0;           // sum of samplevalues
  int CountSamples = 0;         // sample counter

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (CountSamples = 0; CountSamples < NUM_SAMPLES; CountSamples++)
  {
    SumSamples += analogRead(VoltMeasurePIN);
    delay(200);
  }

  // *** Determine the source AverageValue:
  AverageValue = (float)SumSamples / (float)CountSamples; // Calculate avg raw value.
  AverageValue = map(AverageValue * 10, 0, 1023, 0, ref5V * 10000); //map raw value to 0...5V
  AverageValue *= RATIO_RESISTORS; // Account for voltage-reduction
  return AverageValue;
}

void loop()

{
  startTime = millis();
  digitalWrite(activityLED, HIGH);

  voltage = sensV();

  Serial.print(voltage);
  Serial.println(" V");
  digitalWrite(activityLED, LOW);
  elapsedTime = millis() - startTime;

  // *** Wait until 5sec have passed.
  delay(abs(5000 - elapsedTime));
}
