/*
  both Motor speed measurement - 双电机速度测量,判断电机运转方向
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#define MLPinDir 4      // Direction of the left motor.
#define MLPinSpeed 5    // Power of left motor.
#define MRPinDir 9      // Direction of the right motor.
#define MRPinSpeed 6    // Power of right motor.

#define EncoderLXOR  2  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderLB  7
#define EncoderRXOR  3  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderRB  8

static volatile bool lastLeftA;
static volatile bool lastLeftB;
static volatile bool lastRightA;
static volatile bool lastRightB;

unsigned long previousMillis = 0;   // 记录上一次时间
int intervalTime = 200;             // 采样时间
uint16_t pulseCountRight = 0;       // 右电机脉冲计数
uint16_t pulseCountLeft = 0;        // 左电机脉冲计数
int Speed = 150;                    // 电机运行速度
bool motorLeftDir = true;           // 电机方向true - 正向; false - 反向
bool motorRightDir = true;          // 电机方向true - 正向; false - 反向

void setup() {
  Serial.begin(115200);
  pinMode(EncoderLXOR, INPUT_PULLUP);
  pinMode(EncoderLB, INPUT_PULLUP);
  pinMode(EncoderRXOR, INPUT_PULLUP);
  pinMode(EncoderRB, INPUT_PULLUP);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MLPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);

  analogWrite(MLPinSpeed, Speed);
  digitalWrite(MLPinDir, LOW);
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
  attachInterrupt(digitalPinToInterrupt(EncoderRXOR), countRight, CHANGE);  //引脚电平由低变高时触发
  attachInterrupt(digitalPinToInterrupt(EncoderLXOR), countLeft, CHANGE);   //引脚电平由低变高时触发
  previousMillis = millis();
}

void loop() {
  if (millis() - previousMillis >= intervalTime) {
    detachInterrupt(digitalPinToInterrupt(EncoderRXOR)); // 关闭外部中断
    detachInterrupt(digitalPinToInterrupt(EncoderLXOR)); // 关闭外部中断
    Serial.print("pulseCountRight = ");
    if (motorRightDir)
      Serial.print(pulseCountRight);
    else Serial.print(pulseCountRight - 65535);
    Serial.print("  pulseCountLeft = ");
    if (motorLeftDir)
      Serial.print(pulseCountLeft);
    else Serial.print(pulseCountLeft - 65535);
    pulseCountRight = 0;
    pulseCountLeft = 0;
    previousMillis = millis();
    Serial.println();
    attachInterrupt(digitalPinToInterrupt(EncoderRXOR), countRight, CHANGE);  //重新打开外部中断
    attachInterrupt(digitalPinToInterrupt(EncoderLXOR), countLeft, CHANGE);   //重新打开外部中断
  }
}

void countRight() {
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
    //    Serial.print(" Right dir =");
    if (((lastRightB ^ nowRightA) - (nowRightB ^ lastRightA)) > 0) {
      motorRightDir = true;
      //      Serial.print("CW, ");
    } else {
      motorRightDir = false;
      //      Serial.print("CCW, ");
    }
    pulseCountRight += (lastRightB ^ nowRightA) - (nowRightB ^ lastRightA);
    //  Serial.print(" pulseCountRight =");
    //  Serial.print(pulseCountRight);
    lastRightA = nowRightA;
    lastRightB = nowRightB;
  } else {
//    Serial.println("ERROR!!!!! ");
  }
  //  Serial.println();
}

void countLeft() {
  bool nowLeftB = digitalRead(EncoderLB);
  bool nowLeftXOR = digitalRead(EncoderLXOR);
  bool nowLeftA = nowLeftXOR ^ nowLeftB ;
  if (!((lastLeftA ^ nowLeftA) & (lastLeftB ^ nowLeftB))) {
    //    Serial.print(" nowLeftB =");
    //    Serial.print(nowLeftB);
    //    Serial.print(" nowLeftXOR =");
    //    Serial.print(nowLeftXOR);
    //    Serial.print(" nowLeftA =");
    //    Serial.print(nowLeftA);
    //    Serial.print(" Left dir =");
    if (((lastLeftB ^ nowLeftA) - (nowLeftB ^ lastLeftA)) > 0) {
      motorLeftDir = true;
      //      Serial.print("CW, ");
    } else {
      motorLeftDir = false;
      //      Serial.print("CCW, ");
    }
    pulseCountLeft += (lastLeftB ^ nowLeftA) - (nowLeftB ^ lastLeftA);
    //  Serial.print(" pulseCountLeft =");
    //  Serial.print(pulseCountLeft);
    lastLeftA = nowLeftA;
    lastLeftB = nowLeftB;
  } else {
//    Serial.println("ERROR!!!!! ");
  }
  //  Serial.println();
}
