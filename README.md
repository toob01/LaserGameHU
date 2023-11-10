# Assignment Laser Game 2023 with ESP32 and CleanRTOS

Welcome to the LaserGame project! This is an exciting multiplayer laser tag game implemented on the ESP32 microcontroller using the CleanRTOS wrapper library around FreeRTOS.

We went through a full development cycle consisting of:

1. Brainstorming
2. Constructing Use Cases
3. Designing Activity Diagrams
4. Creating Object Models
5. Drafting Class Diagrams
6. Developing State Transition Diagrams
7. Writing Code
8. Testing and Bugfixing
9. Party

# Our Team:

- Tobias Bosch
- Jorg van den Burg
- Felix Klaassen
- Joriam van Slooten
- Wessel van Toorn

# Developed Components

## Player

- Shooting and Reloading
- Two-way communication with IR
- Communication with Game Leader through WiFi
- Status Display
- Sound feedback / effects
- Configurable game settings

## Game Leader

- Two-way communication with Players
- Web server to configure game settings

# Installation / deployment

1. `git clone https://github.com/JoriamVS/Lasergame.git`
2. Install ESP-IDF according to : https://github.com/mavehu/lasergame-template
3. Connect ESP32
4. Use device manager to discern assigned COM port of Player
5. Run in ESP-IDF terminal `idf.py -p COMx flash monitor` for each player gun
6. Change in `HTTP.hpp` : uncomment `HTTP_Server HTTP_server("HTTP_Server", 2, 10000, ARDUINO_RUNNING_CORE);`
7. Change in `main.cpp` : include `HTTP.hpp` and uninclude `allTest.hpp`
8. Add file password.h to folder `apps/HTTP` containing:
```cpp
#pragma once
const char* ssid = <your SSID here>;
const char* password = <your password here>;
```
9. Use device manager to discern assigned COM port of Game Leader
10. Run in ESP-IDF terminal `idf.py -p COMx flash monitor`
11. Boot up Game Leader, connect to your created network on an external device
12. Open in web browser: `http://192.168.4.1/gameSettings/`
13. Configure settings, click Submit
14. Boot up players, then press trigger and reload buttons
15. Play.

# Pin-out

|Pin|Usage|
|--|--|
|D15|triggerButton|
|D13|reloadButton|
|RX2|pin 3 mp3 reader|
|TX2|pin 2 mp3 reader|
|D21|SDA display|
|D22|SCK display|
|D27|IR reciever|
|D32|IR led|
|D34|logger dump pin|
