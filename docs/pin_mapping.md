# 📍 Smart Parking System — ESP32 Pin Mapping

## 1. Pin Mapping Overview

This document defines the complete GPIO allocation for the **Smart Parking System Using Ultrasonic Sensors**.

The current prototype uses:

- ESP32 DevKit
- 4 × HC-SR04 ultrasonic sensors
- SSD1306 OLED
- 4 × green LEDs
- 4 × red LEDs
- Buzzer
- SG90 servo

The pin mapping is designed to keep each parking slot clearly associated with its sensor and status indicators.

---

# 2. Complete Pin Mapping

| Component | Signal | ESP32 GPIO | Direction | Function |
|---|---|---:|---|---|
| Slot 1 HC-SR04 | TRIG | GPIO 5 | OUTPUT | Trigger ultrasonic pulse |
| Slot 1 HC-SR04 | ECHO | GPIO 17 | INPUT | Receive echo pulse |
| Slot 2 HC-SR04 | TRIG | GPIO 16 | OUTPUT | Trigger ultrasonic pulse |
| Slot 2 HC-SR04 | ECHO | GPIO 4 | INPUT | Receive echo pulse |
| Slot 3 HC-SR04 | TRIG | GPIO 27 | OUTPUT | Trigger ultrasonic pulse |
| Slot 3 HC-SR04 | ECHO | GPIO 26 | INPUT | Receive echo pulse |
| Slot 4 HC-SR04 | TRIG | GPIO 25 | OUTPUT | Trigger ultrasonic pulse |
| Slot 4 HC-SR04 | ECHO | GPIO 35 | INPUT | Receive echo pulse |
| SSD1306 OLED | SDA | GPIO 21 | I/O | I²C data |
| SSD1306 OLED | SCL | GPIO 22 | OUTPUT | I²C clock |
| SG90 Servo | PWM | GPIO 18 | OUTPUT | Gate position control |
| Buzzer | Signal | GPIO 19 | OUTPUT | Parking-full alert |
| Slot 1 Green LED | Anode | GPIO 12 | OUTPUT | FREE indication |
| Slot 1 Red LED | Anode | GPIO 2 | OUTPUT | OCCUPIED indication |
| Slot 2 Green LED | Anode | GPIO 14 | OUTPUT | FREE indication |
| Slot 2 Red LED | Anode | GPIO 13 | OUTPUT | OCCUPIED indication |
| Slot 3 Green LED | Anode | GPIO 32 | OUTPUT | FREE indication |
| Slot 3 Red LED | Anode | GPIO 23 | OUTPUT | OCCUPIED indication |
| Slot 4 Green LED | Anode | GPIO 15 | OUTPUT | FREE indication |
| Slot 4 Red LED | Anode | GPIO 33 | OUTPUT | OCCUPIED indication |

---

# 3. Parking Slot Mapping

Each parking slot has one ultrasonic sensor and one green/red LED pair.

## Slot 1

```text
HC-SR04 TRIG → GPIO 5
HC-SR04 ECHO → GPIO 17

Green LED → GPIO 12
Red LED   → GPIO 2
```

Logic:

```text
FREE
  ↓
Green LED ON
Red LED OFF
```

```text
OCCUPIED
  ↓
Green LED OFF
Red LED ON
```

---

## Slot 2

```text
HC-SR04 TRIG → GPIO 16
HC-SR04 ECHO → GPIO 4

Green LED → GPIO 14
Red LED   → GPIO 13
```

---

## Slot 3

```text
HC-SR04 TRIG → GPIO 27
HC-SR04 ECHO → GPIO 26

Green LED → GPIO 32
Red LED   → GPIO 23
```

---

## Slot 4

```text
HC-SR04 TRIG → GPIO 25
HC-SR04 ECHO → GPIO 35

Green LED → GPIO 15
Red LED   → GPIO 33
```

---

# 4. Ultrasonic Sensor Connections

Each HC-SR04 has four primary connections.

## Slot 1

```text
HC-SR04 VCC  → 5V
HC-SR04 GND  → GND
HC-SR04 TRIG → GPIO 5
HC-SR04 ECHO → GPIO 17
```

## Slot 2

```text
HC-SR04 VCC  → 5V
HC-SR04 GND  → GND
HC-SR04 TRIG → GPIO 16
HC-SR04 ECHO → GPIO 4
```

## Slot 3

```text
HC-SR04 VCC  → 5V
HC-SR04 GND  → GND
HC-SR04 TRIG → GPIO 27
HC-SR04 ECHO → GPIO 26
```

## Slot 4

```text
HC-SR04 VCC  → 5V
HC-SR04 GND  → GND
HC-SR04 TRIG → GPIO 25
HC-SR04 ECHO → GPIO 35
```

---

# 5. OLED Connections

The SSD1306 OLED communicates through I²C.

```text
OLED VCC → 3.3V
OLED GND → GND
OLED SDA → GPIO 21
OLED SCL → GPIO 22
```

### I²C Address

The current implementation uses:

```text
0x3C
```

The OLED is used to display:

- Slot distances
- Slot states
- Available slots
- Threshold
- Gate status

---

# 6. Servo Connections

The SG90 servo uses a PWM control signal.

```text
Servo Signal/PWM → GPIO 18
Servo V+         → 5V
Servo GND        → GND
```

### Gate States

```text
Available Slots > 0
        ↓
Gate OPEN
```

```text
Available Slots = 0
        ↓
Gate CLOSED
```

The servo is used as a capacity-based gate prototype.

---

# 7. Buzzer Connection

```text
Buzzer Signal → GPIO 19
Buzzer GND    → GND
```

The buzzer is activated when:

```text
Available Slots = 0
```

The firmware generates controlled alert pulses.

---

# 8. LED Connections

Every LED should have a current-limiting resistor.

## Slot 1

```text
GPIO 12 → 220 Ω → Green LED → GND
GPIO 2  → 220 Ω → Red LED   → GND
```

## Slot 2

```text
GPIO 14 → 220 Ω → Green LED → GND
GPIO 13 → 220 Ω → Red LED   → GND
```

## Slot 3

```text
GPIO 32 → 220 Ω → Green LED → GND
GPIO 23 → 220 Ω → Red LED   → GND
```

## Slot 4

```text
GPIO 15 → 220 Ω → Green LED → GND
GPIO 33 → 220 Ω → Red LED   → GND
```

### LED Logic

```text
FREE
  ↓
Green = ON
Red   = OFF
```

```text
OCCUPIED
  ↓
Green = OFF
Red   = ON
```

---

# 9. Power and Ground Connections

All modules should share a common ground.

```text
ESP32 GND
   │
   ├── HC-SR04 #1 GND
   ├── HC-SR04 #2 GND
   ├── HC-SR04 #3 GND
   ├── HC-SR04 #4 GND
   ├── OLED GND
   ├── Buzzer GND
   ├── Servo GND
   └── LED Grounds
```

### Typical Supply Arrangement

```text
ESP32 → USB / regulated supply
HC-SR04 → 5V
OLED → 3.3V-compatible supply
Servo → suitable 5V supply
LEDs → ESP32 GPIO through resistors
Buzzer → GPIO-controlled
```

For physical deployment, the servo may require a separate adequate-current supply.

---

# 10. GPIO Summary

## Output GPIOs

| GPIO | Device | Role |
|---:|---|---|
| 5 | Slot 1 TRIG | Ultrasonic trigger |
| 16 | Slot 2 TRIG | Ultrasonic trigger |
| 27 | Slot 3 TRIG | Ultrasonic trigger |
| 25 | Slot 4 TRIG | Ultrasonic trigger |
| 21 | OLED SDA | I²C data |
| 22 | OLED SCL | I²C clock |
| 18 | Servo | PWM |
| 19 | Buzzer | Alert |
| 12 | Slot 1 Green | FREE |
| 2 | Slot 1 Red | OCCUPIED |
| 14 | Slot 2 Green | FREE |
| 13 | Slot 2 Red | OCCUPIED |
| 32 | Slot 3 Green | FREE |
| 23 | Slot 3 Red | OCCUPIED |
| 15 | Slot 4 Green | FREE |
| 33 | Slot 4 Red | OCCUPIED |

## Input GPIOs

| GPIO | Device | Role |
|---:|---|---|
| 17 | Slot 1 ECHO | Ultrasonic echo |
| 4 | Slot 2 ECHO | Ultrasonic echo |
| 26 | Slot 3 ECHO | Ultrasonic echo |
| 35 | Slot 4 ECHO | Ultrasonic echo |

---

# 11. Pin Allocation by Subsystem

```text
┌────────────────────────────────────┐
│            ESP32 GPIO              │
├────────────────────────────────────┤
│ Ultrasonic TRIG: 5, 16, 27, 25    │
│ Ultrasonic ECHO: 17, 4, 26, 35    │
│ OLED I²C:         21, 22           │
│ Servo PWM:        18               │
│ Buzzer:           19               │
│ Green LEDs:       12, 14, 32, 15  │
│ Red LEDs:          2, 13, 23, 33  │
└────────────────────────────────────┘
```

---

# 12. GPIO Functional Diagram

```text
                     ESP32
                       │
        ┌──────────────┼──────────────┐
        │              │              │
        ▼              ▼              ▼
   Ultrasonic        OLED          Outputs
   Sensors ×4         │               │
        │             │       ┌───────┼────────┐
        │             │       │       │        │
        ▼             ▼       ▼       ▼        ▼
     Slot Data       I²C     LEDs   Buzzer    Servo
```

---

# 13. Complete Connection Table

| Device | VCC | GND | Signal 1 | Signal 2 |
|---|---|---|---|---|
| HC-SR04 Slot 1 | 5V | GND | TRIG → 5 | ECHO → 17 |
| HC-SR04 Slot 2 | 5V | GND | TRIG → 16 | ECHO → 4 |
| HC-SR04 Slot 3 | 5V | GND | TRIG → 27 | ECHO → 26 |
| HC-SR04 Slot 4 | 5V | GND | TRIG → 25 | ECHO → 35 |
| SSD1306 OLED | 3.3V | GND | SDA → 21 | SCL → 22 |
| SG90 Servo | 5V | GND | PWM → 18 | — |
| Buzzer | — | GND | Signal → 19 | — |
| Slot 1 Green LED | GPIO 12 | GND | — | 220 Ω |
| Slot 1 Red LED | GPIO 2 | GND | — | 220 Ω |
| Slot 2 Green LED | GPIO 14 | GND | — | 220 Ω |
| Slot 2 Red LED | GPIO 13 | GND | — | 220 Ω |
| Slot 3 Green LED | GPIO 32 | GND | — | 220 Ω |
| Slot 3 Red LED | GPIO 23 | GND | — | 220 Ω |
| Slot 4 Green LED | GPIO 15 | GND | — | 220 Ω |
| Slot 4 Red LED | GPIO 33 | GND | — | 220 Ω |

---

# 14. Electrical Safety Notes

For physical hardware implementation:

### HC-SR04 ECHO

Some HC-SR04 modules may provide approximately 5V ECHO logic.

ESP32 GPIOs require compatible logic levels.

Use:

- A resistor divider
- A suitable level shifter
- Another compatible interface

Do not assume a 5V ECHO signal is directly safe for every ESP32 GPIO configuration.

### Servo

Do not attempt to power a high-current physical servo directly from an ESP32 GPIO.

Use an appropriate supply and share the ground.

### LEDs

Use a current-limiting resistor for every LED.

### Power

Use regulated supplies and verify the polarity before powering the circuit.

---

# 15. Sensor Interference

Multiple ultrasonic sensors should not be triggered at exactly the same time.

The firmware processes the sensors sequentially:

```text
Read Slot 1
    ↓
Read Slot 2
    ↓
Read Slot 3
    ↓
Read Slot 4
```

This reduces possible ultrasonic cross-talk.

For physical installation, sensor spacing and timing should be validated experimentally.

---

# 16. Sensor Placement

For a real parking installation:

```text
        HC-SR04
           │
           │ Ultrasonic Beam
           ▼
     ┌────────────┐
     │   Vehicle  │
     └────────────┘
           │
           ▼
      Parking Slot
```

Recommended practices:

- Mount sensors at consistent heights.
- Point sensors toward the bumper/vehicle detection region.
- Keep sensor orientations consistent.
- Avoid physical obstructions.
- Calibrate the threshold after installation.

The current virtual prototype uses a **35 cm threshold**.

---

# 17. Pin Assignment Rationale

The GPIO allocation was selected to provide:

- Separate trigger pins for all ultrasonic sensors
- Separate echo inputs
- Dedicated I²C pins for the OLED
- A dedicated PWM pin for the servo
- A dedicated buzzer output
- Two dedicated LED outputs per slot

This produces a deterministic and easy-to-debug hardware architecture.

---

# 18. Production Deployment Considerations

The following should be reviewed before physical deployment:

- GPIO electrical limits
- HC-SR04 ECHO voltage compatibility
- Servo current requirements
- Power-supply capacity
- Common-ground arrangement
- EMI/noise
- Waterproofing where required
- Sensor mounting
- Threshold calibration
- Mechanical gate safety
- Emergency/manual gate operation

---

# 19. Future Hardware Extensions

Potential hardware additions include:

```text
Entry Sensor
Exit Sensor
RFID Reader
IR Vehicle Sensor
Barrier Safety Sensor
RGB LED Strip
Larger Display
Additional Parking Sensors
Camera
Environmental Sensor
```

A production-oriented extension could be:

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

# 20. Hardware Verification Checklist

Before connecting real hardware:

```text
[ ] ESP32 board verified
[ ] Sensor pin mapping verified
[ ] OLED I²C address verified
[ ] LED resistors installed
[ ] Buzzer wiring verified
[ ] Servo power verified
[ ] Common ground verified
[ ] HC-SR04 ECHO voltage checked
[ ] Power supply checked
[ ] No GPIO conflicts
[ ] Sensor placement planned
[ ] Detection threshold calibrated
```

---

# 21. Virtual vs Physical Pin Mapping

The same logical GPIO mapping is used in the Wokwi prototype and intended physical design.

| Function | Virtual GPIO | Physical Implementation |
|---|---:|---|
| Slot 1 TRIG | 5 | GPIO 5 |
| Slot 1 ECHO | 17 | GPIO 17 |
| Slot 2 TRIG | 16 | GPIO 16 |
| Slot 2 ECHO | 4 | GPIO 4 |
| Slot 3 TRIG | 27 | GPIO 27 |
| Slot 3 ECHO | 26 | GPIO 26 |
| Slot 4 TRIG | 25 | GPIO 25 |
| Slot 4 ECHO | 35 | GPIO 35 |
| OLED SDA | 21 | GPIO 21 |
| OLED SCL | 22 | GPIO 22 |
| Servo | 18 | GPIO 18 |
| Buzzer | 19 | GPIO 19 |

Physical implementation still requires electrical verification before powering the circuit.

---

# 22. Final Pin Mapping Summary

```text
ESP32 DEVKIT
│
├── GPIO 5  → S1 TRIG
├── GPIO 17 → S1 ECHO
│
├── GPIO 16 → S2 TRIG
├── GPIO 4  → S2 ECHO
│
├── GPIO 27 → S3 TRIG
├── GPIO 26 → S3 ECHO
│
├── GPIO 25 → S4 TRIG
├── GPIO 35 → S4 ECHO
│
├── GPIO 21 → OLED SDA
├── GPIO 22 → OLED SCL
│
├── GPIO 18 → Servo PWM
├── GPIO 19 → Buzzer
│
├── GPIO 12 → S1 Green
├── GPIO 2  → S1 Red
│
├── GPIO 14 → S2 Green
├── GPIO 13 → S2 Red
│
├── GPIO 32 → S3 Green
├── GPIO 23 → S3 Red
│
├── GPIO 15 → S4 Green
└── GPIO 33 → S4 Red
```

---

# 23. Implementation Reference

Main firmware:

```text
src/main.cpp
```

Architecture:

```text
docs/architecture.md
```

Hardware details:

```text
docs/hardware.md
```

Algorithm:

```text
docs/algorithm.md
```

Simulation:

```text
docs/simulation.md
```

Testing:

```text
docs/testing.md
```

---

# 24. Final Specification

```text
Controller        : ESP32 DevKit
Slots             : 4
Ultrasonic        : HC-SR04 × 4
OLED              : SSD1306 128×64
Green LEDs        : 4
Red LEDs          : 4
Buzzer            : 1
Servo             : SG90
Threshold         : 35 cm
I²C SDA           : GPIO 21
I²C SCL           : GPIO 22
Servo PWM         : GPIO 18
Buzzer            : GPIO 19
Wi-Fi             : ESP32
Dashboard         : HTTP
Simulation        : Wokwi
Build             : PlatformIO
Project Status    : Functional Virtual Prototype
Physical Testing  : Not performed
```

---

# 25. Summary

The pin allocation provides a clear, deterministic interface between the ESP32 and every subsystem of the Smart Parking System.

The complete hardware signal flow is:

```text
HC-SR04 Sensors
       ↓
ESP32 GPIO
       ↓
Distance Processing
       ↓
Parking Decision
       ↓
OLED + LEDs + Buzzer
       ↓
Servo Gate
       ↓
Wi-Fi Web Dashboard
```

This mapping is intended to be the single reference for the project's firmware, Wokwi circuit, documentation, testing, and future physical implementation.
