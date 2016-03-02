#ifndef __UbidotsESP8266_H_
#define __UbidotsESP8266_H_
#define DEBUG_UBIDOTS

#include "Arduino.h"
#include <SoftwareSerial.h>

#define SERVER "things.ubidots.com"
#define PORT 80

typedef struct Value {
  char  *id;
  char  *context_1;
  char  *context_2;
  float value_id;
} Value;

class Ubidots{
    private:
        SoftwareSerial _client = SoftwareSerial(2, 3);      
        char* _token;
        char* readData(uint16_t timeout);
        void flushInput();
        uint8_t maxValues;
        uint8_t currentValue;
        Value * val;        

    public:
        Ubidots(char* token);
        bool wifiConnection(char* ssid, char* pass);
        bool saveValue(char *id, float value);
        float getValue(char* id);
        void add(char *variable_id, double value, char *context1, char *context2);
        bool sendAll();  
};
#endif