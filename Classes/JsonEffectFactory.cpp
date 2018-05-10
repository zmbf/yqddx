//
//  JsonEffect.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/10.
//
#include "JsonEffectFactory.h"
#include "BallOrdinary.h"
std::vector<EliminateAct*>* JsonEffectFactory::m_eliminateActVec = new (std::nothrow) std::vector<EliminateAct*>();
void JsonEffectFactory::initBalleliminateAct(const int & count){
    if(m_eliminateActVec->capacity()<MAXBALLCOUNT){
        m_eliminateActVec->reserve(MAXBALLCOUNT);
    }
    for(int i =0;i<count;i++){
        auto eliminateAct = new EliminateAct();
        eliminateAct->actvec = new std::vector<cocostudio::ActionObject*>();
        eliminateAct->actvec->reserve(BALLTYPECOUNT);
        eliminateAct->node = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("effect/ballEliminate/putongqiutexiao.json");
        eliminateAct->node->setAnchorPoint(cocos2d::Vec2(0.5f,0.5f));
        eliminateAct->node->retain(); //retain 一下
        for(int i = 1;i<BALLTYPECOUNT;i++){
            auto  act = cocostudio::ActionManagerEx::getInstance()->getActionByName("effect/ballEliminate/putongqiutexiao.json", cocos2d::StringUtils::format("Animation_%d",i).c_str());
            act->retain();//需要retain 一下
            eliminateAct->actvec->push_back(act);
        }
        m_eliminateActVec->push_back(eliminateAct);
        eliminateAct->node->retain();
        eliminateAct->node->setVisible(false);
     }
}
EliminateAct*  JsonEffectFactory::getBalleliminateAct(cocos2d::Node*  parentNode,const int & localZorder){
    
    if(m_eliminateActVec->empty()){
        JsonEffectFactory::initBalleliminateAct(1);
    }
    auto eliminateAct = m_eliminateActVec->back();
    m_eliminateActVec->pop_back();
    if(!eliminateAct->node->getParent()){
        parentNode->addChild(eliminateAct->node);
        eliminateAct->node->release();
    }
    eliminateAct->node->setVisible(true);
    eliminateAct->node->setLocalZOrder(localZorder);
    
    return eliminateAct;
}
void JsonEffectFactory::revertBalleliminateAct(EliminateAct*  eliminateAct){
    CCASSERT(std::find(m_eliminateActVec->begin(),m_eliminateActVec->end(),eliminateAct)==m_eliminateActVec->end(),"can't revert again");
    eliminateAct->node->setVisible(false);
    m_eliminateActVec->push_back(eliminateAct);
    cocos2d::log("%d",m_eliminateActVec->size());
}
