#include <IBusBM.h>

IBusBM ibusRc;

HardwareSerial& ibusRcSerial = Serial1;
HardwareSerial& debugSerial = Serial;

// define pin
const int lfm = 2;
const int lbm = 3;
const int rfm = 5;
const int rbm = 6;
const int ctm = 7;

// Constants for motor speed calculation
const float MOTOR_SPEED_FACTOR = 2.5;
const float DIAGONAL_SPEED_FACTOR = 1.2;

void setup() {
  debugSerial.begin(74880);
  ibusRc.begin(ibusRcSerial);
  pinMode(lfm, OUTPUT);
  pinMode(lbm, OUTPUT);
  pinMode(rfm, OUTPUT);
  pinMode(rbm, OUTPUT);
}

int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibusRc.readChannel(channelInput);
  return (ch < 100) ? defaultValue : map(ch, 1000, 2000, minLimit, maxLimit);
}

bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  analogWrite(lfm, leftSpeed);
  analogWrite(lbm, leftSpeed);
  analogWrite(rfm, rightSpeed);
  analogWrite(rbm, rightSpeed);
}

void setCenterMotorSpeed(int speed) {
  analogWrite(ctm, speed);
}

void loop() {
  //print readchannel value
  for (byte i = 0; i < 4; i++) {
    int value = readChannel(i, -100, 100, 0);
    debugSerial.print("Ch");
    debugSerial.print(i + 1);
    debugSerial.print(": ");
    debugSerial.println(value);
  }
  
  int ud = readChannel(1, -100, 100, 0);
  int lr = readChannel(0, -100, 100, 0);
  int cut = readChannel(2, 1, 100, 0);

  int leftSpeed = 0, rightSpeed = 0;

  if (ud > 0) {
    leftSpeed = rightSpeed = ud * MOTOR_SPEED_FACTOR;
  } else if (ud < 0) {
    leftSpeed = rightSpeed = ud * -MOTOR_SPEED_FACTOR;
  }

  if (lr < 0) {
    leftSpeed += lr * -MOTOR_SPEED_FACTOR;
    rightSpeed -= lr * -MOTOR_SPEED_FACTOR;
  } else if (lr > 0) {
    leftSpeed -= lr * -MOTOR_SPEED_FACTOR;
    rightSpeed += lr * -MOTOR_SPEED_FACTOR;
  }

  if (cut > 2) {
    setCenterMotorSpeed(cut * MOTOR_SPEED_FACTOR);
  } else {
    setCenterMotorSpeed(0);
  }

  setMotorSpeeds(leftSpeed, rightSpeed);
}
