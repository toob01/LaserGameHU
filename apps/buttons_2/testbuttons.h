// by Marius Versteegen, 2023

#pragma once
#include <crt_CleanRTOS.h>
#include <crt_Button.h>

namespace crt
{
	class ButtonsTask : public IButtonListener, public Task
	{
	private:
		const char* name;
		Queue<const char*, 10> queueButtons;

	public:
		ButtonsTask(IButton& buttonA, IButton& buttonB,
			const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
			Task( taskName, taskPriority, taskSizeBytes, taskCoreNumber), name(""), queueButtons(this)
		{
			buttonA.addButtonListener(this);
            buttonB.addButtonListener(this);
			start();
		}

		void buttonPressed(IButton* pButton)
		{
			name = pButton->getButtonName();

			queueButtons.write(name);
			// Voor Debugging (vervang bovenstaande regel):
			// bool bResult = queueButtons.write(name);
			// if (!bResult)
			// {	
			// 	ESP_LOGI("Test2ButtonsTask", "Clearing Queue");
			// 	vTaskDelay(1);
			// 	queueButtons.clear();
			// 	bResult = queueButtons.write(name);
			// 	if( !bResult ){
			// 		ESP_LOGI("Test2ButtonsTask", "Something wrong with Queue!");
			// 		vTaskDelay(1);
			// 	}
			// }
		}

		void buttonReleased(IButton* pButton)
		{
			// name = pButton->getButtonName());
			//implementeer hier evt iets leuks
		}

		void main(){
			while(true){
				wait(queueButtons);
				queueButtons.read(name);
				ESP_LOGI("Button Pressed", "%s", name);
			}
		}
  
	};//end class 2 buttons task
};// end namespace crt
