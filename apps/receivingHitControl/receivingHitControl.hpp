#pragma once
#include <crt_CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "IReceivingHitListener.hpp"

namespace crt
{

class ReceivingHitControl : public Task{
private:
    struct RHit{
        uint8_t damage;
        uint8_t playerNum;
        uint8_t teamNum;

        RHit(uint8_t damage=0, uint8_t playerNum=0, uint8_t teamNum=0):
        damage(damage), playerNum(playerNum), teamNum(teamNum)
        {}
    };

    Flag flagStart;
    Flag flagStop;
    Queue<RHit, 10> hitQueue;

    enum state_RHC_t {Idle, waitForHit, storeHit};
    state_RHC_t state_RHC = state_RHC_t::Idle;

    GameData_t& GameData;
    SpeakerControl& speakerControl;
    DisplayControl& displayControl;

    IReceivingHitListener* arListeners[1] = {};
    uint16_t nListeners;

public:

    ReceivingHitControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
        GameData_t& GameData, SpeakerControl& speakerControl, DisplayControl& displayControl) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), flagStart(this), flagStop(this), hitQueue(this),
        GameData(GameData), speakerControl(speakerControl), displayControl(displayControl), nListeners(0)
    {
        start();
        for (int i = 0;i < 1;i++)
			{
				arListeners[i] = nullptr;
			}
    }

    void addListener(IReceivingHitListener* pReceivingHitListener){
        arListeners[nListeners++] = pReceivingHitListener;
    }

    void hitReceived(uint8_t damage, uint8_t playerNum, uint8_t teamNum){
        RHit hit(damage, playerNum, teamNum);
        hitQueue.write(hit);
    }

    void init(){
        flagStart.set();
    }

    void disable(){
        flagStop.set();
    }

    void main() {
        RHit hit;
        for(;;){
            switch(state_RHC){
                case state_RHC_t::Idle :
                    wait(flagStart);
                    state_RHC = state_RHC_t::waitForHit;
                    break;
                case state_RHC_t::waitForHit :
                    waitAny(flagStop + hitQueue);
                    if (hasFired(flagStop)){
                        hitQueue.clear();
                        state_RHC = state_RHC_t::Idle;
                        break;
                    } else if (hasFired(hitQueue)){
                        ESP_LOGI("ReceivingHitControl", "Hit Received from queue");
                        hitQueue.read(hit);
                        state_RHC = state_RHC_t::storeHit;
                        break;
                    }
                    break;
                case state_RHC_t::storeHit :
                    GameData.addHit(GameData.getGameTime(), hit.playerNum);
                    arListeners[0]->decrementHealth(hit.damage);
                    speakerControl.hitSet();
                    displayControl.setLives(GameData.getHealth());
                    displayControl.drawDisplaySet();
                    hitQueue.clear();
                    state_RHC = state_RHC_t::waitForHit;
                    break;
            }
        }
    }
};

} // namespace crt
