#include <crt_CleanRTOS.h>
#include "displayControl.hpp"
#include "readyUpControl.hpp"
#include "ConnectControl.hpp"
#include "GameData.hpp"

namespace crt{

class GameSetupControl : public Task{
private:
    DisplayControl& displayControl;
    ReadyUpControl& readyUpControl;
    ConnectControl& connectControl;
    GameData_t& GameData;

    Flag startFlag;
    enum setupState_t = {Idle, Setup}

public:
    GameSetupControl(DisplayControl& displayControl, ReadyUpControl& readyUpControl, ConnectControl& connectControl, GameData_t& GameData,
    const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        displayControl(displayControl), readyUpControl(readyUpControl), 
        connectControl(connectControl), GameData(GameData),
        startFlag(this)
        {}

    void main(){
        for(;;){
            switch(setupState){
                case Idle:
                    wait(startFlag);
                    setupState = setupState_t::Setup;
                break;
                case Setup:
                    while( !connectControl.CheckConnection() ){
                        
                    };
                    connectControl.getGameData();
            }
            
        }

    }
};

} // namespace crt