//
//  ToolFunction.cpp
//  yqxxl
//
//  Created by xdlc on 2018/4/21.
//
//

#include "ToolFunction.h"
std::chrono::steady_clock::time_point ToolFunction::_lastUpdate = std::chrono::steady_clock::now();
cocos2d::Vec2 DESIGN_TO_DESIGN(float x,float y){
    return cocos2d::Vec2((x) * cocos2d::Director::getInstance()->getVisibleSize().width / 720.0f,(y) * cocos2d::Director::getInstance()->getVisibleSize().height / 1280.0f);
}
cocos2d::Vec2 DESIGN_TO_DESIGN(cocos2d::Vec2(p)){
    return cocos2d::Vec2((p).x * cocos2d::Director::getInstance()->getVisibleSize().width / 720.0f,(p).y * cocos2d::Director::getInstance()->getVisibleSize().height / 1280.0f);
}
float DESIGNY_TO_DESIGNY(float y){
    return y * cocos2d::Director::getInstance()->getVisibleSize().height / 1280.0f;
}
float DESIGNX_TO_DESIGNX(float x){
    return x * cocos2d::Director::getInstance()->getVisibleSize().width / 720.0f;
}


cocos2d::Animation* ToolFunction::createFrameAnimation(int indexStar,int indexEnd,std::string urlFormat,std::string nameForSave /*保存的名字*/,float perUnit,bool isRestoreOriginalFrame  /* 是否返回到第一帧 */){
    auto animation =cocos2d::Animation::create();
    for(int i = indexStar; i<= indexEnd ;i++){
        auto fileName = cocos2d::StringUtils::format(urlFormat.c_str(),i);
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
        if(frame){
            animation->addSpriteFrame(frame);
        }else{
            animation->addSpriteFrameWithFile(fileName);
        }
    }
    animation->setDelayPerUnit(perUnit);
    animation->setRestoreOriginalFrame(isRestoreOriginalFrame);
    if(nameForSave.compare("")){//存在保存名字
        cocos2d::AnimationCache::getInstance()->addAnimation(animation,nameForSave);
    }
    return animation;
}
