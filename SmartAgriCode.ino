#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "time.h"
#include <WiFiManager.h> 

// --- NETWORK & TIME SETTINGS ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 20700; // Nepal Time (UTC+5:45)
const int   daylightOffset_sec = 0;

// --- PIN DEFINITIONS ---
#define DHTPIN 4
#define BUZZER_PIN 18
#define GAS_PIN 34    // MQ-135 Gas Sensor
#define SOIL_PIN 35   // Soil Moisture Sensor
#define PUMP_PIN 13   // Relay for Water Pump

// --- SENSOR INITIALIZATION ---
DHT dht(DHTPIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change to 0x3F if screen is blank
WebServer server(80);

// --- GLOBAL VARIABLES ---
String customMsg1 = "", customMsg2 = "";
String alarmTime = "00:00"; 
bool alarmActive = false;
unsigned long msgStartTime = 0;
unsigned long lastScreenSwitch = 0;
int currentScreen = 0; 

// --- MOBILE DASHBOARD (HTML) ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="refresh" content="5"> <style>
    body { font-family: sans-serif; text-align: center; background: #fff0f5; padding: 15px;}
    .box { background: white; padding: 20px; border-radius: 20px; box-shadow: 0 10px 20px rgba(0,0,0,0.1); max-width: 400px; margin: auto; }
    h2 { color: #ff1493; margin-bottom: 5px; }
    .status-card { background: #f8f9fa; padding: 15px; border-radius: 10px; margin: 15px 0; text-align: left; border-left: 5px solid #ff1493; font-size: 14px;}
    input { padding: 12px; margin: 8px 0; width: 90%; border-radius: 8px; border: 1px solid #ffb6c1; }
    .btn { background: #ff1493; color: white; border: none; padding: 15px; width: 95%; border-radius: 8px; cursor: pointer; font-weight: bold; }
    .on { color: #d9534f; font-weight: bold; } /* Red for active pump */
    .off { color: #5cb85c; font-weight: bold; } /* Green for standby */
  </style>
</head><body>
  <div class="box">
    <h2>Smart Agricultural Monitoring System</h2>
    <div class="status-card">
      <strong>Pump Status:</strong> %PUMP_STATUS%<br>
      <strong>Soil Moisture:</strong> %MOISTURE%<br>
      <strong>Air Quality:</strong> %AIR_STATUS%
    </div>
    <form action="/msg">
      <input type="text" name="l1" placeholder="Msg Line 1" maxlength="16"><br>
      <input type="text" name="l2" placeholder="Msg Line 2" maxlength="16"><br>
      <input type="submit" class="btn" value="Send to Display">
    </form>
    <hr style="border: 0.5px solid #eee; margin: 15px 0;">
    <form action="/setalarm">
      <strong>Alarm (HH:MM):</strong><br>
      <input type="time" name="atime"><br>
      <input type="submit" class="btn" value="Set Alarm">
    </form>
    <br><button class="btn" style="background:#6c757d;" onclick="location.href='/stop'">Dismiss Alarm</button>
  </div>
</body></html>)rawliteral";

// --- WEB SERVER FUNCTIONS ---
void handleRoot() {
  String html = index_html;
  
  // Inject Pump Status
  html.replace("%PUMP_STATUS%", (digitalRead(PUMP_PIN) == HIGH) ? "<span class='on'>ON</span>" : "<span class='off'>OFF</span>");

  // Inject Moisture Data
  int soilVal = analogRead(SOIL_PIN);
  if (soilVal > 10 && soilVal < 4090) {
    int moisture = map(soilVal, 4095, 0, 0, 100);
    html.replace("%MOISTURE%", String(moisture) + "%");
  } else {
    html.replace("%MOISTURE%", "No Sensor");
  }

  // Inject Air Status
  int gasVal = analogRead(GAS_PIN);
  html.replace("%AIR_STATUS%", (gasVal > 1500) ? "<span class='on'>DANGER</span>" : "<span class='off'>GOOD</span>");

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW); // Start with pump off

  Wire.begin(21, 22);
  lcd.init(); lcd.backlight();
  dht.begin();

  // WiFi Setup Mode
  lcd.print("WiFi Setup Mode");
  lcd.setCursor(0,1); lcd.print("Check Phone WiFi");

  WiFiManager wm;
  wm.autoConnect("Love-Bot-Setup"); // Create hotspot

  lcd.clear();
  lcd.print("Connected!");
  lcd.setCursor(0, 1); lcd.print(WiFi.localIP()); 
  delay(5000); 
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  server.on("/", handleRoot);
  server.on("/msg", []() {
    customMsg1 = server.arg("l1"); customMsg2 = server.arg("l2");
    msgStartTime = millis();
    server.send(200, "text/html", "<script>location.href='/';</script>");
  });
  server.on("/setalarm", []() {
    alarmTime = server.arg("atime");
    server.send(200, "text/html", "<script>location.href='/';</script>");
  });
  server.on("/stop", []() {
    alarmActive = false; digitalWrite(BUZZER_PIN, LOW);
    server.send(200, "text/html", "<script>location.href='/';</script>");
  });
  server.begin();
}

void loop() {
  server.handleClient();
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) return;

  char currentTimeStr[6];
  strftime(currentTimeStr, 6, "%H:%M", &timeinfo);

  // Environmental & Agriculture Logic
  int gasVal = analogRead(GAS_PIN);
  int soilVal = analogRead(SOIL_PIN);
  bool soilPresent = (soilVal > 10 && soilVal < 4090);
  int moisturePercent = map(soilVal, 4095, 0, 0, 100);

  // Auto-Pump: Moisture < 30%
  if (soilPresent && moisturePercent < 30) digitalWrite(PUMP_PIN, HIGH);
  else digitalWrite(PUMP_PIN, LOW);

  // Alarm & Gas Warning
  if (String(currentTimeStr) == alarmTime && timeinfo.tm_sec < 2) alarmActive = true;
  if (alarmActive || gasVal > 1500) digitalWrite(BUZZER_PIN, (millis() % 500 < 250));
  else digitalWrite(BUZZER_PIN, LOW);

  // Display Screen Management
  if ((customMsg1 != "" || customMsg2 != "") && (millis() - msgStartTime < 10000)) {
    lcd.setCursor(0,0); lcd.print(customMsg1 + "                ");
    lcd.setCursor(0,1); lcd.print(customMsg2 + "                ");
  } else {
    customMsg1 = ""; customMsg2 = "";
    if (millis() - lastScreenSwitch > 5000) {
      lastScreenSwitch = millis();
      currentScreen++;
      if (currentScreen > 2) currentScreen = 0;
    }
    lcd.clear();
    if (currentScreen == 0) { // Time/Weather
      lcd.print("T:"); lcd.print((int)dht.readTemperature()); lcd.print("C H:"); lcd.print((int)dht.readHumidity()); lcd.print("%");
      lcd.setCursor(0,1); lcd.print("Time: "); lcd.print(currentTimeStr);
    } else if (currentScreen == 1) { // Air Quality
      lcd.print("Air Quality:");
      lcd.setCursor(0,1); lcd.print("Val:"); lcd.print(gasVal); lcd.print(gasVal > 1500 ? " !DANGER!" : " OK");
    } else if (currentScreen == 2) { // Agriculture
      if (soilPresent) {
        lcd.print("Soil Moisture:");
        lcd.setCursor(0,1); lcd.print(moisturePercent); lcd.print("% PUMP:"); lcd.print(digitalRead(PUMP_PIN) ? "ON" : "OFF");
      } else { currentScreen = 0; }
    }
  }
  delay(200);
}
