#include "crt_CleanRTOS.h"
#include "displaySSD1306.hpp"

namespace crt{
    MainInits mainInits;
    shooting shooting1("shooting1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}