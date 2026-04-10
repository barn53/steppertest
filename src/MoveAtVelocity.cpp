#if 0

#include <TMC2209.h>

HardwareSerial& serial_stream = Serial0;
TMC2209 stepper_driver;

const int DELAY = 2000;

const uint16_t MICROSTEPS_PER_STEP = 1;
const float RPM = 10.f;

int32_t rpmToVelocity(float rpm, uint16_t microstepsPerStep)
{
    uint16_t fullStepsPerRev = 200; // motor
    const float fclk = 12000000.0f; // TMC2209 internal clock frequency, see datasheet 4.1.2
    return (int32_t)((rpm * microstepsPerStep * fullStepsPerRev * 16777216.0f) / (60.0f * fclk) + 0.5f);
}

const int32_t velocity = rpmToVelocity(RPM, MICROSTEPS_PER_STEP);
void setup()
{
    Serial.begin();

    stepper_driver.setup(serial_stream);
    stepper_driver.setHardwareEnablePin(5);

    delay(DELAY);

    stepper_driver.enableStealthChop();
    stepper_driver.enableAutomaticCurrentScaling();
    stepper_driver.enableAutomaticGradientAdaptation();
    stepper_driver.setPowerDownDelay(20);
    stepper_driver.enableCoolStep();
    stepper_driver.setRMSCurrent(250, 0.11f);
    stepper_driver.setMicrostepsPerStep(1);
    stepper_driver.moveAtVelocity(velocity);

    stepper_driver.enable();
}

void loop()
{
    if (not stepper_driver.isSetupAndCommunicating()) {
        Serial.println("Stepper driver not setup and communicating!");
        return;
    }

    bool hardware_disabled = stepper_driver.hardwareDisabled();
    TMC2209::Settings settings = stepper_driver.getSettings();
    TMC2209::Status status = stepper_driver.getStatus();

    if (hardware_disabled) {
        Serial.println("Stepper driver is hardware disabled!");
    } else if (!settings.software_enabled) {
        Serial.println("Stepper driver is software disabled!");
    } else if ((!status.standstill)) {
        Serial.print("Moving at velocity ");
        Serial.println(velocity);
    }

    Serial.println();
    delay(DELAY);
}

#endif
