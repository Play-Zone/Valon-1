/*
  Motor speed measurement - 单霍尔电机速度测量-单右电机
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define MRPinDir 9        //Direction of the motor.
#define MRPinSpeed 6      //Power of motor.
#define EncoderRXOR  3
//#define EncoderRB  8

unsigned long previousMillis = 0;   // 记录上一次时间
int intervalTime = 1000;             // 采样时间
unsigned int pulseCount = 0;        // 脉冲计数
int rp = 0;                         // 转数
float rpm = 0;                      // 转速 ,转/分
int ppr = 2;                        // 单圈脉冲数
int Speed = 120;                    // 电机运行速度

void setup() {
  Serial.begin(115200);
  pinMode(EncoderRXOR, INPUT);
  pinMode(MRPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);

  analogWrite(MRPinSpeed, Speed);
  digitalWrite(MRPinDir, HIGH);
  /*
    mode: defines when the interrupt should be triggered. Four constants are predefined as valid values:
    - LOW to trigger the interrupt whenever the pin is low,
    - CHANGE to trigger the interrupt whenever the pin changes value
    - RISING to trigger when the pin goes from low to high,
    - FALLING for when the pin goes from high to low.
    The Due board allows also:
    - HIGH to trigger the interrupt whenever the pin is high.
  */
  attachInterrupt(digitalPinToInterrupt(EncoderRXOR), count, CHANGE);  //引脚电平发生改变时触发
  previousMillis = millis();
}

void loop() {
  if (millis() - previousMillis >= intervalTime) {
    detachInterrupt(digitalPinToInterrupt(EncoderRXOR)); // 关闭外部中断
    Serial.print("pulseCount = ");
    Serial.print(pulseCount);
    rp = pulseCount / (ppr * 2);
    Serial.print(" rp =");
    Serial.print(rp);
    rpm = (float)(rp * 60000 / intervalTime); // 转/分
    Serial.print(" rpm =");
    Serial.println(rpm);
    pulseCount = 0;
    previousMillis = millis();
    attachInterrupt(digitalPinToInterrupt(EncoderRXOR), count, CHANGE);  //重新打开外部中断
  }
}

void count() {
  pulseCount += 1;
}
