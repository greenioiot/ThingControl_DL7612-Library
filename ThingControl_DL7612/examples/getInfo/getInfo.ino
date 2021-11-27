/*
 *  Get Infomation from Maxiiot DL7612-AS923-TH Shield
 *  Date : 12/11/2021
*/
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>

ThingControl_DL7612 lora;

unsigned long previousMillis,currentMillis;
const unsigned long interval = 60000;

void setup() {
  Serial.begin(115200);
  lora.setupModule();
  lora.LoRaShowConfig();
}

void loop() {
  int portTransmit = random(1, 223);
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    lora.sendData(5,"HELLO");
    lora.sendHexData(2,"0A0B");    
    previousMillis = currentMillis;
  }
}
