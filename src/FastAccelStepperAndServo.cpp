#if 1

#include "ota_support.h"

#include <Arduino.h>
#include <ESP32Servo.h>
#include <FastAccelStepper.h>
#include <TMC2209.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

HardwareSerial& serial_stream = Serial0;
TMC2209 stepper_driver;

// My Servo: https://towerpro.com.tw/product/mg90s-3/
Servo servo;

constexpr static uint8_t EN_PIN = 5;
constexpr static uint8_t STEP_PIN = 6;
constexpr static uint8_t DIR_PIN = 7;

constexpr static uint8_t SERVO_PIN = 4;

constexpr static uint32_t DELAY = 2000;

constexpr static uint16_t MICROSTEPS_PER_STEP = 4;

void setup()
{
    Serial.begin();
    otaBegin();

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    ///  FastAccelStepper
    engine.init();
    stepper = engine.stepperConnectToPin(STEP_PIN);
    if (stepper) {
        stepper->setDirectionPin(DIR_PIN);

        stepper->setSpeedInUs(500); // the parameter is us/step !!!
        stepper->setAcceleration(200);
    }
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////

    stepper_driver.setup(serial_stream);
    stepper_driver.setHardwareEnablePin(EN_PIN);

    servo.setPeriodHertz(50);
    servo.attach(SERVO_PIN, 500, 2400);

    otaDelay(DELAY);

    stepper_driver.enableStealthChop();
    stepper_driver.enableAutomaticCurrentScaling();
    stepper_driver.enableAutomaticGradientAdaptation();
    stepper_driver.setPowerDownDelay(20);
    stepper_driver.enableCoolStep();
    stepper_driver.setRMSCurrent(500, 0.11f);
    stepper_driver.setMicrostepsPerStep(MICROSTEPS_PER_STEP);
    stepper_driver.moveUsingStepDirInterface();
    stepper_driver.enable();
}

int32_t move = 20000;
int angles[] = { 0, 20, 40, 90, 120, 180 };
int angleIndex = 0;
void loop()
{
    otaHandle();

    servo.write(angles[angleIndex++ % (sizeof(angles) / sizeof(angles[0]))]);

    if (not stepper_driver.isSetupAndCommunicating()) {
        Serial.println("Stepper driver not setup and communicating!");
        otaDelay(DELAY);
        return;
    }

    if (!stepper->isRunning()) {
        stepper->move(move);
        move = -move;
    }

    otaDelay(DELAY);
}

#endif
