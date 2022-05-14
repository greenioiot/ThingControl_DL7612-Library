/*############################ An example receive data from LoRa Bridge (LoRa (P2P)) ##########################
   Using thingcontrol board V 1.0 receive data from LoRa Bridge (LoRa (P2P)) by LoRa shield (thingcontrol)
  ########################################################################################################################*/
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>
#include "REG_CONFIG.h"

#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

ThingControl_DL7612 lora;
String deveui, payload01, token01;
struct dataDecode
{
  String deveui;
  String temp;
  String hum;
};

dataDecode datadec;

#define WIFI_AP ""
#define WIFI_PASSWORD ""

String deviceToken = "XXXXXXXXXXXX";

char thingsboardServer[] = "mqtt.thingcontrol.io";

String json = "";

WiFiClientSecure wifiClient;
PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
int PORT = 8883;

void setup() {
  Serial.begin(115200);
  deveui = lora.getDevEui();
  Serial.println(F("Starting... LoRa Bridge"));
  Serial.print("Device EUI  : ");
  Serial.println(deveui);
  Serial.println(F("***********************************"));

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect("@Thingcontrol_AP")) {
    Serial.println("failed to connect and hit timeout");
    delay(1000);
  }
  client.setServer( thingsboardServer, PORT );
  reconnectMqtt();
}

void loop() {
  status = WiFi.status();
  if ( status == WL_CONNECTED)
  {
    if ( !client.connected() )
    {
      reconnectMqtt();
    }
    client.loop();
  }
  sendSensor();
  delay(3000);
}

String convert2Hex(String tempdata) {
  unsigned long datatemp;
  int dataLength;
  String sensorData;
  datatemp = tempdata.toInt();
  sensorData = String(datatemp, HEX);
  dataLength = sensorData.length();
  if (dataLength == 3) {
    sensorData = "0" + sensorData;
  } else if (dataLength == 2) {
    sensorData = "00" + sensorData;
  } else if (dataLength == 1) {
    sensorData = "000" + sensorData;
  } else  if (sensorData == 0) {
    sensorData = "0000";
  }
  return sensorData;
}

void sendSensor() {
  /*
    Device EUI 8 Byte
    Temperature (0-100)  2 Byte
    Humidity (0-100)  2 Byte
  */
  int dataLength;
  String sensorData;
  sensorData = deveui + "00100020";
  dataLength = sensorData.length();
  payload01 = lora.sendp2pHexData(dataLength / 2 , sensorData);
  if (payload01 != ("N/A")) {
    byte index = payload01.indexOf(F(","));
    payload01 = payload01.substring(index + 1, payload01.length());
    Serial.print("Receive Payload : ");
    Serial.println(payload01);    
    sensorDataDecode(payload01);
    sendtelemetry();
  }
}

void sensorDataDecode(String hexStr) {
  String data_str = hexStr;
      datadec.deveui = data_str.substring(1,16);
      datadec.temp = data_str.substring(17,20);
      datadec.hum = data_str.substring(21,24);
  }

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setWiFi()
{
  Serial.println("OK");
  Serial.println("+:Reconfig WiFi  Restart...");
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  if (!wifiManager.startConfigPortal("ThingControlCommand"))
  {
    Serial.println("failed to connect and hit timeout");
    delay(5000);
  }
  Serial.println("OK");
  Serial.println("+:WiFi Connect");
  client.setServer( thingsboardServer, PORT );  // secure port over TLS 1.2
  reconnectMqtt();
}

void processTele(char jsonTele[])
{
  char *aString = jsonTele;
  Serial.println("OK");
  Serial.print(F("+:topic v1/devices/me/ , "));
  Serial.println(aString);
  client.publish( "v1/devices/me/telemetry", aString);
}

void reconnectMqtt()
{
  if ( client.connect("Thingcontrol_AT", deviceToken.c_str(), NULL) )
  {
    Serial.println( F("Connect MQTT Success."));
    client.subscribe("v1/devices/me/rpc/request/+");
  }
}

void sendtelemetry()
{
  String json = "";
  json.concat("{\"deveui\":\"");
  json.concat(datadec.deveui);
  json.concat("\",\"temp\":\"");
  json.concat(datadec.temp);
  json.concat("\",\"hum\":\"");
  json.concat(datadec.hum);
  json.concat("\"}");
  Serial.println(json);
  // Length (with one extra character for the null terminator)
  int str_len = json.length() + 1;
  // Prepare the character array (the buffer)
  char char_array[str_len];
  // Copy it over
  json.toCharArray(char_array, str_len);
  processTele(char_array);
}
