/*
  Infrared remote control LED - 红外遥控LED
  LED Pin conflict with serial port, Cannot be used simultaneously
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

//#define SERIALDEBUG
#define SOUND_PIN    11        // arduino pin number
#include <IRremote.h>
#define RECV_PIN A0    //Sets the receive pin for the IR.
IRrecv irrecv(RECV_PIN);
decode_results results;

int LED_PIN = 1;   // LED D2

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#else
  pinMode(LED_PIN, OUTPUT);
#endif
  irrecv.enableIRIn(); // Start the receiver
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif
}

void loop() {
  if (irrecv.decode(&results)) {
    dosomething(&results);
    MSound(1, 100, 2500);
    delay(100);
    irrecv.resume(); // Receive the next value
  }
  delay(50);
}

void dosomething(decode_results *results) {
#ifdef SERIALDEBUG
  Serial.print(results->value, HEX);
  Serial.println("");
#endif
  switch (results->value) {
    case 0xFFA25D:   // 红外开关按键   -- 点亮led
#ifndef SERIALDEBUG
      digitalWrite(LED_PIN, HIGH);
#endif
      break;
    case 0xFFE21D:   // 红外 menu按键  -- 熄灭led
#ifndef SERIALDEBUG
      digitalWrite(LED_PIN, LOW);
#endif
      break;
  }
}

// BUGBUG:: Move to some library...
//==============================================================================
//    SoundNoTimer - Quick and dirty tone function to try to output a frequency
//            to a speaker for some simple sounds.
//    快速而粗糙的音调功能试图向扬声器输出一些频率以获得一些简单的声音。
//==============================================================================
#ifdef SOUND_PIN
void SoundNoTimer(unsigned long duration,  unsigned int frequency) {
#ifdef __AVR__
  volatile uint8_t *pin_port;
  volatile uint8_t pin_mask;
#else
  volatile uint32_t *pin_port;
  volatile uint16_t pin_mask;
#endif
  long toggle_count = 0;
  long lusDelayPerHalfCycle;

  // Set the pinMode as OUTPUT
  pinMode(SOUND_PIN, OUTPUT);

  pin_port = portOutputRegister(digitalPinToPort(SOUND_PIN));
  pin_mask = digitalPinToBitMask(SOUND_PIN);

  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L / (frequency * 2);

  // if we are using an 8 bit timer, scan through prescalars to find the best fit
  while (toggle_count--) {
    // toggle the pin
    *pin_port ^= pin_mask;

    // delay a half cycle
    delayMicroseconds(lusDelayPerHalfCycle);
  }
  *pin_port &= ~(pin_mask);  // keep pin low after stop
}

void MSound(byte cNotes, ...) {
  va_list ap;
  unsigned int uDur;
  unsigned int uFreq;
  va_start(ap, cNotes);

  while (cNotes > 0) {
    uDur = va_arg(ap, unsigned int);
    uFreq = va_arg(ap, unsigned int);
    SoundNoTimer(uDur, uFreq);
    cNotes--;
  }
  va_end(ap);
}
#else
void MSound(byte cNotes, ...)
{
};
#endif
