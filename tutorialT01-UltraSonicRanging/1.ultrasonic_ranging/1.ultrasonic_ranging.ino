/*
  Ultrasonic ranging - 超声波测距
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#include "oledfont.h"
#include "valonOLED.h"

#define UltrasonicTrigPin 12
#define UltrasonicEchoPin 13

unsigned int dis_mm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示
  pinMode(UltrasonicTrigPin, OUTPUT);
  pinMode(UltrasonicEchoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  dis_mm = UltrasonicRanging();
  OLED_ShowString(0, 0, "u_dis:", 16);
  OLED_ShowNum(48, 2, dis_mm, 4, 16);
  OLED_ShowString(82, 2, "mm", 16);
  //  Serial.print(dis_mm);
  //  Serial.println("mm");
  delay(100);
}

int UltrasonicRanging() {
  int distance_mm = 0;
  digitalWrite(UltrasonicTrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(UltrasonicTrigPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(UltrasonicTrigPin, LOW);
  //将回波时间换算成mm   公式：距离= 高电平时间*声速（340M/S）/2
  distance_mm = pulseIn(UltrasonicEchoPin, HIGH) * 0.17;
  return distance_mm;
}
