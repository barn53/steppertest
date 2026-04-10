#if 0

#include <Arduino.h>


// Pins for TMC2209
const uint8_t PIN_EN = 5; // Enable (active low)
const uint8_t PIN_STEP = 6; // Step pulse
const uint8_t PIN_DIR = 7; // Direction

void setup()
{
    // Set pin modes
    pinMode(PIN_EN, OUTPUT);
    pinMode(PIN_STEP, OUTPUT);
    pinMode(PIN_DIR, OUTPUT);

    // Enable driver (EN low)
    digitalWrite(PIN_EN, LOW);

    // Set direction (e.g., forward)
    digitalWrite(PIN_DIR, HIGH);

    // Microstepping: MS1/MS2 not connected, so defaults to 8 microsteps
    // Adjust current via potentiometer on TMC2209 board
}

void loop()
{
    // Generate STEP pulses for ~60 RPM
    // NEMA 17: 200 full steps/rev
    // Assuming 8 microsteps (default when MS pins not connected): 200 * 8 = 1600 microsteps/rev
    // 60 RPM = 1 rev/sec = 1600 microsteps/sec
    // Pulse frequency: ~1600 Hz
    // Approximate with 1600 Hz pulses

    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(500); // Pulse high for 300 us
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(500); // Pulse low for 300 us

    // Total period: 600 us = ~1667 Hz (close to 1600 Hz)
    // Actual RPM will be ~60; adjust delay if needed
}

#endif
