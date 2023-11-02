#pragma once

namespace crt {

class ID_ButtonListener {
    public:
        virtual void buttonPressed(int buttonID)=0;
        virtual void buttonReleased()=0;
    };

};