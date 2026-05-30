#include "ota_support.h"

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

#if __has_include("ota_secrets.h")
#include "ota_secrets.h"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

#ifndef OTA_HOSTNAME
#define OTA_HOSTNAME "steppertest"
#endif

#ifndef OTA_PASSWORD
#define OTA_PASSWORD ""
#endif

namespace {
constexpr uint32_t WIFI_CONNECT_TIMEOUT_MS = 15000;
constexpr uint32_t WIFI_CONNECT_POLL_MS = 250;
constexpr uint32_t OTA_DELAY_POLL_MS = 10;

const char* wifiSsid = WIFI_SSID;
const char* wifiPassword = WIFI_PASSWORD;
const char* otaHostname = OTA_HOSTNAME;
const char* otaPassword = OTA_PASSWORD;

bool otaReady = false;

bool hasValue(const char* value)
{
    return value != nullptr && value[0] != '\0';
}

void printOtaError(ota_error_t error)
{
    Serial.printf("[OTA] Error %u: ", error);

    switch (error) {
    case OTA_AUTH_ERROR:
        Serial.println("auth failed");
        break;
    case OTA_BEGIN_ERROR:
        Serial.println("begin failed");
        break;
    case OTA_CONNECT_ERROR:
        Serial.println("connect failed");
        break;
    case OTA_RECEIVE_ERROR:
        Serial.println("receive failed");
        break;
    case OTA_END_ERROR:
        Serial.println("end failed");
        break;
    default:
        Serial.println("unknown");
        break;
    }
}
}

void otaBegin()
{
    if (!hasValue(wifiSsid)) {
        Serial.println("[OTA] Set WIFI_SSID in include/ota_secrets.h to enable OTA.");
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(wifiSsid, wifiPassword);

    Serial.print("[OTA] Connecting to WiFi");
    const uint32_t startedAt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startedAt < WIFI_CONNECT_TIMEOUT_MS) {
        Serial.print(".");
        delay(WIFI_CONNECT_POLL_MS);
    }
    Serial.println();

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[OTA] WiFi connection failed; continuing without OTA.");
        return;
    }

    ArduinoOTA.setHostname(otaHostname);
    if (hasValue(otaPassword)) {
        ArduinoOTA.setPassword(otaPassword);
    }

    ArduinoOTA
        .onStart([]() {
            Serial.println("[OTA] Update started");
        })
        .onEnd([]() {
            Serial.println();
            Serial.println("[OTA] Update finished");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            if (total > 0) {
                Serial.printf("[OTA] Progress: %u%%\r", (progress * 100U) / total);
            }
        })
        .onError([](ota_error_t error) {
            printOtaError(error);
        });

    ArduinoOTA.begin();
    otaReady = true;

    Serial.print("[OTA] Ready at ");
    Serial.println(WiFi.localIP());
}

void otaHandle()
{
    if (otaReady) {
        ArduinoOTA.handle();
    }
}

void otaDelay(uint32_t durationMs)
{
    const uint32_t startedAt = millis();
    while (millis() - startedAt < durationMs) {
        otaHandle();

        const uint32_t elapsedMs = millis() - startedAt;
        const uint32_t remainingMs = durationMs - elapsedMs;
        delay(remainingMs < OTA_DELAY_POLL_MS ? remainingMs : OTA_DELAY_POLL_MS);
    }
}

bool otaIsReady()
{
    return otaReady;
}
