# 🧠 System Logic

## How the System Works

The ESP32 acts as the central brain of the entire system. On every loop cycle (every 200ms), it reads all sensors, makes decisions, updates the LCD display, and serves the web dashboard simultaneously.

---

## 🔁 Main Logic Flow

```
POWER ON
   │
   ▼
Initialize Sensors, LCD, Serial
   │
   ▼
WiFiManager Captive Portal
   │  (user connects to "Love-Bot-Setup" hotspot)
   │  (user selects WiFi and enters password)
   ▼
Connected to WiFi
   │
   ▼
Display IP Address on LCD (5 seconds)
   │
   ▼
Sync Time via NTP (Nepal Standard Time UTC+5:45)
   │
   ▼
Start Web Server on Port 80
   │
   ▼
┌─────────────────────────────────────────────────────┐
│                    MAIN LOOP (every 200ms)          │
│                                                     │
│  ┌─────────────────────────────────────────────┐   │
│  │           READ SENSORS                      │   │
│  │  - DHT11: Temperature & Humidity            │   │
│  │  - MQ-135: Gas value (analog)               │   │
│  │  - Soil Sensor: Moisture value (analog)     │   │
│  │  - NTP: Current time (HH:MM)                │   │
│  └───────────────────┬─────────────────────────┘   │
│                      │                             │
│          ┌───────────▼────────────┐                │
│          │  Soil Moisture < 30%?  │                │
│          └───────┬───────┬────────┘                │
│                YES       NO                        │
│                 │         │                        │
│          PUMP ON      PUMP OFF                     │
│                                                    │
│          ┌───────────▼────────────┐                │
│          │   Gas Value > 1500?    │                │
│          │   OR Alarm Active?     │                │
│          └───────┬───────┬────────┘                │
│                YES       NO                        │
│                 │         │                        │
│           BEEP BUZZER   BUZZER OFF                 │
│                                                    │
│          ┌─────────────────────────────────────┐   │
│          │  Alarm time == Current time?        │   │
│          │  → Set alarmActive = true           │   │
│          └─────────────────────────────────────┘   │
│                                                    │
│          ┌─────────────────────────────────────┐   │
│          │        LCD DISPLAY LOGIC            │   │
│          │                                     │   │
│          │  Custom message sent in last 10s?   │   │
│          │  YES → Show custom message          │   │
│          │  NO  → Rotate screens every 5s:     │   │
│          │        Screen 0: Temp, Humidity,    │   │
│          │                  Current Time       │   │
│          │        Screen 1: Air Quality,       │   │
│          │                  Gas Sensor Value   │   │
│          │        Screen 2: Soil Moisture %,   │   │
│          │                  Pump Status        │   │
│          └─────────────────────────────────────┘   │
│                                                    │
│          ┌─────────────────────────────────────┐   │
│          │    Serve Web Client Requests         │   │
│          │    /         → Dashboard            │   │
│          │    /msg      → Show message on LCD  │   │
│          │    /setalarm → Set alarm time        │   │
│          │    /stop     → Dismiss alarm         │   │
│          └─────────────────────────────────────┘   │
└─────────────────────────────────────────────────────┘
```

---

## 🌊 Auto Irrigation Logic

```cpp
if (soilPresent && moisturePercent < 30)
    PUMP = ON
else
    PUMP = OFF
```

- Soil sensor is checked to confirm it is plugged in (value between 10 and 4090)
- Raw analog value (0–4095) is mapped to 0–100% moisture
- Pump relay is activated only when needed — prevents overwatering

---

## 💨 Air Quality & Alarm Logic

```cpp
if (alarmActive || gasVal > 1500)
    BUZZER = BEEP (toggles every 250ms)
else
    BUZZER = OFF
```

- Gas value threshold of **1500** (out of 4095) triggers danger mode
- Buzzer alternates ON/OFF every 250ms for an audible pulsing alarm
- Alarm can be dismissed via web dashboard `/stop` endpoint

---

## 📟 LCD Screen Rotation

| Screen | Duration | Content |
|---|---|---|
| 0 — Weather | 5 seconds | Temperature, Humidity, Current Time |
| 1 — Air Quality | 5 seconds | Gas sensor value + status (OK / DANGER) |
| 2 — Agriculture | 5 seconds | Soil moisture % + Pump status (ON/OFF) |

- Custom messages override all screens for **10 seconds**, then rotation resumes
- If soil sensor is absent (disconnected), Screen 2 is skipped automatically

---

## 🌐 Web Server Endpoints

| Route | Action |
|---|---|
| `GET /` | Returns the live HTML dashboard with injected sensor data |
| `GET /msg?l1=...&l2=...` | Sends a 2-line message to the LCD for 10 seconds |
| `GET /setalarm?atime=HH:MM` | Sets the alarm time string |
| `GET /stop` | Sets `alarmActive = false` and silences the buzzer |
