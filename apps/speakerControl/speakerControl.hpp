#pragma once
#include <crt_CleanRTOS.h>
#include "DFRobotDFPlayerMini.h"

namespace crt
{
	class SpeakerControl : public Task{
		private:
		Flag startUpFlag;
		Flag gameOverFlag;
		Flag gunShotFlag;
		Flag hitFlag;
		Flag reloadFlag;

		enum state_SpeakerControl_t {idle, startUp, gunShot, hit, reload, gameOver};
		state_SpeakerControl_t state_SpeakerControl = state_SpeakerControl_t::idle;

        public:
		HardwareSerial Serial_df = HardwareSerial(2);
		DFRobotDFPlayerMini myDFPlayer;

		SpeakerControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), 
			startUpFlag(this), gameOverFlag(this), gunShotFlag(this), hitFlag(this), reloadFlag(this)
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

		void reloadSet(){
			reloadFlag.set();
		}

		void main(){
			Serial_df.begin(9600);
			Serial.begin(115200);
			myDFPlayer.begin(Serial_df, false);
			myDFPlayer.setTimeOut(500);
			myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
			myDFPlayer.volume(30);
			vTaskDelay(100);
			
            for(;;){
				switch(state_SpeakerControl){
					case state_SpeakerControl_t::idle:
						waitAny(startUpFlag + gunShotFlag + hitFlag + reloadFlag + gameOverFlag);
						if(hasFired(startUpFlag)){
							state_SpeakerControl = state_SpeakerControl_t::startUp;
							break;
						}else if(hasFired(gunShotFlag)){
							state_SpeakerControl = state_SpeakerControl_t::gunShot;
							break;
						}else if(hasFired(hitFlag)){
							state_SpeakerControl = state_SpeakerControl_t::hit;
							break;
						}else if(hasFired(reloadFlag)){
							state_SpeakerControl = state_SpeakerControl_t::reload;
							break;
						}else if(hasFired(gameOverFlag)){
							state_SpeakerControl = state_SpeakerControl_t::gameOver;
							break;
						}else{break;}
					case state_SpeakerControl_t::startUp:
						myDFPlayer.play(4);
						vTaskDelay(5000);
						state_SpeakerControl = state_SpeakerControl_t::idle;
						break;
					case state_SpeakerControl_t::gameOver:
						myDFPlayer.play(3);
						vTaskDelay(3000);
						state_SpeakerControl = state_SpeakerControl_t::idle;
						break;
					case state_SpeakerControl_t::gunShot:
						myDFPlayer.play(2);
						vTaskDelay(200);
						state_SpeakerControl = state_SpeakerControl_t::idle;
						break;
					case state_SpeakerControl_t::hit:
						myDFPlayer.play(1);
						vTaskDelay(500);
						state_SpeakerControl = state_SpeakerControl_t::idle;
						break;
					case state_SpeakerControl_t::reload:
						myDFPlayer.play(5);
						vTaskDelay(2000);
						state_SpeakerControl = state_SpeakerControl_t::idle;
						break;
					default:
						break;
				}
            }
        }
    };
}
 