#include <Keyboard.h>

int buttonPin = 9;  // Set a button to any pin

void setup()
{
  //pinMode(buttonPin, INPUT);  // Set the button as an input
  //digitalWrite(buttonPin, HIGH);  // Pull the button high
}

void loop()
{
  int inPin = analogRead(A0);
  //Serial.println(inPin);
  if ( inPin > 0)
  {
    Keyboard.write('A');  // send a 'z' to the computer via Keyboard HID
    buttonPin++;
  }

  delay(250);
}

