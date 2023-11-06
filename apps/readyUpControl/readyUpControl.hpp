#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "crt_CleanRTOS.h"
#include "crt_Button.h"
#include "connectControl.hpp"
#include "RGBLED.hpp"

namespace crt
{
class ReadyUpControl : public IButtonListener, public Task {
private:
    Queue<const char*, 10> buttonQueue;
    ConnectControl& connectControl;
    RGB& rgb;

    enum state_ReadyUpControl_t {Idle, waitForReady, sendReady};
    state_ReadyUpControl_t state_ReadyUpControl = state_ReadyUpControl_t::Idle;

public:
    ReadyUpControl(IButton& TriggerButton, IButton& ReloadButton, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    ConnectControl& connectControl, RGB& rgb) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), buttonQueue(this),
        connectControl(connectControl), rgb(rgb)
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
			name = pButton->getButtonName();
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
                    if(button == "ReloadButton"){
                        bReload = true;
                    } else if(button == "TriggerButton"){
                        bTrigger = true;
                    }
                    if(bReload && bTrigger){
                        buttonQueue.clear();
                        state_ReadyUpControl = state_ReadyUpControl_t::sendReady;
                        break;
                    }
                    break;
                case state_ReadyUpControl_t::sendReady :
                    ConnectControl.sendReady(GameData.getPlayerNum());
                    // rgb.setRGB(GameData.getTeamColor());
                    wait(startGameFlag);
                    GameStateControl._start();
                    state_ReadyUpControl = state_ReadyUpControl_t::Idle;
                    break;
            }
        }
    }
};
}
