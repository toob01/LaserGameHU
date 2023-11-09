#pragma once
#include "Arduino.h"
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <istream>
#include <fstream>
#include <string>
#include "nvs_flash.h"
#include "HTTP_WiFi.hpp"
#include <crt_CleanRTOS.h>
#include "IClientListener.hpp"
#include "password.h" // Wifi "ssid" and "password" are set here. Both as const char*; "#pragma once" On line 1

namespace crt
{

    extern ILogger &logger;

    class HTTP_Client : public Task
    {

    private:
        enum state_Client_t {Idle, PostPlayer, ReadGameSettings, CheckGameStart, ClientSetup};
        state_Client_t state_Client = state_Client_t::Idle;

        HTTP_WiFi serverWiFi;
        HTTPClient http;
        const char *serverURLplayers = "http://192.168.4.1/players";
        const char *serverURLreadPlayers = "http://192.168.4.1/readPlayers";
        const char *serverURLgameSettings = "http://192.168.4.1/readGameSettings";
        const char *serverURLreadStart = "http://192.168.4.1/readStart";
        bool requested = false;
        bool fSettingsSet = false;
        JsonObject jsonSettings;
        int httpResponseCode;
        bool serverGameStart = false;
        IClientListener* arClientListeners[1];
        int listenerAmount;

        Pool<bool> poolReady;
        Pool<bool> poolGameOver;
        Flag flagPostPlayer;
        Flag flagReadGameSettings;
        Flag flagCheckGameStart;
        Flag flagClientSetup;

    private:
        void readJSONdata()
        {
            PplayerAmount = jsonSettings["PplayerAmount"].as<int>(); // max 32 / 5bit
            PteamAmount = jsonSettings["PteamAmount"].as<int>();     // max 8 / 3bit
            Plives = jsonSettings["Plives"].as<int>();               // defealt 100
            PgameLength = jsonSettings["PgameLength"].as<int>();     // in seconds
            PweaponDamage = jsonSettings["PweaponDamage"].as<int>(); // max 127 / 7bit
            PreloadTime = jsonSettings["PreloadTime"].as<int>();     // in seconds
            PmaxAmmo = jsonSettings["PmaxAmmo"].as<int>();

            Serial.println("JSON settings available");
            Serial.println(PplayerAmount);
            Serial.println(PteamAmount);
            Serial.println(PgameLength);
            Serial.println(PmaxAmmo);
            Serial.println(Plives);
            Serial.println(PweaponDamage);
            Serial.println(PreloadTime);
        }

    public:
        void postPlayer(bool ready, bool GameOver){
            poolReady.write(ready);
            poolGameOver.write(GameOver);
            flagPostPlayer.set();
        }

        void readGameSettings(){
            flagReadGameSettings.set();
        }

        void checkGameStart(){
            flagCheckGameStart.set();
        }

        void HTTP_Client_Setup(){
            flagClientSetup.set();
        }

    public:
        HTTP_Client(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), listenerAmount(0), poolReady(), poolGameOver(), 
        flagPostPlayer(this), flagReadGameSettings(this), flagCheckGameStart(this), flagClientSetup(this)
        {
            start();
            arClientListeners[0] = nullptr;
        }

        int PplayerAmount; // max 32 / 5bit
        int PteamAmount;   // max 8 / 3bit
        int Plives;        // defealt 100
        int PmaxAmmo;
        int PgameLength;   // in seconds
        int PweaponDamage; // max 127 / 7bit
        int PreloadTime;   // in seconds
        String playerID;
        int playerID_int;

        void addListener(IClientListener* pConnectControl){
            arClientListeners[listenerAmount++] = pConnectControl;
        }

        void main()
        {
            ESP_LOGI("HTTP_Player", "Setting up HTTPPlayer");
            // Initialize NVS
            // Start listening for a HTTP client (from ESP32 #1)

            // variables PostPlayer state
            String httpRequestData;
            String currentData;
            JsonArray players;
            bool playerExists;
            JsonObject player;
            JsonObject newPlayer;
            String payload;
            bool ready;
            bool GameOver;

            // variables ReadGameSettings state
            // unsigned long startTime;

            for (;;)
            {
                switch(state_Client){
                    case state_Client_t::Idle:
                        waitAny(flagPostPlayer + flagReadGameSettings + flagCheckGameStart + flagClientSetup);
                        if(hasFired(flagPostPlayer)){
                            state_Client = state_Client_t::PostPlayer;
                            break;
                        } else if (hasFired(flagReadGameSettings)){
                            state_Client = state_Client_t::ReadGameSettings;
                            break;
                        } else if (hasFired(flagCheckGameStart)){
                            state_Client = state_Client_t::CheckGameStart;
                            break;
                        } else if (hasFired(flagClientSetup)){
                            state_Client = state_Client_t::ClientSetup;
                            break;
                        }
                        break;
                    case state_Client_t::PostPlayer:
                        http.begin(serverURLreadPlayers); // Use URL for GET request
                        http.addHeader("Content-Type", "application/json");

                        httpResponseCode = http.GET();
                        if (httpResponseCode == HTTP_CODE_OK)
                        {
                            currentData = http.getString();
                            Serial.println("Current Player Data: " + currentData);

                            // Parse current data
                            DynamicJsonDocument doc(1024);
                            deserializeJson(doc, currentData);

                            // Check if the "players" array exists, create it if not
                            if (!doc.containsKey("players"))
                            {
                                doc.createNestedArray("players");
                            }

                            players = doc["players"].as<JsonArray>();

                            // Check if the player already exists
                            playerExists = false;
                            for (size_t i = 0; i < players.size(); ++i)
                            {
                                player = players[i].as<JsonObject>();
                                if (player["Pplayer_ID"].as<String>() == playerID && player["PplayerIP"].as<String>() == WiFi.localIP().toString())
                                {
                                    player["PplayerReady"] = ready;
                                    player["PplayerGameOver"] = GameOver;
                                    playerExists = true;
                                    break;
                                }
                            }

                            // If the player doesn't exist, add a new player
                            if (!playerExists)
                            {
                                newPlayer = players.createNestedObject();
                                newPlayer["Pplayer_ID"] = playerID;
                                newPlayer["PplayerIP"] = WiFi.localIP().toString();
                                poolReady.read(ready);
                                newPlayer["PplayerReady"] = ready;
                                poolGameOver.read(GameOver);
                                newPlayer["PplayerGameOver"] = GameOver;
                            }

                            // Serialize the updated data
                            String updatedData;
                            serializeJson(doc, updatedData);

                            httpRequestData = updatedData;
                            Serial.print("Updated Player Data: ");
                            Serial.println(httpRequestData);

                            // Send HTTP POST request with updated data
                            http.end();                   // Close the connection from the previous GET request
                            http.begin(serverURLplayers); // Use URL for POST request
                            httpResponseCode = http.POST(httpRequestData);
                            Serial.print("HTTP Response code: ");
                            Serial.println(httpResponseCode);

                            if (httpResponseCode == HTTP_CODE_OK)
                            {
                                payload = http.getString();
                                Serial.println("Server Response: " + payload);
                            }
                            else
                            {
                                Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
                            }
                        }
                        else
                        {
                            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
                        }

                        // Free resources
                        http.end();
                        state_Client = state_Client_t::Idle;
                        break;

                    case state_Client_t::ReadGameSettings:
                        http.begin(serverURLgameSettings);

                        ESP_LOGI("HTTPClient", "readGameSettings loop fSettingsSet = true");
                        if (fSettingsSet != true)
                        ESP_LOGI("HTTPClient", "readGameSettings loop");
                        {
                            {
                                httpResponseCode = http.GET();
                                if (httpResponseCode > 0)
                                {
                                    if (httpResponseCode == HTTP_CODE_OK)
                                    {
                                        payload = http.getString();
                                        Serial.println("Response payload: " + payload);

                                        DynamicJsonDocument doc(1024);

                                        deserializeJson(doc, payload);

                                        jsonSettings = doc.as<JsonObject>();

                                        // Check if settings have been set
                                        if (jsonSettings["fSettingsSet"].as<bool>() == true)
                                        {
                                            playerID = WiFi.localIP().toString().substring(10);
                                            Serial.println("PlayerID : ");
                                            Serial.println(playerID);
                                            playerID_int = playerID.toInt();
                                            ESP_LOGI("HTTPClient", "PlayerNum assigned: %d", playerID_int);
                                            readJSONdata();
                                            http.end();
                                            fSettingsSet = true;
                                            arClientListeners[0]->connectSucces();
                                            state_Client = state_Client_t::Idle;
                                        }
                                    }
                                    else
                                    {
                                        Serial.println("HTTP request failed with error code: " + String(httpResponseCode));
                                        http.end();
                                    }
                                }
                                else
                                {
                                    Serial.println("Connection failed");
                                    http.end();
                                }
                            }
                        }
                        break;

                    case state_Client_t::CheckGameStart:
                        // Make the GET request
                        http.begin(serverURLreadStart);
                        httpResponseCode = http.GET();

                        if (httpResponseCode == HTTP_CODE_OK)
                        {
                            String responseData = http.getString();

                            // Parse JSON data
                            DynamicJsonDocument doc(1024);
                            deserializeJson(doc, responseData);

                            // Check if "fGameStart" is present and true
                            if (doc.containsKey("fGameStart"))
                            {
                                if(doc["fGameStart"].as<bool>()){
                                    arClientListeners[0]->startGame();
                                    state_Client = state_Client_t::Idle;
                                } else {
                                    break;
                                }
                            }
                        }
                        break;
                    
                    case state_Client_t::ClientSetup:
                        vTaskDelay(100); // To prevent freeze on boot
                        Serial.begin(115200);
                        serverWiFi.set_nvs_flash();
                        serverWiFi.wifi_setup();
                        serverWiFi.wifi_connect(ssid, password);
                        Serial.print("Connected to SSID: ");
                        Serial.print(ssid);
                        Serial.print(", with IP: ");
                        Serial.print(WiFi.localIP());
                        Serial.println();
                        state_Client = state_Client_t::Idle;
                        break;

                    default:
                        break;
                }
                // originele debug dinges code
            //     if (WiFi.status() == WL_CONNECTED)
            //     {

            //         if (requested != true)
            //         {
            //             requested = true;
            //             postPlayer(false, false);
            //             readGameSettings();
            //             postPlayer(true, false);
            //             while (checkGameStart() != true)
            //             {
            //                 vTaskDelay(1);
            //             }
            //             // Start Player code
            //             Serial.println("Game has started!");
            //             arClientListeners[0]->startGame();
            //         }
            //     }
            //     else
            //     {
            //         Serial.println("WiFi Disconnected");
            //     }
            //     vTaskDelay(1);
            // }

            vTaskDelay(2); // Prevent watchdog trigger
        }
    };
};
}
