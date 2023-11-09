#pragma once

#include <ArduinoJson.h>
#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include "nvs_flash.h"
#include "HTTP_WiFi.hpp"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "password.h" // Wifi "ssid" and "password" are set here. Both as const char*; "#pragma once" On line 1

namespace crt
{

    extern ILogger &logger;
    constexpr uint16_t SERVER_PORT = 4080;

    class HTTP_Server : public Task
    {

    private:
        // WiFiServer HTTPserver = WiFiServer(SERVER_PORT);
        // WiFiServer HTTPserver = WiFiServer(80);
        AsyncWebServer AWserver;
        HTTP_WiFi serverWiFi;
        String s_HostIP;
        const char *c_HostIP;

        String header;
        String gameLength;
        String s_PplayerAmount; // max 32 / 5bit
        String s_PteamAmount;   // max 8 / 3bit
        String s_Plives;        // defealt 100
        String s_PgameLength;   // in seconds
        String s_PweaponDamage; // max 127 / 7bit
        String s_PreloadTime;   // in seconds  */
        DynamicJsonDocument playersData;
        String playersDataString = "{}";

        JsonObject objGameSettings;
        String gameSettingsBuffer = "NULL";
        bool debugReadGameSettings = false;
        bool gameSettingsSet = false;

    private:
        void getSettingFromURL(String settingName, String &settingValue)
        {
            String start = settingName + "=";
            String stop;
            settingName == "PreloadTime" ? stop = " " : stop = "&";

            int startIndex = header.indexOf(start, 0);
            int stopIndex = header.indexOf(stop, startIndex);

            settingValue = header.substring(startIndex + start.length(), stopIndex);
            Serial.print(settingName + " is: ");
            Serial.print(settingValue);
            Serial.println("");
        }

        void printWebPageBasic(WiFiClient &client)
        {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
        }

        void setAWserverActions()
        {
            AWserver.on(
                "/players",
                HTTP_POST,
                [](AsyncWebServerRequest *request) {},
                NULL,
                [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                {
                    Serial.println("Current server side player data at start of request:");
                    Serial.println(playersDataString);
                    DynamicJsonDocument doc(1024);
                    String response;

                    deserializeJson(doc, data);

                    if (doc.is<JsonArray>())
                    {
                        JsonArray players = doc.as<JsonArray>();

                        for (size_t i = 0; i < players.size(); ++i)
                        {
                            const JsonObject player = players[i].as<JsonObject>();
                            Serial.println(player["Pplayer_ID"].as<String>());
                            Serial.println(player["PplayerIP"].as<String>());
                            // Process each player as needed
                        }
                        playersData = doc;
                        playersDataString = "";
                        serializeJson(doc, playersDataString);
                        response = "{\"response\":\"JSON Array Received\"}";
                    }
                    else if (doc.is<JsonObject>())
                    {
                        const JsonObject player = doc.as<JsonObject>();
                        Serial.println(player["Pplayer_ID"].as<String>());
                        Serial.println(player["PplayerIP"].as<String>());
                        // Process the single player as needed
                        playersData = doc;
                        playersDataString = "";
                        serializeJson(doc, playersDataString);
                        response = "{\"response\":\"JSON Object Received\"}";
                    }
                    else
                    {
                        response = "{\"response\":\"Invalid JSON\"}";
                    }
                    playersData = doc;
                    playersDataString = "";
                    serializeJson(doc, playersDataString);
                    Serial.println("Current server side player data at end of request:");
                    Serial.println(playersDataString);

                    // Send a response back to the client
                    request->send(200, "application/json", response);
                });

            AWserver.on("/readPlayers",
                        HTTP_GET,
                        [&](AsyncWebServerRequest *request)
                        {
                            Serial.println("Someone requested player data");
                            Serial.println("The playerdata they will receive= ");
                            Serial.println(playersDataString);
                            if (playersDataString == "{}")
                            {
                                request->send(200, "application/json", playersDataString);
                            }

                            if (!playersData.isNull())
                            {
                                request->send(200, "application/json", playersDataString);
                            }
                            else
                            {
                                request->send(200, "application/json", "{\"response\":\"No player data available\"}");
                            }
                        });

            AWserver.on(
                "/processGameSettings",
                HTTP_POST,
                [](AsyncWebServerRequest *request) {},
                NULL,
                [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                {
                    DynamicJsonDocument doc(1024);
                    // for (size_t i = 0; i < len; i++)
                    // {
                    //     Serial.write(data[i]);
                    // }

                    gameSettingsBuffer = String((char *)data);
                    String temp_buf = gameSettingsBuffer;
                    deserializeJson(doc, temp_buf);
                    objGameSettings = doc.as<JsonObject>();
                    Serial.println("GameSettingsBuffer shown below:");
                    Serial.println(gameSettingsBuffer);
                    Serial.println("Sending requester back to index_html");
                    request->send_P(200, "text/html", index_html);
                });

            AWserver.on("/readGameSettings", HTTP_GET, [&](AsyncWebServerRequest *request)
                        {
                            if (objGameSettings.isNull() != true)
                            {
                                // DynamicJsonDocument doc(1024);
                                String output;
                                serializeJson(objGameSettings, output);
                                Serial.println("The value that is contained in output: ");
                                Serial.println(output);
                                request->send(200, "application/json", gameSettingsBuffer);
                            }
                            else if (gameSettingsBuffer == "NULL")
                            {
                                request->send(200, "text/html", "No game settings available");
                            }
                            else
                            {
                                request->send(200, "text/html", gameSettingsBuffer);
                            } });
            // Route for root / web page
            AWserver.on("/gameSettings", HTTP_GET, [&](AsyncWebServerRequest *request)
                        { request->send_P(200, "text/html", index_html); });
        }

    public:
        HTTP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), AWserver(80), playersData(1024)
        {
            start();
        }

        void main()
        {
            vTaskDelay(100); // To prevent freeze on boot
            Serial.begin(115200);

            ESP_LOGI("HTTP_Server", "Setting up HTTPserver");
            // Initialize NVS
            serverWiFi.set_nvs_flash();
            serverWiFi.wifi_setup();

            // Create WiFi SoftAP and get IP
            IPAddress IP = serverWiFi.wifi_ap_create(ssid, password);
            s_HostIP = IP.toString();
            c_HostIP = s_HostIP.c_str();

            // Start listening for a HTTP client (from ESP32 #1)
            ESP_LOGI("HTTP_Server", "Server started at: %s", c_HostIP);
            setAWserverActions();
            AWserver.begin();
            Serial.println("HTTP Server started");
            for (;;)
            {
                vTaskDelay(1);
            }

            vTaskDelay(2); // Prevent watchdog trigger
        }

        const char *index_html PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    body { max-width: 600px; margin: 0px auto; padding-bottom: 25px; }
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <form id="settingsForm">
    <label for="PplayerAmount">Player Amount:</label>
    <input type="text" id="PplayerAmount" name="PplayerAmount"><br>
    <label for="PteamAmount">Team Amount:</label>
    <input type="text" id="PteamAmount" name="PteamAmount"><br>
    <label for="Plives">Lives:</label>
    <input type="text" id="Plives" name="Plives"><br>
    <label for="PgameLength">Game Length:</label>
    <input type="text" id="PgameLength" name="PgameLength"><br>
    <label for="PweaponDamage">Weapon Damage:</label>
    <input type="text" id="PweaponDamage" name="PweaponDamage"><br>
    <label for="PreloadTime">Reload Time:</label>
    <input type="text" id="PreloadTime" name="PreloadTime"><br>
    <br>
    <input type="button" value="Submit" onclick="submitForm()">
  </form>
  <script>
    function submitForm() {
      var PplayerAmount = document.getElementById("PplayerAmount").value;
      var PteamAmount = document.getElementById("PteamAmount").value;
      var Plives = document.getElementById("Plives").value;
      var PgameLength = document.getElementById("PgameLength").value;
      var PweaponDamage = document.getElementById("PweaponDamage").value;
      var PreloadTime = document.getElementById("PreloadTime").value;
      var fSettingsSet = true;
      var jsonData = {
        PplayerAmount: PplayerAmount,
        PteamAmount: PteamAmount,
        Plives: Plives,
        PgameLength: PgameLength,
        PweaponDamage: PweaponDamage,
        PreloadTime: PreloadTime,
        fSettingsSet: fSettingsSet
      };
      var xhr = new XMLHttpRequest();
      xhr.open("POST", "/processGameSettings", true);
      xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
      xhr.send(JSON.stringify(jsonData));
      xhr.onload = function () {
        if (xhr.status === 200) {
          console.log("Server response: " + xhr.responseText);
        }
      };
    }
  </script>
</body>
</html>
)rawliteral";
    };
};
