#include <IBusBM.h>

IBusBM ibusRc;

HardwareSerial& ibusRcSerial = Serial1;
HardwareSerial& debugSerial = Serial;

int lfmPin = 2;
int lbmPin = 3;
int rfmPin = 4;
int rbmPin = 5;

void setup() {
  debugSerial.begin(74880);
  ibusRc.begin(ibusRcSerial);
  pinMode(lfmPin, OUTPUT);
  pinMode(lbmPin, OUTPUT);
}

// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue){
  uint16_t ch = ibusRc.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Red the channel and return a boolean value
bool redSwitch(byte channelInput, bool defaultValue){
  int intDefaultValue = (defaultValue)? 100: 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

void loop() {
  for (byte i = 0; i<4; i++){
    int value = readChannel(i, -100, 100, 0);
    debugSerial.print("Ch");
    debugSerial.print(i + 1);
    debugSerial.print(": ");
    debugSerial.print(value);
    debugSerial.print(" ");
  }
  // front no cut
  if (readChannel(1, -100, 100, 0) > 0) && (readChannel(0, -100, 100, 0) == 0) && (readChannel(2, -100, 100, 0) == 0){
    analogWrite(lfmPin, 255);
    analogWrite(lbmPin, 0);
    analogWrite(rfmPin, 255);
    analogWrite(rbmPin, 0);
  }else if (readChannel(1, -100, 100, 0) < 0) && (readChannel(0, -100, 100, 0) == 0) && (readChannel(2, -100, 100, 0) == 0){
    analogWrite(lfmPin, 0);
    analogWrite(lbmPin, 255);
    analogWrite(rfmPin, 0);
    analogWrite(rbmPin, 255);
  }else{(readChannel(1, -100, 100, 0) == 0) && (readChannel(0, -100, 100, 0)== 0) && (readChannel(2, -100, 100, 0)== 0){
    // do nothing
    analogWrite(lfmPin, 0);
    analogWrite(lbmPin, 0);
    analogWrite(rfmPin, 0);
    analogWrite(rbmPin, 0);
    analogWrite(lfmPin, 0);
    analogWrite(lbmPin, 0);
    
  }
  debugSerial.print("Ch5: ");
  debugSerial.print(redSwitch(4, false));
  debugSerial.println();
  delay(10);
}
