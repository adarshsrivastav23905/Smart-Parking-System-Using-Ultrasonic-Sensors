# 🏗️ Smart Parking System — System Architecture

## 1. Architecture Overview

The **Smart Parking System Using Ultrasonic Sensors** is an ESP32-based embedded and IoT prototype designed to monitor four parking slots in real time.

The system combines:

- Four HC-SR04 ultrasonic sensors
- ESP32 DevKit
- SSD1306 OLED display
- Individual green/red LED indicators
- Parking-full buzzer
- SG90 servo gate
- ESP32 Wi-Fi connectivity
- HTTP web dashboard
- Wokwi virtual simulation

The ESP32 acts as the **central controller**. It acquires sensor data, processes parking states, controls the outputs, and provides parking information through the web dashboard.

---

# 2. High-Level Architecture

```text
                         SMART PARKING SYSTEM
                                  │
                                  ▼
                    ┌──────────────────────────┐
                    │       ESP32 DevKit       │
                    │                          │
                    │   Central Controller     │
                    │   Embedded Processing    │
                    └────────────┬─────────────┘
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
        ▼                        ▼                        ▼
 ┌───────────────┐       ┌───────────────┐       ┌───────────────┐
 │ Sensor Layer  │       │ Processing    │       │ Network Layer │
 │               │       │ Layer         │       │               │
 │ HC-SR04 × 4   │──────►│ Parking Logic │──────►│ Wi-Fi / HTTP  │
 └───────────────┘       └───────┬───────┘       └───────┬───────┘
                                 │                        │
                                 │                        ▼
                                 │                 Web Dashboard
                                 │
                    ┌────────────┼────────────┐
                    │            │            │
                    ▼            ▼            ▼
                 ┌──────┐    ┌───────┐    ┌────────┐
                 │ OLED │    │ LEDs  │    │ Buzzer │
                 └──────┘    └───────┘    └────────┘
                                               │
                                               ▼
                                            ┌───────┐
                                            │ Servo │
                                            │ Gate  │
                                            └───────┘
```

---

# 3. System Layers

The system can be divided into the following logical layers:

```text
┌───────────────────────────────────────────────┐
│              Presentation Layer               │
│        OLED Display + Web Dashboard           │
├───────────────────────────────────────────────┤
│                Output Layer                   │
│        LEDs + Buzzer + Servo Gate             │
├───────────────────────────────────────────────┤
│               Decision Layer                  │
│     FREE/OCCUPIED + Availability Logic        │
├───────────────────────────────────────────────┤
│              Processing Layer                 │
│  Distance Calculation + State Management      │
├───────────────────────────────────────────────┤
│                Sensor Layer                   │
│              HC-SR04 × 4                      │
├───────────────────────────────────────────────┤
│             Controller Layer                  │
│                 ESP32                         │
└───────────────────────────────────────────────┘
```

---

# 4. Controller Layer

## ESP32 DevKit

The ESP32 is the central processing and control unit.

### Responsibilities

The ESP32 performs the following operations:

1. Initializes GPIO pins.
2. Triggers ultrasonic sensors.
3. Measures echo duration.
4. Calculates distance.
5. Classifies parking slots.
6. Counts available slots.
7. Updates the OLED.
8. Controls slot LEDs.
9. Controls the buzzer.
10. Controls the servo gate.
11. Connects to Wi-Fi.
12. Hosts the HTTP web dashboard.
13. Provides serial debugging information.

### Why ESP32?

ESP32 was selected because it provides:

- Multiple GPIO pins
- Built-in Wi-Fi
- I²C support
- PWM capability
- Sufficient processing capability
- Arduino framework support
- PlatformIO support
- Easy IoT integration

---

# 5. Sensor Layer

The sensor layer consists of four HC-SR04 ultrasonic sensors.

Each sensor is assigned to one parking slot.

```text
HC-SR04 #1 → Slot 1
HC-SR04 #2 → Slot 2
HC-SR04 #3 → Slot 3
HC-SR04 #4 → Slot 4
```

Each sensor contains:

```text
VCC
GND
TRIG
ECHO
```

The ESP32 generates a trigger pulse and measures the echo duration to determine the distance to an object.

---

# 6. Sensor Pin Mapping

| Parking Slot | TRIG | ECHO |
|---|---:|---:|
| Slot 1 | GPIO 5 | GPIO 17 |
| Slot 2 | GPIO 16 | GPIO 4 |
| Slot 3 | GPIO 27 | GPIO 26 |
| Slot 4 | GPIO 25 | GPIO 35 |

This deterministic mapping simplifies:

- Firmware development
- Circuit debugging
- Slot identification
- Testing
- Future expansion

---

# 7. Ultrasonic Measurement Flow

The distance measurement process is:

```text
ESP32
  │
  ▼
Generate TRIG pulse
  │
  ▼
HC-SR04 sends ultrasonic pulse
  │
  ▼
Pulse reflects from object
  │
  ▼
ECHO signal returns
  │
  ▼
ESP32 measures ECHO duration
  │
  ▼
Distance calculation
  │
  ▼
Distance in centimeters
```

---

# 8. Distance Calculation

The system calculates distance using:

```text
Distance = (Echo Time × Speed of Sound) / 2
```

The implementation uses approximately:

```text
Speed of Sound = 0.0343 cm/µs
```

The division by two is required because the ultrasonic pulse travels a round trip:

```text
Sensor → Object → Sensor
```

---

# 9. Parking Decision Layer

The measured distance is compared with the configured threshold.

Current threshold:

```text
35 cm
```

Decision logic:

```text
             Measured Distance
                     │
                     ▼
              Is distance < 35?
                 /          \
               YES           NO
                │             │
                ▼             ▼
            OCCUPIED        FREE
```

### Classification Table

| Distance | Slot State |
|---:|---|
| Less than 35 cm | OCCUPIED |
| 35 cm or greater | FREE |

### Example

| Distance | Result |
|---:|---|
| 10 cm | OCCUPIED |
| 20 cm | OCCUPIED |
| 30 cm | OCCUPIED |
| 35 cm | FREE |
| 50 cm | FREE |
| 80 cm | FREE |

The threshold can be modified in `src/main.cpp`.

---

# 10. Slot State Management

Each parking slot maintains its own state.

Conceptually:

```text
Slot
├── Distance
├── Occupied State
├── Pending State
├── Candidate State
└── Candidate Timestamp
```

The algorithm compares a newly measured state with the current confirmed state.

```text
New Measurement
      ↓
Compare with Current State
      ↓
Same State?
   /       \
 YES       NO
  │         │
  ▼         ▼
Keep     Start Confirmation
State        Timer
              │
              ▼
        Confirm Stability
              │
              ▼
        Update Slot State
```

---

# 11. State Confirmation

Ultrasonic readings can vary slightly because of:

- Reflection angle
- Object movement
- Sensor noise
- Simulation variation

A short confirmation period is used before accepting a state change.

### State transition

```text
Current State = FREE
        ↓
New Decision = OCCUPIED
        ↓
Start Confirmation
        ↓
State remains OCCUPIED
        ↓
Confirm transition
        ↓
Current State = OCCUPIED
```

The same process is used for:

```text
OCCUPIED → FREE
```

---

# 12. Availability Calculation

The system counts occupied slots and derives the number of available spaces.

```text
Available Slots =
Total Slots - Occupied Slots
```

For the current four-slot implementation:

```text
Available Slots = 4 - Occupied Slots
```

### Example

```text
Slot 1 → OCCUPIED
Slot 2 → FREE
Slot 3 → OCCUPIED
Slot 4 → FREE
```

Therefore:

```text
Occupied Slots = 2
Available Slots = 2
```

The system reports:

```text
Available: 2/4
Occupied: 2/4
```

---

# 13. Output Layer

The output layer provides parking information and control feedback.

```text
                  Parking State
                       │
       ┌───────────────┼────────────────┐
       │               │                │
       ▼               ▼                ▼
      OLED             LEDs           Buzzer
       │               │                │
       └───────────────┼────────────────┘
                       │
                       ▼
                   Servo Gate
```

---

# 14. OLED Subsystem

The SSD1306 OLED provides local visual feedback.

### I²C Connections

```text
OLED SDA → GPIO 21
OLED SCL → GPIO 22
```

### OLED Responsibilities

The OLED displays:

- Slot distance
- FREE/OCCUPIED state
- Available slots
- Detection threshold
- Gate status

Example:

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

Where:

```text
F  = FREE
O  = OCCUPIED
TH = Threshold
```

---

# 15. LED Subsystem

Each parking slot has two LEDs.

```text
Green LED → FREE
Red LED   → OCCUPIED
```

### LED Mapping

| Slot | Green LED | Red LED |
|---|---:|---:|
| Slot 1 | GPIO 12 | GPIO 2 |
| Slot 2 | GPIO 14 | GPIO 13 |
| Slot 3 | GPIO 32 | GPIO 23 |
| Slot 4 | GPIO 15 | GPIO 33 |

### Logic

```text
IF slot == FREE

Green LED = ON
Red LED   = OFF
```

```text
IF slot == OCCUPIED

Green LED = OFF
Red LED   = ON
```

---

# 16. Buzzer Subsystem

The buzzer is connected to:

```text
GPIO 19
```

The primary alert condition is parking-full status.

```text
Available Slots = 0
        │
        ▼
PARKING FULL
        │
        ▼
Buzzer Alert
```

The buzzer alert is controlled at an interval so that the system remains responsive.

---

# 17. Servo Gate Subsystem

The SG90 servo is connected to:

```text
Servo PWM → GPIO 18
```

The current prototype uses **capacity-based gate control**.

```text
Available Slots > 0
        │
        ▼
    GATE OPEN
```

```text
Available Slots = 0
        │
        ▼
   GATE CLOSED
```

### Important Scope

The final circuit does not contain a dedicated vehicle-entry sensor or push button.

Therefore, the servo demonstrates **parking-capacity access status** rather than vehicle-arrival detection.

A production implementation could add:

- Dedicated entry sensor
- Exit sensor
- RFID reader
- Vehicle detection loop
- License-plate recognition
- Safety barrier sensor

---

# 18. IoT / Network Layer

The ESP32 provides Wi-Fi connectivity and hosts an HTTP server.

```text
ESP32
  │
  ▼
Wi-Fi
  │
  ▼
HTTP Web Server
  │
  ▼
Browser
  │
  ▼
Parking Dashboard
```

The Wokwi simulation exposes the dashboard at:

```text
http://localhost:8180
```

---

# 19. Web Dashboard Architecture

The dashboard receives the same logical information used by the local interfaces.

```text
HC-SR04 Sensors
       │
       ▼
ESP32 Processing
       │
       ▼
Slot States
       │
       ▼
Available Count
       │
       ▼
HTTP Server
       │
       ▼
HTML Dashboard
       │
       ▼
Web Browser
```

### Dashboard Information

The web dashboard displays:

- Available slots
- Occupied slots
- Individual slot states
- Individual distance values
- Detection threshold
- System status
- Gate status

Example:

```text
SMART PARKING SYSTEM

Available Slots: 2/4
Occupied Slots: 2/4

Slot 1 → OCCUPIED → 20 cm
Slot 2 → FREE      → 80 cm
Slot 3 → OCCUPIED → 20 cm
Slot 4 → FREE      → 80 cm

Detection Threshold: 35 cm
System Status: SPACE AVAILABLE
Gate: OPEN
```

---

# 20. Complete Data Flow

The complete information flow through the system is:

```text
              HC-SR04 Sensors
                     │
                     ▼
              Echo Measurement
                     │
                     ▼
             Distance Calculation
                     │
                     ▼
              Threshold Comparison
                     │
              ┌──────┴──────┐
              │             │
              ▼             ▼
          OCCUPIED         FREE
              │             │
              └──────┬──────┘
                     ▼
                Slot States
                     │
                     ▼
               Occupied Count
                     │
                     ▼
               Available Count
                     │
         ┌───────────┼───────────┐
         │           │           │
         ▼           ▼           ▼
        OLED         LEDs        Web
         │           │           │
         └─────┬─────┘           │
               ▼                 │
             Buzzer              │
               │                 │
               ▼                 │
             Servo               │
                                 │
                                 ▼
                         Web Dashboard
```

---

# 21. Firmware Module Architecture

The firmware is divided into logical functions.

| Function | Responsibility |
|---|---|
| `readDistanceCM()` | Measures HC-SR04 distance |
| `updateSlotState()` | Classifies and confirms slot state |
| `updateLEDs()` | Controls green/red LEDs |
| `getOccupiedCount()` | Counts occupied slots |
| `getFreeCount()` | Calculates available slots |
| `updateOLED()` | Updates OLED |
| `printSerialStatus()` | Prints debugging information |
| `beepFullParking()` | Generates parking-full alert |
| `updateGate()` | Controls servo gate |
| `generateWebPage()` | Generates dashboard HTML |
| `handleRoot()` | Handles web root request |
| `connectWiFi()` | Establishes Wi-Fi connection |

This modular structure improves:

- Readability
- Maintainability
- Debugging
- Testing
- Future expansion

---

# 22. Software Execution Flow

The firmware follows this sequence:

```text
START
  │
  ▼
Initialize GPIO
  │
  ▼
Initialize HC-SR04 Sensors
  │
  ▼
Initialize LEDs
  │
  ▼
Initialize Buzzer
  │
  ▼
Initialize Servo
  │
  ▼
Initialize OLED
  │
  ▼
Connect Wi-Fi
  │
  ▼
Start Web Server
  │
  ▼
MAIN LOOP
  │
  ├── Read Slot 1
  ├── Read Slot 2
  ├── Read Slot 3
  ├── Read Slot 4
  │
  ▼
Calculate Distances
  │
  ▼
Classify Slot States
  │
  ▼
Confirm State Changes
  │
  ▼
Calculate Available Count
  │
  ├── Update OLED
  ├── Update LEDs
  ├── Update Buzzer
  ├── Update Servo
  └── Handle Web Requests
  │
  ▼
Repeat
```

---

# 23. Error Handling

The system handles invalid ultrasonic readings.

If the echo signal times out:

```text
Echo Timeout
     │
     ▼
Invalid Reading
     │
     ▼
Ignore Reading
     │
     ▼
Retain Current Slot State
```

This prevents a single invalid sensor reading from immediately changing the parking state.

---

# 24. Timing and Responsiveness

The firmware uses timing controls for:

- Ultrasonic echo timeout
- Slot-state confirmation
- OLED refresh
- Serial reporting
- Buzzer alerts

The four sensors are processed sequentially to reduce possible ultrasonic cross-talk.

The servo is commanded only when its required state changes, preventing unnecessary repeated commands.

---

# 25. Interface Consistency

The same logical parking state is reflected across all interfaces.

For example:

```text
Slot 2 becomes OCCUPIED
          │
          ├── OLED → S2 O
          │
          ├── Red LED → ON
          │
          ├── Green LED → OFF
          │
          └── Web Dashboard → OCCUPIED
```

This makes the system easier to understand, debug, and demonstrate.

---

# 26. Wokwi Simulation Architecture

The complete system is represented virtually in Wokwi.

```text
                    WOKWI SIMULATION
                           │
         ┌─────────────────┼─────────────────┐
         │                 │                 │
         ▼                 ▼                 ▼
     Sensors           ESP32              Outputs
         │                 │                 │
         │                 │                 ├── OLED
         │                 │                 ├── LEDs
         │                 │                 ├── Buzzer
         │                 │                 └── Servo
         │                 │
         └─────────────────┤
                           ▼
                    Embedded Firmware
                           │
                           ▼
                     Web Dashboard
```

The simulation is used to validate:

- Sensor input
- Parking-state logic
- OLED output
- LED behavior
- Buzzer behavior
- Servo behavior
- Web dashboard

---

# 27. Repository Architecture

The project repository separates firmware, documentation, testing, simulation, outputs, and evidence.

```text
Smart-Parking-Ultrasonic-Sensor-System/
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
├── simulation/
│   ├── diagram.json
│   ├── README.md
│   └── wokwi.toml
│
├── test_cases/
│   ├── README.md
│   └── test_cases.csv
│
├── outputs/
│   ├── final_result.txt
│   ├── parking_status.txt
│   ├── serial_output.txt
│   └── test_results.csv
│
├── data/
│   └── sample_sensor_data.csv
│
├── reports/
│   └── project_report.md
│
├── screenshots/
│   └── Validation Evidence
│
├── README.md
├── platformio.ini
└── .gitignore
```

---

# 28. Design Principles

The architecture follows practical embedded-system design principles.

## Modularity

Each subsystem has a specific responsibility.

## Maintainability

Hardware functions and decision logic are separated into reusable functions.

## Observability

The OLED, LEDs, buzzer, serial monitor, and web dashboard make the system state visible.

## Fault Handling

Invalid sensor readings are ignored instead of immediately causing incorrect state transitions.

## Scalability

The four-slot implementation can serve as the foundation for a larger parking system.

## IoT Readiness

The ESP32 Wi-Fi and HTTP architecture provides a path toward cloud integration.

---

# 29. Scalability

The current prototype monitors four slots:

```text
4 × Sensors
      │
      ▼
    ESP32
      │
      ▼
4 Slot States
```

A larger deployment could use multiple ESP32 nodes:

```text
Multiple Parking Sensors
           │
           ▼
       ESP32 Nodes
           │
           ▼
       Network Gateway
           │
           ▼
      Cloud Platform
           │
           ▼
   Central Dashboard
```

Future extensions may include:

- More parking slots
- Multiple parking floors
- MQTT
- Cloud database
- Mobile application
- Reservation system
- Occupancy analytics
- Remote monitoring

---

# 30. Security Considerations

The current Wokwi prototype is intended for demonstration and does not implement production-grade security.

A real deployment should consider:

- Secure Wi-Fi credentials
- Dashboard authentication
- HTTPS
- Authorization for actuator control
- Input validation
- Secure firmware updates
- Network segmentation
- Protection against unauthorized gate operation

---

# 31. Physical Deployment Considerations

When moving from Wokwi to real hardware:

### Sensor Placement

Mount all sensors at consistent heights and angles.

### Threshold Calibration

The 35 cm threshold should be experimentally calibrated according to the parking layout and sensor position.

### Electrical Safety

HC-SR04 ECHO signals must be electrically compatible with ESP32 GPIO voltage levels.

### Servo Power

A physical servo may require a suitable external power source. Ensure a common ground with the ESP32.

### Environmental Factors

Physical ultrasonic readings can be affected by:

- Vehicle shape
- Sensor angle
- Reflective surfaces
- Temperature
- Installation geometry
- Sensor interference

---

# 32. Future Architecture

A production-oriented version could evolve into:

```text
                  SMART PARKING PLATFORM
                           │
           ┌───────────────┼────────────────┐
           │               │                │
           ▼               ▼                ▼
      Slot Sensors    Entry/Exit        Access Control
           │             Sensors            │
           │               │                │
           └───────────────┼────────────────┘
                           ▼
                         ESP32
                           │
                           ▼
                        MQTT/API
                           │
                           ▼
                    Cloud Backend
                           │
             ┌─────────────┼─────────────┐
             │             │             │
             ▼             ▼             ▼
        Web Dashboard   Mobile App   Analytics
```

Possible additions:

- RFID
- License-plate recognition
- Cloud storage
- Mobile application
- Parking reservation
- Payment integration
- Multi-floor support
- Historical analytics
- Notifications

---

# 33. Architecture Summary

The final system follows this end-to-end flow:

```text
                 SENSING
                    │
                    ▼
             HC-SR04 × 4
                    │
                    ▼
              ESP32 GPIO
                    │
                    ▼
         Distance Measurement
                    │
                    ▼
           Threshold Decision
                    │
              ┌─────┴─────┐
              ▼           ▼
           FREE        OCCUPIED
              │           │
              └─────┬─────┘
                    ▼
            Availability Count
                    │
       ┌────────────┼────────────┐
       │            │            │
       ▼            ▼            ▼
      OLED         LEDs       Web Dashboard
       │            │            │
       └────────────┼────────────┘
                    ▼
                 Buzzer
                    │
                    ▼
               Servo Gate
```

The architecture demonstrates a complete:

```text
Sensor
   ↓
Embedded Processing
   ↓
Decision Logic
   ↓
Local Feedback
   ↓
Actuation
   ↓
IoT Monitoring
```

workflow suitable for an embedded-systems and IoT portfolio project.

---

# 34. Implementation Reference

Main firmware:

```text
src/main.cpp
```

Related documentation:

```text
docs/algorithm.md
docs/hardware.md
docs/pin_mapping.md
docs/simulation.md
docs/testing.md
docs/troubleshooting.md
test_cases/README.md
```

---

# 35. Final Architecture Status

```text
Controller        : ESP32 DevKit
Parking Slots     : 4
Sensors           : HC-SR04 × 4
Threshold         : 35 cm
Local Display     : SSD1306 OLED
Indicators        : Green/Red LEDs
Alert             : Buzzer
Actuator          : SG90 Servo
Connectivity      : Wi-Fi
Web Interface     : HTTP Dashboard
Simulation        : Wokwi
Build System      : PlatformIO
Project Type      : Embedded + IoT Prototype
Validation        : Virtual Simulation
Physical Testing  : Not performed
Project Status    : Functional Virtual Prototype
```
