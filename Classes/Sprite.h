//
//  Sprite.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/2.
//

#ifndef Sprite_hpp
#define Sprite_hpp
#include "cocos2d.h"
#include <stdio.h>
class Sprite : public cocos2d::Sprite{
public:
    static Sprite* create(const std::string& filename);
    virtual ~Sprite(){};
    virtual void setNewFile(const std::string &filename);
};
#endif /* Sprite_hpp */
