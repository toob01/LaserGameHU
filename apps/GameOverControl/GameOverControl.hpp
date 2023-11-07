#pragma once
#include <CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "shootingControl.hpp"
#include "recievingHitControl.hpp"

namespace crt {
    class GameOverControl : public Task {
        private:
            Flag startFlag;
            Timer clockTimer;

            enum GameOverState_t {WaitGameOver, WaitTimer};
            GameOverState_t GameOverState = GameOverState_t::WaitGameOver;

            GameData_t& GameData;
            SpeakerControl& SpeakerControl;
            ReceivingHitControl& receivingHitControl;
            ShootingControl& shootingControl;
            DisplayControl& displayControl;
            SendPostGameDataControl& sendPostGameDataControl;

        public:
            GameOverControl( GameData_t& GameData, SpeakerControl& speakerControl, ReceivingHitControl& receivingHitControl, ShootingControl& shootingControl,
            DisplayControl& displayControl, SendPostGameDataControl& sendPostGameDataControl
            const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
                Task( taskName, taskPriority, taskSizeBytes, taskCoreNumber), clockTimer(this), startFlag(this), GameData(GameData)
            {
                start();
            }

            void _start(){
                startFlag.set();
            }

            void main(){
                for(;;){
                    switch(GameOverState){
                        case GameOverState_t::WaitGameOver:
                        //wacht tot start flag is gezet (door gameStateControl)
                            startFlag.clear();
                            wait(startFlag);
                        //disable hits en shots, geef game over aan met beeld en geluid
                            shootingControl.disable();
                            receivingHitControl.disable();
                            SpeakerControl.gameOverSet();
                            displayControl.gameOverSet();
                        //start SendPostGameDataControl als de game time om is, ga anders naar WaitTimer
                            if( GameData.getGameTime() == 0){
                                GameOverState = GameOverState_t::WaitGameOver;
                                sendPostGameDataControl._start();
                            }else{ GameOverState = GameOverState_t::WaitTimer;}
                        break;

                        case GameOverState_t::WaitTimer: 
                        //check om de seconde of gametime voorbij is.
                            clockTimer.start_periodic(1'000'000); 
                            wait(clockTimer);
                            if( GameData.getGameTime() == 0){
                                clockTimer.stop();
                                sendPostGameDataControl._start();
                                GameOverState = GameOverState_t::WaitGameOver;
                            }
                        break;
                    }
                }
            }




    };
};