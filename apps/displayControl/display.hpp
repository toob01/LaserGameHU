#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class display : public Task{
        public:

        display(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber, int bulletCount, int lives, int timer) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1),
            bulletCount(bulletCount),
            lives(lives),
            timer(timer)
        {
            start();
        }

        private:
        const uint8_t SCREEN_WIDTH = 128;
        const uint8_t SCREEN_HEIGHT = 64;
        Adafruit_SSD1306 oled;

        void drawDisplay(){
            oled.setCursor(0, 10);
            oled.clearDisplay();
            oled.print("bullets:");
            oled.println(bulletCount);
            oled.print("lives:  ");
            oled.println(lives);
            oled.print("time:   ");
            oled.println(timer);
            oled.display();
            return;
        }

         void startUp(){
            oled.setTextSize(2);
            oled.setTextColor(WHITE);
            oled.setCursor(0, 10);
            oled.clearDisplay();
            oled.println("Welcome to");
            oled.println("Mislukke-");
            oled.println("Laser");
            oled.display(); 
            delay(2000);
            oled.setCursor(0, 10);
            oled.clearDisplay();
            oled.println("Connecting...");
            oled.display(); 
            delay(2000);
            return;
        }
        
        void main() {
            Serial.begin(9600);

            if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
                Serial.println(F("failed to start SSD1306 OLED"));
                while (1);
            }

            delay(2000);
            startUp();
            for(int i = 0; i < 10; i++){
                drawDisplay();
                bulletShot();
                delay(200);
            }
            delay(1000);
            bulletReload();
            drawDisplay();
            delay(1000);
            lifeLost();
            drawDisplay();
            delay(1000);
            for(int i = 0; i < 60; i++){
                timerDecrease();
                drawDisplay();
                delay(1000);
            }

            for(;;){
                vTaskDelay(1);
            }
        }
    };
}
