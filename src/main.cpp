#include <Arduino.h>
#include "WiFi.h"
#include "AsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>
#include <string>

#include <HTTPClient.h>


#define LED 15


float value;
String lastMessage = "";
AsyncWebServer server(80);
// void recvMsg(uint8_t *data, size_t len){
//   WebSerial.println("Received Data...");
//   String d = "";
//   for(int i=0; i < len; i++){
//     d += char(data[i]);
//   }
//   WebSerial.println(d);
//   if (d == "ON"){
//     digitalWrite(LED, HIGH);
//   }
//   if (d=="OFF"){
//     digitalWrite(LED, LOW);
//   }
// }



void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(LED, OUTPUT);


  const char* ssid = "Carpenters";
  const char* password = "1201carp";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {}
  delay(100);

  Serial.println("Setup done");
  Serial.println(WiFi.localIP());

  WebSerial.begin(&server);
  //WebSerial.msgCallback(recvMsg);
  server.begin();



  

}

void loop() {
  
  delay(250);

  
  int value = analogRead(32);

  while (WiFi.status() != WL_CONNECTED) {
  Serial.println("Reconnecting to WiFi...");
  WiFi.reconnect();
  delay(1000);
  if (WiFi.status() != WL_CONNECTED){
    continue;
  }
  Serial.println("Reconnected: " + WiFi.localIP());
  }
  

  if(value < 300) {
    digitalWrite(LED, HIGH);
    if(lastMessage!="Light On") {
      lastMessage = "Light On";
      WebSerial.println(lastMessage);
      }
  }
  else {
    digitalWrite(LED, LOW);
    if(lastMessage!="Light Off") {
      HTTPClient http;
      http.begin("http://192.168.86.58:8000/addLightData/?brightness="+String(value));  
      lastMessage = "Light Off";
      WebSerial.println(lastMessage);
      http.addHeader("method", "GET");
      http.addHeader("scheme", "http");
      int httpResponseCode = http.GET();
       if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }






      http.end();

      }
  }
  
   
}