#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <crt_CleanRTOS.h>
#include "messageSender.hpp"
#include "speakerControl.hpp"
#include "GameData.hpp"
#include "displayControl.hpp"
#include "crt_Button.h"

namespace crt
{
class ShootingControl : public IButtonListener, public Task{
private:

    Flag startFlag;
    Flag stopFlag;
    Queue<const char*, 10> buttonQueue;
    Timer reloadTimer;
    const char* name;

    enum state_ShootingControl_t {Idle, waitForTrigger, Shoot, waitForReload, Reload};
    state_ShootingControl_t state_ShootingControl = state_ShootingControl_t::Idle;

    MessageSender& messageSender;
    SpeakerControl& speakerControl;
    GameData_t& GameData;
    DisplayControl& displayControl;
public:

    ShootingControl(IButton& TriggerButton, IButton& ReloadButton, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    MessageSender& messageSender, SpeakerControl& speakerControl, GameData_t& GameData, DisplayControl& displayControl) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), startFlag(this), stopFlag(this), buttonQueue(this), reloadTimer(this),
        messageSender(messageSender), speakerControl(speakerControl), GameData(GameData), displayControl(displayControl)
    {
        start();
        TriggerButton.addButtonListener(this);
        ReloadButton.addButtonListener(this);
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

    void init(int ammo){
        GameData.setMaxAmmo(ammo);
        startFlag.set();
    }

    void disable(){
        stopFlag.set();
    }

    void main() {
        int ammo = GameData.getMaxAmmo();
        const char* button = "";
        for(;;){
            switch(state_ShootingControl){
                case state_ShootingControl_t::Idle :
                    wait(startFlag);
                    state_ShootingControl = state_ShootingControl_t::waitForTrigger;
                    buttonQueue.clear();
                    break;
                case state_ShootingControl_t::waitForTrigger :
                    ESP_LOGI("ShootingControl", "Wait for trigger");
                    displayControl.setBulletCount(ammo);
                    displayControl.drawDisplaySet();
                    waitAny(buttonQueue + stopFlag);
                    if(hasFired(stopFlag)){
                        state_ShootingControl = state_ShootingControl_t::Idle;
                        break;
                    }
                    buttonQueue.read(button);
                    if(button[0] == 'T'){
                        if(ammo != 0){
                            buttonQueue.clear();
                            state_ShootingControl = state_ShootingControl_t::Shoot;
                            break;
                        } else {
                            buttonQueue.clear();
                            state_ShootingControl = state_ShootingControl_t::waitForReload;
                            break;
                        }
                    } else if (button[0] == 'R'){
                        buttonQueue.clear();
                        state_ShootingControl = state_ShootingControl_t::Reload;
                        break;
                    }
                    break;
                case state_ShootingControl_t::Shoot :
                    ESP_LOGI("ShootingControl", "Shoot message");
                    ESP_LOGI("ShootingControl", "Ammo amount is: %d", ammo);
                    messageSender.sendShoot(GameData.getWeaponDamage(), GameData.getPlayerNum(), GameData.getTeamNum());
                    speakerControl.gunShotSet();
                    GameData.setShotsTaken(GameData.getShotsTaken()+1);
                    ammo -= 1;
                    displayControl.setBulletCount(ammo);
                    displayControl.drawDisplaySet();
                    state_ShootingControl = state_ShootingControl_t::waitForTrigger;
                    break;
                case state_ShootingControl_t::waitForReload :
                    buttonQueue.read(button);
                    if(button[0] == 'R'){
                        state_ShootingControl = state_ShootingControl_t::Reload;
                        break;
                    }
                    break;
                case state_ShootingControl_t::Reload :
                    speakerControl.reloadSet();
                    displayControl.reloadSet();
                    displayControl.drawDisplaySet();
                    vTaskDelay(GameData.getReloadTime()*1000);
                    ammo = GameData.getMaxAmmo();
                    displayControl.setBulletCount(ammo);
                    displayControl.drawDisplaySet();
                    state_ShootingControl = state_ShootingControl_t::waitForTrigger;
                    break;
            }
        }
    }
};
}
