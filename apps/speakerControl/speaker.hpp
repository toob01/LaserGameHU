#pragma once
#include "DFRobotDFPlayerMini.h"

namespace crt
{
	class speaker : public Task{
		private:
		Flag startUpFlag;
		Flag gameOverFlag;
		Flag gunShotFlag;
		Flag hitFlag;

		enum state_speaker_t {idle, startUp, gunShot, hit, gameOver};
		state_speaker_t state_speaker = state_speaker_t::idle;

        public:
		HardwareSerial Serial_df = HardwareSerial(2);
		DFRobotDFPlayerMini myDFPlayer;

		speaker(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), 
			startUpFlag(this), gameOverFlag(this), gunShotFlag(this), hitFlag(this)
        {
            start();
        }

		void gameOverSet(){
			gameOverFlag.set();
		}

		void startUpSet(){
			startUpFlag.set();
		}

		void gunShotSet(){
			gunShotFlag.set();
		}

		void hitSet(){
			hitFlag.set();
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
				switch(state_speaker){
					case state_speaker_t::idle:
						waitAny(startUpFlag + gunShotFlag + hitFlag + gameOverFlag);
						if(hasFired(startUpFlag)){
							state_speaker = state_speaker_t::startUp;
							break;
						}else if(hasFired(gameOverFlag)){
							state_speaker = state_speaker_t::gameOver;
							break;
						}else if(hasFired(gunShotFlag)){
							state_speaker = state_speaker_t::gunShot;
							break;
						}else if(hasFired(hitFlag)){
							state_speaker = state_speaker_t::hit;
							break;
						}else{break;}
					case state_speaker_t::startUp:
						myDFPlayer.play(4);
						vTaskDelay(5000);
						state_speaker = state_speaker_t::idle;
						startUpFlag.clear();
						break;
					case state_speaker_t::gameOver:
						myDFPlayer.play(3);
						vTaskDelay(3000);
						state_speaker = state_speaker_t::idle;
						gameOverFlag.clear();
						break;
					case state_speaker_t::gunShot:
						myDFPlayer.play(2);
						vTaskDelay(200);
						state_speaker = state_speaker_t::idle;
						gunShotFlag.clear();
						break;
					case state_speaker_t::hit:
						myDFPlayer.play(1);
						vTaskDelay(500);
						state_speaker = state_speaker_t::idle;
						hitFlag.clear();
						break;
					default:
						break;
				}
            }
        }
    };
}
 