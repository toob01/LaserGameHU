#include <crt_CleanRTOS.h>
#include "GameData.hpp"
#include "displayControl.hpp"
#include "GameOverControl.hpp"

namespace crt
{
    class GameStateControl : public Task{
    private:
        Flag startFlag;
        Timer clockTimer;

        GameData_t& GameData;
        GameOverControl& gameOverControl;
        DisplayControl& displayControl;

        enum state_GameStateControl_t {Idle, UpdateGameTimer, UpdateDisplay, GameOver};
        state_GameStateControl_t state_GameStateControl = state_GameStateControl_t::Idle;

        bool bForceGameOver = false;

        GameStateControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, 
        GameData_t& GameData, GameOverControl& gameOverControl, DisplayControl& displayControl) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            startFlag(this), clockTimer(this), GameData(GameData), gameOverControl(gameOverControl), displayControl(displayControl)
        {
            start();
        }

    public:

        void _start(){
            startFlag.set();
        }

        void setShotsTaken(int x){
            GameData.setShotsTaken(x);
        }

        void decrementHealth(int x){
            GameData.setHealth(GameData.getHealth()-x);
        }

        void forceGameOver(){
            bForceGameOver = true;
        }

        void main() {
            for(;;){
                switch(state_GameStateControl){
                    case state_GameStateControl_t::Idle :
                        bForceGameOver = false;
                        wait(startFlag);
                        state_GameStateControl = state_GameStateControl_t::UpdateGameTimer;
                        break;
                    case state_GameStateControl_t::UpdateGameTimer :
                        GameData.setGameTime(GameData.getGameTime()-1);
                        state_GameStateControl = state_GameStateControl_t::UpdateDisplay;
                        break;
                    case state_GameStateControl_t::UpdateDisplay :
                        clockTimer.start_periodic(1'000'000); // 1 second timer
                        displayControl.setTimer(GameData.getGameTime());
                        displayControl.drawDisplaySet();
                        if(GameData.getHealth() == 0 || GameData.getGameTime() == 0 || bForceGameOver){
                            clockTimer.stop();
                            state_GameStateControl = state_GameStateControl_t::GameOver;
                        } else {
                            wait(clockTimer);
                            state_GameStateControl = state_GameStateControl_t::UpdateGameTimer;
                        }
                        break;
                    case state_GameStateControl_t::GameOver :
                        gameOverControl._start();
                        state_GameStateControl = state_GameStateControl_t::Idle;
                        break;
                }
            }
        }
    };
}
