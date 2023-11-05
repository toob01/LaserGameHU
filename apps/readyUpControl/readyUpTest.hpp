#include "crt_CleanRTOS.h"
#include "readyUp.hpp"

namespace crt{
    MainInits mainInits;
    shooting readyUp1("readyUp1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}