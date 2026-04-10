#if 0

#include <TMC2209.h>

HardwareSerial& serial_stream = Serial0;

TMC2209 driver;

const int32_t VELOCITY = 50000;

static void dumpDriverState(const char* label)
{
    auto s = driver.getSettings();
    auto st = driver.getStatus();
    auto gs = driver.getGlobalStatus();

    Serial.printf(
        "\n[%s]\n"
        "comm=%d setup=%d hwDis=%d swEn=%d micro=%u stealth=%d standstillMode=%u\n"
        "irun%%=%u irunReg=%u ihold%%=%u iholdReg=%u iholdDelay%%=%u iholdDelayReg=%u\n"
        "autoScale=%d autoGrad=%d pwmOffset=%u pwmGrad=%u coolStep=%d\n"
        "analogCurrent=%d internalRsense=%d ifcnt=%u tstep=%lu\n"
        "drv: cs=%u stealthMode=%u standstill=%u otw=%u ots=%u s2ga=%u s2gb=%u ola=%u olb=%u\n"
        "pwm: scaleSum=%u scaleAuto=%d offsetAuto=%u gradAuto=%u\n"
        "gstat: reset=%u drv_err=%u uv_cp=%u\n",
        label,
        s.is_communicating,
        s.is_setup,
        driver.hardwareDisabled(),
        s.software_enabled,
        s.microsteps_per_step,
        s.stealth_chop_enabled,
        s.standstill_mode,
        s.irun_percent,
        s.irun_register_value,
        s.ihold_percent,
        s.ihold_register_value,
        s.iholddelay_percent,
        s.iholddelay_register_value,
        s.automatic_current_scaling_enabled,
        s.automatic_gradient_adaptation_enabled,
        s.pwm_offset,
        s.pwm_gradient,
        s.cool_step_enabled,
        s.analog_current_scaling_enabled,
        s.internal_sense_resistors_enabled,
        driver.getInterfaceTransmissionCounter(),
        (unsigned long)driver.getInterstepDuration(),
        st.current_scaling,
        st.stealth_chop_mode,
        st.standstill,
        st.over_temperature_warning,
        st.over_temperature_shutdown,
        st.short_to_ground_a,
        st.short_to_ground_b,
        st.open_load_a,
        st.open_load_b,
        driver.getPwmScaleSum(),
        driver.getPwmScaleAuto(),
        driver.getPwmOffsetAuto(),
        driver.getPwmGradientAuto(),
        gs.reset,
        gs.drv_err,
        gs.uv_cp);
}

void testCurrent(uint16_t mA)
{
    driver.setRMSCurrent(mA, 0.110f, 1.0f);

    driver.moveAtVelocity(0);
    dumpDriverState("stopped");
    delay(3000);

    driver.moveAtVelocity(VELOCITY);
    dumpDriverState("moving");
    delay(4000);

    driver.moveAtVelocity(0);
    dumpDriverState("stopped_again");
    delay(1);
}

void setup()
{
    Serial.begin();

    driver.setup(serial_stream);
    driver.setHardwareEnablePin(5);
    driver.useExternalSenseResistors();
    if (false) {
        driver.enableStealthChop();
        driver.enableAutomaticCurrentScaling();
        driver.enableAutomaticGradientAdaptation();
        driver.setPowerDownDelay(20);
    } else {
        driver.disableStealthChop();
    }
    driver.enableCoolStep();
    driver.setMicrostepsPerStep(64);
    driver.setHoldDelay(100);
    driver.setStandstillMode(TMC2209::STRONG_BRAKING);
    driver.enable();
    delay(5000);

    dumpDriverState("fresh");
}

void loop()
{
    Serial.println("\n=== 500 mA test ===");
    testCurrent(500);

    delay(3000);
    Serial.println("\n=== 1200 mA test ===");
    testCurrent(1200);
}

#endif
