#include "crt_CleanRTOS.h"
#include "bulletsLCD1602.hpp"

namespace crt{
    MainInits mainInits;
    bullets bullets1("bullets1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {

}

void loop() {
    vTaskDelay(1);
}