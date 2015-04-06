/*
  sensChipTemp
  Measures the on-chip temperature of the ATMEGA328

  original code: http://playground.arduino.cc/Main/InternalTemperatureSensor
*/

// Internal Temperature Sensor
// Example sketch for ATmega328 types.
//
// April 2012, Arduino 1.0

/*
  (1) - Load this sketch into the ATMEGA
  (2) - Power down the ATMEGA
  (3) - Wait for 30 minutes for the chip to reach ambient temperature
  (4) - Power up and *IMMEDIATELY* read the serial-port for temperature values.
  (5) - Make a note of the first few reported temperatures and the actual
        ambient temperature.

  Repeat this for different temperatures, then determine the values for gain
  and offset.
    With two points given:
    gain(new)   = (ambient[1] - ambient[2]) / (chip[1] - chip[2])
    offset(new) = ambient[1] - gain * chip[1]  OR ambient[2] - gain * chip[2]

  If you have more data use a least-squares fit of ambient (y) vs. chip (x) to
  find the new gain and offset.

  CT_gain   = gain(old)   * gain(new)
  CT_offset = offset(old) * gain(new) + offset(new)
*/

static const float CT_gain   = 1;
static const float CT_offset = 0;

void setup()
{
  Serial.begin(9600);

  Serial.println(F("Internal Temperature Sensor"));
}

void loop()
{
  // Show the temperature in degrees Celcius.
  Serial.println(GetTemp(),2);
  delay(500);
}

double GetTemp(void)
{
  unsigned int wADC;
  double t;

  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.

  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC

  delay(20);            // wait for voltages to become stable.

  ADCSRA |= _BV(ADSC);  // Start the ADC

  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));

  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;

  // The offset of 324.31 could be wrong. It is just an indication.
  t = CT_gain * (wADC) + CT_offset;

  // The returned temperature is in degrees Celcius.
  return (t);
}
