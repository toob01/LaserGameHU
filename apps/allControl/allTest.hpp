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

namespace crt{
    MainInits mainInits;
    GameData_t globalGameData;

    DisplayControl displayControl("display1", 3, 4000, ARDUINO_RUNNING_CORE);
    SpeakerControl speakerControl("speaker1", 3, 4000, ARDUINO_RUNNING_CORE);

    Handler<10 /*MAXLISTENERCOUNT*/> buttonHandler("ButtonHandler", 2 /*priority*/, ARDUINO_RUNNING_CORE, 70 /*periodMs*/, 3000 /*batchTimeUs*/); // Don't forget to call its start() member during setup().
	Button<2> reloadButton("R" /*name*/, 13 /*pin*/, true /*positive logic*/, buttonHandler);
    Button<2> triggerButton("T" /*name*/, 15 /*pin*/, true /*positive logic*/, buttonHandler);

    MessageReceiver messageReceiver("MessageReceiver", 1 /*priority*/, 2000 /*stackBytes*/, ARDUINO_RUNNING_CORE);
    NecReceiver necReceiver("NECReceiver", 1, 3000, ARDUINO_RUNNING_CORE, messageReceiver);
    SignalPauseDetector signalPauseDetector("SignalPauseDetector", 1, 3000, ARDUINO_RUNNING_CORE, necReceiver, 32);
    MessageSender messageSender("messageSender", 1, 4000, ARDUINO_RUNNING_CORE, 19, false);

    GameStateControl gameStateControl("GameStateControl", 2, 3000, ARDUINO_RUNNING_CORE, globalGameData, displayControl);

    ReadyUpControl readyUpControl(triggerButton, reloadButton, "ReadyUpControl", 2, 4000, ARDUINO_RUNNING_CORE, gameStateControl, globalGameData);

    ReceivingHitControl receivingHitControl("ReceivingHitControl", 1, 4000, ARDUINO_RUNNING_CORE, globalGameData, speakerControl, displayControl, gameStateControl);

    ShootingControl shootingControl(triggerButton, reloadButton, "ShootingControl", 1, 3500, ARDUINO_RUNNING_CORE, messageSender, speakerControl, globalGameData, displayControl);

    GameSetupControl gameSetupControl(displayControl, readyUpControl, globalGameData, "GameSetupControl", 2, 3000, ARDUINO_RUNNING_CORE);

    ConnectControl connectControl("ConnectControl", 2, 5000, ARDUINO_RUNNING_CORE, gameSetupControl, readyUpControl, gameStateControl, globalGameData);

    SendPostGameDataControl sendPostGameDataControl(globalGameData, connectControl, "SendPostGameDataControl", 2, 2000, ARDUINO_RUNNING_CORE);

    GameOverControl gameOverControl(globalGameData, speakerControl, receivingHitControl, shootingControl, displayControl, sendPostGameDataControl, connectControl, "GameOverControl", 2, 4000, ARDUINO_RUNNING_CORE);
}

void setup() {
}

void loop() {
    vTaskDelay(1);
}