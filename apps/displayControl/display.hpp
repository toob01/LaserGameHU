#pragma once
#include <crt_CleanRTOS.h>
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

        enum state_display_t {idle, startUp, gameOver};
		state_display_t state_display = state_display_t::idle;

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

        void startUpSet(){
            startUpFlag.set();
        }

        void gameOverSet(){
            gameOverFlag.set();
        }
        
        void main() {
            Serial.begin(9600);

            if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
                Serial.println(("failed to start SSD1306 OLED"));
                while (1);
            }
            vTaskDelay(2000);

            for(;;){
                vTaskDelay(1);
                switch(state_display){
					case state_display_t::idle:
                        waitAny(startUpFlag + gameOverFlag);
						if(hasFired(startUpFlag)){
							state_display = state_display_t::startUp;
							break;
						}else if(hasFired(gameOverFlag)){
							state_display = state_display_t::gameOver;
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
					default:
						break;
                }
            }
        }
    };
}
