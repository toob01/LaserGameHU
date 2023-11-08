#pragma once
#include <crt_CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "shootingControl.hpp"
#include "receivingHitControl.hpp"
#include "SendPostGameDataControl.hpp"
#include "StartGameOver.hpp"

namespace crt {
    class GameOverControl : public Task, public StartGameOver {
    private:
        Flag startFlag;
        Timer clockTimer;

        GameData_t& GameData;
        SpeakerControl& speakerControl;
        ReceivingHitControl& receivingHitControl;
        ShootingControl& shootingControl;
        DisplayControl& displayControl;
        SendPostGameDataControl& sendPostGameDataControl;
        ConnectControl& connectControl;

        bool bForceGameOver;

        enum GameOverState_t {Idle, WaitGameOver, WaitTimer};
        GameOverState_t GameOverState = GameOverState_t::Idle;

    public:
        GameOverControl( GameData_t& GameData, SpeakerControl& speakerControl, ReceivingHitControl& receivingHitControl, ShootingControl& shootingControl,
        DisplayControl& displayControl, SendPostGameDataControl& sendPostGameDataControl, ConnectControl& connectControl, const char *taskName,
        unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber ) :
            Task( taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this), clockTimer(this), 
            GameData(GameData), speakerControl(speakerControl), receivingHitControl(receivingHitControl), shootingControl(shootingControl), 
            displayControl(displayControl), sendPostGameDataControl(sendPostGameDataControl), connectControl(connectControl), bForceGameOver(false)
            {
                start();
            }

        void _start(){
            startFlag.set();
        }

        void forceGameOver(){
            bForceGameOver = true;
        }

        void main(){
            for(;;){
                switch(GameOverState){
                    case GameOverState_t::Idle:
                        //wacht tot start flag is gezet (door gameStateControl)
                        bForceGameOver = false;
                        wait(startFlag);
                        GameOverState = GameOverState_t::WaitGameOver;
                        break;
                        
                    case GameOverState_t::WaitGameOver:
                    //disable hits en shots, geef game over aan met beeld en geluid
                        connectControl.meldGameOver();
                        shootingControl.disable();
                        receivingHitControl.disable();
                        speakerControl.gameOverSet();
                        displayControl.gameOverSet();
                    //start SendPostGameDataControl als de game time om is, ga anders naar WaitTimer
                        if( GameData.getGameTime() == 0 || bForceGameOver){
                            GameOverState = GameOverState_t::Idle;
                            sendPostGameDataControl._start();
                        } else {
                            GameOverState = GameOverState_t::WaitTimer;}
                        break;

                    case GameOverState_t::WaitTimer:
                    //check om de seconde of gametime voorbij is.
                        clockTimer.start_periodic(1000000); 
                        wait(clockTimer);
                        if( GameData.getGameTime() == 0 || bForceGameOver){
                            clockTimer.stop();
                            sendPostGameDataControl._start();
                            GameOverState = GameOverState_t::Idle;
                        }
                        break;
                }
            }
        }

    
    };
};