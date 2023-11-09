#pragma once
#include <crt_CleanRTOS.h>
#include "GameSetupControl.hpp"
#include "readyUpControl.hpp"
#include "SendPostGameDataControl.hpp"
#include "gameStateControl.hpp"
#include "GameData.hpp"
#include "ISetupListener.hpp"
#include "IReadyUpListener.hpp"

namespace crt
{
class ConnectControl : public Task, public ISetupListener, public IReadyUpListener {
private:

    GameSetupControl& gameSetupControl;
    ReadyUpControl& readyUpControl;
    GameStateControl& gameStateControl;
    GameData_t& GameData;

    Flag flagGameOver;
    Flag flagGameData;
    Flag flagSendReady;
    Flag flagPostGameData;
    
    Pool<HitArray> poolHit;
    Pool<int> poolLivesLeft;
    Pool<int> poolShotsTaken;

    enum state_connectControl_t {BootWifi, Idle, GameOver, Get_GameData, Send_Ready, SendPostGameData};
    state_connectControl_t state_connectControl = state_connectControl_t::BootWifi;

public:
    ConnectControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    GameSetupControl& gameSetupControl, ReadyUpControl& readyUpControl, GameStateControl& gameStateControl, GameData_t& GameData) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), gameSetupControl(gameSetupControl), readyUpControl(readyUpControl), gameStateControl(gameStateControl), GameData(GameData),
        flagGameOver(this), flagGameData(this), flagSendReady(this), flagPostGameData(this),
        poolHit(), poolLivesLeft(), poolShotsTaken()
    {   
        gameSetupControl.addListener(this);
        readyUpControl.addListener(this);
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
        // bool bStarted = false;
        // gpio_pad_select_gpio(18);
        // gpio_set_direction((gpio_num_t)18, GPIO_MODE_INPUT);
        GameData_t gameData;
        int lives;
        int shots;
        HitArray hits;
        for(;;){
            ESP_LOGI("ConnectControl", "ConnectControl in state :%d", state_connectControl);
            switch (state_connectControl){
                case state_connectControl_t::BootWifi:
                    // do the big wifi start, then:
                    ESP_LOGI("ConnectControl", "In state BootWifi yoohoo!");
                    gameSetupControl._start();
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::Idle:
                    //wait on everything and do the if statemten
                    // if(gpio_get_level((gpio_num_t)18) && !bStarted){
                    //     // substitute for getting ready signal from host server
                    //     readyUpControl.startGame();
                    //     bStarted = true;
                    // } else if(gpio_get_level((gpio_num_t)18) && bStarted){
                    //     gameStateControl.forceGameOver();
                    //     bStarted = false;
                    // }
                    waitAny(flagGameOver + flagGameData + flagSendReady + flagPostGameData);
                    if(hasFired(flagGameOver)){
                        state_connectControl = state_connectControl_t::GameOver;
                        break;
                    } else if(hasFired(flagGameData)){
                        state_connectControl = state_connectControl_t::Get_GameData;
                        break;
                    } else if(hasFired(flagSendReady)){
                        state_connectControl = state_connectControl_t::Send_Ready;
                        break;
                    } else if(hasFired(flagPostGameData)){
                        state_connectControl = state_connectControl_t::SendPostGameData;
                        break;
                    }
                    break;

                case state_connectControl_t::GameOver:
                    ESP_LOGI("ConnectControl", "Player Dead via meldGameOver : %d", GameData.getPlayerNum());
                    // do the sendy thing to Host Server that you're dead-o
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::Get_GameData:
                    //Read from host server
                    ESP_LOGI("ConnectControl", "ConnectControl Get_GameData");
                    gameData = GameData_t(1, 1, 20, 200, 15, 50, 2);
                    gameSetupControl.sendGameData(gameData);
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::Send_Ready:
                    ESP_LOGI("ConnectControl", "Player Ready %d", GameData.getPlayerNum());
                    // do the sendy thing to Host Server that you're ready
                    readyUpControl.startGame();
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::SendPostGameData:
                    poolLivesLeft.read(lives);
                    poolShotsTaken.read(shots);
                    poolHit.read(hits);
                    ESP_LOGI("ConnectControl", "SendPostGameData; lives: %d, shots: %d", lives, shots);
                    hits.log_contents();
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                default:
                    break;
            }
        }
    }
};
} // namespace

