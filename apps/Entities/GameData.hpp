#ifndef POSTGAMEDATA_HPP
#define POSTGAMEDATA_HPP

#include <array>

struct HitArray {
    std::array<Hit, 50> hitArray;
};

struct Hit {
private:
    int gameTime;
    uint8_t playerNum;
public:
    Hit(int gametime, uint8_t playerNum):
    gameTime(gametime), playerNum(playerNum)
    {}
};

struct GameData_t {
private:
    uint8_t playerNum;
    uint8_t teamNum;
    int gameTime;
    int health;
    int shotsTaken;
    int maxAmmo;
    int weaponDamage;
    int reloadTime;
    HitArray hit;
    int counter = 0;
public:
    void setPlayerNum(uint8_t x){
        playerNum = x;
    }

    void setTeamNum(uint8_t x){
        teamNum = x;
    }

    void addHit(int timestamp, int pN){
        hit.hitArray[counter] = Hit(timestamp, pN);
        counter ++;
    }

    void setHealth(int x){
        health = x;
    }

    void setGameTime(int x){
        gameTime = x;
    }

    void setShotsTaken(int x){
        shotsTaken = x;
    }

    void setMaxAmmo(int x){
        maxAmmo = x;
    }

    void setReloadTime(int x){
        reloadTime = x;
    }

    void setWeaponDamage(int x){
        weaponDamage = x;
    }

    HitArray getHits(){
        return hit;
    }

    int getGameTime(){
        return gameTime;
    }

    int getHealth(){
        return health;
    }

    int getShotsTaken(){
        return shotsTaken;
    }

    uint8_t getPlayerNum(){
        return playerNum;
    }

    uint8_t getTeamNum(){
        return teamNum;
    }

    int getMaxAmmo(){
        return maxAmmo;
    }

    int getWeaponDamage(){
        return weaponDamage;
    }

    int getReloadTime(){
        return reloadTime;
    }

};


#endif
