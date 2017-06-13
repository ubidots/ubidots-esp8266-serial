# Logger 

The logger will be in charge of taking sensor (pin) readings and send values to the **Telemetry Unit** via serial. 

You can use any microcontroller as logger, but please note that if the logger (microcontroller) does not have more than one hardware serial ports you will not able to visualize the response, but it is also possible to send data without any issue.

We decided to used the **Arduino MEGA** as logger, because it has more than one hardware serial ports available. 

## Hardware setup - Wiring

Once the code is uploaded into the telemetry unit (**ESP8266**), you have to make the right connections to be able to communicate the telemetry unit with the logger via serial. Please follow the table below to make the connections, also verify the connections with the diagram provided:

* **Arduino MEGA - ESP8266 Connections**:

Arduino MEGA | ESP8266
-------------|---------
RX1 - (19) | TX
TX1 - (18) | RX 
3.3V | 3V3
3.3V | CH_PD 
GND | GND

* **Diagram Connections**:

![ESP8266 Logger](https://cdn2.hubspot.net/hubfs/329717/Documentation%20files/images/ArduinoMEGA_ESP8266_Logger.png) 


## Firmware Setup - Arduino IDE 

1. Once you completed the step before go to the **Arduino IDE**.
2. Assign the board to compile, in this case **Arduino Mega**. Click on **Tools > Board: "Arduino/Genuino Mega or Mega 2560"**.
3. Select the Port assigned for the board. Click on **Tools > Port > Select the port assgined**.
4. Paste the code provided below with the modification required (reference to the next step).
5. Upload the code into the board. Then, verify it works through the **Serial Monitor**. 

## Send command via serial - Manual 

The following code lets you send commands through the Serial Monitor. This will help you to verify if the logger and the Telemetry unit are working together. 

**NOTE**: To know how to build the command required, reference library's **README**. 

Once the code is uploaded, open the **Serial Monitor** and send the command, then, when all is programmed correctly, you have to receive the **"OK"**response from the server. If you receive an **"ERROR"** response verify the structure of the command.

```c++
/****************************************
 * Define Constants
 ****************************************/
String command = "";  // command
String telemetry_unit = ""; // response of the telemetry unit

/****************************************
 * Main Functions
 ****************************************/ 
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {

  if (Serial.available() > 0) { 
    command = Serial.readString();  
    /* Sends the command to the telemetry unit */
    Serial.println(command);
    Serial1.print(command); 
  }
  
  /* Reading the telemetry unit */
  if (Serial1.available() > 0) {
    telemetry_unit = Serial1.readString(); 
    Serial.print(telemetry_unit);   
  }   
}
```

## Send command via serial - Automatically 

Once you verify that everythings works as it should, you can use the following code to manage your sensors readings and send the values to Ubidots.

As you can see at the first part of the code you just have to assign your Ubidots TOKEN where is indicated. And also, please assign your device and variable parameters apporpriately if you adjusted any portion of code provided in this documentation. 

```c++
/****************************************
 * Define Constants
 ****************************************/
namespace {
  const char * USER_AGENT = "UbidotsESP8266"; // Assgin the user agent
  const char * VERSION =  "1.0"; // Assign the version 
  const char * METHOD = "POST"; // Set the method
  const char * TOKEN = "Put_your_Ubidots_token_here"; // Assign your Ubidots TOKEN
  const char * DEVICE_NAME = "telemetry"; // Assign the desire device name 
  const char * DEVICE_LABEL = "ESP8266"; // Assign the device label 
  const char * VARIABLE_LABEL_1 = "temperature"; // Assign the variable label 
  const char * VARIABLE_LABEL_2 = "humidity"; // Assign the variable label
}

char command[700]; // command
String telemetry_unit = ""; // response of the telemetry unit

/* Space to store values to send */
char str_sensor1[10];
char str_sensor2[10];

/****************************************
 * Main Functions
 ****************************************/ 
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {

  
  /* Analog reading */ 
  float sensor1 = analogRead(A0);
  float sensor2 = analogRead(A1);
  
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(sensor1, 4, 2, str_sensor1);
  dtostrf(sensor2, 4, 2, str_sensor2);

  /* Building the logger command */ 
  sprintf(command, "init#");
  sprintf(command, "%s%s/%s|%s|%s|", command, USER_AGENT, VERSION, METHOD, TOKEN);
  sprintf(command, "%s%s:%s=>", command, DEVICE_NAME, DEVICE_LABEL);
  sprintf(command, "%s%s:%s", command, VARIABLE_LABEL_1, str_sensor1);
  //sprintf(command, "%s,%s:%s", command, VARIABLE_LABEL_2, str_sensor2); // uncomment this line to send sensor 2 values
  sprintf(command, "%s|end#final", command);

  Serial.println(command);
  /* Sends the command to the telemetry unit */
  Serial1.print(command);

  /* Reading the telemetry unit */
  if (Serial1.available() > 0) {
    telemetry_unit = Serial1.readString();
    Serial.println(telemetry_unit);
  }
  delay(5000);
}
```

If you desire sending more values, reference the **README** to learn how to build the request containing more than one variable. Then, you will have to modify the command (as seen above) `/* Building the logger command */` to include a data traffic rate that is greater than one variable.



