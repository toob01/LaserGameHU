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
    Flag readyForDataFlag;

    enum state_SPGDC_t {Idle, waitForReady, sendGameData};
    state_SPGDC_t state_SPGDC = state_SPGDC_t::Idle;

public:

SendPostGameDataControl(GameData_t& GameData, ConnectControl& connectControl, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), GameData(GameData), connectControl(connectControl), startFlag(this), readyForDataFlag(this)
    {
        start();
    }

void _start(){
    startFlag.set();
}

void readyForData(){
    readyForDataFlag.set();
}

void main() {
    for(;;){
        switch(state_SPGDC){
            case state_SPGDC_t::Idle :
                wait(startFlag);
                state_SPGDC = state_SPGDC_t::waitForReady;
                break;

            case state_SPGDC_t::waitForReady :
                wait(readyForDataFlag);
                state_SPGDC = state_SPGDC_t::sendGameData;
                break;

            case state_SPGDC_t::sendGameData :
                HitArray hits = GameData.getHits();
                int health = GameData.getHealth();
                int shotsTaken = GameData.getShotsTaken();
                connectControl.sendPostGameData(hits, health, shotsTaken);
                state_SPGDC = state_SPGDC_t::Idle;
                break;
        }
    }
}
};

}