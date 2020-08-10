/*
  OLED显示屏显示例程 - 显示中文
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
  OLED_ShowString(36, 0, "VALON-I", 16);
  OLED_ShowChinese(20, 3, 0, 16);
  OLED_ShowChinese(38, 3, 1, 16);
  OLED_ShowChinese(56, 3, 2, 16);
  OLED_ShowChinese(74, 3, 3, 16);
  OLED_ShowChinese(92, 3, 4, 16);
  OLED_ShowString(36, 6, "YFROBOT", 16);
}

void loop()
{
}
