/*
  simple line follow - 巡线行驶
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
#define SERIALDEBUG
#include "oledfont.h"
#include "valonOLED.h"
#include <QTRSensors.h>

#define EMITTER_PIN   10              // Sets the emitter control pin for the sensors.
#define NUM_SENSORS   5               // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4     // average 4 analog samples per sensor reading
const byte qtrPin[] = {A0, A1, A2, A3, A6};

QTRSensorsAnalog qtrA(qtrPin, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];//store the value of QTRSensor

#define MLPinDir 4      //Direction of the motor.
#define MLPinSpeed 5    //Power of motor.
#define MRPinDir 9
#define MRPinSpeed 6

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

  // calibrate
#ifdef SERIALDEBUG
  Serial.println("start calibrate");
#endif
  OLED_ShowString(0, 2, "start calibrate", 16);
  for (int i = 0; i < 80; i++)  // make the calibration take about 10 seconds
  {
    qtrA.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
    if (i < 20 || i >= 60)
      setMotor(80, -80);
    else
      setMotor(-80, 80);
  }
  setMotor(0, 0);
  delay(500);
#ifdef SERIALDEBUG
  Serial.println("done calibrate");
#endif
  OLED_ShowString(0, 2, "done calibrate", 16);
  OLED_Clear();
}

void loop() {
  unsigned int last_proportional = 0;
  long integral = 0;
  unsigned int position = qtrA.readLine(sensorValues);
//  OLED_ShowNum(0, 5, position, 4, 16);
  int proportional = ((int)position) - 2000;
  int derivative = proportional - last_proportional;
  integral += proportional;
  last_proportional = proportional;
  int power_difference = proportional / 3 + integral / 3000 + derivative * 3 / 400;
  const int maxcount = 150;
  if (power_difference > maxcount)
    power_difference = maxcount;
  if (power_difference < -maxcount)
    power_difference = -maxcount;

  if (power_difference < 0)
    setMotor(maxcount + power_difference, maxcount);
  else
    setMotor(maxcount, maxcount - power_difference);
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
