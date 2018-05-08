#ifndef OpenAnimationLayer_hpp
#define OpenAnimationLayer_hpp
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
#include "cocos2d.h"
class OpenAnimationLayer:public cocos2d::Layer{
public:
    CREATE_FUNC(OpenAnimationLayer);
    void setEndCallFunc(std::function<void()> endCallfunc){m_endCallfunc = endCallfunc;};
private:
    virtual bool init();
    void  onEnter();
    void onEnterTransitionDidFinish();
    void  cleanup();
    void btnClickFunc();
    void endAct(float dt);
private:
    int m_currMissionID;
    int m_iAction;
    cocos2d::ui::Button* m_btn_kongbai;
    cocos2d::ui::ImageView* m_img_beijing;
    cocos2d::ui::ImageView* m_img_beijing1;
    cocos2d::ui::ImageView* m_img_beijing2;
    cocos2d::ui::ImageView* m_img_sige1;
    cocos2d::ui::ImageView* m_img_sige2;
    cocos2d::ui::ImageView* m_img_sige3;
    cocos2d::ui::ImageView* m_img_sige4;
    cocos2d::ui::ImageView* m_img_sige5;
    cocos2d::ui::ImageView* m_img_sige6;
    cocos2d::ui::ImageView* m_img_sige7;
    cocos2d::ui::ImageView* m_img_sige8;
    cocos2d::ui::ImageView* m_img_sige9;
    cocos2d::ui::ImageView* m_img_sige10;
    cocos2d::ui::ImageView* m_img_sige11;
    cocos2d::ui::ImageView* m_img_sige12;
    cocos2d::ui::ImageView* m_img_sige13;
    cocos2d::ui::ImageView* m_img_sige14;
    
    cocos2d::ui::Text* m_label_tishi1;
    cocos2d::ui::Text* m_label_tishi2;
    cocos2d::ui::Text* m_label_tishi3;
    std::function<void()> m_endCallfunc;
};

#endif
