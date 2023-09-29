# lasergame-template

This template is meant as a quick startup repo for students that start with a lasergame project on ESP32. 

These are the steps that should be followed:

## Install ESP-IDF

Select a fast drive (like nvme disk) with to install the ESP-IDF on (it takes about 50GB after building) (building thousands of small files, drive speed often is the bottleneck).

- Download and execute the latest offline ESP-IDF installer of version 4 (at time of writing: v4.4.5) from:
  [dl.espressif.com/dl/esp-idf/](https://dl.espressif.com/dl/esp-idf/)
  When asked, check all checkboxes for maximum support of esp32 variations.

- Pin the powershell that opens to the desktop.

## Teamleader only: create team repo for the lasergame

- Teamleader creates a new, **empty** **private** repo named lasergame on Github (without .gitignore, readme.md and whatever).

- Teamleader adds team members as collaborators.

- Teamleader clones this template to a local folder:
  git clone "https://github.com/mavehu/lasergame-template.git" lasergame

- cd lasergame

- git remote remove origin

- git remote add origin "https://github.com/teamleadergitname/lasergame.git"

- git push -u origin master

## Other team members clone the team repo

The other team members get their own clone of the team repo:

- git clone "https://github.com/teamleadergitname/lasergame.git" lasergame

## All team members import the arduino ide component

To be able to use libraries that were built for arduino ide in the ESP-IDF project, the arduino ide component for ESP32 needs to be added:

- cd lasergame

- cd components

- git clone https://github.com/espressif/arduino-esp32.git

## Test

- Open the ESP-IDF powershell

- cd to the lasergame folder that contains the team repo

- connect the ESP32 devkit to your computer

- check which new com port appeared in windows device manager or in Arduino IDE (port)

- Type (in my case its COM4)
  idf.py -p COM4 flash monitor

- After flashing, you should see "HelloWorld: Hello World!" repeatedly.

- Abort the serial monitor with CTRL+]

## Other useful ESP-IDF commands

- For building without flashing:
  idf.py build

- For cleaning up in for complete rebuild:
  idf.py fullclean

- For changing settings (careful):
  idf.py menuconfig

- For changing build target for different ESP32 types:
  idf.py set-target ESP32s2
  idf.py set-target ESP32s3

- 