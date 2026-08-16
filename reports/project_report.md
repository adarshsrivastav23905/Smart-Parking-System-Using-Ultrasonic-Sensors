# Smart Parking System Using Ultrasonic Sensors
## Project Report

**Project Type:** Embedded Systems and IoT Prototype  
**Controller:** ESP32 DevKit  
**Development Environment:** Visual Studio Code + PlatformIO  
**Simulation Platform:** Wokwi  
**Programming Language:** Embedded C/C++  
**Project Status:** Functional Virtual Prototype  
**Physical Hardware Validation:** Not performed  

---

# 1. Abstract

The **Smart Parking System Using Ultrasonic Sensors** is an ESP32-based embedded and IoT prototype developed to monitor four parking spaces in real time.

Four HC-SR04 ultrasonic sensors are assigned to four parking slots. The ESP32 measures the distance detected by each sensor and classifies the corresponding slot as **FREE** or **OCCUPIED** using a configurable 35 cm threshold.

The system calculates the number of available parking spaces and presents the result using several interfaces. An SSD1306 OLED provides local status information, green and red LEDs indicate individual slot states, a buzzer provides a parking-full alert, and an SG90 servo demonstrates capacity-based gate control. The ESP32 also hosts a local HTTP dashboard that provides browser-based parking information.

The complete prototype was designed, implemented, simulated, and functionally validated using Wokwi and PlatformIO. The project demonstrates sensor interfacing, GPIO programming, timing, state management, I²C communication, PWM control, Wi-Fi networking, HTTP serving, debugging, validation, and professional project documentation.

The project is intended as a proof of concept for future physical deployment and larger smart-city parking systems.

---

# 2. Introduction

Parking management is a common real-world automation problem. In many parking environments, drivers need to determine which spaces are available before attempting to park. Manual monitoring can be inefficient and does not provide continuously updated information.

Embedded systems provide a practical solution by combining sensors, processing, local displays, actuators, and network connectivity.

This project implements a four-slot smart parking prototype using an ESP32 as the central controller. Each parking slot is monitored by an ultrasonic sensor. The measured distance is converted into a parking state and used to update all system outputs.

The project was developed virtually because physical hardware components were not available during development. Wokwi was used to reproduce the embedded circuit, while PlatformIO was used for firmware development and build management.

---

# 3. Problem Statement

A conventional parking area may not provide drivers or operators with real-time knowledge of available spaces.

Common limitations include:

- Manual parking inspection
- Limited visibility of available slots
- Lack of centralized status information
- No automatic full-capacity indication
- No real-time browser monitoring

The project addresses these limitations through automated parking-space sensing and centralized status processing.

---

# 4. Aim of the Project

The aim of the project is to design and validate an embedded smart parking prototype that:

1. Detects parking-slot occupancy automatically.
2. Calculates available parking capacity.
3. Provides clear local indications.
4. Demonstrates automatic gate control based on capacity.
5. Provides real-time browser-based monitoring.

---

# 5. Objectives

The main objectives are:

- Monitor four individual parking slots.
- Use HC-SR04 sensors to measure distance.
- Classify each slot as FREE or OCCUPIED.
- Calculate total available spaces.
- Display parking status on an OLED.
- Provide green/red LED slot indication.
- Activate a buzzer when all slots are occupied.
- Demonstrate a capacity-based servo gate.
- Provide a local ESP32 web dashboard.
- Validate the system using Wokwi.
- Organize the project for GitHub publication.
- Document testing and technical implementation.

---

# 6. Scope of the Project

## 6.1 Included Scope

The implemented prototype includes:

- ESP32 DevKit
- Four HC-SR04 ultrasonic sensors
- SSD1306 OLED
- Four green LEDs
- Four red LEDs
- Eight 220 Ω LED resistors
- Buzzer
- SG90 servo
- Wi-Fi connectivity
- HTTP web dashboard
- Serial monitoring
- Wokwi simulation
- PlatformIO build system

## 6.2 Excluded Scope

The current version does not include:

- Physical hardware testing
- Cloud database
- Mobile application
- RFID
- License-plate recognition
- Parking reservation
- Online payment
- Production security
- Dedicated entrance vehicle sensor
- Dedicated exit vehicle sensor
- Real-world mechanical barrier validation

---

# 7. System Requirements

## 7.1 Functional Requirements

### Sensor Monitoring

The system shall read all four ultrasonic sensors.

### Slot Classification

The system shall use:

```text
Distance < 35 cm  → OCCUPIED
Distance >= 35 cm → FREE
```

### Availability Counting

The system shall calculate:

```text
Available Slots = 4 - Occupied Slots
```

### Display

The system shall display current slot and availability status on the OLED.

### LED Indication

```text
FREE      → Green LED ON
OCCUPIED  → Red LED ON
```

### Parking-Full Alert

When:

```text
Available Slots = 0
```

the buzzer shall alert the user.

### Servo Control

The gate shall operate according to capacity:

```text
Available > 0 → OPEN
Available = 0 → CLOSED
```

### Web Dashboard

The ESP32 shall provide a browser-accessible parking dashboard.

---

# 8. Hardware Design

## 8.1 ESP32 DevKit

The ESP32 acts as the central controller.

Responsibilities include:

- Sensor control
- Distance processing
- Slot-state classification
- Counting
- OLED control
- LED control
- Buzzer control
- Servo control
- Wi-Fi connectivity
- HTTP server operation

## 8.2 HC-SR04 Sensors

Four ultrasonic sensors are used:

```text
Sensor 1 → Slot 1
Sensor 2 → Slot 2
Sensor 3 → Slot 3
Sensor 4 → Slot 4
```

Each sensor provides:

- TRIG
- ECHO
- VCC
- GND

## 8.3 OLED

The SSD1306 OLED is used for local display.

```text
SDA → GPIO 21
SCL → GPIO 22
```

The configured I²C address is:

```text
0x3C
```

## 8.4 LEDs

Each slot has:

```text
1 Green LED
1 Red LED
```

Green represents FREE and red represents OCCUPIED.

## 8.5 Buzzer

The buzzer is connected to GPIO 19 and provides the parking-full alert.

## 8.6 Servo

The SG90 servo is connected to GPIO 18.

It represents the parking-capacity gate.

---

# 9. Pin Configuration

| Component | Signal | ESP32 GPIO |
|---|---|---:|
| Slot 1 HC-SR04 | TRIG | 5 |
| Slot 1 HC-SR04 | ECHO | 17 |
| Slot 2 HC-SR04 | TRIG | 16 |
| Slot 2 HC-SR04 | ECHO | 4 |
| Slot 3 HC-SR04 | TRIG | 27 |
| Slot 3 HC-SR04 | ECHO | 26 |
| Slot 4 HC-SR04 | TRIG | 25 |
| Slot 4 HC-SR04 | ECHO | 35 |
| OLED | SDA | 21 |
| OLED | SCL | 22 |
| Servo | PWM | 18 |
| Buzzer | Signal | 19 |
| Slot 1 Green LED | GPIO | 12 |
| Slot 1 Red LED | GPIO | 2 |
| Slot 2 Green LED | GPIO | 14 |
| Slot 2 Red LED | GPIO | 13 |
| Slot 3 Green LED | GPIO | 32 |
| Slot 3 Red LED | GPIO | 23 |
| Slot 4 Green LED | GPIO | 15 |
| Slot 4 Red LED | GPIO | 33 |

---

# 10. Software Architecture

The firmware is organized around the following logical functions:

```text
Sensor Acquisition
        ↓
Distance Calculation
        ↓
Slot State Management
        ↓
Availability Calculation
        ↓
Output Control
        ↓
Web Dashboard
```

The main implementation is located in:

```text
src/main.cpp
```

The project uses the Arduino framework under PlatformIO.

---

# 11. Distance Measurement

The ultrasonic sensors measure distance using echo timing.

The implemented relationship is:

```text
Distance = (Echo Time × Speed of Sound) / 2
```

The approximate speed of sound is:

```text
0.0343 cm/µs
```

The division by two is required because the ultrasonic pulse travels from the sensor to the object and back.

---

# 12. Occupancy Algorithm

The project uses a 35 cm detection threshold.

```text
Measured Distance
        ↓
Is Distance < 35 cm?
       /          \
     YES           NO
      │             │
      ▼             ▼
 OCCUPIED          FREE
```

Examples:

| Distance | State |
|---:|---|
| 10 cm | OCCUPIED |
| 20 cm | OCCUPIED |
| 30 cm | OCCUPIED |
| 35 cm | FREE |
| 50 cm | FREE |
| 80 cm | FREE |

---

# 13. State Management

Ultrasonic measurements can vary slightly from one reading to another.

The firmware therefore uses state confirmation logic so that a transient reading does not unnecessarily change the displayed parking status.

Conceptually:

```text
New Reading
     ↓
Candidate State
     ↓
Confirmation
     ↓
Confirmed State
```

This improves status stability.

---

# 14. Availability Calculation

For four slots:

```text
Available Slots = 4 - Occupied Slots
```

Example:

```text
S1 = OCCUPIED
S2 = FREE
S3 = OCCUPIED
S4 = FREE
```

Therefore:

```text
Occupied = 2
Available = 2
```

---

# 15. Output Processing

## OLED

Displays:

- Slot distance
- Slot status
- Available count
- Threshold
- Gate state

Example:

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

## LEDs

```text
FREE      → Green ON, Red OFF
OCCUPIED  → Green OFF, Red ON
```

## Buzzer

```text
Available = 0
        ↓
Buzzer Alert
```

## Servo

```text
Available > 0
        ↓
Gate OPEN
```

```text
Available = 0
        ↓
Gate CLOSED
```

---

# 16. Web Dashboard

The ESP32 hosts an HTTP dashboard.

In the Wokwi environment the dashboard is exposed through:

```text
http://localhost:8180
```

The dashboard provides:

- Available slots
- Occupied slots
- Individual slot state
- Distance values
- Detection threshold
- System status
- Gate status

The dashboard is intended to provide a simple browser-based view of the same state used by the embedded outputs.

---

# 17. System Data Flow

```text
HC-SR04 Sensors
       ↓
Distance Measurement
       ↓
ESP32 Processing
       ↓
FREE / OCCUPIED
       ↓
Available Count
       │
       ├─────────────┐
       │             │
       ▼             ▼
     Local         Web
    Outputs      Dashboard
       │
 ┌─────┼─────┐
 ▼     ▼     ▼
OLED  LEDs  Buzzer
       │
       ▼
     Servo
```

---

# 18. Wokwi Simulation

The complete prototype is simulated in Wokwi.

The simulation contains:

```text
ESP32
HC-SR04 × 4
SSD1306 OLED
Green LEDs × 4
Red LEDs × 4
Resistors × 8
Buzzer
SG90 Servo
```

The virtual sensor distance can be changed to simulate different parking conditions.

Example:

```text
80 / 80 / 80 / 80 → 4/4 FREE
20 / 80 / 80 / 80 → 3/4 FREE
20 / 20 / 80 / 80 → 2/4 FREE
20 / 20 / 20 / 80 → 1/4 FREE
20 / 20 / 20 / 20 → 0/4 PARKING FULL
```

---

# 19. Testing Methodology

Testing is performed as a virtual functional validation process.

Testing includes:

- Slot detection
- Threshold verification
- Availability count
- OLED verification
- LED verification
- Buzzer verification
- Servo verification
- Web dashboard verification
- Reset behavior
- Invalid reading handling

The detailed test procedures are stored in:

```text
test_cases/README.md
```

---

# 20. Main Functional Test Cases

| Test ID | Scenario | Input | Expected |
|---|---|---|---|
| TC01 | All slots free | 80/80/80/80 | 4/4 available |
| TC02 | One occupied | 20/80/80/80 | 3/4 available |
| TC03 | Two occupied | 20/20/80/80 | 2/4 available |
| TC04 | Three occupied | 20/20/20/80 | 1/4 available |
| TC05 | Parking full | 20/20/20/20 | 0/4 + buzzer + closed gate |
| TC06 | Slot released | 20/20/20/80 | 1/4 + reopened gate |
| TC07 | OLED | 20/80/20/80 | Correct OLED |
| TC08 | Web dashboard | 20/80/20/80 | Correct dashboard |
| TC09 | Green LED | 80 cm | Green ON |
| TC10 | Red LED | 20 cm | Red ON |
| TC11 | Buzzer | 20/20/20/20 | Buzzer alert |
| TC12 | Servo | Full/available | Correct gate state |
| TC13 | Reset | Restart | Safe initialization |

---

# 21. Validation Criteria

The virtual prototype is considered functionally validated when:

- Four sensors produce valid readings.
- Threshold classification behaves correctly.
- Availability calculation matches slot states.
- OLED matches current state.
- LEDs match individual slot states.
- Buzzer activates at full capacity.
- Servo closes when full.
- Servo reopens when a slot is available.
- Web dashboard matches system state.
- PlatformIO build succeeds.
- Evidence is captured for major test scenarios.

---

# 22. Sample Demonstration Sequence

The recommended demonstration sequence is:

```text
80 / 80 / 80 / 80
        ↓
20 / 80 / 80 / 80
        ↓
20 / 20 / 80 / 80
        ↓
20 / 20 / 20 / 80
        ↓
20 / 20 / 20 / 20
        ↓
20 / 20 / 20 / 80
```

Available-space sequence:

```text
4 → 3 → 2 → 1 → 0 → 1
```

Gate sequence:

```text
OPEN
OPEN
OPEN
OPEN
CLOSED
OPEN
```

Buzzer sequence:

```text
OFF
OFF
OFF
OFF
ON
OFF
```

This is a useful demonstration because one sensor change propagates through the complete system.

---

# 23. Development Challenges

## Serial Monitor

The project required debugging of the Wokwi virtual serial-monitor path.

The resolution involved verifying serial initialization, matching baud rate, checking the Wokwi connection, and avoiding duplicate monitor sessions.

## OLED Visibility

The OLED was repositioned within the virtual circuit so that its display remains visible and is not obscured by wires.

## Fourth Sensor

Sequential ultrasonic measurement was used to improve stability and reduce potential ultrasonic interference.

## Servo Gate

The servo required verification of capacity-based transitions.

The final intended behavior is:

```text
Available > 0 → OPEN
Available = 0 → CLOSED
```

## Web Dashboard

The dashboard was integrated into the same parking-state data used by the embedded outputs.

## Repository Organization

Documentation was divided into dedicated areas for architecture, algorithm, hardware, simulation, testing, outputs, evidence, and reports.

---

# 24. Limitations

The current system has several limitations:

1. It supports four parking slots.
2. Validation is virtual.
3. Physical hardware testing has not been performed.
4. Ultrasonic threshold calibration for real-world deployment remains to be completed.
5. The servo is capacity-based rather than vehicle-triggered.
6. No cloud connectivity is implemented.
7. No mobile application is implemented.
8. No RFID or license-plate recognition is implemented.
9. No reservation or payment system is implemented.
10. No production-grade authentication is implemented.

---

# 25. Physical Deployment Considerations

Before moving to real hardware:

### Electrical

- Verify GPIO voltage levels.
- Verify HC-SR04 ECHO compatibility.
- Use level shifting where required.
- Provide suitable servo power.
- Maintain common ground.
- Use LED current-limiting resistors.

### Mechanical

- Mount sensors consistently.
- Calibrate the vehicle detection distance.
- Verify the servo barrier mechanically.
- Provide safe gate movement.

### Environmental

Physical readings may be affected by:

- Vehicle geometry
- Sensor angle
- Reflections
- Temperature
- Sensor interference
- Mounting position

---

# 26. Security Considerations

The current web dashboard is intended for local demonstration.

A production version should implement:

- Dashboard authentication
- Secure Wi-Fi credentials
- HTTPS
- Authorization
- Secure firmware updates
- Input validation
- Network protection
- Safe actuator control

---

# 27. Future Scope

Possible future improvements include:

### Hardware

- Dedicated entry and exit sensors
- RFID reader
- IR vehicle detector
- Safety barrier sensors
- Additional parking slots
- Camera integration

### Software

- MQTT
- Cloud database
- REST API
- Mobile application
- Historical analytics
- Notifications

### Smart Parking Services

- Reservation
- Online payment
- User accounts
- Multi-floor parking
- Vehicle identification
- Occupancy analytics

---

# 28. Repository Organization

The recommended project repository is:

```text
Smart-Parking-Ultrasonic-Sensor-System/
│
├── .gitignore
├── README.md
├── PROJECT.md
├── platformio.ini
├── diagram.json
├── wokwi.toml
│
├── src/
│   └── main.cpp
│
├── docs/
│   ├── algorithm.md
│   ├── architecture.md
│   ├── hardware.md
│   ├── pin_mapping.md
│   ├── simulation.md
│   ├── testing.md
│   └── troubleshooting.md
│
├── circuit_diagram/
│   └── README.md
│
├── simulation/
│   └── README.md
│
├── test_cases/
│   ├── README.md
│   └── test_cases.csv
│
├── data/
│   └── sample_sensor_data.csv
│
├── outputs/
│   ├── final_result.txt
│   ├── parking_status.txt
│   ├── serial_output.txt
│   └── test_results.csv
│
├── reports/
│   └── project_report.md
│
└── screenshots/
    ├── README.md
    └── validation evidence
```

---

# 29. File Responsibilities

| File | Purpose |
|---|---|
| `README.md` | Main GitHub overview |
| `PROJECT.md` | Detailed project definition |
| `src/main.cpp` | ESP32 firmware |
| `platformio.ini` | Build configuration |
| `diagram.json` | Wokwi circuit |
| `wokwi.toml` | Wokwi configuration |
| `docs/algorithm.md` | Algorithm documentation |
| `docs/architecture.md` | System architecture |
| `docs/hardware.md` | Hardware documentation |
| `docs/pin_mapping.md` | GPIO mapping |
| `docs/simulation.md` | Simulation guide |
| `docs/testing.md` | Testing strategy |
| `docs/troubleshooting.md` | Troubleshooting |
| `test_cases/README.md` | Detailed tests |
| `test_cases/test_cases.csv` | Structured tests |
| `data/sample_sensor_data.csv` | Sample data |
| `outputs/final_result.txt` | Final result summary |
| `outputs/parking_status.txt` | Parking-state reference |
| `outputs/serial_output.txt` | Serial evidence |
| `outputs/test_results.csv` | Test result records |
| `reports/project_report.md` | Formal report |
| `screenshots/README.md` | Screenshot index |

---

# 30. Industry Relevance

This project demonstrates an end-to-end embedded engineering workflow:

```text
Requirements
      ↓
System Design
      ↓
Hardware Mapping
      ↓
Firmware Development
      ↓
Sensor Processing
      ↓
Decision Logic
      ↓
Local Interface
      ↓
Actuation
      ↓
IoT Dashboard
      ↓
Testing
      ↓
Documentation
      ↓
Version Control
```

The architecture is relevant to:

- Smart-city systems
- Parking automation
- Access control
- Embedded monitoring
- IoT automation
- Industrial sensing

---

# 31. Skills Demonstrated

## Embedded Systems

- ESP32 programming
- GPIO control
- Sensor interfacing
- Timing
- State management
- PWM

## Electronics

- Ultrasonic sensing
- I²C
- OLED
- LED interfacing
- Buzzer
- Servo

## IoT

- Wi-Fi
- HTTP
- Embedded web server

## Software Engineering

- C/C++
- PlatformIO
- Wokwi
- Git
- GitHub
- Testing
- Documentation
- Debugging

---

# 32. Project Status

```text
┌────────────────────────────────────────────┐
│            PROJECT STATUS                  │
├────────────────────────────────────────────┤
│ Four-slot sensing          ✅               │
│ Occupancy detection        ✅               │
│ OLED display               ✅               │
│ LED indication             ✅               │
│ Buzzer alert               ✅               │
│ Servo gate                 ✅               │
│ Web dashboard              ✅               │
│ Wokwi simulation           ✅               │
│ PlatformIO build           ✅               │
│ Documentation              ✅               │
│ Testing framework          ✅               │
│ Physical testing           ⏳ Not performed │
└────────────────────────────────────────────┘
```

---

# 33. Final Project Statement

> The Smart Parking System Using Ultrasonic Sensors is a functional ESP32-based embedded and IoT virtual prototype that integrates ultrasonic sensing, real-time parking-state processing, local visual and audio feedback, capacity-based servo gate control, and browser-based parking monitoring, validated using Wokwi and PlatformIO.

---

# 34. Final Verification Checklist

Before final publication:

```text
[ ] README.md reviewed
[ ] PROJECT.md reviewed
[ ] Main firmware builds
[ ] Wokwi simulation starts
[ ] Four sensors tested
[ ] OLED tested
[ ] LEDs tested
[ ] Buzzer tested
[ ] Servo tested
[ ] Dashboard tested
[ ] Serial output captured
[ ] Test results updated
[ ] Screenshots reviewed
[ ] Duplicate configurations removed
[ ] Obsolete files removed
[ ] No passwords or secrets committed
[ ] .pio/ and .vscode/ ignored
[ ] Git status reviewed
[ ] GitHub push successful
```

---

# 35. Conclusion

The Smart Parking System successfully demonstrates how ultrasonic sensing, embedded processing, visual indication, audible alerts, servo actuation, and IoT connectivity can be combined into a practical four-slot smart parking prototype.

The final system follows:

```text
SENSING
   ↓
PROCESSING
   ↓
DECISION
   ↓
LOCAL FEEDBACK
   ↓
ACTUATION
   ↓
WEB MONITORING
```

The project provides a strong foundation for future physical implementation and can be extended into a larger smart parking platform with entry/exit sensing, cloud connectivity, mobile applications, reservations, payments, and advanced vehicle identification.

---

# 36. Related Documentation

```text
docs/algorithm.md
docs/architecture.md
docs/hardware.md
docs/pin_mapping.md
docs/simulation.md
docs/testing.md
docs/troubleshooting.md

circuit_diagram/README.md
simulation/README.md
test_cases/README.md
```

---

# 37. Final Technical Summary

```text
Project Title     : Smart Parking System Using Ultrasonic Sensors
Project Type      : Embedded Systems + IoT Prototype
Controller        : ESP32 DevKit
Parking Slots     : 4
Sensors           : HC-SR04 × 4
Detection         : Distance-based
Threshold         : 35 cm
Display           : SSD1306 OLED 128×64
Green LEDs        : 4
Red LEDs          : 4
Buzzer            : 1
Servo             : SG90
Connectivity      : ESP32 Wi-Fi
Web Interface     : HTTP Dashboard
Simulation        : Wokwi
Build System      : PlatformIO
Language          : Embedded C/C++
Version Control   : Git
Repository        : GitHub
Validation        : Virtual Prototype
Physical Testing  : Not performed
Project Status    : FUNCTIONAL VIRTUAL PROTOTYPE
```
