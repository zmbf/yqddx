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
    if(nameForSave.compare("")){//不是""则保存名字
        cocos2d::AnimationCache::getInstance()->addAnimation(animation,nameForSave);
    }
    return animation;
}
inline void ToolFunction::calculateDeltaTimeFunc(std::string id){
#if COCOS2D_DEBUG > 0
    auto now = std::chrono::steady_clock::now();
    auto _deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastUpdate).count();
    cocos2d::log("timeDelta  %s :  %lld",id.c_str(),_deltaTime);
    _lastUpdate = now;
#endif
}
cocos2d::BezierTo* ToolFunction::getBzier(cocos2d::Vec2 _startPoint, cocos2d::Vec2 _endPoint,int height,float deltime){
    cocos2d::ccBezierConfig cfg;
    
    float p1_X =(_startPoint.x -400)+(rand()%9) * 20;
    
    if(p1_X > cocos2d::Director::getInstance()->getVisibleSize().width){
        p1_X = cocos2d::Director::getInstance()->getVisibleSize().width - 100;
    }
    
    float p2_X = (_startPoint.x +600)+(rand()%9) * 20;
    
    if(p2_X > cocos2d::Director::getInstance()->getVisibleSize().width){
        p1_X =  p1_X = cocos2d::Director::getInstance()->getVisibleSize().width - 100;
    }
    cfg.controlPoint_1 = cocos2d::Vec2( p1_X, _startPoint.y + 100+height );
    cfg.controlPoint_2 = cocos2d::Vec2( p1_X, _startPoint.y + 200+height );
    cfg.endPosition =_endPoint;
    auto bezierto =  cocos2d::BezierTo::create(deltime, cfg);
    return bezierto;
}











