#pragma once

namespace crt
{
	class AllControl : public Task{
        public:
        AllControl(DisplayControl& display1, SpeakerControl& speaker1, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            display1(display1),
            speaker1(speaker1)
        {
            start();
        }

        private:
        DisplayControl& display1;
        SpeakerControl& speaker1;
        int bulletCount = 70;
        int lives = 99;
        int timer = 60;

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
            // vTaskDelay(1000);
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
                vTaskDelay(1);
            }
        }
    };
}