#include <Arduino.h>

#include <NeoPixelBus.h>
#include <TMC2209.h>

// This example will not work on Arduino boards without HardwareSerial ports,
// such as the Uno, Nano, and Mini.
//
// See this reference for more details:
// https://www.arduino.cc/reference/en/language/functions/communication/serial/

const uint32_t SERIAL_BAUD_RATE = 460800;
const int32_t VELOCITY = 1000;
const int32_t MICROSTEPS = 8;
TMC2209 stepper_driver;
HardwareSerial& serial_stream = Serial0;

NeoPixelBus<NeoRgbFeature, NeoWs2812xMethod> strip(1, 7);

void init()
{
    strip.SetPixelColor(0, RgbColor(0, 0, 3));
    strip.Show();
    delay(1000);

    Serial.println("init()");
    stepper_driver.setup(serial_stream, SERIAL_BAUD_RATE);
    stepper_driver.setRunCurrent(50);
    stepper_driver.enableCoolStep();
    stepper_driver.setMicrostepsPerStep(MICROSTEPS);
    stepper_driver.enable();
    stepper_driver.moveAtVelocity(VELOCITY);
}

void setup()
{
    strip.Begin();
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) {
        delay(10);
    }

    delay(4000);

    init();

    strip.SetPixelColor(0, RgbColor(3, 0, 3));
    strip.Show();
    delay(1000);
}

int32_t vactual = VELOCITY;
void loop()
{
    strip.SetPixelColor(0, RgbColor(3, 3, 0));
    strip.Show();
    delay(500);

    if (!stepper_driver.isSetupAndCommunicating()) {
        if (stepper_driver.isCommunicatingButNotSetup()) {
            Serial.println("Stepper driver is communicating but not setup!");
            init();
        } else {
            Serial.println("Stepper driver is not setup and not communicating!");
        }
        strip.SetPixelColor(0, RgbColor(3, 0, 0));
        strip.Show();
        delay(2000);
        return;
    }

    stepper_driver.setRunCurrent(50);

    bool hardware_disabled = stepper_driver.hardwareDisabled();
    TMC2209::Settings settings = stepper_driver.getSettings();
    TMC2209::Status status = stepper_driver.getStatus();
    uint16_t ms = stepper_driver.getMicrostepsPerStep();

    if (hardware_disabled) {
        Serial.println("Stepper driver is hardware disabled!");
    } else if (!settings.software_enabled) {
        Serial.println("Stepper driver is software disabled!");
    } else if ((!status.standstill)) {
        stepper_driver.setMicrostepsPerStep(MICROSTEPS);
        stepper_driver.moveAtVelocity(vactual);

        Serial.print("Moving at velocity ");
        Serial.print(vactual);

        Serial.print(", Microsteps: ");
        Serial.print(ms);

        Serial.print(", Current: ");
        Serial.print(settings.irun_percent);

        uint32_t interstep_duration = stepper_driver.getInterstepDuration();
        Serial.print(" which is equal to an interstep_duration of ");
        Serial.print(interstep_duration);

        Serial.print(" temp warning: ");
        Serial.print(status.over_temperature_warning);
        Serial.print(" temp shutdown: ");
        Serial.println(status.over_temperature_shutdown);

    } else {
        Serial.println("Not moving, something is wrong!");

        stepper_driver.enable();
        stepper_driver.moveAtVelocity(vactual);
    }

    Serial.println();

    strip.SetPixelColor(0, RgbColor(0, 3, 0));
    strip.Show();
    delay(5000);

    vactual *= -1;
}
