#ifndef GameData_h
#define GameData_h
#include "cocos2d.h"

//tileMap y轴位置的偏移
#define TMX_OFFSET 140.0f
//球的刚体半径
#define BALL_DATA_PHYSICS_BODY_RADIUS 80.0f

#define clickSound "clickSound"
#define pauseSound "clickSound"

struct CKF_GameData
{
    int GameTime;
    int BallScore;
    int HitScore;
    int ThemeID;
    std::string ThemeName;
    std::string TiledName;
    std::string GeneralName;
    int GeneralID;
    
    int StarOne;
    int StarTwo;
    int StarThree;
    
    int Need_Star;
    int Need_Red;
    int Need_Yellow;
    int Need_Blue;
    int Need_Green;
    int Need_Purple;
    int Need_Ice;
    int Need_Debris;
    int bossBlood;
};
struct Ice_Need
{
    cocos2d::Vec2 location;
    int life;
};
#endif
