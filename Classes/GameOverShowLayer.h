//
//  GameOverShowLayer.hpp
//  yqddx
//
//  Created by xdlc on 2018/5/9.
//
//

#ifndef GameOverShowLayer_hpp
#define GameOverShowLayer_hpp
#include "cocos2d.h"
#include "cocos/ui/UILayout.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
class GameOverShowLayer: public cocos2d::Layer{
public:
    CREATE_FUNC(GameOverShowLayer);
private:
    GameOverShowLayer();
    virtual ~GameOverShowLayer(){};
    virtual void onEnterTransitionDidFinish();
    cocos2d::ui::Widget * m_root;
};
#endif /* GameOverShowLayer_hpp */
