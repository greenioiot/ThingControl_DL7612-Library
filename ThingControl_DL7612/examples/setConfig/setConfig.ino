/*
 *  Set configure
 *  Maxiiot DL7612-AS923-TH Module/Shield by thingcontrol.io
 *  
 *  Author: thingcontrol.io team
 *  Date : 12/11/2021
*/
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>

ThingControl_DL7612 lora;

String devAddr='12345678'
String devEui='1234567890123456'
String nwksKey='12345678901234567890123456789012'
String AppsKey='12345678901234567890123456789012'
String AppEui='1234567890123456'
String AppKey='12345678901234567890123456789012'

void setup() {
  Serial.begin(115200);
  lora.setupModule();
  delay(500);
  // begin Configure Mode
  lora.ConfigMode();
  lora.setJoinABP(); // or setJoinOTAA();
  lora.setClassA(); // or setClassC();
  lora.setAppEui(AppEui); // or lora.setAppEui('1234567890123456');
  lora.setAppKey(AppsKey); // or lora.setAppKey('12345678901234567890123456789012');
  lora.setDevAddr(devAddr); // or lora.setDevAddr('12345678');
  lora.setAppsKey(AppsKey); // or lora.setAppsKey('12345678901234567890123456789012');
  lora.setNwksKey(nwksKey); // or lora.setNwksKey('12345678901234567890123456789012');
  // Save Configure
  lora.SaveConfig();
  lora.ShowConfig();
}

void loop() {
}
