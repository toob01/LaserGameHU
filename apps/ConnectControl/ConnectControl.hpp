#include <crt_CleanRTOS.h>
#include "GameSetupControl.hpp"
#include "readyUpControl.hpp"
#include "SendPostGameDataControl.hpp"
#include "gameStateControl.hpp" 

namespace crt
{
class ConnectControl : public Task {
private:
    GameSetupControl& gameSetupControl;
    ReadyUpControl& readyUpControl;
    SendPostGameDataControl& sendPostGameDataControl;
    GameStateControl& gameStateControl;

    Flag flagGameOver;

    enum state_ConnectControl

public:
    ConnectControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
    {
        start();
    }

    void meldGameOver(){
        flagGameOver.set();
    }

    GameData_t getgameData(){
        // get game settings from host
        // for now hardcoded settings:
        GameData_t gameData(
            
        )
        return
    }

    void main(){
        for(;;){
            switch(state_connectControl){
                case GetGameData:
                    
                case GameOver:
                    ESP_LOGI("ConnectControl", "Player Dead via meldGameOver : %d", GameData.getPlayerNum());
                    // do the sendy thing to Host Server
                    break;
                
            }
        }
    }
};
}
