//
//  BallOrdinary.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//
#include "BallOrdinary.h"
#include "cocos/physics/CCPhysicsShape.h"
#include "CKF_Sound.h"
#include "JsonEffectFactory.h"
BallOrdinary* BallOrdinary::create(const int & type)
{
    BallOrdinary * ballOrdinary = new (std::nothrow) BallOrdinary();
    if (ballOrdinary && ballOrdinary->init(type))
    {
        ballOrdinary->autorelease();
        return ballOrdinary;
    }
    CC_SAFE_DELETE(ballOrdinary);
    return nullptr;
}
bool BallOrdinary::init(const int & type){
    
    std::string fileName = cocos2d::StringUtils::format("ball%d-1.png",type);
    auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
    if(frame){
        Sprite::initWithSpriteFrame(frame);
    }else{
        Sprite::initWithFile(fileName);
    }
    auto material = cocos2d::PhysicsMaterial(1,0,0);
    auto body = cocos2d::PhysicsBody::createCircle(this->getContentSize().width*0.5 ,material);
    body->setRotationEnable(false);//关闭旋转
    this->setPhysicsBody(body);
    body->setGravityEnable(true);
    m_type = type;
    return true;
}
void BallOrdinary::setIsSelect(const bool & isSelect){
    std::string fileName;
    if(isSelect&&!isInselect){
        this->setLocalZOrder(this->getLocalZOrder()+1);
        fileName = cocos2d::StringUtils::format("ball%d-2.png",m_type);
        this->setNewFile(fileName);
    }else if(isInselect){
        this->setLocalZOrder(this->getLocalZOrder()-1);
        fileName  = cocos2d::StringUtils::format("ball%d-1.png",m_type);
        this->setNewFile(fileName);
    }
    isInselect = isSelect;
}
void BallOrdinary::helpAction(const bool & isHelp){
    std::string fileName;
    if(isHelp&&!isInhelp){
        this->setLocalZOrder(this->getLocalZOrder()+1);
        fileName = cocos2d::StringUtils::format("ball%d-2.png",m_type);
        this->setNewFile(fileName);
        auto action = cocos2d::RepeatForever::create(cocos2d::Blink::create(2, 2));
        action->setTag(1);
        this->runAction(action);
    }else if(isInhelp){
        this->setLocalZOrder(this->getLocalZOrder()-1);
        fileName  = cocos2d::StringUtils::format("ball%d-1.png",m_type);
        this->setNewFile(fileName);
        this->stopActionByTag(1);
    }
    isInhelp = isHelp;
}
void BallOrdinary::eliminate(const std::function<void(void)> & callback){
    m_callFunc = callback;
 //   this->runAction(<#cocos2d::Action *action#>)
//    Sprite* sp = static_cast<Sprite*>(this->getChildByName("boomLight"));
//    if(!sp){
//        std::string fileName = cocos2d::StringUtils::format("ball%d-3.png",m_type);
//        sp = Sprite::create(fileName);
//        sp->setScale(0.0f);
//        sp->setPosition(cocos2d::Vec2(this->getContentSize().width*0.5,this->getContentSize().height*0.5));
//        this->addChild(sp);
//        sp->setName("boomLight");
//    }else{
//        sp->setVisible(true);
//        sp->setScale(0.0f);
//    }
//    this->setOpacity(0);
    auto eliminateAct = JsonEffectFactory::getBalleliminateAct(this->getParent(),this->getLocalZOrder()+1);
    eliminateAct->node->setPosition(this->getPosition());
    CKF_Sound::playEffect("01");
    this->runAction(cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.4f,0.0f),cocos2d::CallFuncN::create([=](cocos2d::Ref*ref){
        auto node = static_cast<cocos2d::Node*>(ref);
        node->setVisible(false);
        node->setScale(1.0f);
        JsonEffectFactory::revertBalleliminateAct(eliminateAct);
        if(m_callFunc){
            m_callFunc();
        }
    }),nullptr));
    eliminateAct->actvec->at(m_type-1)->play();

//    this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.3f),cocos2d::CallFuncN::create([=](Ref* ref){
//
//
//    }), nullptr));
}










