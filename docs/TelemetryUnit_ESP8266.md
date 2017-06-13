# Telemetry unit (ESP8266)  

As we know **Telemetry** is an automated communications process by which measurements and other data is collected at remote or inaccessible points and transmitted to receiving equipment for monitoring. 

In this case, we are using the ESP8266 as a telemetry unit. We collect the data from the logger (Arduino MEGA) and the ESP8266 will be in charge of sending the collected data to Ubidots. 

## Hardware setup - Wiring

**IT IS IMPORTANT IDENTIFY CAREFULLY THE PINOUT OF THE ESP8266, BECAUSE THE ESP8266 CAN BREAK IF YOU MAKE A WRONG CONNECTION**

![ESP8266 connection](https://raw.githubusercontent.com/guyz/pyesp8266/master/esp8266_pinout.png)

To program the **ESP8266** you have to use a programmer as **UartSBee**, also you can use the **Arduino MEGA** to program the ESP8266. To upload the code into the ESP8266 you have to follow the connections below.

* **ESP8266 - Arduino MEGA /  ESP8266 - UartSBee connections**: 

ESP8266 | Arduino MEGA | UartSBee
--------|--------------|---------
RXD | RX0 - (0) | TXD
GPIO0 | GND | GND
GPIO2 | — | —
GND | GND | GND
VCC | 3.3V | 3.3V
RST | —  | —
CH_PD | 3.3V | 3.3V
TXD |TX0 -(1) | RXD

**NOTE:** Please be careful with the VCC of the ESP8266, it works only with a 3.3V supply.

If you're using the **Arduino MEGA**, makes sure that there are no other programs running and use the serial communication channel. Also, you have to set Arduino **RST** to **GND**(green wire); if you do not, you will get a console error and will not able to compile the ESP8266.

* **ESP8266 - Arduino MEGA diagram connections**:

![Arduino MEGA connection](https://cdn2.hubspot.net/hubfs/329717/Documentation%20files/images/ArduinoMEGA_ESP8266_programMode_bb.png)

## Firmware Setup - Arduino IDE

1. Go to the [Arduino IDE](https://www.arduino.cc/en/Main/Software).
2. Open the **Arduino IDE**, select **Files -> Preferences** and enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into **Additional Board Manager URLs** field. You can add multiple URLs, separating them with commas.
3. Open **Boards Manager** from **Tools -> Board -> Boards Manager** and install **esp8266** platform. To simply find the correct device, search ESP8266 within the search bar.
4. Select the **Generic ESP8266 Module** from **Tools > Board menu**.
5. Assigns **115200** as baudrate. Click on **Tools > Upload Speed: "115200"**.
6. Download the **UbidotsESP8266Serial library** [here](https://github.com/ubidots/ubidots-esp8266-serial/archive/master.zip).
7. Now, click on **Sketch -> Include Library -> Add .ZIP Library**.
8. Select the .ZIP file of **UbidotsESP8266Serial** and then "**Accept**" or "**Choose**".
9. **Close** the Arduino IDE and **open** it again.
10. Paste the code provide below.

## Telemetry Unit - ESP8266 code

Once you have pasted the code below into the **Arduino IDE**, you will have to assign the parameters required to connect the **ESP8266** to Wi-Fi and Ubidots. Please assign your Wi-Fi SSID, Wi-Fi password, and Ubidots TOKEN where are indicated:

```c++
/****************************************
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
```

Once the code is uploaded into the ESP8266, reference to the [Documentation](https://github.com/ubidots/ubidots-esp8266-serial/tree/master/docs/TelemetryUnit_ESP8266.md) of **telemetry unit - ESP8266** to find all the next steps required. 

**NOTE**: The sampling frequency on the ESP8266 is of 1000 milliseconds

## FAQs and Troubleshooting

One of the most common troubleshooting with the ESP8266 is this one:

![ESP8266ISSUE](https://cdn2.hubspot.net/hubfs/329717/Documentation%20files/images/ERROR_ESP8266.png)

If you got the issue above, please verify if the connections are in the right way, check if the Arduino **RST** is setted to **GND** , also verify if the board **Generic ESP8266 Module** is selected on the Arduino IDE.

**NOTE**: If you use the Arduino MEGA to program the ESP8266 don't forget establish the new wires connections to be able to communicate the devices through the hardware serial port. 