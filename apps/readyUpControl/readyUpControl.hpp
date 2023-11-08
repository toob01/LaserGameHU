#pragma once
#include <crt_CleanRTOS.h>
#include "crt_Button.h"
#include "ConnectControl.hpp"
#include "GameData.hpp"

namespace crt
{
class ReadyUpControl : public IButtonListener, public Task {
private:

    Flag startFlag;
    Flag startGameFlag;
    Queue<const char*, 10> buttonQueue;
    ConnectControl& connectControl;
    GameData_t& GameData;

    enum state_ReadyUpControl_t {Idle, waitForReady, sendReady};
    state_ReadyUpControl_t state_ReadyUpControl = state_ReadyUpControl_t::Idle;

public:
    ReadyUpControl(IButton& TriggerButton, IButton& ReloadButton, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, ConnectControl& connectControl, GameData_t& GameData) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this), startGameFlag(this), buttonQueue(this), connectControl(connectControl), GameData(GameData)
    {
        start();
        TriggerButton.addButtonListener(this);
        ReloadButton.addButtonListener(this);
    }

    void _start(){
        startFlag.set();
    }

    void startGame(){
        startGameFlag.set();
    }

    void buttonPressed(IButton* pButton)
		{
			const char* name = pButton->getButtonName();
			buttonQueue.write(name);
		}

	void buttonReleased(IButton* pButton)
		{
			// name = pButton->getButtonName());
			//implementeer hier evt iets leuks
		}

    void main() {
        bool bReload = false;
        bool bTrigger = false;
        const char* button;
        for(;;){
            switch(state_ReadyUpControl){
                case state_ReadyUpControl_t::Idle :
                    wait(startFlag);
                    buttonQueue.clear();
                    state_ReadyUpControl = state_ReadyUpControl_t::waitForReady;
                    break;
                case state_ReadyUpControl_t::waitForReady :
                    buttonQueue.read(button);
                    if(button[0] == 'R'){
                        bReload = true;
                    } else if(button[0] == 'T'){
                        bTrigger = true;
                    }
                    if(bReload && bTrigger){
                        buttonQueue.clear();
                        state_ReadyUpControl = state_ReadyUpControl_t::sendReady;
                        break;
                    }
                    break;
                case state_ReadyUpControl_t::sendReady :
                    connectControl.sendReady();
                    // rgb.setRGB(GameData.getTeamColor());
                    wait(startGameFlag);
                    gameStateControl._start();
                    state_ReadyUpControl = state_ReadyUpControl_t::Idle;
                    break;
            }
        }
    }
};
}
