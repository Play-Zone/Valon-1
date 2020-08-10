/*
  OLED显示屏显示例程
  ----------------------------------------------------------------
  GND   电源地
  VCC   5v电源
  scl   A5（SCL）
  sda   A4（SDA）
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#include "oledfont.h"
#include "valonOLED.h"

void setup()
{
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示
  Serial.begin(9600);
  while (1)
  {
    OLED_DrawBMP(0, 0, 128, 64, BMP_Valon_128);
    delay(1000);
    OLED_Clear();
    OLED_DrawBMP(0, 0, 128, 64, BMP_Valon);
    delay(1000);
    OLED_Clear();
  }
}

void loop()
{
}
