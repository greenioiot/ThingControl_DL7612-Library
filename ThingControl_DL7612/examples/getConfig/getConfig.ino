/*
    Get configuration
    Maxiiot DL7612-AS923-TH Module/Shield by thingcontrol.io

    Author: thingcontrol.io team
    Date : 12/11/2021
*/
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>

ThingControl_DL7612 lora;

void setup() {
  Serial.begin(115200);
  lora.ShowConfig();
  delay(500);
}

void loop() {
}
