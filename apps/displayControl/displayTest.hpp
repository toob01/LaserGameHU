#include "crt_CleanRTOS.h"
#include "display.hpp"

namespace crt{
    MainInits mainInits;
    display display1("display1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}