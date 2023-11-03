#pragma once
#include <crt_CleanRTOS.h>
#include "message.h"

namespace crt{

extern ILogger& logger;

class MessageReceiver : public Task{
private:

    void log_bytes_from_uint64(uint64_t& msg, int nofBytes){
        uint8_t byte;
        logger.logText("Message Received:");
        for(int i=0; i<nofBytes; i++){
            byte = (msg >> (i*8))& 0xFF;
            logger.logUint32(byte);
        }
        logger.logText("Message End.");
    }

    Queue<Message, 10> messageChannel;

public:
    MessageReceiver(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), messageChannel(this){
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
        Message message;
        for(;;){
            vTaskDelay(1);
        }
    }
};
}