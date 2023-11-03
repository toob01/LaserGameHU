#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class shooting : public Task{
        public:
        uint8_t teamNum;

        shooting(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

        private:
        void getTeamNum(){
            //get teamNum from gameState
        }

        void setRGB(){
            if(teamNum == 1){
                //set RGB to team 1
            }
            else if(teamNum == 2){
                //set RGB to team 2
            }
        }

        void sendReady(){
            //send ready signal with teamNum to connectControl
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
