#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class gameState : public Task{
        public:
        uint8_t playerNum;
        uint8_t teamNum;
        uint8_t timer;
        uint8_t ammo;
        uint8_t health;
        uint8_t shotsTaken;

        gameState(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int timer) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            timer(timer)
        {
            start();
        }

        private:
        void get(data){
            //get data from gameData
        }

        void set(data){
            //set data to gameData
        }


        void main() {
            Serial.begin(9600);
            delay(2000);
            

            for(;;){
                vTaskDelay(1);
            }
        }
    };
}
