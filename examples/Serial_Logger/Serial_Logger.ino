/****************************************
 * Define Constants
 ****************************************/

char command[700];  // command
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