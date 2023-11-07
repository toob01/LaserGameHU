#include <crt_Logger.h>
#include <crt_CleanRTOS.h>
#include <crt_Handler.h>
#include "trigger_en_reload_print.h"
#include "id_button.h"

//als we de knoppen voor meer dingen gebruiken:
namespace crt{

    const unsigned int pinButtonDump = 34;
    Logger<100> theLogger("Logger", 2, ARDUINO_RUNNING_CORE, pinButtonDump);
	ILogger& logger = theLogger;

    MainInits mainInits;
    Handler<2> knopHandler( "trigger_reload_handler", 2, 5000, ARDUINO_RUNNING_CORE, 70);
    ID_Button<1> knop1(1, 14, true, knopHandler);
    ID_Button<1> knop2(2, 12, true, knopHandler);

    trigger_reload_print testtaak( "trigger_reload_print", 2, 5000, ARDUINO_RUNNING_CORE, knop1, knop2);
}


void setup(){
    vTaskDelay(10);
    ESP_LOGI("test", "start test");
}

void loop() {
    vTaskDelay(1);
}