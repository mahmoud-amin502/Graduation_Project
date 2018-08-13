#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial client(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&client);//Connect RX,TX,PWR,
String Longitude = "33.024469";
String Latitude = "32.210909";
String reading1 = "{\"latitude\": {\"value\":";
String reading2 = ",\"type\": \"Float\"},\"longitude\": {\"value\":";
String reading3 = ",\"type\": \"Float\"}}";

void setup()
{
  Serial.begin(9600);
  client.begin(9600);
  delay(500);
  Con();
  connectGPRS();
  connectHTTP();
}




void loop()
{

}



void Con()
{
  reading1 += String(Longitude);
  reading1 += reading2;
  reading1 += String(Latitude);
  reading1 += reading3;
}


void connectGPRS()
{
  Serial.println(reading1);
  delay(1000);
  ShowSerialData;





  
  client.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=3,1,\"APN\",\"\"");//APN
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

  client.println("AT+HTTPPARA=\"URL\",\"http://130.206.125.128:1026/v2/entities/+20000/attrs\"");//Public server IP address
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  ShowSerialData();


  client.println("AT+HTTPDATA=" + String(reading1.length()) + ",100000");
  delay(1000);
  ShowSerialData();

  client.println(reading1);
  delay(1000);
  ShowSerialData;

  client.println("AT+HTTPACTION =1");
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

