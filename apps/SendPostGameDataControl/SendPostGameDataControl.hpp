#pragma once
#include <CleanRTOS.h>


namespace crt {
    class SendPostGameDataControl : public Task {
    private:
        GameData_t& GameData
        Flag startFlag;
        Flag readyForDataFlag;

    public:

    SendPostGameDataControl(GameData_t& GameData):
        GameData(GameData), startFlag(this), readyForDataFlag(this)
    {}

    void _start(){
        startFlag.set();
    }

    void readyForData(){
        readyForDataFlag();
    }

    void main() {
        for(;;){
            wait(startFlag);
            wait(readyForDataFlag);
            //get&send GameData
            //als het kan als struct: in zn geheel
            //zo niet, bereken ook al info van de hits.

            
        }
    }


    }
}