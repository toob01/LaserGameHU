#pragma once
#include <crt_CleanRTOS.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace crt
{
    class DisplayControl : public Task{
        private:
        int bulletCount;
        int lives;
        int timer;
        Flag startUpFlag;
        Flag gameOverFlag;
        Flag drawDisplayFlag;
        Flag reloadFlag;
        const uint8_t SCREEN_WIDTH = 128;
        const uint8_t SCREEN_HEIGHT = 64;
        Adafruit_SSD1306 oled;

        enum state_DisplayControl_t {idle, startUp, drawDisplay, reload, gameOver};
		state_DisplayControl_t state_DisplayControl = state_DisplayControl_t::idle;

        public:
        DisplayControl(const char *taskName, unsigned int taskPriority, unsigned int taskSizeBytes, unsigned int taskCoreNumber) :
            Task(taskName, taskPriority, taskSizeBytes, taskCoreNumber),
            startUpFlag(this), gameOverFlag(this), drawDisplayFlag(this), reloadFlag(this),
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

        void reloadSet(){
            reloadFlag.set();
        }
        
        void main() {
            Serial.begin(9600);

            if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
                Serial.println(("failed to start SSD1306 OLED"));
                while (1);
            }
            // oled.setTextSize(2);
            // oled.setTextColor(WHITE);
            // oled.setCursor(0, 10);
            // oled.clearDisplay();
            vTaskDelay(2000);

            for(;;){
                switch(state_DisplayControl){
					case state_DisplayControl_t::idle:
                        waitAny(startUpFlag + drawDisplayFlag + reloadFlag + gameOverFlag);
						if(hasFired(startUpFlag)){
							state_DisplayControl = state_DisplayControl_t::startUp;
							break;
                        }else if(hasFired(drawDisplayFlag)){
                            state_DisplayControl = state_DisplayControl_t::drawDisplay;
                            break;
						}else if(hasFired(reloadFlag)){
                            state_DisplayControl = state_DisplayControl_t::reload;
                            break;
                        }else if(hasFired(gameOverFlag)){
							state_DisplayControl = state_DisplayControl_t::gameOver;
							break;
						}else{break;}
                    case state_DisplayControl_t::startUp:
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
                        state_DisplayControl = state_DisplayControl_t::idle;
						break;
					case state_DisplayControl_t::gameOver:
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
                        state_DisplayControl = state_DisplayControl_t::idle;
						break;
                    case state_DisplayControl_t::drawDisplay:
                        oled.setTextSize(2);
                        oled.setTextColor(WHITE);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.print("bullets:");
                        oled.println(bulletCount);
                        oled.print("lives: ");
                        oled.println(lives);
                        oled.print("time:   ");
                        oled.println(timer);
                        oled.display();
                        state_DisplayControl = state_DisplayControl_t::idle;
                        break;
                    case state_DisplayControl_t::reload:
                        oled.setTextSize(2);
                        oled.setTextColor(WHITE);
                        oled.setCursor(0, 10);
                        oled.clearDisplay();
                        oled.println("Reloading...");
                        oled.display(); 
                        vTaskDelay(2000);
                        state_DisplayControl = state_DisplayControl_t::idle;
                        break;
					default:
						break;
                }
            }
        }
    };
}
