#pragma once
#include <WiFi.h>
#include <istream>
#include <fstream>
#include "password.h"

const char* ssid = "Skynet";     // SET WIFI SSID. PASSWORD IS SET IN GITIGNORED "PASSWORD.H"

namespace crt{

extern ILogger& logger;

constexpr int LED_PIN = 18; // ESP32 pin GPIO18 connected to LED
constexpr uint16_t SERVER_PORT = 4080;

class TCP_Server : public Task {

    private:
    WiFiServer TCPserver = WiFiServer(SERVER_PORT);

    public:
    TCP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber){
        start();
    }

    void main(){

        ESP_LOGI("TCP_Server", "Setting up server");

        // Connect to Wi-Fi
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(1000);
            ESP_LOGI("TCP_Server", "Connecting to WiFi %s", ssid);
        }
        ESP_LOGI("TCP_Server", "Connected to WiFi");

        // Print your local IP address:
        ESP_LOGI("TCP_Server", "ESP32 #2: TCP Server IP address: ");
        const char* IP = (WiFi.localIP()).toString().c_str();
        ESP_LOGI("TCP_Server", "%s", IP);
        ESP_LOGI("TCP_Server", "ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

        // Start listening for a TCP client (from ESP32 #1)
        TCPserver.begin();
        ESP_LOGI("TCP_Server", "Server started.");
        for (;;)
        {
            
            // Wait for a TCP client from ESP32 #1:
            WiFiClient client = TCPserver.available();

            if (client) {
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
        
    }

}
};
}