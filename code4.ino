#include <arduino.h>
#include <Wire.h>           // ไลบรารีสำหรับการใช้งาน I2C
#include <U8g2lib.h>        // ไลบรารี U8g2 สำหรับควบคุมหน้าจอ OLED

// Initialize OLED display object (I2C pins: SCL = 14, SDA = 12)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);

void setup() {
  Serial.begin(9600);  // เริ่มต้นการใช้งาน Serial Monitor
  u8g2.begin();        // เริ่มต้นหน้าจอ OLED
}

void loop() {
  u8g2.clearBuffer();  // ล้างบัฟเฟอร์
  u8g2.setFont(u8g2_font_ncenB08_tr);  // กำหนดฟอนต์
  u8g2.drawStr(0, 10, "Hello from Arduino!");  // วาดข้อความ
  u8g2.sendBuffer();  // ส่งข้อมูลไปที่หน้าจอ OLED
  delay(1000);  // รอ 1 วินาที
}
