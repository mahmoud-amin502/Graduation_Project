#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>
#define PIN_TX    10
#define PIN_RX    11

SoftwareSerial client(PIN_TX, PIN_RX);
DFRobot_SIM808 sim808(&client);//Connect RX,TX,PWR,

String numm = "+";
String Longitude = "30.123456";
String Latitude = "31.123456";
bool flag1 = true;
bool flag2 = true;
char response[100];

const char string_0[] PROGMEM = "{\"longitude\": {\"value\":";   
const char string_1[] PROGMEM = ",\"type\": \"Float\"},\"latitude\": {\"value\":";
const char string_2[] PROGMEM = ",\"type\": \"Float\"}}";
const char string_3[] PROGMEM = "{\"contextRegistrations\": [{\"entities\": [{\"type\": \"thing\",\"isPattern\": \"false\",\"id\":\"" ;
const char string_4[] PROGMEM = "\"}],\"attributes\": [{\"name\": \"longitude\",\"type\": \"float\",\"isDomain\": \"false\"},{\"name\": \"latitude\",\"type\": \"float\",\"isDomain\": \"false\"}],\"providingApplication\": \"http://130.206.125.128:1026/ngsi10\"}]}";
const char string_5[] PROGMEM = "{\"id\":\"";
const char string_6[] PROGMEM = "\",\"type\": \"thing\",\"longitude\": {\"value\": 30.027923,\"type\": \"Float\"},\"latitude\": {\"value\": 31.241547,\"type\": \"Float\"}}";
const char* const string_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5,string_6};
char buffer[300];

void setup() {
  client.begin(9600);
  Serial.begin(9600);

  //******** Initialize sim808 module *************
  while (!sim808.init()) {
    delay(1000);
    Serial.print("Sim808 init error\r\n");
  }
  connectGPRS();
  //Con1();
  //delay(2000);
  //Con2();
  //delay(2000);
  //Con3();
  //getGPSdata();
  getnumber();
  number();
  connectHTTP_C_register();
  delay(5000);
  connectHTTP_D_register();
  delay(5000);
  //iupdate();

}



void loop() {
  
  iupdate();
  flag1 = true;
  
}



void getnumber()
{
  client.println(F("AT+HTTPINIT"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"CID\",1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+CUSD=1,\"*878#\""));
  delay(5000);
  for (int i = 0 ; client.available() > 0 && i < 100  ; i++) {
    response[i] = client.read();
  }
  client.println(F("AT+HTTPTERM"));
  delay(1000);
}


void number()
{
  for (int i = 47; i < 59; i++) {
    numm += response[i];
  }
  Serial.println(numm);
}













void connectGPRS()
{
  client.println(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+SAPBR=3,1,\"APN\",\"\""));//APN
  delay(1000);
  ShowSerialData();

  client.println(F("AT+SAPBR=1,1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+SAPBR=2,1"));
  delay(1000);
  ShowSerialData();
}






///////////////////////////////////////////////////////// ALL OTHER FUNCTIONS ///////////////////////////////////////////////////

void getGPSdata() {
  //************** Get GPS data *******************
  if ( sim808.attachGPS())
    Serial.println("Open the GPS power success");
  else
    Serial.println("Open the GPS power failure");
  while (flag1) {
    if (sim808.getGPS()) {
      float latit = (sim808.GPSdata.lat);
      float longit = (sim808.GPSdata.lon);
      Longitude = String(longit, 6);
      Latitude = String(latit, 6);
      Serial.println(Longitude);
      Serial.println(Latitude);

      //************* Turn off the GPS power ************
      sim808.detachGPS();
      flag1 = false;
    }
  }
}



void connectHTTP_update()
{
  String u1="";
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[0])));
  u1+=buffer;
  u1+=Longitude;
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[1])));
  u1+=buffer;
  u1+=Latitude;
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[2])));
  u1+=buffer;
  Serial.println(u1);
  Serial.println(u1.length());
  delay(3000);


  
  client.println(F("AT+HTTPINIT"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"CID\",1"));
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"URL\",\"http://130.206.125.128:1026/v2/entities/" + numm + "/attrs\""); //Public server IP address
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  delay(1000);
  ShowSerialData();


  client.println(F("AT+HTTPDATA=98,100000"));
  delay(1000);
  ShowSerialData();

  client.println(u1);
  delay(1000);
  ShowSerialData;

  client.println(F("AT+HTTPACTION =1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPREAD"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPTERM"));
  delay(1000);
  ShowSerialData;
}


//////////////////////////////////////////////////////register context//////////////////////////////////////////////////////////////////

void connectHTTP_C_register()
{
  String c1="";
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[5])));
  c1+=buffer;
  c1+=numm;
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[6])));
  c1+=buffer;
  Serial.println(c1);
  Serial.println(c1.length());
  delay(3000);


  
  client.println(F("AT+HTTPINIT"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"CID\",1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"URL\",\"http://130.206.125.128:1026/v2/entities\""));//Public server IP address
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  delay(1000);
  ShowSerialData();


  client.println(F("AT+HTTPDATA=137,100000"));
  delay(1000);
  ShowSerialData();
  
  client.println(c1);
  delay(1000);
  ShowSerialData;

  client.println(F("AT+HTTPACTION=1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPREAD"));
  delay(1000);
  ShowSerialData();

}

///////////////////////////////////////register discovery//////////////////////////////////////////////////////////////////

void connectHTTP_D_register()
{

  String d1="";
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[3])));
  d1+=buffer;
  d1+=numm;
  strcpy_P(buffer, (char*)pgm_read_word(&(string_table[4])));
  d1+=buffer;
  Serial.println(d1);
  Serial.println(d1.length());
  delay(3000);



  

  client.println(F("AT+HTTPPARA=\"URL\",\"http://130.206.121.137:8065/ngsi9/registerContext\""));//Public server IP address
  delay(2000);
  ShowSerialData();


  client.println(F("AT+HTTPPARA=\"CONTENT\",\"application/json\""));
  delay(1000);
  ShowSerialData();


  client.println(F("AT+HTTPDATA=295,100000"));
  delay(1000);
  ShowSerialData();
  
  client.println(d1);
  delay(1000);
  ShowSerialData;

  client.println(F("AT+HTTPACTION=1"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPREAD"));
  delay(1000);
  ShowSerialData();

  client.println(F("AT+HTTPTERM"));
  delay(1000);
  ShowSerialData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void iupdate() {
  getGPSdata();
  delay(2000);
  connectHTTP_update();
  delay(2000);
}

//////////////////////////////////////// Printing Function /////////////////////////////////////

void ShowSerialData()
{
  while (client.available() != 0)
  {
    Serial.write(client.read());
  }
}

