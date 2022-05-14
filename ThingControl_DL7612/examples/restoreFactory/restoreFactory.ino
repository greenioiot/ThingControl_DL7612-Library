/*
 *  Restore product factory parameters
 *  Maxiiot DL7612-AS923-TH Module/Shield by thingcontrol.io
 *  
 *  Author: thingcontrol.io team
 *  Date : 12/5/2022
*/
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>

ThingControl_DL7612 lora;

void setup() {
  Serial.begin(115200);
  lora.restoreFactory();
  delay(500);
  lora.ShowConfig();
}

void loop() {
}
