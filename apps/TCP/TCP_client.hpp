#pragma once
#include "Arduino.h"
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

    class TCP_Client : public Task
    {

    private:
        // WiFiServer TCPserver = WiFiServer(SERVER_PORT);
        WiFiClient TCPclient;
        TCP_WiFi serverWiFi;
        const char *serverAddress = "192.168.4.1"; // ESP32 SERVER ADRESS
        const int server_Port = 4080;

    public:
        TCP_Client(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

        void main()
        {

            ESP_LOGI("TCP_Client", "Setting up server");
            //Serial.begin(115200);
            //Serial.println("Test");
            // Initialize NVS
            serverWiFi.set_nvs_flash();
            serverWiFi.wifi_setup();
            //serverWiFi.wifi_scan();
            serverWiFi.wifi_connect(ssid, password);
            //Serial.println("Test2");
            // connect to TCP server (ESP32 Server)
            //String printAdress = serverAddress.c_str();
            ESP_LOGI("TCP_Client", "Connecting to %s, at port %i", serverAddress, server_Port);
            //Serial.println("Test3");
            if (TCPclient.connect(serverAddress, server_Port, 500))
            {
                ESP_LOGI("TCP_Client", "Connected to TCP server");
            }
            else
            {
                ESP_LOGI("TCP_Client", "Failed to connect to TCP server");
            }
            vTaskDelay(100);
            for (;;)
            {
                if (!TCPclient.connected())
                {
                    ESP_LOGI("TCP_Client", "Connection is disconnected");
                    TCPclient.stop();

                    // reconnect to TCP server (ESP32 Server)
                    if (TCPclient.connect(serverAddress, server_Port, 500))
                    {
                        ESP_LOGI("TCP_Client", "Reconnected to TCP server");
                    }
                    else
                    {
                        ESP_LOGI("TCP_Client", "Failed to reconnect to TCP server");
                    }
                } else
                {

                //TCPclient.write('1');
                //TCPclient.flus1h();
                TCPclient.println("Test");
                ESP_LOGI("TCP_Client", "Sent a 1");
                vTaskDelay(1000);
                if (!TCPclient.connected())
                {
                    ESP_LOGI("TCP_Client", "Already disconnected here");
                }
                
                // TCPclient.write('0');
                // TCPclient.flush();
                // TCPclient.println("0");
                // ESP_LOGI("TCP_Client", "Sent a 0");
                // vTaskDelay(1000);
                }
                vTaskDelay(1);
            }
        }
    };
}