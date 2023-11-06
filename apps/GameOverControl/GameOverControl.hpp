#pragma once
#include <CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "shootingControl.hpp"
#include "recievingHitControl.hpp"



namespace crt {
    class gameOver : public Task {
        private:
            Flag startFlag;
            Timer clockTimer;
            GameData_t& GameData;

            enum GameOverState_t {WaitGameOver, WaitTimer};
            GameOverState_t GameOverState = GameOverState_t::WaitGameOver;


        public:
            gameOver( GameData_t& GameData, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int timer) :
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
                            ShootingControl.disable();
                            ReceivingHitControl.disable();
                            Speaker.gameOverSet();
                            DisplayControl.gameOverSet();
                        //start SendPostGameDataControl als de game time om is, ga anders naar WaitTimer
                            if( GameData.getGameTime() == 0){
                                GameOverState = GameOverState_t::WaitGameOver;
                                SendPostDataGameDataControl._start();
                            }else{ GameOverState = GameOverState_t::WaitTimer;}
                        break;

                        case GameOverStat_t::WaitTimer: 
                        //check om de seconde of gametime voorbij is.
                            clockTimer.start_periodic(1'000'000); 
                            wait(clockTimer);
                            if( GameData.getGameTime() == 0){
                                clockTimer.stop();
                                SendPostDataGameDataControl._start();
                                GameOverState = GameOverState_t::WaitGameOver;
                            }
                        break;
                    }
                }
            }




    };
};