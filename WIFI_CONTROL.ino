#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
// test 2
ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);
typedef struct {
  uint16_t teplota;
  uint16_t cas;
} TermostatStruct;

TermostatStruct termostatTable[4];
typedef struct {
  uint16_t casOn;
  uint16_t casOff;
} SpinackyStruct;

SpinackyStruct spinackyTable[4];
uint8_t pin_led = 2;
char *ssid = "wifinet";
char *password = "hesloludek";
#include "web_page.h"
void setup() {
  pinMode(pin_led, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() { server.send_P(200, "text/html", webpage); });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

// Add the main program code into the continuous loop() function
void loop() {}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload,
                    size_t length) {
  if (type == WStype_TEXT) {
    if (payload[0] == '#') {
      String data = String(*payload);
      uint8_t delimiters[10];
      uint8_t index = 0;
      for (int i = 0; i < length; i++) {
        if (payload[i] == '*') delimiters[index++] = i;
      }
      String sub = "";
	  //tabulka termostatu
	  //termostat table teplota 1
      sub = data.substring(delimiters[0], delimiters[1]);
      termostatTable[0].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 1
      termostatTable[0].cas = sub.substring(sub.indexOf('_')).toInt();
      // termostat table teplota 2
      sub = data.substring(delimiters[1], delimiters[2]);
      termostatTable[1].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 2
      termostatTable[1].cas = sub.substring(sub.indexOf('_')).toInt();
      // termostat table teplota 3
      sub = data.substring(delimiters[2], delimiters[3]);
      termostatTable[2].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 3
      termostatTable[2].cas = sub.substring(sub.indexOf('_')).toInt();
      // termostat table teplota 4
      sub = data.substring(delimiters[3], delimiters[4]);
      termostatTable[3].teplota = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas 4
      termostatTable[3].cas = sub.substring(sub.indexOf('_')).toInt();

	  // tabulka spinacek
      // spinacky table cas ON 1
      sub = data.substring(delimiters[0], delimiters[1]);
      spinackyTable[0].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 1
      spinackyTable[0].casOff = sub.substring(sub.indexOf('_')).toInt();
      // termostat table cas ON 2
      sub = data.substring(delimiters[1], delimiters[2]);
      spinackyTable[1].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 2
      spinackyTable[1].casOff = sub.substring(sub.indexOf('_')).toInt();
      // termostat table cas ON 3
      sub = data.substring(delimiters[2], delimiters[3]);
      spinackyTable[2].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 3
      spinackyTable[2].casOff = sub.substring(sub.indexOf('_')).toInt();
      // termostat table cas ON 4
      sub = data.substring(delimiters[3], delimiters[4]);
      spinackyTable[3].casOn = sub.substring(0, sub.indexOf('_')).toInt();
      // termostat table cas OFF 4
      spinackyTable[3].casOff = sub.substring(sub.indexOf('_')).toInt();
    }

    else {
      for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
      Serial.println();
    }
  }
}
