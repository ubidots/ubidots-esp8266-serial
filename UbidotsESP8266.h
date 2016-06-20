
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
#ifndef __UbidotsESP8266_H_
#define __UbidotsESP8266_H_
#define DEBUG_UBIDOTS

#include "Arduino.h"
#include <SoftwareSerial.h>

#define SERVER "things.ubidots.com"
#define PORT 80
#define MAX_VALUES 5

typedef struct Value {
  char  *id;
  char  *context_1;
  float value_id;
} Value;

class Ubidots{
    private:
        SoftwareSerial _client = SoftwareSerial(2, 3);      
        char* _token;
        char* readData(uint16_t timeout);
        void flushInput();
        uint8_t currentValue;
        Value * val;        

    public:
        Ubidots(char* token);
        bool wifiConnection(char* ssid, char* pass);
        bool saveValue(char *id, float value);
        float getValue(char* id);
        void add(char *variable_id, float value, char *context1 = NULL);
        bool sendAll();  
};
#endif