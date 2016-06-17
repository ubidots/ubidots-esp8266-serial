#include <UbidotsESP8266.h>
#include <SoftwareSerial.h> 


#define SSID "Your_WiFi_SSID_here"
#define PASS "Your_WiFi_PASWORD_here"


#define TOKEN "Your_token_here"
#define ID "Your_id_here" // Replace it with your Ubidots' variable ID
#define ID2 "Your_id2_here" 
#define ID3 "Your_id3_here"
#define ID4 "Your_id4_here" 

Ubidots client(TOKEN);

void setup() {
  Serial.begin(9600);
  client.wifiConnection(SSID,PASS);
}


void loop() {
  float value = analogRead(A0);
  float value2 = analogRead(A1);
  float value3 = analogRead(A2);
  float value4 = analogRead(A3);
  client.add(ID,value);
  client.add(ID2,value2);
  client.add(ID3,value3);
  client.add(ID4,value4);
  client.sendAll();
}