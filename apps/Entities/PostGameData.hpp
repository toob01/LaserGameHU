#ifndef POSTGAMEDATA_HPP
#define POSTGAMEDATA_HPP

#include <array>

struct PostGameData_t {
private:
    indPostGameData_t GameData[32];
public:
    PostGameData_t(){}

    void addPlayer(const char* i, int pN, int tN, bool bGO, bool bR){
        GameData[pN] = indPostGameData_t(i, pN, tN, bGO, bR);
    }
};

struct indPostGameData_t {
private:
    int playerNum; 
    int lives_left;
    int hits_on_others;
    int accuracy;      //percentage between 0-100

public:
    indPostGameData_t(int playerNum, int lives_left, int hits_on_others, int accuracy):
    playerNum(playerNum), lives_left(lives_left), hits_on_others(hits_on_others), accuracy(accuracy)
    {}
};

#endif