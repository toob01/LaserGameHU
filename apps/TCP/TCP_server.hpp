#pragma once
#include <WiFi.h>
#include <istream>
#include <fstream>
#include <string>
#include "nvs_flash.h"
#include "TCP_WiFi.hpp"
#include "password.h" // Wifi "ssid" and "password" are set here. Both as const char*; "#pragma once" On line 1

namespace crt
{

    extern ILogger &logger;
    constexpr uint16_t SERVER_PORT = 4080;

    class TCP_Server : public Task
    {

    private:
        WiFiServer TCPserver = WiFiServer(SERVER_PORT);
        TCP_WiFi serverWiFi;
       
    public:
        TCP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

        void main()
        {

            ESP_LOGI("TCP_Server", "Setting up server");
            // Initialize NVS
            serverWiFi.set_nvs_flash();
            serverWiFi.wifi_setup();
            serverWiFi.wifi_scan();
            serverWiFi.wifi_connect(ssid, password);

            // Start listening for a TCP client (from ESP32 #1)
            TCPserver.begin();
            ESP_LOGI("TCP_Server", "Server started.");
            for (;;)
            {

                // Wait for a TCP client from ESP32 #1:
                WiFiClient client = TCPserver.available();

                if (client)
                {
                    ESP_LOGI("TCP_Server", "Waiting for client data");
                    // Read the command from the TCP client:
                    char command = client.read();
                    ESP_LOGI("TCP_Server", "ESP32 #2: - Received command: ");
                    ESP_LOGI("TCP_Server", "%c", command);

                    if (command == '1')
                        ESP_LOGI("TCP_Server", "Received 1");
                    else if (command == '0')
                        ESP_LOGI("TCP_Server", "Received 0");

                    client.stop();
                }
                vTaskDelay(1); // Prevent watchdog trigger
            }
        }
    };
}