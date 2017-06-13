
/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
#define WIFISSID "Put_your_WIFI_SSID_here" // Assign your WiFi SSID 
#define PASSWORD "Put_your_WIFI_password_here" // Assign your WiFi password
#define TOKEN "Put_your_Ubidots_Token_here" // Assign your Ubidots TOKEN

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/ 
void setup() {
  Serial.begin(115200);
  client.wifiConnection(WIFISSID, PASSWORD);
}

void loop() {
  client.readData(); // Reads the command from the logger
  delay(1000);
}