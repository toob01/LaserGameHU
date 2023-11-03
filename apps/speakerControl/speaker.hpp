#include "DFRobotDFPlayerMini.h"

namespace crt
{
	class speaker : public Task{
		private:
		Flag startUpFlag;
		Flag gameOverFlag;

        public:
		HardwareSerial Serial_df = HardwareSerial(2);
		DFRobotDFPlayerMini myDFPlayer;

		speaker(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), startUpFlag(this), gameOverFlag(this)
        {
            start();
        }

		void gameOver(){
			gameOverFlag.set();
		}

		void startUp(){
			startUpFlag.set();
		}

		void gunShot(){
			myDFPlayer.play(2);
			vTaskDelay(200);
		}

		void hit(){
			myDFPlayer.play(1);
			vTaskDelay(500);
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
			myDFPlayer.volume(20);
			vTaskDelay(100);
			
            for(;;){
                vTaskDelay(1);
				auto event = wait(startUpFlag);
				switch(event){
					case gameOverFlag:
						myDFPlayer.play(3);
						vTaskDelay(3000);
						break;
					case startUpFlag:
						myDFPlayer.play(4);
						vTaskDelay(5000);
						break;
					default:
						break;
				}
            }
        }
    };
}
 