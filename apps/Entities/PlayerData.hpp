#pragma once
#include <array>

struct indPlayerData_t {
    const char* ip;
    int playerNum;  //max 32 / 5bit
    int teamNum;    //max 8 / 3bit
    int lives;      //default 100
    bool bGameOver;        //default false
    bool bReady;           //default false

    indPlayerData_t(const char* ip, int playerNum, int teamNum, int lives=100, bool bGameOver=false, bool bReady=false):
    ip(ip), playerNum(playerNum), teamNum(teamNum), lives(lives), bGameOver(bGameOver), bReady(bReady)
    {}
};

struct PlayerData_t{
private:
    indPlayerData_t playerData[32];
public:
    PlayerData_t(){}

    void addPlayer(const char* i, int pN, int tN, bool bGO, bool bR){
        playerData[pN] = indPlayerData_t(i, pN, tN, bGO, bR);
    }

    std::array<int,32> getReadyPlayers(){
        int elements = 0;
        std::array<int, 32> readyPlayers;
        for(indPlayerData_t i : playerData){
            if(i.bReady == true){
                readyPlayers[elements]= i.playerNum;
                elements++;
            }
        }
        return readyPlayers;
    }

    void setPlayerReady(int x){
        // for(int i : PlayerData){
        //     if(i.playerNum == x){
        //        i.bReady = true; 
        //     }
        // }
        playerData[x].bReady = true;
    }

    void setPlayerState(int x, bool b){
        // for(int i : PlayerData){
        //     if(i.playerNum == x){
        //        i.bGameOver = b; 
        //     }
        // }
        playerData[x].bGameOver = b;
    }
};
