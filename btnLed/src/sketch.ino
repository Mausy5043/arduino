/*
  btnLed sketch
  Push a button to turn on a LED.
  Push the button again to turn the LED off.

  *******
  Do not connect more than 5 volts directly to an Arduino pin!!
  *******
*/

#define pushbuttonPIN 2
#define onoffPIN 3

volatile int flag = LOW;
unsigned long timestamp = 0;

void setup()
{
  pinMode(onoffPIN, OUTPUT);       // An LED to signal on or off state
  attachInterrupt(0, interrupt, HIGH); // Interrupt when button is pressed
}

void interrupt()
{
  // Only change the flag if more than 1000 ms has passed since previous IRQ
  // to avoid debouncing.
  if ( (timestamp + 1000) < millis() )
  {
    flag = !flag;
  }
  timestamp = millis();
}

void loop()
{
  digitalWrite(onoffPIN, flag);
}
