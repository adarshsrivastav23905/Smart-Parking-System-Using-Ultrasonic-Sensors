# 🔧 Smart Parking System — Hardware Design

## 1. Hardware Overview

The **Smart Parking System Using Ultrasonic Sensors** is designed around an **ESP32 DevKit** and four **HC-SR04 ultrasonic sensors**.

The hardware architecture supports four parking slots and provides multiple output interfaces:

- SSD1306 OLED display
- Individual green/red LEDs
- Parking-full buzzer
- SG90 servo gate
- Wi-Fi connectivity through the ESP32

The current implementation is a **virtual prototype validated in Wokwi** because physical components were not available during development.

---

# 2. Hardware Architecture

```text
                         SMART PARKING HARDWARE
                                  │
                                  ▼
                         ┌────────────────┐
                         │   ESP32 DevKit │
                         │ Main Controller│
                         └───────┬────────┘
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
        ▼                        ▼                        ▼
 ┌───────────────┐       ┌───────────────┐       ┌───────────────┐
 │ HC-SR04 × 4   │       │   OLED        │       │    Outputs    │
 │ Parking Sense │       │ SSD1306       │       │               │
 └───────────────┘       └───────────────┘       │ LEDs          │
                                                 │ Buzzer        │
                                                 │ Servo         │
                                                 └───────────────┘
```

---

# 3. Main Controller

## ESP32 DevKit

The ESP32 is the central hardware controller.

### Main responsibilities

- Read four ultrasonic sensors.
- Process distance measurements.
- Determine parking-slot states.
- Count available slots.
- Drive LEDs.
- Drive the buzzer.
- Control the servo.
- Communicate with the OLED.
- Connect to Wi-Fi.
- Host the local web dashboard.

### Why ESP32?

ESP32 is a strong choice for this project because it provides:

- Multiple digital GPIO pins
- I²C support
- PWM capability
- Built-in Wi-Fi
- Sufficient processing capability
- Arduino framework support
- PlatformIO support
- Good suitability for embedded and IoT prototypes

---

# 4. HC-SR04 Ultrasonic Sensors

## Quantity

```text
4 sensors
```

One sensor is assigned to each parking slot:

```text
HC-SR04 #1 → Slot 1
HC-SR04 #2 → Slot 2
HC-SR04 #3 → Slot 3
HC-SR04 #4 → Slot 4
```

## Purpose

The ultrasonic sensors measure the distance between the sensor and an object such as a vehicle.

The measured distance is used to determine whether a parking slot is:

```text
FREE
```

or:

```text
OCCUPIED
```

## Operating Principle

The HC-SR04 works by:

1. Receiving a trigger pulse.
2. Sending an ultrasonic burst.
3. Waiting for the reflected signal.
4. Generating an ECHO pulse.
5. Allowing the ESP32 to measure the ECHO duration.

```text
ESP32
  │
  │ TRIG
  ▼
HC-SR04
  │
  │ Ultrasonic Pulse
  ▼
Object / Vehicle
  │
  │ Reflected Pulse
  ▼
HC-SR04
  │
  │ ECHO
  ▼
ESP32
```

## Pins

| HC-SR04 Pin | Function |
|---|---|
| VCC | Power |
| GND | Ground |
| TRIG | Trigger input |
| ECHO | Echo output |

---

# 5. Ultrasonic Sensor Pin Mapping

| Parking Slot | TRIG | ECHO |
|---|---:|---:|
| Slot 1 | GPIO 5 | GPIO 17 |
| Slot 2 | GPIO 16 | GPIO 4 |
| Slot 3 | GPIO 27 | GPIO 26 |
| Slot 4 | GPIO 25 | GPIO 35 |

This mapping is used by the current firmware.

---

# 6. Ultrasonic Distance Measurement

The distance is calculated using:

```text
Distance = (Echo Time × Speed of Sound) / 2
```

The implementation uses approximately:

```text
Speed of Sound = 0.0343 cm/µs
```

The division by two is necessary because the ultrasonic signal travels from the sensor to the object and back.

```text
Sensor
  │
  ▼
Object
  │
  ▼
Sensor

Round Trip = 2 × Distance
```

---

# 7. Parking Threshold

The current prototype uses:

```text
35 cm
```

The decision rule is:

```text
Distance < 35 cm
        ↓
    OCCUPIED
```

```text
Distance >= 35 cm
        ↓
       FREE
```

### Example

| Measured Distance | Result |
|---:|---|
| 10 cm | OCCUPIED |
| 20 cm | OCCUPIED |
| 30 cm | OCCUPIED |
| 35 cm | FREE |
| 50 cm | FREE |
| 80 cm | FREE |

The threshold should be physically calibrated when deploying the system with real sensors.

---

# 8. OLED Display

## SSD1306 OLED 128×64

The OLED provides local real-time feedback.

### Purpose

It displays:

- Parking-slot distance
- Slot state
- Available slot count
- Detection threshold
- Gate status

### Interface

The OLED uses I²C.

```text
SDA → GPIO 21
SCL → GPIO 22
```

### Example Display

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

Legend:

```text
F  = FREE
O  = OCCUPIED
TH = Threshold
```

---

# 9. LED Indicators

Each parking slot uses two LEDs.

```text
Green LED → FREE
Red LED   → OCCUPIED
```

## Slot LED Mapping

| Slot | Green LED | Red LED |
|---|---:|---:|
| Slot 1 | GPIO 12 | GPIO 2 |
| Slot 2 | GPIO 14 | GPIO 13 |
| Slot 3 | GPIO 32 | GPIO 23 |
| Slot 4 | GPIO 15 | GPIO 33 |

## LED Behavior

### Free slot

```text
Green LED = ON
Red LED   = OFF
```

### Occupied slot

```text
Green LED = OFF
Red LED   = ON
```

Each LED should use an appropriate current-limiting resistor.

---

# 10. LED Resistors

The prototype uses:

```text
220 Ω
```

series resistors for LED current limiting.

Conceptually:

```text
ESP32 GPIO
     │
     ▼
  220 Ω
  Resistor
     │
     ▼
    LED
     │
     ▼
    GND
```

For a physical implementation, select the resistor value according to the LED forward voltage, desired current, and ESP32 GPIO limits.

---

# 11. Buzzer

The buzzer provides an audible warning when the parking facility reaches full capacity.

### Connection

```text
Buzzer Signal → GPIO 19
Buzzer GND    → GND
```

### Operation

```text
Available Slots = 0
        ↓
PARKING FULL
        ↓
Buzzer Alert
```

The current firmware uses short controlled alert pulses.

---

# 12. Servo Gate

## SG90 Micro Servo

The SG90 is used as a prototype parking barrier actuator.

### Connection

```text
Servo PWM → GPIO 18
Servo V+  → 5V
Servo GND → GND
```

### Gate Behavior

The current project uses capacity-based gate control.

```text
Available Slots > 0
        ↓
    GATE OPEN
```

```text
Available Slots = 0
        ↓
   GATE CLOSED
```

### Important Scope

The final circuit does not contain a dedicated entry sensor or push button.

Therefore, the servo is used to represent **parking-capacity access status** rather than detecting the physical arrival or departure of a vehicle.

A production implementation would normally include dedicated entrance detection and safety sensing.

---

# 13. Wi-Fi Connectivity

The ESP32 provides built-in Wi-Fi.

The networking layer enables the local web dashboard.

```text
ESP32
  │
  ▼
Wi-Fi
  │
  ▼
HTTP Server
  │
  ▼
Browser Dashboard
```

For Wokwi, the configured network is:

```text
Wokwi-GUEST
```

The dashboard is accessed through:

```text
http://localhost:8180
```

---

# 14. Complete Pin Configuration

| Hardware | Signal | ESP32 GPIO |
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

# 15. Power and Ground Architecture

All components should share a common ground in the physical implementation.

```text
             ┌───────────────┐
             │     ESP32     │
             └───────┬───────┘
                     │
                  Common GND
                     │
       ┌─────────────┼──────────────┐
       │             │              │
       ▼             ▼              ▼
    Sensors        OLED         Outputs
                                  │
                           ┌──────┼──────┐
                           ▼      ▼      ▼
                         LEDs  Buzzer  Servo
```

---

# 16. Physical Power Considerations

The current system is designed and validated virtually.

For physical implementation:

### ESP32

Use a suitable USB or regulated supply.

### HC-SR04

Typical modules are powered from 5V.

### OLED

Use a voltage supply compatible with the selected SSD1306 breakout.

### Servo

The servo may draw significantly more current than an ESP32 GPIO can provide.

Use an appropriate external supply when required.

### Ground

Connect the external supply ground to the ESP32 ground.

---

# 17. HC-SR04 ECHO Voltage Consideration

A major hardware consideration for physical deployment is the HC-SR04 ECHO signal.

Some HC-SR04 modules can provide an ECHO signal at approximately 5V.

ESP32 GPIO is designed for lower logic levels.

Therefore, for real hardware, use:

- A resistor divider
- A suitable level shifter
- Or another electrically compatible interface

Example resistor-divider concept:

```text
HC-SR04 ECHO
      │
      ▼
   Resistor
      │
      ├──────────► ESP32 GPIO
      │
   Resistor
      │
      ▼
     GND
```

For Wokwi, the simulated connections are sufficient for the virtual prototype.

---

# 18. Recommended Physical Sensor Placement

For a real installation:

- Mount sensors at consistent heights.
- Point sensors toward the vehicle detection area.
- Keep the sensor orientation consistent.
- Avoid direct obstruction of the ultrasonic path.
- Leave enough spacing between sensors.
- Calibrate the threshold after installation.

Conceptually:

```text
        HC-SR04
           │
           │ Ultrasonic beam
           ▼
      ┌───────────┐
      │  Vehicle  │
      └───────────┘
           │
           ▼
       Parking Slot
```

---

# 19. Sensor Interference Considerations

Multiple ultrasonic sensors may interfere if triggered simultaneously.

The software therefore processes the sensors sequentially.

```text
Read Slot 1
   ↓
Read Slot 2
   ↓
Read Slot 3
   ↓
Read Slot 4
```

A small delay between sensor operations helps reduce cross-talk.

For a physical system, the final timing should be validated experimentally.

---

# 20. Hardware State Mapping

The hardware responds to the logical parking state.

```text
                 SLOT STATE
                     │
          ┌──────────┴──────────┐
          │                     │
        FREE                OCCUPIED
          │                     │
          ▼                     ▼
    Green LED ON           Red LED ON
    Red LED OFF            Green LED OFF
```

For the complete system:

```text
             Parking Capacity
                    │
        ┌───────────┴───────────┐
        │                       │
 Available > 0            Available = 0
        │                       │
        ▼                       ▼
   Gate OPEN               Gate CLOSED
                               │
                               ▼
                         Buzzer Alert
```

---

# 21. Hardware Interface Summary

| Subsystem | Input/Output | Interface |
|---|---|---|
| HC-SR04 | Sensor input | GPIO + timing |
| OLED | Display output | I²C |
| Green LEDs | Status output | GPIO |
| Red LEDs | Status output | GPIO |
| Buzzer | Alert output | GPIO |
| Servo | Actuator output | PWM |
| Wi-Fi | Network communication | ESP32 radio |

---

# 22. Physical Hardware Bill of Materials

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit | 1 | Main controller |
| HC-SR04 | 4 | Slot sensing |
| SSD1306 OLED 128×64 | 1 | Display |
| Green LED | 4 | Free indication |
| Red LED | 4 | Occupied indication |
| 220 Ω resistor | 8 | LED current limiting |
| SG90 Servo | 1 | Gate mechanism |
| Buzzer | 1 | Parking-full alert |
| Breadboard | 1 or more | Prototyping |
| Jumper wires | As required | Connections |
| 5V supply | As required | Sensors/actuators |
| Resistors for level shifting | As required | HC-SR04 ECHO compatibility |

---

# 23. Expected Hardware Behavior

## All Slots Free

```text
S1 → FREE
S2 → FREE
S3 → FREE
S4 → FREE

Green LEDs → ON
Red LEDs   → OFF
Buzzer     → OFF
Gate       → OPEN
OLED       → FREE:4/4
```

## Two Slots Occupied

```text
S1 → OCCUPIED
S2 → FREE
S3 → OCCUPIED
S4 → FREE

Green:
S2, S4 → ON

Red:
S1, S3 → ON

Available → 2/4
Gate → OPEN
Buzzer → OFF
```

## Parking Full

```text
S1 → OCCUPIED
S2 → OCCUPIED
S3 → OCCUPIED
S4 → OCCUPIED

Available → 0/4
Green LEDs → OFF
Red LEDs → ON
Buzzer → ON
Gate → CLOSED
OLED → PARKING FULL
```

---

# 24. Hardware Safety Considerations

When moving from simulation to physical hardware:

- Do not exceed ESP32 GPIO voltage limits.
- Do not power a high-current servo directly from a GPIO.
- Use a suitable servo power source.
- Maintain a common ground.
- Use current-limiting resistors with LEDs.
- Use level shifting for incompatible logic levels.
- Verify wiring before powering the system.
- Avoid short circuits.
- Use regulated supplies.
- Protect the circuit from accidental reverse polarity.
- Keep sensor wiring organized.

---

# 25. Virtual Hardware vs Physical Hardware

| Feature | Wokwi | Physical Hardware |
|---|---|---|
| ESP32 | Simulated | Required |
| HC-SR04 | Simulated | Required |
| OLED | Simulated | Required |
| LEDs | Simulated | Required |
| Buzzer | Simulated | Required |
| Servo | Simulated | Required |
| Wiring | Virtual | Breadboard/PCB |
| Sensor noise | Simplified | Real-world |
| Calibration | Simulated | Required |
| Power issues | Mostly abstracted | Must be managed |
| Environmental effects | Limited | Present |

The Wokwi prototype validates the firmware and logical integration, while physical deployment would require additional electrical, mechanical, and calibration validation.

---

# 26. Hardware Expansion

The hardware can be expanded with:

- Entry vehicle sensor
- Exit vehicle sensor
- RFID reader
- Additional ultrasonic sensors
- IR sensors
- Barrier safety sensor
- RGB LEDs
- Larger display
- Ethernet/Wi-Fi gateway
- Environmental sensors
- Camera system

A future physical architecture could be:

```text
Parking Sensors
      │
      ▼
    ESP32
      │
 ┌────┼───────────┐
 │    │           │
 ▼    ▼           ▼
OLED LEDs       Servo
      │
      ▼
   Wi-Fi
      │
      ▼
Cloud / Dashboard
```

---

# 27. Hardware Design Summary

The current prototype uses the ESP32 as a central embedded controller with four ultrasonic sensors as inputs and multiple actuators/displays as outputs.

The hardware data flow is:

```text
HC-SR04 Sensors
       ↓
ESP32 GPIO
       ↓
Distance Measurement
       ↓
Parking Decision
       ↓
┌──────┼─────────┬─────────┐
│      │         │         │
OLED  LEDs     Buzzer     Servo
       │                    │
       └────────┬───────────┘
                ▼
           Parking Status
                │
                ▼
          Web Dashboard
```

---

# 28. Implementation Reference

Main firmware:

```text
src/main.cpp
```

Pin reference:

```text
docs/pin_mapping.md
```

System architecture:

```text
docs/architecture.md
```

Algorithm:

```text
docs/algorithm.md
```

Simulation guide:

```text
docs/simulation.md
```

Testing guide:

```text
docs/testing.md
```

---

# 29. Final Hardware Specification

```text
Controller        : ESP32 DevKit
Parking Slots     : 4
Ultrasonic        : HC-SR04 × 4
Display           : SSD1306 OLED 128×64
Green LEDs        : 4
Red LEDs          : 4
Buzzer            : 1
Servo             : SG90
Threshold         : 35 cm
Connectivity      : ESP32 Wi-Fi
Web Dashboard     : HTTP
Simulation        : Wokwi
Development       : VS Code + PlatformIO
Physical Testing  : Not performed
Project Status    : Functional Virtual Prototype
```

---

# 30. Final Note

The hardware design is intentionally modular so that the virtual prototype can be converted into a physical prototype later.

The current implementation demonstrates the core embedded concepts of:

```text
Sensor Interfacing
       ↓
GPIO Control
       ↓
Distance Measurement
       ↓
Decision Logic
       ↓
Display / Indicators
       ↓
Actuator Control
       ↓
IoT Connectivity
```

The next physical-development stage would be sensor mounting, voltage-level verification, power design, threshold calibration, and physical safety validation.
