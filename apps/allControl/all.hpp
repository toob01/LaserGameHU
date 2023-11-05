#pragma once

namespace crt
{
	class all : public Task{
        public:
        all(display& display1, speaker& speaker1, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            display1(display1),
            speaker1(speaker1)
        {
            start();
        }

        private:
        display& display1;
        speaker& speaker1;
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
            display1.startUpSet();
            // speaker1.startUpSet();
            display1.setBulletCount(bulletCount);
            display1.setLives(lives);
            display1.setTimer(timer);
            display1.drawDisplaySet();
            // speaker1.gunShotSet();
            // speaker1.hitSet();
            // speaker1.gunShotSet();
            // speaker1.gunShotSet();
            display1.gameOverSet();
            // speaker1.gameOverSet();

            for(;;){
                vTaskDelay(1);
            }
        }
    };
}