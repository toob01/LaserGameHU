#pragma once
#include <crt_CleanRTOS.h>
#include "displayControl.hpp"
#include "readyUpControl.hpp"
#include "ConnectControl.hpp"
#include "GameData.hpp"

namespace crt{

class GameSetupControl : public Task{
private:
    DisplayControl& displayControl;
    ReadyUpControl& readyUpControl;
    ConnectControl& connectControl;
    GameData_t& GameData;

    Flag startFlag;
    Flag flagDataReady;

    enum setupState_t {Idle, Setup};
    setupState_t setupState = setupState_t::Idle;

    Pool<GameData_t> poolGameData;
    GameData_t gameData;

public:
    GameSetupControl(DisplayControl& displayControl, ReadyUpControl& readyUpControl, ConnectControl& connectControl, GameData_t& GameData,
    const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        displayControl(displayControl), readyUpControl(readyUpControl), 
        connectControl(connectControl), GameData(GameData),
        startFlag(this), flagDataReady(this)
        {}
        
    void gameDataReady(GameData_t gameData){
        poolGameData.write(gameData);
        flagDataReady.set();
    }


    void main(){
        for(;;){
            switch(setupState){
                case setupState_t::Idle:
                    wait(startFlag);
                    DisplayControl.startUpSet();
                    setupState = setupState_t::Setup;
                    break;

                case setupState_t::Setup:
                    connectControl.getGameData();
                    wait(flagDataReady);
                    poolGameData.read(gameData);
                    GameData.setData(gameData);
                    readyUpControl._start();
                    setupState = setupState_t::Idle;
                    break;
            }
            
        }

    }
};

} // namespace crt