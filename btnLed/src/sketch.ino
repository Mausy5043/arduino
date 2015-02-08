/*
  btnLed sketch
  Push a button to turn on a LED.
  Push the button again to turn the LED off.

  *******
  Do not connect more than 5 volts directly to an Arduino pin!!
  *******
*/

#define pushbuttonPIN 2
#define onoffLED 3

boolean flag = LOW;

void setup()
{
  pinMode(onoffLED, OUTPUT);       // An LED to signal on or off state
  pinMode(pushbuttonPIN, INPUT);  // A button to switch the LED
}

void loop()
{
  // wait for a button press
  if (digitalRead(pushbuttonPIN) == HIGH)
  {
    digitalWrite(onoffLED, HIGH);
    delay(2000);
    digitalWrite(onoffLED,LOW);
  }
}
