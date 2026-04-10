#if 1

#include <FastAccelStepper.h>
#include <TMC2209.h>

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* stepper = NULL;

HardwareSerial& serial_stream = Serial0;
TMC2209 stepper_driver;

constexpr static uint8_t EN_PIN = 5;
constexpr static uint8_t STEP_PIN = 6;
constexpr static uint8_t DIR_PIN = 7;

constexpr static uint32_t DELAY = 2000;

constexpr static uint16_t MICROSTEPS_PER_STEP = 4;

void setup()
{
    Serial.begin();

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

    delay(DELAY);

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
void loop()
{
    if (not stepper_driver.isSetupAndCommunicating()) {
        Serial.println("Stepper driver not setup and communicating!");
        delay(DELAY);
        return;
    }

    if (!stepper->isRunning()) {
        stepper->move(move);
        move = -move;
    }

    delay(DELAY);
}

#endif
