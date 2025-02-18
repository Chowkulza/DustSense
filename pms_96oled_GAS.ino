#include <Arduino.h>
#include <Wire.h>                 // Library for 1-wire communication
#include <PMS.h>                  // Library for PMS sensors (e.g., PM75003)
#include <DHT.h>                  // Library for DHT sensors (e.g., DHT11 or DHT22)
#include <U8g2lib.h>              // Library for controlling OLED displays
#include <SoftwareSerial.h>       // Library for software-based serial communication
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Initialize DHT sensor on pin D5 with sensor type DHT11
DHT dht(D3, DHT22);

// Define software serial for PMS sensor on pins D2 (RX)>>PMS TX and D1 (TX)>> PMS RX
SoftwareSerial pmsSerial(D2, D1);  
PMS pms(pmsSerial);               // PMS sensor object
PMS::DATA data;                   // Data structure for storing PMS readings

//.................setup wifi.......
//Define Wifi SSID and Password
const char* ssid = "xxxx"; //--> Wifi / SSID.
const char* password = "xxxx"; //-->  Password wifi
 
//.....Host & httpsPort config.......
const char* host = "script.google.com";
const int httpsPort = 443;
// define the google apps script ID here
String GAS_ID="xxxxxxxxxxxxxxxx"; //-->spreadsheet script ID *** edits here!!!!

WiFiClientSecure client;

// Variables to store PM sensor readings
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

// Timing variables for periodic sensor readings
unsigned long previousMillis = 0;  
const long interval = 60*1000*1;      // Interval for sensor reading (15 seconds)


// Initialize OLED display object (I2C pins: SCL = 14, SDA = 12)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, 12, 14, U8X8_PIN_NONE);

void setup(void) {
  
  //rs232, pms, and u8g2 setup
  Serial.begin(9600);             // Initialize serial communication for debugging 9600 baud rate(ความเร็วในการส่งสัญญานแบบ serial)
  pmsSerial.begin(9600);          // Initialize PMS sensor communication
  pms.passiveMode();              // Set PMS sensor to passive mode (on-demand readings)
  u8g2.begin();                   // Initialize OLED display
  dht.begin();                    // Initialize DHT sensor
  
  // Display a welcome message on the OLED
  u8g2.clearBuffer(); //clearหน้าจอ
  u8g2.setFont(u8g2_font_ncenB14_tr);  // Set font size and style
  u8g2.drawStr(10, 45, "DustSense");     // Display "Dustsense" at coordinates (10, 45)
  u8g2.sendBuffer();                 // Send buffer to OLED for rendering
  delay(30000);// delay 5 second
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  
  // Start the hardware Serial communication for the computer
  //Serial.begin(9600);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) { //still not connected print 'dot'
  Serial.print("."); //print ที่คอม
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);  // Set font size and style
  u8g2.drawStr(0, 45, "Wifi Connecting..");     // Display "Hello" at coordinates (0, 45)
  u8g2.sendBuffer();                 // Send buffer to OLED for rendering
  } //end while loop
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_ncenB14_tr);  // Set font size and style
  u8g2.drawStr(0, 45, "Wifi Connected");     // Display "Hello" at coordinates (0, 45)
  u8g2.sendBuffer();                 // Send buffer to OLED for rendering
  //----------------------------------------
  client.setInsecure();
}

void loop(void) {
  unsigned long currentMillis = millis();  // Get the current time (in milliseconds)

    

  // Check if 15 seconds have passed since the last reading
  if (currentMillis - previousMillis >= interval) {
    pms.wakeUp(); // Wake up the PMS sensor from sleep mode
    delay(50000);
    pms.requestRead();  // Request a reading from the PMS sensor
    
    // If a valid reading is received, store the data
    if (pms.readUntil(data)) {
      pm1 = data.PM_AE_UG_1_0;    // PM1.0 concentration in µg/m³
      pm2_5 = data.PM_AE_UG_2_5;  // PM2.5 concentration in µg/m³
      pm10 = data.PM_AE_UG_10_0;  // PM10 concentration in µg/m³
    }

    // Create formatted strings for PM values and temperature/humidity
    String pm1Str =  "PM 1   :    " + String(pm1) + "   (ug/m3)";
    String pm25Str = "PM 2.5 :    " + String(pm2_5) + "   (ug/m3)";
    String pm10Str = "PM 10  :    " + String(pm10) + "   (ug/m3)";
    float temp = dht.readTemperature();  // Read temperature from DHT sensor
    float humidity = dht.readHumidity();  // Read humidity from DHT sensor
    String temp_hum = String("    T: ") + String(temp, 1) + String(" C | H: ") 
                      + String(humidity, 0) + String(" %");

    // Print sensor readings to the Serial Monitor
    Serial.print("PM 1.0 : ");
    Serial.println(pm1);
    Serial.print("PM 2.5 : ");
    Serial.println(pm2_5);
    Serial.print("PM 10 : ");
    Serial.println(pm10);
    Serial.println(temp_hum);
    Serial.println();

    // Update the OLED display with sensor readings
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);  // Set font size for display
    //u8g2.drawStr(0, 10, "        AIR QUALITY");  // Display title
    u8g2.drawStr(0, 10, "    DustSense PM");  // Display title
    u8g2.drawStr(0, 25, pm1Str.c_str());         // Display PM1.0 reading
    u8g2.drawStr(0, 35, pm25Str.c_str());        // Display PM2.5 reading
    u8g2.drawStr(0, 45, pm10Str.c_str());        // Display PM10 reading
    u8g2.drawStr(0, 60, temp_hum.c_str());       // Display temperature and humidity
    u8g2.sendBuffer();                           // Render the buffer on the OLED
    sendData(temp,humidity,pm1,pm2_5,pm10); //send to google sheet
    previousMillis = currentMillis;  // Update the last reading timestamp
    pms.sleep();  // Put the PMS sensor into sleep mode to save power
  }
}


// send data function
void sendData(float value1, float value2, unsigned int value3, unsigned int value4, unsigned int value5) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Connect to Google host script.google.com
  if (!client.connect(host, httpsPort)) { // If not connected
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------
  float string_t = value1;
  float string_h = value2;
  unsigned int string_pm1 = value3; 
  unsigned int string_pm25 = value4;
  unsigned int string_pm10 = value5;

  // Construct the URL for the GET request
  String url = "/macros/s/" + GAS_ID + "/exec?temp=" + string_t + "&humid=" + string_h +
               "&pm1=" + string_pm1 + "&pm25=" + string_pm25 + "&pm10=" + string_pm10;

  Serial.print("requesting URL: ");
  Serial.println(url); // To check the URL being sent

  // Send the GET request
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
} // End of sendData function
