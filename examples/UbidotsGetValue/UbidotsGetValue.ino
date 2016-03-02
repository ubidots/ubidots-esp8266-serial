#include <UbidotsESP8266.h>
#include <SoftwareSerial.h> 

#define SSID "Your_WiFi_SSID_here"
#define PASS "Your_WiFi_PASWORD_here"

#define TOKEN "Your_token_here"
#define ID "Your_ID_here"

Ubidots client(TOKEN);

void setup() {
  Serial.begin(9600);
  client.wifiConnection(SSID,PASS);
}

void loop() {
  float value;
  value = client.getValue(ID);
} 