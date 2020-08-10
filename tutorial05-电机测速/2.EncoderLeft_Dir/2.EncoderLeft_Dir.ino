/*
  Motor speed measurement and Detection direction - 电机速度测量,并判断电机运转方向
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define MLPinDir 4      // Direction of the motor.
#define MLPinSpeed 5    // 电机速度
#define EncoderLXOR  2  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderLB  7    // B相 引脚

static volatile bool lastLeftA;
static volatile bool lastLeftB;

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
  pinMode(EncoderLXOR, INPUT_PULLUP);
  pinMode(EncoderLB, INPUT_PULLUP);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MLPinDir, OUTPUT);

  analogWrite(MLPinSpeed, Speed);
  digitalWrite(MLPinDir, HIGH);
  /*
    mode: defines when the interrupt should be triggered. Four constants are predefined as valid values:
    - LOW to trigger the interrupt whenever the pin is low,
    - CHANGE to trigger the interrupt whenever the pin changes value
    - RISING to trigger when the pin goes from low to high,
    - FALLING for when the pin goes from high to low.
    The Due board allows also:
    - HIGH to trigger the interrupt whenever the pin is high.
  */
  attachInterrupt(digitalPinToInterrupt(EncoderLXOR), count, CHANGE);  //引脚电平由低变高时触发
  previousMillis = millis();
}

void loop() {
//  if (motorcount > reverse) {   // 电机反转运行
//    motorcount = 0;
//    digitalWrite(MLPinDir, !digitalRead(MLPinDir));
//  }
  if (millis() - previousMillis >= intervalTime) {
    detachInterrupt(digitalPinToInterrupt(EncoderLXOR)); // 关闭外部中断
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
    attachInterrupt(digitalPinToInterrupt(EncoderLXOR), count, CHANGE);  //重新打开外部中断
  }
}

void count() {
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
    //    Serial.print(" dir =");
    if (((lastLeftB ^ nowLeftA) - (nowLeftB ^ lastLeftA)) > 0) {
      motordir = true;
      //      Serial.print("CW, ");
    } else {
      motordir = false;
      //      Serial.print("CCW, ");
    }
    //    Serial.print(motordir);
    pulseCount += (lastLeftB ^ nowLeftA) - (nowLeftB ^ lastLeftA);
    //  Serial.print(" pulseCount =");
    //  Serial.print(pulseCount);
    lastLeftA = nowLeftA;
    lastLeftB = nowLeftB;
  } else {
    Serial.print("ERROR!!!!! ");
  }
}
