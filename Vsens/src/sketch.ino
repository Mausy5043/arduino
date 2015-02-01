/*--------------------------------------------------------------
  Adapted from : volt_measure
                 22 May 2013 by W.A. Smith, http://startingelectronics.com

  Modified by  : M. Hendrix
--------------------------------------------------------------*/

// number of samples per measurement
#define numSamples 10
#define measurePin A0

int sumSamples = 0;             // sum of samples taken
int cntSamples = 0;             // current sample number
float voltage = 0.0;            // calculated voltage

const float ref5V = 5.000;     // calibration of reference value for 5.0V ADC        


void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // take a number of analog samples and add them up
    for (cntSamples = 0; cntSamples <= numSamples; cntSamples++) {
        sumSamples += analogRead(measurePin);
        delay(10);
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = ((float)sumSamples / (float)numSamples * ref5V) / 1024.0;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value
    Serial.print(voltage * 11.000);
    Serial.println (" V");
    cntSamples = 0;
    sumSamples = 0;
}


/*
  DisplayMoreThan5V sketch
  prints the voltage on analog pin to the serial port
  Do not connect more than 5 volts directly to an Arduino pin.
+/
const float referenceVolts = 5;        // the default reference on a 5-volt board

const float R1 = 1000; // value for a maximum voltage of 10 volts
const float R2 = 1000;
// determine by voltage divider resistors, see text
const float resistorFactor = 1023.0 * (R2/(R1 + R2));
const int batteryPin = A0;         // +V from battery is connected to analog pin 0

const int led[6] = {3, 5, 6, 9, 10, 11};
const int sensPin = A0;
const int maxoutput = 191;
const float factor = (6.0*maxoutput)/1024.0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i<=5; i++) {
    pinMode(led[i],OUTPUT);
    digitalWrite(led[i],HIGH);
  }
  delay(500);
  ledsoff();
}

void loop() {

   int val = analogRead(sensPin);  // read the value from the sensor
   Serial.print(val);
   Serial.print(" = ");
   float volts = (val / resistorFactor) * referenceVolts ; // calculate the ratio
   Serial.println(volts);  // print the value in volts
  ledon(val * factor);
  /* pulse
  for (int i = 0; i <= 6*maxoutput; i++) {
    ledon(i);
    //delay(0.5);
  }
  for (int i = 6*maxoutput; i >= 0; i--) {
    ledon(i);
    delay(1);
  }
  +/
  delay(1000);
}

void ledsoff() {
  for (int i = 0; i<=5; i++) {
    digitalWrite(led[i],LOW);
  }
}

void ledon(int v) {
  int w;
  //Serial.print(v);
  //Serial.print(": ");
  if (v > 6* maxoutput) { v=6*maxoutput; }

  for (int out=5; out >= 0; out = out -1) {
    if (v > out * maxoutput) {
      w = v - out * maxoutput;
   //     Serial.print(w);
   //     Serial.print(" ");
      analogWrite(led[out], w);
      v = v- w;
    }
    else { analogWrite(led[out],0); }
  }
  //Serial.println(".");
}

*/
