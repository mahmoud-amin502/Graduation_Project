#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial client(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&client);//Connect RX,TX,PWR,
String ID = "sens50";
String reading1 = "{\"contextRegistrations\": [{\"entities\": [{\"type\": \"thing\",\"isPattern\": \"false\",\"id\":\"";
String reading2 = "\"}],\"attributes\": [{\"name\": \"longitude\",\"type\": \"float\",\"isDomain\": \"false\"},{\"name\": \"latitude\",\"type\": \"float\",\"isDomain\": \"false\"}],\"providingApplication\": \"http://130.206.125.128:1026/ngsi10\"}]}";
//char *reading3 = reading1 + ID + reading2;
//const char *reading = reading3.c_str();
//String reading = "{\"contextRegistrations\": [{\"entities\": [{\"type\": \"thing\",\"isPattern\": \"false\",\"id\": \"sensore50\"}],\"attributes\": [{\"name\": \"temperature\",\"type\": \"int\",\"isDomain\": \"false\"}],\"providingApplication\": \"http://mysensors.com/Rooms\"}],\"duration\": \"P1M\"}";


void setup()
{
  client.begin(9600);
  Serial.begin(9600);
  delay(500);
//  initSIM();
  Con();
  connectGPRS();
  connectHTTP();
}


void Con()
{
  reading1+=String(ID);
  reading1+=reading2;
  Serial.println(reading1);
}

void loop()
{

}

void connectGPRS()
{

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

  client.println("AT+HTTPPARA=\"URL\",\"http://130.206.121.137:8065/ngsi9/registerContext\"");//Public server IP address
  delay(2000);
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

