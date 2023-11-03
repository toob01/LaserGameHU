#include "DFRobotDFPlayerMini.h"

namespace crt
{
	class speaker : public Task{
        public:
		HardwareSerial Serial_df = HardwareSerial(2);
		DFRobotDFPlayerMini myDFPlayer;

		speaker(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

		private:
		void gameOver(){
			myDFPlayer.play(3);
			delay(3000);
		}

		void startUp(){
			myDFPlayer.play(4);
			delay(5000);
		}

		void gunShot(){
			myDFPlayer.play(2);
			delay(200);
		}

		void hit(){
			myDFPlayer.play(1);
			delay(500);
		}

		void main(){
			Serial_df.begin(9600);
			Serial.begin(115200);

			if (!myDFPlayer.begin(Serial_df, false)){
				Serial.println(F("Not initialized:"));
				Serial.println(F("1. Check the DFPlayer Mini connections"));
				Serial.println(F("2. Insert an SD card"));
			}else Serial.println(F("DFPlayer Mini online."));

			myDFPlayer.setTimeOut(500);
			myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
			myDFPlayer.volume(10);
			delay(100);
			
			startUp();
			gunShot();
			gunShot();
			gunShot();
			delay(1000);
			gunShot();
			hit();
			gunShot();
			hit();
			gameOver();
        
            for(;;){
                vTaskDelay(1);

            }
        }
    };
}
 