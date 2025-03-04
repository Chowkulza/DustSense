#include <Wire.h>                 // Library for I2C communication
#include <PMS.h>                  // Library for PMS sensors (e.g., PMS7003)
#include <DHT.h>                  // Library for DHT sensors (e.g., DHT11 or DHT22)
#include <U8g2lib.h>              // Library for controlling OLED displays
#include <SoftwareSerial.h>       // Library for software-based serial communication

// Initialize DHT sensor on pin D5 with sensor type DHT11
DHT dht(D3, DHT22);

// Define software serial for PMS sensor on pins D2 (RX)>>PMS TX and D1 (TX)>> PMS RX
SoftwareSerial pmsSerial(D2, D1);  
PMS pms(pmsSerial);               // PMS sensor object
PMS::DATA data;                   // Data structure for storing PMS readings

// Variables to store PM sensor readings
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

// Timing variables for periodic sensor readings
unsigned long previousMillis = 0;  
const long interval = 15000;      // Interval for sensor reading (15 seconds) 
// can replaced by xx*1000*60 where xx (eg. 0.25) is the time in minute

// Initialize OLED display object (I2C pins: SCL = 14, SDA = 12)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);

void setup(void) {
  Serial.begin(9600);             // Initialize serial communication for debugging
  pmsSerial.begin(9600);          // Initialize PMS sensor communication
  pms.passiveMode();              // Set PMS sensor to passive mode (on-demand readings)
  u8g2.begin();                   // Initialize OLED display
  dht.begin();                    // Initialize DHT sensor

  // Display a welcome message on the OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);  // Set font size and style
  u8g2.drawStr(10, 31, "DustSense");     // Display "Hello" at coordinates (30, 45)
  u8g2.sendBuffer();                 // Send buffer to OLED for rendering
  delay(5000); //delay 5 second
}

void loop(void) {
  unsigned long currentMillis = millis();  // Get the current time (in milliseconds)
  // Check if specific time have passed since the last reading
  if (currentMillis - previousMillis >= interval) {
    pms.wakeUp();  // Wake up the PMS sensor from sleep mode
    delay(60000); //delay 60 second as the minimum time 30 sec are require
    pms.requestRead();  // Request a reading from the PMS sensor
    // If a valid reading is received, store the data
    if (pms.readUntil(data)) {
      pm1 = data.PM_AE_UG_1_0;    // PM1.0 concentration in µg/m³
      pm2_5 = data.PM_AE_UG_2_5;  // PM2.5 concentration in µg/m³
      pm10 = data.PM_AE_UG_10_0;  // PM10 concentration in µg/m³
    }
    // Create formatted strings for PM values and temperature/humidity
    String pm1Str =  "PM 1    :    " + String(pm1) + "   (ug/m3)";
    String pm25Str = "PM 2.5 :    " + String(pm2_5) + "   (ug/m3)";
    String pm10Str = "PM 10  :    " + String(pm10) + "   (ug/m3)";
    float temp = dht.readTemperature();  // Read temperature from DHT sensor
    float humidity = dht.readHumidity();  // Read humidity from DHT sensor
    String temp_hum = String("T: ") + String(temp, 1) + String(" C | H: ") 
                      + String(humidity, 1) + String(" %");

    // Print sensor readings (srings) to the Serial Monitor
    Serial.println(pm1Str);
    Serial.println(pm25Str);
    Serial.println(pm10Str);
    Serial.println();

    // Update the OLED display with sensor readings
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);  // Set font size for display
    u8g2.drawStr(28, 10, "DustSense PM");  // Display title
    u8g2.drawStr(0, 25, pm1Str.c_str());         // Display PM1.0 reading
    u8g2.drawStr(0, 35, pm25Str.c_str());        // Display PM2.5 reading
    u8g2.drawStr(0, 45, pm10Str.c_str());        // Display PM10 reading
    u8g2.drawStr(15, 60, temp_hum.c_str());       // Display temperature and humidity
    u8g2.sendBuffer();                           // Render the buffer on the OLED
    previousMillis = currentMillis;  // Update the last reading timestamp
    pms.sleep();  // Put the PMS sensor into sleep mode to save power
  }// end if 
} // end loop
