#include <Arduino.h>
<<<<<<< HEAD
// #include <TestButton.ino>
// #include <blink_ir.h>
// #include "trigger_en_reload.h"
//#include "buttons_2_test.h"
// #include "Queue.ino"
//#include <receiverTest.hpp>
//#include <sendTest.hpp>
#include <allTest.hpp>
=======
// #include <HelloWorld.ino>    // For initial test.?
//#include <receiverTest.hpp>
//#include <TCP.hpp> 
#include <HTTP.hpp>
//#include <WiFiScan.ino>
//#include <ClockPin.ino>
//#include <crt_TestWeightScale_hx711.h>
//#include <TenTasks.ino>
//#include <Vec2.ino>
//#include <Panel.ino>
//#include <WifiScan.ino>
//#include <LITTLEFS_test.ino>
//void touch_calibrate();
//#include <Button_demo.ino>
//#include <LITTLEFS_test.ino>
//#include <AsyncDisplay.ino>
//#include <Free_Font_Demo.ino>
//#include <TouchscreenButton.ino>
//#include <TouchscreenButtonGroup.ino>
//#include <TouchscreenKeyboardLowerCase.ino>  // Not finalised yet.
//#include <Queue2.ino>                        // Not sure if this is already finalised.

//------------------------------------
// Above, you can copy or include the contents of .ino examples from the arduino IDE.
// The only thing is: you may have to forward declare functions or change the order
// of functions, such that they're known before being called.
// (apparrently the esp IDF compiler is a bit less smart in that respect than the
// compiler used in the Arduino IDE)
>>>>>>> a915d128d47d11c9964fff95de713ec9385de5c4

extern "C" {
	void app_main();
}

void app_main(void)
{
	setup();
	for(;;)
	{
		loop();
<<<<<<< HEAD
		vTaskDelay(1);
=======
		//yield();
		vTaskDelay(1);  // prevent the watchdog timer to kick in for this thread.
>>>>>>> a915d128d47d11c9964fff95de713ec9385de5c4
	}
}
