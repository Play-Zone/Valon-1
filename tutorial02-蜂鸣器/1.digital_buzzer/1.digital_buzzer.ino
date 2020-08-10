/*
  buzzer blink
  This example code is in the public domain.
  www.yfrobot.com
*/

// buzzer connected to pin 11
int buzzer = 11;

void setup()
{
  // initialize the pin as an output.
  pinMode(buzzer, OUTPUT);
}

void loop()
{ 
  digitalWrite(buzzer, HIGH); //turn on buzzer(set logic level high)
  delay(1000);  //wait 1s
  digitalWrite(buzzer, LOW);  //turn off buzzer(set logic level low)
  delay(1000);  //wait 1s
}
