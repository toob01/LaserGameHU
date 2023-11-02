#include "crt_CleanRTOS.h" // This file includes crt_Config.h  You'll need to change defines there for a release build.
#include <crt_Logger.h>
#include <crt_Handler.h>
// All Tasks should be created in this main file.
// #include "messageReceiver.hpp"
// #include "signalPauseDetector.hpp"
// #include "necReciever.hpp"
#include "TCP_server.hpp"
#include "TCP_client.hpp"
// https://stackoverflow.com/questions/74142323/esp32-fgets-to-read-from-serial-input-does-not-wait-for-input
void getLineInput(char buf[], size_t len)
{
    memset(buf, 0, len);
    fpurge(stdin); // clears any junk in stdin
    char *bufp;
    bufp = buf;
    while (true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        *bufp = getchar();
        if (*bufp != '\0' && *bufp != 0xFF && *bufp != '\r') // ignores null input, 0xFF, CR in CRLF
        {
            //'enter' (EOL) handler
            if (*bufp == '\n')
            {
                *bufp = '\0';
                break;
            } // backspace handler
            else if (*bufp == '\b')
            {
                if (bufp - buf >= 1)
                    bufp--;
            }
            else
            {
                // pointer to next character
                bufp++;
            }
        }

        // only accept len-1 characters, (len) character being null terminator.
        if (bufp - buf > (len)-2)
        {
            bufp = buf + (len - 1);
            *bufp = '\0';
            break;
        }
    }
}

void TCP_Startup()
{
    ESP_LOGI("TCP_Startup", "Start \"CLIENT\" by pressing 1 or \"SERVER\" by pressing 2.");

    enum TCP_MODE
    {
        CLIENT,
        SERVER
    };
    char userInput[5];
    // getLineInput(userInput, 5);
    userInput[0] = TCP_MODE::SERVER;
    if (int(userInput[0]) == TCP_MODE::CLIENT)
    {
        crt::TCP_Client tcp_client("TCP_Client", 2, 3000, ARDUINO_RUNNING_CORE);
    }
    else if (int(userInput[0]) == TCP_MODE::SERVER)
    {
        crt::TCP_Server tcp_server("TCP_Server", 2, 3000, ARDUINO_RUNNING_CORE);
    }
}

namespace crt
{

    const unsigned int pinButtonDump = 18;
    Logger<1000> messageLogger("messageLogger", 3, ARDUINO_RUNNING_CORE, pinButtonDump);
    ILogger &logger = messageLogger;

    MainInits mainInits; // Initialize CleanRTOS.
                         // MessageReceiver messageReceiver("MessageReceiver", 2 /*priority*/, 2000 /*stackBytes*/, ARDUINO_RUNNING_CORE);
    TCP_Client tcp_client("TCP_Client", 2, 3000, ARDUINO_RUNNING_CORE);
    //TCP_Server tcp_server("TCP_Server", 2, 3000, ARDUINO_RUNNING_CORE);
}

void setup()
{
    //TCP_Startup();
}

void loop()
{
    vTaskDelay(1); // Nothing to do in loop - all example code runs in the 4 threads above.
}
