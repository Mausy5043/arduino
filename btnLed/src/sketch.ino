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

void setup()
{
  pinMode(onoffPIN, OUTPUT);       // An LED to signal on or off state
  attachInterrupt(0, ISRbutton, RISING); // Interrupt when button is pressed
  //pinMode(pushbuttonPIN, INPUT);  // A button to switch the LED
}

void loop()
{
  digitalWrite(onoffPIN, flag);
  // wait for a button press
/*
  if ((digitalRead(pushbuttonPIN) == HIGH) && (flag == LOW))
  {
    flag = HIGH;
    digitalWrite(onoffLED, HIGH);
    delay(500);
    //digitalWrite(onoffLED,LOW);
  }
  if ((digitalRead(pushbuttonPIN) == HIGH) && (flag == HIGH))
  {
    flag = LOW;
    digitalWrite(onoffLED, LOW);
    delay(500);
    //digitalWrite(onoffLED,LOW);
  }
*/
}


// toggles LED when interrupt pin changes state

void ISRbutton()
{
  flag = !flag;
}
