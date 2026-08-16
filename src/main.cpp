#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// ============================================================
// SMART PARKING SYSTEM
// ESP32 + 4 HC-SR04 + OLED + LEDs + Buzzer + Servo + Web
// ============================================================

// ============================================================
// OLED
// ============================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// ============================================================
// WIFI / WEB SERVER
// ============================================================

const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

WebServer server(80);

// ============================================================
// SYSTEM SETTINGS
// ============================================================

const int NUM_SLOTS = 4;

// Slot detection threshold
const float SLOT_THRESHOLD_CM = 35.0;

// State confirmation time
const unsigned long STATE_CONFIRM_TIME = 500;

// OLED update interval
const unsigned long DISPLAY_UPDATE_INTERVAL = 300;

// Serial output interval
const unsigned long SERIAL_UPDATE_INTERVAL = 1000;

// Full parking buzzer interval
const unsigned long FULL_BEEP_INTERVAL = 3000;

// ============================================================
// ULTRASONIC SENSOR PINS
// ============================================================

struct UltrasonicSensor
{
    int trigPin;
    int echoPin;
};

UltrasonicSensor sensors[NUM_SLOTS] =
{
    {5, 17},     // Slot 1
    {16, 4},     // Slot 2
    {27, 26},    // Slot 3
    {25, 35}     // Slot 4
};

// ============================================================
// LED PINS
// ============================================================

struct LedPair
{
    int greenPin;
    int redPin;
};

LedPair slotLEDs[NUM_SLOTS] =
{
    {12, 2},     // Slot 1
    {14, 13},    // Slot 2
    {32, 23},    // Slot 3
    {15, 33}     // Slot 4
};

// ============================================================
// BUZZER
// ============================================================

const int BUZZER_PIN = 19;

// ============================================================
// SERVO
// ============================================================

const int SERVO_PIN = 18;

const int GATE_CLOSED_ANGLE = 0;
const int GATE_OPEN_ANGLE = 90;

Servo gateServo;

bool gateOpen = false;
int lastKnownFreeSlots = -1;

// ============================================================
// SLOT STATE
// ============================================================

struct Slot
{
    bool occupied;

    bool pendingState;

    bool candidateState;

    unsigned long candidateSince;

    float distance;
};

Slot slots[NUM_SLOTS];

// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

float readDistanceCM(
    int trigPin,
    int echoPin
);

void updateSlotState(
    int slotIndex,
    float distance
);

void updateLEDs(
    int slotIndex
);

int getOccupiedCount();

int getFreeCount();

void updateOLED();

void printSerialStatus();

void beepFullParking();

void updateGate();

String generateWebPage();

void handleRoot();

void connectWiFi();

// ============================================================
// HC-SR04 DISTANCE MEASUREMENT
// ============================================================

float readDistanceCM(
    int trigPin,
    int echoPin
)
{
    // Trigger LOW
    digitalWrite(
        trigPin,
        LOW
    );

    delayMicroseconds(2);

    // 10 microsecond trigger pulse
    digitalWrite(
        trigPin,
        HIGH
    );

    delayMicroseconds(10);

    digitalWrite(
        trigPin,
        LOW
    );

    // Wait for echo
    // 6000 us is enough for our parking range
    // and keeps the four-slot system responsive.
    unsigned long duration =
        pulseIn(
            echoPin,
            HIGH,
            6000
        );

    // Invalid reading
    if (duration == 0)
    {
        return -1.0;
    }

    // Distance formula:
    // Distance = (Echo Time × Speed of Sound) / 2

    float distance =
        (duration * 0.0343) / 2.0;

    // Basic validity check
    if (
        distance < 2.0 ||
        distance > 400.0
    )
    {
        return -1.0;
    }

    return distance;
}

// ============================================================
// SLOT STATE PROCESSING
// ============================================================

void updateSlotState(
    int slotIndex,
    float distance
)
{
    // Ignore invalid readings
    if (distance < 0)
    {
        return;
    }

    slots[slotIndex].distance =
        distance;

    // Below threshold = occupied
    bool newState =
        distance < SLOT_THRESHOLD_CM;

    // Current state is already correct
    if (
        newState ==
        slots[slotIndex].occupied
    )
    {
        slots[slotIndex].pendingState =
            false;

        return;
    }

    // Start state confirmation
    if (
        !slots[slotIndex].pendingState
    )
    {
        slots[slotIndex].pendingState =
            true;

        slots[slotIndex].candidateState =
            newState;

        slots[slotIndex].candidateSince =
            millis();

        return;
    }

    // Confirm that candidate state remained stable
    if (
        newState ==
        slots[slotIndex].candidateState
    )
    {
        if (
            millis() -
            slots[slotIndex].candidateSince
            >= STATE_CONFIRM_TIME
        )
        {
            slots[slotIndex].occupied =
                newState;

            slots[slotIndex].pendingState =
                false;

            updateLEDs(
                slotIndex
            );

            Serial.print(
                "Slot "
            );

            Serial.print(
                slotIndex + 1
            );

            Serial.print(
                " -> "
            );

            if (newState)
            {
                Serial.println(
                    "OCCUPIED"
                );
            }
            else
            {
                Serial.println(
                    "FREE"
                );
            }
        }
    }
    else
    {
        // Candidate changed, restart confirmation
        slots[slotIndex].candidateState =
            newState;

        slots[slotIndex].candidateSince =
            millis();
    }
}

// ============================================================
// LED CONTROL
// ============================================================

void updateLEDs(
    int slotIndex
)
{
    if (
        slots[slotIndex].occupied
    )
    {
        // Red ON
        digitalWrite(
            slotLEDs[slotIndex].redPin,
            HIGH
        );

        // Green OFF
        digitalWrite(
            slotLEDs[slotIndex].greenPin,
            LOW
        );
    }
    else
    {
        // Green ON
        digitalWrite(
            slotLEDs[slotIndex].greenPin,
            HIGH
        );

        // Red OFF
        digitalWrite(
            slotLEDs[slotIndex].redPin,
            LOW
        );
    }
}

// ============================================================
// SLOT COUNTERS
// ============================================================

int getOccupiedCount()
{
    int count = 0;

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        if (
            slots[i].occupied
        )
        {
            count++;
        }
    }

    return count;
}

int getFreeCount()
{
    return NUM_SLOTS -
           getOccupiedCount();
}

// ============================================================
// OLED DISPLAY
// ============================================================

void updateOLED()
{
    int freeSlots =
        getFreeCount();

    display.clearDisplay();

    display.setTextColor(
        SSD1306_WHITE
    );

    display.setTextSize(1);

    // --------------------------------------------------------
    // Title
    // --------------------------------------------------------

    display.setCursor(
        0,
        0
    );

    display.println(
        "SMART PARKING"
    );

    // --------------------------------------------------------
    // Slot 1
    // --------------------------------------------------------

    display.setCursor(
        0,
        14
    );

    display.print(
        "S1 "
    );

    if (
        slots[0].distance >= 0
    )
    {
        display.print(
            (int)slots[0].distance
        );

        display.print(
            "cm "
        );
    }
    else
    {
        display.print(
            "--cm "
        );
    }

    display.print(
        slots[0].occupied
            ? "O"
            : "F"
    );

    // --------------------------------------------------------
    // Slot 2
    // --------------------------------------------------------

    display.setCursor(
        64,
        14
    );

    display.print(
        "S2 "
    );

    if (
        slots[1].distance >= 0
    )
    {
        display.print(
            (int)slots[1].distance
        );

        display.print(
            "cm "
        );
    }
    else
    {
        display.print(
            "--cm "
        );
    }

    display.print(
        slots[1].occupied
            ? "O"
            : "F"
    );

    // --------------------------------------------------------
    // Slot 3
    // --------------------------------------------------------

    display.setCursor(
        0,
        28
    );

    display.print(
        "S3 "
    );

    if (
        slots[2].distance >= 0
    )
    {
        display.print(
            (int)slots[2].distance
        );

        display.print(
            "cm "
        );
    }
    else
    {
        display.print(
            "--cm "
        );
    }

    display.print(
        slots[2].occupied
            ? "O"
            : "F"
    );

    // --------------------------------------------------------
    // Slot 4
    // --------------------------------------------------------

    display.setCursor(
        64,
        28
    );

    display.print(
        "S4 "
    );

    if (
        slots[3].distance >= 0
    )
    {
        display.print(
            (int)slots[3].distance
        );

        display.print(
            "cm "
        );
    }
    else
    {
        display.print(
            "--cm "
        );
    }

    display.print(
        slots[3].occupied
            ? "O"
            : "F"
    );

    // --------------------------------------------------------
    // Free count
    // --------------------------------------------------------

    display.setCursor(
        0,
        43
    );

    display.print(
        "FREE:"
    );

    display.print(
        freeSlots
    );

    display.print(
        "/"
    );

    display.print(
        NUM_SLOTS
    );

    // --------------------------------------------------------
    // Threshold
    // --------------------------------------------------------

    display.setCursor(
        70,
        43
    );

    display.print(
        "TH:"
    );

    display.print(
        (int)SLOT_THRESHOLD_CM
    );

    // --------------------------------------------------------
    // Gate status
    // --------------------------------------------------------

    display.setCursor(
        0,
        56
    );

    if (
        freeSlots == 0
    )
    {
        display.print(
            "FULL GATE:CLOSED"
        );
    }
    else
    {
        display.print(
            "FREE GATE:OPEN"
        );
    }

    display.display();
}

// ============================================================
// SERIAL OUTPUT
// ============================================================

void printSerialStatus()
{
    Serial.println();

    Serial.println(
        "=============================================="
    );

    Serial.println(
        "          SMART PARKING SYSTEM"
    );

    Serial.println(
        "=============================================="
    );

    Serial.print(
        "Threshold: "
    );

    Serial.print(
        SLOT_THRESHOLD_CM,
        1
    );

    Serial.println(
        " cm"
    );

    Serial.println();

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        Serial.print(
            "Slot "
        );

        Serial.print(
            i + 1
        );

        Serial.print(
            " | Distance: "
        );

        if (
            slots[i].distance >= 0
        )
        {
            Serial.print(
                slots[i].distance,
                1
            );

            Serial.print(
                " cm"
            );
        }
        else
        {
            Serial.print(
                "INVALID"
            );
        }

        Serial.print(
            " | Status: "
        );

        Serial.println(
            slots[i].occupied
                ? "OCCUPIED"
                : "FREE"
        );
    }

    Serial.println();

    Serial.print(
        "Available Slots: "
    );

    Serial.print(
        getFreeCount()
    );

    Serial.print(
        "/"
    );

    Serial.println(
        NUM_SLOTS
    );

    Serial.print(
        "Occupied Slots: "
    );

    Serial.print(
        getOccupiedCount()
    );

    Serial.print(
        "/"
    );

    Serial.println(
        NUM_SLOTS
    );

    if (
        getFreeCount() == 0
    )
    {
        Serial.println(
            "STATUS: PARKING FULL"
        );

        Serial.println(
            "GATE: CLOSED"
        );
    }
    else
    {
        Serial.println(
            "STATUS: SPACE AVAILABLE"
        );

        Serial.println(
            "GATE: OPEN"
        );
    }

    Serial.println(
        "=============================================="
    );
}

// ============================================================
// BUZZER
// ============================================================

void beepFullParking()
{
    // Beep 1
    tone(
        BUZZER_PIN,
        2000
    );

    delay(180);

    noTone(
        BUZZER_PIN
    );

    delay(120);

    // Beep 2
    tone(
        BUZZER_PIN,
        2000
    );

    delay(180);

    noTone(
        BUZZER_PIN
    );
}

// ============================================================
// CAPACITY-BASED SERVO GATE
//
// This is the final behavior because the entry sensor and
// button were removed.
//
// FREE SLOTS > 0 → OPEN
// FREE SLOTS = 0 → CLOSED
//
// The servo is commanded only when the state changes.
// ============================================================

void updateGate()
{
    int freeSlots = getFreeCount();

    // Initialize the gate state once
    if (lastKnownFreeSlots == -1)
    {
        lastKnownFreeSlots = freeSlots;

        // Start gate according to current parking state
        if (freeSlots > 0)
        {
            gateServo.write(GATE_OPEN_ANGLE);
            gateOpen = true;

            Serial.println("INITIAL GATE: OPEN");
        }
        else
        {
            gateServo.write(GATE_CLOSED_ANGLE);
            gateOpen = false;

            Serial.println("INITIAL GATE: CLOSED");
        }

        return;
    }

    // Nothing changed
    if (freeSlots == lastKnownFreeSlots)
    {
        return;
    }

    // Save new parking state
    lastKnownFreeSlots = freeSlots;

    // ========================================================
    // Parking became full
    // ========================================================

    if (freeSlots == 0)
    {
        if (gateOpen)
        {
            gateServo.write(GATE_CLOSED_ANGLE);

            delay(500);

            gateOpen = false;

            Serial.println("GATE -> CLOSED");
        }

        return;
    }

    // ========================================================
    // At least one slot is available
    // ========================================================

    if (freeSlots > 0)
    {
        if (!gateOpen)
        {
            gateServo.write(GATE_OPEN_ANGLE);

            delay(500);

            gateOpen = true;

            Serial.println("GATE -> OPEN");
        }
    }
}

// ============================================================
// WEB DASHBOARD
// ============================================================

String generateWebPage()
{
    int freeSlots =
        getFreeCount();

    int occupiedSlots =
        getOccupiedCount();

    String html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";

    html +=
        "<meta name='viewport' "
        "content='width=device-width, "
        "initial-scale=1'>";

    html +=
        "<meta http-equiv='refresh' "
        "content='2'>";

    html +=
        "<title>Smart Parking Dashboard</title>";

    html += "<style>";

    html +=
        "body{"
        "margin:0;"
        "font-family:Arial,Helvetica,sans-serif;"
        "background:#f4f6f8;"
        "color:#222;"
        "}";

    html +=
        ".header{"
        "background:#1f2937;"
        "color:white;"
        "padding:20px;"
        "text-align:center;"
        "}";

    html +=
        ".container{"
        "max-width:1000px;"
        "margin:25px auto;"
        "padding:15px;"
        "}";

    html +=
        ".summary{"
        "display:grid;"
        "grid-template-columns:repeat(2,1fr);"
        "gap:15px;"
        "margin-bottom:20px;"
        "}";

    html +=
        ".summary-card{"
        "background:white;"
        "border-radius:12px;"
        "padding:20px;"
        "text-align:center;"
        "box-shadow:0 2px 8px rgba(0,0,0,0.1);"
        "}";

    html +=
        ".summary-number{"
        "font-size:36px;"
        "font-weight:bold;"
        "}";

    html +=
        ".slots{"
        "display:grid;"
        "grid-template-columns:repeat(auto-fit,"
        "minmax(200px,1fr));"
        "gap:15px;"
        "}";

    html +=
        ".slot{"
        "background:white;"
        "border-radius:12px;"
        "padding:20px;"
        "box-shadow:0 2px 8px rgba(0,0,0,0.1);"
        "}";

    html +=
        ".free{"
        "border-left:7px solid #16a34a;"
        "}";

    html +=
        ".occupied{"
        "border-left:7px solid #dc2626;"
        "}";

    html +=
        ".status{"
        "font-size:20px;"
        "font-weight:bold;"
        "}";

    html +=
        ".distance{"
        "font-size:18px;"
        "margin-top:8px;"
        "}";

    html +=
        ".footer{"
        "background:white;"
        "margin-top:20px;"
        "padding:18px;"
        "border-radius:12px;"
        "box-shadow:0 2px 8px rgba(0,0,0,0.1);"
        "}";

    html += "</style>";

    html += "</head>";

    html += "<body>";

    // --------------------------------------------------------
    // Header
    // --------------------------------------------------------

    html +=
        "<div class='header'>";

    html +=
        "<h1>SMART PARKING SYSTEM</h1>";

    html +=
        "<p>ESP32 IoT Parking Dashboard</p>";

    html +=
        "</div>";

    html +=
        "<div class='container'>";

    // --------------------------------------------------------
    // Summary
    // --------------------------------------------------------

    html +=
        "<div class='summary'>";

    html +=
        "<div class='summary-card'>";

    html +=
        "<div>Available Slots</div>";

    html +=
        "<div class='summary-number'>";

    html +=
        String(
            freeSlots
        );

    html +=
        "/4</div>";

    html +=
        "</div>";

    html +=
        "<div class='summary-card'>";

    html +=
        "<div>Occupied Slots</div>";

    html +=
        "<div class='summary-number'>";

    html +=
        String(
            occupiedSlots
        );

    html +=
        "/4</div>";

    html +=
        "</div>";

    html +=
        "</div>";

    // --------------------------------------------------------
    // Slot cards
    // --------------------------------------------------------

    html +=
        "<div class='slots'>";

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        if (
            slots[i].occupied
        )
        {
            html +=
                "<div class='slot occupied'>";
        }
        else
        {
            html +=
                "<div class='slot free'>";
        }

        html +=
            "<h2>Slot ";

        html +=
            String(
                i + 1
            );

        html +=
            "</h2>";

        html +=
            "<div class='status'>";

        html +=
            slots[i].occupied
                ? "OCCUPIED"
                : "FREE";

        html +=
            "</div>";

        html +=
            "<div class='distance'>";

        html +=
            "Distance: ";

        if (
            slots[i].distance >= 0
        )
        {
            html +=
                String(
                    slots[i].distance,
                    1
                );

            html +=
                " cm";
        }
        else
        {
            html +=
                "Invalid";
        }

        html +=
            "</div>";

        html +=
            "</div>";
    }

    html +=
        "</div>";

    // --------------------------------------------------------
    // System information
    // --------------------------------------------------------

    html +=
        "<div class='footer'>";

    html +=
        "<h3>System Information</h3>";

    html +=
        "<p>Detection Threshold: ";

    html +=
        String(
            SLOT_THRESHOLD_CM,
            1
        );

    html +=
        " cm</p>";

    if (
        freeSlots == 0
    )
    {
        html +=
            "<p><b>System Status: "
            "PARKING FULL</b></p>";

        html +=
            "<p><b>Gate: CLOSED</b></p>";
    }
    else
    {
        html +=
            "<p><b>System Status: "
            "SPACE AVAILABLE</b></p>";

        html +=
            "<p><b>Gate: OPEN</b></p>";
    }

    html +=
        "</div>";

    html +=
        "</div>";

    html +=
        "</body>";

    html +=
        "</html>";

    return html;
}

// ============================================================
// WEB SERVER ROOT
// ============================================================

void handleRoot()
{
    server.send(
        200,
        "text/html",
        generateWebPage()
    );
}

// ============================================================
// WIFI CONNECTION
// ============================================================

void connectWiFi()
{
    Serial.println(
        "Connecting to Wokwi WiFi..."
    );

    WiFi.mode(
        WIFI_STA
    );

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    unsigned long startTime =
        millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - startTime < 5000
    )
    {
        delay(100);

        Serial.print(
            "."
        );
    }

    Serial.println();

    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        Serial.println(
            "WiFi connected."
        );

        Serial.print(
            "IP Address: "
        );

        Serial.println(
            WiFi.localIP()
        );
    }
    else
    {
        Serial.println(
            "WiFi connection failed."
        );

        Serial.println(
            "Continuing in offline mode."
        );
    }
}

// ============================================================
// SETUP
// ============================================================

void setup()
{
    // --------------------------------------------------------
    // Serial
    // --------------------------------------------------------

    Serial.begin(
        115200
    );

    delay(500);

    Serial.println();

    Serial.println(
        "=============================================="
    );

    Serial.println(
        "          SMART PARKING SYSTEM"
    );

    Serial.println(
        "=============================================="
    );

    Serial.println(
        "System starting..."
    );

    // --------------------------------------------------------
    // Ultrasonic sensors
    // --------------------------------------------------------

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        pinMode(
            sensors[i].trigPin,
            OUTPUT
        );

        pinMode(
            sensors[i].echoPin,
            INPUT
        );

        digitalWrite(
            sensors[i].trigPin,
            LOW
        );
    }

    // --------------------------------------------------------
    // LEDs
    // --------------------------------------------------------

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        pinMode(
            slotLEDs[i].greenPin,
            OUTPUT
        );

        pinMode(
            slotLEDs[i].redPin,
            OUTPUT
        );

        // Initial state = FREE
        slots[i].occupied =
            false;

        slots[i].pendingState =
            false;

        slots[i].candidateState =
            false;

        slots[i].candidateSince =
            0;

        slots[i].distance =
            -1.0;

        updateLEDs(
            i
        );
    }

    // --------------------------------------------------------
    // Buzzer
    // --------------------------------------------------------

    pinMode(
        BUZZER_PIN,
        OUTPUT
    );

    noTone(
        BUZZER_PIN
    );

    // --------------------------------------------------------
    // Servo
    // --------------------------------------------------------

   gateServo.setPeriodHertz(50);

gateServo.attach(
    SERVO_PIN,
    500,
    2400
);

gateServo.write(
    GATE_CLOSED_ANGLE
);

gateOpen = false;
lastKnownFreeSlots = -1;
    // --------------------------------------------------------
    // OLED
    // --------------------------------------------------------

    Wire.begin(
        21,
        22
    );

    if (
        !display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS
        )
    )
    {
        Serial.println(
            "OLED ERROR!"
        );
    }
    else
    {
        Serial.println(
            "OLED initialized."
        );

        display.clearDisplay();

        display.setTextColor(
            SSD1306_WHITE
        );

        display.setTextSize(1);

        display.setCursor(
            0,
            0
        );

        display.println(
            "SMART PARKING"
        );

        display.println();

        display.println(
            "Initializing..."
        );

        display.display();

        delay(1000);
    }

    // --------------------------------------------------------
    // Wi-Fi
    // --------------------------------------------------------

    connectWiFi();

    // --------------------------------------------------------
    // Web server
    // --------------------------------------------------------

    server.on(
        "/",
        handleRoot
    );

    server.begin();

    Serial.println(
        "Web server started."
    );

    // --------------------------------------------------------
    // Initial display
    // --------------------------------------------------------

    updateOLED();

    // --------------------------------------------------------
    // Initial gate decision
    // --------------------------------------------------------

    updateGate();

    Serial.println(
        "System ready."
    );

    Serial.print(
        "Threshold = "
    );

    Serial.print(
        SLOT_THRESHOLD_CM
    );

    Serial.println(
        " cm"
    );
}

// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
    // --------------------------------------------------------
    // Web server
    // --------------------------------------------------------

    server.handleClient();

    // --------------------------------------------------------
    // Read the four parking sensors
    // --------------------------------------------------------

    for (
        int i = 0;
        i < NUM_SLOTS;
        i++
    )
    {
        float distance =
            readDistanceCM(
                sensors[i].trigPin,
                sensors[i].echoPin
            );

        if (
            distance >= 0
        )
        {
            updateSlotState(
                i,
                distance
            );
        }

        // Small delay reduces sensor cross-talk
        delay(5);
    }

    // --------------------------------------------------------
    // Servo gate
    // --------------------------------------------------------

    updateGate();

    // --------------------------------------------------------
    // OLED
    // --------------------------------------------------------

    static unsigned long lastDisplay =
        0;

    if (
        millis() - lastDisplay
        >= DISPLAY_UPDATE_INTERVAL
    )
    {
        updateOLED();

        lastDisplay =
            millis();
    }

    // --------------------------------------------------------
    // Serial
    // --------------------------------------------------------

    static unsigned long lastSerial =
        0;

    if (
        millis() - lastSerial
        >= SERIAL_UPDATE_INTERVAL
    )
    {
        printSerialStatus();

        lastSerial =
            millis();
    }

    // --------------------------------------------------------
    // Full parking buzzer
    // --------------------------------------------------------

    static unsigned long lastFullBeep =
        0;

    if (
        getFreeCount() == 0
    )
    {
        if (
            millis() - lastFullBeep
            >= FULL_BEEP_INTERVAL
        )
        {
            beepFullParking();

            lastFullBeep =
                millis();
        }
    }

    delay(5);
}