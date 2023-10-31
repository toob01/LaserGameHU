#include "crt_CleanRTOS.h"
#include "recievingHit.hpp"

namespace crt{
    MainInits mainInits;
    recievingHit recievingHit1("recievingHit1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}