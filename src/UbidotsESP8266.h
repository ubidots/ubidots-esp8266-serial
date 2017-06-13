/*
Copyright (c) 2017 Ubidots.

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

Made by: ----- María Carlina Hernández ---- Developer at Ubidots Inc 
         https://github.com/mariacarlinahernandez  
         ----- Jose Garcia ---- Developer at Ubidots Inc 
         https://github.com/jotathebest

*/

#ifndef __UbidotsESP8266_H_
#define __UbidotsESP8266_H_

#include <ESP8266WiFi.h>
#include "WiFiUdp.h"

namespace {
    const char *  SERVER = "translate.ubidots.com";
    const int  PORT = 9012;
}

class Ubidots {
 public:
    Ubidots(char* token, const char* server = SERVER);
    bool wifiConnection(char *ssid, char *pass);
    void readData();    
 private:
    uint8_t sendData();
    uint8_t checkCommand();
    void readServer();
    char _command[300];
    char* _token;
    char* _request;
    const char* _server;
    WiFiClient _client;
};

#endif