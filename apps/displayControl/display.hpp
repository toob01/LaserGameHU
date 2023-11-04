#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class display : public Task{
        private:
        int bulletCount = 70;
        int lives = 99;
        int timer = 60;
        Flag startUpFlag;
        Flag gameOverFlag;
        const uint8_t SCREEN_WIDTH = 128;
        const uint8_t SCREEN_HEIGHT = 64;
        Adafruit_SSD1306 oled;

        public:
        display(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            startUpFlag(this), gameOverFlag(this), oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
        {
            start();
        }        
        
        void bulletShot(){
            bulletCount--;
            return;
        }

        void bulletReload(){
            bulletCount = 70;
            vTaskDelay(1000);
            return;
        }

        void lifeLost(){
            lives--;
            return;
        }

        void timerDecrease(){
            timer--;
            return;
        }

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
            startUpFlag.set();
        }

        void gameOver(){
            gameOverFlag.set();
        }
        
        void main() {
            Serial.begin(9600);

            if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
                Serial.println(F("failed to start SSD1306 OLED"));
                while (1);
            }
            vTaskDelay(2000);

            for(;;){
                vTaskDelay(1);
                waitAny(startUpFlag + gameOverFlag);
				if(hasFired(startUpFlag)){
					event = startUpFlag;
				}else if(hasFired(gameOverFlag)){
					event = gameOverFlag;
				}
				switch(event){
                    case startUpFlag:
						oled.setTextSize(2);
                        oled.setTextColor(WHITE);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.println("Welcome to");
                        oled.println("Mislukke-");
                        oled.println("Laser");
                        oled.display(); 
                        vTaskDelay(2200);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.println("Connecting...");
                        oled.display(); 
                        vTaskDelay(2000);
						break;
					case gameOverFlag:
						oled.setTextSize(2);
                        oled.setTextColor(WHITE);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.println("Game Over...");
                        oled.display(); 
                        vTaskDelay(2000);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.println("Return to host...");
                        oled.display(); 
                        vTaskDelay(2000);
						break;
					default:
						break;
                }
            }
        }
    };
}
