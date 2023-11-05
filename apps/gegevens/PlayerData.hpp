#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

#include <array>


struct PlayerData_t{
private:
    std::array<indPlayerData_t, 32> playerData;
public:
    PlayerData_t(){}

    void addPlayer(const char* i, int pN, int tN, bool bGO, bool bR){
        playerData[pN] = indPlayerData_t(i, pN, tN, bGO, bR);
    }

    std::array<int,32> getReadyPlayers(){
        std::array<int, 32> readyPlayers;
        for(int i : PlayerData){
            if(i.bReady == true){
                readyPlayers.add(i.playerNum);
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

struct indPlayerData_t {
private:
    const char* ip;
    static int playerNum;  //max 32 / 5bit
    static int teamNum;    //max 8 / 3bit
    static int lives;      //default 100
    bool bGameOver;        //default false
    bool bReady;           //default false

public:
    indPlayerData_t(const char* ip, int playerNum, int teamNum, int lives=100, bool bGameOver=false, bool bReady=false):
    ip(ip), playerNum(playerNum), teamNum(teamNum), lives(lives), bGameOver(bGameOver), bReady(bReady)
    {}
};

#endif