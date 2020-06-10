#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include "web_page.h"
#include "EEPROM.h"
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
#define DBG
// test 3

OneWire oneWire(D9);                 // inicializace OneWire
DallasTemperature ds18b20(&oneWire); // vytvo�en� objektu �idla teploty

uint64 rtc_counter;
uint64 out_timer;
#pragma region IO setting

typedef struct
{
  uint8_t num;
  uint8_t last_state; //posledni stav/zmena
  uint8_t id;
} Input;
Input Inputs[4];

typedef struct
{
  uint8_t num;
  uint8_t last_state; //posledni stav/zmena
  uint8_t id;
} Output;
Output Outputs[4];

uint8_t outs[4] = { D13, D12, D11,D10};
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
Output termostatOut;

typedef struct
{
  uint16_t casOn;
  uint16_t casOff;
} SpinackyStruct;

SpinackyStruct spinackyTable[4];
Output spinackyOut;

// typedef enum
// {
//   none = -1,
//   trvale = 0,
//   puls = 1,
//   casovac = 2
// } IoModeEnum;

typedef struct
{
  int8_t mode;
  Output* assoc_out;
  uint16_t time;
} IOctrlStruct;

IOctrlStruct IO_controls[3];

uint16 old_adc_val;
uint16 adc_val;
uint16_t old_temperature;
uint16_t temperature;
uint16_t minutes;
char *ssid = "MujNet";
char *password = "hesloludek";
//#include "web_page.h"

//////////////////////////////////////////////
void setup()
{
  EEPROM.begin(512);
  SetIO();
  ds18b20.begin();
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  int numofdallas = ds18b20.getDeviceCount();
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("pocet dallasu:");Serial.println(numofdallas);
  server.on("/", []() {
    server.send_P(200, "text/html", webpage);
  });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

// Add the main program code into the continuous loop() function
char msg[6];
unsigned long counter;
void loop()
{

 //cas
 if (millis() > counter)
 {
   ReadAdc();
   Termostat();
   counter = millis() + 1000;
   if (++rtc_counter % 60 == 0)
     minutes++;
#ifdef DBG
    char buff[28];
    sprintf(buff, "%02u:%02u:%02u", minutes / 60, minutes % 60, rtc_counter % 60);
    Serial.println(buff);
#endif
 }

 //   ///spinacky
 if (spinackyOut.num != 0)
   Spinacky();
 //   //termostat
 //   if (termostatOut.num != 0)
 //     Termostat();
 //web
 webSocket.loop();
 server.handleClient();
 TestVstupu();
}

void TestVstupu()
{

  
  for (size_t i = 0; i < 3; i++)
  {
    //kontrola jestli je seply casovany vystup
    if (IO_controls[i].mode == 2 && out_timer > 0)
    {
      if (IO_controls[i].assoc_out->last_state == HIGH)
      {
        if (millis()  > out_timer)
        {
        ChangeOutput(IO_controls[i].assoc_out, LOW);
        out_timer = 0;
        }
       
      }
    }

    if (!digitalRead(Inputs[i].num))
    {
      if (Inputs[i].last_state != 1)
      {
        sprintf(msg, "#I%don", Inputs[i].id);
#ifdef DBG
        Serial.println(msg);
#endif
        Inputs[i].last_state = 1;
        webSocket.broadcastTXT(msg, 5);
        if (IO_controls[i].mode == 0)
          ChangeOutput(IO_controls[i].assoc_out, HIGH);
        else if (IO_controls[i].mode == 1)
        {
#ifdef DBG
        Serial.println("mode1");
#endif          
          if(IO_controls[i].assoc_out->last_state == 0){ChangeOutput(IO_controls[i].assoc_out,HIGH);}
          else {ChangeOutput(IO_controls[i].assoc_out,LOW);}
        }
        else if (IO_controls[i].mode == 2)
        {
#ifdef DBG
        Serial.println("mode2");
#endif             
          out_timer = millis() + (IO_controls[i].time * 1000);
          ChangeOutput(IO_controls[i].assoc_out, HIGH);
        }
      }
    }

    else
    {
      if (Inputs[i].last_state != 0)
      {
        sprintf(msg, "#I%dof", Inputs[i].id);
#ifdef DBG
        Serial.println(msg);
#endif
        Inputs[i].last_state = 0;
        webSocket.broadcastTXT(msg, 5);
        if (IO_controls[i].mode == 0)
          ChangeOutput(IO_controls[i].assoc_out, LOW);
      }
    }
  }
}

void ReadTemp()
{
  char msg[10];
  ds18b20.requestTemperatures();
  float t = ds18b20.getTempCByIndex(0);
  temperature = (uint16_t)(t * 10);
  if (old_temperature != temperature)
  {
    old_temperature = temperature;
    sprintf(msg, "#T%02u,%1u", temperature/10,temperature%10);
    webSocket.broadcastTXT(msg, 6);

#ifdef DBG  
  Serial.print(t);
#endif  
  }
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {
    String data = "";
#ifdef DBG
    Serial.println("ok"); /***/
#endif
    if (payload[0] == '&')
    {

#ifdef DBG
      Serial.print("message lenght: "); /***/
      Serial.println(length);           /***/
#endif
      dataLenght = length;
      uint8_t delimiters[19];
      uint8_t index = 0;
      for (int i = 0; i < length; i++)
      {
        data += (char)payload[i];
        if (payload[i] == '*')
        {
          delimiters[index++] = i;
#ifdef DBG
          Serial.print(i);
          Serial.print(" ");
#endif
        }
      }
if(payload[1] == 'S')
{
  DataToPage();
  return;
}

if (payload[1] == 'T')
{
  String tstamp = data.substring(delimiters[0] + 1, delimiters[1]);
  minutes = tstamp.toInt();
  rtc_counter = minutes * 60;

#ifdef DBG
        char buf[15];
        String s = itoa(rtc_counter, buf, 10);
        Serial.println(tstamp);
        Serial.println(buf);
#endif
        return;
      }
#ifdef DBG
      Serial.print("data: "); /***/
      Serial.println(data);   /***/
      Serial.println("sub:"); /***/
#endif
      String sub = "";
      //tabulka termostatu
      //termostat table teplota 1
      sub = data.substring(delimiters[0] + 1, delimiters[1]);
#ifdef DBG
      Serial.println(sub); /***/
#endif
      termostatTable[0].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 1
      termostatTable[0].cas = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table teplota 2
      sub = data.substring(delimiters[1] + 1, delimiters[2]);
#ifdef DBG
      Serial.println(sub); /***/
#endif
      termostatTable[1].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 2
      termostatTable[1].cas = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table teplota 3
      sub = data.substring(delimiters[2] + 1, delimiters[3]);
#ifdef DBG
      Serial.println(sub); /***/
#endif
      termostatTable[2].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 3
      termostatTable[2].cas = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table teplota 4
      sub = data.substring(delimiters[3] + 1, delimiters[4]);
#ifdef DBG
      Serial.println(sub); /***/
#endif
      termostatTable[3].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 4
      termostatTable[3].cas = sub.substring(sub.indexOf('_') + 1).toInt();
#ifdef DBG
      Serial.println(); /***/
#endif
      // tabulka spinacek
      // spinacky table cas ON 1
      sub = data.substring(delimiters[5] + 1, delimiters[6]);
#ifdef DBG
      Serial.println(sub); /***/
#endif
      spinackyTable[0].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 1
      spinackyTable[0].casOff = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table cas ON 2
      sub = data.substring(delimiters[6] + 1, delimiters[7]);
#ifdef DBG
      Serial.println(sub);
#endif
      spinackyTable[1].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 2
      spinackyTable[1].casOff = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table cas ON 3
      sub = data.substring(delimiters[7] + 1, delimiters[8]);
#ifdef DBG
      Serial.println(sub);
#endif
      spinackyTable[2].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 3
      spinackyTable[2].casOff = sub.substring(sub.indexOf('_') + 1).toInt();
      // termostat table cas ON 4
      sub = data.substring(delimiters[8] + 1, delimiters[9]);
#ifdef DBG
      Serial.println(sub);
#endif
      spinackyTable[3].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 4
      spinackyTable[3].casOff = sub.substring(sub.indexOf('_') + 1).toInt();

      //vystup pro termostat
      sub = data.substring(delimiters[10] + 1, delimiters[11]);
#ifdef DBG
      Serial.println(sub);
#endif
      uint8_t idx = sub.toInt();
      termostatOut.num = outs[idx];
      termostatOut.id = idx + 1;
      //vystup pro spinacky
      sub = data.substring(delimiters[11] + 1, delimiters[12]);
#ifdef DBG
      Serial.println(sub);
#endif
      idx = sub.toInt();
      spinackyOut.num = outs[idx];
      spinackyOut.id = idx + 1;
      //IO nastaveni 1
      sub = data.substring(delimiters[13] + 1, delimiters[14]);
#ifdef DBG
      Serial.println(sub);
#endif
      IO_controls[0].mode = sub.substring(0, sub.indexOf('_')).toInt();
      idx = sub.substring(sub.indexOf('_') + 1, sub.indexOf('|')).toInt();
      IO_controls[0].assoc_out = &Outputs[idx];
      IO_controls[0].time = sub.substring(sub.indexOf('|') + 1).toInt();
      //IO nastaveni 2
      sub = data.substring(delimiters[14] + 1, delimiters[15]);
#ifdef DBG
      Serial.println(sub);
#endif
      IO_controls[1].mode = sub.substring(0, sub.indexOf('_')).toInt();
      idx =  sub.substring(sub.indexOf('_') + 1, sub.indexOf('|')).toInt();
      IO_controls[1].assoc_out = &Outputs[idx];
      IO_controls[1].time = sub.substring(sub.indexOf('|') + 1).toInt();
      //IO nastaveni 1
      sub = data.substring(delimiters[15] + 1, delimiters[16]);
#ifdef DBG
      Serial.println(sub);
#endif
      IO_controls[2].mode = sub.substring(0, sub.indexOf('_')).toInt();
      idx = sub.substring(sub.indexOf('_') + 1, sub.indexOf('|')).toInt();
      IO_controls[2].assoc_out = &Outputs[idx];
      IO_controls[2].time = sub.substring(sub.indexOf('|') + 1).toInt();
      //IO nastaveni 1
      sub = data.substring(delimiters[16] + 1, delimiters[17]);
#ifdef DBG
      Serial.println(sub);
#endif
      // IO_controls[3].mode = sub.substring(0, sub.indexOf('_')).toInt();
      // idx = sub.substring(sub.indexOf('_') + 1, sub.indexOf('|')).toInt();
      // IO_controls[3].assoc_out = &Outputs[idx];
      // IO_controls[3].time = sub.substring(sub.indexOf('|') + 1).toInt();
      PushToEeprom();
      PullFromEeprom();
    }

    else if (payload[0] == '#')
    {
#ifdef DBG
      Serial.print("IO ");
      Serial.print(payload[3]);
      Serial.print(payload[5]);
#endif

      switch (payload[3])
      {
      case '1':
        if (payload[5] == 'n')
          ChangeOutput(&Outputs[0], HIGH);
        else
          ChangeOutput(&Outputs[0], LOW);
        break;

      case '2':
        if (payload[5] == 'n')
          ChangeOutput(&Outputs[1], HIGH);
        else
          ChangeOutput(&Outputs[1], LOW);
        break;

      case '3':
        if (payload[5] == 'n')
          ChangeOutput(&Outputs[2], HIGH);
        else
          ChangeOutput(&Outputs[2], LOW);
        break;

      case '4':
        if (payload[5] == 'n')
          ChangeOutput(&Outputs[3], HIGH);
        else
          ChangeOutput(&Outputs[3], LOW);
        break;
      default:
        break;
      }

#ifdef DBG
  Serial.print(Outputs[0].last_state);Serial.print(Outputs[1].last_state);Serial.print(Outputs[2].last_state);Serial.println(Outputs[3].last_state);
#endif      
    }
  }
}

boolean outSpinIsSetOn = false, outSpinIsSetOff = false;
boolean outTermIsSetOn = false, outTermIsSetOff = false;

void Spinacky()
{
#ifdef DBG
  // Serial.print("min:");Serial.println(minutes);
#endif

  for (uint8_t i = 0; i < 4; i++)
  {
    if (spinackyTable[i].casOn == minutes)
    {
      if (outSpinIsSetOn == false)
      {
        outSpinIsSetOn = true;
        outSpinIsSetOff = false;
        ChangeOutput(&spinackyOut, HIGH);
      }
    }
    else if (spinackyTable[i].casOff == minutes)
    {
      if (outSpinIsSetOff == false)
      {
        outSpinIsSetOff = true;
        outSpinIsSetOn = false;
        ChangeOutput(&spinackyOut, LOW);
      };
    }
  }
}

void Termostat()
{
  uint8_t pom_i;
  ReadTemp();
  for (size_t i = 0; i < 4; i++)
  {
    pom_i = i + 1;
    if(i ==3)
      pom_i = 0;
    if (minutes >= termostatTable[i].cas && minutes < termostatTable[pom_i].cas)
    {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(termostatTable[i].cas);
      Serial.print("-");Serial.print(minutes);
     Serial.print("-/-");

          Serial.print(termostatTable[i].teplota);
      Serial.print("-");Serial.print(temperature);
    if (temperature < (termostatTable[i].teplota-5))
    {
      if(outTermIsSetOn == false)
      {
        outTermIsSetOn = true;
        outTermIsSetOff = false;
        ChangeOutput(&termostatOut, HIGH);
 #ifdef DBG       
        Serial.println(" tstaton");
 #endif
      }
    }

    else if (temperature >= termostatTable[i].teplota)
    {
      if(outTermIsSetOff == false)
      {
        outTermIsSetOn = false;
        outTermIsSetOff = true;
        ChangeOutput(&termostatOut, LOW);
 #ifdef DBG       
        Serial.println(" tstatoff");
 #endif        
      }
    }
    break;
    }
}


}

void PushToEeprom()
{
#ifdef DBG
  Serial.println("push EE");
#endif
  int start_point = 0;
  EEPROM.put(start_point, termostatTable);
  start_point += 16;//sizeof(termostatTable);
delay(10);
#ifdef DBG
  Serial.print(start_point);
  Serial.print(">");
#endif
  EEPROM.put(start_point, spinackyTable);
 delay(10);  
  start_point += 16;//sizeof(spinackyTable);
#ifdef DBG
  Serial.print(start_point);
  Serial.print(">");
#endif
  EEPROM.put(start_point, termostatOut);
  delay(10);
  start_point += sizeof(Output);
#ifdef DBG
  Serial.print(start_point);
  Serial.print(">");
#endif
  EEPROM.put(start_point, spinackyOut);
  delay(10);
  start_point += sizeof(Output);
#ifdef DBG
  Serial.println(start_point);
#endif
  EEPROM.put(start_point, IO_controls);
  delay(10);
}

void PullFromEeprom()
{
#ifdef DBG
  Serial.print("pull EE? ");
#endif
  uint8_t c;
  for (int i = 0; i < 46; i++)
  {
    c = EEPROM.read(i);
#ifdef DBG
    Serial.print(c);
    Serial.print(" ");
#endif
  }
  int start_point = 0;
  EEPROM.get(start_point, termostatTable);
  start_point += sizeof(termostatTable);
  EEPROM.get(start_point, spinackyTable);
  start_point += sizeof(spinackyTable);
  EEPROM.get(start_point, termostatOut);
  start_point += sizeof(Output);
  EEPROM.get(start_point, spinackyOut);
  start_point += sizeof(Output);
  EEPROM.get(start_point, IO_controls);
#ifdef DBG
//Serial.print("data");
//DataToPage();
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
  Serial.print(termostatOut.num);
  Serial.print("x");
  Serial.println(spinackyOut.num);
  for (int i = 0; i < 3; i++)
  {
    Serial.print(IO_controls[i].mode);
    Serial.print("-");
    Serial.print(IO_controls[i].assoc_out->num);
    Serial.print("-");
    Serial.println(IO_controls[i].time);
  }
#endif
}

void DataToPage()
{
//&#T*200_60*200_120*200_180*200_240*#S*60_120*180_240*300_360*420_480*#O*0*1*#IO*0_2|0*1_3|0*-1_-1|0*
String send_data = "&S*";
send_data += String(termostatTable[0].cas) + "_";
send_data += String(termostatTable[0].teplota)+ "*";
send_data += String(termostatTable[1].cas) + "_";
send_data += String(termostatTable[1].teplota)+ "*";
send_data += String(termostatTable[2].cas) + "_";
send_data += String(termostatTable[3].teplota)+ "*";
send_data += String(termostatTable[3].cas) + "_";
send_data += String(termostatTable[3].teplota)+ "*#S*";
send_data += String(spinackyTable[0].casOn) + "_";
send_data += String(spinackyTable[0].casOff)+ "*";
send_data += String(spinackyTable[1].casOn) + "_";
send_data += String(spinackyTable[1].casOff)+ "*";
send_data += String(spinackyTable[2].casOn) + "_";
send_data += String(spinackyTable[2].casOff)+ "*";
send_data += String(spinackyTable[3].casOn) + "_";
send_data += String(spinackyTable[3].casOff)+ "*#O*";
sint8_t j = 0;
for (size_t i = 0; i < 4; i++)if(outs[i] == termostatOut.num )j=i;
send_data += String(j)+ "*";
j = 0;
for (size_t i = 0; i < 4; i++)if(outs[i] == spinackyOut.num )j=i;
send_data += String(j)+ "*#IO*";
send_data += String(IO_controls[0].mode)+ "_"; 
j = -1;
for (size_t i = 0; i < 4; i++)if(IO_controls[0].assoc_out == Outputs + i )j=i;
send_data += String(j)+ "|";
send_data += String(IO_controls[0].time)+ "*";
send_data += String(IO_controls[1].mode)+ "_";
j = -1;
for (size_t i = 0; i < 4; i++)if(IO_controls[1].assoc_out == Outputs + i )j=i;
send_data += String(j)+ "|";
send_data += String(IO_controls[1].time)+ "*";
send_data += String(IO_controls[2].mode)+ "_";
j = -1;
for (size_t i = 0; i < 4; i++)if(IO_controls[2].assoc_out == Outputs + i )j=i;
send_data += String(j)+ "|";
send_data += String(IO_controls[2].time)+ "*";
#ifdef DBG
Serial.println("topage");
Serial.println(send_data);
#endif
char msg[sizeof(send_data)];
send_data.toCharArray(msg, sizeof(send_data));
webSocket.broadcastTXT(msg, sizeof(msg));
}

void ChangeOutput(Output *_out, uint8_t val)
{
  uint8_t x = _out->id;
   char msg[10];
   sprintf(msg, "#O%d%s", _out->id, val == 1 ? "on" : "of");
#ifdef DBG
  Serial.println(msg);
#endif
 _out->last_state = val;
  digitalWrite(_out->num, val);
#ifdef DBG
  Serial.println(_out->last_state);
#endif
   webSocket.broadcastTXT(msg, 5);
}

void SetIO()
{
  //pinMode(D2, INPUT);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);

  pinMode(D13, OUTPUT);
  pinMode(D12, OUTPUT);
  pinMode(D11, OUTPUT);
  pinMode(D10, OUTPUT);

  Inputs[0].num = D2;
  Inputs[0].last_state = 0;
  Inputs[0].id = 1;
  Inputs[1].num = D3;
  Inputs[1].last_state = 0;
  Inputs[1].id = 2;
  Inputs[2].num = D4;
  Inputs[2].last_state = 0;
  Inputs[2].id = 3;
  // Inputs[3].num = D4;
  // Inputs[3].last_state = 0;
  // Inputs[3].id = 4;
  Outputs[0].num = D13;
  Outputs[0].id = 1;
  Outputs[0].last_state = 0;
  Outputs[1].num = D12;
  Outputs[1].id = 2;
  Outputs[1].last_state = 0;
  Outputs[2].num = D11;
  Outputs[2].id = 3;
  Outputs[2].last_state = 0;
  Outputs[3].num = D10;
  Outputs[3].id = 4;
  Outputs[3].last_state = 0;
  IO_controls[0].assoc_out = &Outputs[0];
  IO_controls[1].assoc_out = &Outputs[1];
  IO_controls[2].assoc_out = &Outputs[2];
  // IO_controls[3].assoc_out = &Outputs[3];
}

void ReadAdc()
{
  char msg[10];
  adc_val = analogRead(A0);
  if(old_adc_val != adc_val)
  {
    old_adc_val = adc_val;
    sprintf(msg, "#A%04u", adc_val);
  //  webSocket.broadcastTXT(msg, 6);
// #ifdef DBG
//   Serial.println(msg);
// #endif
  }

}