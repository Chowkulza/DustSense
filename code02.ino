#include <Arduino.h>
int temperature = 39;
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600); // baudrate= 9600
  delay(2000);      
  Serial.println("Hello this is my second program");
  delay(3000);        
}
void loop() {
  if (temperature > 38) {
    Serial.println("อุณหภูมิสูงกว่า 38 C ร้อนมาก");
  } else {
    Serial.print("อุณหภูมิเท่ากับ");
    Serial.print(temperature);
    Serial.println("   องศาเซลเซียส");
  }
  delay(1000);  // รอ 1 วินาที
}
