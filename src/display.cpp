#if 0

#include "Ucglib.h"
#include <Arduino.h>
#include <SPI.h>

Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*sclk=*/1, /*data=*/4, /*cd=*/2, /*cs=*/5, /*reset=*/3);


void setup(void)
{
    delay(1000);
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
    ucg.setFont(ucg_font_ncenR14_hr);
    ucg.clearScreen();
}

void loop(void)
{
}

#endif
