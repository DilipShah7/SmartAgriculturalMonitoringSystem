# 📶 WiFi Connection Process

When the ESP32 is powered on for the first time, it enters setup mode and displays the following on the LCD screen:

```
WiFi Setup Mode
Check Phone WiFi
```

---

## Step 1 — Connect to the Hotspot

On your phone or laptop, open WiFi settings and connect to the hotspot named:

```
System-WiFi-Setup
```

---

## Step 2 — Open the Setup Page

Once connected, a setup page will automatically open in your browser.

If it does not open automatically, go to this address manually:

```
http://192.168.4.1
```

---

## Step 3 — Select Your WiFi

On the setup page, select your home or local WiFi network from the list, enter its password, and tap **Save**.

---

## Step 4 — Connected!

The ESP32 will connect to your WiFi and show this on the LCD:

```
Connected!
192.168.1.XX
```

Note down the IP address shown — you will need it to open the dashboard.

---

## Step 5 — Access the Dashboard

Open any browser on a device connected to the **same WiFi network** and go to:

```
http://<IP_ADDRESS_SHOWN_ON_LCD>
```

For example: `http://192.168.1.45`

---

## 🔁 Next Boot

From the next boot onwards, the ESP32 remembers the WiFi credentials and connects automatically — no setup needed again.

> To reset WiFi and re-enter setup mode, flash the code again or clear the saved credentials via code.
