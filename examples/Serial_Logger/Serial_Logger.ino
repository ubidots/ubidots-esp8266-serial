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