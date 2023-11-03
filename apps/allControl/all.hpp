#pragma once

namespace crt
{
	class all : public Task{
        public:
        all(display& display1, speaker& speaker1, const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            display1(display1),
            speaker1(speaker1)
        {
            start();
        }

        private:
        display& display1;
        speaker& speaker1;

        void main(){
            display1.startUp();
            speaker1.startUp();

            for(;;){
                vTaskDelay(1);
            }
        }
    };
}