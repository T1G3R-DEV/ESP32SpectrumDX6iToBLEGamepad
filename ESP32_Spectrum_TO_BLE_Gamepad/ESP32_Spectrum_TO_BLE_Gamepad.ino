//BLE Gamepad library https://github.com/lemmingDev/ESP32-BLE-Gamepad
#include <BleGamepad.h>

BleGamepad bleGamepad;

//pin on the esp32
int interruptPin = 17;

unsigned int rT=0;
int ch[6];
int lastUpdated=0;
bool updated=false;

void IRAM_ATTR fI();

void IRAM_ATTR rI() {
  rT=micros();
  attachInterrupt(interruptPin, fI, FALLING);
}
void IRAM_ATTR fI() {
  unsigned int fT = micros();
  if (fT - rT >=5000)
  {
    lastUpdated=0;
    attachInterrupt(interruptPin, rI, RISING);
    return;
  }
  ch[lastUpdated % 6]=(fT - rT-690)*38;
  if (lastUpdated % 6 ==5 )
    updated=true;
  ++lastUpdated;
  
  attachInterrupt(interruptPin, rI, RISING);
}

void setup() {
    for(int i=0; i<6; i++){
      ch[i]=0;
    }
    Serial.begin(115200);
    attachInterrupt(interruptPin, rI, RISING);
    bleGamepad.begin();
}

void loop() {
  if (bleGamepad.isConnected())
  {
    if(updated)
    { 
      updated=false;
      bleGamepad.setAxes(ch[3], ch[0], 0, ch[1], ch[2], ch[4], ch[5], 0);
    }
    delay(1);
  }
}
