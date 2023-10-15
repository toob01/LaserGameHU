#include "crt_CleanRTOS.h"       // This file includes crt_Config.h  You'll need to change defines there for a release build.
#include <crt_Logger.h>
#include <crt_Handler.h>
// All Tasks should be created in this main file.
#include "messageReceiver.hpp"
#include "signalPauseDetector.hpp"
#include "necReciever.hpp"
namespace crt
{   
    // const unsigned int pinButtonDump = 18;
    // Logger<1000> messageLogger("messageLogger", 2, ARDUINO_RUNNING_CORE, pinButtonDump);
    // ILogger& logger = messageLogger;

	MainInits mainInits;            // Initialize CleanRTOS.
	MessageReceiver messageReceiver("MessageReceiver", 2 /*priority*/, 4000 /*stackBytes*/, ARDUINO_RUNNING_CORE);
    NecReceiver necReceiver(messageReceiver, "NECReceiver", 2, 6000, ARDUINO_RUNNING_CORE);
    SignalPauseDetector signalPauseDetector("SignalPauseDetector", 2, 6000, ARDUINO_RUNNING_CORE, necReceiver);
}

void setup()
{
}

void loop()
{
	vTaskDelay(1);// Nothing to do in loop - all example code runs in the 4 threads above.
}
