# Smart Gas Leak Detector
###Smart simple IoT for Gas Leak Detector which is integrated with Platform Blynk and can close the gas flow if it detected gas leak
----------------------------------------------------------------------------
## Component Use : 
- one Breadboard
- one ESP32-WROOM-32D
- one MQ-6 Senor
- one Buzzer 5V
- one LED 5mm
- one Solenoid Valve 12V
- one Step Up converter
- one Relay 1 channel
- one resistor 150-220 ohm
- Cable Jumper

## Software Use :
- Arduino IDE
- Blynk IoT app
- Internet
- Latest Blynk Library

## Wiring Diagram
![gasLeakDetector3dDiagram_bb](https://github.com/user-attachments/assets/1f884a6c-742c-42f5-8ccb-8c7454d07845)

## Install Blynk Library in Arduino IDE
- Open Arduino IDE
- Go to Sketch > Include Library > Manage Libraries
- Search for "Blynk" and install the latest version
- Make sure you’ve also installed ESP32/ESP8266 boards via Boards Manager

## Create a New Project in Blynk App
- Open Blynk IoT app
- Log in or sign up
- Tap “+ New Device” > “Quickstart Template”
- Set:
  - Template Name (e.g., "Smart Leak Detector")
  - Device Name
  - Select Board Type (ESP32)
- After setup, note down:
  - Template ID
  - Template Name
  - Auth Token

## Set Up Blynk Dashboard
- Add Widgets like Button, Slider, Gauge, etc.
- Assign each widget to a Virtual Pin, note down:
  - GasValueH (V0) -> Double, 
