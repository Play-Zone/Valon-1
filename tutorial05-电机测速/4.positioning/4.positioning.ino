/*
  positioning - 
  根据电机检测的脉冲,运行小车完成一个正方形运行轨迹
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
//#define SERIALDUBUG

#define MLPinDir 4      // Direction of the left motor.
#define MLPinSpeed 5    // Power of left motor.
#define MRPinDir 9      // Direction of the right motor.
#define MRPinSpeed 6    // Power of right motor.
#define EncoderLXOR  2  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderLB  7
#define EncoderRXOR  3  // 连接到中断引脚,该引脚信号来自于 AB相异或
#define EncoderRB  8
#define SOUND_PIN    11        // buzzer pin 

static volatile bool lastLeftA;
static volatile bool lastLeftB;
static volatile bool lastRightA;
static volatile bool lastRightB;

int speedVal = 150;                 // 电机速度
int conutError = 5;               // 计数误差
int intervalTime = 50;              // 采样时间

unsigned long previousMillis = 0;   // 记录上一次时间
long pulseCountRight = 0;           // 右电机脉冲计数
long pulseCountLeft = 0;            // 左电机脉冲计数
long lastPulseCountRight = 0;       // 上一个右电机脉冲计数
long lastPulseCountLeft = 0;        // 上一个左电机脉冲计数
int Speed_L = 0;                    // 电机运行速度
int Speed_R = 0;                    // 电机运行速度
bool motorLeftDir = true;           // 电机方向true - 正向; false - 反向
bool motorRightDir = true;          // 电机方向true - 正向; false - 反向

int action = 0; // 动作计数

void setup() {
#ifdef SERIALDUBUG
  Serial.begin(115200);
#endif
  pinMode(EncoderLXOR, INPUT_PULLUP);
  pinMode(EncoderLB, INPUT_PULLUP);
  pinMode(EncoderRXOR, INPUT_PULLUP);
  pinMode(EncoderRB, INPUT_PULLUP);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MLPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);

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
  delay(1000);
  previousMillis = millis();
}

int turnval = 463;  // 

void loop() {
  if (action == -1) {
    //    action = 0;
  } else if (action == 0)
    setMotorPos(1000, 1000);
  else if (action == 1) {
    setMotorPos(-turnval, turnval);
  } else if (action == 2) {
    setMotorPos(1000, 1000);
  } else if (action == 3) {
    setMotorPos(-turnval, turnval);
  } else if (action == 4) {
    setMotorPos(1000, 1000);
  } else if (action == 5) {
    setMotorPos(-turnval, turnval);
  } else if (action == 6) {
    setMotorPos(1000, 1000);
  } else if (action == 7) {
    setMotorPos(-turnval, turnval);
  } else {
    setMotorPos(0, 0);
  }

  if (millis() - previousMillis >= intervalTime) {
#ifdef SERIALDUBUG
    Serial.print("pulseCountRight pos = ");
    Serial.print(pulseCountRight);
    Serial.print(" pos = ");
    Serial.print(lastPulseCountRight);
    Serial.print("  pulseCountLeft pos = ");
    Serial.print(pulseCountLeft);
    Serial.print(" pos = ");
    Serial.print(lastPulseCountLeft);
    Serial.print("  action = ");
    Serial.print(action);
    Serial.println();
#endif
    previousMillis = millis();
  }
}

/*
  设置位置函数
  参数：mlPos - 0~65535  mrPos - 0~65535
*/
int setMotorPos(long mlPos, long mrPos) {
  long displacementRight = pulseCountRight - lastPulseCountRight;
  long displacementLeft = pulseCountLeft - lastPulseCountLeft;
  if (displacementRight >= (mrPos - conutError) && displacementRight <= (mrPos + conutError)) {
    Speed_R = 0;
  } else if (displacementRight < (mrPos - conutError)) {
    Speed_R = speedVal;
  } else if (displacementRight > (mrPos + conutError)) {
    Speed_R = -speedVal;
  }

  if (displacementLeft >= (mlPos - conutError) && displacementLeft <= (mlPos + conutError)) {
    Speed_L = 0;
  } else if (displacementLeft < (mlPos - conutError)) {
    Speed_L = speedVal;
  } else if (displacementLeft > (mlPos + conutError)) {
    Speed_L = -speedVal;
  }
  setMotor(Speed_L, Speed_R);
  if (!(Speed_L | Speed_R)) {     // 左右电机都停止时返回计数
    lastPulseCountRight += mrPos;
    lastPulseCountLeft += mlPos;
    return action ++;
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

// BUGBUG:: Move to some library...
//==============================================================================
//    SoundNoTimer - Quick and dirty tone function to try to output a frequency
//            to a speaker for some simple sounds.
//    快速而粗糙的音调功能试图向扬声器输出一些频率以获得一些简单的声音。
//==============================================================================
#ifdef SOUND_PIN
void SoundNoTimer(unsigned long duration,  unsigned int frequency) {
#ifdef __AVR__
  volatile uint8_t *pin_port;
  volatile uint8_t pin_mask;
#else
  volatile uint32_t *pin_port;
  volatile uint16_t pin_mask;
#endif
  long toggle_count = 0;
  long lusDelayPerHalfCycle;

  // Set the pinMode as OUTPUT
  pinMode(SOUND_PIN, OUTPUT);

  pin_port = portOutputRegister(digitalPinToPort(SOUND_PIN));
  pin_mask = digitalPinToBitMask(SOUND_PIN);

  toggle_count = 2 * frequency * duration / 1000;
  lusDelayPerHalfCycle = 1000000L / (frequency * 2);

  // if we are using an 8 bit timer, scan through prescalars to find the best fit
  while (toggle_count--) {
    // toggle the pin
    *pin_port ^= pin_mask;

    // delay a half cycle
    delayMicroseconds(lusDelayPerHalfCycle);
  }
  *pin_port &= ~(pin_mask);  // keep pin low after stop
}

void MSound(byte cNotes, ...) {
  va_list ap;
  unsigned int uDur;
  unsigned int uFreq;
  va_start(ap, cNotes);

  while (cNotes > 0) {
    uDur = va_arg(ap, unsigned int);
    uFreq = va_arg(ap, unsigned int);
    SoundNoTimer(uDur, uFreq);
    cNotes--;
  }
  va_end(ap);
}
#else
void MSound(byte cNotes, ...)
{
};
#endif
