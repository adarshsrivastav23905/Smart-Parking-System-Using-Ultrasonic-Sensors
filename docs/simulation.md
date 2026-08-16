# 🧪 Smart Parking System — Simulation Guide

## Simulation Platform

The project is simulated using:

**Wokwi**

Development environment:

**Visual Studio Code + PlatformIO**

---

## Simulation Components

```text
ESP32 DevKit
4 × HC-SR04
SSD1306 OLED
4 × Green LEDs
4 × Red LEDs
8 × 220 Ω resistors
Buzzer
SG90 Servo# 🧪 Smart Parking System — Simulation Guide

## 1. Simulation Overview

The **Smart Parking System Using Ultrasonic Sensors** is implemented as a complete virtual embedded-system prototype using **Wokwi** and **PlatformIO**.

The simulation reproduces the main behavior of the intended physical system without requiring physical hardware.

The virtual system contains:

- ESP32 DevKit
- Four HC-SR04 ultrasonic sensors
- SSD1306 OLED
- Four green LEDs
- Four red LEDs
- Buzzer
- SG90 servo
- ESP32 Wi-Fi
- HTTP web dashboard

The simulation is used to validate:

- Sensor interfacing
- Distance measurement
- Parking-slot classification
- Available-slot counting
- OLED output
- LED indication
- Buzzer alert
- Servo gate behavior
- Web dashboard
- System integration

---

# 2. Simulation Objectives

The Wokwi simulation is intended to verify that:

1. Four parking slots can be monitored.
2. HC-SR04 distance values are read correctly.
3. Distances below 35 cm are classified as OCCUPIED.
4. Distances of 35 cm or greater are classified as FREE.
5. Available slots are calculated correctly.
6. OLED output matches the parking state.
7. Green/red LEDs indicate individual slot states.
8. The buzzer activates when parking is full.
9. The servo closes when all slots are occupied.
10. The servo reopens when at least one slot becomes available.
11. The ESP32 web dashboard matches the embedded state.
12. The complete firmware builds successfully using PlatformIO.

---

# 3. Simulation Architecture

```text
                         WOKWI SIMULATION
                                │
                                ▼
                       ┌─────────────────┐
                       │    ESP32        │
                       │   Controller    │
                       └────────┬────────┘
                                │
          ┌─────────────────────┼─────────────────────┐
          │                     │                     │
          ▼                     ▼                     ▼
   HC-SR04 × 4               OLED                 Outputs
   Slot Sensors                                  │
          │                              ┌───────┼───────┐
          │                              │       │       │
          ▼                              ▼       ▼       ▼
   Distance Input                      LEDs   Buzzer   Servo
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

---

# 4. Simulation Components

| Component | Quantity | Simulation Role |
|---|---:|---|
| ESP32 DevKit | 1 | Main controller |
| HC-SR04 | 4 | Parking-slot sensing |
| SSD1306 OLED | 1 | Local display |
| Green LED | 4 | FREE indication |
| Red LED | 4 | OCCUPIED indication |
| Resistor | 8 | LED current limiting |
| Buzzer | 1 | Parking-full alert |
| SG90 Servo | 1 | Capacity-based gate |
| Wokwi | — | Virtual hardware platform |

---

# 5. Wokwi Project Files

The simulation is controlled by the Wokwi configuration files.

Depending on the final repository structure, the authoritative simulation files should be stored together.

Recommended structure:

```text
simulation/
├── diagram.json
├── wokwi.toml
└── README.md
```

### Important

Do not maintain multiple competing copies of `diagram.json` or `wokwi.toml`.

Use the copies that are actually connected to the working Wokwi project.

---

# 6. Circuit Configuration

The virtual circuit contains:

```text
ESP32
   │
   ├── HC-SR04 Slot 1
   ├── HC-SR04 Slot 2
   ├── HC-SR04 Slot 3
   ├── HC-SR04 Slot 4
   │
   ├── SSD1306 OLED
   ├── Green/Red LEDs
   ├── Buzzer
   └── SG90 Servo
```

The final circuit intentionally does not include:

```text
Entry Button
Entry Sensor
Exit Button
```

The servo therefore represents **capacity-based gate control**.

---

# 7. Final GPIO Mapping

| Function | GPIO |
|---|---:|
| Slot 1 TRIG | 5 |
| Slot 1 ECHO | 17 |
| Slot 2 TRIG | 16 |
| Slot 2 ECHO | 4 |
| Slot 3 TRIG | 27 |
| Slot 3 ECHO | 26 |
| Slot 4 TRIG | 25 |
| Slot 4 ECHO | 35 |
| OLED SDA | 21 |
| OLED SCL | 22 |
| Servo PWM | 18 |
| Buzzer | 19 |
| Slot 1 Green | 12 |
| Slot 1 Red | 2 |
| Slot 2 Green | 14 |
| Slot 2 Red | 13 |
| Slot 3 Green | 32 |
| Slot 3 Red | 23 |
| Slot 4 Green | 15 |
| Slot 4 Red | 33 |

For detailed hardware information, see:

```text
docs/hardware.md
docs/pin_mapping.md
```

---

# 8. Detection Threshold

The virtual prototype uses:

```text
35 cm
```

Decision logic:

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

---

# 9. Starting the Simulation

## Step 1 — Open the project

Open:

```text
Smart-Parking-Ultrasonic-Sensor-System
```

in Visual Studio Code.

---

## Step 2 — Verify PlatformIO

Make sure the PlatformIO extension is installed.

The project should contain:

```text
platformio.ini
```

and:

```text
src/main.cpp
```

---

## Step 3 — Build the firmware

Use:

```text
PlatformIO
→ Project Tasks
→ esp32dev
→ General
→ Build
```

Expected:

```text
SUCCESS
```

---

## Step 4 — Start Wokwi

Open the Wokwi simulation using the project's configured simulation setup.

Start the simulation.

---

## Step 5 — Wait for initialization

The Serial Monitor should show startup information such as:

```text
SMART PARKING SYSTEM
System starting...
OLED initialized.
Web server started.
System ready.
```

Exact output may vary according to the current firmware.

---

# 10. Sensor Distance Control

Each Wokwi HC-SR04 allows the simulated distance to be changed.

For example:

```text
Slot 1 = 80 cm
Slot 2 = 80 cm
Slot 3 = 80 cm
Slot 4 = 80 cm
```

represents four free parking slots.

Reducing a sensor distance below the threshold simulates a vehicle occupying that parking slot.

Example:

```text
Slot 1 = 20 cm
```

represents:

```text
Slot 1 → OCCUPIED
```

---

# 11. Simulation Test Sequence

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

Expected available-slot sequence:

```text
4 → 3 → 2 → 1 → 0 → 1
```

This sequence demonstrates both occupancy detection and slot release.

---

# 12. Test 1 — All Slots Free

Set:

```text
S1 = 80 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

Expected:

```text
S1 → FREE
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots → 4/4
Gate → OPEN
Buzzer → OFF
Green LEDs → ON
Red LEDs → OFF
```

Recommended screenshot:

```text
screenshots/04_all_slots_free.png
```

---

# 13. Test 2 — One Slot Occupied

Set:

```text
S1 = 20 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

Expected:

```text
S1 → OCCUPIED
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots → 3/4
```

Recommended screenshot:

```text
screenshots/05_one_slot_occupied.png
```

---

# 14. Test 3 — Two Slots Occupied

Set:

```text
S1 = 20 cm
S2 = 20 cm
S3 = 80 cm
S4 = 80 cm
```

Expected:

```text
Available Slots → 2/4
Occupied Slots → 2/4
Gate → OPEN
Buzzer → OFF
```

Recommended screenshot:

```text
screenshots/06_two_slots_occupied.png
```

---

# 15. Test 4 — Three Slots Occupied

Set:

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 80 cm
```

Expected:

```text
Available Slots → 1/4
Occupied Slots → 3/4
Gate → OPEN
```

Recommended screenshot:

```text
screenshots/07_three_slots_occupied.png
```

---

# 16. Test 5 — Parking Full

Set:

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 20 cm
```

Expected:

```text
Available Slots → 0/4
Occupied Slots → 4/4
System Status → PARKING FULL
Buzzer → ON
Gate → CLOSED
Green LEDs → OFF
Red LEDs → ON
```

Recommended screenshot:

```text
screenshots/08_parking_full.png
```

---

# 17. Test 6 — Slot Becomes Free

Starting from:

```text
20 / 20 / 20 / 20
```

Change Slot 4:

```text
S4 = 80 cm
```

Expected:

```text
S4 → FREE
Available Slots → 1/4
Buzzer → OFF
Gate → OPEN
```

Recommended screenshot:

```text
screenshots/09_servo_reopen.png
```

This is particularly important because it demonstrates the servo changing from CLOSED back to OPEN.

---

# 18. OLED Verification

Use:

```text
20 / 80 / 20 / 80
```

The OLED should display approximately:

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

Verify:

- Distances are visible.
- Slot states are correct.
- Available count is correct.
- Threshold is displayed.
- Gate state is displayed.

Recommended screenshot:

```text
screenshots/10_oled_distance_status.png
```

---

# 19. LED Verification

## Free Slot

Use:

```text
Slot distance >= 35 cm
```

Expected:

```text
Green LED → ON
Red LED → OFF
```

Recommended screenshot:

```text
screenshots/11_green_led.png
```

## Occupied Slot

Use:

```text
Slot distance < 35 cm
```

Expected:

```text
Green LED → OFF
Red LED → ON
```

Recommended screenshot:

```text
screenshots/12_red_led.png
```

---

# 20. Buzzer Verification

Set all slots to an occupied distance:

```text
20 / 20 / 20 / 20
```

Expected:

```text
Available Slots = 0
Parking Full = YES
Buzzer = ON
Gate = CLOSED
```

Recommended screenshot:

```text
screenshots/13_buzzer.png
```

A short screen recording is better than a still image for proving that the buzzer actually produces sound.

---

# 21. Servo Verification

The servo uses capacity-based logic.

## Open Condition

```text
Available Slots > 0
```

Expected:

```text
Servo → OPEN position
```

## Closed Condition

```text
Available Slots = 0
```

Expected:

```text
Servo → CLOSED position
```

## Reopen Condition

Free one slot after full occupancy.

Expected:

```text
0/4 → 1/4
Servo → OPEN
```

Recommended screenshot:

```text
screenshots/09_servo_reopen.png
```

---

# 22. Serial Monitor Verification

The Serial Monitor provides debugging information.

The output should include information similar to:

```text
==============================================
          SMART PARKING SYSTEM
==============================================

Threshold: 35.0 cm

Slot 1 | Distance: XX.X cm | Status: FREE
Slot 2 | Distance: XX.X cm | Status: OCCUPIED
Slot 3 | Distance: XX.X cm | Status: FREE
Slot 4 | Distance: XX.X cm | Status: FREE

Available Slots: 3/4
Occupied Slots: 1/4
STATUS: SPACE AVAILABLE
GATE: OPEN
```

Only save actual captured serial output to:

```text
outputs/serial_output.txt
```

Do not fabricate sensor values.

---

# 23. Web Dashboard

The ESP32 hosts an HTTP dashboard.

Open:

```text
http://localhost:8180
```

The dashboard displays:

```text
Available Slots
Occupied Slots
Slot 1 Status
Slot 2 Status
Slot 3 Status
Slot 4 Status
Distance Values
Detection Threshold
System Status
Gate Status
```

---

# 24. Web Dashboard Test — All Free

Set:

```text
80 / 80 / 80 / 80
```

Expected:

```text
Available Slots: 4/4
Occupied Slots: 0/4
System Status: SPACE AVAILABLE
Gate: OPEN
```

Screenshot:

```text
screenshots/14_web_dashboard_all_free.png
```

---

# 25. Web Dashboard Test — Mixed State

Set:

```text
20 / 80 / 20 / 80
```

Expected:

```text
Available Slots: 2/4
Occupied Slots: 2/4

Slot 1 → OCCUPIED
Slot 2 → FREE
Slot 3 → OCCUPIED
Slot 4 → FREE
```

Screenshot:

```text
screenshots/15_web_dashboard_mixed.png
```

---

# 26. Web Dashboard Test — Full

Set:

```text
20 / 20 / 20 / 20
```

Expected:

```text
Available Slots: 0/4
Occupied Slots: 4/4
PARKING FULL
Gate: CLOSED
```

Screenshot:

```text
screenshots/16_web_dashboard_full.png
```

---

# 27. End-to-End Demonstration

For the strongest project demonstration, perform:

```text
STEP 1
80 / 80 / 80 / 80

        ↓

STEP 2
20 / 80 / 80 / 80

        ↓

STEP 3
20 / 20 / 80 / 80

        ↓

STEP 4
20 / 20 / 20 / 80

        ↓

STEP 5
20 / 20 / 20 / 20

        ↓

STEP 6
20 / 20 / 20 / 80
```

Expected available count:

```text
4 → 3 → 2 → 1 → 0 → 1
```

Expected gate behavior:

```text
OPEN
OPEN
OPEN
OPEN
CLOSED
OPEN
```

Expected buzzer behavior:

```text
OFF
OFF
OFF
OFF
ON
OFF
```

This is the best single demonstration sequence for a project presentation or portfolio video.

---

# 28. Simulation Validation Matrix

| Scenario | Input | Available | Gate | Buzzer |
|---|---|---:|---|---|
| All free | 80/80/80/80 | 4/4 | OPEN | OFF |
| One occupied | 20/80/80/80 | 3/4 | OPEN | OFF |
| Two occupied | 20/20/80/80 | 2/4 | OPEN | OFF |
| Three occupied | 20/20/20/80 | 1/4 | OPEN | OFF |
| Full | 20/20/20/20 | 0/4 | CLOSED | ON |
| Slot released | 20/20/20/80 | 1/4 | OPEN | OFF |

---

# 29. Simulation Performance Checks

The following should be verified:

### Sensor Response

Sensor state should update after the configured confirmation period.

### OLED

OLED should update without becoming unresponsive.

### LEDs

LED states should correspond to the current parking state.

### Buzzer

The buzzer should activate only for the parking-full condition.

### Servo

The servo should move only when the required gate state changes.

### Web Dashboard

The dashboard should reflect the latest available-slot count.

---

# 30. Simulation Limitations

Wokwi is a powerful virtual hardware environment, but simulation does not reproduce every physical condition.

The simulation does not fully represent:

- Real ultrasonic reflections
- Physical sensor tolerances
- Electrical noise
- Power-supply variations
- Mechanical servo load
- Environmental conditions
- Real vehicle geometry
- Physical wiring faults

Therefore:

> Successful Wokwi validation demonstrates functional firmware and system logic, but it does not replace physical hardware validation.

---

# 31. Physical Hardware Transition

After successful virtual validation, the next stage would be:

```text
Wokwi Prototype
      ↓
Purchase / Assemble Hardware
      ↓
Build Physical Circuit
      ↓
Verify GPIO Connections
      ↓
Check Voltage Levels
      ↓
Calibrate Ultrasonic Sensors
      ↓
Tune Detection Threshold
      ↓
Test Servo Mechanically
      ↓
Perform Physical System Tests
```

---

# 32. Physical Deployment Considerations

Before connecting actual hardware:

- Verify ESP32 GPIO voltage limits.
- Check HC-SR04 ECHO compatibility.
- Use level shifting where required.
- Provide suitable servo power.
- Maintain a common ground.
- Use LED current-limiting resistors.
- Calibrate the 35 cm threshold.
- Test each sensor independently.
- Avoid simultaneous ultrasonic triggering.
- Verify mechanical gate movement.
- Provide safe manual override if used in a real barrier system.

---

# 33. Wokwi Files

The simulation depends on:

```text
diagram.json
wokwi.toml
```

If stored inside this directory:

```text
simulation/
├── diagram.json
├── wokwi.toml
└── README.md
```

make sure the paths in `wokwi.toml` correctly reference the compiled firmware.

Example structure:

```toml
[wokwi]
version = 1
firmware = "../.pio/build/esp32dev/firmware.bin"
elf = "../.pio/build/esp32dev/firmware.elf"
rfc2217ServerPort = 4001

[[net.forward]]
from = "localhost:8180"
to = "target:80"
```

Use the exact paths from the currently working project rather than maintaining duplicate configurations.

---

# 34. Simulation Troubleshooting

## Simulation Does Not Start

Check:

- Wokwi configuration file exists.
- Compiled firmware exists.
- PlatformIO build succeeded.
- `diagram.json` is valid.
- ESP32 board type is correct.

## OLED Not Working

Check:

```text
SDA → GPIO 21
SCL → GPIO 22
Address → 0x3C
```

## Servo Not Moving

Check:

```text
PWM → GPIO 18
V+ → 5V
GND → GND
```

Also verify that the parking state actually changes between:

```text
Available > 0
```

and:

```text
Available = 0
```

## Slot Not Changing

Check the simulated sensor distance.

Remember:

```text
< 35 cm → OCCUPIED
>= 35 cm → FREE
```

## Dashboard Not Opening

Check:

```text
Wokwi running
Wi-Fi initialization
Web server started
Port forwarding
```

Then open:

```text
http://localhost:8180
```

---

# 35. Simulation Evidence Checklist

Capture:

```text
[ ] Project structure
[ ] PlatformIO build success
[ ] Complete Wokwi circuit
[ ] All slots free
[ ] One slot occupied
[ ] Two slots occupied
[ ] Three slots occupied
[ ] Parking full
[ ] Servo reopening
[ ] OLED distance/status
[ ] Green LED
[ ] Red LED
[ ] Buzzer
[ ] Web dashboard — all free
[ ] Web dashboard — mixed
[ ] Web dashboard — full
```

Recommended filenames:

```text
01_project_structure.png
02_platformio_build_success.png
03_complete_wokwi_circuit.png
04_all_slots_free.png
05_one_slot_occupied.png
06_two_slots_occupied.png
07_three_slots_occupied.png
08_parking_full.png
09_servo_reopen.png
10_oled_distance_status.png
11_green_led.png
12_red_led.png
13_buzzer.png
14_web_dashboard_all_free.png
15_web_dashboard_mixed.png
16_web_dashboard_full.png
```

---

# 36. Validation Against Test Cases

The detailed test plan is maintained in:

```text
test_cases/README.md
```

The test data file is:

```text
test_cases/test_cases.csv
```

The testing strategy is documented in:

```text
docs/testing.md
```

---

# 37. Simulation Workflow

The complete development workflow is:

```text
Design Circuit
      ↓
Create Wokwi Diagram
      ↓
Configure PlatformIO
      ↓
Write Firmware
      ↓
Build Firmware
      ↓
Start Wokwi
      ↓
Test Sensors
      ↓
Test Slot Logic
      ↓
Test OLED
      ↓
Test LEDs
      ↓
Test Buzzer
      ↓
Test Servo
      ↓
Test Web Dashboard
      ↓
Capture Evidence
      ↓
Document Results
```

---

# 38. Recommended Portfolio Demonstration

For a GitHub or LinkedIn demonstration, show a short progression:

```text
4/4 FREE
   ↓
3/4 FREE
   ↓
2/4 FREE
   ↓
1/4 FREE
   ↓
0/4 FREE
   ↓
PARKING FULL
   ↓
1/4 FREE
   ↓
GATE REOPEN
```

At the same time show:

- Wokwi circuit
- OLED
- Servo
- Web dashboard

This demonstrates that a sensor-state change propagates through the complete embedded system.

---

# 39. Simulation Acceptance Criteria

The virtual prototype is considered successfully validated when:

- Four HC-SR04 sensors respond.
- Distances are processed correctly.
- Distances below 35 cm are classified as OCCUPIED.
- Distances at or above 35 cm are classified as FREE.
- Available-slot count is correct.
- OLED matches the slot states.
- LEDs match the slot states.
- Buzzer activates when all slots are occupied.
- Servo closes when the parking is full.
- Servo reopens when a slot becomes available.
- Web dashboard matches the embedded state.
- PlatformIO build completes successfully.
- Evidence screenshots are captured.

---

# 40. Final Simulation Specification

```text
Simulation Platform : Wokwi
Controller          : ESP32 DevKit
Parking Slots       : 4
Sensors             : HC-SR04 × 4
Display             : SSD1306 OLED 128×64
Green LEDs          : 4
Red LEDs             : 4
Buzzer              : 1
Servo               : SG90
Threshold           : 35 cm
Connectivity        : ESP32 Wi-Fi
Dashboard           : HTTP
Build Environment   : PlatformIO
Firmware            : Embedded C/C++
Validation          : Virtual Simulation
Physical Testing    : Not performed
Project Status      : Functional Virtual Prototype
```

---

# 41. Related Documentation

```text
docs/architecture.md
docs/algorithm.md
docs/hardware.md
docs/pin_mapping.md
docs/testing.md
docs/troubleshooting.md
test_cases/README.md
```

---

# 42. Final Note

The Wokwi simulation provides a reproducible environment for demonstrating and validating the Smart Parking System.

It allows the complete workflow to be observed:

```text
Sensor Input
     ↓
Distance Measurement
     ↓
Parking Decision
     ↓
Availability Count
     ↓
OLED / LEDs
     ↓
Buzzer / Servo
     ↓
ESP32 Web Dashboard
```

The simulation should be treated as a **functional development and validation environment**, while physical deployment would require additional electrical, mechanical, calibration, environmental, and safety testing.
