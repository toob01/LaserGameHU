#include "crt_CleanRTOS.h"       // This file includes crt_Config.h  You'll need to change defines there for a release build.
#include <crt_Logger.h>
#include <crt_Handler.h>
// All Tasks should be created in this main file.
#include "messageSender.hpp"
#include "messageSenderTest.hpp"
namespace crt
{   
    // const unsigned int pinButtonDump = 18;
    // Logger<1000> messageLogger("messageLogger", 3, ARDUINO_RUNNING_CORE, pinButtonDump);
    // ILogger& logger = messageLogger;

	// MainInits mainInits;            // Initialize CleanRTOS.
    MessageSender messageSender("messageSender", 1, 4000, ARDUINO_RUNNING_CORE, 19, false);
    testSender test("testSender", 2, 3000, ARDUINO_RUNNING_CORE, messageSender);
}

// void setup()
// {
// }

// void loop()
// {
// 	vTaskDelay(1);// Nothing to do in loop - all example code runs in the 4 threads above.
// }
