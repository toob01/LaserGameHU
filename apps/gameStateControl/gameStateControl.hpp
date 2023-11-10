#pragma once
#include <crt_CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "StartGameOver.hpp"
#include "shootingControl.hpp"
#include "receivingHitControl.hpp"
#include "IReceivingHitListener.hpp"

namespace crt
{   
    class GameStateControl : public Task, public IReceivingHitListener{
    private:
        Flag startFlag;
        Timer clockTimer;

        GameData_t& GameData;
        DisplayControl& displayControl;
        ReceivingHitControl& receivingHitControl;
        ShootingControl& shootingControl;

        enum state_GameStateControl_t {Idle, UpdateDisplay};
        state_GameStateControl_t state_GameStateControl = state_GameStateControl_t::Idle;

        bool bForceGameOver = false;
        StartGameOver* arGameOvers[1] = {};

    public:
        GameStateControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, 
        GameData_t& GameData, DisplayControl& displayControl, ReceivingHitControl& receivingHitControl, ShootingControl& shootingControl) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            startFlag(this), clockTimer(this), GameData(GameData), displayControl(displayControl), receivingHitControl(receivingHitControl), shootingControl(shootingControl)
        {
            arGameOvers[0] = nullptr;
            receivingHitControl.addListener(this);
            start();
        }

        void addGameOver(StartGameOver* pGameOver){
            arGameOvers[0] = pGameOver;
        }

        void _start(){
            startFlag.set();
        }

        void setShotsTaken(int x){
            GameData.setShotsTaken(x);
        }

        void decrementHealth(int x){
            GameData.setHealth(GameData.getHealth()-x);
        }

        void forceGameOver(){
            bForceGameOver = true;
        }

        void main() {
            for(;;){
                switch(state_GameStateControl){
                    case state_GameStateControl_t::Idle :
                        bForceGameOver = false;
                        wait(startFlag);
                        receivingHitControl.init();
                        shootingControl.init(GameData.getMaxAmmo());
                        state_GameStateControl = state_GameStateControl_t::UpdateDisplay;
                        break;
                    case state_GameStateControl_t::UpdateDisplay :
                        GameData.setGameTime(GameData.getGameTime()-1);
                        clockTimer.start_periodic(1000000); // 1 second timer
                        displayControl.setTimer(GameData.getGameTime());
                        displayControl.setLives(GameData.getHealth());
                        displayControl.drawDisplaySet();
                        if(GameData.getHealth() == 0 || GameData.getGameTime() == 0 || bForceGameOver){
                            clockTimer.stop();
                            arGameOvers[0] -> _start();
                            state_GameStateControl = state_GameStateControl_t::Idle;
                        } else {
                            wait(clockTimer);
                        }
                        break;
                }
            }
        }
    };
}