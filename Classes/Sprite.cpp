//
//  Sprite.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/2.
//

#include "Sprite.h"
Sprite* Sprite::create(const std::string& filename)
{
    Sprite *sprite = new (std::nothrow) Sprite();
    auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if (sprite)
    {
        if(!frame){
            sprite->initWithFile(filename);
        }else{
            sprite->initWithSpriteFrame(frame);
        }
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
void Sprite::setNewFile(const std::string &filename){
    auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
    if(frame){
        this->setSpriteFrame(frame);
    }else{
        this->setTexture(filename);
    }
}
