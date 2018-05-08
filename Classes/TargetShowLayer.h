//
//  TargerShowLayer.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/7.
//
//游戏开始的目标展示zang'si板
#ifndef TargerShowLayer_hpp
#define TargerShowLayer_hpp

#include "cocos2d.h"
#include "cocos/ui/UIButton.h"
#include "cocos/ui/UIImageView.h"
#include "cocos/ui/UILayout.h"
#include "cocos/ui/UITextAtlas.h"
#include "AppDelegate.h"
class TargetShowLayer : public cocos2d::Layer{
public:
    CREATE_FUNC(TargetShowLayer);
    void onShow(std::map<std::string, sXDLCMissionSuccessCondition> missionSuccess);
private:
    TargetShowLayer();
    void onEnterTransitionDidFinish();
    void onBtnClose();
private:
    int m_iId;  //关卡id
    cocos2d::ui::Widget* m_root;     //板子json
    cocos2d::ui::ImageView* m_imageAim[3]; //目标1
    cocos2d::ui::TextAtlas* m_atlasAim[3]; //目标数量3
    cocos2d::ui::Button* m_btnFull;      //全屏关闭按钮
};
#endif /* TargerShowLayer_hpp */
