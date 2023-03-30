#include <Arduino_JSON.h>
#include <WiFi.h>
#include <TM1640.h>  // sub-class of TM16xx
#include <TM16xxDisplay.h>
#include <TM1638Anode.h>
#include <WebSocketsClient.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <time.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//millis for Access Point mode
const unsigned long eventInterval = 15000;
unsigned long previousTime = 0;
unsigned long currentTime;
// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "uid";
//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;
String uid;
#define trig_pin 16

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* uidPath = "/uid.txt";

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
//IPAddress localGateway;
IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

const char* serverUrl = "ratedis.nepatronix.org";
const int webSocketPort = 80;
WebSocketsClient webSocket;
WebSocketsServer webSocket1 = WebSocketsServer(81);
#define DIO 25
#define CLK 33
#define STB1 32
#define STB2 26
#define STB3 27
#define STB4 14
#define STB5 12
#define STB6 13
#define STB7 2
#define STB8 4
#define STB9 5
#define STB10 18

// ( byte dataPin, byte clockPin, byte numDigits=4, boolean activateDisplay=true, byte intensity=7, byte displaymode = TM1650_DISPMODE_4x8 )
TM1638Anode module1(DIO, CLK, STB1, true, 7);   // dataPin, clockPin, strobePin, numDigits, activateDisplay, intensity
TM16xxDisplay display1(&module1, 6);    // TM16xx object, 8 digits

TM1638Anode module2(DIO, CLK, STB2, true, 7);
TM16xxDisplay display2(&module2, 6);

TM1638Anode module3(DIO, CLK, STB3, true, 7);
TM16xxDisplay display3(&module3, 6);

TM1638Anode module4(DIO, CLK, STB4, true, 7);
TM16xxDisplay display4(&module4, 6);

TM1638Anode module5(DIO, CLK, STB5, true, 7);
TM16xxDisplay display5(&module5, 6);

TM1638Anode module6(DIO, CLK, STB6, true, 7);
TM16xxDisplay display6(&module6, 6);

TM1638Anode module7(DIO, CLK, STB7, true, 7);
TM16xxDisplay display7(&module7, 6);

TM1638Anode module8(DIO, CLK, STB8, true, 7);
TM16xxDisplay display8(&module8, 6);

TM1638Anode module9(DIO, CLK, STB9, true, 7);
TM16xxDisplay display9(&module9, 6);

TM1638Anode module10(DIO, CLK, STB10, true, 7);
TM16xxDisplay display10(&module10, 6);

//Your Domain name with URL path or IP address with path
String valueReadingsArr[9];
String valueReadings;
String data_date;
String buy_gold_22_per_tola;
String buy_gold_24_per_tola;
String buy_gold_22_per_10g;
String buy_silver;
String sell_gold_22_per_tola;
String sell_gold_24_per_tola;
String sell_gold_22_per_10g;
String sell_silver;
//Dispaly Variables
String s_HGoldTen;
String b_HGoldTen;
String s_SilverTen;
String b_SilverTen;
String s_HGold;
String b_HGold;
String s_TGold;
String b_TGold;

char dateString[20];
char timeString[20];

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  initSPIFFS();
  pinMode(trig_pin, INPUT_PULLUP);
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  uid = readFile (SPIFFS, uidPath);
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(uid);
  String UID = "uid:" + String(uid);
  //  checkWiFi();
  WiFi.begin(ssid.c_str(), pass.c_str());
  configTime(20700, 0, "pool.ntp.org");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  webSocket1.begin();
  webSocket.begin(serverUrl, webSocketPort, "/ws/connect/");
  webSocket.setExtraHeaders(UID.c_str());
  webSocket.onEvent(webSocketEvent);
  delay(3000);
}
void loop() {
  currentTime = millis();
  webSocket.loop();
  webSocket1.loop();
  if (digitalRead(trig_pin) == LOW) {
    if (currentTime - previousTime >= eventInterval) {
      AP_Mode();
      previousTime = currentTime;
    }
  }
  //Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  updateTime();
}
