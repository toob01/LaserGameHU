#pragma once
#include <crt_CleanRTOS.h>
#include "message.h"
#include "GameData.hpp" // ToDo: update to actual entity header
#include "recievingHitControl.hpp"

namespace crt{

extern ILogger& logger;

class MessageReceiver : public Task{
private:

    struct ShootMessage_t{
        uint8_t damage;
        uint8_t playerNum;
        uint8_t teamNum;

        ShootMessage_t(uint8_t damage=0, uint8_t playerNum=0, uint8_t teamNum=0):
        damage(damage), playerNum(playerNum), teamNum(teamNum){}
    };

    uint8_t generateChecksum(uint64_t& message, int nofBytes){
        uint8_t result = 0;
        for (int byte = 0; byte<=nofBytes*8; byte+=8){
            uint8_t xorByte = (message >> byte)&0xFF;
            result ^= xorByte;
        }
        return result;
    }

    void log_bytes_from_uint64(uint64_t& msg, int nofBytes){
        uint8_t byte;
        logger.logText("Message Received:");
        for(int i=0; i<nofBytes; i++){
            byte = (msg >> (i*8))& 0xFF;
            logger.logUint32(byte);
        }
        logger.logText("Message End.");
    }

    bool verifyChecksum(Message& msg){
        uint8_t givenChecksum = msg.getByte(msg.nofBytes);
        uint64_t msgNoChecksum = (msg.msg >> 8) & 0x00FFFFFFFFFFFFFF; // ensure first 8 bits aren't being 1-extended
        uint8_t generatedChecksum = generateChecksum(msgNoChecksum, msg.nofBytes-1);
        return givenChecksum == generatedChecksum;
    }

    ShootMessage_t decodeShoot(Message& msg){
        uint8_t damage = msg.getByte(2);
        uint8_t playerNum = (msg.getByte(3) & 0x1f) >> 5;
        uint8_t teamNum = (msg.getByte(3) & 0x7) >> 3;
        ShootMessage_t shootMessage(damage, playerNum, teamNum);
        return shootMessage;
    }

    enum state_messageReceiver_t {Idle, waitingForMessage, verifyChecksum, identifyMessage, isShootMessage, isUnknownMessage, hitReceived};
    state_messageReceiver_t state_messageReceiver = state_messageReceiver_t::waitingForMessage;
    Queue<Message, 10> messageChannel;

    GameData_t& GameData;
    ReceivingHitControl& receivingHitControl;

public:
    MessageReceiver(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    GameData_t& GameData, ReceivingHitControl& receivingHitControl):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), messageChannel(this),
    GameData(GameData), receivingHitControl(receivingHitControl){
        start();
        ESP_LOGI("MessageReceiver", "Start MessageReceiver");
    }

    void messageReceived(uint64_t& msg, int& nofBytes){
        if(nofBytes != 0){
            log_bytes_from_uint64(msg, nofBytes);
            ESP_LOGI("MessageReceiver", "Message Received: %d bytes", nofBytes);
            ESP_LOGI("MessageReceiver", "%llu", msg);
            ESP_LOGI("MessageReceiver", "End of Message.");
        }
        Message received(msg, nofBytes);
        messageChannel.write(received);
    }

    void main(){
        Message msg;
        ShootMessage_t shoot;
        for(;;){
            switch(state_messageReceiver){
                case state_messageReceiver_t::waitingForMessage :
                    messageChannel.read(msg);
                    state_messageReceiver = state_messageReceiver_t::verifyChecksum;
                    break;
                case state_messageReceiver_t::verifyChecksum :
                    if(verifyChecksum(msg)){
                        state_messageReceiver = state_messageReceiver_t::identifyMessage;
                        break;
                    } else {
                        state_messageReceiver = state_messageReceiver_t::waitingForMessage;
                        break;
                    }
                    break;
                case state_messageReceiver_t::identifyMessage :
                    if(msg.getByte(1) == 0b10101010 && msg.getByte(2)&0x80 == 0b0){
                        state_messageReceiver = state_messageReceiver_t::isShootMessage;
                        break;
                    } else {
                        state_messageReceiver = state_messageReceiver_t::isUnknownMessage;
                        break;
                    }
                    break;
                case state_messageReceiver_t::isShootMessage :
                    shoot = decodeShoot(msg);
                    if(shoot.playerNum != GameData.getPlayerNum() && shoot.teamNum != GameData.getTeamNum()){
                        state_messageReceiver = state_messageReceiver_t::hitReceived;
                        break;
                    } else {
                        state_messageReceiver = state_messageReceiver_t::waitingForMessage;
                        break;
                    }
                    break;
                case state_messageReceiver_t::isUnknownMessage :
                    state_messageReceiver = state_messageReceiver_t::waitingForMessage;
                    break;
                case state_messageReceiver_t::hitReceived :
                    receivingHitControl.hitReceived(shoot.damage, shoot.playerNum, shoot.teamNum);
                    break;
            }
        }
    }
};
}