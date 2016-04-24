/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Made by Mateo Velez - Metavix for Ubidots Inc
*/


#include "UbidotsESP8266.h"
#include <SoftwareSerial.h>

/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    _client.begin(9600);
    maxValues = 4;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/** 
 * This function is to read the data from ESP8266 pins. This function is from Adafruit_FONA library
 * @arg timeout of response
 * @return the response buffer
 */
char* Ubidots::readData(uint16_t timeout){
  uint16_t replyidx = 0;
  char replybuffer[550];
  while (timeout--) {
    if (replyidx >= 549) {
      break;
    }
    while(_client.available()) {
      char c =  _client.read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;
      }
      replybuffer[replyidx] = c;
      replyidx++;
        if (replyidx >= 549) {
            break;
        }
    }

    if (timeout == 0) {
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = '\0';  // null term
#ifdef DEBUG_UBIDOTS
  Serial.println("Response of ESP8266:");
  Serial.println(replybuffer);
#endif
  while(_client.available()){
    _client.read();
  }
  return replybuffer;
}
/** 
 * This function is to set the WiFi connection
 * @arg ssid of the WiFi
 * @arg pass of the WiFi
 * @return true upon success
 */
bool Ubidots::wifiConnection(char* ssid, char* pass){
    _client.println(F("AT"));
    if((strstr(readData(2000),"OK")==NULL)){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("clearin data"));
#endif
    }
    _client.println(F("AT+RST"));
    if((strstr(readData(2000),"OK")==NULL)){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Reset of ESP8266"));
#endif
    }
    _client.println(F("AT+GMR"));
    if((strstr(readData(2000),"OK")==NULL)){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error at GMR"));
#endif
    }
    _client.println(F("AT+CWMODE=3"));
    if((strstr(readData(2000),"ERROR")!=NULL)){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+CWMODE"));
#endif
        return false;
    }
    _client.print(F("AT+CWJAP=\""));
    _client.print(ssid);
    _client.print(F("\",\""));
    _client.print(pass);
    _client.println(F("\""));            
    if(strstr(readData(15000),"ERROR")!=NULL){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+CWJAP"));
#endif
        return false;
    }
    _client.println(F("AT+CIFSR"));
    if((strstr(readData(2000),"ERROR")!=NULL)){
    
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+CIFSR"));
#endif
        return false;
    }
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
    float num;
    String raw;
    uint8_t bodyPosinit;
    uint8_t bodyPosend;
    _client.println(F("AT"));
    if((strstr(readData(2000),"OK")==NULL)){
#ifdef DEBUG_UBIDOTS
        Serial.println(F("clearin data"));
#endif
    }
    _client.println(F("AT+CIPMUX=0"));
    if(strstr(readData(3000),"ERROR")!=NULL){
        Serial.println(F("Error at CIPMUX"));        
        return false;
    }
    _client.print(F("AT+CIPSTART=\"TCP\",\""));
    _client.print(SERVER);
    _client.print(F("\","));
    _client.println(PORT);
    if(strstr(readData(10000),"Ok Linked")!=NULL){
        Serial.println(F("Error at CIPSTART"));
        return false;
    }
    _client.println(F("AT+CIPMODE=1"));
    if(strstr(readData(1000),"OK")==NULL){
        Serial.println(F("Error at CIPMODE"));        
        return false;
    }
    _client.println(F("AT+CIPSEND"));
    if(strstr(readData(1000),">")==NULL){
        Serial.println(F("Error at CIPSEND"));        
        return false;
    }
    _client.print(F("GET /api/v1.6/variables/"));
    _client.print(id);
    _client.println(F("/values?page_size=1 HTTP/1.1"));
    _client.println(F("Host: things.ubidots.com"));
    _client.println(F("User-Agent: Arduino-ESP8266/1.0"));
    _client.print(F("X-Auth-Token: "));
    _client.println(_token);
    _client.println(F("Connection: close"));
    _client.println();
    _client.println(F("+++"));
    char* reply = readData(5000);
    char* pch = strstr(reply,"\"value\":");
    raw = String(pch);
    bodyPosinit =9+ raw.indexOf("\"value\":");
    bodyPosend = raw.indexOf(", \"timestamp\"");
    raw.substring(bodyPosinit,bodyPosend).toCharArray(reply, 10);
    num = atof(reply);      
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 * @arg context1 context name : context value to save in a struct
 * @arg context2 context name : context value to save in a struct
 */
void Ubidots::add(char *variable_id, double value, char *context1, char *context2){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  (val+currentValue)->context_1 = context1;
  (val+currentValue)->context_2 = context2;
  currentValue++;
  if(currentValue >maxValues){
    currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
    int i;
    String all;
    char vals[10];
    String str;
    char b[3];
    all = "[";
    for(i=0; i<currentValue;){
        str = String(((val+i)->value_id),5);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":\"";
        all += str;
        all += "\", \"context\":{";
        all += String((val+i)->context_1);
        all += ", ";
        all += String((val+i)->context_2);
        all += "}}";
        i++;
        if(i<currentValue){
            all += ", "; 
        }
    }
    all += "]";
    i = all.length();
    // Next for is to calculate the lenght of the data that you will send
    _client.println(F("AT+CIPMUX=0"));
    if(strstr(readData(3000),"ERROR")!=NULL){
        Serial.println(F("Error at CIPMUX"));        
        return false;
    }
    _client.print(F("AT+CIPSTART=\"TCP\",\""));
    _client.print(SERVER);
    _client.print(F("\","));
    _client.println(PORT);
    if(strstr(readData(10000),"Ok Linked")!=NULL){
        Serial.println(F("Error at CIPSTART"));        
        return false;
    }
    _client.println(F("AT+CIPMODE=1"));
    if(strstr(readData(1000),"OK")==NULL){
        Serial.println(F("Error at CIPMODE"));        
        return false;
    }
    _client.println(F("AT+CIPSEND"));
    if(strstr(readData(1000),">")==NULL){
        Serial.println(F("Error CIPSEND"));        
        return false;
    }
    _client.println(F("POST /api/v1.6/collections/values/?force=true HTTP/1.1"));
    _client.println(F("Host: things.ubidots.com"));
    _client.println(F("User-Agent: Arduino-ESP8266/1.0"));
    _client.print(F("X-Auth-Token: "));
    _client.println(_token);
    _client.println(F("Connection: close"));
    _client.println(F("Content-Type: application/json"));
    _client.print(F("Content-Length: "));
    _client.println(String(i));
    _client.println();
    _client.println(all);
    _client.println();
    _client.println(F("+++"));
    if(strstr(readData(5000),">")!=NULL){
        Serial.println(F("Error"));     
        
        return false;
    }
    // Next for is to send all data from the struct   
    currentValue = 0;
    return true;
}