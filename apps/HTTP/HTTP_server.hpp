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
#include "password.h" // Wifi "ssid" and "password" are set here. Both as const char*; "#pragma once" On line 1

namespace crt
{

    extern ILogger &logger;
    constexpr uint16_t SERVER_PORT = 4080;

    class HTTP_Server : public Task
    {

    private:
        // WiFiServer HTTPserver = WiFiServer(SERVER_PORT);
        WiFiServer HTTPserver = WiFiServer(80);
        HTTP_WiFi serverWiFi;
        String s_HostIP;
        const char *c_HostIP;

        String output26State = "off";
        String output27State = "off";
        String header;
        String gametime;

    public:
        HTTP_Server(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) : Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber)
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
            HTTPserver.begin();
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

                WiFiClient client = HTTPserver.available(); // Listen for incoming clients

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
                                    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                                    // and a content-type so the client knows what's coming, then a blank line:
                                    client.println("HTTP/1.1 200 OK");
                                    client.println("Content-type:text/html");
                                    client.println("Connection: close");
                                    client.println();

                                    else if (header.indexOf("GET /30/test") >= 0)
                                    {
                                        Serial.println("Test?");
                                    }
                                    String start = "test?fgametime=";
                                    String stop = " ";
                                    int startIndex = header.indexOf(start, 0);
                                    int stopIndex = header.indexOf(stop, startIndex);
                                    gametime = header.substring(startIndex + start.length(), stopIndex);
                                    Serial.print("Gametime is: ");
                                    Serial.print(gametime);
                                    Serial.println("");

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

                                    // Web Page Heading
                                    client.println("<body><h1>ESP32 Web Server</h1>");
                                    client.println("<form action=\"/30/test\"><label for=\"fgametime\">GameTime:</label><input type=\"text\" id=\"fgametime\" name=\"fgametime\"><br><br><input type=\"submit\" value=\"Submit\"></form>");
                                    client.println("<p>" + gametime + "</p>");
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
