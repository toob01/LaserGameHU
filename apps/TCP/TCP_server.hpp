#pragma once
#include <WiFi.h>
#include <istream>
#include <fstream>
#include "nvs_flash.h"
#include "password.h"
#include <string>

const char *ssid = "Desktop_Skynet"; // SET WIFI SSID. PASSWORD IS SET IN GITIGNORED "PASSWORD.H"
namespace crt
{

    extern ILogger &logger;
    constexpr uint16_t SERVER_PORT = 4080;

    class TCP_Server : public Task
    {

    private:
        WiFiServer TCPserver = WiFiServer(SERVER_PORT);
        String s_IPadress;
        const char *c_IPadress;

        String s_WiFiState;
        const char *c_WiFiState;

        void set_nvs_flash()
        {
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
            {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
            }
            ESP_ERROR_CHECK(ret);
        }
        void wifi_setup()
        {
            WiFi.mode(WIFI_STA);
            WiFi.disconnect(); // Disconnect if still connected to a previous station
        }
        void wifi_scan()
        {
            int networks = WiFi.scanNetworks();
            if (networks == 0)
            {
                ESP_LOGI("TCP_Server", "No networks found");
                for (;;)
                {
                }
            }

            for (int i = 0; i < networks; i++)
            {
                ESP_LOGI("TCP_Server", "Network : %i, SSID: %s", networks, WiFi.SSID(i).c_str());
            }
        }
        void wifi_connect(const char* given_ssid, const char* given_password)
        {
            WiFi.begin(given_ssid, given_password);

            bool WiFi_Connected = false;
            while (WiFi_Connected != true)
            {

                switch (WiFi.status())
                {
                case WL_NO_SSID_AVAIL:
                    ESP_LOGI("TCP_Server", "[WiFi] SSID not found");
                    break;
                case WL_CONNECT_FAILED:
                    ESP_LOGI("TCP_Server", "[WiFi] Failed - WiFi not connected! Reason: ");
                    break;
                case WL_CONNECTION_LOST:
                    ESP_LOGI("TCP_Server", "[WiFi] Connection was lost");
                    break;
                case WL_SCAN_COMPLETED:
                    ESP_LOGI("TCP_Server", "[WiFi] Scan is completed");
                    break;
                case WL_DISCONNECTED:
                    ESP_LOGI("TCP_Server", "[WiFi] WiFi is disconnected");
                    break;
                case WL_CONNECTED:
                    ESP_LOGI("TCP_Server", "[WiFi] WiFi is connected!");
                    ESP_LOGI("TCP_Server", "[WiFi] IP address: ");
                    s_IPadress = WiFi.localIP().toString();
                    c_IPadress = s_IPadress.c_str();
                    ESP_LOGI("TCP_Server", "%s", c_IPadress);
                    WiFi_Connected = true;
                    break;
                default:
                    ESP_LOGI("TCP_Server", "[WiFi] WiFi Status unknown ");
                    break;
                }

                vTaskDelay(500);
            }
        }
    public:
        TCP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
        {
            start();
        }

        void main()
        {

            ESP_LOGI("TCP_Server", "Setting up server");
            // Initialize NVS
            set_nvs_flash();
            wifi_setup();
            wifi_scan();
            wifi_connect(ssid, password);
            

            for (;;)
            {
                vTaskDelay(1);
            }

            //     WiFi.begin(ssid, password);
            //     while (WiFi.status() != WL_CONNECTED) {
            //         vTaskDelay(1000);
            //         ESP_LOGI("TCP_Server", "Still Connecting to WiFi %s", ssid);
            //     }
            //     ESP_LOGI("TCP_Server", "Connected to WiFi");

            //     // Print your local IP address:
            //     ESP_LOGI("TCP_Server", "ESP32 #2: TCP Server IP address: ");
            //     const char* IP = (WiFi.localIP()).toString().c_str();
            //     ESP_LOGI("TCP_Server", "%s", IP);
            //     ESP_LOGI("TCP_Server", "ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

            //     // Start listening for a TCP client (from ESP32 #1)
            //     TCPserver.begin();
            //     ESP_LOGI("TCP_Server", "Server started.");
            //     for (;;)
            //     {

            //         // Wait for a TCP client from ESP32 #1:
            //         WiFiClient client = TCPserver.available();

            //         if (client) {
            //             // Read the command from the TCP client:
            //             char command = client.read();
            //             ESP_LOGI("TCP_Server", "ESP32 #2: - Received command: ");
            //             ESP_LOGI("TCP_Server", "%c", command);

            //             if (command == '1')
            //             ESP_LOGI("TCP_Server", "Received 1");
            //             else if (command == '0')
            //             ESP_LOGI("TCP_Server", "Received 0");

            //             client.stop();
            //                 }

            // }
        }
    };
}