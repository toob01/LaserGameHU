#pragma once
#include <crt_CleanRTOS.h>
#include "ConnectControl.hpp"
#include "GameData.hpp"

namespace crt {

class SendPostGameDataControl : public Task {
private:
    GameData_t& GameData;
    ConnectControl& connectControl;
    Flag startFlag;

    enum state_SPGDC_t {Idle, sendGameData};
    state_SPGDC_t state_SPGDC = state_SPGDC_t::Idle;

public:

SendPostGameDataControl(GameData_t& GameData, ConnectControl& connectControl, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), GameData(GameData), connectControl(connectControl), startFlag(this)
    {
        start();
    }

void _start(){
    startFlag.set();
}

void main() {
    int health;
    int shotsTaken;
    for(;;){
        switch(state_SPGDC){
            case state_SPGDC_t::Idle :
                wait(startFlag);
                state_SPGDC = state_SPGDC_t::sendGameData;
                break;
            
            case state_SPGDC_t::sendGameData :
                HitArray hits = GameData.getHits();
                health = GameData.getHealth();
                shotsTaken = GameData.getShotsTaken();
                connectControl.sendPostGameData(hits, health, shotsTaken);
                state_SPGDC = state_SPGDC_t::Idle;
                break;
        }
    }
}
};

}