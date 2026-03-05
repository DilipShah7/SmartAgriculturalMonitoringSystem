# 🌱 Smart Agricultural Monitoring System

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Language](https://img.shields.io/badge/Language-Arduino%20C%2B%2B-orange)
![License](https://img.shields.io/badge/License-MIT-green)
![Status](https://img.shields.io/badge/Status-Active-brightgreen)

---

## 📌 Introduction

Agriculture is the backbone of developing countries, yet traditional farming methods face challenges such as inefficient water usage, lack of environmental monitoring, and limited access to smart technologies. With the advancement of the **Internet of Things (IoT)**, it is now possible to automate agricultural processes and monitor environmental conditions in real time.

This project — **"Smart Agricultural Monitoring System"** — is an ESP32-based IoT solution that integrates smart irrigation, environmental protection, air quality monitoring, and wireless communication into a single low-cost platform. It also features a **secret wireless messenger** that lets users send text directly to an LCD display from any browser on the same network.

---

## ⚠️ Problem Statement

Traditional and manual farming approaches suffer from several critical limitations:

- Manual irrigation leads to **excessive and inefficient water usage**
- Farmers lack access to **real-time soil moisture and air quality data**
- Environmental hazards like **gas pollution often go undetected**
- Available smart farming systems are **costly and technically complex**
- There is no easy way to **remotely communicate** with on-field display boards

There is therefore a clear need for a **low-cost, automated, and intelligent agricultural monitoring system** accessible to small and medium-scale farmers.

---

## 🌍 Applications

This system can be applied across a variety of real-world use cases:

- 🌾 **Smart Agriculture** — Automated irrigation and crop condition monitoring
- 🏭 **Environmental Monitoring** — Detecting harmful gases in fields or enclosed spaces
- 🏠 **Smart Home Automation** — Repurposing as a home environment monitor with alerts
- 🎓 **Academic & Research Projects** — IoT demonstrations and embedded systems coursework
- 🏫 **Exhibitions & Science Fairs** — Showcasing IoT, sensors, and wireless control

---

## ✅ Expected Solution

The proposed system uses an **ESP32 microcontroller** as the central control unit with the following capabilities:

- **Automated Irrigation** — Water pump activates automatically when soil moisture drops below 30%, and turns off when moisture is sufficient
- **Air Quality Monitoring** — MQ-135 gas sensor continuously checks air and triggers a buzzer alarm when dangerous gas levels are detected
- **Temperature & Humidity Tracking** — DHT11 sensor reads real-time environmental data displayed on LCD and web dashboard
- **Web-Based Dashboard** — A mobile-friendly interface served from the ESP32 itself, accessible from any browser on the same network — no internet required
- **Secret Messenger** — Users can wirelessly send custom two-line messages to the LCD display via browser
- **Timed Alarm** — Set and dismiss alarms remotely from the web interface
- **WiFi Auto-Setup** — First-boot captive portal using WiFiManager — no hardcoded passwords needed
- **NTP Time Sync** — Automatically syncs accurate local time (Nepal Standard Time, UTC+5:45) from the internet

---

## 📂 Repository Structure

```
SmartAgri/
│
├── README.md                  ← You are here
│
├── code/
│   └── SmartAgri.ino          ← Full Arduino source code
│
├── docs/
│   ├── REQUIREMENTS.md        ← Hardware & software requirements
│   └── SYSTEM_LOGIC.md        ← How the system works, logic flow
│
├── circuit/
│   └── CIRCUIT_CONNECTION.md  ← Wiring and pin connection guide
│
├── diagrams/
│   └── BLOCK_DIAGRAM.md       ← System block diagram (ASCII)
│
└── LICENSE                    ← MIT License
```

---

## 🔗 Quick Links

| Section | File |
|---|---|
| ⚙️ Requirements | [docs/REQUIREMENTS.md](docs/REQUIREMENTS.md) |
| 🔌 Circuit Connections | [circuit/CIRCUIT_CONNECTION.md](circuit/CIRCUIT_CONNECTION.md) |
| 🧠 System Logic | [docs/SYSTEM_LOGIC.md](docs/SYSTEM_LOGIC.md) |
| 📊 Block Diagram | [diagrams/BLOCK_DIAGRAM.md](diagrams/BLOCK_DIAGRAM.md) |
| 💻 Project Code | [code/SmartAgri.ino](code/SmartAgri.ino) |

---

<p align="center">Made with ❤️ for Smart Farming</p>
