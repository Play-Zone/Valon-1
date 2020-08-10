/*
  Infrared remote control Valon - 红外遥控Valon 运动
  LED Pin conflict with serial port, Cannot be used simultaneously
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define SERIALDEBUG
#define SOUND_PIN    11        // buzzer pin

#define MLPinDir 4
#define MLPinSpeed 5
#define MRPinDir 9
#define MRPinSpeed 6

#include "oledfont.h"
#include "valonOLED.h"
#include <IRremote.h>
#define RECV_PIN A0    //Sets the receive pin for the IR.
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long previousMillis = millis();

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示
  pinMode(MLPinDir, OUTPUT);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);
  //
  //        OLED_DrawBMP(40, 2, 48, 33, DIR_left);
  //delay(1000);
  irrecv.enableIRIn(); // Start the receiver
#ifdef SERIALDEBUG
  Serial.println("Enabled IRin");
#endif
}

void loop() {
  if (irrecv.decode(&results)) {
    dosomething(&results);
    MSound(1, 100, 2500);
    irrecv.resume(); // Receive the next value
  }
}

void dosomething(decode_results *results) {
  OLED_DrawBMP(40, 1, 48, 48, DIR_empty);
#ifdef SERIALDEBUG
  Serial.print(results->value, HEX);
  Serial.println("");
#endif
  switch (results->value) {
    case 0xFF02FD:   // 红外 + 按键   -- 前进
      OLED_DrawBMP(47, 1, 33, 48, DIR_up);
      setMotor(80, 80);
      break;
    case 0xFF9867:   // 红外 - 按键  -- 后退
      setMotor(-80, -80);
      OLED_DrawBMP(47, 1, 33, 48, DIR_down);
      break;
    case 0xFFE01F:   // 红外 |<< 按键  -- 左转
      OLED_DrawBMP(40, 2, 48, 33, DIR_left);
      setMotor(10, 80);
      break;
    case 0xFF906F:   // 红外 >>| 按键  -- 右转
      OLED_DrawBMP(40, 2, 48, 33, DIR_right);
      setMotor(80, 10);
      break;
    default :        // 红外 其他 按键  -- 停止
      OLED_DrawBMP(40, 1, 48, 48, DIR_stop);
      setMotor(0, 0);
      break;
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
