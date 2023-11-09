#pragma once
#include <array>

struct Hit {
    int gameTime;
    uint8_t playerNum;
    
    Hit(int gametime=0, uint8_t playerNum=0):
    gameTime(gametime), playerNum(playerNum)
    {}
};

struct HitArray {
    std::array<Hit, 50> hitArray;

    HitArray():
    hitArray(){}

    void log_contents(){
        for(Hit hit : hitArray){
            ESP_LOGI("HitArray", "Contents: time = %d, player = %d", hit.gameTime, hit.playerNum);
        }
    }
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
    int counter;
public:

    GameData_t(uint8_t playerNum=0, uint8_t teamNum=0, int gameTime=0, int health=0, int maxAmmo=0, int weaponDamage=0, int reloadTime=0):
    playerNum(playerNum), teamNum(teamNum), gameTime(gameTime), health(health), shotsTaken(0), maxAmmo(maxAmmo), 
    weaponDamage(weaponDamage), reloadTime(reloadTime), hit(), counter(0){}

    void setData(GameData_t& gameData){
        playerNum = gameData.playerNum;
        teamNum = gameData.teamNum;
        gameTime = gameData.gameTime;
        health = gameData.health;
        shotsTaken = gameData.shotsTaken;
        maxAmmo = gameData.maxAmmo;
        weaponDamage = gameData.weaponDamage;
        reloadTime = gameData.reloadTime;
        hit = gameData.hit;
        counter = gameData.counter;
    }

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