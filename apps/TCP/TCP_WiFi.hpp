#pragma once
#include <WiFi.h>
#include "nvs_flash.h"

class TCP_WiFi
{
private:
    String s_IPadress;
    const char *c_IPadress;

    String s_WiFiState;
    const char *c_WiFiState;

public:
    TCP_WiFi(){};
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
            ESP_LOGI("TCP_WiFi", "No networks found");
            for (;;)
            {
            }
        }

        for (int i = 0; i < networks; i++)
        {
            ESP_LOGI("TCP_WiFi", "Network : %i, SSID: %s", networks, WiFi.SSID(i).c_str());
        }
    }
    void wifi_connect(const char *given_ssid, const char *given_password)
    {
        WiFi.begin(given_ssid, given_password);

        bool WiFi_Connected = false;
        while (WiFi_Connected != true)
        {

            switch (WiFi.status())
            {
            case WL_NO_SSID_AVAIL:
                ESP_LOGI("TCP_WiFi", "[WiFi] SSID not found");
                break;
            case WL_CONNECT_FAILED:
                ESP_LOGI("TCP_WiFi", "[WiFi] Failed - WiFi not connected! Reason: ");
                break;
            case WL_CONNECTION_LOST:
                ESP_LOGI("TCP_WiFi", "[WiFi] Connection was lost");
                break;
            case WL_SCAN_COMPLETED:
                ESP_LOGI("TCP_WiFi", "[WiFi] Scan is completed");
                break;
            case WL_DISCONNECTED:
                ESP_LOGI("TCP_WiFi", "[WiFi] WiFi is disconnected");
                break;
            case WL_CONNECTED:
                ESP_LOGI("TCP_WiFi", "[WiFi] WiFi is connected!");
                ESP_LOGI("TCP_WiFi", "[WiFi] IP address: ");
                s_IPadress = WiFi.localIP().toString();
                c_IPadress = s_IPadress.c_str();
                ESP_LOGI("TCP_WiFi", "%s", c_IPadress);
                WiFi_Connected = true;
                break;
            default:
                ESP_LOGI("TCP_WiFi", "[WiFi] WiFi Status unknown ");
                break;
            }

            delay(500);
        }
    }
};