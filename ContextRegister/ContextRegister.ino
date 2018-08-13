#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial client(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&client);//Connect RX,TX,PWR,
String ID = "sens50";
String reading_c1 = "{\"id\":\"";
String reading_c2 = "\",\"type\": \"thing\",\"longitude\": {\"value\": 30.027923,\"type\": \"Float\"},\"latitude\": {\"value\": 31.241547,\"type\": \"Float\"}}";

void setup()
{
  Serial.begin(9600);
  client.begin(9600);
  delay(500);
  //initSIM();
  Con3();
  connectGPRS();
  connectHTTP();
}

void loop()
{

}

void Con3() {
  reading_c1 += String(ID);
  reading_c1 += reading_c2;
  Serial.println(reading_c1);
}

void connectGPRS()
{
  client.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=3,1,\"APN\",\"www\"");//APN
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=1,1");
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=2,1");
  delay(1000);
  ShowSerialData();
}

void connectHTTP()
{
  client.println("AT+HTTPINIT");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"URL\",\"http://130.206.125.128:1026/v2/entities\"");//Public server IP address
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  ShowSerialData();


  client.println("AT+HTTPDATA=" + String(reading_c1.length()) + ",100000");
  delay(1000);
  ShowSerialData();

  client.println(reading_c1);
  delay(1000);
  ShowSerialData;

  client.println("AT+HTTPACTION=1");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPREAD");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPTERM");
  delay(1000);
  ShowSerialData;
}

void ShowSerialData()
{
  while (client.available() != 0)
  {
    Serial.write(client.read());
  }
}

