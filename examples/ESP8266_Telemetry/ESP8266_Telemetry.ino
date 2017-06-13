x/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESP8266.h"

/****************************************
 * Define Constants
 ****************************************/
namespace { 
  const char * WIFISSID = "Put_your_WIFI_SSID_here"; // Assign your WiFi SSID 
  const char * PASSWORD = "Put_your_WIFI_password_here"; // Assign your WiFi password
  const char * TOKEN = "Put_your_Ubidots_Token_here"; // Assign your Ubidots TOKEN
}

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