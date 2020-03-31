#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
//test
ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

uint8_t pin_led = 2;
char* ssid = "wifinet";
char* password = "hesloludek";
#include "web_page.h"
void setup()
{
  pinMode(pin_led, OUTPUT);
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

  server.on("/", []() { server.send_P(200, "text/html", webpage); });
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

// Add the main program code into the continuous loop() function
void loop()
{


}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)                    
{
  if (type == WStype_TEXT) {
    if (payload[0] == '#') {
      uint16_t brightness =
          (uint16_t)strtol((const char*)&payload[1], NULL, 10);
      brightness = 1024 - brightness;
      analogWrite(pin_led, brightness);
      Serial.print("brightness= ");
      Serial.println(brightness);
    }

    else {
      for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
      Serial.println();
    }
  }
}