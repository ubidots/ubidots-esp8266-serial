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

  /* Prints the command sent */
  //Serial.println(command);// uncomment this line to print the command
  
  /* Sends the command to the telemetry unit */
  Serial1.print(command);

  /* Reading the telemetry unit */
  int i = 0;
  while (Serial1.available() > 0) {
    telemetry_unit[i++] = (char)Serial1.read(); 
  }
  Serial.println(telemetry_unit);
  i = 0;
  delay(5000);
}