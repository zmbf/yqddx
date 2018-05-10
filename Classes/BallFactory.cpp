//
//  BallFactory.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//

#include "BallFactory.h"
std::vector<std::list<BallOrdinary*>*>* BallFactory::m_BallOrdinaryMap = new(std::nothrow) std::vector<std::list<BallOrdinary*>*>(BALLTYPECOUNT);
BallOrdinary* BallFactory::getBallOrdinary(const int & type){
    BallOrdinary* ball = nullptr;
    auto list = m_BallOrdinaryMap->at(type);//获取 这个type 的vector
    if(!list){
        list = new(std::nothrow) std::list<BallOrdinary*>();
        m_BallOrdinaryMap->at(type) = list;
    }
    if(list->empty()){
        ball = BallOrdinary::create(type);
        ball->retain();
    }else{
        ball = list->back();
        list->pop_back();
    }
    ball->setVisible(true);
    ball->m_isInUse = true;
    return ball;
}
void BallFactory::revertBall(BallOrdinary * ball){
    CCASSERT(ball->m_isInUse,"ball->m_isInUse  isNeed beUse ");//避免重复添加
    const int type = ball->getType();
    //恢复状态
    ball->setIsSelect(false);
    ball->helpAction(false);
    ball->setVisible(false);
    ball->m_isInUse = false;
    ball->setVisible(false);
    ball->setPosition(cocos2d::Vec2(10000,10000));
    ball->removeFromParent();
    auto list = m_BallOrdinaryMap->at(type);
    list->push_back(ball);
}
