#include "Arduino.h"
#include "crt_CleanRTOS.h" // This file includes crt_Config.h  You'll need to change defines there for a release build.
#include <crt_Logger.h>
#include <crt_Handler.h>


namespace crt
{

    // MainInits mainInits; // Initialize CleanRTOS.
                         // MessageReceiver messageReceiver("MessageReceiver", 2 /*priority*/, 2000 /*stackBytes*/, ARDUINO_RUNNING_CORE);
    
    // HTTP_Server HTTP_server("HTTP_Server", 2, 10000, ARDUINO_RUNNING_CORE);
}

// void setup()
// {
//     //TCP_Startup();
// }

// void loop()
// {
//     vTaskDelay(1); // Nothing to do in loop - all example code runs in the 4 threads above.
// }
