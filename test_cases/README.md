# 🧪 Smart Parking System — Test Cases

## 1. Test Environment

| Parameter | Configuration |
|---|---|
| Controller | ESP32 DevKit |
| Simulation | Wokwi |
| Parking Slots | 4 |
| Sensor | HC-SR04 |
| Detection Threshold | 35 cm |
| Display | SSD1306 OLED 128×64 |
| Indicators | 4 Green + 4 Red LEDs |
| Alert | Buzzer |
| Gate | SG90 Servo |
| Web Interface | ESP32 HTTP Dashboard |

---

## 2. Functional Test Matrix

| Test ID | Test Scenario | Sensor Input | Expected Result | Actual Result | Status |
|:---:|---|---|---|---|:---:|
| TC01 | All slots free | 80 / 80 / 80 / 80 cm | 4/4 available; gate open |  | ⬜ |
| TC02 | One slot occupied | 20 / 80 / 80 / 80 cm | 3/4 available |  | ⬜ |
| TC03 | Two slots occupied | 20 / 20 / 80 / 80 cm | 2/4 available |  | ⬜ |
| TC04 | Three slots occupied | 20 / 20 / 20 / 80 cm | 1/4 available |  | ⬜ |
| TC05 | Parking full | 20 / 20 / 20 / 20 cm | 0/4; buzzer on; gate closed |  | ⬜ |
| TC06 | Slot becomes free | One slot changed to ≥35 cm | Count increases; gate reopens |  | ⬜ |
| TC07 | OLED verification | Mixed slot distances | Correct distance/state/count |  | ⬜ |
| TC08 | Web dashboard | Mixed slot distances | Dashboard matches system state |  | ⬜ |
| TC09 | Green LED | Slot FREE | Green LED ON |  | ⬜ |
| TC10 | Red LED | Slot OCCUPIED | Red LED ON |  | ⬜ |
| TC11 | Buzzer | All slots occupied | Buzzer alert |  | ⬜ |
| TC12 | Servo gate | 0/4 ↔ 1/4 transition | Gate closes/opens correctly |  | ⬜ |
| TC13 | System reset | Restart simulation | System initializes safely |  | ⬜ |

---

## 3. Detailed Test Cases

### TC01 — All Parking Slots Free

**Input**

```text
S1 = 80 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

**Expected**

```text
S1 → FREE
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots → 4/4
Gate → OPEN
Buzzer → OFF
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/04_all_slots_free.png
```

---

### TC02 — One Slot Occupied

**Input**

```text
S1 = 20 cm
S2 = 80 cm
S3 = 80 cm
S4 = 80 cm
```

**Expected**

```text
S1 → OCCUPIED
S2 → FREE
S3 → FREE
S4 → FREE

Available Slots → 3/4
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/05_slot1_occupied.png
```

---

### TC03 — Two Slots Occupied

**Input**

```text
S1 = 20 cm
S2 = 20 cm
S3 = 80 cm
S4 = 80 cm
```

**Expected**

```text
S1 → OCCUPIED
S2 → OCCUPIED
S3 → FREE
S4 → FREE

Available Slots → 2/4
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/06_two_slots_occupied.png
```

---

### TC04 — Three Slots Occupied

**Input**

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 80 cm
```

**Expected**

```text
S1 → OCCUPIED
S2 → OCCUPIED
S3 → OCCUPIED
S4 → FREE

Available Slots → 1/4
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/07_three_slots_occupied.png
```

---

### TC05 — Parking Full

**Input**

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 20 cm
```

**Expected**

```text
Available Slots → 0/4
System Status   → PARKING FULL
Green LEDs      → OFF
Red LEDs        → ON
Buzzer          → ON
Gate            → CLOSED
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/08_parking_full.png
```

---

### TC06 — Slot Becomes Free

**Initial condition**

```text
S1 = 20 cm
S2 = 20 cm
S3 = 20 cm
S4 = 20 cm
```

Then change:

```text
S4 = 80 cm
```

**Expected**

```text
S4 → FREE
Available Slots → 1/4
Buzzer → OFF
Gate → OPEN
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/09_servo_reopen.png
```

---

### TC07 — OLED Verification

**Input**

```text
S1 = 20 cm
S2 = 80 cm
S3 = 20 cm
S4 = 80 cm
```

**Expected OLED**

```text
SMART PARKING

S1 20cm O    S2 80cm F
S3 20cm O    S4 80cm F

FREE:2/4     TH:35
FREE GATE:OPEN
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/10_oled_distance_status.png
```

---

### TC08 — Web Dashboard Verification

**Input**

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

**Expected**

```text
Available Slots: 2/4
Occupied Slots: 2/4

Slot 1 → OCCUPIED
Slot 2 → FREE
Slot 3 → OCCUPIED
Slot 4 → FREE
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/15_web_dashboard_mixed.png
```

---

### TC09 — Green LED Verification

**Input**

Make one slot free, for example:

```text
S1 = 80 cm
```

**Expected**

```text
S1 Green LED → ON
S1 Red LED   → OFF
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/11_green_led.png
```

---

### TC10 — Red LED Verification

**Input**

Make one slot occupied:

```text
S1 = 20 cm
```

**Expected**

```text
S1 Green LED → OFF
S1 Red LED   → ON
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/12_red_led.png
```

---

### TC11 — Buzzer Verification

**Input**

```text
20 / 20 / 20 / 20 cm
```

**Expected**

```text
Available Slots → 0/4
Parking Full    → YES
Buzzer          → ON
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/13_buzzer.png
```

---

### TC12 — Servo Gate Verification

#### Part A — Close gate

Start with:

```text
80 / 80 / 80 / 80 cm
```

Then set:

```text
20 / 20 / 20 / 20 cm
```

**Expected**

```text
4/4 or partial availability → Gate Open
0/4 available              → Gate Closed
```

#### Part B — Reopen gate

Change one sensor back to:

```text
80 cm
```

**Expected**

```text
1/4 available
Gate → OPEN
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

**Evidence**

```text
screenshots/09_servo_reopen.png
```

---

### TC13 — System Reset

**Input**

Restart the Wokwi simulation.

**Expected**

```text
ESP32 initializes
OLED initializes
Sensors initialize
LEDs initialize
Buzzer remains silent
Servo starts safely
Web server starts
System becomes ready
```

**Actual Result**

```text
____________________________________________
```

**Status**

```text
⬜ PASS    ⬜ FAIL
```

---

## 4. Test Summary

Complete this section **after** running the tests.

| Category | Total | Passed | Failed | Pending |
|---|---:|---:|---:|---:|
| Slot Detection | 6 |  |  |  |
| OLED | 1 |  |  |  |
| Web Dashboard | 1 |  |  |  |
| LEDs | 2 |  |  |  |
| Buzzer | 1 |  |  |  |
| Servo | 1 |  |  |  |
| Reset | 1 |  |  |  |
| **Total** | **13** |  |  |  |

---

## 5. Evidence Map

| Test ID | Evidence File |
|---|---|
| TC01 | `screenshots/04_all_slots_free.png` |
| TC02 | `screenshots/05_slot1_occupied.png` |
| TC03 | `screenshots/06_two_slots_occupied.png` |
| TC04 | `screenshots/07_three_slots_occupied.png` |
| TC05 | `screenshots/08_parking_full.png` |
| TC06 | `screenshots/09_servo_reopen.png` |
| TC07 | `screenshots/10_oled_distance_status.png` |
| TC08 | `screenshots/15_web_dashboard_mixed.png` |
| TC09 | `screenshots/11_green_led.png` |
| TC10 | `screenshots/12_red_led.png` |
| TC11 | `screenshots/13_buzzer.png` |
| TC12 | `screenshots/09_servo_reopen.png` |
| TC13 | Build/run evidence |

---

## 6. Acceptance Criteria

The virtual prototype is considered ready for portfolio publication when:

- Four parking sensors produce valid readings.
- Distances below 35 cm are classified as OCCUPIED.
- Distances at or above 35 cm are classified as FREE.
- The available-slot count matches the slot states.
- OLED output matches the current slot state.
- Green/red LEDs reflect the slot state.
- The buzzer activates for the parking-full condition.
- The servo changes state correctly when parking changes between available and full.
- The web dashboard matches the embedded system state.
- The PlatformIO build completes successfully.
- Screenshots provide reproducible evidence for the major test scenarios.