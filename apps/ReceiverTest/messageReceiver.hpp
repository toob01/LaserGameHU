#pragma once
#include <crt_CleanRTOS.h>

namespace crt{

extern ILogger& logger;

class MessageReceiver : public Task{
private:
public:
    MessageReceiver(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber):
    Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber){
        start();
        ESP_LOGI("MessageReceiver", "Start MessageReceiver");
    }

    void messageReceived(uint64_t& msg, int& nofBytes){
        // logger.logText("Message Received:\n");
        // logger.logUint32((msg>>32)&0x00FF);
        // logger.logUint32(msg&0x00FF);
        // logger.logText("Message End.\n");
        if(nofBytes != 0){
            ESP_LOGI("MessageReceiver", "Message Received: %d bytes", nofBytes);
            ESP_LOGI("MessageReceiver", "%llu", msg);
            ESP_LOGI("MessageReceiver", "End of Message.");
        }
    }

    void main(){
        for(;;){
            vTaskDelay(1);
        }
    }
};
}