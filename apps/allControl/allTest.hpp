#include <crt_CleanRTOS.h>
#include "displayControl.hpp"
#include "speakerControl.hpp"
#include "GameOverControl.hpp"
#include "ConnectControl.hpp"
#include "GameSetupControl.hpp"
#include "readyUpControl.hpp"
#include "gameStateControl.hpp"
#include "messageReceiver.hpp"
#include "messageSender.hpp"
#include "receivingHitControl.hpp"
#include "shootingControl.hpp"
#include "necReciever.hpp"
#include "signalPauseDetector.hpp"
#include "GameData.hpp"
#include <crt_Logger.h>
#include <crt_Handler.h>
#include "crt_Button.h"
#include "HTTP_client.hpp"
#include "HTTP_server.hpp"
#include "HTTP_WiFi.hpp"


namespace crt{

    const unsigned int pinButtonDump = 34;
    Logger<1000> messageLogger("messageLogger", 3, ARDUINO_RUNNING_CORE, pinButtonDump);
    ILogger& logger = messageLogger;

    MainInits mainInits;
    HTTP_Server HTTP_server("HTTP_Server", 2, 10000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}