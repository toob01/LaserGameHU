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
    }

    void disable(){
        stopFlag.set();
    }

    void main() {
        int ammo = GameData.getMaxAmmo();
        const char* button;
        for(;;){
            switch(state_ShootingControl){
                case state_ShootingControl_t::Idle :
                    wait(startFlag);
                    state_ShootingControl = state_ShootingControl_t::waitForTrigger;
                    break;
                case state_ShootingControl_t::waitForTrigger :
                    //
                    buttonQueue.read(button);
                    if(button == "TriggerButton"){
                        if(ammo != 0){
                            buttonQueue.clear();
                            state_ShootingControl = state_ShootingControl_t::Shoot;
                            break;
                        } else {
                            buttonQueue.clear();
                            state_ShootingControl = state_ShootingControl_t::waitForReload;
                            break;
                        }
                    } else if (button == "ReloadButton"){
                        buttonQueue.clear();
                        state_ShootingControl = state_ShootingControl_t::Reload;
                        break;
                    }
                    break;
                case state_ShootingControl_t::Shoot :
                    messageSender.sendShoot(GameData.getWeaponDamage(), GameData.getPlayerNum(), GameData.getTeamNum());
                    speakerControl.gunShotSet();
                    GameData.setShotsTaken(GameData.getShotsTaken()+1);
                    ammo -= 1;
                    displayControl.setBulletCount(ammo);
                    displayControl.drawDisplaySet();
                    break;
                case state_ShootingControl_t::waitForReload :
                    buttonQueue.read(button);
                    if(button == "ReloadButton"){
                        state_ShootingControl = state_ShootingControl_t::Reload;
                        break;
                    }
                    break;
                case state_ShootingControl_t::Reload :
                    //
                    speakerControl.reloadSet();
                    vTaskDelay(GameData.getReloadTime()*1000);
                    ammo = GameData.getMaxAmmo();
                    displayControl.setBulletCount(ammo);
                    displayControl.drawDisplaySet();
                    break;
            }
        }
    }
};
}
