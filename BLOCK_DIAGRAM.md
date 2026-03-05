# 📊 System Block Diagram

## Overall System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        POWER SUPPLY (5V)                            │
└───────────────────────────────┬─────────────────────────────────────┘
                                │
                    ┌───────────▼───────────┐
                    │                       │
                    │     ESP32 DevKit      │◄────── NTP Server (WiFi)
                    │   (Central Control)   │◄────── Web Browser (User)
                    │                       │
                    └───┬───┬───┬───┬───┬───┘
                        │   │   │   │   │
           ┌────────────┘   │   │   │   └─────────────┐
           │                │   │   │                  │
           ▼                ▼   │   ▼                  ▼
   ┌───────────────┐  ┌─────────┐ │  ┌──────────┐  ┌──────────────┐
   │  DHT11 Sensor │  │ MQ-135  │ │  │  Soil    │  │  I2C LCD     │
   │               │  │  Gas    │ │  │ Moisture │  │  16x2        │
   │ Temp/Humidity │  │ Sensor  │ │  │  Sensor  │  │  Display     │
   └───────────────┘  └─────────┘ │  └──────────┘  └──────────────┘
                                  │
                    ┌─────────────┴──────────────┐
                    │                            │
                    ▼                            ▼
           ┌─────────────────┐        ┌─────────────────┐
           │  Active Buzzer  │        │  Relay Module   │
           │                 │        │                 │
           │  • Gas Alarm    │        │  • Water Pump   │
           │  • Timed Alarm  │        │    Control      │
           └─────────────────┘        └────────┬────────┘
                                               │
                                               ▼
                                      ┌─────────────────┐
                                      │   Water Pump    │
                                      │  (Irrigation)   │
                                      └─────────────────┘
```

---

## Input / Output Breakdown

```
┌─────────────────────────────────────────────────────┐
│                   INPUTS                            │
├─────────────────────────────────────────────────────┤
│  DHT11         →  Temperature, Humidity             │
│  MQ-135        →  Air Quality (Analog 0–4095)       │
│  Soil Sensor   →  Soil Moisture (Analog 0–4095)     │
│  NTP Server    →  Real-time Clock (via WiFi)        │
│  Web Browser   →  Custom messages, Alarm time       │
└─────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────┐
│                   OUTPUTS                           │
├─────────────────────────────────────────────────────┤
│  I2C LCD       →  Sensor readings, Messages         │
│  Relay/Pump    →  Automatic irrigation              │
│  Buzzer        →  Gas & alarm alerts                │
│  Web Server    →  Live dashboard (HTML)             │
└─────────────────────────────────────────────────────┘
```

---

## Communication Architecture

```
┌───────────────┐        WiFi (802.11)       ┌────────────────┐
│  Smartphone   │ ◄─────────────────────────► │     ESP32      │
│  or Browser   │   HTTP on local network     │  Web Server    │
└───────────────┘                             │  Port 80       │
                                              └────────────────┘
                                                      │
                                                      │ Internet
                                                      ▼
                                              ┌────────────────┐
                                              │  NTP Server    │
                                              │ pool.ntp.org   │
                                              └────────────────┘

Sensors → ESP32 ADC/GPIO (wired, direct)
LCD     → ESP32 via I2C bus (SDA/SCL)
Relay   → ESP32 GPIO digital out (wired)
Buzzer  → ESP32 GPIO digital out (wired)
```

---

## LCD Screen Rotation Flow

```
  ┌─────────────────────────────────────────────────┐
  │           Custom Message Received?              │
  │         (via /msg endpoint)                     │
  └────────────────┬───────────┬────────────────────┘
                 YES            NO
                  │              │
                  ▼              ▼
         ┌──────────────┐  ┌─────────────────────┐
         │ Show custom  │  │ Auto-Rotate Screens  │
         │ message for  │  │   every 5 seconds    │
         │  10 seconds  │  │                      │
         └──────────────┘  │ Screen 0             │
                           │ Temp | Humidity | Time│
                           │                      │
                           │ Screen 1             │
                           │ Air Quality + Value  │
                           │                      │
                           │ Screen 2             │
                           │ Soil % | Pump Status │
                           └─────────────────────┘
```
