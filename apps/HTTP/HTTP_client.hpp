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
#include "password.h" // Wifi "ssid" and "password" are set here. Both as const char*; "#pragma once" On line 1

namespace crt
{

    extern ILogger &logger;

    class HTTP_Client : public Task
    {

    private:
        HTTP_WiFi serverWiFi;
        const char *serverURLplayers = "http://192.168.4.1/players";
        const char *serverURLgameSettings = "http://192.168.4.1/readGameSettings";
        bool requested = false;
        bool fSettingsSet = false;
        JsonObject jsonSettings;
        int httpResponseCode;

    private:
        void readJSONdata()
        {
            PplayerAmount = jsonSettings["PplayerAmount"].as<int>(); // max 32 / 5bit
            PteamAmount = jsonSettings["PteamAmount"].as<int>();     // max 8 / 3bit
            Plives = jsonSettings["Plives"].as<int>();               // defealt 100
            PgameLength = jsonSettings["PgameLength"].as<int>();     // in seconds
            PweaponDamage = jsonSettings["PweaponDamage"].as<int>(); // max 127 / 7bit
            PreloadTime = jsonSettings["PreloadTime"].as<int>();     // in seconds

            Serial.println(PplayerAmount);
            Serial.println(PteamAmount);
            Serial.println(Plives);
            Serial.println(PgameLength);
            Serial.println(PweaponDamage);
            Serial.println(PreloadTime);
        }
        void postPlayer(HTTPClient &http, bool ready)
        {
            http.begin(serverURLplayers);
            http.addHeader("Content-Type", "application/json");
            String playerID = WiFi.localIP().toString().substring(10);
            String httpRequestData;
            if (ready == true)
            {
                httpRequestData = "{\"Pplayer_ID\":\"" + playerID + "\",\"PplayerIP\":\"" + WiFi.localIP().toString() + "\", \"PplayerReady\":true}";
            } else if (ready != true)
            {
                httpRequestData = "{\"Pplayer_ID\":\"" + playerID + "\",\"PplayerIP\":\"" + WiFi.localIP().toString() + "\", \"PplayerReady\":false}";
            }
            
            Serial.print("httpRequestData Contains: ");
            Serial.println(httpRequestData);

            // Send HTTP POST request
            httpResponseCode = http.POST(httpRequestData);
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);

            // httpCode will be negative on error
            if (httpResponseCode > 0)
            {
                // file found at server
                if (httpResponseCode == HTTP_CODE_OK)
                {
                    String payload = http.getString();
                    Serial.println(payload);
                }
                else
                {
                    // HTTP header has been send and Server response header has been handled
                    Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
                }
            }
            else
            {
                Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
            }

            // Free resources
            http.end();
        }
        void readGameSettings(HTTPClient &http)
        {

            http.begin(serverURLgameSettings);

            unsigned long startTime;
            startTime = millis();
            while (fSettingsSet != true)
            {
                if (millis() - startTime >= 5000)
                {
                    // 5 seconds have elapsed. ... do something interesting ...
                    startTime = millis();
                    httpResponseCode = http.GET();
                    if (httpResponseCode > 0)
                    {
                        if (httpResponseCode == HTTP_CODE_OK)
                        {
                            String payload = http.getString();
                            Serial.println("Response payload: " + payload);

                            DynamicJsonDocument doc(1024);

                            deserializeJson(doc, payload);

                            jsonSettings = doc.as<JsonObject>();

                            // Check if settings have been set
                            if (jsonSettings["fSettingsSet"].as<bool>() == true)
                            {
                                readJSONdata();
                                http.end();
                                fSettingsSet = true;
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
                vTaskDelay(1);
            }
        }

    public:
        HTTP_Client(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

        int PplayerAmount; // max 32 / 5bit
        int PteamAmount;   // max 8 / 3bit
        int Plives;        // defealt 100
        int PgameLength;   // in seconds
        int PweaponDamage; // max 127 / 7bit
        int PreloadTime;   // in seconds

        void main()
        {
            vTaskDelay(100); // To prevent freeze on boot
            Serial.begin(115200);

            ESP_LOGI("HTTP_Player", "Setting up HTTPPlayer");
            // Initialize NVS
            serverWiFi.set_nvs_flash();
            serverWiFi.wifi_setup();
            serverWiFi.wifi_connect(ssid, password);
            Serial.print("Connected to SSID: ");
            Serial.print(ssid);
            Serial.print(", with IP: ");
            Serial.print(WiFi.localIP());
            Serial.println();
            // Start listening for a HTTP client (from ESP32 #1)

            for (;;)
            {
                if (WiFi.status() == WL_CONNECTED)
                {

                    if (requested != true)
                    {
                        requested = true;
                        HTTPClient http;
                        postPlayer(http, false);
                        readGameSettings(http);
                        postPlayer(http, true);
                    }
                }
                else
                {
                    Serial.println("WiFi Disconnected");
                }
                vTaskDelay(1);
            }

            vTaskDelay(2); // Prevent watchdog trigger
        }
    };
};
