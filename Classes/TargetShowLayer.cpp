//
//  TargerShowLayer.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/5/7.
//

#include "TargetShowLayer.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/UITextAtlas.h"
#include "cocos/ui/CocosGUI.h"
TargetShowLayer::TargetShowLayer(){
    m_root = nullptr;
}
void TargetShowLayer::onEnterTransitionDidFinish(){
    cocos2d::Layer::onEnterTransitionDidFinish();
    //加载UI数据
    if(!m_root){
        m_root = static_cast<cocos2d::ui::Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/Blue_Internal_4/Blue_Internal_4.json"));
        for(int i = 0;i<3;i++){
             m_imageAim[i] = static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName("Image_mubiao")->getChildByName(cocos2d::StringUtils::format("Image_%d",i+1)));
             m_atlasAim[i] = static_cast<cocos2d::ui::TextAtlas*>(m_root->getChildByName("Image_mubiao")->getChildByName(cocos2d::StringUtils::format("AtlasLabel_%d",i+7)));
        }
         m_btnFull = static_cast<cocos2d::ui::Button*>( m_root->getChildByName("Button_quanpingguanbi"));
         m_btnFull->addClickEventListener(CC_CALLBACK_0(TargetShowLayer::onBtnClose,this));
         this->addChild( m_root);
    }
    this->setVisible(false);
    m_btnFull->setTouchEnabled(false);
}
void TargetShowLayer::onBtnClose(){
    m_btnFull->setTouchEnabled(false);
    auto act = cocostudio::ActionManagerEx::getInstance()->getActionByName("aimBroad/Blue_Internal_4.json", "Animation0");
    act->play(cocos2d::CallFunc::create([this](){
        this->setVisible(false);
    }));
}
void TargetShowLayer::onShow(std::map<std::string, sXDLCMissionSuccessCondition> missionSuccess){
    this->setVisible(true);
    int i =0;
    int type = 0;
    //初始化目标板上的 目标
    for(auto & condition : missionSuccess){
        if(!condition.second.valueVar.compare("RedCount")){
            type = 1;
        }else if(!condition.second.valueVar.compare("YellowCount")){
            type = 2;
        }else if(!condition.second.valueVar.compare("BlueCount")){
            type = 3;
        }else if(!condition.second.valueVar.compare("GreenCount")){
            type = 4;
        }else if(!condition.second.valueVar.compare("PurpleCount")){
            type = 5;
        }
        std::string fileName = cocos2d::StringUtils::format("ball%d-1.png", type);
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
        if(frame){
            m_imageAim[i]->loadTexture(fileName,cocos2d::ui::TextureResType::PLIST);
        }else{
            m_imageAim[i]->loadTexture(fileName);
        }
        m_atlasAim[i]->setString(condition.second.limitedValue.c_str());
        i++;
    }

    auto act = cocostudio::ActionManagerEx::getInstance()->getActionByName("aimBroad/Blue_Internal_4.json", "Animation1");
    act->play(cocos2d::CallFunc::create([=](){
        m_btnFull->setTouchEnabled(true);
    }));
}

