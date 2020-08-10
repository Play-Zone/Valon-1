/*
  Read the Sensors - 读传感器
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
byte QTR_CALI = 400;

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  OLED_Init();
  OLED_ColorTurn(0);    // 0正常显示 1反色显示
  OLED_DisplayTurn(0);  // 0正常显示 1翻转180度显示

  // calibrate
#ifdef SERIALDEBUG
  Serial.println("start calibrate");
#endif
  OLED_ShowString(0, 2, "start calibrate", 16);
  OLED_ShowString(26, 5, "%",  16);
  // calibrate / 校准-将传感器逐个划过黑线
  for (int i = 0; i < 200; i++) {  // make the calibration take about 10 seconds
    qtrA.calibrate();       // reads all sensors 10 times at 2.5 ms per five sensors (i.e. ~25 ms per call)
    OLED_ShowNum(0, 5, i/2, 3, 16);
  }
#ifdef SERIALDEBUG
  Serial.println("done calibrate");
#endif
  OLED_ShowString(0, 2, "done calibrate", 16);
  OLED_Clear();
}

void loop() {
  qtrA.readLine(sensorValues);  // 机器人相对线的位置
#ifdef SERIALDEBUG
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }
  Serial.println();
#endif
  OLED_ShowNum(0, 5, sensorValues[0], 4, 16);
  OLED_ShowNum(12, 1, sensorValues[1], 4, 16);
  OLED_ShowNum(46, 1, sensorValues[2], 4, 16);
  OLED_ShowNum(80, 1, sensorValues[3], 4, 16);
  OLED_ShowNum(96, 5, sensorValues[4], 4, 16);
  delay(50);
}
