#pragma once
#include <crt_CleanRTOS.h>
#include "GameSetupControl.hpp"
#include "readyUpControl.hpp"
#include "SendPostGameDataControl.hpp"
#include "gameStateControl.hpp"
#include "GameData.hpp"
#include "ISetupListener.hpp"
#include "IReadyUpListener.hpp"
#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include "nvs_flash.h"
#include "HTTP_WiFi.hpp"
#include "password.h"
#include "HTTP_client.hpp"
#include "IClientListener.hpp"

namespace crt
{
class ConnectControl : public Task, public ISetupListener, public IReadyUpListener, public IClientListener {
private:

    GameSetupControl& gameSetupControl;
    ReadyUpControl& readyUpControl;
    GameStateControl& gameStateControl;
    GameData_t& GameData;
    HTTP_Client& httpClient;

    Flag flagGameOver;
    Flag flagGameData;
    Flag flagSendReady;
    Flag flagPostGameData;
    Flag flagStartGame;
    
    Pool<HitArray> poolHit;
    Pool<int> poolLivesLeft;
    Pool<int> poolShotsTaken;

    enum state_connectControl_t {BootWifi, Idle, GameOver, Get_GameData, Send_Ready, WaitForStart, SendPostGameData};
    state_connectControl_t state_connectControl = state_connectControl_t::BootWifi;

    HTTP_WiFi serverWiFi;
    const char *serverURLplayers = "http://192.168.4.1/players";
    const char *serverURLreadPlayers = "http://192.168.4.1/readPlayers";
    const char *serverURLgameSettings = "http://192.168.4.1/readGameSettings";
    const char *serverURLreadStart = "http://192.168.4.1/readStart";
    bool requested = false;
    bool fSettingsSet = false;
    JsonObject jsonSettings;
    int httpResponseCode;
    bool serverGameStart = false;

public:
    ConnectControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    GameSetupControl& gameSetupControl, ReadyUpControl& readyUpControl, GameStateControl& gameStateControl, GameData_t& GameData, HTTP_Client& httpClient) :
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), gameSetupControl(gameSetupControl), readyUpControl(readyUpControl), gameStateControl(gameStateControl), 
        GameData(GameData), httpClient(httpClient), flagGameOver(this), flagGameData(this), flagSendReady(this), flagPostGameData(this), flagStartGame(this),
        poolHit(), poolLivesLeft(), poolShotsTaken()
    {
        gameSetupControl.addListener(this);
        readyUpControl.addListener(this);
        httpClient.addListener(this);
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

    void startGame() override {
        flagStartGame.set();
    }

    void sendPostGameData(HitArray hit, int livesLeft, int shotsTaken){
        poolHit.write(hit);
        poolLivesLeft.write(livesLeft);
        poolShotsTaken.write(shotsTaken);
        flagPostGameData.set();
    }

    void main(){
        GameData_t gameData;
        int lives;
        int shots;
        HitArray hits;
        for(;;){
            ESP_LOGI("ConnectControl", "ConnectControl in state :%d", state_connectControl);
            switch (state_connectControl){
                case state_connectControl_t::BootWifi:
                    httpClient.HTTP_Client_Setup();
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
                    httpClient.postPlayer(false, true);
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::Get_GameData:
                    //Read from host server
                    ESP_LOGI("ConnectControl", "ConnectControl Get_GameData");
                    httpClient.readGameSettings();
                    //Only use even team numbers for now
                    gameData = GameData_t(httpClient.playerID_int, 2, httpClient.PgameLength, httpClient.Plives, httpClient.PmaxAmmo, httpClient.PweaponDamage, httpClient.PreloadTime);
                    gameSetupControl.sendGameData(gameData);
                    state_connectControl = state_connectControl_t::Idle;
                    break;
                case state_connectControl_t::Send_Ready:
                    ESP_LOGI("ConnectControl", "Player Ready %d", GameData.getPlayerNum());
                    httpClient.postPlayer(true, false);
                    // do the sendy thing to Host Server that you're ready
                    state_connectControl = state_connectControl_t::WaitForStart;
                    break;
                case state_connectControl_t::WaitForStart:
                    wait(flagStartGame);
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

