//
//  BallFactory.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//

#ifndef BallFactory_hpp
#define BallFactory_hpp
#include "BallOrdinary.h"
#include "cocos2d.h"
#include <vector>
class BallFactory{
public:
    static void initBallOrdinary(const int & count,const int & type);//初始化count 个球
    static BallOrdinary* getBallOrdinary(const int & type); //取出一个球
    static void revertBall(BallOrdinary * ball);            //返回一个球
private:
    static std::vector<std::vector<BallOrdinary*>*>* m_BallOrdinaryMap; //球map
};

#endif /* BallFactory_hpp */
