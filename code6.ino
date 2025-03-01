#include <DHT.h>                  // Library for DHT sensors (e.g., DHT11 or DHT22)
#include <U8g2lib.h>              // Library for controlling OLED displays
// Initialize DHT sensor on pin D3 with sensor type DHT22
DHT dht(D3, DHT22);
// Initialize OLED display object (I2C pins: SCL = 14, SDA = 12)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);

void setup(void) {
  u8g2.begin();                   // Initialize OLED display
  dht.begin();                    // Initialize DHT sensor
  // Display a welcome message on the OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);  // Set font size and style
  u8g2.drawStr(10, 31, "DustSense");     // Display "Hello" at coordinates (30, 45)
  u8g2.sendBuffer();                 // Send buffer to OLED for rendering
  delay(10000);// delay 5 second
}

void loop(void) {

    float temp = dht.readTemperature();  // Read temperature from DHT sensor
    float humidity = dht.readHumidity();  // Read humidity from DHT sensor
    String tempc = String("T=  ") + String(temp, 1) + String(" C");
    String humid=  String("H=  ") + String(humidity, 1) + String(" %");
    // Update the OLED display with sensor readings
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_15b_tr);  // Set font size for display
    u8g2.drawStr(28, 15, "DustSense");  // Display title
    u8g2.drawStr(28, 30, tempc.c_str());       // Display temperature and humidity
    u8g2.drawStr(28, 45, humid.c_str());       // Display temperature and humidity
    u8g2.sendBuffer();                           // Render the buffer on the OLED
    delay(5000);// delay 5 second
  }
