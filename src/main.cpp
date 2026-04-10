#if 0

// Stepper Motor Controller Library:
// https://github.com/gin66/FastAccelStepper
//

#include "Ucglib.h"
#include <Arduino.h>
#include <TMC2209.h>

const uint32_t SERIAL_BAUD_RATE = 460800;

const uint8_t PIN_MOTOR_EN = 5;
const uint8_t PIN_MOTOR_STEP = 6;
const uint8_t PIN_MOTOR_DIR = 7;

int32_t vactual = 1000;

Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*scl=*/10, /*sda=*/8, /*cd=*/2, /*cs=*/3, /*reset=*/1);

TMC2209 stepper_driver;
HardwareSerial& serial_stream = Serial0;

void print(const char* str)
{
    ucg.clearScreen();
    ucg.setColor(random(0, 256), random(0, 256), random(0, 256));
    ucg.setPrintPos(0, 10);
    Serial.printf("%s\n", str);
    ucg.print(str);
}

void print(const char* str, int32_t value)
{
    ucg.clearScreen();
    ucg.setColor(random(0, 256), random(0, 256), random(0, 256));
    ucg.setPrintPos(0, 10);
    Serial.printf("%s %d\n", str, value);
    ucg.print(str);
    ucg.print(value);
}

void blah()
{
    delay(1000);
    stepper_driver.setup(serial_stream, SERIAL_BAUD_RATE);
    stepper_driver.setHardwareEnablePin(PIN_MOTOR_EN);
    stepper_driver.setRunCurrent(100);
    stepper_driver.setHoldCurrent(20);
    stepper_driver.enableAutomaticCurrentScaling();
    stepper_driver.enableAutomaticGradientAdaptation();
    stepper_driver.enableCoolStep();
    stepper_driver.enableDoubleEdge();
    stepper_driver.enableStealthChop();
    stepper_driver.enable();
    stepper_driver.setMicrostepsPerStep(8);
    // stepper_driver.moveAtVelocity(vactual);
    stepper_driver.moveUsingStepDirInterface();

    print("blah()");
}

void setup()
{
    pinMode(PIN_MOTOR_STEP, OUTPUT);
    pinMode(PIN_MOTOR_DIR, OUTPUT);
    digitalWrite(PIN_MOTOR_STEP, LOW);
    digitalWrite(PIN_MOTOR_DIR, LOW);

    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) {
        delay(10);
    }

    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
    ucg.clearScreen();
    ucg.setFont(ucg_font_4x6_mr);
    delay(500);
    print("setup()");
    delay(2000);
    blah();
    delay(1000);
    uint16_t ms = stepper_driver.getMicrostepsPerStep();
    print("Microsteps: ", ms);
}

uint32_t count = 0;
void loopAAAAA()
{
    delay(10);
    digitalWrite(PIN_MOTOR_STEP, !digitalRead(PIN_MOTOR_STEP));

    if (count % 1000 == 0) {
        uint16_t msc = stepper_driver.getMicrostepCounter();
        print("Microstep Counter: ", msc);
    }

    // if (count > 1000) {
    //     stepper_driver.disable();
    //     return;
    // }
    // if (count > 1000) {
    //     count = 0;
    //     // digitalWrite(PIN_MOTOR_DIR, !digitalRead(PIN_MOTOR_DIR));
    // }
    ++count;
}

void loop()
{
    if (!stepper_driver.isSetupAndCommunicating()) {
        if (stepper_driver.isCommunicatingButNotSetup()) {
        } else {
            print("not setup and not communicating!");
            delay(2000);
            return;
        }
    }

    bool hardware_disabled = stepper_driver.hardwareDisabled();
    TMC2209::Settings settings = stepper_driver.getSettings();
    TMC2209::Status status = stepper_driver.getStatus();
    uint16_t ms = stepper_driver.getMicrostepsPerStep();

    if (hardware_disabled) {
        print("hardware disabled!");
    } else if (!settings.software_enabled) {
        print("software disabled!");
    } else if ((!status.standstill)) {

        stepper_driver.moveAtVelocity(vactual);

        print("Moving at velocity ", vactual);
        // print("Microsteps: ", ms);
        // print("Current: ", settings.irun_percent);

        uint32_t interstep_duration = stepper_driver.getInterstepDuration();
        // print("interstep_duration ", interstep_duration);

        // print("temp warning: ", status.over_temperature_warning);
        // print("temp shutdown: ", status.over_temperature_shutdown);

    } else {
        print("Not moving, something is wrong!");
    }

    delay(500);

    vactual *= -1;
}

#endif
