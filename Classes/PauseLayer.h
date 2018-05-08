//
//  PauseLayer.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/27.
//

#ifndef PauseLayer_hpp
#define PauseLayer_hpp
#include "cocos2d.h"
#include "cocos/ui/UIButton.h"
#include "cocos/ui/UIImageView.h"
#include "cocos/ui/UILayout.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
class PauseLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(PauseLayer);
private:
    PauseLayer();
    void onEnterTransitionDidFinish();
    void goOnCallBack(void);
    void goHomeCallBack(void);
    void onBtnClose(void);
    void onBtnHelp(void);
    void onBtnHelpClose(Ref* pSender);
    void onMusic(void);
    void openPauseLayer();
    void music();
    void effect();
private:
    cocos2d::ui::Button* m_btn_help;
    cocos2d::ui::Button* m_btn_continue;
    cocos2d::ui::Button* btn_again;
    cocos2d::ui::Button* btn_close_kuangwai;
    cocos2d::ui::Button* btn_exit;
    cocos2d::ui::Button* btn_music;
    cocos2d::ui::Button* btn_yinxiao;
    
    cocos2d::ui::Layout * m_root;
    cocos2d::ui::ImageView* Image_open_shengyin;
    cocos2d::ui::ImageView* Image_close_shengyin;
    cocos2d::ui::ImageView* Image_open_yinxiao;
    cocos2d::ui::ImageView* Image_close_yinxiao;
};
#endif /* PauseLayer_hpp */
