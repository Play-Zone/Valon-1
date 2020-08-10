/*
  Trapped robot - 被困的机器人
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/
//#define SERIALDEBUG
#include <QTRSensors.h>

#define EMITTER_PIN   10              // Sets the emitter control pin for the sensors.
#define NUM_SENSORS   5               // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4     // average 4 analog samples per sensor reading
const byte qtrPin[] = {A0, A1, A2, A3, A6};

QTRSensorsAnalog qtrA(qtrPin, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];//store the value of QTRSensor
int thresholdValue = 800;

#define MLPinDir 4      //Direction of the motor.
#define MLPinSpeed 5    //Power of motor.
#define MRPinDir 9
#define MRPinSpeed 6

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(115200);
#endif
  pinMode(MLPinDir, OUTPUT);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);

  // 自定义校准值- calibrate
  qtrA.calibrate();
  for (int i = 0; i < NUM_SENSORS; i++)
    qtrA.calibratedMinimumOn[i] = 250;
  for (int i = 0; i < NUM_SENSORS; i++)
    qtrA.calibratedMaximumOn[i] = 800;
}

void loop() {
  qtrA.readLine(sensorValues);  // 机器人相对线的位置
#ifdef SERIALDEBUG
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }
  Serial.println();
#else
  if (sensorValues[0] > thresholdValue) {
    setMotor(80, -80);
    delay(100);
  } else if (sensorValues[1] > thresholdValue) {
    setMotor(-80, -80);
    delay(100);
    setMotor(80, -80);
    delay(100);
  } else if (sensorValues[2] > thresholdValue) {
    setMotor(-80, -80);
    delay(100);
    setMotor(150, -80);
    delay(100);
  } else if (sensorValues[3] > thresholdValue) {
    setMotor(-80, -80);
    delay(100);
    setMotor(-80, 80);
    delay(100);
  } else if (sensorValues[4] > thresholdValue) {
    setMotor(-80, 80);
    delay(100);
  } else if (sensorValues[0] <= thresholdValue && sensorValues[1] <= thresholdValue && sensorValues[2] <= thresholdValue &&
             sensorValues[3] <= thresholdValue && sensorValues[4] <= thresholdValue) {
    setMotor(80, 80);
    delay(10);
  }
#endif
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
