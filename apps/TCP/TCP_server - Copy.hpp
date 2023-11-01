#pragma once
#include <WiFi.h>
#include <istream>
#include <fstream>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "password.h"

const char* ssid = "Desktop_Skynet";     // SET WIFI SSID. PASSWORD IS SET IN GITIGNORED "PASSWORD.H"

namespace crt{

extern ILogger& logger;
constexpr uint16_t SERVER_PORT = 4080;

class TCP_Server : public Task {

    private:
    WiFiServer TCPserver = WiFiServer(SERVER_PORT);

    static void wifi_scan(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));
    ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
    for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
        ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);
        print_auth_mode(ap_info[i].authmode);
        if (ap_info[i].authmode != WIFI_AUTH_WEP) {
            print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
        }
        ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
    }

}


    public:
    TCP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber){
        start();
    }

    void main(){

        // vTaskDelay(1000);W
        ESP_LOGI("TCP_Server", "Setting up server");

        // WiFi.nvs_enable()
        // // Connect to Wi-Fi

        // Initialize NVS
        esp_err_t ret = nvs_flash_init();
        if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            ret = nvs_flash_init();
        }
        ESP_ERROR_CHECK( ret );
        wifi_scan();

    //     WiFi.mode(WIFI_STA);
    //     WiFi.disconnect(); // Disconnect if still connected to a previous station
    //     int networks = WiFi.scanNetworks();
    //     if (networks == 0)
    //     {
    //         ESP_LOGI("TCP_Server", "No networks found");
    //         for (;;)
    //         {
    //         }
            
    //     }
        
    //     for (int i = 0; i < networks; i++)
    //     {
    //         ESP_LOGI("TCP_Server", "Network : %i, SSID: %s", networks, WiFi.SSID(i).c_str());
    //     }
        


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