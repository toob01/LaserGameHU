#ifndef POSTGAMEDATA_HPP
#define POSTGAMEDATA_HPP

#include <array>

struct HitArray {
private:
    std::array<Hit, 50> hitArray;
public:
    HitArray(){}
};

struct Hit {
private:
    int gameTime;
    uint8_t playerNum;
public:
    Hit(){}
};

struct GameData_t {
private:
    uint8_t playerNum;
    uint8_t teamNum;
    int gameTime;
    int lives;
    int shotsTaken;
    HitArray hit;
public:
    GameData_t(){}

    void setPlayerNum(uint8_t x){
        playerNum = x;
    }

    void setTeamNum(uint8_t x){
        teamNum = x;
    }

    void addHit(int timestamp, int playerNum){

    }

    void setlives(int x){
        lives = x;
    }

    void setGameTime(int x){
        gameTime = x;
    }

    void setShotsTaken(int x){
        shotsTaken = x;
    }

    HitArray getHits(){
        return hit;
    }

    int getGameTime(){
        return gameTime;
    }

    int getlives(){
        return lives;
    }

    int getShotsTaken(){
        return shotsTaken;
    }

    uint8_t getPlayerNum(){
        return playerNum;
    }

    uint8_t geteamNum(){
        return teamNum;
    }

};


#endif