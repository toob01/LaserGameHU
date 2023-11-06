#include "crt_CleanRTOS.h"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "allControl.hpp"

namespace crt{
    MainInits mainInits;
    DisplayControl display1("display1", 2, 10000, ARDUINO_RUNNING_CORE);
    SpeakerControl speaker1("speaker1", 2, 10000, ARDUINO_RUNNING_CORE);
    AllControl all1(display1, speaker1, "all1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}