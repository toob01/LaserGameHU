#ifndef POSTGAMEDATA_HPP
#define POSTGAMEDATA_HPP

struct PostGameData_t {
private:
    indPostGameData_t GameData[32];
public:
    PostGameData_t(){}

    void addPlayer(int playerNum, int lives_left, int hits_on_others, int accuracy){
        GameData[playerNum] = indPostGameData_t(playerNum, lives_left, hits_on_others, accuracy);
    }

    indPostGameData_t getPlayerStats(int playerNum){
        for(indPostGameData_t player : GameData){
            if(playerNum == player.playerNum){
                return player;
            }
        }
    }
};

struct indPostGameData_t {
    int playerNum;
    int lives_left;
    int hits_on_others;
    int accuracy;      //percentage between 0-100

    indPostGameData_t(int playerNum=0, int lives_left=0, int hits_on_others=0, int accuracy=0):
    playerNum(playerNum), lives_left(lives_left), hits_on_others(hits_on_others), accuracy(accuracy)
    {}
};

#endif