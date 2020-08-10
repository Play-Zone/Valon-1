/*
  Infrared receive - 红外接收
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#define SERIALDEBUG

#include "oledfont.h"
#include "valonOLED.h"
#include <IRremote.h>
#define RECV_PIN A0    //Sets the receive pin for the IR.
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示

  irrecv.enableIRIn(); // Start the receiver
  OLED_ShowString(0, 2, "Enabled IRin", 16);
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif
  delay(100);
  OLED_Clear();
#ifdef SERIALDEBUG
  Serial.println("IR Recv Code:");
#endif
  OLED_ShowString(0, 1, "IR Recv Code:", 16);
}

void loop() {
  if (irrecv.decode(&results)) {
#ifdef SERIALDEBUG
    Serial.println(results.value, HEX);
#endif
    char code[7];
    snprintf(code, 7, "FF%X", results.value);
    OLED_ShowString(0, 5, code, 16);
#ifdef SERIALDEBUG
    Serial.println(code);
#endif
    irrecv.resume(); // Receive the next value
  }
  delay(50);
}
