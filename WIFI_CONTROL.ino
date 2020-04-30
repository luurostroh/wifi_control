#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "web_page.h"
#include "EEPROM.h"
// test 3
#pragma region   IO setting
uint8_t Inputs[4] = {2,3,4,5};
uint8_t Outputs[4]= {12,13,10,11};
#pragma endregion
int dataLenght;
ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);
typedef struct
{
  uint16_t teplota;
  uint16_t cas;
} TermostatStruct;

TermostatStruct termostatTable[4];
uint8_t termostatOut;
typedef struct
{
  uint16_t casOn;
  uint16_t casOff;
} SpinackyStruct;

SpinackyStruct spinackyTable[4];
uint8_t spinackyOut;

uint16_t minutes;
uint8_t pin_led = 2;
char *ssid = "MujNet";
char *password = "hesloludek";
//#include "web_page.h"

//////////////////////////////////////////////
void setup()
{
  EEPROM.begin(512);
  SetIO();
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send_P(200, "text/html", webpage);  
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
 
}

// Add the main program code into the continuous loop() function
void loop() 
{  
  webSocket.loop();
  server.handleClient();
  for (size_t i = 0; i < 4; i++)
  {
    if(!digitalRead(Inputs[i]))
    {

    }
  }
  
}

 void ReadTemp()
 {
 }
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {
Serial.println("ok");/***/
    if (payload[0] == '&')
    {
      String data = "";
Serial.print("message lenght: ");/***/
Serial.println(length);/***/
dataLenght = length;
uint8_t delimiters[13];
uint8_t index = 0;
for (int i = 0; i < length; i++)
{
  data += (char)payload[i];
  if (payload[i] == '*')
  {
    delimiters[index++] = i;
    Serial.print(i);
    Serial.print(" ");
  }
      }
Serial.print("data: ");/***/
Serial.println(data);/***/
Serial.println("sub:");/***/
      String sub = "";
	  //tabulka termostatu
	  //termostat table teplota 1
      sub = data.substring(delimiters[0]+1, delimiters[1]);
Serial.println(sub);/***/
      termostatTable[0].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 1
      termostatTable[0].cas = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table teplota 2
      sub = data.substring(delimiters[1]+1, delimiters[2]);
Serial.println(sub);/***/
      termostatTable[1].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 2
      termostatTable[1].cas = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table teplota 3
      sub = data.substring(delimiters[2]+1, delimiters[3]);
Serial.println(sub);/***/
      termostatTable[2].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 3
      termostatTable[2].cas = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table teplota 4
      sub = data.substring(delimiters[3]+1, delimiters[4]);
Serial.println(sub);/***/
      termostatTable[3].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 4
      termostatTable[3].cas = sub.substring(sub.indexOf('_')+1).toInt();
Serial.println();/***/
	  // tabulka spinacek
      // spinacky table cas ON 1
      sub = data.substring(delimiters[5]+1, delimiters[6]);
Serial.println(sub);/***/
      spinackyTable[0].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 1
      spinackyTable[0].casOff = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table cas ON 2
      sub = data.substring(delimiters[6]+1, delimiters[7]);
Serial.println(sub);
      spinackyTable[1].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 2
      spinackyTable[1].casOff = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table cas ON 3
      sub = data.substring(delimiters[7]+1, delimiters[8]);
Serial.println(sub);
      spinackyTable[2].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 3
      spinackyTable[2].casOff = sub.substring(sub.indexOf('_')+1).toInt();
      // termostat table cas ON 4
      sub = data.substring(delimiters[8]+1, delimiters[9]);
Serial.println(sub);
      spinackyTable[3].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 4
      spinackyTable[3].casOff = sub.substring(sub.indexOf('_')+1).toInt();

      //vystup pro termostat
Serial.println(payload[delimiters[10]+ 1]);
      termostatOut = payload[delimiters[10]+ 1] - 0x30;
      //vystup pro spinacky
Serial.println(payload[delimiters[11]+ 1]);
      spinackyOut = payload[delimiters[11]+ 1] - 0x30;

      PushToEeprom();
      PullFromEeprom();

    }

    else if(payload[0]=='#')
    {
#ifdef DBG
      Serial.println("IO");
#endif
     switch (payload[3])
     {
     case '1':
       if(payload[5]=='n')ChangeOutput(Outputs[0], HIGH);        
       else ChangeOutput(Outputs[0], LOW);
       break;

     case '2':
       if(payload[5]=='n')ChangeOutput(Outputs[1], HIGH);        
       else ChangeOutput(Outputs[1], LOW);
       break;

     case '3':
       if(payload[5]=='n')ChangeOutput(Outputs[2], HIGH);        
       else ChangeOutput(Outputs[2], LOW);
       break;

     case '4':
       if(payload[5]=='n')ChangeOutput(Outputs[3], HIGH);        
       else ChangeOutput(Outputs[3], LOW);
       break;
     default:
       break;
     }
    }
  }
}

void Spinacky()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    if (spinackyTable[i].casOn >= minutes && !digitalRead(spinackyOut))
    {
      digitalWrite(spinackyOut, HIGH);
    }
    else if (spinackyTable[i].casOff >= minutes && digitalRead(spinackyOut))
    {
      digitalWrite(spinackyOut, LOW);
    }
  }
}

void PushToEeprom()
{

  Serial.println();
      int start_point = 0;
      EEPROM.put(start_point,termostatTable);
      start_point += sizeof(termostatTable); Serial.print(start_point);Serial.print(">");
      EEPROM.put(start_point,spinackyTable);
      start_point += sizeof(spinackyTable);Serial.print(start_point);Serial.print(">");
      EEPROM.put(start_point,termostatOut);
      start_point ++;Serial.print(start_point);Serial.println(">");
      EEPROM.put(start_point,spinackyOut);
}

void PullFromEeprom()
{
     Serial.print("EE? ");
     uint8_t c;
  for (int i = 0; i < 34; i++)
  {
    c=EEPROM.read(i);
    Serial.print(c);Serial.print(" ");
  }
      int start_point = 0;
      EEPROM.get(start_point,termostatTable);
      start_point += sizeof(termostatTable);
      EEPROM.get(start_point,spinackyTable);
      start_point += sizeof(spinackyTable);
      EEPROM.get(start_point,termostatOut );
      start_point ++;
      EEPROM.get(start_point,spinackyOut);
Serial.println();      
for (int i = 0; i < 4; i++)
{
Serial.print(termostatTable[i].teplota);
Serial.print("-");
Serial.println(termostatTable[i].cas);
}
for (int i = 0; i < 4; i++)
{
Serial.print(spinackyTable[i].casOn);
Serial.print("-");
Serial.println(spinackyTable[i].casOff);
}
      Serial.print(termostatOut);
      Serial.print("x");
      Serial.println(spinackyOut);
  
}

void DataToPage()
{

 //webSocket.broadcastTXT(c, sizeof(c));
}

void ChangeOutput(uint8_t pin, uint8_t val)
{
  digitalWrite(pin, val);
}

void SetIO()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

}