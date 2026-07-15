# ESP32-CAM Motion Detection System with Firebase

## Project Overview

This project is an IoT-based smart security system that detects motion using a PIR sensor and captures an image using the ESP32-CAM. The captured image is encoded into Base64 format and uploaded to Firebase Realtime Database using the Firebase REST API.

---

# Objectives

- Detect human motion automatically.
- Capture an image when motion is detected.
- Send the captured image to Firebase.
- Monitor events remotely through the cloud.
- Learn ESP32-CAM programming and IoT communication.

---

# Hardware Components

| Component | Quantity |
|-----------|---------:|
| ESP32-CAM AI Thinker | 1 |
| PIR Motion Sensor (HC-SR501) | 1 |
| USB-to-TTL Programmer | 1 |
| Breadboard | 1 |
| Jumper Wires | As required |
| 5V Power Supply | 1 |

---

# Software Requirements

- Arduino IDE
- ESP32 Board Package
- Firebase Realtime Database
- WiFi Library
- HTTPClient Library
- ESP32 Camera Library

---

# System Workflow

1. Power on the ESP32-CAM.
2. Connect to the configured Wi-Fi network.
3. Initialize the camera module.
4. Monitor the PIR sensor for motion.
5. Capture an image when motion is detected.
6. Encode the image into Base64 format.
7. Upload the image to Firebase Realtime Database.
8. Wait for the next motion event.

---

# Features

- Motion Detection
- Automatic Image Capture
- Wi-Fi Connectivity
- Firebase Integration
- Base64 Image Encoding
- REST API Communication
- Automatic Wi-Fi Reconnection
- Flash LED Status Indicators

---

# Project Structure

```
ESP32-CAM-Firebase-Security-System
│
├── Arduino_Code
├── Circuit_Diagram
├── Documentation
├── Images
├── Screenshots
└── README.md
```

---

# Future Improvements

- Upload images to Firebase Storage instead of storing Base64 in the database.
- Mobile application for monitoring.
- Push notifications.
- Email alerts.
- Live video streaming.
- Face recognition.
- Cloud event history.

---

# Learning Outcomes

Through this project I learned:

- ESP32-CAM programming
- Embedded Systems
- IoT Application Development
- Firebase Integration
- REST API Communication
- Wi-Fi Networking
- Camera Programming
- Cloud-Based Monitoring

---

# Author

**Muhammad Abdullah**

BS Embedded Systems

University of Lahore
