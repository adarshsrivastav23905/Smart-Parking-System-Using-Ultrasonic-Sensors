# 🔧 Smart Parking System — Troubleshooting Guide

## 1. Purpose

This document provides a structured troubleshooting guide for the **Smart Parking System Using Ultrasonic Sensors**.

It covers the major development and simulation issues that can occur with:

- ESP32 firmware
- PlatformIO
- Wokwi
- HC-SR04 sensors
- SSD1306 OLED
- LEDs
- Buzzer
- SG90 servo
- Wi-Fi
- HTTP web dashboard
- Serial Monitor
- Git/GitHub workflow

Use this document as the first reference when a previously working feature stops behaving correctly.

---

# 2. Quick Diagnosis Flow

When the system is not working, follow this order:

```text
1. Check PlatformIO build
          ↓
2. Check Wokwi simulation starts
          ↓
3. Check Serial output
          ↓
4. Check sensor distances
          ↓
5. Check OLED
          ↓
6. Check LEDs
          ↓
7. Check buzzer
          ↓
8. Check servo
          ↓
9. Check Wi-Fi
          ↓
10. Check web dashboard
```

Do not change multiple parts of the code at the same time. Test one subsystem, confirm it works, then continue.

---

# 3. First-Level Health Check

Before debugging a specific component, verify:

```text
[ ] Correct project folder opened in VS Code
[ ] PlatformIO environment is available
[ ] src/main.cpp exists
[ ] platformio.ini exists
[ ] Wokwi configuration exists
[ ] diagram.json is valid
[ ] Build succeeds
[ ] Wokwi simulation starts
[ ] ESP32 initializes
```

Run a clean build when appropriate:

```text
PlatformIO
→ Project Tasks
→ esp32dev
→ General
→ Clean
```

Then:

```text
PlatformIO
→ Project Tasks
→ esp32dev
→ General
→ Build
```

Expected result:

```text
SUCCESS
```

---

# 4. PlatformIO Build Errors

## 4.1 `undefined reference to handleRoot()`

### Symptom

The linker reports something similar to:

```text
undefined reference to `handleRoot()'
```

### Cause

`handleRoot()` is declared or referenced but no valid implementation exists.

### Fix

Make sure the function has one implementation:

```cpp
void handleRoot() {
    // Generate and send dashboard response
}
```

Avoid declaring a function and then accidentally deleting its implementation.

---

# 5. `redefinition of handleRoot()`

### Symptom

The compiler reports:

```text
error: redefinition of 'void handleRoot()'
```

### Cause

The same function was added twice to `main.cpp`.

### Fix

Search for:

```cpp
void handleRoot()
```

There must be only one implementation.

Also check for accidental duplicate pasted sections.

---

# 6. `redefinition of lastDisplay`

### Symptom

The compiler reports:

```text
redefinition of 'lastDisplay'
```

or similar messages for:

```text
lastSerial
lastFullBeep
```

### Cause

Variables declared inside `loop()` were duplicated during code editing.

### Fix

Keep only one declaration of each variable in the same scope.

For example:

```cpp
static unsigned long lastDisplay = 0;
static unsigned long lastSerial = 0;
static unsigned long lastFullBeep = 0;
```

Do not repeat them later in the same `loop()`.

---

# 7. `pulseIn was not declared`

### Symptom

Compiler error:

```text
'pulseIn' was not declared in this scope
```

### Common Cause

The project is being compiled for an unexpected board/framework or the source code contains an incompatible implementation.

### Fix

Verify `platformio.ini` contains the intended ESP32 environment.

Example:

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
```

Then clean and rebuild.

---

# 8. Missing Library Errors

### Symptom

Example:

```text
fatal error: Adafruit_SSD1306.h: No such file or directory
```

or:

```text
fatal error: ESP32Servo.h: No such file or directory
```

### Fix

Check `platformio.ini` and add the required library dependencies.

Example:

```ini
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    madhephaestus/ESP32Servo
```

Then:

```text
PlatformIO
→ Clean
→ Build
```

Use the libraries actually imported by the current `main.cpp`.

---

# 9. Build Works but Wokwi Does Not Start

## Symptoms

- PlatformIO build succeeds.
- Wokwi does not start.
- Simulation remains blank or fails to load.

### Check

```text
[ ] diagram.json is valid
[ ] ESP32 board type is correct
[ ] Firmware path is correct
[ ] ELF path is correct
[ ] Wokwi configuration exists
[ ] Compiled firmware exists
```

Do not replace the working `diagram.json` with a guessed version.

---

# 10. Duplicate `diagram.json` or `wokwi.toml`

### Problem

The project may contain:

```text
diagram.json
wokwi.toml
```

at the project root and also:

```text
simulation/
├── diagram.json
└── wokwi.toml
```

### Risk

You may edit one file while Wokwi actually uses the other.

### Fix

Choose one authoritative Wokwi configuration.

Use the configuration that is connected to the working simulation.

Keep the repository structure clear and document the location.

---

# 11. Wokwi RFC2217 / Port Already in Use

### Symptom

You may see:

```text
Wokwi RFC2217 Server failed to start
EADDRINUSE
address already in use
```

### Cause

Port `4000`, `4001`, or another configured port is already being used.

### Fix

Stop the previous monitor/debug process.

Close:

- Old Wokwi terminals
- Old PlatformIO monitor sessions
- Previous debug sessions

Then restart the simulation.

If necessary, configure another unused RFC2217 port in `wokwi.toml`.

Example:

```toml
rfc2217ServerPort = 4002
```

Use a port that is not already occupied.

---

# 12. Wokwi Debug Mode Error

### Symptom

You may see:

```text
Wokwi.gdbServerPort is not set in wokwi.toml
```

### Meaning

Debug mode requires an additional GDB server configuration.

### Fix

For normal project execution, use:

```text
Start Simulation
```

rather than:

```text
Start with Debugger
```

unless you intentionally configure debugging.

---

# 13. Serial Monitor Is Empty

## Symptoms

- Wokwi runs.
- Terminal appears.
- No serial messages are visible.

### Check 1 — Firmware serial initialization

Verify `setup()` contains something such as:

```cpp
Serial.begin(115200);
```

The baud rate in the monitor must match.

### Check 2 — Correct monitor

In Wokwi, use the simulation's serial terminal/monitor.

For PlatformIO:

```text
PlatformIO
→ Device Monitor
```

Do not confuse the normal VS Code terminal with the serial monitor.

### Check 3 — Wokwi serial connection

The `diagram.json` should expose the ESP32 serial interface to the Wokwi monitor.

### Check 4 — Firmware is actually running

Add or verify a startup message:

```cpp
Serial.println("SMART PARKING SYSTEM STARTING...");
```

If this does not appear, the issue is earlier in the execution path.

---

# 14. PlatformIO Monitor Shows Port Selection Prompt

### Symptom

You see:

```text
Available ports:
Enter port index or full name:
```

### Meaning

PlatformIO is trying to find a physical serial port.

### In Wokwi

Use the Wokwi terminal/serial connection or the Wokwi RFC2217 endpoint rather than expecting a physical COM port.

If using PlatformIO's monitor, ensure the intended Wokwi port is configured correctly.

---

# 15. PlatformIO Monitor Shows RFC2217

A line such as:

```text
Terminal on rfc2217://localhost:4001 | 115200 8-N-1
```

means PlatformIO is connecting to Wokwi's virtual serial interface.

If output still does not appear:

```text
1. Verify simulation is running.
2. Verify firmware prints Serial output.
3. Verify baud rate.
4. Close duplicate monitor sessions.
5. Restart Wokwi.
6. Restart PlatformIO monitor.
```

---

# 16. Python Not Found

### Symptom

Windows shows:

```text
Python was not found; run without arguments to install from the Microsoft Store
```

### Cause

A Windows Python alias is being invoked.

### Important

The Smart Parking firmware itself does not require Python to run.

PlatformIO manages its own tools inside its environment.

If a separate script requires Python, install Python or use the Python interpreter provided by the relevant development environment.

---

# 17. OLED Not Displaying

## Symptoms

- Simulation runs.
- OLED remains blank.

### Check wiring

```text
OLED VCC → 3.3V
OLED GND → GND
OLED SDA → GPIO 21
OLED SCL → GPIO 22
```

### Check I²C address

Current design:

```text
0x3C
```

### Check software initialization

Verify that the OLED initialization succeeds before attempting to draw.

Typical initialization concept:

```cpp
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
```

### Additional checks

```text
[ ] Correct OLED model
[ ] Correct library
[ ] Correct I²C address
[ ] Correct GPIOs
[ ] display.clearDisplay()
[ ] display.display()
```

Remember that drawing commands may not appear until the display buffer is sent.

---

# 18. OLED Shows Text but It Is Hard to Read

### Cause

The OLED may be too close to wires or positioned behind other components in the Wokwi diagram.

### Fix

Move the OLED physically upward and away from overlapping wires/components in `diagram.json`.

The display location affects only the simulation layout, not the firmware.

A professional screenshot should show:

```text
ESP32
Sensors
OLED
LEDs
Buzzer
Servo
```

without wires obscuring the display.

---

# 19. HC-SR04 Sensor Not Changing State

### Check threshold

Current threshold:

```text
35 cm
```

Therefore:

```text
< 35 cm  → OCCUPIED
>= 35 cm → FREE
```

### Example

```text
20 cm → OCCUPIED
80 cm → FREE
```

### Check sensor pin mapping

```text
S1 TRIG → GPIO 5
S1 ECHO → GPIO 17

S2 TRIG → GPIO 16
S2 ECHO → GPIO 4

S3 TRIG → GPIO 27
S3 ECHO → GPIO 26

S4 TRIG → GPIO 25
S4 ECHO → GPIO 35
```

The firmware and diagram must use the same mapping.

---

# 20. Fourth Sensor Appears Slow

### Possible causes

Multiple HC-SR04 sensors are being processed sequentially.

This is intentional to reduce cross-talk.

Expected sequence:

```text
Slot 1
  ↓
Slot 2
  ↓
Slot 3
  ↓
Slot 4
```

### Improvement

Avoid excessively long blocking delays.

Use:

- Short echo timeouts
- Controlled sensor intervals
- Non-blocking display/serial timers
- Sequential sensor triggering

Do not remove the fourth sensor merely because it updates slightly later unless it is actually malfunctioning.

---

# 21. Ultrasonic Sensor Cross-Talk

### Symptom

Multiple sensors produce unstable readings.

### Cause

Two ultrasonic sensors can interfere if their pulses overlap.

### Fix

Trigger them sequentially:

```text
Read S1
wait
Read S2
wait
Read S3
wait
Read S4
```

Physical deployment requires additional spacing and timing validation.

---

# 22. LED Not Turning On

### Check mapping

```text
S1 Green → GPIO 12
S1 Red   → GPIO 2

S2 Green → GPIO 14
S2 Red   → GPIO 13

S3 Green → GPIO 32
S3 Red   → GPIO 23

S4 Green → GPIO 15
S4 Red   → GPIO 33
```

### Check logic

```text
FREE:
Green ON
Red OFF
```

```text
OCCUPIED:
Green OFF
Red ON
```

### Check resistor

Each LED should have a current-limiting resistor.

Current design:

```text
220 Ω
```

---

# 23. Both Green and Red LEDs Turn On

### Cause

Common causes include:

- Incorrect GPIO mapping
- Duplicate control logic
- LED polarity/wiring problem
- State update function being called incorrectly

### Fix

Make the logic mutually exclusive:

```cpp
if (occupied) {
    green = LOW;
    red = HIGH;
} else {
    green = HIGH;
    red = LOW;
}
```

Verify there is only one final LED-control function.

---

# 24. Red LEDs Are Always On

### Check

Verify the current distance values.

If a sensor is below:

```text
35 cm
```

the slot is correctly considered occupied.

Also verify that `occupied` is not initialized incorrectly.

Restart the simulation and check startup state.

---

# 25. Buzzer Not Making Sound

## Check 1

Verify:

```text
Buzzer signal → GPIO 19
Buzzer GND → GND
```

## Check 2

Parking must be full:

```text
20 / 20 / 20 / 20
```

Expected:

```text
Available = 0/4
Buzzer = ON
Gate = CLOSED
```

## Check 3

The firmware may use periodic beep logic instead of continuous sound.

A still screenshot cannot prove audio output. A short video is better evidence.

---

# 26. Buzzer Is Always On

### Possible causes

- `availableSlots` is incorrectly calculated as zero.
- Sensor states are stuck at OCCUPIED.
- Buzzer logic is outside the full-parking condition.
- Buzzer is not being turned off after a slot becomes free.

Expected:

```text
Available > 0 → Buzzer OFF
Available = 0 → Buzzer alert
```

---

# 27. Servo Is Not Moving

## Check connections

```text
Servo PWM → GPIO 18
Servo V+  → 5V
Servo GND → GND
```

## Check logic

The gate is capacity-based.

```text
Available Slots > 0
→ OPEN
```

```text
Available Slots = 0
→ CLOSED
```

Test with:

```text
80 / 80 / 80 / 80
```

then:

```text
20 / 20 / 20 / 20
```

then release one slot:

```text
20 / 20 / 20 / 80
```

Expected:

```text
OPEN → CLOSED → OPEN
```

---

# 28. Servo Stays in One Position

### Possible causes

- Parking state never changes.
- Servo update function is never called.
- Incorrect PWM GPIO.
- Duplicate servo declarations.
- Servo commands are being overwritten.
- Gate-state variable is incorrectly initialized.

### Recommended debugging

Print:

```text
Available Slots
Gate State
Requested Gate State
```

Example:

```text
Available: 0
Gate: CLOSED
```

Then:

```text
Available: 1
Gate: OPEN
```

---

# 29. Servo Moves Constantly

### Cause

The servo is being commanded repeatedly every loop.

### Better logic

Keep a gate-state variable:

```cpp
bool gateOpen = false;
```

Only move the servo when the requested state differs from the current state.

Conceptually:

```text
Requested State != Current State
        ↓
Move Servo
        ↓
Update Current State
```

This reduces unnecessary commands and improves stability.

---

# 30. Servo Gate Does Not Match Parking Status

Use this validation:

| Available Slots | Expected Gate |
|---:|---|
| 4 | OPEN |
| 3 | OPEN |
| 2 | OPEN |
| 1 | OPEN |
| 0 | CLOSED |

If the gate does not match the table, inspect the availability calculation before changing servo code.

---

# 31. Web Dashboard Not Opening

## Expected URL

```text
http://localhost:8180
```

## Check

```text
[ ] Wokwi running
[ ] Wi-Fi initialized
[ ] HTTP server started
[ ] Port forwarding configured
[ ] No duplicate port conflict
```

Typical Wokwi forwarding:

```toml
[[net.forward]]
from = "localhost:8180"
to = "target:80"
```

Use the exact configuration that belongs to the current working project.

---

# 32. Dashboard Opens but Does Not Update

### Possible causes

- Page is not refreshed.
- Dashboard endpoint serves static state.
- Global slot values are not updated before the HTTP response.
- Browser caching.
- Web server handler is not using current state.

### Check

Make sure the page reads current variables when `handleRoot()` runs.

Also verify that the dashboard and OLED use the same slot-state variables.

---

# 33. Dashboard Data Does Not Match OLED

### Cause

Two different state calculations may exist.

### Correct design

Both interfaces should use the same source of truth:

```text
Sensor Reading
      ↓
Slot State
      ↓
Available Count
      ├── OLED
      └── Web Dashboard
```

Do not independently recalculate occupancy inside the HTML handler.

---

# 34. Wi-Fi Does Not Connect

For Wokwi, verify the configured network.

Typical Wokwi environment:

```text
SSID: Wokwi-GUEST
Password: empty
```

Then confirm startup output reports the connection state.

If the dashboard still works, the networking path is functioning.

---

# 35. Port 8180 Not Available

### Symptom

The dashboard cannot open because the port is already in use.

### Fix

Check whether another process is using the port.

On Windows PowerShell:

```powershell
netstat -ano | findstr :8180
```

Stop the stale application if appropriate, then restart Wokwi.

Use a different host port only when necessary and update the documentation accordingly.

---

# 36. Port 4000 / 4001 Already in Use

Check the port:

```powershell
netstat -ano | findstr :4001
```

Then close stale:

- Wokwi sessions
- PlatformIO monitors
- Debug sessions

Restart the simulation.

---

# 37. `wokwi.toml` Does Not Open Correctly

### Check

Ensure the file is named exactly:

```text
wokwi.toml
```

not:

```text
wokwi.toml.txt
```

Enable file extensions in Windows Explorer if necessary.

The file should be a plain text TOML configuration file.

---

# 38. `diagram.json` Does Not Open as Expected

Ensure the filename is exactly:

```text
diagram.json
```

not:

```text
diagram.json.txt
```

JSON syntax must also be valid.

Avoid manually editing large connection sections without saving a backup.

---

# 39. Wokwi Circuit Looks Too Small

### Cause

The simulator canvas has been zoomed out.

### Fix

Use Wokwi zoom controls or browser/simulator zoom to make the circuit readable.

For project screenshots:

- Zoom until component labels are visible.
- Ensure OLED text can be read.
- Keep the complete circuit in frame.

---

# 40. Wires Hide the OLED

### Fix

Move the OLED higher in `diagram.json`.

The OLED should be positioned:

- Away from thick wire bundles.
- High enough to remain readable.
- Close enough to the main circuit to look organized.

This is a diagram layout issue, not a firmware issue.

---

# 41. Serial Output Contains No Distance Values

### Check firmware

The main loop should periodically print distance/state information.

Recommended format:

```text
Slot 1 | Distance: XX.X cm | Status: FREE
Slot 2 | Distance: XX.X cm | Status: OCCUPIED
Slot 3 | Distance: XX.X cm | Status: FREE
Slot 4 | Distance: XX.X cm | Status: FREE

Available Slots: 3/4
Occupied Slots: 1/4
```

Make sure the serial timer does not prevent repeated output.

---

# 42. Serial Output Is Too Fast

### Cause

Serial printing may occur on every loop iteration.

### Fix

Use a timed reporting interval.

Concept:

```cpp
if (millis() - lastSerial >= serialInterval) {
    lastSerial = millis();
    printSerialStatus();
}
```

This keeps the output readable and prevents unnecessary serial traffic.

---

# 43. OLED Refresh Is Too Frequent

### Symptom

Display appears to flicker or the system becomes slower.

### Fix

Update periodically instead of every instruction cycle.

Use:

```text
millis()-based scheduling
```

rather than long blocking delays.

---

# 44. System Becomes Slow

### Common causes

- Long `delay()` calls
- Long `pulseIn()` timeouts
- Excessive OLED refreshes
- Excessive serial output
- Repeated servo commands
- Blocking web/server logic

### Recommended architecture

Use short, controlled timing windows:

```text
Sensor Read
   ↓
Process State
   ↓
Update Outputs
   ↓
Handle Web
   ↓
Repeat
```

Avoid large blocking delays in the main loop.

---

# 45. Fourth Slot Updates Later Than Other Slots

This can be normal when the sensors are read sequentially.

If the delay is excessive:

```text
Check Slot 4 ECHO timeout
Check Slot 4 trigger
Check confirmation timing
Check accidental delay in the loop
```

Do not immediately conclude that GPIO 35 is faulty.

---

# 46. System Resets Unexpectedly

### Possible causes

- Invalid memory access
- Duplicate or conflicting library usage
- Servo/power issue in physical hardware
- Watchdog timeout
- Large blocking delay
- Invalid array index

### Wokwi

Check the debug/serial output for crash information.

### Code review

Verify:

```text
[ ] Array indices are 0–3
[ ] No out-of-bounds access
[ ] Functions return expected values
[ ] No duplicate global definitions
```

---

# 47. Web Server Causes Build Errors

### Common cause

Duplicate functions such as:

```cpp
handleRoot()
```

or duplicate web-server declarations.

### Fix

Organize the firmware into clear sections:

```text
Declarations
↓
Helper functions
↓
Web handlers
↓
setup()
↓
loop()
```

Keep each function implemented exactly once.

---

# 48. Git Shows Many Deleted Screenshots

### Cause

The project changed screenshot naming from:

```text
1_project_structure.png
2_platformio_build_success.png
...
```

to:

```text
01_project_structure.png
02_platformio_build_success.png
...
```

### This is not automatically an error.

Git sees renamed files as deleted + new until similarity detection recognizes the rename.

Before committing, verify that the new screenshots are the correct files.

---

# 49. Git Rebase Conflict: `test_results.md`

### Example

```text
CONFLICT (modify/delete):
test_cases/test_results.md
```

### Meaning

One branch deleted the file while the other modified it.

If the final project intentionally replaced it with:

```text
test_cases/README.md
```

then decide whether the old file should remain deleted.

After resolving:

```powershell
git add/rm <file>
git rebase --continue
```

---

# 50. Vim Appears During `git rebase --continue`

### Exit and continue

If Vim opens:

```text
Esc
:wq
Enter
```

This saves the commit message and exits Vim.

### If you only want to quit without saving

```text
Esc
:q!
Enter
```

Do not force-quit if you are not sure whether the commit message has already been entered.

---

# 51. Vim Swap File Warning

### Symptom

You see:

```text
E325: ATTENTION
Found a swap file...
```

### Usually means

An old Vim session left a `.swp` file.

### Safe approach

If no other editor is actively editing the same Git commit message:

```text
D
```

to delete the old swap file.

Then continue.

Alternatively:

```text
Q
```

or:

```text
A
```

can exit/abort the current editor interaction.

Do not recover the old file unless you actually need its unsaved content.

---

# 52. Rebase Completed Successfully

A healthy result is:

```text
On branch main
Your branch is ahead of 'origin/main' by 1 commit.

nothing to commit, working tree clean
```

This means the rebase/local commit is complete and there are no uncommitted changes.

---

# 53. Git Push Rejected — `fetch first`

### Symptom

```text
! [rejected] main -> main (fetch first)
```

### Cause

The remote GitHub branch contains commits that your local branch does not yet contain.

### Safe process

First:

```powershell
git status
```

Then, if your working tree is clean:

```powershell
git fetch origin
git rebase origin/main
```

Resolve any conflicts.

After a successful rebase:

```powershell
git push origin main
```

Do not use:

```powershell
git push --force
```

unless you fully understand the consequences and intentionally want to rewrite the remote branch history.

---

# 54. Git Shows `ahead of origin/main by 1 commit`

This is normally good.

It means:

```text
Local main
   ↓
contains one commit
   ↓
not yet on GitHub
```

Push with:

```powershell
git push origin main
```

provided there are no remote changes that need integration.

---

# 55. Git Working Tree Is Not Clean

### Check:

```powershell
git status
```

For each modified file, decide:

```text
Keep change
OR
Discard change
```

To keep:

```powershell
git add .
git commit -m "Finalize project documentation and validation"
```

To discard an accidental change:

```powershell
git restore <file>
```

Be careful with `git restore` because it discards local changes.

---

# 56. GitHub Repository Looks Messy

Use a professional structure:

```text
Smart-Parking-Ultrasonic-Sensor-System/
│
├── README.md
├── platformio.ini
├── src/
├── docs/
├── simulation/
├── test_cases/
├── data/
├── outputs/
├── screenshots/
├── circuit_diagram/
└── reports/
```

Remove unused duplicate files and old documentation.

---

# 57. GitHub README Screenshots Are Broken

### Check relative paths

Example:

```markdown
![All Slots Free](screenshots/04_all_slots_free.png)
```

The filename must match the actual GitHub filename exactly.

GitHub paths are case-sensitive.

---

# 58. Markdown Table Errors

### Symptom

GitHub reports or displays malformed tables.

### Correct format

```markdown
| Test ID | Input | Expected | Status |
|---|---|---|---|
| TC01 | 80/80/80/80 | 4/4 | PASS |
```

Every row must have the same number of `|`-separated columns.

Do not insert broken lines inside a table row.

---

# 59. CSV Errors

### Check

- Header row exists.
- Every row uses the same number of columns.
- Commas inside text are quoted.
- No accidental Markdown has been pasted into CSV.
- Save as UTF-8 CSV.

Example:

```csv
test_id,input,expected,status
TC01,"80/80/80/80","4/4 available",PASS
```

---

# 60. Sample Data vs Actual Evidence

Do not present:

```text
sample_sensor_data.csv
```

as physical measurement data.

Clearly label it:

```text
Sample / Simulation Data
```

Actual project evidence should come from:

- Wokwi simulation
- Serial output
- Screenshots
- Test execution

---

# 61. Incorrect Claims to Avoid

Do not claim:

```text
Physical hardware tested
```

unless physical hardware was actually tested.

Do not claim:

```text
Production-ready
```

when the project is a simulation prototype.

Better wording:

```text
Functional virtual prototype validated in Wokwi.
```

---

# 62. Physical Hardware Troubleshooting

When moving to real hardware, first test one subsystem at a time:

```text
ESP32
  ↓
One HC-SR04
  ↓
OLED
  ↓
One LED pair
  ↓
Buzzer
  ↓
Servo
  ↓
All sensors
  ↓
Complete system
```

Do not connect all hardware at once when diagnosing the first physical build.

---

# 63. HC-SR04 ECHO Safety

Many HC-SR04 modules use a 5V logic-level ECHO signal.

ESP32 GPIOs have different voltage requirements.

For physical hardware:

```text
HC-SR04 ECHO
      ↓
Voltage Divider / Level Shifter
      ↓
ESP32 GPIO
```

Verify the exact sensor breakout before connecting it.

---

# 64. Servo Power Troubleshooting

A physical servo can draw more current than the ESP32 supply path comfortably provides.

Symptoms of inadequate power can include:

- Servo not moving
- ESP32 resets
- Unstable behavior
- Flickering outputs

Use an appropriate external servo supply when needed and connect grounds together.

---

# 65. Physical Sensor Calibration

The simulated:

```text
35 cm
```

threshold is not automatically the correct real-world threshold.

Calibrate using:

```text
Empty slot
Vehicle at expected position
Different vehicle sizes
Different sensor heights
```

Then select a stable threshold based on measured results.

---

# 66. Web Dashboard Security Limitation

The current prototype is intended for demonstration.

A production deployment should add:

- Authentication
- HTTPS
- Authorization
- Input validation
- Secure Wi-Fi credentials
- Network isolation
- Safe actuator controls

Do not expose an unauthenticated gate-control endpoint to an untrusted network.

---

# 67. Most Common Problems — Quick Table

| Symptom | First Check |
|---|---|
| Build fails | `platformio.ini` + duplicate functions |
| OLED blank | GPIO 21/22 + address `0x3C` |
| Sensor stuck | Distance + GPIO mapping |
| Slot 4 slow | Sensor timeout/sequential timing |
| Green LED off | GPIO + resistor + FREE state |
| Red LED off | GPIO + OCCUPIED state |
| Buzzer silent | Full condition + GPIO 19 |
| Servo static | GPIO 18 + available/full transition |
| Dashboard unavailable | Wi-Fi + port forwarding |
| Serial empty | `Serial.begin()` + correct monitor |
| Wokwi port busy | Close stale sessions/change port |
| Git push rejected | Fetch/rebase remote first |
| Git conflict | Resolve → `git add` → `git rebase --continue` |
| Vim stuck | `Esc` → `:wq` → Enter |

---

# 68. Recovery Checklist

When a previously working project suddenly fails:

```text
[ ] Stop Wokwi
[ ] Save files
[ ] Check git status
[ ] Build with PlatformIO
[ ] Fix compiler errors first
[ ] Restart Wokwi
[ ] Check Serial startup
[ ] Check sensor distances
[ ] Check OLED
[ ] Check LEDs
[ ] Check buzzer
[ ] Check servo
[ ] Check dashboard
[ ] Capture new evidence if behavior changed
```

---

# 69. Preventive Development Practices

To avoid repeating debugging problems:

### Keep one source of truth

Avoid duplicate:

```text
diagram.json
wokwi.toml
main.cpp
```

### Make small changes

Change one subsystem at a time.

### Build frequently

Run PlatformIO Build after meaningful code changes.

### Commit working checkpoints

Example:

```text
feat: add parking detection
feat: add OLED status
feat: add LED indicators
feat: add buzzer alert
feat: add servo gate
feat: add web dashboard
docs: finalize testing documentation
```

### Preserve working versions

Before major refactoring, make a Git commit.

---

# 70. Final Troubleshooting Procedure

Use this sequence for professional debugging:

```text
              PROBLEM
                 │
                 ▼
          Reproduce Issue
                 │
                 ▼
          Check Build
                 │
                 ▼
         Check Serial Output
                 │
                 ▼
       Isolate Subsystem
                 │
       ┌─────────┼─────────┐
       ▼         ▼         ▼
    Sensor     Display    Output
       │         │         │
       └─────────┼─────────┘
                 ▼
          Apply Small Fix
                 │
                 ▼
             Rebuild
                 │
                 ▼
           Re-run Test
                 │
             ┌───┴───┐
             │       │
          PASS       FAIL
             │       │
             ▼       └──────► Continue Diagnosis
        Document Fix
```

---

# 71. Final System Health Checklist

Before declaring the project ready for GitHub:

```text
[ ] PlatformIO build succeeds
[ ] Wokwi starts
[ ] Four sensors respond
[ ] 35 cm threshold works
[ ] OLED displays state
[ ] Green LEDs work
[ ] Red LEDs work
[ ] Buzzer works when full
[ ] Servo opens with available space
[ ] Servo closes when full
[ ] Servo reopens after slot release
[ ] Web dashboard loads
[ ] Dashboard matches OLED/system state
[ ] Serial output is readable
[ ] Test cases are documented
[ ] Screenshots are correctly named
[ ] No duplicate Wokwi configurations
[ ] Git working tree is understood
[ ] GitHub push succeeds
```

---

# 72. Project Status Reference

The current documented project should be described as:

```text
Controller        : ESP32 DevKit
Parking Slots     : 4
Sensors           : HC-SR04 × 4
Display           : SSD1306 OLED 128×64
Indicators        : 4 Green + 4 Red LEDs
Alert             : Buzzer
Gate              : SG90 Servo
Threshold         : 35 cm
Connectivity      : ESP32 Wi-Fi
Dashboard         : HTTP Web Dashboard
Simulation        : Wokwi
Build Environment : PlatformIO
Firmware          : Embedded C/C++
Validation        : Virtual Prototype
Physical Testing  : Not performed
Status            : Functional Virtual Prototype
```

---

# 73. Related Documentation

```text
README.md
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

# 74. Final Note

The most reliable debugging strategy for this project is:

```text
Build
 ↓
Run
 ↓
Observe
 ↓
Isolate
 ↓
Fix
 ↓
Rebuild
 ↓
Retest
 ↓
Document
```
