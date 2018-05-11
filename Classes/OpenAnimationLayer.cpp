//
//  OpenAnimationLayer.cpp
//  yqxxl-mobile
//
//  Created by xdlc on 2018/4/21.
//

#include "OpenAnimationLayer.h"

bool OpenAnimationLayer::init(){
    Layer::init();
    return true;
}
void OpenAnimationLayer::onEnter(){
    Layer::onEnter();
    m_currMissionID = 1;
    m_iAction = -1;
    
    
    cocos2d::ui::Widget* rootNode = static_cast<cocos2d::ui::Widget*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/OpenAnimation/NewUi_1.ExportJson"));
    this->addChild(rootNode);
    m_btn_kongbai = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_kongbaitu"));
    m_img_beijing = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_beijing"));
    m_img_beijing1 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_beijing2"));
    m_img_beijing2 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_beijing3"));
    m_img_sige1 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige1"));
    m_img_sige2 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige2"));
    m_img_sige3 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige3"));
    m_img_sige4 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige4"));
    m_img_sige5 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige5"));
    m_img_sige6 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige6"));
    m_img_sige7 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige7"));
    m_img_sige8 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige8"));
    m_img_sige9 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige9"));
    m_img_sige10 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige10"));
    m_img_sige11 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige11"));
    m_img_sige12 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige12"));
    m_img_sige13 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige13"));
    m_img_sige14 = static_cast<cocos2d::ui::ImageView*>(rootNode->getChildByName("Image_sige14"));
    m_label_tishi1 = static_cast<cocos2d::ui::Text*>(rootNode->getChildByName("Label_tishi1"));
    m_label_tishi2 = static_cast<cocos2d::ui::Text*>(rootNode->getChildByName("Label_tishi2"));
    m_label_tishi3 = static_cast<cocos2d::ui::Text*>(rootNode->getChildByName("Label_tishi3"));
    
    m_btn_kongbai->setVisible(false);
    m_btn_kongbai->setEnabled(false);
    m_btn_kongbai->addClickEventListener(CC_CALLBACK_0(OpenAnimationLayer::btnClickFunc, this));
    m_label_tishi1->setVisible(false);
    m_label_tishi2->setVisible(false);
    m_label_tishi3->setVisible(false);
    
    
}
void OpenAnimationLayer::onEnterTransitionDidFinish(){
    Layer::onEnterTransitionDidFinish();
    if(m_currMissionID == 1)
    {
        m_iAction =0;
        cocostudio::ActionManagerEx::getInstance()->getActionByName("ui/OpenAnimation/NewUi_1.json", "Animation0")->play();
        scheduleOnce(schedule_selector(OpenAnimationLayer::endAct),2.8f);
    }else if(m_currMissionID == 5){
        m_img_beijing->setVisible(false);
        m_img_beijing1->setVisible(false);
        m_img_sige1->setVisible(false);
        m_img_sige2->setVisible(false);
        m_img_sige3->setVisible(false);
        m_img_sige4->setVisible(false);
        m_img_sige5->setVisible(false);
        m_img_sige6->setVisible(false);
        m_img_sige7->setVisible(false);
        m_img_sige8->setVisible(false);
        m_img_sige9->setVisible(false);
        
        m_img_beijing2->setVisible(true);
        m_img_sige10->setVisible(true);
        m_img_sige11->setVisible(true);
        m_img_sige12->setVisible(true);
        m_img_sige13->setVisible(true);
        m_img_sige14->setVisible(true);
        cocostudio::ActionManagerEx::getInstance()->getActionByName("ui/NewGuide/OpenAnimation/NewUi_1.json", "Animation2")->play();
        m_iAction = 2;
        scheduleOnce(schedule_selector(OpenAnimationLayer::endAct), 3.3);
    }
}
void  OpenAnimationLayer::cleanup(){
    Layer::cleanup();
    //
}
void OpenAnimationLayer::btnClickFunc(void)
{
    if (m_iAction == 0)
    {
        m_btn_kongbai->setVisible(false);
        m_btn_kongbai->setEnabled(false);
        
        m_label_tishi1->setVisible(true);
        m_img_beijing->setVisible(false);
        m_img_sige1->setVisible(false);
        m_img_sige2->setVisible(false);
        m_img_sige3->setVisible(false);
        m_img_sige4->setVisible(false);
        m_img_sige5->setVisible(false);
        
        m_img_beijing1->setVisible(true);
        m_img_sige6->setVisible(true);
        m_img_sige7->setVisible(true);
        m_img_sige8->setVisible(true);
        m_img_sige9->setVisible(true);
        
        cocostudio::ActionManagerEx::getInstance()->getActionByName("ui/NewGuide/OpenAnimation/NewUi_1.json", "Animation1")->play();
        m_iAction = 1;
        scheduleOnce(schedule_selector(OpenAnimationLayer::endAct), 2.4);
    }
    else if (m_iAction == 1)
    {
        if(m_currMissionID == 1){
            m_endCallfunc();
            this->removeFromParent();
        }
    }else if(m_iAction == 2){
        if(m_currMissionID == 5){
            m_endCallfunc();
            this->removeFromParent();
        }
    }
    
}
void OpenAnimationLayer::endAct(float dt){
    m_btn_kongbai->setEnabled(true);
    m_btn_kongbai->setVisible(true);
    if(0 == m_iAction){
        m_label_tishi1->setVisible(true);
    }else if(1 == m_iAction){
        m_label_tishi2->setVisible(true);
    }else if(2 == m_iAction){
        m_label_tishi3->setVisible(true);
    }
    
}








