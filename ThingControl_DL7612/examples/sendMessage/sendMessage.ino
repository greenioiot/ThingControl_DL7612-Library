/*  
 *  Send a message by HEX and ASCII format 
 *  Maxiiot DL7612-AS923-TH Module/Shield by thingcontrol.io
 *  
 *  Author: thingcontrol.io team
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
  delay(500);
}

void loop() {
  int portTransmit = random(1, 223);
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // Send a message by ASCII format
    lora.sendData(5,"HELLO");    
    // Send a message by HEX format
    lora.sendHexData(2,"0A0B");
    previousMillis = currentMillis;
  }
}
