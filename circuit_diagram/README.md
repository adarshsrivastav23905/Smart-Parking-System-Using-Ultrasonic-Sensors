# 🔌 Smart Parking System — Circuit Diagram 

## 1. Overview

This document describes the circuit design and electrical connections for the **Smart Parking System Using Ultrasonic Sensors**.

The current prototype is designed around:

- ESP32 DevKit
- 4 × HC-SR04 ultrasonic sensors
- SSD1306 OLED 128×64
- 4 × green LEDs
- 4 × red LEDs
- 8 × 220 Ω resistors
- Buzzer
- SG90 servo
- Common ground
- Wokwi virtual simulation

The circuit is designed to monitor four individual parking slots and provide visual, audio, actuator, and web-based feedback.

---

## 2. Circuit Architecture

```text
                         ┌──────────────────┐
                         │    ESP32 DevKit  │
                         │   Main Controller│
                         └────────┬─────────┘
                                  │
             ┌────────────────────┼────────────────────┐
             │                    │                    │
             ▼                    ▼                    ▼
     ┌───────────────┐      ┌────────────┐      ┌───────────────┐
     │ HC-SR04 × 4   │      │ SSD1306    │      │ Output Layer  │
     │ Slot Sensors  │      │ OLED       │      │               │
     └───────┬───────┘      └─────┬──────┘      │ LEDs          │
             │                    │              │ Buzzer        │
             │                    │              │ Servo         │
             ▼                    ▼              └───────────────┘
       Distance Data          Local Status
             │
             ▼
      Parking Logic
             │
             ▼
      FREE / OCCUPIED
             │
             ▼
      Available Count
             │
             ▼
        Web Dashboard
```

---

## 3. Parking Slot Structure

Each parking slot is represented by:

```text
1 × HC-SR04
1 × Green LED
1 × Red LED
2 × GPIO connections for the sensor
2 × GPIO connections for status LEDs
```

### Slot behavior

```text
                 SLOT STATUS
                     │
             ┌───────┴───────┐
             │               │
           FREE           OCCUPIED
             │               │
             ▼               ▼
       Green LED ON      Red LED ON
       Red LED OFF       Green LED OFF
```

---

## 4. Component List

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 DevKit | 1 | Main microcontroller |
| HC-SR04 | 4 | Parking-slot distance sensing |
| SSD1306 OLED 128×64 | 1 | Local parking display |
| Green LED | 4 | FREE indication |
| Red LED | 4 | OCCUPIED indication |
| 220 Ω resistor | 8 | LED current limiting |
| Buzzer | 1 | Parking-full alert |
| SG90 Servo | 1 | Capacity-based gate |
| Wokwi | — | Virtual circuit simulation |

---

## 5. ESP32 Connections

The ESP32 is the central controller.

### Ultrasonic sensors

```text
Slot 1:
TRIG → GPIO 5
ECHO → GPIO 17

Slot 2:
TRIG → GPIO 16
ECHO → GPIO 4

Slot 3:
TRIG → GPIO 27
ECHO → GPIO 26

Slot 4:
TRIG → GPIO 25
ECHO → GPIO 35
```

### OLED

```text
SDA → GPIO 21
SCL → GPIO 22
```

### Servo

```text
PWM → GPIO 18
```

### Buzzer

```text
Signal → GPIO 19
```

### LEDs

```text
Slot 1 Green → GPIO 12
Slot 1 Red   → GPIO 2

Slot 2 Green → GPIO 14
Slot 2 Red   → GPIO 13

Slot 3 Green → GPIO 32
Slot 3 Red   → GPIO 23

Slot 4 Green → GPIO 15
Slot 4 Red   → GPIO 33
```

---

# 6. HC-SR04 Wiring

Each HC-SR04 has:

```text
VCC
GND
TRIG
ECHO
```

## Slot 1

```text
HC-SR04 VCC  → ESP32 5V
HC-SR04 GND  → ESP32 GND
HC-SR04 TRIG → GPIO 5
HC-SR04 ECHO → GPIO 17
```

## Slot 2

```text
HC-SR04 VCC  → ESP32 5V
HC-SR04 GND  → ESP32 GND
HC-SR04 TRIG → GPIO 16
HC-SR04 ECHO → GPIO 4
```

## Slot 3

```text
HC-SR04 VCC  → ESP32 5V
HC-SR04 GND  → ESP32 GND
HC-SR04 TRIG → GPIO 27
HC-SR04 ECHO → GPIO 26
```

## Slot 4

```text
HC-SR04 VCC  → ESP32 5V
HC-SR04 GND  → ESP32 GND
HC-SR04 TRIG → GPIO 25
HC-SR04 ECHO → GPIO 35
```

---

# 7. OLED Wiring

The SSD1306 OLED uses I²C communication.

```text
OLED VCC → 3.3V
OLED GND → GND
OLED SDA → GPIO 21
OLED SCL → GPIO 22
```

### OLED Address

The current prototype uses:

```text
0x3C
```

### OLED Function

The display provides:

- Slot distance
- FREE/OCCUPIED state
- Available slot count
- Detection threshold
- Gate state

---

# 8. LED Circuit

Each LED is connected through a current-limiting resistor.

## Slot 1

```text
GPIO 12
   │
   ▼
220 Ω resistor
   │
   ▼
Green LED
   │
   ▼
GND
```

```text
GPIO 2
   │
   ▼
220 Ω resistor
   │
   ▼
Red LED
   │
   ▼
GND
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

---

# 9. Buzzer Wiring

The buzzer provides an audible parking-full warning.

```text
Buzzer Signal → GPIO 19
Buzzer GND    → GND
```

The buzzer is activated when:

```text
Available Slots = 0
```

System response:

```text
PARKING FULL
      ↓
Buzzer ON
      ↓
Gate CLOSED
```

---

# 10. Servo Wiring

The SG90 servo is used as a capacity-based parking gate.

```text
Servo PWM → GPIO 18
Servo V+  → 5V
Servo GND → GND
```

### Gate logic

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

> The final circuit intentionally does not contain a dedicated vehicle-entry sensor or button. The servo therefore represents capacity-based gate status rather than physical vehicle arrival detection.

---

# 11. Power Architecture

The virtual circuit uses the appropriate simulated supply connections.

For physical implementation:

```text
                POWER
                  │
        ┌─────────┴─────────┐
        │                   │
       5V                 3.3V
        │                   │
        ▼                   ▼
 HC-SR04 / Servo        OLED / Logic
```

All components share a common ground:

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

---

# 12. Complete Connection Table

| Component | Connection | ESP32 / Supply |
|---|---|---|
| Slot 1 HC-SR04 | TRIG | GPIO 5 |
| Slot 1 HC-SR04 | ECHO | GPIO 17 |
| Slot 1 HC-SR04 | VCC | 5V |
| Slot 1 HC-SR04 | GND | GND |
| Slot 2 HC-SR04 | TRIG | GPIO 16 |
| Slot 2 HC-SR04 | ECHO | GPIO 4 |
| Slot 2 HC-SR04 | VCC | 5V |
| Slot 2 HC-SR04 | GND | GND |
| Slot 3 HC-SR04 | TRIG | GPIO 27 |
| Slot 3 HC-SR04 | ECHO | GPIO 26 |
| Slot 3 HC-SR04 | VCC | 5V |
| Slot 3 HC-SR04 | GND | GND |
| Slot 4 HC-SR04 | TRIG | GPIO 25 |
| Slot 4 HC-SR04 | ECHO | GPIO 35 |
| Slot 4 HC-SR04 | VCC | 5V |
| Slot 4 HC-SR04 | GND | GND |
| OLED | SDA | GPIO 21 |
| OLED | SCL | GPIO 22 |
| OLED | VCC | 3.3V |
| OLED | GND | GND |
| Servo | PWM | GPIO 18 |
| Servo | V+ | 5V |
| Servo | GND | GND |
| Buzzer | Signal | GPIO 19 |
| Buzzer | GND | GND |
| Slot 1 Green LED | Control | GPIO 12 |
| Slot 1 Red LED | Control | GPIO 2 |
| Slot 2 Green LED | Control | GPIO 14 |
| Slot 2 Red LED | Control | GPIO 13 |
| Slot 3 Green LED | Control | GPIO 32 |
| Slot 3 Red LED | Control | GPIO 23 |
| Slot 4 Green LED | Control | GPIO 15 |
| Slot 4 Red LED | Control | GPIO 33 |

---

# 13. Circuit Logic

The complete circuit follows:

```text
HC-SR04 Sensors
       ↓
ESP32
       ↓
Distance Calculation
       ↓
FREE / OCCUPIED
       ↓
Available Slot Count
       │
       ├───────────┬────────────┬───────────┐
       ▼           ▼            ▼           ▼
      OLED        LEDs        Buzzer      Servo
       │           │            │           │
       └───────────┴────────────┴───────────┘
                         │
                         ▼
                    Web Dashboard
```

---

# 14. Parking-State Circuit Behavior

## Four Slots Free

```text
S1 → FREE
S2 → FREE
S3 → FREE
S4 → FREE

Green LEDs → ON
Red LEDs → OFF
Buzzer → OFF
Servo → OPEN
```

## Two Slots Occupied

```text
S1 → OCCUPIED
S2 → FREE
S3 → OCCUPIED
S4 → FREE

S1 Red → ON
S2 Green → ON
S3 Red → ON
S4 Green → ON

Available → 2/4
Servo → OPEN
Buzzer → OFF
```

## Parking Full

```text
S1 → OCCUPIED
S2 → OCCUPIED
S3 → OCCUPIED
S4 → OCCUPIED

Green LEDs → OFF
Red LEDs → ON
Buzzer → ON
Servo → CLOSED
Available → 0/4
```

---

# 15. Sensor Placement Concept

For a physical implementation:

```text
           HC-SR04
              │
              │ Ultrasonic beam
              ▼
        ┌─────────────┐
        │   Vehicle   │
        └─────────────┘
              │
              ▼
        Parking Slot
```

Each ultrasonic sensor should be mounted so that it reliably detects the expected vehicle/object region.

The current virtual prototype uses:

```text
Threshold = 35 cm
```

Physical installation requires calibration.

---

# 16. Ultrasonic Sensor Separation

Because four ultrasonic sensors are used, the design should avoid simultaneous triggering.

The firmware processes sensors sequentially:

```text
Slot 1
  ↓
Slot 2
  ↓
Slot 3
  ↓
Slot 4
```

This reduces possible ultrasonic cross-talk.

Physical sensor spacing and timing should be validated during hardware deployment.

---

# 17. Physical Hardware Safety

Before using real hardware:

### HC-SR04 ECHO

Some HC-SR04 modules can produce approximately 5V ECHO logic.

ESP32 GPIO compatibility must be verified.

Use:

- Resistor divider
- Logic-level shifter
- Other suitable interface

where required.

### Servo

Do not power a high-current physical servo directly from an ESP32 GPIO.

Use an appropriate power source and common ground.

### LEDs

Use current-limiting resistors.

### Power

Use regulated supplies and verify polarity before connection.

---

# 18. Wokwi Simulation

The circuit is currently validated virtually using Wokwi.

Wokwi allows:

- ESP32 simulation
- HC-SR04 distance adjustment
- OLED visualization
- LED behavior
- Buzzer simulation
- Servo movement
- Wi-Fi/web-dashboard testing

The simulation configuration should use a single authoritative set of:

```text
diagram.json
wokwi.toml
```

Avoid keeping multiple conflicting versions of these files.

---

# 19. Recommended Circuit Evidence

Capture the following screenshots:

### Complete Circuit

```text
screenshots/03_complete_wokwi_circuit.png
```

The complete screenshot should clearly show:

- ESP32
- Four HC-SR04 sensors
- OLED
- LEDs
- Buzzer
- Servo

### Parking States

```text
screenshots/04_all_slots_free.png
screenshots/05_one_slot_occupied.png
screenshots/06_two_slots_occupied.png
screenshots/07_three_slots_occupied.png
screenshots/08_parking_full.png
```

### Outputs

```text
screenshots/09_servo_reopen.png
screenshots/10_oled_distance_status.png
screenshots/11_green_led.png
screenshots/12_red_led.png
screenshots/13_buzzer.png
```

### Web Dashboard

```text
screenshots/14_web_dashboard_all_free.png
screenshots/15_web_dashboard_mixed.png
screenshots/16_web_dashboard_full.png
```

---

# 20. Circuit Verification Checklist

Before considering the circuit complete:

```text
[ ] ESP32 connected
[ ] Slot 1 HC-SR04 connected
[ ] Slot 2 HC-SR04 connected
[ ] Slot 3 HC-SR04 connected
[ ] Slot 4 HC-SR04 connected
[ ] OLED SDA connected
[ ] OLED SCL connected
[ ] Four green LEDs connected
[ ] Four red LEDs connected
[ ] Eight LED resistors present
[ ] Buzzer connected
[ ] Servo connected
[ ] Common GND verified
[ ] GPIO mapping matches firmware
[ ] No entry button present
[ ] No entry sensor present
[ ] Wokwi simulation starts
[ ] All major outputs tested
```

---

# 21. Repository References

Related circuit documentation:

```text
docs/hardware.md
docs/pin_mapping.md
docs/architecture.md
docs/algorithm.md
docs/simulation.md
docs/testing.md
```

Main virtual circuit:

```text
simulation/diagram.json
```

Simulation configuration:

```text
simulation/wokwi.toml
```

Main firmware:

```text
src/main.cpp
```

---

# 22. Final Circuit Specification

```text
Controller        : ESP32 DevKit
Parking Slots     : 4
Ultrasonic        : HC-SR04 × 4
OLED              : SSD1306 128×64
Green LEDs        : 4
Red LEDs          : 4
LED Resistors     : 220 Ω × 8
Buzzer            : 1
Servo             : SG90
Threshold         : 35 cm
OLED SDA          : GPIO 21
OLED SCL          : GPIO 22
Servo PWM         : GPIO 18
Buzzer            : GPIO 19
Wi-Fi              : ESP32
Dashboard         : HTTP
Simulation        : Wokwi
Build              : PlatformIO
Physical Testing  : Not performed
Project Status    : Functional Virtual Prototype
```

---

# 23. Circuit Summary

The circuit implements a complete embedded parking-monitoring system:

```text
                ┌────────────────┐
                │     ESP32      │
                └───────┬────────┘
                        │
          ┌─────────────┼─────────────┐
          │             │             │
          ▼             ▼             ▼
      HC-SR04 ×4      OLED        Output Devices
          │                           │
          │                     ┌─────┼─────┐
          │                     │     │     │
          ▼                     ▼     ▼     ▼
       Distance              LEDs  Buzzer Servo
          │
          ▼
    Parking Decision
          │
          ▼
    Available Count
          │
          ▼
     Web Dashboard
```

The final circuit therefore provides:

```text
Sensing
   ↓
Processing
   ↓
Local Visualization
   ↓
Alerts
   ↓
Actuation
   ↓
IoT Monitoring
```

The circuit is designed as a modular virtual prototype that can later be transferred to physical hardware after voltage, power, mechanical, and sensor-calibration requirements are verified.
