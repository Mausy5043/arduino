/*
  cmdVsens sketch
  Waits for a request on the serialport.
  Then measures the AverageValue on VoltMeasurePIN and prints the result to the serialport

  *******
  Do not connect more than 5 volts directly to an Arduino pin!!
  *******
*/

// +V from battery is connected to analog pin 0
#define VoltMeasurePIN A0
#define activityLED 3

// *** declare constants
const float REF5V = 5.00;   // reference: 5.0V on VoltMeasurePIN == 1023.0
const float R1 = 100000.0;  // Resistor value used for R1
const float R2 = 10000.0;   // Resistor value used for R2

// (NUM_SAMPLES ^ 0.5) is the S/N ratio that can be achieved.
// To get a S/N-ratio of 3 requires 9 samples. S/N=4 requires 16 samples
// More samples means longer measurement times.
// Beware, NUM_SAMPLES * 1024 should not exceed the positive maximum for the
// `int` data-type!
// Therefore the maximum NUM_SAMPLES = (32767 / 1023 = ) 32
const int NUM_SAMPLES = 16;  // number of measurements used for one result

// *** declare calculated constants
const float RATIO_RESISTOR = (R2/(R1 + R2));
const float SCALE_RAW_TO_VOLTS = REF5V / (RATIO_RESISTOR * 1023.0);

// *** declare variables
float ReturnedValue = 0.0;
byte ActionRequest;

void setup()
{
  pinMode(ActivityLED, OUTPUT);   // An LED to signal activity
  Serial.begin(9600);             // Initialise serial port
}

int serialRX()
{
  // read a a byte from the serialbuffer
  return Serial.read();
}

float sensV() // measure the AverageValue
{
  float AverageValue = 0.0;        // result
  int SumSamples = 0;         // sum of samplevalues
  int CountSamples = 0;         // sample counter

  // *** Add up the pre-defined number of samples for Sample Averaging
  for (CountSamples = 0; CountSamples < NUM_SAMPLES; CountSamples++)
  {
    SumSamples += analogRead(VoltMeasurePIN);
    delay(10);
  }

  // *** Determine the source AverageValue:
  AverageValue = (float)SumSamples / (float)CountSamples; // Calculate avg raw value.
  AverageValue *= SCALE_RAW_TO_VOLTS;      // Scale avg raw value to source AverageValue.
  return AverageValue;

}

void loop()
{
  if (Serial.available() > 0)
  {
    digitalWrite(activityLED, HIGH);  // signal activity detected

    ActionRequest = serialRX();                 // See what the input is

    Serial.print((char)ActionRequest);          // Signal start of telegram
    Serial.print(" ");                // Delimiter
    switch (ActionRequest)
    {
      case 'V':
      case 'v':
        ReturnedValue = sensV();
        Serial.print(ReturnedValue);        // AverageValue
        break;
      default:
        Serial.print("NaN");          // Invalid ActionRequest returns `NaN`
    }
    Serial.println(" !");             // Signal end of telegram

    digitalWrite(activityLED, LOW);   // end of activity
  }
}
