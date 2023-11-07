#pragma once
#include <crt_CleanRTOS.h>
#include "crt_Button.h"

namespace crt
{
	class AllControl : public Task, public IButtonListener {
        public:
        AllControl(DisplayControl& display1, SpeakerControl& speaker1, IButton& reloadButton, IButton& triggerButton, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            display1(display1),
            speaker1(speaker1),
            name(""), 
            queueButtons(this)
        {
            reloadButton.addButtonListener(this);
            triggerButton.addButtonListener(this);
            start();
        }

        private:
        DisplayControl& display1;
        SpeakerControl& speaker1;
        const char* name;
        Queue<const char*, 10> queueButtons;
        int bulletCount = 70;
        int lives = 99;
        int timer = 60;

        void buttonPressed(IButton* pButton){
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

		void buttonReleased(IButton* pButton){
			// name = pButton->getButtonName());
			//implementeer hier evt iets leuks
		}

        void bulletShot(){
            bulletCount--;
            return;
        }

        void bulletReload(){
            bulletCount = 70;
            vTaskDelay(1000);
            return;
        }

        void lifeLost(){
            lives--;
            return;
        }

        void timerDecrease(){
            timer--;
            return;
        }

        void main(){
            vTaskDelay(1000);
            // display1.startUpSet();
            // speaker1.startUpSet();
            // vTaskDelay(5000);
            // display1.setBulletCount(bulletCount);
            // display1.setLives(lives);
            // display1.setTimer(timer);
            // display1.drawDisplaySet();
            // speaker1.gunShotSet();
            // vTaskDelay(200);
            // speaker1.hitSet();
            // vTaskDelay(500);
            // speaker1.gunShotSet();
            // vTaskDelay(1000);
            // speaker1.gunShotSet();
            // vTaskDelay(1000);
            // display1.gameOverSet();
            // speaker1.gameOverSet();
            // vTaskDelay(3000);
            for(;;){
                wait(queueButtons);
                ESP_LOGI("AllControl", "Button pressed: %s", name);
                queueButtons.read(name);
                if(name[0] == 'R'){
                    bulletReload();
                    display1.reloadSet();
                    speaker1.reloadSet();
                    display1.setBulletCount(bulletCount);
                    display1.drawDisplaySet();
                    queueButtons.clear();
                }else if(name[0] == 'T'){
                    bulletShot();
                    speaker1.gunShotSet();
                    display1.setBulletCount(bulletCount);
                    display1.drawDisplaySet();
                }
            }
        }
    };
}