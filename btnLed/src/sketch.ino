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

volatile int flag = LOW;
unsigned long timestamp = 0;

void setup()
{
  pinMode(onoffLED, OUTPUT);       // An LED to signal on or off state
  attachInterrupt(0, interrupt, HIGH); // Interrupt when button is pressed
}

void interrupt()
{
  // Only change the flag if more than 1000 ms has passed since previous IRQ
  // to handle debouncing-effect.
  if ( (unsigned long)(millis() - timestamp) > 1000 )
  {
    flag = !flag;
    timestamp = millis();
  }
}

void loop()
{
  digitalWrite(onoffLED, flag);
}
