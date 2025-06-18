// Modified by Mike Eitel to read temperatur and humidity by ESP32-C3 based board
// All special modified libraries are stored in the via platformio.ini defined libraries directory !!!
// Took some time to get the right versions etc. working flawless, so this unusual approach is choosen deliberately.
// No rights reserved when used non commercial, otherwise contact author.

// XIAO-ESP32-C3 pinout 
// D0   GPIO2   A0/D0    =  2     Used for powering DHT to have it resetable
// D1   GPIO3   A1/D1    =  3     Used for DHT data
// D2   GPIO4   A2/D2    =  4     Used for DS18B20x data
// D3   GPIO5   A3/D3    =  5     Chip select for MX6675
// D4   GPIO6   SDA/D4   =  6     Data for MAX6675
// D5   GPIO7   SCL/D5   =  7     Clock for MAX6675
// D6   GPIO21  TX/D6    = 21     Not used

// 5V                             Supply 5V
// GND                            GND
// 3V3                            Internal 3.3V
// D10  GPIO10  MOSI/D10  = 10    
// D9   GPIO3   MISO/D9   =  9    BOOT button on board
// D8   GPIO4   SCK/D8    =  8    Boot option
// D7   GPIO5   RX/D7/SS  = 20    Not used


//#define TEST     // Testmodus
//#define OTA       // If defined use OTA programming posibility

#include <WiFi.h>

const char* ssid = "ESP32_Server";
const char* password = "12345678";
const char* clientID = "ANT1";

#if defined(OTA)              // Augment by OTA posibility
  #include <ArduinoOTA.h>

void setupOTA(){                          // --- OTA Setup ---
   ArduinoOTA.setPort(3232);              // Port defaults to 3232

  // Hostname defaults to esp32-[ChipID] Set this BEFORE calling begin()
  ArduinoOTA.setHostname(iamclient);      // Choose a unique name
  // No authentication by default
  // Password can be set with it's MD5 hash as well   MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //ArduinoOTA.setPassword("MySecurePassword"); // <--- CHANGE THIS TO A STRONG PASSWORD!

  // OTA event handlers
  ArduinoOTA.onStart([]() {
    String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.begin(); // Start the OTA service

  Serial.println("OTA Initialized");
}

void checkOTA(){
  ArduinoOTA.handle();
}
#endif

// XXXXXXXXXXXXXXXXXXXXXXXX PROGRAM  START XXXXXXXXXXXXXXXXXXXXXXX

void setup() {
  Serial.begin(115200);
  Serial.println("Start");
  WiFi.begin(ssid, password);
  WiFi.setTxPower(WIFI_POWER_21dBm);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to AP");
}

//  This is the Main loop
void loop() {    
  int rssi = WiFi.RSSI();
  int channel = WiFi.channel();
  WiFiClient client;
  if (client.connect("192.168.4.1", 80)) {
    client.print("GET /report?rssi=");
    client.print(rssi);
    client.print("&id=");
    client.print(clientID);
    client.print("&channel=");
    client.print(channel);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.4.1");
    client.println("Connection: close");
    client.println();
    Serial.print("Sent RSSI: ");
    Serial.println(rssi);
    } else {
    Serial.println("❌ Not connected");
  }
  client.stop();
  Serial.print(".");
  delay(1000);
                       

  #if defined(TEST)

  #endif

  delay(10);
}