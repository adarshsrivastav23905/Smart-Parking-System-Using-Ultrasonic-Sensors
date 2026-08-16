# 🧠 Smart Parking System — Algorithm

## 1. Purpose

This document describes the algorithm used by the **Smart Parking System Using Ultrasonic Sensors**.

The algorithm controls the complete parking workflow:

```text
Sensor Measurement
        ↓
Distance Calculation
        ↓
FREE / OCCUPIED Classification
        ↓
Available Slot Calculation
        ↓
OLED / LED / Buzzer Update
        ↓
Servo Gate Control
        ↓
Web Dashboard Update
```

The current implementation monitors four parking slots using four HC-SR04 ultrasonic sensors connected to an ESP32.

## 2. System Parameters

| Parameter | Value |
| --- | --- |
| Controller | ESP32 DevKit |
| Number of Slots | 4 |
| Sensor | HC-SR04 |
| Detection Threshold | 35 cm |
| Display | SSD1306 OLED |
| Communication | I²C |
| Connectivity | Wi-Fi |
| Web Protocol | HTTP |
| Gate Actuator | SG90 Servo |
| Alert Device | Buzzer |
| Simulation | Wokwi |

## 3. Inputs

The algorithm receives distance measurements from four parking-slot sensors.

```text
Sensor 1 → Slot 1 Distance
Sensor 2 → Slot 2 Distance
Sensor 3 → Slot 3 Distance
Sensor 4 → Slot 4 Distance
```

Each sensor provides:

```text
TRIG → Control signal from ESP32
ECHO → Timing signal returned to ESP32
```

## 4. Outputs

The algorithm controls:

```text
OLED Display
Green LEDs
Red LEDs
Buzzer
Servo Gate
Web Dashboard
Serial Monitor
```

## 5. Core Detection Rule

The system uses a configurable distance threshold of **35 cm**.

```text
IF distance < 35 cm
    Slot = OCCUPIED
ELSE
    Slot = FREE
```

| Sensor Distance | Parking State |
|---:|---|
| 10 cm | OCCUPIED |
| 20 cm | OCCUPIED |
| 30 cm | OCCUPIED |
| 35 cm | FREE |
| 50 cm | FREE |
| 80 cm | FREE |

The threshold should be calibrated when the system is transferred to physical hardware.

## 6. Ultrasonic Distance Measurement Algorithm

```text
START
  ↓
Set TRIG LOW
  ↓
Wait briefly
  ↓
Set TRIG HIGH for 10 µs
  ↓
Set TRIG LOW
  ↓
Wait for ECHO
  ↓
Measure ECHO pulse duration
  ↓
Check for timeout
  ↓
Calculate distance
  ↓
Validate distance
  ↓
Return distance
```

## 7. Distance Calculation

```text
Distance = (Echo Time × Speed of Sound) / 2
```

The implementation uses approximately:

```text
Speed of Sound = 0.0343 cm/µs
```

The division by two is required because the ultrasonic pulse travels:

```text
Sensor
  ↓
Object
  ↓
Sensor
```

The measured time is therefore a round-trip time.

## 8. Invalid Reading Handling

If no valid echo is received:

```text
ECHO TIMEOUT
     ↓
INVALID READING
     ↓
Ignore reading
     ↓
Keep previous slot state
```

This prevents one missing measurement from immediately changing a slot state.

## 9. Slot State Determination

Each slot maintains state information:

```text
Slot
├── Distance
├── Occupied State
├── Pending State
├── Candidate State
└── Candidate Timestamp
```

The algorithm compares a new decision with the current confirmed state.

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

## 10. State Confirmation

Ultrasonic readings can vary because of reflection angle, object movement, noise, or simulation variation.

A short confirmation period is used before accepting a state change.

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

The same procedure is used for:

```text
OCCUPIED → FREE
```

## 11. Parking Availability Algorithm

After processing all four slots:

```text
Occupied Slots =
Number of slots marked OCCUPIED
```

Then:

```text
Available Slots =
Total Slots - Occupied Slots
```

For the current prototype:

```text
Available Slots = 4 - Occupied Slots
```

## 12. Example Availability Calculation

Example:

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

Final result:

```text
Available = 2/4
Occupied  = 2/4
```

## 13. LED Control Algorithm

Each parking slot has two status LEDs.

```text
GREEN → FREE
RED   → OCCUPIED
```

```text
IF Slot == FREE
    Green LED = ON
    Red LED   = OFF
```

```text
IF Slot == OCCUPIED
    Green LED = OFF
    Red LED   = ON
```

## 14. OLED Update Algorithm

The OLED is updated periodically with:

- Slot distance
- Slot state
- Available slot count
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

## 15. Parking-Full Detection

```text
IF Available Slots == 0
    System Status = PARKING FULL
    Buzzer = ON
    Gate = CLOSED
ELSE
    System Status = SPACE AVAILABLE
    Buzzer = OFF
    Gate = OPEN
```

## 16. Buzzer Algorithm

```text
Available Slots
       ↓
Is Available == 0?
       /       \
     YES        NO
      │          │
      ▼          ▼
 Buzzer Alert   Buzzer OFF
```

The firmware generates controlled alert pulses rather than continuously blocking the main loop.

## 17. Servo Gate Algorithm

The current prototype uses **capacity-based gate control**.

### Gate Opening

```text
IF Available Slots > 0
AND Gate is currently closed

    Move Servo to OPEN angle
    Mark Gate as OPEN
```

### Gate Closing

```text
IF Available Slots == 0
AND Gate is currently open

    Move Servo to CLOSED angle
    Mark Gate as CLOSED
```

### Gate State Model

```text
                Parking Status
                     │
          ┌──────────┴──────────┐
          │                     │
   Available > 0          Available = 0
          │                     │
          ▼                     ▼
     GATE OPEN             GATE CLOSED
                               │
                               ▼
                         BUZZER ALERT
```

The final circuit intentionally does not contain a dedicated vehicle-entry sensor or push button. Therefore, the servo demonstrates capacity-based parking access status rather than vehicle-arrival detection.

## 18. Wi-Fi Initialization Algorithm

```text
START Wi-Fi
     ↓
Connect to configured network
     ↓
Wait for connection
     ↓
Connection successful?
    /          \
  YES           NO
   │             │
   ▼             ▼
Print IP      Continue in
Address       Offline Mode
```

For Wokwi, the configured network is:

```text
Wokwi-GUEST
```

## 19. Web Server Algorithm

```text
Browser Request
      ↓
HTTP Server
      ↓
handleRoot()
      ↓
Read Current Parking State
      ↓
Generate HTML Page
      ↓
Send HTTP Response
      ↓
Browser Displays Dashboard
```

The dashboard displays:

- Available slots
- Occupied slots
- Individual slot status
- Individual distance
- Threshold
- System status
- Gate status

## 20. Main Program Algorithm

```text
START
  │
  ▼
Initialize Serial
  │
  ▼
Initialize Ultrasonic Sensors
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
Count Occupied Slots
  │
  ▼
Calculate Available Slots
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

## 21. Pseudocode

```text
BEGIN

    Initialize ESP32
    Initialize four ultrasonic sensors
    Initialize OLED
    Initialize green and red LEDs
    Initialize buzzer
    Initialize servo
    Connect Wi-Fi
    Start web server

    WHILE system is running

        FOR each parking slot

            Measure ultrasonic echo time

            IF reading is invalid
                Keep current state

            ELSE
                Calculate distance

                IF distance < 35 cm
                    Candidate state = OCCUPIED
                ELSE
                    Candidate state = FREE
                END IF

                Confirm state change
            END IF

        END FOR

        Count occupied slots
        Calculate available slots

        FOR each parking slot

            IF slot is FREE
                Green LED ON
                Red LED OFF
            ELSE
                Green LED OFF
                Red LED ON
            END IF

        END FOR

        Update OLED

        IF available slots == 0
            Activate buzzer
            Close servo gate
        ELSE
            Disable buzzer
            Open servo gate
        END IF

        Handle HTTP web requests
        Print debugging information

    END WHILE

END
```

## 22. Flowchart

```text
                     ┌───────────┐
                     │   START   │
                     └─────┬─────┘
                           │
                           ▼
                 ┌──────────────────┐
                 │ Initialize ESP32 │
                 └────────┬─────────┘
                          │
                          ▼
             ┌──────────────────────────┐
             │ Initialize Sensors/OLED │
             │ LEDs/Buzzer/Servo       │
             └────────────┬─────────────┘
                          │
                          ▼
                  ┌──────────────┐
                  │ Connect Wi-Fi│
                  └──────┬───────┘
                         │
                         ▼
                 ┌───────────────┐
                 │ Start Web     │
                 │ Server        │
                 └──────┬────────┘
                        │
                        ▼
                ┌────────────────┐
                │ Read Sensors   │
                └───────┬────────┘
                        │
                        ▼
                ┌────────────────┐
                │ Calculate      │
                │ Distance       │
                └───────┬────────┘
                        │
                        ▼
               ┌──────────────────┐
               │ Distance < 35cm? │
               └──────┬─────┬─────┘
                      │YES  │NO
                      ▼     ▼
                ┌────────┐ ┌──────┐
                │OCCUPIED│ │ FREE │
                └────┬───┘ └───┬──┘
                     │          │
                     └────┬─────┘
                          ▼
                 ┌─────────────────┐
                 │ Update Slot     │
                 │ State           │
                 └────────┬────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │ Count Available │
                 │ Slots           │
                 └────────┬────────┘
                          │
                          ▼
              ┌────────────────────────┐
              │ Update OLED + LEDs     │
              └───────────┬────────────┘
                          │
                          ▼
                 ┌────────────────┐
                 │ Available = 0? │
                 └───────┬───┬────┘
                         │YES│NO
                         ▼   ▼
                 ┌────────┐ ┌────────┐
                 │ Buzzer │ │ Buzzer │
                 │ ON     │ │ OFF    │
                 │ Gate   │ │ Gate   │
                 │ CLOSED │ │ OPEN   │
                 └────┬───┘ └───┬────┘
                      │          │
                      └────┬─────┘
                           ▼
                 ┌──────────────────┐
                 │ Handle Web       │
                 │ Requests         │
                 └────────┬─────────┘
                          │
                          ▼
                    ┌──────────┐
                    │  REPEAT  │
                    └──────────┘
```

## 23. Example Execution

Input:

```text
S1 = 20 cm
S2 = 80 cm
S3 = 20 cm
S4 = 80 cm
```

Classification:

```text
S1 → OCCUPIED
S2 → FREE
S3 → OCCUPIED
S4 → FREE
```

Count:

```text
Occupied = 2
Available = 2
```

Outputs:

```text
S1 → Red ON
S2 → Green ON
S3 → Red ON
S4 → Green ON

OLED → FREE:2/4
Gate → OPEN
```

The web dashboard reports:

```text
Available Slots: 2/4
Occupied Slots: 2/4
```

## 24. Full-Parking Execution

Input:

```text
20 / 20 / 20 / 20
```

Classification:

```text
S1 → OCCUPIED
S2 → OCCUPIED
S3 → OCCUPIED
S4 → OCCUPIED
```

Count:

```text
Occupied = 4
Available = 0
```

System response:

```text
OLED → PARKING FULL
Green LEDs → OFF
Red LEDs → ON
Buzzer → ON
Servo → CLOSED
Web Dashboard → PARKING FULL
```

## 25. Slot-Release Execution

Initial state:

```text
20 / 20 / 20 / 20
```

Then:

```text
20 / 20 / 20 / 80
```

Result:

```text
S4 → FREE
Occupied = 3
Available = 1
```

System response:

```text
S4 Green LED → ON
S4 Red LED → OFF
Buzzer → OFF
Servo → OPEN
OLED → FREE:1/4
Web Dashboard → Available:1/4
```

## 26. Time and Performance Considerations

The firmware uses timing controls for:

- Ultrasonic echo timeout
- Slot-state confirmation
- OLED refresh
- Serial output
- Buzzer interval

The four sensors are processed sequentially to reduce possible ultrasonic cross-talk.

The servo is commanded only when its required state changes, preventing unnecessary repeated servo commands.

## 27. Error and Recovery Behavior

### Invalid Sensor Reading

```text
Sensor Timeout
      ↓
Ignore Reading
      ↓
Retain Previous Slot State
      ↓
Continue Monitoring
```

### Wi-Fi Failure

```text
Wi-Fi Connection Attempt
        ↓
Connection Successful?
       /             YES         NO
      │           │
      ▼           ▼
Start Web      Continue Embedded
Dashboard      Parking Functions
```

Parking detection therefore remains the primary embedded function even if the network interface is unavailable.

## 28. Complexity

For `N` parking slots, sensor processing is approximately:

```text
O(N)
```

For this prototype:

```text
N = 4
```

The processing requirement is small and suitable for an ESP32-class controller.

## 29. Scalability

The algorithm can be generalized to additional slots:

```text
FOR i = 1 to N

    Read sensor i
    Calculate distance
    Classify slot i
    Update slot state

END FOR

Available = N - Occupied
```

This allows the same core logic to be extended to larger parking facilities.

## 30. Algorithm Summary

```text
                 SENSOR INPUT
                      │
                      ▼
              DISTANCE MEASUREMENT
                      │
                      ▼
                  VALIDATION
                      │
                      ▼
              THRESHOLD CHECK
                      │
              ┌───────┴───────┐
              ▼               ▼
           FREE            OCCUPIED
              │               │
              └───────┬───────┘
                      ▼
                STATE UPDATE
                      │
                      ▼
              AVAILABILITY COUNT
                      │
          ┌───────────┼───────────┐
          │           │           │
          ▼           ▼           ▼
         OLED        LEDs        WEB
          │
          ▼
        BUZZER
          │
          ▼
        SERVO
```

The overall algorithm follows:

```text
SENSING
   ↓
PROCESSING
   ↓
DECISION
   ↓
INDICATION
   ↓
ACTUATION
   ↓
WEB MONITORING
```

## 31. Implementation Reference

Main firmware:

```text
src/main.cpp
```

Related documentation:

```text
docs/architecture.md
docs/hardware.md
docs/pin_mapping.md
docs/simulation.md
docs/testing.md
docs/troubleshooting.md
test_cases/README.md
```

## 32. Final Algorithm Status

```text
Controller        : ESP32
Parking Slots     : 4
Sensor            : HC-SR04 × 4
Threshold         : 35 cm
Local Display     : SSD1306 OLED
Indicators        : Green/Red LEDs
Alert             : Buzzer
Actuator          : SG90 Servo
Connectivity      : Wi-Fi
Web Interface     : HTTP Dashboard
Simulation        : Wokwi
Build System      : PlatformIO
Project Status    : Functional Virtual Prototype
```
