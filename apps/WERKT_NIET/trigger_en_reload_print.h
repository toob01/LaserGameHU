#include <crt_CleanRTOS.h>
#include "id_button.h"

namespace crt {

    class trigger_reload_print : public Task, public ID_ButtonListener {
    private:
        int ID;
        Queue<int, 10> queueButtons;

    public:
        trigger_reload_print(const char* taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, IID_Button& trigger, IID_Button& reload):
        Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber), ID(0), queueButtons(this)
        {
            trigger.addButtonListener(this);
            reload.addButtonListener(this);
            start();
        }


        void buttonPressed(int buttonID){
            ID = buttonID;
            queueButtons.write(ID);
        }

        void buttonReleased(){
            int ID = 2;
            queueButtons.write(ID);
        }

        void main(){
            while(true){
                wait(queueButtons);
                queueButtons.read(ID);
				ESP_LOGI("trigger_reload_print","%d", ID);
                vTaskDelay(1);
            }
        }

    };
}