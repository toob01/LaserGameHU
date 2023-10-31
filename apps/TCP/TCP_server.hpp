#pragma once
#include <WiFi.h>

#define LED_PIN  18 // ESP32 pin GPIO18 connected to LED
#define SERVER_PORT 4080

const char* ssid = "";     // SET WIFI SSID
const char* password = ""; // SET WIFI PASSWORD

namespace crt{

extern ILogger& logger;

class TCP_Server : public task{

    private:
    WifiServer Server(SERVER_PORT);
    
    void TCP_Print(std::string to_print){ // For simple text prints
        ESP_LOGI("TCP_Server", to_print);
    }

    public:
    TCP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber){
        start();
    }

    void main(){

        TCP_Print("Setting up server");

        // Connect to Wi-Fi
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            TCP_Print("Connecting to WiFi...");
        }
        TCP_Print("Connected to WiFi");

        // Print your local IP address:
        TCP_Print("ESP32 #2: TCP Server IP address: ");
        TCP_Print("%f", WiFi.localIP());
        TCP_Print("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

        // Start listening for a TCP client (from ESP32 #1)
        TCP_Server.begin();

        for (;;)
        {
            // Wait for a TCP client from ESP32 #1:
            WiFiClient client = Server.available();

            if (client) {
                // Read the command from the TCP client:
                char command = client.read();
                TCP_Print("ESP32 #2: - Received command: ");
                TCP_Print(command);

                if (command == '1')
                TCP_Print("Received 1");
                else if (command == '0')
                TCP_Print("Received 0");

                client.stop();
                    }
        
    }

}


}


void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("ESP32 #2: TCP SERVER + AN LED");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print your local IP address:
  Serial.print("ESP32 #2: TCP Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("ESP32 #2: -> Please update the serverAddress in ESP32 #1 code");

  // Start listening for a TCP client (from ESP32 #1)
 TCP_Server.begin();
}

void loop() {
  // Wait for a TCP client from ESP32 #1:
  WiFiClient client = TCP_Server.available();

  if (client) {
    // Read the command from the TCP client:
    char command = client.read();
    Serial.print("ESP32 #2: - Received command: ");
    Serial.println(command);

    if (command == '1')
      digitalWrite(LED_PIN, HIGH); // Turn LED on
    else if (command == '0')
      digitalWrite(LED_PIN, LOW);  // Turn LED off

    client.stop();
  }
}


