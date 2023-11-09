#pragma once
#include <crt_CleanRTOS.h>
#include "displayControl.hpp"
#include "readyUpControl.hpp"
#include "GameData.hpp"
#include "ISetupListener.hpp"
#include "speakerControl.hpp"

namespace crt{

class GameSetupControl : public Task{
private:
    DisplayControl& displayControl;
    ReadyUpControl& readyUpControl;
    GameData_t& GameData;
    SpeakerControl& speakerControl;

    Flag startFlag;
    Flag flagDataReady;

    enum setupState_t {Idle, Setup};
    setupState_t setupState = setupState_t::Idle;

    Pool<GameData_t> poolGameData;

    ISetupListener* arListeners[1] = {};
    uint16_t nListeners;

public:
    GameSetupControl(DisplayControl& displayControl, ReadyUpControl& readyUpControl, GameData_t& GameData, SpeakerControl& speakerControl,
    const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        displayControl(displayControl), readyUpControl(readyUpControl), GameData(GameData), speakerControl(speakerControl),
        startFlag(this), flagDataReady(this), nListeners(0)
        {
            for (int i = 0;i < 1;i++)
			{
				arListeners[i] = nullptr;
			}
            start();
            ESP_LOGI("GameSetupControl", "Start GameSetupControl");
        }

    void _start(){
        ESP_LOGI("GameSetupControl", "GameSetupControl _start called");
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
        GameData_t gameData;
        for(;;){
            switch(setupState){
                case setupState_t::Idle:
                    wait(startFlag);
                    displayControl.startUpSet();
                    speakerControl.startUpSet();
                    setupState = setupState_t::Setup;
                    break;

                case setupState_t::Setup:
                    ESP_LOGI("SetupControl", "Before get data");
                    for( unsigned int i = 0; i < nListeners; i++){
                        arListeners[i] -> getGameData();
                    }
                    wait(flagDataReady);
                    poolGameData.read(gameData);
                    GameData.setData(gameData);
                    ESP_LOGI("GameSetupControl", "Gamedata set: %d, %d, %d, %d, %d, %d, %d", gameData.getPlayerNum(), gameData.getTeamNum(), gameData.getGameTime(), gameData.getHealth(), gameData.getMaxAmmo(), gameData.getWeaponDamage(), gameData.getReloadTime());
                    readyUpControl._start();
                    setupState = setupState_t::Idle;
                    break;
            }
            
        }

    }
};

}; // namespace crt