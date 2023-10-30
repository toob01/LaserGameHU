#include <Arduino.h>
//#include <HelloWorld.ino>
#include <displayMain.hpp>

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
