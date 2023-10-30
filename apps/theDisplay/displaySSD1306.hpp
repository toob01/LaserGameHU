#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class display : public Task{
        public:
        int bulletCount = 70;
        int lives = 3;
        int score = 0;

        display(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
        {
            start(); // For simplicity, the task is started right away in it's constructor.
        }

        private:
        const uint8_t SCREEN_WIDTH = 128; // OLED width,  in pixels
        const uint8_t SCREEN_HEIGHT = 64; // OLED height, in pixels
        Adafruit_SSD1306 oled;

        void drawDisplay(){
            oled.clearDisplay(); // clear display
            oled.setTextSize(2);         // set text size
            oled.setTextColor(WHITE);    // set text color
            oled.setCursor(0, 10);       // set position to display
            oled.print("bullets:");
            oled.println(bulletCount);   // set text	
            oled.print("lives:  ");
            oled.println(lives);
            oled.print("score:  ");
            oled.println(score);
            oled.display();              // display on OLED
            return;
        }

        void bulletShot(){
            bulletCount--;
            return;
        }

        void bulletReload(){
            bulletCount = 70;
            return;
        }

        void lifeLost(){
            lives--;
            return;
        }

        void scoreIncrease(){
            score++;
            return;
        }
        
        void main() {
            Serial.begin(9600);

            // initialize OLED display with I2C address 0x3C
            if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
                Serial.println(F("failed to start SSD1306 OLED"));
                while (1);
            }

            delay(2000);
            for(int i = 0; i < 10; i++){
                drawDisplay();
                bulletShot();
                delay(200);
            }
            bulletReload();
            lifeLost();
            scoreIncrease();
            drawDisplay();

            for(;;){
                vTaskDelay(1);
            }            // display on OLED
        }
    };
} // namespace crt

