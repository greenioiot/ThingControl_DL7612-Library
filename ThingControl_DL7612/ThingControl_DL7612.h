#ifndef ThingControl_DL7612_h
#define ThingControl_DL7612_h

#include <Arduino.h>
#include <HardwareSerial.h>

class ThingControl_DL7612{
  public:
    ThingControl_DL7612();
    void sendCommand(String atComm);
    void sendData(int type, String data);
    void sendHexData(int type, String data);
    void setupModule();
    void check_module_ready();
    void reboot_module();
    void LoRaShowConfig();
    void connectNetwork();
    void setJoinABP();
    void setJoinOTAA();
    void setClassA();
    void setClassC();
    void setAppEui(String appeui);
    void setAppKey(String appkey);
    void setDevAddr(String devaddr);
    void setAppsKey(String appskey);
    void setNwksKey(String nwkskwy);
    String getMode();
    String getClass();
    String getDevEui();
    String getAppEui();
    String getAppKey();
    String getDevAddr();
    String getAppsKey();
    String getNwksKey();
    void blankChk(String & val); 
  private:
    char _rxBuffer[64];    
};
#endif