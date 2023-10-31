#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class shooting : public Task{
        public:
        int shotsTaken = 0;

        shooting(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int ammo) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            ammo(ammo)
        {
            start();
        }

        private:
        void sendMessage(uint8_t damage, uint8_t playerNum, uint8_t teamNum){
            //send message to other esp32 over ir
            return;
        }

        void setAmmo(){
            ammo--;
            //set ammo of GameStateControl naar ammo
            return;
        }

        void setShotsTaken(){
            shotsTaken++;
            //set shotsTaken of GameStateControl naar shotsTaken
            return;
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
