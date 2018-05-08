//
//  BallFactory.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//

#include "BallFactory.h"
std::unordered_map<int,std::vector<BallOrdinary*>*>* BallFactory::m_BallOrdinaryMap = new(std::nothrow) std::unordered_map<int,std::vector<BallOrdinary*>*>();
BallOrdinary* BallFactory::getBallOrdinary(const int & type){
    BallOrdinary* ball = nullptr;
    auto itr = m_BallOrdinaryMap->find(type);//获取 这个type 的vector
    if(itr == m_BallOrdinaryMap->end()){
        ball = BallOrdinary::create(type);
        ball->retain();
    }else{
        auto ballVec = (*itr).second;
        if(ballVec->empty()){  //vector 为空 创建新球
            ball = BallOrdinary::create(type);
            ball->retain();
        }else{                   //取球
            ball = ballVec->back();
            ballVec->pop_back();
        }
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
    auto itr = m_BallOrdinaryMap->find(type);
    if(itr == m_BallOrdinaryMap->end()){    //没有这个type 的vector
        auto ballVec =new(std::nothrow) std::vector<BallOrdinary*>();
        ballVec->reserve(40);//初始一个大小
        m_BallOrdinaryMap->insert(std::pair<int,std::vector<BallOrdinary*>*>(type,ballVec));
        ballVec->push_back(ball);
    }else{
        (*itr).second->push_back(ball);
    }
    ball->m_isInUse = false;
    ball->setPosition(cocos2d::Vec2(-1000,-1000));
    ball->removeFromParent();
}
