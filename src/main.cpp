#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// =====================================================
// SMART PARKING SYSTEM
// Arduino UNO + 4 HC-SR04 + I2C LCD + LEDs + Buzzer
// + Servo Gate
// =====================================================

// -----------------------------
// LCD
// -----------------------------
LiquidCrystal_I2C lcd(0x20, 16, 2);

// -----------------------------
// Ultrasonic sensors
// -----------------------------
const byte TRIG_PINS[4] = {2, 4, 6, 8};
const byte ECHO_PINS[4] = {3, 5, 7, 9};

// -----------------------------
// Indicators
// -----------------------------
const byte GREEN_LED = 10;
const byte RED_LED   = 11;
const byte BUZZER    = 12;

// -----------------------------
// Servo
// -----------------------------
const byte SERVO_PIN = A0;

Servo gateServo;

const int GATE_CLOSED = 0;
const int GATE_OPEN   = 90;

// -----------------------------
// Parking configuration
// -----------------------------
const byte TOTAL_SLOTS = 4;
const float OCCUPIED_THRESHOLD =15.0;

// -----------------------------
// Timing
// -----------------------------
unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 1000;

// =====================================================
// Read HC-SR04 distance
// =====================================================

float readDistance(byte trigPin, byte echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);

    unsigned long duration =
        pulseIn(echoPin, HIGH, 30000);

    if (duration == 0)
    {
        return 999.0;
    }

    return duration / 58.0;
}

// =====================================================
// Display slot status
// =====================================================

void showSlotStatus(bool occupied[])
{
    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("S1:");
    lcd.print(occupied[0] ? "Occupied" : "Free");

    lcd.print(" S2:");
    lcd.print(occupied[1] ? "Occupied" : "Free");

    lcd.setCursor(0, 1);

    lcd.print("S3:");
    lcd.print(occupied[2] ? "Occupied" : "Free");

    lcd.print(" S4:");
    lcd.print(occupied[3] ? "Occupied" : "Free");
}

// =====================================================
// Display available slots
// =====================================================

void showAvailable(int available)
{
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Available: ");
    lcd.print(available);
    lcd.print("/4");

    lcd.setCursor(0, 1);

    if (available == 0)
    {
        lcd.print("PARKING FULL");
    }
    else
    {
        lcd.print("SPACE AVAILABLE");
    }
}

// =====================================================
// Print status to Serial Monitor
// =====================================================

void printStatus(float distances[], bool occupied[], int available)
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("     SMART PARKING SYSTEM");
    Serial.println("==============================");

    for (byte i = 0; i < 4; i++)
    {
        Serial.print("Slot ");
        Serial.print(i + 1);

        Serial.print(" | Distance: ");
        Serial.print(distances[i]);
        Serial.print(" cm | ");

        if (occupied[i])
        {
            Serial.println("OCCUPIED");
        }
        else
        {
            Serial.println("FREE");
        }
    }

    Serial.print("Available slots: ");
    Serial.println(available);

    if (available == 0)
    {
        Serial.println("STATUS: PARKING FULL");
    }
    else
    {
        Serial.println("STATUS: SPACE AVAILABLE");
    }

    Serial.println("==============================");
}

// =====================================================
// LED + buzzer control
// =====================================================

void updateIndicators(int available)
{
    if (available > 0)
    {
        // At least one slot available
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        noTone(BUZZER);
    }
    else
    {
        // All slots occupied
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        tone(BUZZER, 1000);
    }
}

// =====================================================
// Test LEDs at startup
// =====================================================

void testLEDs()
{
    // Green LED test
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(1000);

    // Red LED test
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    delay(1000);

    // Both OFF
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
    Serial.begin(9600);

    // -----------------------------
    // Ultrasonic pins
    // -----------------------------
    for (byte i = 0; i < 4; i++)
    {
        pinMode(TRIG_PINS[i], OUTPUT);
        pinMode(ECHO_PINS[i], INPUT);

        digitalWrite(TRIG_PINS[i], LOW);
    }

    // -----------------------------
    // LEDs
    // -----------------------------
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    // -----------------------------
    // Buzzer
    // -----------------------------
    pinMode(BUZZER, OUTPUT);
    noTone(BUZZER);

    // -----------------------------
    // Servo
    // -----------------------------
    gateServo.attach(SERVO_PIN);
    gateServo.write(GATE_CLOSED);

    // -----------------------------
    // LCD
    // -----------------------------
    Wire.begin();

    lcd.init();
    lcd.backlight();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("SMART PARKING");

    lcd.setCursor(0, 1);
    lcd.print("SYSTEM STARTING");

    // -----------------------------
    // LED startup test
    // -----------------------------
    testLEDs();

    lcd.clear();

    Serial.println();
    Serial.println("SMART PARKING SYSTEM");
    Serial.println("System started");
    Serial.println("Gate: CLOSED");
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
    bool occupied[4];
    float distances[4];

    int available = 0;

    // -----------------------------
    // Read all sensors
    // -----------------------------
    for (byte i = 0; i < 4; i++)
    {
        distances[i] =
            readDistance(
                TRIG_PINS[i],
                ECHO_PINS[i]
            );

        occupied[i] =
            (distances[i] < OCCUPIED_THRESHOLD);

        if (!occupied[i])
        {
            available++;
        }

        // Prevent ultrasonic interference
        delay(80);
    }

    // -----------------------------
    // Indicators
    // -----------------------------
    updateIndicators(available);

    // -----------------------------
    // Gate logic
    // -----------------------------
    static bool firstScan = true;
    static bool previousOccupied[4] =
    {
        false,
        false,
        false,
        false
    };

    bool newVehicleDetected = false;

    // Do not open gate during initial startup scan
    if (!firstScan)
    {
        for (byte i = 0; i < 4; i++)
        {
            if (occupied[i] && !previousOccupied[i])
            {
                newVehicleDetected = true;
            }
        }

        if (newVehicleDetected && available > 0)
        {
            Serial.println("Vehicle detected");
            Serial.println("Gate OPEN");

            gateServo.write(GATE_OPEN);

            delay(2000);

            gateServo.write(GATE_CLOSED);

            Serial.println("Gate CLOSED");
        }
    }

    // Save states
    for (byte i = 0; i < 4; i++)
    {
        previousOccupied[i] = occupied[i];
    }

    firstScan = false;

    // -----------------------------
    // LCD
    // -----------------------------
    if (millis() - lastDisplayUpdate >= DISPLAY_INTERVAL)
    {
        lastDisplayUpdate = millis();

        showSlotStatus(occupied);

        delay(700);

        showAvailable(available);
    }

    // -----------------------------
    // Serial Monitor
    // -----------------------------
    printStatus(
        distances,
        occupied,
        available
    );

    delay(500);
}