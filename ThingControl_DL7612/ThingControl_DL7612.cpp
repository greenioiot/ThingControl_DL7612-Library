#include <Arduino.h>
#include "ThingControl_DL7612.h"
#include <HardwareSerial.h>

#define rxPin                   14
#define txPin                   27
#define buadrate                115200
#define configParam             SERIAL_8N1

//Serial configuration
HardwareSerial loraSerial(2);

byte printResponse;
String data_input;
unsigned int previous_check = 0;

/****************************************/
/**        Initialization Module       **/
/****************************************/

ThingControl_DL7612::ThingControl_DL7612(){
   loraSerial.begin(115200,  SERIAL_8N1, 14, 27);
}

//***************************************************************************
void ThingControl_DL7612::sendCommand(String atComm) {
  String response;
  loraSerial.print(atComm + "\r\n");
  Serial.println("*******AT Command Sent");
  Serial.print(atComm + "\r\n");
  delay(300);
  while (loraSerial.available()) {
    char ch = loraSerial.read();
    response += ch;
  }
  Serial.println(response);
}

void ThingControl_DL7612::sendData(int type, String data) {
  Serial.println("*******TEXT Build");
  String command = (String)
                   "AT+NCMGS=" + type + "," + data + (String)
                   "\r\n";
  sendCommand(command);
}

void ThingControl_DL7612::sendHexData(int type, String data) {
  Serial.println("*******HEX Build");
  String command = (String)
                   "AT+NMGS=" + type + "," + data + (String)
                   "\r\n";
  sendCommand(command);
}

void ThingControl_DL7612::setupModule() {
  Serial.print(F(">>Rebooting ."));
  reboot_module();
  Serial.println(F(">>Check module status "));
  check_module_ready();
}

void ThingControl_DL7612::check_module_ready() {
  int count = 0;
  previous_check = millis();
  loraSerial.println(F("AT"));
  delay(100);
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
}

void ThingControl_DL7612::reboot_module() {
  loraSerial.println(F("AT+NRB"));
  delay(100);
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("..OK"));
        break;
      }
    }
  }
}

void ThingControl_DL7612::LoRaShowConfig()
{
  Serial.print("Joint Mode : "); Serial.println(getMode());
  Serial.print("Class : "); Serial.println(getClass());
  Serial.print("Device EUI : "); Serial.println(getDevEui());
  Serial.print("Application EUI : "); Serial.println(getAppEui());
  Serial.print("Application Key : "); Serial.println(getAppKey());
  Serial.print("DEvice Address : "); Serial.println(getDevAddr());
  Serial.print("Application Seesion Key : "); Serial.println(getAppsKey());
  Serial.print("Network Seesion Key : "); Serial.println(getNwksKey());
  Serial.println();
}

// Attach network : 1 connected, 0 disconnected
void ThingControl_DL7612::connectNetwork() {
  loraSerial.println(F("AT+CGATT"));
  for (int i = 0; i < 30; i++) {
    if (loraSerial.available()) {
      data_input =  loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) break;
      else if (data_input.indexOf(F("ERROR")) != -1) break;
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setJoinABP() {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  loraSerial.println(F("AT+ACTIVATE=0")); // 0 = ABP, 1 = OTAA
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);

  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setJoinOTAA() {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  loraSerial.println(F("AT+ACTIVATE=1")); // 0 = ABP, 1 = OTAA
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
  
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setClassA() {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  loraSerial.println(F("AT+CLASS=A"));
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);

  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setClassC() {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  loraSerial.println(F("AT+CLASS=C"));
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);

  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setAppEui(String data) {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  String command = (String)
                 "AT+APPEUI=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
    while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setAppKey(String data) {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  String command = (String)
                 "AT+APPKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
    while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setDevAddr(String data) {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  String command = (String)
                 "AT+ADDR=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
    while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setAppsKey(String data) {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  String command = (String)
                 "AT+APPSKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
    while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

void ThingControl_DL7612::setNwksKey(String data) {
  int count = 0;
  loraSerial.println(F("AT+DEBUG=1"));
  delay(100);
  String command = (String)
                 "AT+NWKSKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");
  delay(100);
  loraSerial.println(F("AT+SAVE"));
  delay(100);
    while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("OK")) != -1) {
        Serial.println(F("...OK"));
        break;
      }
    }
    else {
      unsigned int current_check = millis();
      if (current_check - previous_check > 5000) {
        if (count > 5) Serial.print(F("\nError to connect LoRA Module, rebooting..."));
        previous_check = current_check;
        Serial.print(F("."));
        loraSerial.println(F("AT"));
        delay(100);
        count++;
      }
    }
  }
  Serial.print(F("."));
}

String ThingControl_DL7612::getMode() {
  loraSerial.flush();
  String Mode;
  loraSerial.println(F("AT+NCONFIG"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("ACTIVATE")) != -1) {
        byte index = data_input.indexOf(F("ACTIVATE"));
        Mode = data_input.substring(index + 9);
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  Mode.replace(F("\""), "");
  Mode.trim();
  blankChk(Mode);
  if (Mode == "0") {
    Mode = "ABP";
  }
  else if (Mode == "1") {
    Mode = "OTAA";
  }
  data_input = "";
  return Mode;
}

String ThingControl_DL7612::getClass() {
  loraSerial.flush();
  String Class;
  loraSerial.println(F("AT+NCONFIG"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("CLASS")) != -1) {
        byte index = data_input.indexOf(F("CLASS"));
        Class = data_input.substring(index + 6);
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  Class.replace(F("\""), "");
  Class.trim();
  Class.toUpperCase();

  blankChk(Class);
  data_input = "";
  return Class;
}

String ThingControl_DL7612::getDevEui() {
  loraSerial.flush();
  String DevEui;
  loraSerial.println(F("AT+DEVEUI"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+DevEUI")) != -1) {
        byte index = data_input.indexOf(F(":"));
        DevEui = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  DevEui.replace(F("\""), "");
  DevEui.trim();
  DevEui.toUpperCase();

  blankChk(DevEui);
  data_input = "";
  return DevEui;
}

String ThingControl_DL7612::getAppEui() {
  loraSerial.flush();
  String AppEui;
  loraSerial.println(F("AT+APPEUI"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+APPEUI")) != -1) {
        byte index = data_input.indexOf(F(":"));
        AppEui = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  AppEui.replace(F("\""), "");
  AppEui.trim();
  AppEui.toUpperCase();

  blankChk(AppEui);
  data_input = "";
  return AppEui;
}

String ThingControl_DL7612::getAppKey() {
  loraSerial.flush();
  String AppKey;
  loraSerial.println(F("AT+APPKEY"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+APPKEY")) != -1) {
        byte index = data_input.indexOf(F(":"));
        AppKey = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  AppKey.replace(F("\""), "");
  AppKey.trim();
  AppKey.toUpperCase();

  blankChk(AppKey);
  data_input = "";
  return AppKey;
}

String ThingControl_DL7612::getDevAddr() {
  loraSerial.flush();
  String DevAddr;
  loraSerial.println(F("AT+ADDR"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+ADDR")) != -1) {
        byte index = data_input.indexOf(F(":"));
        DevAddr = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  DevAddr.replace(F("\""), "");
  DevAddr.trim();
  DevAddr.toUpperCase();

  blankChk(DevAddr);
  data_input = "";
  return DevAddr;
}

String ThingControl_DL7612::getAppsKey() {
  loraSerial.flush();
  String AppsKey;
  loraSerial.println(F("AT+APPSKEY"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+APPSKEY")) != -1) {
        byte index = data_input.indexOf(F(":"));
        AppsKey = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  AppsKey.replace(F("\""), "");
  AppsKey.trim();
  AppsKey.toUpperCase();

  blankChk(AppsKey);
  data_input = "";
  return AppsKey;
}

String ThingControl_DL7612::getNwksKey() {
  loraSerial.flush();
  String NwksKey;
  loraSerial.println(F("AT+NWKSKEY"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+NWKSKEY")) != -1) {
        byte index = data_input.indexOf(F(":"));
        NwksKey = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  NwksKey.replace(F("\""), "");
  NwksKey.trim();
  NwksKey.toUpperCase();

  blankChk(NwksKey);
  data_input = "";
  return NwksKey;
}

void ThingControl_DL7612::blankChk(String & val) {
  if (val == "") {
    val = "N/A";
  }
}