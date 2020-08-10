/*
  buzzer tone
  This example code is in the public domain.
  www.yfrobot.com
*/

// buzzer connected to pin 11
int buzzer = 11;

void setup()
{
  /* tone(pin, frequency, duration) */
  tone(buzzer, 262, 300); //set buzzer pin to play 264Hz for 300ms
  delay(1000);  //wait 1s

  tone(buzzer, 294, 300); //set buzzer pin to play 297Hz for 300ms
  delay(1000);  //wait 1s

  tone(buzzer, 330, 300); //set buzzer pin to play 330Hz for 300ms
  delay(1000);  //wait 1s
}

void loop()
{
  // no need to repeat the tone.
}
