#include "crt_CleanRTOS.h"
#include "messageSender.hpp"
namespace crt{

class testSender : public Task{
private:
    MessageSender& test;
public:
    testSender(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, MessageSender& messageSender):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
        test(messageSender){
            start();
            ESP_LOGI("testSender", "Start testSender");
        }
    
    void main(){
        vTaskDelay(1000);
        ESP_LOGI("testSender", "Sending shoot");
        test.sendShoot(18, 15, 2);
        for(;;){
            vTaskDelay(1);
        }
    }
};

}