//
//  GameOverShowLayer.cpp
//  yqddx
//
//  Created by xdlc on 2018/5/9.
//
//

#include "GameOverShowLayer.h"
GameOverShowLayer::GameOverShowLayer(){
    m_root = nullptr;
}
void GameOverShowLayer::onEnterTransitionDidFinish(){
    if(!m_root){
        m_root = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/stepOver/bushujiangli_1.json");
        this->addChild(m_root);
    }
    auto act = cocostudio::ActionManagerEx::getInstance()->getActionByName("ui/game_ui/stepOver/bushujiangli_1.json", "Animation0");
    act->play();
    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(2.6f),cocos2d::RemoveSelf::create(),nullptr));
}
