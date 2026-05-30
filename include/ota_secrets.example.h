#pragma once

// Copy this file to include/ota_secrets.h and fill in your local values.
// include/ota_secrets.h is ignored by git.

#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define OTA_HOSTNAME "steppertest"

// Leave empty to disable OTA password authentication.
// If you set this, also add the matching --auth upload flag in platformio.ini.
#define OTA_PASSWORD ""
