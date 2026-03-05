# ⚙️ Requirements

## 🔧 Hardware Requirements

| Component | Quantity | Purpose |
|---|---|---|
| ESP32 Development Board | 1 | Main microcontroller & WiFi |
| DHT11 Temperature & Humidity Sensor | 1 | Reads temp and humidity |
| MQ-135 Gas Sensor | 1 | Detects air pollution / gas leaks |
| Soil Moisture Sensor | 1 | Monitors soil water content |
| 5V Relay Module | 1 | Switches the water pump ON/OFF |
| Mini Water Pump (5V DC) | 1 | Irrigation pump |
| I2C LCD Display (16x2) | 1 | Shows sensor data and messages |
| Active Buzzer | 1 | Alarm for gas detection and timer |
| Jumper Wires | Several | Connections between components |
| Breadboard or PCB | 1 | Mounting and prototyping |
| 5V Power Supply / USB Cable | 1 | Powers the entire system |

---

## 💻 Software Requirements

### Tools
- [Arduino IDE](https://www.arduino.cc/en/software) — v1.8 or v2.x

### ESP32 Board Package
- Install via **Board Manager** in Arduino IDE
- Board Manager URL:
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```

### Required Libraries
Install all via **Sketch > Include Library > Manage Libraries**:

| Library | Author | Purpose |
|---|---|---|
| `WiFiManager` | tzapu | Captive portal WiFi setup |
| `LiquidCrystal_I2C` | Frank de Brabander | I2C LCD control |
| `DHT sensor library` | Adafruit | DHT11 sensor readings |
| `Adafruit Unified Sensor` | Adafruit | Dependency for DHT library |
| `WebServer` | ESP32 Core (built-in) | Serving the web dashboard |
| `WiFi` | ESP32 Core (built-in) | WiFi connectivity |
