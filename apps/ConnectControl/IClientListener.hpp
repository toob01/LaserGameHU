#pragma once

namespace crt{

class IClientListener{
public:
    virtual void startGame()=0;
    virtual void connectSucces()=0;
};

} // namespace crt