/*############################ An example send data via LoRa (P2P) to LoRa Bridge (LoRa (P2P)) ##########################
   Using thingcontrol board V 1.0 read ambient temperature and humidity values from an XY-MD02 sensor via Modbus RTU send
   to LoRa Bridge (LoRa (P2P)) by LoRa shield (thingcontrol)
  ########################################################################################################################*/
#include <ModbusMaster.h>
#include <HardwareSerial.h>
#include <ThingControl_DL7612.h>
#include "REG_CONFIG.h"

HardwareSerial modbus(1);
ThingControl_DL7612 lora;

ModbusMaster node;

struct xy
{
  String temp;
  String hum;
};

xy sensor;
String deveui;

void setup() {
  Serial.begin(115200);
  modbus.begin(buadrate1, configParam1, rxPin1, txPin1);
  node.begin(ID_SENSOR, modbus);
  lora.ShowConfig();
  delay(500);
  deveui = lora.getDevEui();
  Serial.println(F("Starting... LoRa P2P"));
  Serial.print("Device EUI  : ");
  Serial.println(deveui);
  Serial.println(F("***********************************"));
}

void loop() {
  read_Modbus(reg_ADDR);
  sendSensor();
  delay(60000);
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

void read_Modbus(uint16_t REG)
{
  uint8_t j, result;
  uint16_t data[2];
  float ftemp, fhum;
  // slave: read (6) 16-bit registers starting at register 2 to RX buffer
  result = node.readInputRegisters(REG, 2);
  // do something with data if read is successful
  if (result == node.ku8MBSuccess)
  {
    for (j = 0; j < 2; j++)
    {
      data[j] = node.getResponseBuffer(j);
    }
  }
  sensor.temp = data[0];
  sensor.hum =  data[1];
  ftemp = data[0];
  fhum = data[1];

  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(ftemp / 10);
  Serial.print(" C");
  Serial.print(" Humidity: ");
  Serial.print(fhum / 10);
  Serial.println("%");
  delay(2000);
}

void sendSensor() {
  /* 
    Device EUI 8 Byte
    Temperature (0-100)  2 Byte
    Humidity (0-100)  2 Byte
  */
  int dataLength;
  String sensorData;
  sensorData = deveui + convert2Hex(sensor.temp) + convert2Hex(sensor.hum);
  dataLength = sensorData.length();
  lora.sendp2pHexData(dataLength / 2 , sensorData);
}
