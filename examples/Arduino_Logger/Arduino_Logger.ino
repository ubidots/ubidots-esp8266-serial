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
