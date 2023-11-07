#pragma once
#include "Arduino.h"
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
        // WiFiServer HTTPserver = WiFiServer(SERVER_PORT);
        HTTP_WiFi serverWiFi;
        String s_HostIP;
        const char *c_HostIP;
        const char *serverURL = "192.168.4.1:80/players";
        bool debugReadGameSettings = false;

    public:
        HTTP_Client(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

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
            // HTTPserver.begin();

            for (;;)
            {
                if (WiFi.status() == WL_CONNECTED)
                {

                    WiFiClient client;
                    HTTPClient http;
                    Serial.println("Still working here.");
                    http.begin(client, serverURL);
                    Serial.println("Still working here. 2");
                    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                    String httpRequestData = "player_ID=1&playerIP=" + WiFi.localIP();
                    // Send HTTP POST request
                    int httpResponseCode = http.POST(httpRequestData);
                    Serial.println("Still working here. 3");
                    Serial.print("HTTP Response code: ");
                    Serial.println(httpResponseCode);

                    // Free resources
                    http.end();
                    /*
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
                    */
                }
                else{Serial.println("WiFi Disconnected");}
                vTaskDelay(1);
            }

            vTaskDelay(2); // Prevent watchdog trigger
        }
    };
};
