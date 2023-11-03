#include "crt_CleanRTOS.h"
#include "speaker.hpp"

namespace crt{
    MainInits mainInits;
    speaker speaker1("speaker1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}