/*############################ An example send data via LoRaWAN (LoRa Gateway)############################################
   Using thingcontrol board V 1.0 read ambient temperature and humidity values from an XY-MD02 sensor via Modbus RTU send
   to LoRaWAN with OTAA Mode and Class A by LoRa shield (thingcontrol)
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
String deveui, payload01;

void setup() {
  Serial.begin(115200);
  modbus.begin(buadrate1, configParam1, rxPin1, txPin1);
  node.begin(ID_SENSOR, modbus);
  lora.ShowConfig();
  delay(500);
  deveui = lora.getDevEui();
  Serial.println(F("Starting... LoRaWAN"));
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
  /* Decode
    case 0x0673:// Device EUI  8 Byte
    case 0x0267:// Temperature (0-100)  2 Byte
    case 0x0768:// Humidity (0-100)  2 Byte

    example data
    sensorData = "067311112222333316CE026700FA07680171";
                  0673 11112222333316ce 0267 00fa 0768 0171
    sensorData = '0673' + deveui + '0267' + sensor.temperature + '0768' + sensor.moisture
  */
  int dataLength;
  String sensorData;
  String devcode = "0673";
  String tempcode = "0267";
  String humicode = "0768";
  sensorData = devcode + deveui + tempcode + convert2Hex(sensor.temp) + humicode + convert2Hex(sensor.hum);
  dataLength = sensorData.length();
  payload01 = lora.sendHexData(dataLength / 2 , sensorData);
    if (payload01 != ("N/A")) {
    byte index = payload01.indexOf(F(","));
    payload01 = payload01.substring(index + 1, payload01.length());
    Serial.print("Receive Payload : ");
    Serial.println(payload01);
  }
}
