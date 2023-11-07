#include <Arduino.h>
// #include <TestButton.ino>
// #include <blink_ir.h>
// #include "trigger_en_reload.h"
#include "buttons_2_test.h"
// #include "Queue.ino"
//#include <receiverTest.hpp>
//#include <sendTest.hpp>
#include <allTest.hpp>

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
