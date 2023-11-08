#pragma once
#include <crt_CleanRTOS.h>
#include "displayControl.hpp"
#include "readyUpControl.hpp"
#include "GameData.hpp"
#include "ISetupListener.hpp"

namespace crt{

class GameSetupControl : public Task{
private:
    DisplayControl& displayControl;
    ReadyUpControl& readyUpControl;
    GameData_t& GameData;

    Flag startFlag;
    Flag flagDataReady;

    enum setupState_t {Idle, Setup};
    setupState_t setupState = setupState_t::Idle;

    Pool<GameData_t> poolGameData;
    GameData_t gameData;

    ISetupListener* arListeners[1] = {};
    uint16_t nListeners;

public:
    GameSetupControl(DisplayControl& displayControl, ReadyUpControl& readyUpControl, GameData_t& GameData,
    const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        displayControl(displayControl), readyUpControl(readyUpControl), GameData(GameData),
        startFlag(this), flagDataReady(this), nListeners(0)
        {
            for (int i = 0;i < 1;i++)
			{
				arListeners[i] = nullptr;
			}
            start();
        }

    void _start(){
        startFlag.set();
    }

    void addListener(ISetupListener* pSetupListener){
        arListeners[nListeners++] = pSetupListener;
    }
        
    void sendGameData(GameData_t gameData){
        poolGameData.write(gameData);
        flagDataReady.set();
    }


    void main(){
        for(;;){
            switch(setupState){
                case setupState_t::Idle:
                    wait(startFlag);
                    displayControl.startUpSet();
                    setupState = setupState_t::Setup;
                    break;

                case setupState_t::Setup:
                    for( unsigned int i = 0; i < nListeners; i++){
                        arListeners[i] -> getgameData();
                    }
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

}; // namespace crt