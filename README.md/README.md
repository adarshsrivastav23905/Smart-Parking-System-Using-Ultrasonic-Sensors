# Smart Parking System using Ultrasonic Sensors

## Overview

An Arduino UNO based embedded smart parking system that monitors four parking slots using HC-SR04 ultrasonic sensors.

The system detects whether each parking slot is free or occupied, calculates available spaces, displays status on a 16x2 I2C LCD, provides LED and buzzer alerts, and controls a servo-based parking barrier.

## Features

- Four parking slots
- Four HC-SR04 ultrasonic sensors
- Automatic occupancy detection
- Available-slot counting
- 16x2 I2C LCD
- Green availability indicator
- Red parking-full indicator
- Buzzer alert
- Servo barrier gate
- Serial Monitor diagnostics
- PlatformIO development
- Wokwi virtual simulation

## Technology

- Arduino UNO
- Embedded C/C++
- PlatformIO
- Wokwi
- HC-SR04
- I2C LCD
- Servo