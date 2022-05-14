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

Author: thingcontrol team  
Create Date: 23 July 2021. 
*/

#ifndef ThingControl_DL7612_h
#define ThingControl_DL7612_h

#include <Arduino.h>
#include <HardwareSerial.h>

class ThingControl_DL7612{
  public:
    ThingControl_DL7612();
    void sendCommand(String atComm);
    void sendData(int type, String data);
    String sendHexData(int type, String data);
    String sendp2pHexData(int type, String data);
    void receivep2pHexData();
    void setupModule();
    void checkModuleReady();
    void rebootModule();
    void ShowConfig();
    void connectNetwork();
    void restoreFactory();
    void setJoinABP();
    void setJoinOTAA();
    void setClassA();
    void setClassC();
    void setAdaptiveDataRate(String mode);
    void setTXPower(String power);
    void setConfirmMode(String mode);
    void setIsmBand(String mode);
    void setAppEui(String appeui);
    void setAppKey(String appkey);
    void setDevAddr(String devaddr);
    void setAppsKey(String appskey);
    void setNwksKey(String nwkskwy);
    void blankChk(String & val);
    void ConfigMode();
    void SaveConfig();
    void waitCommandResponse();
    String getMode();
    String getClass();
    String getAdaptiveDataRate();
    String getTXPower();
    String getConfirmMode();
    String getIsmBand();
    String getDevEui();
    String getAppEui();
    String getAppKey();
    String getDevAddr();
    String getAppsKey();
    String getNwksKey();
    String getCSQ();
  private:
    char _rxBuffer[64];    
};
#endif