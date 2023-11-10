#pragma once
#include <crt_CleanRTOS.h>
#include "crt_Button.h"
#include "IReadyUpListener.hpp"
#include "GameData.hpp"
#include "gameStateControl.hpp"

namespace crt
{
class ReadyUpControl : public IButtonListener, public Task {
private:

    Flag startFlag;
    Flag startGameFlag;
    Queue<const char*, 10> buttonQueue;
    GameData_t& GameData;
    GameStateControl& gameStateControl;

    enum state_ReadyUpControl_t {Idle, waitForReady, sendReady};
    state_ReadyUpControl_t state_ReadyUpControl = state_ReadyUpControl_t::Idle;

    IReadyUpListener* arListeners[1] = {};
    uint16_t nListeners;

public:
    ReadyUpControl(IButton& TriggerButton, IButton& ReloadButton, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, GameData_t& GameData, GameStateControl& gameStateControl) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this), startGameFlag(this), buttonQueue(this), GameData(GameData), gameStateControl(gameStateControl), nListeners(0)
    {
        for (int i = 0;i < 1;i++){
			arListeners[i] = nullptr;
		}

        start();
        TriggerButton.addButtonListener(this);
        ReloadButton.addButtonListener(this);
    }


    void addListener(IReadyUpListener* pReadyUpListener){
				arListeners[nListeners++] = pReadyUpListener;
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
                    ESP_LOGI("readyUpControl", "ReadyUpControl waitForReady");
                    buttonQueue.read(button);
                    if(button[0] == 'R'){
                        ESP_LOGI("ReadyUpControl", "Button pressed R");
                        bReload = true;
                    } else if(button[0] == 'T'){
                        ESP_LOGI("ReadyUpControl", "Button pressed T");
                        bTrigger = true;
                    }
                    if(bReload && bTrigger){
                        buttonQueue.clear();
                        state_ReadyUpControl = state_ReadyUpControl_t::sendReady;
                        break;
                    }
                    break;
                case state_ReadyUpControl_t::sendReady :
                    ESP_LOGI("ReadyUpControl", "Send ready");
                    for( unsigned int i = 0; i < nListeners; i++){
                            arListeners[i] -> sendReady();
                    }
                    // rgb.setRGB(GameData.getTeamColor());
                    wait(startGameFlag);
                    ESP_LOGI("ReadyUpControl", "StartGame sent to gamestatecontrol");
                    gameStateControl._start();
                    state_ReadyUpControl = state_ReadyUpControl_t::Idle;
                    break;
            }
        }
    }
};
}
