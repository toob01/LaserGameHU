#include "crt_CleanRTOS.h"
#include "display.hpp"
#include "speaker.hpp"
#include "all.hpp"

namespace crt{
    MainInits mainInits;
    display display1("display1", 2, 10000, ARDUINO_RUNNING_CORE);
    speaker speaker1("speaker1", 2, 10000, ARDUINO_RUNNING_CORE);
    all all1(display1, speaker1, "all1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}