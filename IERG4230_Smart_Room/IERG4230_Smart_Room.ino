#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "AM2320.h"
#include <stdio.h> 
#include "TPLink.h"

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

//Light Sensor Setup
const int ledPin = 16;         // GPIO16(D0)
const int ldrPin_analog = A0;  // ADC
const int ldrPin_digital = 12; // GPIO12(D6)

//This is for calling the function of AM2320 for getting humidity and temperture
AM2320 th;

//TP-Link SmartPlug
char* hs110_1 = "192.168.137.194"; //It should be found on mobile hotspot and be changed for each connection
TPL_SmartPlug SP1(hs110_1);

//API Keys
//Write a channel feed
const String write_channel = "http://api.thingspeak.com/update?api_key=FIUY5IXW0Y1LDKYZ&";
//Read a channel feed
const String read_channel = "http://api.thingspeak.com/channels/1603157/feeds.json?api_key=HSB377D2S5LD0I0K&";
//Read a channel field
//https://api.thingspeak.com/channels/1603157/fields/1.json?api_key=HSB377D2S5LD0I0K&results=2
//Read channel status change
//https://api.thingspeak.com/channels/1603157/status.json?api_key=HSB377D2S5LD0I0K
//Write API Key: 
//FIUY5IXW0Y1LDKYZ
//Read API Keys:
//HSB377D2S5LD0I0K

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println("ESP8266-12E/F LDR testing program\n");
  Serial.println("Build-in LED1 at GPIO-16(D0)");
  Serial.println("Mic analog Pin at A0");
  Serial.println("Mic digital Pin at GPIO-12(D6)");
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin_analog, INPUT);
  pinMode(ldrPin_digital, INPUT);

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
  //Get status for light sensor
  int ldrStatus = analogRead(ldrPin_analog);
  
  //Get temperature and humidity from AM2320
  float temperature = th.getTemperature();
  float humidity = th.getHumidity();

  //Get Current Voltage, Current, Power and Relay Status
  float Voltage = SP1.V;
  float Current = SP1.A;
  float Power = SP1.W;
  bool isReplayOn = SP1.Relay;
  
  //wait for the wifi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    SP1.Print();
    WiFiClient client;
    HTTPClient http;
    String request_url = write_channel + "field1=" + temperature + "&field2=" + humidity + "&field3=" + ldrStatus 
                         + "&field4=" + Voltage + "&field5=" + Current + "&field6=" + Power 
                         + "&field7=" + isReplayOn;
    Serial.println(request_url);
    http.begin(client, request_url);
    int httpCode = http.GET();

    if (httpCode>0){
      String payload = http.getString();
      Serial.println(httpCode);
      //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      Serial.println(payload);
      Serial.print("humidity: ");
      Serial.print(humidity);
      Serial.print("%");
      Serial.print("|| temperature: ");
      Serial.print(temperature);
      Serial.print("*C");
      Serial.print("|| Light Sensor Status: ");
      Serial.print(ldrStatus);
      Serial.println("---------------");
    }
    http.end();
    
  }

//Set the temperature to turn on/ off the smart plug
  if ( temperature >= 30 ){
    SP1.turnOn();
  }
  else{
    SP1.turnOff();
  }
    
  delay(1000);
}
