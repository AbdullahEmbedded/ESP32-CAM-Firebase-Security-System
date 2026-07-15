# 📷 ESP32-CAM Motion Detection System with Firebase

An IoT-based smart security system developed using the **ESP32-CAM** and a **PIR motion sensor**. When motion is detected, the ESP32-CAM captures an image, converts it to Base64 format, and uploads it to **Firebase Realtime Database** using the Firebase REST API.

---

# 🚀 Features

- Motion detection using PIR sensor
- Automatic image capture
- Built-in Flash LED support
- Base64 image encoding
- Upload image to Firebase Realtime Database
- HTTP REST API communication
- Automatic WiFi reconnection
- Camera warm-up before capture
- LED status indicators for system events
- Error handling and automatic restart

---

# 🛠 Hardware Used

| Component | Description |
|-----------|-------------|
| ESP32-CAM AI Thinker | Main Controller |
| PIR Motion Sensor (HC-SR501) | Motion Detection |
| USB to TTL Programmer | Upload Code |
| Breadboard | Prototype |
| Jumper Wires | Connections |
| 5V Power Supply | Power Source |

---

# 💻 Software Used

- Arduino IDE
- ESP32 Board Package
- Firebase Realtime Database
- REST API
- WiFi Library
- ESP32 Camera Library
- HTTPClient Library

---

# ⚙️ Project Workflow

```
PIR Motion Detected
        │
        ▼
ESP32-CAM Detects Motion
        │
        ▼
Flash LED Turns ON
        │
        ▼
Capture Image
        │
        ▼
Convert Image to Base64
        │
        ▼
Connect to Firebase
        │
        ▼
Upload Image using HTTP PUT
        │
        ▼
Store Data in Firebase Realtime Database
```

---

# 📂 Project Structure

```
ESP32-CAM-Firebase-Security-System
│
├── Arduino_Code
│   └── ESP32_CAM_Firebase.ino
│
├── Circuit_Diagram
│
├── Documentation
│
├── Images
│
├── Screenshots
│
├── LICENSE
└── README.md
```

---

# 🔧 Configuration

Before uploading the code, replace the following values:

```cpp
#define WIFI_SSID      "YOUR_WIFI_SSID"
#define WIFI_PASSWORD  "YOUR_WIFI_PASSWORD"
#define DATABASE_URL   "https://your-project-default-rtdb.firebaseio.com/"
```

with your own WiFi credentials and Firebase Realtime Database URL.

---

# 📸 LED Status Indicators

| LED Pattern | Status |
|-------------|--------|
| Slow Blink | System Starting |
| Two Quick Blinks | WiFi Connected |
| Double Blink | Motion Detected |
| Triple Blink | Upload Successful |
| Long Blink | Upload Failed |
| Rapid Blink | WiFi Connection Failed |

---

# 📷 How It Works

1. ESP32-CAM starts and initializes the camera.
2. Connects to the configured WiFi network.
3. Waits for motion detection from the PIR sensor.
4. Captures an image when motion is detected.
5. Encodes the image into Base64.
6. Sends image data to Firebase Realtime Database using an HTTP PUT request.
7. Waits for the next motion event.

---

# 📚 Libraries Used

- Arduino.h
- WiFi.h
- HTTPClient.h
- WiFiClientSecure.h
- esp_camera.h

---

# 📸 Images

Add project images here.

Example:

- ESP32-CAM Hardware
- PIR Sensor Wiring
- Firebase Realtime Database
- Arduino Serial Monitor
- Motion Detection Demo

---

# 🔮 Future Improvements

- Firebase Storage support
- Mobile Application
- Email Notifications
- Push Notifications
- Live Video Streaming
- Face Recognition
- Multiple Camera Support

---

# 👨‍💻 Author

**Muhammad Abdullah**

BS Embedded Systems Student

University of Lahore

Lahore, Pakistan

---

# 📄 License

This project is licensed under the MIT License.
