#include <Arduino.h>
//#include <HelloWorld.ino>
//#include <speakerControl.hpp>
//#include <displayControl.hpp>
#include <allControl.hpp>
//#include <test.hpp>

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
