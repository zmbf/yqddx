//
//  PauseLayer.cpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/27.
//

#include "PauseLayer.h"
#include "CKF_Sound.h"
#include "GameData.h"
#include  <map>
#include "GameLayer.h"
#include "AppDelegate.h"
PauseLayer::PauseLayer(){
    m_root = nullptr;
}
void PauseLayer::onEnterTransitionDidFinish(){
    cocos2d::Layer::onEnterTransitionDidFinish();
    if(!m_root){
        m_root = static_cast<cocos2d::ui::Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/pause/json_Pause.json"));
        this->addChild(m_root);
        CKF_Sound::playEffect(pauseSound);
        auto bg = static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName("Image_bg"));
        //重新开始
        btn_again = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_again"));
        btn_again->addClickEventListener(CC_CALLBACK_0(PauseLayer::goOnCallBack,this));
        //继续游戏
        m_btn_continue = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_close"));
        m_btn_continue->addClickEventListener(CC_CALLBACK_0(PauseLayer::onBtnClose, this));
        //继续游戏(界面外围屏幕)
        btn_close_kuangwai = static_cast<cocos2d::ui::Button*>(m_root->getChildByName("Button_close_kuangwai"));
        btn_close_kuangwai->addClickEventListener(CC_CALLBACK_0(PauseLayer::onBtnClose, this));
        //退出
        btn_exit = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_quit"));
        btn_exit->addClickEventListener(CC_CALLBACK_0(PauseLayer::goHomeCallBack,this));
        // 帮助按钮
        m_btn_help = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_Help"));
        m_btn_help->addClickEventListener(CC_CALLBACK_0(PauseLayer::onBtnHelp,this));
        
        btn_music = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_shengyin"));
        btn_yinxiao = static_cast<cocos2d::ui::Button*>(bg->getChildByName("Button_yinxiao"));
        
        Image_open_shengyin = static_cast<cocos2d::ui::ImageView*>(btn_music->getChildByName("Image_open"));
        Image_close_shengyin = static_cast<cocos2d::ui::ImageView*>(btn_music->getChildByName("Image_close"));
        
        Image_open_yinxiao = static_cast<cocos2d::ui::ImageView*>(btn_yinxiao->getChildByName("Image_open"));
        Image_close_yinxiao = static_cast<cocos2d::ui::ImageView*>(btn_yinxiao->getChildByName("Image_close"));
        
        btn_music->addClickEventListener(CC_CALLBACK_0(PauseLayer::music,this));
        btn_yinxiao->addClickEventListener(CC_CALLBACK_0(PauseLayer::effect,this));
    }else{
        //重新注册按钮事件
        m_btn_help->setTouchEnabled(false);
        m_btn_continue->setTouchEnabled(false);
        btn_again->setTouchEnabled(false);
        btn_close_kuangwai->setTouchEnabled(false);
        btn_exit->setTouchEnabled(false);
        btn_music->setTouchEnabled(false);
        btn_yinxiao->setTouchEnabled(false);
        
        m_btn_help->setTouchEnabled(true);
        m_btn_continue->setTouchEnabled(true);
        btn_again->setTouchEnabled(true);
        btn_close_kuangwai->setTouchEnabled(true);
        btn_exit->setTouchEnabled(true);
        btn_music->setTouchEnabled(true);
        btn_yinxiao->setTouchEnabled(true);
    }
    if(CKF_Sound::getBackGroundVolume() <0.01f)
    {
        Image_open_shengyin->setVisible(false);
        Image_close_shengyin->setVisible(true);
    }
    else
    {
        Image_open_shengyin->setVisible(true);
        Image_close_shengyin->setVisible(false);
    }
    
    if(CKF_Sound::getEffectsVolume() < 0.01f)
    {
        Image_open_yinxiao->setVisible(false);
        Image_close_yinxiao->setVisible(true);
    }
    else
    {
        Image_open_yinxiao->setVisible(true);
        Image_close_yinxiao->setVisible(false);
    }
}
void PauseLayer::music(){
    CKF_Sound::playEffect("buttonpause");
    if(CKF_Sound::getBackGroundVolume() <0.01f){
        Image_open_shengyin->setVisible(true);
        Image_close_shengyin->setVisible(false);
        CKF_Sound::setBackGroundVolume(1.0f);
    }
    else{
        CKF_Sound::setBackGroundVolume(0.0f);
        Image_close_shengyin->setVisible(true);
        Image_open_shengyin->setVisible(false);
    }
}
void PauseLayer::effect(){
    CKF_Sound::playEffect("buttonpause");
    if(CKF_Sound::getEffectsVolume() <0.01f){
        CKF_Sound::setEffectsVolume(1.0f);
        Image_open_yinxiao->setVisible(true);
        Image_close_yinxiao->setVisible(false);
    }
    else{
        CKF_Sound::setEffectsVolume(0.0f);
        Image_close_yinxiao->setVisible(true);
        Image_open_yinxiao->setVisible(false);
    }
    
}

void PauseLayer::onBtnHelp(void)
{
    cocos2d::ui::Widget* root = static_cast<cocos2d::ui::Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/json_item_property_pause/json_item_property_pause.json"));
    cocos2d::ui::Button* btnFull = static_cast<cocos2d::ui::Button*>(root->getChildByName("Button_close_kuangwai"));
    cocos2d::ui::Button* btnClose = static_cast<cocos2d::ui::Button*>(root->getChildByName("Image_bg")->getChildByName("Button_close"));
    this->addChild(root, 2);
    auto closeCallBack = [root](Ref* ref){
        root->removeFromParent();
    };
    btnFull->addClickEventListener(closeCallBack);
    btnClose->addClickEventListener(closeCallBack);
    
}
void PauseLayer::goOnCallBack(void)
{
    CKF_Sound::playEffect(clickSound);
    auto gameLayer = static_cast<GameLayer*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName("gameLayer"));
    auto data = gameLayer->getData();
    auto scene = GameLayer::createScene(data);
    cocos2d::Director::getInstance()->replaceScene(scene);
}


void PauseLayer::onBtnClose(void)
{
    CKF_Sound::playEffect(clickSound);
    this->runAction(cocos2d::Sequence::create(cocos2d::CallFunc::create([this](){
        this->removeFromParentAndCleanup(true);
        cocos2d::Director::getInstance()->getRunningScene()->getChildByName("gameLayer")->resume();
    }), nullptr));
}
void PauseLayer::goHomeCallBack(){
    CKF_Sound::playEffect(clickSound);
    CKF_Sound::stopBackGround();
#ifndef NOT_USE_SELF_FRAME
    auto scene = cocos2d::Director::getInstance()->getRunningScene();
    std::map<string,int> Value;
    __NotificationCenter::getInstance()->removeObserver(this, "pause");
    __NotificationCenter::getInstance()->removeObserver(this, "resume");
    Value.insert(map<string,int>::value_type("Score",0));
    Value.insert(map<string,int>::value_type("Gold",0));
    Value.insert(map<string,int>::value_type("MeanWhileBoomCount",0));
    Value.insert(map<string,int>::value_type("BombCount",0));
    Value.insert(map<string,int>::value_type("ThunderCount",0));
    Value.insert(map<string,int>::value_type("CorssCount",0));
    Value.insert(map<string,int>::value_type("SwirlCount",0));
    Value.insert(map<string,int>::value_type("BallCount",0));
    Value.insert(map<string,int>::value_type("RedCount",0));
    Value.insert(map<string,int>::value_type("YellowCount",0));
    Value.insert(map<string,int>::value_type("BlueCount",0));
    Value.insert(map<string,int>::value_type("GreenCount",0));
    Value.insert(map<string,int>::value_type("PurpleCount",0));
    Value.insert(map<string,int>::value_type("Temperature",0));
    Value.insert(map<string,int>::value_type("ItemCount",scene->m_item_score_add*100));
    Value.insert(map<string,int>::value_type("PropCount",scene->savePropCount));
    Value.insert(map<string,int>::value_type("BuyItemCount",scene->saveBuyItemCount));
    scene->removeStarListener();  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    CKF_Sound::stopBackGround();
    CXDLC_GameTestApp::getInstance()->game.endPlay(Value);
#endif
}

