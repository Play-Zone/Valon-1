/*
  电压检测

  This example code is in the public domain.
  www.yfrobot.com
*/
//#define SERIALDEBUG  // 串口调试

#include "oledfont.h"
#include "valonOLED.h"
#define voltagePin A7
float voltageVal = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(voltagePin, INPUT);
  analogReference(DEFAULT); // 设置参考电压5V
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示
}

void loop()
{
  voltageVal = analogRead(voltagePin);
  long volVal = (long)voltageVal * 2000 / 1023;  //换算过程请看程序最后的电压检测函数 - CheckVoltage() 
#ifdef SERIALDEBUG
  Serial.print("voltage:");
  Serial.print(voltageVal);
  Serial.print("\t");
  Serial.print(volVal);
  Serial.println();
  
  Serial.print("voltage:");
  Serial.print(floatToChar(volVal));
  Serial.print("V");
  Serial.println();
#endif
  OLED_ShowString(0, 0, "voltage:", 16);
  OLED_ShowString(65, 0, floatToChar(volVal), 16);
  OLED_ShowString(108, 0, "v", 16);
  delay(500);
}

// 3位 long 转 char*
char* floatToChar(long f) {
  int n_h = (f / 100) % 10;
  int n_t = (f / 10) % 10;
  int n_e = f % 10;
  char *str_v = new char[10];
  sprintf(str_v, "%d.%d%d", n_h, n_t, n_e);
  return str_v;
}

/*
  // 电压检测函数
  void CheckVoltage() {
  voltageVal = analogRead(voltagePin);
  //  Serial.print(voltageVal);
  //  Serial.print("\t");
  // 基准电压 5v
  // read/1023 = V/5 可得 V  (read 为模拟端口读取值)
  // V/Vin = 10/(30+10) 得到输出电压Vin
  // 得到Vin x100 例如：637 代表 6.37V
  // 最终公式：Vin = read * 2000 /1023 (read 为模拟端口读取值)

  voltageVal = (long)voltageVal * 2000 / 1023;
  //  Serial.println((long)voltageVal);
  }
*/
