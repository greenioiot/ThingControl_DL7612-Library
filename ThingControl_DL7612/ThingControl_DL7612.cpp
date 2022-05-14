/*
Copyright (c) 2021, GreenIO Co., LTd.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

AT Command Dictionary for DL7612 version 1.7
support DL7612 with thingcontrol Board
LoRa with AT command

Author: thingcontrol.io team  
Create Date: 23 July 2021. 
*/

#include <Arduino.h>
#include "ThingControl_DL7612.h"
#include <HardwareSerial.h>

#define rxPin                   14
#define txPin                   27
#define buadrate                115200
#define configParam             SERIAL_8N1

//Serial configuration
HardwareSerial loraSerial(2);

byte printResponse = 1;  // if printResponse = 1 or 0
String data_input;
unsigned int previous_check = 0;

/****************************************/
/**        Initialization Module       **/
/****************************************/

ThingControl_DL7612::ThingControl_DL7612() {
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
     if (printResponse)
   {
      Serial.println(response);
   }  
}
//***************************************************************************
void ThingControl_DL7612::sendData(int type, String data) {
  Serial.println("*******TEXT Build");
  String command = (String)
                   "AT+NCMGS=" + type + "," + data + (String)
                   "\r\n";
  sendCommand(command);
}
//***************************************************************************
String ThingControl_DL7612::sendHexData(int type, String data) {
  String npayload;
  String atComm = (String)
                   "AT+NMGS=" + type + "," + data + (String)
                   "\r\n";
  loraSerial.flush();
  loraSerial.print(atComm + "\r\n");
//  Serial.println("*******AT Command Sent");
  Serial.print(atComm + "\r\n");
  delay(300);

  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+NNMI:")) != -1) {
        byte index = data_input.indexOf(F(":"));
        npayload = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  npayload.replace(F("\""), "");
  npayload.trim();
  npayload.toUpperCase();

  blankChk(npayload);
  data_input = "";
  return npayload;
}

//***************************************************************************
String ThingControl_DL7612::sendp2pHexData(int type, String data) {
  String pppayload;
  String atComm = (String)
                   "AT+PMGS=" + type + "," + data + (String)
                   "\r\n";
  loraSerial.flush();
  loraSerial.print(atComm + "\r\n");
//  Serial.println("*******AT Command Sent");
  Serial.print(atComm + "\r\n");
  delay(300);

  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+PNMI:")) != -1) {
        byte index = data_input.indexOf(F(":"));
        pppayload = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  pppayload.replace(F("\""), "");
  pppayload.trim();
  pppayload.toUpperCase();

  blankChk(pppayload);
  data_input = "";
  return pppayload;
}

//***************************************************************************
void ThingControl_DL7612::receivep2pHexData() {
  String command = (String)
                   "AT+PMGR"
                   "\r\n";
  sendCommand(command);
}
//***************************************************************************
void ThingControl_DL7612::setupModule() {
  Serial.println(F(">>Rebooting ."));
  rebootModule();
  Serial.println(F(">>Check module status "));
  checkModuleReady();
}
//***************************************************************************
void ThingControl_DL7612::checkModuleReady() {
  loraSerial.println(F("AT"));
  waitCommandResponse();
  Serial.println(F("..OK"));  
}
//***************************************************************************
void ThingControl_DL7612::rebootModule() {
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
//***************************************************************************
void ThingControl_DL7612::ShowConfig() {
  Serial.print("Joint Mode : "); Serial.println(getMode());
  Serial.print("Class : "); Serial.println(getClass());
  Serial.print("Device EUI : "); Serial.println(getDevEui());
  Serial.print("Application EUI : "); Serial.println(getAppEui());
  Serial.print("Application Key : "); Serial.println(getAppKey());
  Serial.print("DEvice Address : "); Serial.println(getDevAddr());
  Serial.print("Application Seesion Key : "); Serial.println(getAppsKey());
  Serial.print("Network Seesion Key : "); Serial.println(getNwksKey());
  Serial.print("ISM Band : "); Serial.println(getIsmBand());
  Serial.print("Confirm Mode : "); Serial.println(getConfirmMode());
  Serial.print("Adaptive Data Rate: "); Serial.println(getAdaptiveDataRate());
  Serial.print("TX Power : "); Serial.println(getTXPower());
  Serial.print("CSQ : "); Serial.println(getCSQ());
  Serial.println();
}
//***************************************************************************
// Attach network : 1 connected, 0 disconnected
// Attach network : OK connected,
void ThingControl_DL7612::connectNetwork() {
  loraSerial.println(F("AT+CGATT"));
  waitCommandResponse();
  Serial.println(F("Connected Network"));
}

//***************************************************************************
void ThingControl_DL7612::restoreFactory() {
  ConfigMode();
  loraSerial.println(F("AT+ RESTORE"));
  waitCommandResponse();
  SaveConfig();
  Serial.println(F("Restore Factory Parameters"));
}

//********************* configure ABP ******************************************************
// 0 = ABP, 1 = OTAA
void ThingControl_DL7612::setJoinABP() {
  ConfigMode();
  loraSerial.println(F("AT+ACTIVATE=0"));
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Joined : "));Serial.println(getMode());
}
//************************ configure OTAA ***************************************************
// 0 = ABP, 1 = OTAA
void ThingControl_DL7612::setJoinOTAA() {
  ConfigMode();
  loraSerial.println(F("AT+ACTIVATE=1"));
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Joined : "));Serial.println(getMode());
}
//************************ configure A Class ************************************************
// A, B, C
void ThingControl_DL7612::setClassA() {
  ConfigMode();
  loraSerial.println(F("AT+CLASS=A"));
  waitCommandResponse();
  SaveConfig();
  Serial.println(F("Class : "));Serial.println(getClass());
}
//************************ configure C Class ************************************************
// A, B, C
void ThingControl_DL7612::setClassC() {
  ConfigMode();
  loraSerial.println(F("AT+CLASS=C"));
  waitCommandResponse();
  SaveConfig();
  Serial.println(F("Class : "));Serial.println(getClass());
}
//************************ configure Adaptive Data Rate ************************************************
// 0 disable adaptive data rate, 1 enable adaptive data rate (default)
void ThingControl_DL7612::setAdaptiveDataRate(String mode) 
{
  ConfigMode();
  String command = (String)
                 "AT+ADR=" + mode + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Adaptive Data Rate Set To : "));Serial.println(mode);
}
/************************ configure TX power ************************************************
0: 20dBm (default)
1: 14dBm
2: 11dBm
3: 8dBm
4: 5dBm
5: 2dBm
*/
void ThingControl_DL7612::setTXPower(String mode) {
  ConfigMode();
  String command = (String)
                 "AT+POWER=" + mode + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("TX power Set To : "));Serial.println(mode);
}
//************************ configure Confirm Massage Mode ************************************************
// 0 unconfirm frame mode, 1 confirm frame mode (default)
void ThingControl_DL7612::setConfirmMode(String mode) {
  ConfigMode();
  String command = (String)
                 "AT+CFM=" + mode + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Confirm Massage Mode Set To : "));Serial.println(mode);
}
/************************ configure ISM Band of Lora TM ************************************************
0 US902_928, include channel 0 to channel 71
1 AS923, include channel 0 to channel 15
2 EU863_870, include channel 0 to channel 15
3 CN779_787, include channel 0 to channel 15
4 CN470_510, include channel 0 to channel 95
5 EU433, include channel 0 to channel 15
6 CUSTOMIZE, include channel 0 to channel 15
*/
void ThingControl_DL7612::setIsmBand(String mode) {
  ConfigMode();
  String command = (String)
                 "AT+ISMBAND=" + mode + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("ISM BAND Set To : "));Serial.println(mode);
}
//***************************************************************************
void ThingControl_DL7612::setAppEui(String data) {
  ConfigMode();
  String command = (String)
                 "AT+APPEUI=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Application EUI Set To : "));Serial.println(data);
}
//**************************************************************************
void ThingControl_DL7612::setAppKey(String data) {
  ConfigMode();
  String command = (String)
                 "AT+APPKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Application Key Set To : "));Serial.println(data);
}
//***************************************************************************
void ThingControl_DL7612::setDevAddr(String data) {
  ConfigMode();
  String command = (String)
                 "AT+ADDR=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Device Address Set To : "));Serial.println(data);
}
//***************************************************************************
void ThingControl_DL7612::setAppsKey(String data) {
  ConfigMode();
  String command = (String)
                 "AT+APPSKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Application Session Key Set To : "));Serial.println(data);
}
//***************************************************************************
void ThingControl_DL7612::setNwksKey(String data) {
  ConfigMode();
  String command = (String)
                 "AT+NWKSKEY=" + data + (String)
                 "\r\n";
  loraSerial.print(command + "\r\n");  
  waitCommandResponse();
  SaveConfig();
  Serial.print(F("Network Session Key Set To : "));Serial.println(data);
}
//***************************************************************************
void ThingControl_DL7612::blankChk(String & val) {
  if (val == "") {
    val = "N/A";
  }
}
//***************************************************************************
void ThingControl_DL7612::ConfigMode()
{
  loraSerial.println(F("AT+DEBUG=1"));
  waitCommandResponse();
  Serial.println(F("Start Config Mode"));
}
//***************************************************************************
void ThingControl_DL7612::SaveConfig()
{
  loraSerial.println(F("AT+SAVE"));
  waitCommandResponse();
  Serial.println(F("Saved Config"));
}
//******************** wait for response from module *************************************
void ThingControl_DL7612::waitCommandResponse()
{
  unsigned long currentTime,startTime;
  byte c=0;
  memset(_rxBuffer,0x00,64);    //clear buffer
  startTime = millis(); 
  while (loraSerial.available() == 0)
  {
    currentTime = millis();
    if (currentTime > (startTime + 3000))       //exit wait routine if nor response in 3 seconds
    {
      Serial.println(F("No Resposnse from Lora Module"));
      return;
    }
  }
  
  while (loraSerial.available()) 
    {
      _rxBuffer[c] = loraSerial.read();
      c++;
    }
   if (_rxBuffer[0]=='O' && _rxBuffer[1]=='K')     //check if response is OK
    {
      return; 
    }
   if (printResponse)
   {
      Serial.print(_rxBuffer);
   }
}
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
String ThingControl_DL7612::getAdaptiveDataRate() {
  loraSerial.flush();
  String Mode;
  loraSerial.println(F("AT+ADR"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+ADR")) != -1) {
        byte index = data_input.indexOf(F("+ADR"));
        Mode = data_input.substring(index + 1);
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
    Mode = "Disable";
  }
  else if (Mode == "1") {
    Mode = "Enable";
  }
  data_input = "";
  return Mode;
}
//***************************************************************************
String ThingControl_DL7612::getTXPower() {
  loraSerial.flush();
  String Mode;
  loraSerial.println(F("AT+POWER"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+POWER")) != -1) {
        byte index = data_input.indexOf(F("+POWER"));
        Mode = data_input.substring(index + 1);
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
    Mode = "20dBm";
  }
  else if (Mode == "1") {
    Mode = "14dBm";
  }
  else if (Mode == "2") {
    Mode = "11dBm";
  }
  else if (Mode == "3") {
    Mode = "8dBm";
  }
  else if (Mode == "4") {
    Mode = "5dBm";
  }
  else if (Mode == "5") {
    Mode = "2dBm";
  }
  data_input = "";
  return Mode;
}
//***************************************************************************
String ThingControl_DL7612::getConfirmMode() {
  loraSerial.flush();
  String Mode;
  loraSerial.println(F("AT+CFM"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+CFM")) != -1) {
        byte index = data_input.indexOf(F("+CFM"));
        Mode = data_input.substring(index + 1);
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
    Mode = "unconfirm frame mode";
  }
  else if (Mode == "1") {
    Mode = "confirm frame mode (default)";
  }
  data_input = "";
  return Mode;
}
//***************************************************************************
String ThingControl_DL7612::getIsmBand() {
  loraSerial.flush();
  String Mode;
  loraSerial.println(F("AT+ISMBAND"));
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+ISMBAND")) != -1) {
        byte index = data_input.indexOf(F("+ISMBAND"));
        Mode = data_input.substring(index + 1);
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
    Mode = "US902_928";
  }
  else if (Mode == "1") {
    Mode = "AS923";
  }
  else if (Mode == "2") {
    Mode = "EU863_870";
  }
  else if (Mode == "3") {
    Mode = "CN779_787";
  }
  else if (Mode == "4") {
    Mode = "CN470_510";
  }
  else if (Mode == "5") {
    Mode = "EU433";
  }
  else if (Mode == "6") {
    Mode = "CUSTOMIZE";
  }
  data_input = "";
  return Mode;
}
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
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
//***************************************************************************
String ThingControl_DL7612::getCSQ() {
  loraSerial.flush();
  String csq;
  loraSerial.println(F("AT+CSQ"));
  bool chk = false;
  while (1) {
    if (loraSerial.available()) {
      data_input = loraSerial.readStringUntil('\n');
      if (data_input.indexOf(F("+CSQ")) != -1) {
        byte index = data_input.indexOf(F(":"));
        csq = data_input.substring(index + 1, data_input.length());
      }
      else if (data_input.indexOf(F("OK")) != -1) {
        break;
      }
    }
  }
  csq.replace(F("\""), "");
  csq.trim();
  csq.toUpperCase();

  blankChk(csq);
  data_input = "";
  return csq;
}