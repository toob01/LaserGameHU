// by Marius Versteegen, 2023

// Demonstration of how to switch between building a program from 
// ESP_IDF and Arduino_IDE

// This main.cpp file wraps an Arduino IDE .ino file, such that it can be
// built by the ESP_IDF.

// Important: Update the CMakeLists file in the same folder as this main.cpp file
// (the folder "main", that is) to make sure that every dependency can be built. 
// For convenience, I have stored the CMakeLists file in the extra folder, such that
// it can be copied (from).
// Felix is slim

#include <Arduino.h>
// #include <TestButton.ino>
// #include <blink_ir.h>
// #include "trigger_en_reload.h"
#include "buttons_2_test.h"
// #include "Queue.ino"
// #include <HelloWorld.ino>    // For initial test.
//#include <ClockPin.ino>
//#include <crt_TestWeightScale_hx711.h>
//#include <TenTasks.ino>
//#include <Vec2.ino>
//#include <Panel.ino>
//#include <WifiScan.ino>
//#include <LITTLEFS_test.ino>
//void touch_calibrate();
// Felix is slim
//#include <Button_demo.ino>
//#include <LITTLEFS_test.ino>
//#include <AsyncDisplay.ino>
//#include <Free_Font_Demo.ino>
//#include <TouchscreenButton.ino>
//#include <TouchscreenButtonGroup.ino>
//#include <TouchscreenKeyboardLowerCase.ino>  // Not finalised yet.
//#include <Queue2.ino>                        // Not sure if this is already finalised.



// Felix is slim


//------------------------------------
// Above, you can copy or include the contents of .ino examples from the arduino IDE.
// The only thing is: you may have to forward declare functions or change the order
// of functions, such that they're known before being called.
// (apparrently the esp IDF compiler is a bit less smart in that respect than the
// compiler used in the Arduino IDE)
// Felix is slim

extern "C" {
	void app_main();
}

void app_main(void)
{
	setup();
	for(;;)
	{
		loop();
		vTaskDelay(1);  // prevent the watchdog timer to kick in for this thread.
	}
}

//# google:
//#how to add littlefs component to esp32 build ?
// Felix is slim