#pragma once
#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <istream>
#include <fstream>
#include <string>
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
        //WiFiServer HTTPserver = WiFiServer(80);
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

        struct Player
        {
            int id;
            String IP_Adress;
        };

        Player p1;

        bool debugReadGameSettings = false;

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

    public:
        HTTP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), AWserver(80)
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
            // HTTPserver.begin();
            ESP_LOGI("HTTP_Server", "Server started at: %s", c_HostIP);

            /*
                        if (MDNS.begin("esp32"))
                        {
                            Serial.println("MDNS Responder started");
                        }

                        HTTPserver.on("/", []()
                                  { HTTPserver.send(200, "text/plain", "Hello from ESP32"); });
                        HTTPserver.on("/myServer", [](){

                            String message = "Succes Receiving\n\n";
                            message += "URI: ";
                            message += HTTPserver.uri();
                            message += "\nMethod: ";
                            message += (HTTPserver.method() == HTTP_GET) ? "GET" : "POST";
                            message += "\nArguments: ";
                            message += HTTPserver.args();
                            message += "\n";
                            for (uint8_t i = 0; i < HTTPserver.args(); i++)
                            {
                                message += " " + HTTPserver.argName(i) + ": " + HTTPserver.arg(i) + "\n";
                            }
                            HTTPserver.send(200, "text/plain", message);


                        });
            */
            //HTTPserver.begin();
            AWserver.on(
                "/players",
                HTTP_POST,
                [](AsyncWebServerRequest *request) {},
                NULL,
                [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
                {
                    for (size_t i = 0; i < len; i++)
                    {
                        Serial.write(data[i]);
                    }

                    Serial.println();

                    request->send(200);
                });

            AWserver.begin();
            Serial.println("HTTP Server started");
            for (;;)
            {
                /*
                HTTPserver.handleClient();
                // Wait for a HTTP client from ESP32 #1:
                WiFiClient client = HTTPserver.available();

                if (client)
                {
                    ESP_LOGI("HTTP_Server", "Waiting for client data");
                    // Read the command from the HTTP client:
                    char command = client.read();
                    ESP_LOGI("HTTP_Server", "ESP32 #2: - Received command: ");
                    ESP_LOGI("HTTP_Server", "Received %c", command);

                    if (command == '1')
                    {
                        client.stop();
                    }
                }
                */

                WiFiClient client = AWserver.available(); // Listen for incoming clients

                if (client)
                {                                  // If a new client connects,
                    Serial.println("New Client."); // print a message out in the serial port
                    String currentLine = "";       // make a String to hold incoming data from the client
                    while (client.connected())
                    { // loop while the client's connected
                        if (client.available())
                        {                           // if there's bytes to read from the client,
                            char c = client.read(); // read a byte, then
                            Serial.write(c);        // print it out the serial monitor
                            header += c;
                            if (c == '\n')
                            { // if the byte is a newline character
                                // if the current line is blank, you got two newline characters in a row.
                                // that's the end of the client HTTP request, so send a response:
                                if (currentLine.length() == 0)
                                {

                                    printWebPageBasic(client);

                                    if (header.indexOf("GET /gameSettings") >= 0)
                                    {
                                        // Web Page Heading
                                        client.println("<body><h1>ESP32 Web Server</h1>");
                                        client.println("<form action=\"/gameSettings\">");

                                        client.println("<label for=\"PplayerAmount\">Player Amount:</label><input type=\"text\" id=\"PplayerAmount\" name=\"PplayerAmount\">");
                                        client.println("<label for=\"PteamAmount\">Team Amount:</label><input type=\"text\" id=\"PteamAmount\" name=\"PteamAmount\">");
                                        client.println("<label for=\"Plives\">Lives:</label><input type=\"text\" id=\"Plives\" name=\"Plives\">");
                                        client.println("<label for=\"PgameLength\">Game Length:</label><input type=\"text\" id=\"PgameLength\" name=\"PgameLength\">");
                                        client.println("<label for=\"PweaponDamage\">Weapon Damage:</label><input type=\"text\" id=\"PweaponDamage\" name=\"PweaponDamage\">");
                                        client.println("<label for=\"PreloadTime\">Reload Time:</label><input type=\"text\" id=\"PreloadTime\" name=\"PreloadTime\">");

                                        client.println("<br><br><input type=\"submit\" value=\"Submit\"></form>");

                                        getSettingFromURL("PplayerAmount", s_PplayerAmount);
                                        getSettingFromURL("PteamAmount", s_PteamAmount);
                                        getSettingFromURL("Plives", s_Plives);
                                        getSettingFromURL("PgameLength", s_PgameLength);
                                        getSettingFromURL("PweaponDamage", s_PweaponDamage);
                                        getSettingFromURL("PreloadTime", s_PreloadTime);
                                    }
                                    else if (header.indexOf("GET /readGameSettings/") >= 0)
                                    {

                                        if (debugReadGameSettings == true)
                                        {
                                            client.println("<p> PplayerAmount: " + s_PplayerAmount + "</p>");
                                            client.println("<p> PteamAmount: " + s_PteamAmount + "</p>");
                                            client.println("<p> Plives: " + s_Plives + "</p>");
                                            client.println("<p> PgameLength: " + s_PgameLength + "</p>");
                                            client.println("<p> PweaponDamage: " + s_PweaponDamage + "</p>");
                                            client.println("<p> PreloadTime: " + s_PreloadTime + "</p>");
                                        }
                                        else if (debugReadGameSettings == false)
                                        {
                                            client.println(s_PplayerAmount + "," + s_PteamAmount + "," + s_Plives + "," + s_PgameLength + "," + s_PweaponDamage + "," + s_PreloadTime);
                                        }
                                    }
                                    else if (header.indexOf("GET /players") >= 0)
                                    {
                                        client.println("A player has been added");
                                        getSettingFromURL("playerIP", p1.IP_Adress);
                                        client.println(p1.IP_Adress);
                                    }

                                    client.println("</body></html>");
                                    // The HTTP response ends with another blank line
                                    client.println();
                                    // Break out of the while loop
                                    break;
                                }
                                else
                                { // if you got a newline, then clear currentLine
                                    currentLine = "";
                                }
                            }
                            else if (c != '\r')
                            {                     // if you got anything else but a carriage return character,
                                currentLine += c; // add it to the end of the currentLine
                            }
                        }
                        vTaskDelay(1);
                    }
                    // Clear the header variable
                    header = "";
                    // Close the connection
                    client.stop();
                    Serial.println("Client disconnected.");
                    Serial.println("");
                }
                vTaskDelay(1);
            }

            vTaskDelay(2); // Prevent watchdog trigger
        }
    };
};
