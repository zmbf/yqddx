//
//  BallFactory.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//

#include "BallFactory.h"
std::vector<std::vector<BallOrdinary*>*>* BallFactory::m_BallOrdinaryMap = new(std::nothrow) std::vector<std::vector<BallOrdinary*>*>(BALLTYPECOUNT);
void BallFactory::initBallOrdinary(const int & count,const int & type){
    auto list = m_BallOrdinaryMap->at(type);//获取 这个type 的vector
    if(!list){
        list = new(std::nothrow) std::vector<BallOrdinary*>();
        list->reserve(MAXBALLCOUNT);
        m_BallOrdinaryMap->at(type) = list;
    }
    for(int i =0;i<count;i++){
        auto ball = BallOrdinary::create(type);
        ball->retain();
        ball->pause();
        ball->getPhysicsBody()->setEnabled(false);
        list->push_back(ball);
    }
}
BallOrdinary* BallFactory::getBallOrdinary(const int & type){
    auto list = m_BallOrdinaryMap->at(type);//获取 这个type 的vector
    if(!list||list->empty()){
        BallFactory::initBallOrdinary(1,type);
    }
    auto ball = list->back();
    list->pop_back();
    
    ball->setVisible(true);
    ball->m_isInUse = true;
    ball->resume();
    ball->getPhysicsBody()->setEnabled(true);
    
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
   
    ball->pause();
    ball->getPhysicsBody()->setEnabled(false);//关闭物理特性
    auto list = m_BallOrdinaryMap->at(type);
    list->push_back(ball);
}
