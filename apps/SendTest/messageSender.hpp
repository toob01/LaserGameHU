#pragma once
#include <crt_CleanRTOS.h>
#include "ClockPin.h"
#include "message.h"

namespace crt{

extern ILogger& logger;

class MessageSender : public Task{
private:
    ClockPin sendPin;
    Timer dataTimer;
    Queue<Message, 20> messageChannel;

    enum state_messageSender_t {Idle, waitingForMessage, sendStart, readNextBit, sendEnd, sendZero, sendOne};
	state_messageSender_t state_messageSender = state_messageSender_t::waitingForMessage;

    void rotateMessage(Message& message){
        uint64_t msg = 0;
        uint64_t mloc=message.msg;
        int n = message.nofBytes*8;
        if(n>0){
            msg |= (mloc&1);
            for (int i=0; i<n-1; i++){
                msg <<= 1;
                mloc >>= 1;
                msg |= (mloc&1);
            }
        }
        message.msg = msg;
    }

    Message constructShootMessage(uint8_t damage, uint8_t playerNum, uint8_t teamNum=0){
        if(playerNum > 32){
            ESP_LOGE("MessageSender", "PlayerNum exceeds 5-bit max of 31");
        } else if(damage > 127){
            ESP_LOGE("MessageSender", "damage exceeds 7-bit max of 127");
        } else if(teamNum > 7){
            ESP_LOGE("MessaegSender", "teamNum exceeds 3-bit max of 7");
        }
        uint64_t message = 0xAA;// start with 10101010
        message <<= 8;
        message |= damage;
        message <<= 5;
        message |= playerNum;
        message <<= 3;
        message |= teamNum;
        uint8_t checksum = generateChecksum(message, 3);
        message <<= 8;
        message |= checksum;
        Message shootMessage(message, 4);
        ESP_LOGI("MessageSender", "Created message with msg : %llu, nbytes : %d", shootMessage.msg, shootMessage.nofBytes);
        // rotateMessage(shootMessage);
        // ESP_LOGI("MessageSender", "Rotated message: %llu", shootMessage.msg);
        return shootMessage;
    }

    uint8_t generateChecksum(uint64_t message, int nofBytes){
        uint8_t result = 0;
        for (int byte = 0; byte<=nofBytes*8; byte+=8){
            uint8_t xorByte = (message >> byte)&0xFF;
            result ^= xorByte;
        }
        return result;
    }

public:
    MessageSender(const char* taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber,
    int32_t LEDpin, bool gpio_invert=false):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), sendPin(LEDpin, 38000, gpio_invert),
    dataTimer(this), messageChannel(this){
        start();
        ESP_LOGI("MessageSender", "Start MessageSender");
        gpio_pad_select_gpio(27);
        gpio_set_direction((gpio_num_t)27, GPIO_MODE_OUTPUT);
    }

    void sendMessage(uint64_t msg, int nofBytes){
        Message message(msg, nofBytes);
        messageChannel.write(message);
    }

    void sendMessage(Message message){
        messageChannel.write(message);
    }

    void sendShoot(uint8_t damage, uint8_t playerNum, uint8_t teamNum=0){
        gpio_set_level(gpio_num_t(27), 1);
        Message msg = constructShootMessage(damage, playerNum, teamNum);
        messageChannel.write(msg);
    }

    void doThing(){
        sendPin.enable();
    }

    void stopThing(){
        sendPin.disable();
    }

    void main(){
        Message msg;
        int bitsToSend = 0;
        bool sendBit;
        for(;;){
            switch(state_messageSender){
                case state_messageSender_t::Idle:
                    sendPin.disable();
                    break;
                case state_messageSender_t::waitingForMessage:
                    sendPin.disable();
                    wait(messageChannel);
                    messageChannel.read(msg);
                    ESP_LOGI("MessageSender", "Message: %llu", msg.msg);
                    if(msg.nofBytes != 0){
                        bitsToSend = 8 * msg.nofBytes;
                        ESP_LOGI("MessageSender", "Bits to send: %d", bitsToSend);
                        state_messageSender = state_messageSender_t::sendStart;
                    }
                    break;
                case state_messageSender_t::sendStart:
                    sendPin.enable();
                    dataTimer.sleep_us(9000);
                    sendPin.disable();
                    dataTimer.sleep_us(4500);
                    state_messageSender = state_messageSender_t::readNextBit;
                    break;
                case state_messageSender_t::readNextBit:
                    if(bitsToSend == 0){
                        state_messageSender = state_messageSender_t::sendEnd;
                        break;
                    }
                    sendBit = msg.msg & 1;
                    if(sendBit){
                        state_messageSender = state_messageSender_t::sendOne;
                        break;
                    } else {
                        state_messageSender = state_messageSender_t::sendZero;
                        break;
                    }
                    break;
                case state_messageSender_t::sendEnd:
                    sendPin.enable();
                    dataTimer.sleep_us(560);
                    sendPin.disable();
                    dataTimer.sleep_us(6000);
                    state_messageSender = state_messageSender_t::waitingForMessage;
                    gpio_set_level(gpio_num_t(27), 0);
                    break;
                case state_messageSender_t::sendZero:
                    sendPin.enable();
                    dataTimer.sleep_us(560);
                    sendPin.disable();
                    dataTimer.sleep_us(665);
                    bitsToSend -= 1;
                    msg.msg >>= 1;
                    state_messageSender = state_messageSender_t::readNextBit;
                    break;
                case state_messageSender_t::sendOne:
                    sendPin.enable();
                    dataTimer.sleep_us(560);
                    sendPin.disable();
                    dataTimer.sleep_us(1690);
                    bitsToSend -= 1;
                    msg.msg >>= 1;
                    state_messageSender = state_messageSender_t::readNextBit;
                    break;
                default:
                    assert(false);
            }
        }
    }
};
}