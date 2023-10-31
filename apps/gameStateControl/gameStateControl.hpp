#include "crt_CleanRTOS.h"
#include "recievingHit.hpp"

namespace crt{
    MainInits mainInits;
    gameState gameState1("gameState1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}