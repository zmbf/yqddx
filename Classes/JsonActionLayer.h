//
//  JsonActionLayer.h
//  mmppx
//
//  Created by xdlc on 2017/9/4.
//
//

#ifndef JsonActionLayer_h
#define JsonActionLayer_h

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

class JsonActionLayer: public cocos2d::Layer
{
public:
    JsonActionLayer(void);
    ~JsonActionLayer(void);
    
    CREATE_FUNC(JsonActionLayer);
    
    static JsonActionLayer* createArrowLayer(float delay);
    static Node* createArrowLayer(std::string jsonName, std::string animationName, float delay);
    // 精灵拖尾效果
    // k:需要几个粒子发射器
    static Node* createTuoWeiEffect(int k);
    
    static Node* createChuanSongEffect(cocos2d::Vec2 gravity, int colorType, bool isIn);
    
    static Node* createParticleSkill(void);
    
    static Node* createRoleSkillGuideEffect(cocos2d::Vec2 gravity);
    
private:
    
    void ActionExPlay(cocostudio::ActionObject* act);
    
protected:
    
    
    
};


#endif /* JsonActionLayer_h */
