//https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/
#include <Arduino.h>
#include <Arduino_JSON.h>
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
//#define STASSID "ssha"
//#define STAPSK "goodmorning3"
#define STASSID "Xx802xX"
#define STAPSK "23799766"
#endif
const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

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
    //Send requests to the server
    Serial.println("---------------");
    SP1.Print(); // print out information of the smart plug
    Serial.println("---------------");
    WiFiClient client;
    HTTPClient http;
    String request_url = write_channel + "field1=" + temperature + "&field2=" + humidity  
                         + "&field3=" + Power 
                         + "&field4=" + isReplayOn;
    Serial.println(request_url);
    Serial.println("---------------");
    http.begin(client, request_url);
    int httpCode = http.GET();

    if (httpCode > 0){
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
      Serial.println("---------------");
    }
    else{
      Serial.println("Error: ");
      Serial.print(httpCode);
      Serial.println("---------------");
    }
    http.end();

    //Receive request from the server
    //WiFiClient client2;
    HTTPClient http2;
    String server_url = "http://api.thingspeak.com/channels/1603157/feeds.json?api_key=HSB377D2S5LD0I0K&results=1";
    http2.begin(client, server_url);
    int http2Code = http2.GET();

    if (http2Code > 0){
      String payload2 = http2.getString();
      
      //Handle response
      JSONVar command = JSON.parse(payload2);
      Serial.println("payload2: ");
      Serial.println(command);
      Serial.println(command["feeds"]);
      String command_detail = JSON.stringify(command["feeds"]);
      command_detail.replace("[","");
      command_detail.replace("]","");
      Serial.println(command_detail);
      JSONVar command_control = JSON.parse(command_detail);
      Serial.println("Temperature: ");
      Serial.println(command_control["field1"]);
      Serial.println("Humidity: ");
      Serial.println(command_control["field2"]);      
      Serial.println("Web App Command: ");
      Serial.println(command_control["field5"]);
      Serial.println("Air-con Temperature: ");
      Serial.println(command_control["field7"]);
      Serial.println("---------------");

      //Smart Plug Control Command
      String web_app_command = JSON.stringify(command_control["field5"]);
      String web_app_switch_air_con = JSON.stringify(command_control["field6"]);
      String web_app_temperature = JSON.stringify(command_control["field7"]);

      /*web_app_command: control the smart-plug
       * 1: manual on/ off the air-con by web app
       * 2: set temperature to turn on/ off the air-con by web app
       * 3: follow the build-in setup to turn on/off the air-con by web app
       */
      if ( web_app_command == "1"){
        if ( web_app_switch_air_con == "1" ){
            SP1.turnOn();
        }
        else if ( web_app_switch_air_con == "0" ){
            SP1.turnOff();
        }
      }
      else if ( web_app_command == "2"){
         float set_temperature = web_app_temperature.toFloat();

         if ( temperature >= set_temperature ){
          SP1.turnOn();
         }
         else {
          SP1.turnOff();
         }
      }
      else if ( web_app_command == "3"){
        if ( temperature >= 30 ){
          SP1.turnOn();
        }
        else{
          SP1.turnOff();
          } 
      }
      else{
        Serial.println("Fail to get command from the web app!");
        Serial.println("---------------");
        if ( temperature >= 30 ){
          SP1.turnOn();
        }
        else{
          SP1.turnOff();
          } 
      }
    }
    else{
      Serial.println("Error: ");
      Serial.print(http2Code);
      Serial.println("---------------");
    }
    http2.end();
    
  }
  else{
    Serial.println("WiFi Disconnected!");
  }

  delay(1000);
}
