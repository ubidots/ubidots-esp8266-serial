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

**NOTE**: Take in count that the response is printed with a carriage return.

```c++
/****************************************
 * Define Constants
 ****************************************/
char command[128];  // command
char answer[100]; // answer from server

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {

  int i = 0;
  int j = 0;

  if (Serial.available()){
    while (Serial.available() > 0) {
      command[i++] = (char)Serial.read();
    }  
    /* Sends the command to the telemetry unit */
    Serial1.print(command);
    i = 0;
  }

  if (Serial1.available() > 0) {
    /* Reading the telemetry unit */
    while (Serial1.available() > 0) {
      answer[j++] = (char)Serial1.read();
    }
    /* Response from the server */
    Serial.print(answer);
    j = 0;
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
  bool flag = true;
  const char * USER_AGENT = "UbidotsESP8266"; // Assgin the user agent
  const char * VERSION =  "1.0"; // Assign the version
  const char * METHOD = "POST"; // Set the method
  const char * TOKEN = "........"; // Assign your Ubidots TOKEN
  const char * DEVICE_LABEL = "ESP8266"; // Assign the device label
  const char * VARIABLE_LABEL = "temp"; // Assign the variable label
}

char* command = (char *) malloc(sizeof(char) * 128);
char telemetry_unit[100]; // response of the telemetry unit

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

  /* Wait for the server response to read the values and built the command */
  if (flag) {
    /* Analog reading */
    float sensor1 = analogRead(A0);
    float sensor2 = analogRead(A1);

    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(sensor1, 4, 2, str_sensor1);
    dtostrf(sensor2, 4, 2, str_sensor2);

    /* Building the logger command */
    sprintf(command, "init#");
    sprintf(command, "%s%s/%s|%s|%s|", command, USER_AGENT, VERSION, METHOD, TOKEN);
    sprintf(command, "%s%s=>", command, DEVICE_LABEL);
    sprintf(command, "%s%s:%s", command, VARIABLE_LABEL, str_sensor1);
    sprintf(command, "%s|end#final", command);

    /* Prints the command sent */
    //Serial.println(command);// uncomment this line to print the command

    /* Sends the command to the telemetry unit */
    Serial1.print(command);
    /* free memory*/
    free(command);
    /* Change the status of the flag */
    flag = false;
  }

  /* Reading the telemetry unit */
  int i = 0;
  while (Serial1.available() > 0) {
    telemetry_unit[i++] = (char)Serial1.read();
    /* Change the status of the flag; allows the next command to be built */
    flag = true;
  }

  if (flag) {
    /* Print the server response -> OK */
    Serial.write(telemetry_unit);
    /* free memory */
    memset(telemetry_unit, 0, i);
  }

  delay(1000);
}
```

If you desire sending more values, reference the **README** to learn how to build the request containing more than one variable. Then, you will have to modify the command (as seen above) `/* Building the logger command */` to include a data traffic rate that is greater than one variable.

**WARNING**: Based on the **BUFFER SIZE** you will be able to send a command with a maximum length of **128 characters**. If your command exceed the maximum lenght, you will received an `error message`.
