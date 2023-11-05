#pragma once

struct gameSettings_t {
private:
    static int PplayerAmount;  //max 32 / 5bit
    static int PteamAmount;    //max 8 / 3bit
    static int Plives;         //defealt 100
    static int PgameLength;    //in seconds
    static int PweaponDamage;  //max 127 / 7bit
    static int PreloadTime;    //in seconds
    static int PmaxAmmo;
public:
    gameSettings_t(/*int PplayerAmount, int PteamAmount, int Plives, int PgameLength, int PweaponDamage, int PreloadTime*/)
    {
        Plives = 100;
    }

    void setPPlayerAmount(int x){
        PplayerAmount = x;
    }

    void setPTeamAmount(int x){
        PteamAmount = x;
    }

    void setPLives(int x){
        Plives = x;
    }

    void setPGameLength(int x){
        PgameLength = x;
    }

    void setPWeaponDamage(int x){
        PweaponDamage = x;
    }

    void setPReloadTime(int x){
        PreloadTime = x;
    }

    void setPmaxAmmo(int x){
        PmaxAmmo = x;
    }

    int getPPlayerAmount(){
        return PplayerAmount;
    }

    int getPTeamAmount(){
        return PteamAmount;
    }

    int getPLives(){
        return Plives;
    }

    int getPGameLenth(){
        return PgameLength;
    }

    int getPWeaponDamge(){
        return PweaponDamage;
    }

    int getPReload(){
        return PreloadTime;
    }

    int getPmaxAmmo(){
        return PmaxAmmo;
    }
};