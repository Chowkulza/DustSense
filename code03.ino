#include <Arduino.h>
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600); // baudrate= 9600
  delay(2000);      
  Serial.println("Hello this is my third program");
  delay(3000);        
}
void loop() {

  printtemperature (36);
  delay(1000);      // รอ 1 วินาที

}

void printtemperature (int temperature){

  if (temperature > 38) {
    Serial.println("อุณหภูมิสูงกว่า 38 C ร้อนมาก");
  } else {
    Serial.print("อุณหภูมิเท่ากับ  ");
    Serial.print(temperature);
    Serial.println("   องศาเซลเซียส");
  }
}
