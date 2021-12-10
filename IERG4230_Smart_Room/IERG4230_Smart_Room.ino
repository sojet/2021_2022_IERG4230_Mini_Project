#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "AM2320.h"

//Define the connection to hotspot
#ifndef STASSID
//mobile hotspot
#define STASSID "ssha"
#define STAPSK "goodmorning3"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

//This is for calling the function of AM2320 for getting humidity and temperture
AM2320 th;

//API Keys
//Write a channel feed
const char* write_channel = "http://api.thingspeak.com/update?api_key=FIUY5IXW0Y1LDKYZ&";
//Read a channel feed
const char* read_channel = "http://api.thingspeak.com/channels/1603157/feeds.json?api_key=HSB377D2S5LD0I0K&";
//Read a channel field
//https://api.thingspeak.com/channels/1603157/fields/1.json?api_key=HSB377D2S5LD0I0K&results=2
//Read channel status change
//https://api.thingspeak.com/channels/1603157/status.json?api_key=HSB377D2S5LD0I0K

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
//  WiFi.begin(ssid, password);

}

void loop() {
  // wait for WiFi connection
//    if ((WiFi.status() == WL_CONNECTED)) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://api.thingspeak.com/update?api_key=FIUY5IXW0Y1LDKYZ&field1=0&field2=1");  
    int httpCode = http.GET();

    if (httpCode>0){
      String payload = http.getString();
      Serial.println(httpCode);
      //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      Serial.println(payload);
    }
    http.end();
    
    }
    
//    Serial.print("[HTTP] begin...\n");
//    if (http.begin(client, url_on)) {  // HTTP
//
//
//      Serial.print("[HTTP] GET...\n");
//      // start connection and send HTTP header
//      int httpCode = http.GET();
//
//      // httpCode will be negative on error
//      if (httpCode > 0) {
//        // HTTP header has been send and Server response header has been handled
//        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//
//        // file found at server
//        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
//          String payload = http.getString();
//          Serial.println(payload);
//        }
//      } else {
//        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//      }
//
//      http.end();
//    } else {
//      Serial.printf("[HTTP} Unable to connect\n");
//    }

  delay(1000);
}
