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
  OLED_ShowString(49, 0, "VALON", 8);
  OLED_ShowString(36, 2, "YFROBOT", 16);
  OLED_ShowString(16, 5, "hello,world!", 16);
}

void loop()
{
}
