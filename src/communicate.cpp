#if 0

#include <TMC2209.h>

HardwareSerial& serial_stream = Serial0;

const int DELAY = 3000;
const uint8_t PIN_MOTOR_EN = 5;
const long DRIVER_BAUD_RATE = 115200;

// Instantiate TMC2209
TMC2209 stepper_driver;

void setup()
{
    Serial.begin();

    stepper_driver.setup(serial_stream, DRIVER_BAUD_RATE);
    stepper_driver.setHardwareEnablePin(PIN_MOTOR_EN);
    stepper_driver.enable();
    stepper_driver.clearReset();
    stepper_driver.setMicrostepsPerStep(16);
}

void loop()
{
    auto settings = stepper_driver.getSettings();
    auto status = stepper_driver.getGlobalStatus();

    Serial.printf("comm=%d setup=%d hwDis=%d reset=%d drv_err=%d uv_cp=%d micro=%u\n",
        settings.is_communicating,
        settings.is_setup,
        stepper_driver.hardwareDisabled(),
        status.reset,
        status.drv_err,
        status.uv_cp,
        settings.microsteps_per_step);

    if (stepper_driver.isSetupAndCommunicating()) {
        Serial.println("Stepper driver is setup and communicating.");
    } else if (stepper_driver.isCommunicatingButNotSetup()) {
        Serial.println("Stepper driver is communicating but not fully setup.");
    } else {
        Serial.println("Stepper driver is not communicating.");
    }

    Serial.println();
    delay(DELAY);
}

#endif
