/*
  Motor speed measurement and Detection direction - 电机速度测量,并判断电机运转方向
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define MRPinDir 9      // Direction of the motor.
#define MRPinSpeed 6    // 电机速度
#define EncoderRXOR  3  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderRB  8    // B相 引脚

static volatile bool lastLeftA;
static volatile bool lastLeftB;
static volatile bool lastRightA;
static volatile bool lastRightB;

unsigned long previousMillis = 0;   // 记录上一次时间
int intervalTime = 200;             // 采样时间
uint16_t pulseCount = 0;            // 脉冲计数
int rp = 0;                         // 转数
float rpm = 0;                      // 转速 ,转/分
int ppr = 2;                        // 电机输出轴单圈脉冲数
int Speed = 150;                    // 电机运行速度

int motorcount = 0;           // 记录电机运行次数
int reverse = 10;             // 反转
bool motordir = true;         // 电机方向true - 正向; false - 反向

void setup() {
  Serial.begin(115200);
  pinMode(EncoderRXOR, INPUT_PULLUP);
  pinMode(EncoderRB, INPUT_PULLUP);
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
  attachInterrupt(digitalPinToInterrupt(EncoderRXOR), count, CHANGE);  //引脚电平由低变高时触发
  previousMillis = millis();
}

void loop() {
  if (motorcount > reverse) {   // 电机反转运行
    motorcount = 0;
    digitalWrite(MRPinDir, !digitalRead(MRPinDir));
  }
  if (millis() - previousMillis >= intervalTime) {
    detachInterrupt(digitalPinToInterrupt(EncoderRXOR)); // 关闭外部中断
    motorcount++; // 记录电机运行
    Serial.print("pulseCount = ");
    if (motordir)
      Serial.print(pulseCount);
    else Serial.print(pulseCount - 65535);
    //    rp = pulseCount / (ppr * 2) ;
    //    Serial.print(" rp =");
    //    Serial.print(rp);
    //    rpm = (float)(rp * 60000 / intervalTime); // 转/分
    //    Serial.print(" rpm =");
    //    Serial.print(rpm);
    //    Serial.print(" dir =");
    //    Serial.println(motordir);
    pulseCount = 0;
    previousMillis = millis();
    Serial.println();
    attachInterrupt(digitalPinToInterrupt(EncoderRXOR), count, CHANGE);  //重新打开外部中断
  }
}

void count() {
  bool nowRightB = digitalRead(EncoderRB);
  bool nowRightXOR = digitalRead(EncoderRXOR);
  bool nowRightA = nowRightXOR ^ nowRightB ;
  if (!((lastRightA ^ nowRightA) & (lastRightB ^ nowRightB))) {
    //    Serial.print(" nowRightB =");
    //    Serial.print(nowRightB);
    //    Serial.print(" nowRightXOR =");
    //    Serial.print(nowRightXOR);
    //    Serial.print(" nowRightA =");
    //    Serial.print(nowRightA);
    //    Serial.print(" dir =");
    if (((lastRightB ^ nowRightA) - (nowRightB ^ lastRightA)) > 0) {
      motordir = true;
      //      Serial.print("CW, ");
    } else {
      motordir = false;
      //      Serial.print("CCW, ");
    }
    //    Serial.print(motordir);
    pulseCount += (lastRightB ^ nowRightA) - (nowRightB ^ lastRightA);
    //  Serial.print(" pulseCount =");
    //  Serial.print(pulseCount);
    lastRightA = nowRightA;
    lastRightB = nowRightB;
  } else {
    Serial.print("ERROR!!!!! ");
  }
}
