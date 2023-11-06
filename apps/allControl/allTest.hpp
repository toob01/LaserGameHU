#include "crt_CleanRTOS.h"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "allControl.hpp"
#include <crt_Logger.h>
#include <crt_Handler.h>
#include "crt_Button.h"

namespace crt{
    MainInits mainInits;
    DisplayControl display1("display1", 2, 10000, ARDUINO_RUNNING_CORE);
    SpeakerControl speaker1("speaker1", 2, 10000, ARDUINO_RUNNING_CORE);
    Handler<10 /*MAXLISTENERCOUNT*/> buttonHandler("ButtonHandler", 2 /*priority*/, ARDUINO_RUNNING_CORE, 100 /*periodMs*/, 3000 /*batchTimeUs*/); // Don't forget to call its start() member during setup().
	Button<1> reloadButton("R" /*name*/, 13 /*pin*/, true /*positive logic*/, buttonHandler);
    Button<1> triggerButton("T" /*name*/, 15 /*pin*/, true /*positive logic*/, buttonHandler);
    AllControl all1(display1, speaker1, reloadButton, triggerButton, "all1", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}