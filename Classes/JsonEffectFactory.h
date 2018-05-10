//
//  JsonEffect.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/10.
//

#ifndef JsonEffect_hpp
#define JsonEffect_hpp
#include "cocos2d.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include <vector>
struct EliminateAct;
class JsonEffectFactory{
public:
    static EliminateAct*  getBalleliminateAct(cocos2d::Node*  parentNode,const int & localZorder);//获取消除普通球效果
    static void revertBalleliminateAct(EliminateAct*  eliminateAct);
    static void initBalleliminateAct(const int & count);
private:
    static std::vector<EliminateAct*>* m_eliminateActVec;
};
struct EliminateAct{//球消除
    cocos2d::ui::Widget* node;
    std::vector<cocostudio::ActionObject*>* actvec;
};

#endif /* JsonEffect_hpp */
