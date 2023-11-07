#include <crt_CleanRTOS.h>        // This file includes crt_Config.h  You'll need to change defines there for a release build.
#include "Arduino.h"
// All Tasks should be created in this main file.
#include <crt_Logger.h>
#include <crt_Handler.h>
#include "crt_Button.h"
#include "testbuttons.h"

namespace crt
{
	// Create a "global" logger object withing namespace crt.
	const unsigned int pinButtonDump = 34; // Pressing a button connected to this pin dumps the latest logs to serial monitor.

	Logger<100> theLogger("Logger", 2 /*priority*/, ARDUINO_RUNNING_CORE, pinButtonDump);
	ILogger& logger = theLogger;	// This is the global object. It can be accessed without knowledge of the template parameter of theLogger.

    MainInits mainInits;            // Initialize CleanRTOS.
	Handler<10 /*MAXLISTENERCOUNT*/> buttonHandler("ButtonHandler", 2 /*priority*/, ARDUINO_RUNNING_CORE, 100 /*periodMs*/, 3000 /*batchTimeUs*/); // Don't forget to call its start() member during setup().
	Button<1> buttonA ("A" /*name*/, 14 /*pin*/, true/*positive logic*/, buttonHandler);

	// More buttons can be added here..
    Button<1> buttonB("B" /*name*/, 12 /*pin*/, true /*positive logic*/, buttonHandler);
	ButtonsTask test2Buttons(buttonA, buttonB, "2ButtonsTask", 2 /*priority*/, 5000 /*stackBytes*/, ARDUINO_RUNNING_CORE);
}

void setup()
{
	// initialize serial communication at 115200 bits per second:
	// Serial.begin(115200);    // Only needed when using Serial.print();

	vTaskDelay(10);// allow tasks to initialize.
	ESP_LOGI("checkpoint", "start of main");vTaskDelay(1);
}

void loop()
{
	vTaskDelay(1);// Nothing to do in loop - all example code runs in the 4 threads above.
}
