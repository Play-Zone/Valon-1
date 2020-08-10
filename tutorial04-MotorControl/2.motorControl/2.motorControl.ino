/*
  motor Control - 控制电机
  ----------------------------------------------------------------
  This example code is in the public domain.
  www.yfrobot.com
*/

#define MLPinDir 4
#define MLPinSpeed 5
#define MRPinDir 9
#define MRPinSpeed 6

int speedM = 0;
int speedChange = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MLPinDir, OUTPUT);
  pinMode(MLPinSpeed, OUTPUT);
  pinMode(MRPinDir, OUTPUT);
  pinMode(MRPinSpeed, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  setMotor(speedM, speedM);
  speedM += speedChange;
  if (speedM > 255) {
    speedChange = -5;
  } else if (speedM < -255) {
    speedChange = 5;
  }
  Serial.println(speedM);
  delay(10);
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
