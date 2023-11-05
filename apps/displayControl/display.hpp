#pragma once
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class display : public Task{
        private:
        int bulletCount;
        int lives;
        int timer;
        Flag startUpFlag;
        Flag gameOverFlag;
        Flag drawDisplayFlag;
        const uint8_t SCREEN_WIDTH = 128;
        const uint8_t SCREEN_HEIGHT = 64;
        Adafruit_SSD1306 oled;

        enum state_display_t {idle, startUp, drawDisplay, gameOver};
		state_display_t state_display = state_display_t::idle;

        public:
        display(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            startUpFlag(this), gameOverFlag(this), drawDisplayFlag(this), 
            oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1)
        {
            start();
        }   

        void setBulletCount(int count){
            bulletCount = count;
        }

        void setLives(int count){
            lives = count;
        }

        void setTimer(int count){
            timer = count;
        }

        void drawDisplaySet(){
            drawDisplayFlag.set();
        }

        void startUpSet(){
            startUpFlag.set();
        }

        void gameOverSet(){
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
                switch(state_display){
					case state_display_t::idle:
                        waitAny(startUpFlag + drawDisplayFlag + gameOverFlag);
						if(hasFired(startUpFlag)){
							state_display = state_display_t::startUp;
							break;
						}else if(hasFired(gameOverFlag)){
							state_display = state_display_t::gameOver;
							break;
                        }else if(hasFired(drawDisplayFlag)){
                            state_display = state_display_t::drawDisplay;
                            break;
						}else{break;}
                    case state_display_t::startUp:
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
                        state_display = state_display_t::idle;
						break;
					case state_display_t::gameOver:
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
                        state_display = state_display_t::idle;
						break;
                    case state_display_t::drawDisplay:
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.print("bullets:");
                        oled.println(bulletCount);
                        oled.print("lives:  ");
                        oled.println(lives);
                        oled.print("time:   ");
                        oled.println(timer);
                        oled.display();
                        state_display = state_display_t::idle;
                        break;
					default:
						break;
                }
            }
        }
    };
}
