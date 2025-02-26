#include <Arduino.h>

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600); // baudrate= 9600
  delay(2000);      
  Serial.println("Hello this is my fisrt program");
  delay(3000);      
  
}

void loop() {
    Serial.println("I am in the loops");// print i am in loops
    delay(3000); //delay 3000 ms or 3 sec.

}
