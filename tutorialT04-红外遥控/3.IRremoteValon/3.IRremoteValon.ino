/*
  Infrared remote control Valon - 红外遥控Valon 运动
  LED Pin conflict with serial port, Cannot be used simultaneously
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

//#define SERIALDEBUG
#define SOUND_PIN    11        // buzzer pin

#define MLPinDir 4
#define MLPinSpeed 5
#define MRPinDir 9
#define MRPinSpeed 6

#include <IRremote.h>
#define RECV_PIN A0    //Sets the receive pin for the IR.
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long previousMillis = millis();

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  pinMode(MLPinDir, OUTPUT);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);

  irrecv.enableIRIn(); // Start the receiver
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif
}

void loop() {
  if (irrecv.decode(&results)) {
    dosomething(&results);

    irrecv.resume(); // Receive the next value
  }
}

void dosomething(decode_results *results) {
#ifdef SERIALDEBUG
  Serial.print(results->value, HEX);
  Serial.println("");
#endif
  if (results->value == 0xFF02FD || results->value == 0xE41B916E) {         // 红外 + 按键   -- 前进
    MSound(1, 100, 2500);
    setMotor(-200, -200);
  } else if (results->value == 0xFF9867 || results->value == 0xE41B11EE) {  // 红外 - 按键  -- 后退
    MSound(1, 100, 2500);
    setMotor(200, 200);
  } else if (results->value == 0xFFE01F || results->value == 0xE41B817E) {  // 红外 |<< 按键  -- 左转
    MSound(1, 100, 2500);
    setMotor(-80, -200);
  } else if (results->value == 0xFF906F || results->value == 0xE41BA15E) {   // 红外 >>| 按键  -- 右转
    MSound(1, 100, 2500);
    setMotor(-200,-80);
  } else {       // 红外 其他 按键  -- 停止
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
