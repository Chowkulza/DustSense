#include <Arduino.h>

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600); // baudrate= 9600
  delay(2000);      
  Serial.print("Hello this is my fisrt program");
  delay(3000);      
  
}

void loop() {
    Serial.print("I am in the loops");// put your main code here, to run repeatedly:
    delay(3000); //delay 3000 ms or 3 sec.

}
