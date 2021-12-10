#ifndef TPLink_H
#define TPLink_H
 
#include "Arduino.h"
#include <stdio.h>
#include <WifiClient.h>

// TP-Link HS110 send query( do not change)
char TP_reqInfo[] = "{\"system\":{\"get_sysinfo\":{}}}";
char TP_reqOn[] = "{\"system\":{\"set_relay_state\":{\"state\":1}}}";
char TP_reqOff[] = "{\"system\":{\"set_relay_state\":{\"state\":0}}}";
char TP_reqLEDOn[] = "{\"system\":{\"set_led_off\":{\"off\":0}}}";
char TP_reqLEDOff[] = "{\"system\":{\"set_led_off\":{\"off\":1}}}";
char TP_reqTime[] = "{\"time\":{\"get_time\":{}}}";
char TP_reqEnergy[] = "{\"emeter\":{\"get_realtime\":{}}}";

class TPL_SmartPlug {
  public:
    TPL_SmartPlug(char*);
   
   float V, W, A;   // latest HS110 meter reading
   bool Relay;
   bool LED;
   
    bool getReading();  
    bool getTime();   
    bool getInfo();   
    void Print();
    bool turnOn();
    bool turnOff();
    bool turnLEDOn();
    bool turnLEDOff();

  private:
   const byte _Key = 171;
   
   WiFiClient _client;
   char* _IPAddress;
   int _IPPort = 9999;
   int _mV, _mW, _mA;
   byte _sendBuf[100];
   byte _recvBuf[1000];
   char _recvMsg[1000];
   
   //
    int Send(char []);
    int Recv();   // TP-Link HS110 receive message function, Return received message length (store on recvMsg)
    int Encrypt(char []);
    int Decrypt(int);
    int get(char []);
};

// INIT function
TPL_SmartPlug::TPL_SmartPlug(char* ip)
{
  _IPAddress = ip;  
  _client.setTimeout(999);
}
bool TPL_SmartPlug::getReading()
{
  if(_client.connect(_IPAddress, _IPPort)) {
    //connected
    int count = Send(TP_reqEnergy);
    delay(500);
    //receive data
    count = Recv();
    sscanf(_recvMsg,  "%*[^0-9]%d%*[^0-9]%d%*[^0-9]%d%*s", &_mV, &_mA, &_mW);
    V = _mV/1000.0;
    A = _mA/1000.0;
    W = _mW/1000.0;    
    _client.stop();
    return true;
  }
  return false;
}

bool TPL_SmartPlug::getInfo()
{
  int count = get(TP_reqInfo);
  if(count>0) {
    //Serial.print("Received byte count: ");
    //Serial.println(count);
    //Serial.write(_recvMsg, count);
    //Serial.println();
    int val;
    char *ptr = strstr(_recvMsg, "relay_state\":");
    if(ptr!=NULL) {
      if(sscanf(ptr+13, "%d%^s", &val) ==1) {
        Relay = val;
      }
    }
    ptr = strstr(_recvMsg, "led_off\":");
    if(ptr!=NULL) {
      if(sscanf(ptr+9, "%d%^s", &val) ==1) {
        LED= (val==0?true:false);       
      }
    }
    //Serial.println();
    return true;
  }
  return false;
}
bool TPL_SmartPlug::getTime()
{
  int count = get(TP_reqTime);
  if(count>0) {
    Serial.print("Received byte count: ");
    Serial.println(count);
    Serial.write(_recvMsg, count);
    Serial.println();
    return true;
  }
  return false;
}

bool TPL_SmartPlug::turnOn()
{  
  int count = get(TP_reqOn);
  if(count>0) {
    getReading();
    return true;
  }
  return false;
}
bool TPL_SmartPlug::turnOff()
{
  int count = get(TP_reqOff);
  if(count>0) {
    getReading();
    return true;
  }
  return false;
}

bool TPL_SmartPlug::turnLEDOn()
{
  int count = get(TP_reqLEDOn);
  if(count>0) {
    getReading();
    return true;
  }
  return false;
}
bool TPL_SmartPlug::turnLEDOff()
{
  int count = get(TP_reqLEDOff);
  if(count>0) {
    getReading();
    return true;
  }
  return false;
}

void TPL_SmartPlug::Print() 
{
    char s[100];
    getReading();
    getInfo();
    sprintf( s, "HS110 (%s), Voltage: %.2f V), Current: %.2f A, Power: %.2f W, Relay: %s, LED: %s\n", _IPAddress, V, A, W, Relay?"On":"Off", LED?"On":"Off"); 
    Serial.print(s);
}

// TP-Link HS110 send message function
int TPL_SmartPlug::Send(char command[]) { // Return sent message length
  int count = Encrypt(command);
  _client.write(_sendBuf, count);
  return count;
}

// TP-Link HS110 receive message function
// Return received message length (store on recvMsg)
int TPL_SmartPlug::Recv() 
{   
  int count = _client.read(_recvBuf, sizeof(_recvBuf));
  return Decrypt(count);  
}

// TP-Link HS110 Encrypt function
int TPL_SmartPlug::Encrypt(char cmd[]) {  
  for( int i = 0; i< 100; i++){
    _sendBuf[i] = 0x00;
  }
  int i=0;
  byte key = _Key;
  byte a = 0 ;
  //Serial.print(cmd);
  while( cmd[i] !='\0') {
    a = key ^ (byte)(cmd[i]);
    key = a;
    _sendBuf[i+4] = (byte)a;
    i++;    
  }
  //Serial.print(" ");
  _sendBuf[3] = i;  // length +1
  //Serial.println(sendBuf[3], HEX);
  return i+4;
}


// TP-Link HS110 Decrypt function
int TPL_SmartPlug::Decrypt(int bytes) {
  if(bytes==0)
    return 0;
  //Reset receive message
  for (int i=0; i<1000; i++) {
    _recvMsg[i] = 0;
  }
  // decrypt
  byte key = _Key;
  int shift = 4;
  for (int i=0; i< bytes-shift; i++) {
    _recvMsg[i] = key ^ _recvBuf[i+shift];
    key = _recvBuf[i+shift];
  }
  return bytes-shift;  
}

int TPL_SmartPlug::get(char cmd[] )
{
  if(_client.connect(_IPAddress, _IPPort)) {
    //connected
    int count = Send(cmd);
    delay(100);
    //receive data
    count = 0;
    count = Recv();
    _client.stop();
    return count;
  }
  return -1;
}

#endif
