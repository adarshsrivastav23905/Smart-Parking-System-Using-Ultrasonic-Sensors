# 🧪 Smart Parking System — Testing Strategy

## 1. Purpose

This document defines the testing strategy for the **Smart Parking System Using Ultrasonic Sensors**.

The testing process validates the complete embedded and IoT workflow:

```text
Ultrasonic Sensors
        ↓
Distance Measurement
        ↓
FREE / OCCUPIED Decision
        ↓
Available Slot Count
        ↓
OLED / LEDs / Buzzer
        ↓
Servo Gate
        ↓
ESP32 Web Dashboard
```

The current implementation is validated as a **virtual prototype using Wokwi and PlatformIO** because physical hardware was not available during development.

---

# 2. Testing Objectives

The testing process verifies that the system:

- Reads all four HC-SR04 sensors.
- Calculates distance correctly.
- Classifies parking slots correctly.
- Calculates available slots correctly.
- Updates the OLED correctly.
- Controls green/red LEDs correctly.
- Activates the buzzer when parking is full.
- Controls the servo gate according to parking capacity.
- Updates the ESP32 web dashboard.
- Handles invalid sensor readings safely.
- Initializes correctly after reset.
- Builds successfully using PlatformIO.
- Produces reproducible evidence for GitHub documentation.

---

# 3. Test Environment

| Parameter | Configuration |
|---|---|
| Controller | ESP32 DevKit |
| Simulation | Wokwi |
| Development Environment | VS Code + PlatformIO |
| Parking Slots | 4 |
| Sensor | HC-SR04 × 4 |
| Detection Threshold | 35 cm |
| Display | SSD1306 OLED 128×64 |
| Indicators | 4 Green + 4 Red LEDs |
| Alert | Buzzer |
| Gate | SG90 Servo |
| Connectivity | ESP32 Wi-Fi |
| Web Interface | HTTP Dashboard |
| Validation Type | Virtual Prototype |

---

# 4. Testing Levels

The project uses multiple levels of testing.

## 4.1 Unit Testing

Individual hardware/software modules are verified separately.

Examples:

- Ultrasonic distance measurement
- OLED output
- Green LED
- Red LED
- Buzzer
- Servo

---

## 4.2 Integration Testing

Multiple subsystems are tested together.

```text
Sensor
   ↓
Distance
   ↓
Decision
   ↓
Available Count
   ↓
OLED + LEDs + Buzzer + Servo
```

---

## 4.3 System Testing

The complete parking system is tested in Wokwi.

This includes:

- Four sensors
- OLED
- LEDs
- Buzzer
- Servo
- ESP32 firmware
- Web dashboard

---

## 4.4 Interface Testing

The local embedded state is compared with the web dashboard.

Example:

```text
OLED:
Available = 2/4

Web Dashboard:
Available = 2/4
```

Both should represent the same system state.

---

# 5. Test Inputs

The main virtual test inputs are sensor distance combinations.

```text
80 / 80 / 80 / 80
20 / 80 / 80 / 80
20 / 20 / 80 / 80
20 / 20 / 20 / 80
20 / 20 / 20 / 20
20 / 20 / 20 / 80
```

These represent:

```text
4 free
3 free
2 free
1 free
0 free
1 free after release
```

---

# 6. Detection Logic Under Test

The current detection threshold is:

```text
35 cm
```

Decision rule:

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

# 7. Availability Calculation Under Test

For four parking slots:

```text
Available Slots =
4 - Occupied Slots
```

Example:

```text
Occupied = 2
Available = 4 - 2
Available = 2
```

---

# 8. Functional Test Matrix

| Test ID | Test Scenario | Input | Expected Result | Actual Result | Status |
|---|---|---|---|---|---|
| TC01 | All slots free | 80/80/80/80 cm | 4/4 available | Pending | Pending |
| TC02 | One slot occupied | 20/80/80/80 cm | 3/4 available | Pending | Pending |
| TC03 | Two slots occupied | 20/20/80/80 cm | 2/4 available | Pending | Pending |
| TC04 | Three slots occupied | 20/20/20/80 cm | 1/4 available | Pending | Pending |
| TC05 | Parking full | 20/20/20/20 cm | 0/4, buzzer ON, gate CLOSED | Pending | Pending |
| TC06 | Slot becomes free | 20/20/20/80 cm | 1/4, buzzer OFF, gate OPEN | Pending | Pending |
| TC07 | OLED verification | 20/80/20/80 cm | Correct distance/status/count | Pending | Pending |
| TC08 | Web dashboard | 20/80/20/80 cm | Dashboard matches system | Pending | Pending |
| TC09 | Green LED | Slot ≥35 cm | Green LED ON | Pending | Pending |
| TC10 | Red LED | Slot <35 cm | Red LED ON | Pending | Pending |
| TC11 | Buzzer | 20/20/20/20 cm | Parking-full alert | Pending | Pending |
| TC12 | Servo gate | Available ↔ Full | Gate opens/closes correctly | Pending | Pending |
| TC13 | System reset | Restart simulation | Safe initialization | Pending | Pending |

---

# 9. Detailed Functional Tests

## TC01 — All Slots Free

### Input

```text
S1 = 80 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

### Expected

```text
S1 → FREE
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots = 4/4
Occupied Slots = 0/4
Gate = OPEN
Buzzer = OFF
```

### Evidence

```text
screenshots/04_all_slots_free.png
```

---

## TC02 — One Slot Occupied

### Input

```text
S1 = 20 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

### Expected

```text
S1 → OCCUPIED
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots = 3/4
Occupied Slots = 1/4
```

### Evidence

```text
screenshots/05_one_slot_occupied.png
```

---

## TC03 — Two Slots Occupied

### Input

```text
S1 = 20 cm
S2 = 20 cm
S3 = 80 cm
S4 = 80 cm
```

### Expected

```text
Available Slots = 2/4
Occupied Slots = 2/4
Gate = OPEN
Buzzer = OFF
```

### Evidence

```text
screenshots/06_two_slots_occupied.png
```

---

## TC04 — Three Slots Occupied

### Input

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 80 cm
```

### Expected

```text
Available Slots = 1/4
Occupied Slots = 3/4
Gate = OPEN
```

### Evidence

```text
screenshots/07_three_slots_occupied.png
```

---

## TC05 — Parking Full

### Input

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 20 cm
```

### Expected

```text
Available Slots = 0/4
Occupied Slots = 4/4
System Status = PARKING FULL
Buzzer = ON
Gate = CLOSED
Green LEDs = OFF
Red LEDs = ON
```

### Evidence

```text
screenshots/08_parking_full.png
```

---

## TC06 — Slot Becomes Free

### Initial State

```text
20 / 20 / 20 / 20
```

Then change:

```text
S4 = 80 cm
```

### Expected

```text
S4 → FREE
Available Slots = 1/4
Buzzer = OFF
Gate = OPEN
```

### Evidence

```text
screenshots/09_servo_reopen.png
```

This test verifies both parking-state recovery and servo reopening.

---

# 10. OLED Testing

## TC07 — OLED Verification

### Input

```text
20 / 80 / 20 / 80
```

### Expected

The OLED should show the current sensor values and slot states.

Example:

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

Verify:

- Distances are shown.
- Slot states are correct.
- Available count is correct.
- Threshold is visible.
- Gate status is visible.

### Evidence

```text
screenshots/10_oled_distance_status.png
```

---

# 11. LED Testing

## TC09 — Green LED

### Condition

Use a free-slot distance:

```text
S1 = 80 cm
```

### Expected

```text
S1 Green LED = ON
S1 Red LED   = OFF
```

### Evidence

```text
screenshots/11_green_led.png
```

---

## TC10 — Red LED

### Condition

Use an occupied-slot distance:

```text
S1 = 20 cm
```

### Expected

```text
S1 Green LED = OFF
S1 Red LED   = ON
```

### Evidence

```text
screenshots/12_red_led.png
```

---

# 12. Buzzer Testing

## TC11 — Parking-Full Buzzer

### Input

```text
20 / 20 / 20 / 20
```

### Expected

```text
Available Slots = 0
System Status = PARKING FULL
Buzzer = ON
Gate = CLOSED
```

### Evidence

```text
screenshots/13_buzzer.png
```

A short video or simulation recording is recommended when demonstrating the actual audible alert.

---

# 13. Servo Testing

## TC12 — Capacity-Based Gate

### Test A — Gate Open

Input:

```text
80 / 80 / 80 / 80
```

Expected:

```text
Available > 0
Gate = OPEN
```

### Test B — Gate Close

Change to:

```text
20 / 20 / 20 / 20
```

Expected:

```text
Available = 0
Gate = CLOSED
Buzzer = ON
```

### Test C — Gate Reopen

Change one slot back to:

```text
80 cm
```

Expected:

```text
Available = 1/4
Gate = OPEN
Buzzer = OFF
```

### Scope Note

The servo is capacity-based because the final circuit does not contain a dedicated entry sensor or push button.

It does not claim vehicle-arrival or vehicle-passage detection.

### Evidence

```text
screenshots/09_servo_reopen.png
```

---

# 14. Web Dashboard Testing

## TC08 — Web Dashboard Verification

### Input

```text
S1 = 20 cm
S2 = 80 cm
S3 = 20 cm
S4 = 80 cm
```

Open:

```text
http://localhost:8180
```

### Expected

```text
Available Slots: 2/4
Occupied Slots: 2/4

Slot 1 → OCCUPIED
Slot 2 → FREE
Slot 3 → OCCUPIED
Slot 4 → FREE
```

The dashboard should also reflect:

```text
Threshold = 35 cm
System Status = SPACE AVAILABLE
Gate = OPEN
```

### Evidence

```text
screenshots/15_web_dashboard_mixed.png
```

---

# 15. Web Dashboard State Tests

## All Free

Input:

```text
80 / 80 / 80 / 80
```

Expected:

```text
Available Slots: 4/4
Occupied Slots: 0/4
Gate: OPEN
```

Evidence:

```text
screenshots/14_web_dashboard_all_free.png
```

---

## Mixed State

Input:

```text
20 / 80 / 20 / 80
```

Expected:

```text
Available Slots: 2/4
Occupied Slots: 2/4
```

Evidence:

```text
screenshots/15_web_dashboard_mixed.png
```

---

## Full

Input:

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

Evidence:

```text
screenshots/16_web_dashboard_full.png
```

---

# 16. System Reset Testing

## TC13 — System Reset

### Input

Restart the Wokwi simulation.

### Expected

```text
ESP32 initializes
Ultrasonic sensors initialize
OLED initializes
LEDs initialize
Buzzer initializes
Servo initializes
Wi-Fi initializes
Web server initializes
System becomes ready
```

The system should not remain in an unexpected output state after reset.

---

# 17. Invalid Sensor Reading Test

This test verifies safe behavior when an ultrasonic sensor produces an invalid or timeout reading.

### Condition

Simulate or produce an unavailable echo response.

### Expected

```text
Invalid Reading
      ↓
Ignore Reading
      ↓
Keep Previous Slot State
      ↓
Continue Monitoring
```

The system should not immediately classify a slot as occupied solely because of one invalid measurement.

### Acceptance

- Firmware continues running.
- Other slots continue updating.
- Existing state is preserved.
- No unexpected reset occurs.

---

# 18. Threshold Boundary Testing

The threshold boundary should be explicitly checked.

### Test A

```text
Distance = 34 cm
```

Expected:

```text
OCCUPIED
```

### Test B

```text
Distance = 35 cm
```

Expected:

```text
FREE
```

### Test C

```text
Distance = 36 cm
```

Expected:

```text
FREE
```

This confirms the configured boundary condition.

---

# 19. State Transition Testing

The following transitions should be tested:

```text
FREE → OCCUPIED
```

```text
OCCUPIED → FREE
```

```text
FREE → OCCUPIED → FREE
```

```text
FULL → PARTIAL → FULL
```

Expected behavior:

- State changes only after confirmation.
- LEDs follow the confirmed state.
- Available count changes correctly.
- OLED updates correctly.
- Dashboard updates correctly.
- Servo changes only when required.
- Buzzer changes according to parking capacity.

---

# 20. Multi-Slot Integration Testing

Test the sensors together.

Recommended sequence:

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
```

Expected count:

```text
4 → 3 → 2 → 1 → 0
```

Then release a slot:

```text
20 / 20 / 20 / 80
```

Expected:

```text
0 → 1
```

This is the primary system-integration demonstration.

---

# 21. Performance Testing

The prototype should be checked for:

## Responsiveness

Parking state should update within the intended refresh/confirmation interval.

## Stability

A stable distance should not cause continuous state flickering.

## Servo Stability

The servo should not receive repeated commands when the gate state has not changed.

## Display Stability

The OLED should update without freezing.

## Web Responsiveness

The dashboard should continue serving requests while sensor monitoring is active.

---

# 22. Build Verification

Before running the final validation, build the project using PlatformIO.

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

Save the evidence as:

```text
screenshots/02_platformio_build_success.png
```

---

# 23. Simulation Verification

The Wokwi circuit should contain:

```text
ESP32
4 × HC-SR04
SSD1306 OLED
4 × Green LEDs
4 × Red LEDs
Buzzer
SG90 Servo
```

Verify that the final circuit does not contain the removed:

```text
Entry Button
Entry Sensor
```

---

# 24. Test Evidence Strategy

Every important functional behavior should have evidence.

| Test Area | Evidence |
|---|---|
| Project structure | `01_project_structure.png` |
| Build | `02_platformio_build_success.png` |
| Circuit | `03_complete_wokwi_circuit.png` |
| All free | `04_all_slots_free.png` |
| One occupied | `05_one_slot_occupied.png` |
| Two occupied | `06_two_slots_occupied.png` |
| Three occupied | `07_three_slots_occupied.png` |
| Full parking | `08_parking_full.png` |
| Servo reopen | `09_servo_reopen.png` |
| OLED | `10_oled_distance_status.png` |
| Green LED | `11_green_led.png` |
| Red LED | `12_red_led.png` |
| Buzzer | `13_buzzer.png` |
| Dashboard all free | `14_web_dashboard_all_free.png` |
| Dashboard mixed | `15_web_dashboard_mixed.png` |
| Dashboard full | `16_web_dashboard_full.png` |

---

# 25. Test Result Recording

Detailed individual test results are maintained in:

```text
test_cases/README.md
```

Machine-readable test data can be stored in:

```text
test_cases/test_cases.csv
```

Summary results can be stored in:

```text
outputs/test_results.csv
```

Actual results should be recorded only after observing the behavior in the Wokwi simulation.

---

# 26. Test Result Rules

Use these status values:

```text
PASS
FAIL
PENDING
```

### PASS

Use when the observed result matches the expected result.

### FAIL

Use when the observed result does not match the expected result.

### PENDING

Use before the test has been executed.

Do not mark tests as PASS without evidence.

---

# 27. Test Summary

Complete this table after all tests are executed.

| Category | Total Tests | Passed | Failed | Pending |
|---|---:|---:|---:|---:|
| Slot Detection | 6 | 0 | 0 | 6 |
| OLED | 1 | 0 | 0 | 1 |
| Web Dashboard | 1 | 0 | 0 | 1 |
| LEDs | 2 | 0 | 0 | 2 |
| Buzzer | 1 | 0 | 0 | 1 |
| Servo | 1 | 0 | 0 | 1 |
| Reset | 1 | 0 | 0 | 1 |
| Additional Validation | 3 | 0 | 0 | 3 |
| **TOTAL** | **16** | **0** | **0** | **16** |

The summary should be updated after the actual test run.

---

# 28. Acceptance Criteria

The system is considered functionally validated when:

- [ ] Four HC-SR04 sensors produce valid readings.
- [ ] Distance values below 35 cm are classified as OCCUPIED.
- [ ] Distance values of 35 cm or greater are classified as FREE.
- [ ] Available-slot count matches the detected states.
- [ ] OLED shows the correct slot information.
- [ ] Green LEDs indicate FREE slots.
- [ ] Red LEDs indicate OCCUPIED slots.
- [ ] Buzzer activates when all four slots are occupied.
- [ ] Servo closes when parking becomes full.
- [ ] Servo reopens when a slot becomes available.
- [ ] Web dashboard matches the embedded system state.
- [ ] Invalid readings do not crash the system.
- [ ] Threshold boundary is correct.
- [ ] System resets safely.
- [ ] PlatformIO build succeeds.
- [ ] Evidence screenshots are available for major scenarios.

---

# 29. Test Limitations

The current validation is performed in Wokwi.

Therefore, the following physical factors are not fully validated:

- Real ultrasonic reflection behavior
- Electrical noise
- Power-supply variation
- Actual servo mechanical load
- Real vehicle geometry
- Environmental conditions
- Physical wiring faults
- Long-term sensor reliability

Physical hardware validation would be required before real-world deployment.

---

# 30. Physical Testing Plan

When hardware becomes available, the following additional tests should be performed:

```text
1. Verify power rails
2. Verify GPIO voltage levels
3. Test one HC-SR04
4. Calibrate threshold
5. Test all four sensors
6. Test OLED
7. Test LEDs
8. Test buzzer
9. Test servo under mechanical load
10. Test complete parking workflow
11. Test sensor interference
12. Test power stability
13. Test abnormal sensor readings
14. Test long-duration operation
```

---

# 31. Regression Testing

Whenever the firmware is modified, rerun at minimum:

```text
TC01 All slots free
TC02 One occupied
TC05 Parking full
TC06 Slot becomes free
TC07 OLED
TC08 Dashboard
TC11 Buzzer
TC12 Servo
TC13 Reset
```

This prevents a new feature from breaking an existing function.

---

# 32. Recommended Demonstration Sequence

For a project presentation, demonstrate:

```text
STEP 1
All slots free
4/4
Gate OPEN

        ↓

STEP 2
One occupied
3/4

        ↓

STEP 3
Two occupied
2/4

        ↓

STEP 4
Three occupied
1/4

        ↓

STEP 5
All occupied
0/4
PARKING FULL
Buzzer ON
Gate CLOSED

        ↓

STEP 6
One slot becomes free
1/4
Buzzer OFF
Gate OPEN
```

This demonstrates the complete control loop in a short, understandable sequence.

---

# 33. Test Data Example

Example sensor dataset:

```csv
timestamp,slot1_cm,slot2_cm,slot3_cm,slot4_cm,available_slots,status
T1,80,80,80,80,4,SPACE_AVAILABLE
T2,20,80,80,80,3,SPACE_AVAILABLE
T3,20,20,80,80,2,SPACE_AVAILABLE
T4,20,20,20,80,1,SPACE_AVAILABLE
T5,20,20,20,20,0,PARKING_FULL
T6,20,20,20,80,1,SPACE_AVAILABLE
```

This is **sample simulation data** and should not be represented as physical measurements.

---

# 34. Testing Workflow

```text
Define Test
    ↓
Set Sensor Input
    ↓
Run Wokwi
    ↓
Observe Embedded Outputs
    ↓
Observe Web Dashboard
    ↓
Compare With Expected Result
    ↓
Record Actual Result
    ↓
Mark PASS / FAIL
    ↓
Capture Evidence
    ↓
Update Test Report
```

---

# 35. Test Documentation Structure

The repository separates testing information into:

```text
test_cases/
├── README.md
└── test_cases.csv

outputs/
└── test_results.csv

docs/
└── testing.md
```

### `docs/testing.md`

Contains the overall testing strategy.

### `test_cases/README.md`

Contains detailed individual test procedures.

### `test_cases/test_cases.csv`

Contains structured test data.

### `outputs/test_results.csv`

Contains the final executed results.

---

# 36. Final Testing Checklist

Before publishing the project:

```text
[ ] PlatformIO build successful
[ ] Wokwi simulation starts
[ ] Four sensors tested
[ ] All-free condition tested
[ ] One occupied condition tested
[ ] Two occupied condition tested
[ ] Three occupied condition tested
[ ] Full condition tested
[ ] Slot-release condition tested
[ ] OLED tested
[ ] Green LEDs tested
[ ] Red LEDs tested
[ ] Buzzer tested
[ ] Servo closed tested
[ ] Servo reopened tested
[ ] Web dashboard tested
[ ] Invalid reading behavior checked
[ ] Threshold boundary checked
[ ] Reset checked
[ ] Screenshots captured
[ ] Actual results recorded
[ ] PASS/FAIL values verified
```

---

# 37. Final Validation Statement

The **Smart Parking System Using Ultrasonic Sensors** is considered functionally validated as a virtual prototype when the defined Wokwi test scenarios pass and the corresponding screenshots and outputs have been recorded.

The validation demonstrates the complete embedded workflow:

```text
Sensing
  ↓
Processing
  ↓
Decision
  ↓
Visualization
  ↓
Alert
  ↓
Actuation
  ↓
IoT Monitoring
```

---

# 38. Final Testing Specification

```text
Controller        : ESP32 DevKit
Slots             : 4
Sensors           : HC-SR04 × 4
Threshold         : 35 cm
Display           : SSD1306 OLED
LEDs              : Green + Red per slot
Buzzer            : Parking-full alert
Servo             : Capacity-based gate
Connectivity      : Wi-Fi
Dashboard         : ESP32 HTTP Dashboard
Simulation        : Wokwi
Build              : PlatformIO
Testing Type      : Virtual System Validation
Physical Testing  : Not performed
Project Status    : Functional Virtual Prototype
```

---

# 39. Related Documentation

```text
docs/architecture.md
docs/algorithm.md
docs/hardware.md
docs/pin_mapping.md
docs/simulation.md
docs/troubleshooting.md
test_cases/README.md
```

---

# 40. Conclusion

The testing strategy provides a structured method for verifying the Smart Parking System from individual sensor behavior through complete system integration.

The test plan covers:

- Sensor functionality
- Parking logic
- State management
- Availability counting
- OLED output
- LED indication
- Buzzer alerts
- Servo operation
- Web dashboard
- Error handling
- Reset behavior
- Build verification
- Portfolio evidence