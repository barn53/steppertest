#if 0

#include "Ucglib.h"
#include <Arduino.h>
#include <SPI.h>

Ucglib_SSD1331_18x96x64_UNIVISION_SWSPI ucg(/*scl=*/10, /*sda=*/8, /*cd=*/2, /*cs=*/3, /*reset=*/1);

void setup(void)
{
    delay(1000);
    ucg.begin(UCG_FONT_MODE_TRANSPARENT);
    ucg.setFont(ucg_font_5x7_8f);
    ucg.clearScreen();
}

void loop(void)
{
    ucg.setColor(random(0, 256), random(0, 256), random(0, 256));
    ucg.setPrintPos(0, 10);
    ucg.print("Hello World!");
    delay(500);
}

#endif
