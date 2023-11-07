#pragma once
#include <crt_CleanRTOS.h>
#include "GameSetupControl.hpp"
#include "readyUpControl.hpp"
#include "SendPostGameDataControl.hpp"
#include "gameStateControl.hpp" 

namespace crt
{
class ConnectControl : public Task {
private:
    Flag flagGameOver;
    Flag flagGameData;
    Flag flagSendReady;
    Flag flagPostGameData;

    Pool<HitArray> poolHit;
    Pool<int> poolLivesLeft;
    Pool<int> poolShotsTaken;

    GameSetupControl& gameSetupControl;
    ReadyUpControl& readyUpControl;
    SendPostGameDataControl& sendPostGameDataControl;
    GameStateControl& gameStateControl;

    enum state_connectControl_t {BootWifi, Idle, GameOver, GetGameData, SendReady, SendPostGameData};
    state_connectControl_t state_connectControl = state_connectControl_t::BootWifi;

public:
    ConnectControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    GameSetupControl& gameSetupControl, ReadyUpControl& readyUpControl, SendPostGameDataControl& sendPostGameDataControl, GameStateControl& gameStateControl) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), flagGameOver(this), flagGameData(this), flagSendReady(this), flagPostGamedata(this),
        poolHit(this), poolLivesLeft(this), poolShotsTaken(this), gameSetupControl(gameSetupControl), readyUpControl(readyUpControl), 
        sendPostGameDataControl(sendPostGameDataControl), gameStateControl(gameStateControl)
    {
        start();
    }

    void meldGameOver(){
        flagGameOver.set();
    }

    void getgameData(){
        flagGameData.set();
    }

    void sendReady(){
        flagSendReady.set();
    }

    void sendPostGameData(HitArray hit, int livesLeft, int shotsTaken){
        poolHit.write(hit);
        poolLivesLeft.write(livesLeft);
        poolShotsTaken.write(shotsTaken);
        flagPostGameData.set();
    }

    void main(){
        // TEST CODE
        bool bStarted = false;
        gpio_pad_select_gpio(18);
        gpio_set_direction((gpio_num_t)18, GPIO_MODE_INPUT);
        for(;;){
            switch(state_connectControl){
                case BootWifi:
                    // do the big wifi start, then:
                    ESP_LOGI("ConnectControl", "In state BootWifi yoohoo!");
                    gameSetupControl._start();
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case Idle:
                    //wait on everything and do the if statemten
                    if(gpio_get_level((gpio_num_t)18) && !bStarted){
                        // substitute for getting ready signal from host server
                        readyUpControl.startGame();
                        bStarted = true;
                    } else if(gpio_get_level((gpio_num_t)18) && bStarted){
                        gameStateControl.forceGameOver();
                        bStarted = false;
                    }
                    waitAny(flagGameOver + flagGameData + flagSendReady + flagPostGameData);
                    if(hasFired(flagGameOver)){
                        state_connectControl = state_connectControl_t::GameOver;
                        break;
                    } else if(hasFired(flagGameData)){
                        state_connectControl = state_connectControl_t::GetGameData;
                        break;
                    } else if(hasFired(flagSendReady)){
                        state_connectControl = state_connectControl_t::SendReady;
                        break;
                    } else if(hasFired(flagPostGameData)){
                        state_connectControl = state_connectControl_t::SendPostGameData;
                        break;
                    }
                    break;

                case GetGameData:
                    //Read from host server
                    GameData_t gameData(1, 1, 20, 200, 15, 50, 2);
                    gameSetupControl.sendGameData(gameData);
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                    
                case GameOver:
                    ESP_LOGI("ConnectControl", "Player Dead via meldGameOver : %d", GameData.getPlayerNum());
                    // do the sendy thing to Host Server that you're dead-o
                    state_connectControl = state_connectControl_t::Idle;
                    break;

                case SendReady:
                    ESP_LOGI("ConnectControl", "Player Ready %d", GameData.getPlayerNum());
                    // do the sendy thing to Host Server that you're ready
                    state_connectControl = state_connectControl_t::Idle;
                    break;

                case SendPostGameData:
                    int lives;
                    int shots;
                    poolLivesLeft.read(lives);
                    poolShotsTaken.read(shots);
                    ESP_LOGI("ConnectControl", "SendPostGameData; hits: probably present, lives: %d, shots: %d", lives, shots);
                    state_connectControl = state_connectControl_t::Idle;
                    break;
            }
        }
    }
};
}
