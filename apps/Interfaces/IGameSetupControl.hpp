#pragma once
#include "GameData.hpp"
#include "IGameSetupListener.hpp"

namespace crt{

class IGameSetupControl {
public:
    virtual void addGameSetupListener(IGameSetupListener* pGameSetupListener) = 0;
    virtual void _start() = 0;
    virtual void gameDataReady(GameData_t gameData)=0
}

}