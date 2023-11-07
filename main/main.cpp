#include <Arduino.h>
#include "allTest.hpp"
// #include <receiverTest.hpp>
// #include "ConnectControl.hpp"
//#include <sendTest.hpp>
//#include <receiverTest.hpp>


//------------------------------------
// Above, you can copy or include the contents of .ino examples from the arduino IDE.
// The only thing is: you may have to forward declare functions or change the order
// of functions, such that they're known before being called.
// (apparrently the esp IDF compiler is a bit less smart in that respect than the
// compiler used in the Arduino IDE)

extern "C" {
	void app_main();
}

void app_main(void)
{
	setup();
	for(;;)
	{
		loop();
		vTaskDelay(1);
	}
}
