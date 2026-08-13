# 🚗 Smart Parking System Using Ultrasonic Sensors

A real-time embedded parking management system that automatically detects parking-slot occupancy using ultrasonic sensors, calculates available parking spaces, and provides visual and audio status alerts. The complete four-slot prototype is implemented and tested virtually using Wokwi with an Arduino UNO.

## 👨‍💻 Author

**Adarsh Srivastav**  
Computer Science and Engineering (CSE) Student  
Embedded Systems | IoT | Python | AI

## 📌 Project Overview

The Smart Parking System is an embedded-system project designed to automate parking-slot monitoring. The system uses four HC-SR04 ultrasonic sensors, with each sensor assigned to an individual parking slot. The Arduino UNO continuously measures the distance detected by each sensor and classifies the corresponding slot as **FREE** or **OCCUPIED** based on a configurable distance threshold.

The number of available parking slots is calculated in real time and displayed on a 16×2 I2C LCD. A green LED indicates that parking space is available, while a red LED and buzzer indicate that all parking slots are occupied.

The complete system was designed, implemented, and tested virtually using **Wokwi**, providing a practical demonstration of sensor interfacing, embedded C/C++ programming, GPIO control, LCD interfacing, real-time decision making, automation, debugging, and virtual hardware simulation.

## 🎯 Objectives

- Automatically detect parking-slot occupancy.
- Monitor four parking slots simultaneously.
- Measure distance using HC-SR04 ultrasonic sensors.
- Classify parking slots as FREE or OCCUPIED.
- Calculate the total number of available slots.
- Display parking availability on an I2C LCD.
- Provide visual parking-status indication.
- Generate an audio alert when parking is full.
- Demonstrate embedded-system design using Arduino UNO.
- Validate the complete system using Wokwi virtual simulation.

## ✨ Key Features

- Four-slot automated parking detection
- Four HC-SR04 ultrasonic sensors
- Real-time distance measurement
- Configurable 15 cm occupancy threshold
- Automatic FREE/OCCUPIED classification
- Available-slot counting
- 16×2 I2C LCD display
- Green LED availability indication
- Red LED parking-full indication
- Buzzer-based full-capacity alert
- Serial Monitor debugging
- Wokwi virtual hardware simulation
- Modular and expandable embedded architecture

## 🏗️ System Architecture


                    SMART PARKING SYSTEM
                            │
                            ▼
                  ┌───────────────────┐
                  │    Arduino UNO    │
                  │  Central Control  │
                  └─────────┬─────────┘
                            │
        ┌───────────────────┼───────────────────┐
        │                   │                   │
        ▼                   ▼                   ▼
   HC-SR04 Sensors      Distance Processing   Output Control
        │                   │                   │
        │                   ▼             ┌─────┼─────┐
        │              FREE/OCCUPIED      │     │     │
        │                   │             ▼     ▼     ▼
        ▼                   ▼            LCD  LEDs  Buzzer
   Slot Detection     Available Slots
```

## 🔄 Working Principle

Each HC-SR04 ultrasonic sensor monitors one parking slot.

```text
Ultrasonic Sensor
       ↓
Transmit Ultrasonic Pulse
       ↓
Receive Reflected Echo
       ↓
Measure Echo Duration
       ↓
Calculate Distance
       ↓
Compare With Threshold
       ↓
FREE / OCCUPIED
       ↓
Count Available Slots
       ↓
Update LCD + LED + Buzzer
```

### Parking Detection Logic

```text
Distance < 20cm
       ↓
   OCCUPIED

Distance >= 20
       ↓
      FREE
```

The 20cm value it the figure threshold used in the prototype and can be calibirated for a parking Installation

## 📐 Distance Calculation

The system calculates distance using the ultrasonic echo time:

```text
Distance = (Echo Time × Speed of Sound) / 2
```

The implementation uses approximately:

```text
Speed of Sound = 0.0343 cm/µs
```

The division by two is required because the ultrasonic pulse travels from the sensor to the object and back to the sensor.

## 🔌 Hardware Components

| Component                 |       Quantity    |            Purpose            |
|--                        -|---               :|--                            -|
| Arduino UNO               |        1          | Main microcontroller          |
| HC-SR04 Ultrasonic Sensor |        4          | Parking-slot detection        |
| 16×2 I2C LCD              |        1          | Available-slot display        |
| Green LED                 |        1          | Parking available indication  |
| Red LED                   |        1          | Parking full indication       |
| Buzzer                    |        1          | Parking-full alert            |
| 220Ω Resistor             |        2          | LED current limiting          |
| Breadboard                |        1          | Circuit prototyping           |
| Jumper Wires              |    As required    | Circuit connections           |

## 💻 Software & Tools

- **Programming Language:** Embedded C/C++
- **Microcontroller:** Arduino UNO
- **Simulation:** Wokwi
- **Development Environment:** Visual Studio Code
- **Build Environment:** PlatformIO
- **Display Library:** LiquidCrystal_I2C
- **Version Control:** Git
- **Repository:** GitHub

## 📍 Pin Configuration

| Component      | Arduino UNO Pin |
|--             -|---             :|
| Slot 1 TRIG    | D2              |
| Slot 1 ECHO    | D3              |
| Slot 2 TRIG    | D4              |
| Slot 2 ECHO    | D5              |
| Slot 3 TRIG    | D6              |
| Slot 3 ECHO    | D7              |
| Slot 4 TRIG    | D8              |
| Slot 4 ECHO    | D9              |
| Green LED      | D10             |
| Red LED        | D11             |
| Buzzer         | D12             |
| LCD SDA        | A4              |
| LCD SCL        | A5              |
| VCC            | 5V              |
| GND            | GND             |

## 🖥️ LCD Output

### Parking Available

```text

┌────────────────┐
│ Available: 4   │
│ Slots Available│
└────────────────┘
```

### Parking Full

```text
┌────────────────┐
│ Available: 0   │
│ PARKING FULL   │
└────────────────┘
```

## 🚦 Status Indication

| Parking Condition | Green LED | Red LED | Buzzer |
|---|---|---|---|
| At least one slot free | ON | OFF | OFF |
| All slots occupied | OFF | ON | ON |

## 🧪 Testing

The system was tested using different virtual sensor-distance conditions in Wokwi.

| Test Case      | Slot 1  | Slot 2 | Slot 3 | Slot 4   | Available Slots | Result |
|---             |---     :|---    :|---    :|---      :|--              -|        |
| All slots free | 30 cm   | 30 cm  | 30 cm  | 30 cm    | 4               | PASS   |
| One occupied   | 10 cm   | 30 cm  | 30 cm  | 30 cm    | 3               | PASS   |
| Two occupied   | 10 cm   | 10 cm  | 30 cm  | 30 cm    | 2               | PASS   |
| Three occupied | 10 cm   | 10 cm  | 10 cm  | 30 cm    | 1               | PASS   |
| All occupied   | 10 cm   | 10 cm  | 10 cm  | 10 cm    | 0               | PASS   |
| Vehicle leaves | 10 cm   | 10 cm  | 30 cm  | 10 cm    | 1               | PASS   |

## 📊 Sample Serial Monitor Output

### All Slots Free

```text
Parking Slot Status
-------------------
Slot 1: 29.05 cm - FREE
Slot 2: 29.05 cm - FREE
Slot 3: 29.05 cm - FREE
Slot 4: 29.05 cm - FREE
-------------------
Available Slots: 4
```

### Parking Full

```text
Parking Slot Status
-------------------
Slot 1: 12.01 cm - OCCUPIED
Slot 2: 12.01 cm - OCCUPIED
Slot 3: 12.13 cm - OCCUPIED
Slot 4: 9.09 cm - OCCUPIED
-------------------
Available Slots: 0
```
## 📸 Project Screenshots

### 🔧 Complete Wokwi Circuit

![Complete Wokwi Circuit](screenshots/7.Complete_wokwi_circuit.png)

### 🟢 All Parking Slots Free

![All Slots Free](screenshots/2.all_slots_free.png)

### 🚗 One Parking Slot Occupied

![One Slot Occupied](screenshots/3.one_slot_occupied.png)

### 🚗🚗 Two Parking Slots Occupied

![Two Slots Occupied](screenshots/4.two_slots_occupied.png)

### 🚗🚗🚗 Three Parking Slots Occupied

![Three Slots Occupied](screenshots/5.three_slots_occupied.png)

### 🚨 Parking Full

![Parking Full](screenshots/6.all_slots_occupied.png)

### 📁 Project Structure

![Project Structure](screenshots/1.project_structure.png)

```

## 🧩 Project Structure

```text
Smart-Parking-Ultrasonic-Embedded-System/
│
├── arduino_code/
│   └── smart_parking.ino
│
├── circuit_diagram/
│   └── circuit_diagram.png
│
├── data/
│   └── sample_sensor_data.csv
│
├── docs/
│   ├── algorithm.md
│   ├── architecture.md
│   ├── hardware.md
│   ├── simulation.md
│   └── testing.md
│
├── outputs/
│   ├── final_result.txt
│   ├── parking_status.txt
│   ├── serial_output.txt
│   └── test_results.csv
│
├── reports/
│   └── Smart_Parking_Project_Report.pdf
│
├── screenshots/
│   ├── all_slots_free.png
│   ├── one_slot_occupied.png
│   ├── two_slots_occupied.png
│   ├── three_slots_occupied.png
│   ├── parking_full.png
│   ├── project_structure.png
│   └── wokwi_complete_circuit.png
│
├── simulation/
│   └── Wokwi simulation files
│
├── src/
│   └── main.cpp
│
├── test_cases/
│   └── test_cases.csv
│
├── .gitignore
├── README.md
└── platformio.ini
```

## 🛠️ Implementation Workflow

```text
Project Planning
      ↓
Circuit Design
      ↓
Single Ultrasonic Sensor Testing
      ↓
FREE/OCCUPIED Detection
      ↓
Four-Slot Integration
      ↓
Available Slot Counter
      ↓
LCD Integration
      ↓
LED Indication
      ↓
Buzzer Alert
      ↓
Wokwi Simulation
      ↓
Testing & Validation
      ↓
Documentation
      ↓
GitHub Deployment
```

## ▶️ How to Run the Project

### Wokwi Simulation

1. Open the Wokwi project.
2. Load the Arduino UNO circuit.
3. Open the Arduino source code.
4. Start the simulation.
5. Select an HC-SR04 sensor.
6. Change its simulated distance.
7. Observe the Serial Monitor.
8. Observe LCD, LED, and buzzer behavior.

### VS Code + PlatformIO

1. Clone the repository.
2. Open the project in VS Code.
3. Install the PlatformIO extension.
4. Open the project folder.
5. Build the project.
6. Connect an Arduino UNO if physical hardware is available.
7. Select the correct COM port.
8. Upload the program.
9. Open Serial Monitor at 9600 baud.

## 📈 Results

The implemented prototype successfully:

- Detected four parking slots.
- Measured ultrasonic distance.
- Classified slots as FREE or OCCUPIED.
- Calculated available parking spaces.
- Displayed real-time availability on the LCD.
- Controlled green and red status LEDs.
- Activated the buzzer when parking was full.
- Handled simulated vehicles entering and leaving parking slots.
- Passed the defined virtual test cases.

## 🌐 Applications

- Shopping mall parking
- Hospital parking
- Airport parking
- University and campus parking
- Office parking
- Residential parking
- Railway station parking
- Smart-city parking infrastructure

## ✅ Advantages

- Low-cost prototype
- Simple hardware architecture
- Real-time parking monitoring
- Automated slot detection
- Easy-to-understand embedded logic
- Reduced manual monitoring
- Easy expansion to additional parking slots
- Suitable for IoT integration
- Fully testable using virtual simulation

## ⚠️ Limitations

- Current prototype supports four parking slots.
- Ultrasonic readings depend on sensor placement.
- Physical deployment requires threshold calibration.
- The current prototype does not include cloud connectivity.
- No mobile application is implemented.
- No automatic barrier gate is implemented.
- No RFID or license-plate recognition is implemented.
- The current LED design provides overall parking status rather than individual LED pairs for every slot.

## 🚀 Future Scope

The project can be extended with:

- ESP32-based Wi-Fi connectivity
- IoT cloud dashboard
- Mobile application
- Real-time web dashboard
- Individual slot LED indicators
- Automatic servo-controlled parking barrier
- RFID-based vehicle identification
- Online parking reservation
- Digital payment integration
- License-plate recognition
- Cloud-based parking analytics
- Smart-city parking integration
- Database-backed parking history

## 🎓 Skills Demonstrated

### Embedded Systems
- Microcontroller programming
- GPIO interfacing
- Sensor interfacing
- Real-time control
- Hardware-software integration

### Electronics
- Ultrasonic sensing
- Digital input/output
- LED interfacing
- Buzzer interfacing
- I2C communication

### Programming
- C/C++
- Conditional logic
- Arrays
- Functions
- Sensor data processing
- Serial communication

### Tools
- Visual Studio Code
- PlatformIO
- Wokwi
- Git
- GitHub

## 💼 Industry Relevance

This project demonstrates the ability to develop a complete embedded prototype from requirements to implementation and testing. It combines sensor interfacing, microcontroller programming, real-time decision making, hardware control, simulation, debugging, documentation, and version control.

The architecture can serve as a foundation for larger IoT and smart-city parking solutions.

## 📚 Documentation

Detailed technical documentation is available in:

```text
docs/
├── algorithm.md
├── architecture.md
├── hardware.md
├── simulation.md
└── testing.md
```

Project evidence is available in:

```text
screenshots/ 
outputs/
test_cases/
reports/
```

## 🔬 Project Status

**Completed and Tested**

The four-slot Smart Parking System has been implemented and successfully validated using Wokwi virtual simulation.

## 👨‍🎓 Author

**Adarsh Srivastav**

Computer Science and Engineering (CSE) Student

**Areas of Interest:**  
Embedded Systems | IoT | AI | Python

## ⭐ Conclusion

The Smart Parking System successfully demonstrates how ultrasonic sensing and embedded control can be combined to automate parking-slot monitoring. The project provides real-time occupancy detection, available-slot counting, LCD-based information display, visual indication, and parking-full alerts.

The virtual implementation and testing establish a functional proof of concept that can be further developed into an ESP32-based IoT smart parking platform with cloud connectivity, mobile monitoring, automated access control, and intelligent parking analytics.
