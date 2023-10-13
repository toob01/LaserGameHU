#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 
namespace crt
{
    class bullets : public Task{
        public:
        bullets(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            lcd(0x27, lcdColumns, lcdRows)
        {
            start(); // For simplicity, the task is started right away in it's constructor.
        }

        private:
        int lcdColumns = 16;
        int lcdRows = 1;
        LiquidCrystal_I2C lcd;

        void main(){
            // initialize LCD
            lcd.init();
            // turn on LCD backlight                      
            lcd.backlight();
            // set cursor to first column, first row
            lcd.setCursor(0, 0);
            // print message
            lcd.print("Hello, World!");
            for(;;){ 
                vTaskDelay(1);
            }
        }
    };
} // namespace crt
