#include <IBusBM.h>

IBusBM ibusRc;

HardwareSerial& ibusRcSerial = Serial1;
HardwareSerial& debugSerial = Serial;

// define pin
int lfm = 2;
int lbm = 3;
int rfm = 5;
int rbm = 6;
int ctm = 7;

void setup() {
  debugSerial.begin(74880);
  ibusRc.begin(ibusRcSerial);
  pinMode(lfm, OUTPUT);
  pinMode(lbm, OUTPUT);
  pinMode(rfm, OUTPUT);
  pinMode(rbm, OUTPUT);
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
  //print readchannel value
  for (byte i = 0; i<4; i++){
    int value = readChannel(i, -100, 100, 0);
    debugSerial.print("Ch");
    debugSerial.print(i + 1);
    debugSerial.print(": ");
    debugSerial.print(value);
    debugSerial.print(" ");
    debugSerial.println("");
  }
  int ud = readChannel(1, -100, 100, 0);
  int lr = readChannel(0, -100, 100, 0);
  int cut = readChannel(2, 1, 100, 0);

  if((ud > 0) && (lr == 0)){
    // only up
    analogWrite(lfm, (ud * 2.5));
    analogWrite(lbm, 0);
    analogWrite(rfm, (ud * 2.5));
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud < 0) && (lr == 0)){
    // only down
    analogWrite(lfm, 0);
    analogWrite(lbm, (ud * (-2.5)));
    analogWrite(rfm, 0);
    analogWrite(rbm, (ud * (-2.5)));
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud == 0) && (lr < 0)){
    // only left
    analogWrite(lfm, 0);
    analogWrite(lbm, 0);
    analogWrite(rfm, (lr * (-2.5)));
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud == 0) && (lr > 0)){
    // only right
    analogWrite(lfm, (lr * 2.5));
    analogWrite(lbm, 0);
    analogWrite(rfm, 0);
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud > 0) && (lr < 0)){
    // up left
    analogWrite(lfm, (ud * 1.2));
    analogWrite(lbm, 0);
    analogWrite(rfm, (ud * (-2.5)));
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud > 0) && (lr >0)){
    // up right
    analogWrite(lfm, (ud * 2.5));
    analogWrite(lbm, 0);
    analogWrite(rfm, (ud * 1.2));
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud < 0) && (lr < 0)){
    // down left
    analogWrite(lfm, 0);
    analogWrite(lbm, (ud * (-1.2)));
    analogWrite(rfm, 0);
    analogWrite(rbm, (ud * (-2.5)));
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud < 0) && (lr > 0)){
    // down right
    analogWrite(lfm, 0);
    analogWrite(lbm, (ud * (-2.5)));
    analogWrite(rfm, 0);
    analogWrite(rbm, (ud * (-1.2)));
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }else if((ud == 0) && (lr == 0)){
    // stop
    analogWrite(lfm, 0);
    analogWrite(lbm, 0);
    analogWrite(rfm, 0);
    analogWrite(rbm, 0);
    if (cut == 0){
      analogWrite(ctm, 0);
    }else if(cut > 2){
      analogWrite(ctm, (cut * 2.5));
    }
  }
}
