/*
  Ultrasonic follow - 超声波跟随小车
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#include "oledfont.h"
#include "valonOLED.h"

#define MLPinDir 4
#define MLPinSpeed 5
#define MRPinDir 9
#define MRPinSpeed 6

#define UltrasonicTrigPin 12
#define UltrasonicEchoPin 13

unsigned int dis_mm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(UltrasonicTrigPin, OUTPUT);
  pinMode(UltrasonicEchoPin, INPUT);
  pinMode(MLPinDir, OUTPUT);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示
}

void loop() {
  // put your main code here, to run repeatedly:
  dis_mm = UltrasonicRanging();
  displsyDis(dis_mm);
  //  Serial.print(dis_mm);
  //  Serial.println("mm");
  delay(80);
  if (dis_mm >= 110) {
    setMotor(100, 100);
  } else {
    setMotor(0, 0);
  }
}

/*
  驱动电机函数
  参数：mlSpeed - 左电机速度 mrSpeed - 右电机速度 （取值范围：-255 ~ 255）
*/
void setMotor(int mlSpeed, int mrSpeed) {
  if (mlSpeed > 0) {
    digitalWrite(MLPinDir, LOW);
    analogWrite(MLPinSpeed, mlSpeed);
  } else if (mlSpeed < 0) {
    digitalWrite(MLPinDir, HIGH);
    analogWrite(MLPinSpeed, abs(mlSpeed));
  } else {
    analogWrite(MLPinSpeed, mlSpeed);
  }
  if (mrSpeed > 0) {
    digitalWrite(MRPinDir, LOW);
    analogWrite(MRPinSpeed, mrSpeed);
  } else if (mrSpeed < 0) {
    digitalWrite(MRPinDir, HIGH);
    analogWrite(MRPinSpeed, abs(mrSpeed));
  } else {
    analogWrite(MRPinSpeed, mrSpeed);
  }
}

int UltrasonicRanging() {
  int distance_mm = 0;
  digitalWrite(UltrasonicTrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(UltrasonicTrigPin, HIGH);
  delayMicroseconds(12);
  digitalWrite(UltrasonicTrigPin, LOW);

  distance_mm = pulseIn(UltrasonicEchoPin, HIGH) * 0.17; //将回波时间换算成mm   公式：距离= 高电平时间*声速（340M/S）/2
  return distance_mm;
}

void displsyDis(int dis) {
  OLED_ShowString(0, 0, "u_dis:", 16);
  OLED_ShowNum(48, 0, dis, 4, 16);
  OLED_ShowString(82, 0, "mm", 16);
}

// 中位值滤波法
#define FILTER_N 5
int Filter() {
  int filter_buf[FILTER_N];
  int i, j;
  int filter_temp;
  for (i = 0; i < FILTER_N; i++) {
    filter_buf[i] = UltrasonicRanging();
    delay(1);
  }
  // 采样值从小到大排列（冒泡法）
  for (j = 0; j < FILTER_N - 1; j++) {
    for (i = 0; i < FILTER_N - 1 - j; i++) {
      if (filter_buf[i] > filter_buf[i + 1]) {
        filter_temp = filter_buf[i];
        filter_buf[i] = filter_buf[i + 1];
        filter_buf[i + 1] = filter_temp;
      }
    }
  }
  return filter_buf[(FILTER_N - 1) / 2];
}
