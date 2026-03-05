# 🔌 Circuit Connections

# ESP32 Pin Map

```
┌─────────────────────────────────────────────────────────┐
│                     ESP32 DevKit                        │
│                                                         │
│  GPIO 4   ──────────────────►  DHT11 DATA Pin           │
│  GPIO 13  ──────────────────►  Relay Module IN Pin      │
│  GPIO 18  ──────────────────►  Active Buzzer (+)        │
│  GPIO 21  ──────────────────►  I2C LCD SDA              │
│  GPIO 22  ──────────────────►  I2C LCD SCL              │
│  GPIO 34  ──────────────────►  MQ-135 AOUT (Analog)     │
│  GPIO 35  ──────────────────►  Soil Moisture AOUT       │
│                                                         │
│  3.3V     ──────────────────►  DHT11 VCC                │
│  5V       ──────────────────►  LCD VCC, Relay VCC       │
│           ──────────────────►  MQ-135 VCC               │
│           ──────────────────►  Soil Sensor VCC          │
│           ──────────────────►  Buzzer VCC               │
│  GND      ──────────────────►  All component GNDs       │
└─────────────────────────────────────────────────────────┘
```

---

## 📋 Detailed Connection Table

| ESP32 Pin | Component | Component Pin | Notes |
|---|---|---|---|
| GPIO 4 | DHT11 | DATA | 10kΩ pull-up resistor recommended |
| GPIO 13 | Relay Module | IN | LOW = pump OFF, HIGH = pump ON |
| GPIO 18 | Active Buzzer | + (positive) | Buzzer GND → ESP32 GND |
| GPIO 21 | I2C LCD | SDA | I2C data line |
| GPIO 22 | I2C LCD | SCL | I2C clock line |
| GPIO 34 | MQ-135 | AOUT | Analog input only pin |
| GPIO 35 | Soil Moisture | AOUT | Analog input only pin |
| 3.3V | DHT11 | VCC | — |
| 5V | LCD, Relay, MQ-135, Buzzer | VCC | Use 5V rail |
| GND | All components | GND | Common ground |

---

## 💧 Relay & Water Pump Wiring

```
ESP32 GPIO 13 ──► Relay IN
5V            ──► Relay VCC
GND           ──► Relay GND

Relay COM ──► Water Pump (+)
Relay NO  ──► 5V Power Supply (+)
Power GND ──► Water Pump (-)
```

> The relay acts as a switch. When ESP32 pulls GPIO 13 HIGH, the relay closes and powers the pump.

---

## 🖥️ I2C LCD Wiring

```
LCD Pin   →   ESP32
────────────────────
VCC       →   5V
GND       →   GND
SDA       →   GPIO 21
SCL       →   GPIO 22
```

> **Default I2C Address: `0x27`**
> If the screen stays completely blank after uploading, change the address in code to `0x3F`:
> ```cpp
> LiquidCrystal_I2C lcd(0x3F, 16, 2);
> ```

---

## 🌡️ DHT11 Wiring

```
DHT11 Pin   →   ESP32
──────────────────────
VCC (Pin 1) →   3.3V
DATA (Pin 2)→   GPIO 4  (+ 10kΩ resistor to VCC)
GND (Pin 4) →   GND
```

---

## 💨 MQ-135 Gas Sensor Wiring

```
MQ-135 Pin  →   ESP32
──────────────────────
VCC         →   5V
GND         →   GND
AOUT        →   GPIO 34
```

> GPIO 34 and 35 on ESP32 are **input-only** ADC pins — perfect for sensors.
> Allow a **24-hour burn-in period** on first use for accurate gas readings.

---

## 🌱 Soil Moisture Sensor Wiring

```
Sensor Pin  →   ESP32
──────────────────────
VCC         →   5V
GND         →   GND
AOUT        →   GPIO 35
```

> Higher analog value = **drier soil**. The code maps 4095→0% and 0→100% moisture.
