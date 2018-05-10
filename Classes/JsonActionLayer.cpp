//
//  JsonActionLayer.cpp
//  mmppx
//
//  Created by xdlc on 2017/9/4.
//
//

#include "JsonActionLayer.h"
USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;
JsonActionLayer::JsonActionLayer(void)
{}

JsonActionLayer::~JsonActionLayer(void)
{}


JsonActionLayer* JsonActionLayer::createArrowLayer(float delay)
{
    JsonActionLayer* layer = JsonActionLayer::create();
    
    Widget* arrow = GUIReader::getInstance()->widgetFromJsonFile("ball/effect/lighting/shizixiaochu_1.json");
    ActionObject* act = ActionManagerEx::getInstance()->getActionByName("ball/effect/lighting/shizixiaochu_1.json", "Animation0");

    
    Size size = arrow->getContentSize();
    arrow->setPosition(Point(size.width, size.height / 2));
    
    layer->addChild(arrow, 1);
    layer->setAnchorPoint(Vec2(0, 0));
    layer->runAction(Sequence::create(DelayTime::create(delay),
                                   CallFuncN::create(CC_CALLBACK_0(JsonActionLayer::ActionExPlay, layer, act)),
                                   NULL));
    
    return layer;
}

Node* JsonActionLayer::createArrowLayer(std::string jsonName, std::string animationName,float delay)
{
//    JsonActionLayer* layer = JsonActionLayer::create();
    
    Node* node = Node::create();
    
    Widget* arrow = GUIReader::getInstance()->widgetFromJsonFile(jsonName.c_str());
    arrow->setName("arrow");
    ActionObject* act = ActionManagerEx::getInstance()->getActionByName(jsonName.c_str(), animationName.c_str());
    arrow->setPosition(Vec2(-arrow->getContentSize().width  / 2, -arrow->getContentSize().height / 2 ));
    node->addChild(arrow, 1);
    act->play();
//    node->runAction(Sequence::create(DelayTime::create(delay),
//                                     CallFunc::create([act](){
//                                        act->play();
//                                    }),
//                                   NULL));
//    
    return node;
}

void JsonActionLayer::ActionExPlay(ActionObject* act)
{
    act->play();
}


// 精灵拖尾粒子效果
Node* JsonActionLayer::createTuoWeiEffect(int k){
    
    auto node = Node::create();
    
    
    for(int i = 0; i< k; ++i){
        auto _emitter = ParticleSystemQuad::createWithTotalParticles(50);
        
        _emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
        _emitter->setPositionType(ParticleSystem::PositionType::FREE);
        // duration
        _emitter->setDuration(ParticleSystem::DURATION_INFINITY);
        

        // angle
        _emitter->setAngle(90);
        _emitter->setAngleVar(90);
        
        // emitter position
        _emitter->setPosVar(Point::ZERO);
        

        
        // emits per second
        _emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());
        
        switch (i) {

            case 0:
                // life of particles
                _emitter->setLife(0.5);
                _emitter->setLifeVar(0.2);
                _emitter->setTotalParticles(10);
                _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/xing1.png"));
                _emitter->setStartColor(Color4F(Color4B(255, 245, 165, 255)));
                _emitter->setStartColorVar(Color4F(Color4B(0, 0, 0, 100)));
                _emitter->setEndColor(Color4F(Color4B(255, 245, 165, 0)));
                _emitter->setEndColorVar(Color4F(Color4B(0, 0, 0, 0)));
                
                // size, in pixels
                _emitter->setStartSize(40);
                _emitter->setStartSizeVar(5);
                _emitter->setEndSize(50);
                _emitter->setEndSizeVar(5);
                
                break;
            case 1:
                // life of particles
                _emitter->setLife(0.5);
                _emitter->setLifeVar(0.2);
                _emitter->setTotalParticles(10);
                _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/kongxing.png"));
                _emitter->setStartColor(Color4F(Color4B(255, 245, 165, 255)));
                _emitter->setStartColorVar(Color4F(Color4B(0, 0, 0, 100)));
                _emitter->setEndColor(Color4F(Color4B(255, 245, 165, 0)));
                _emitter->setEndColorVar(Color4F(Color4B(0, 0, 0, 0)));
                
                // size, in pixels
                _emitter->setStartSize(10);
                _emitter->setStartSizeVar(5);
                _emitter->setEndSize(70);
                _emitter->setEndSizeVar(5);
                
                break;
            default:
                _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/xing1.png"));
                break;
        }
        
        node->addChild(_emitter, 10);
    }
    return node;
}

// 传送门粒子效果
Node* JsonActionLayer::createChuanSongEffect(Vec2 gravity, int colorType, bool isIn){
    
    auto node = Node::create();
    
    auto _emitter = ParticleSystemQuad::createWithTotalParticles(5);
    
    _emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
    _emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/ball/effect/liziEffect/lizitupian.png"));
    
    // duration
    _emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    _emitter->setGravity(gravity);
    _emitter->setContentSize(Size(10, 0));
    // angle
    _emitter->setAngle(90);
    _emitter->setAngleVar(90);
    
    // emitter position
    _emitter->setPosition(Vec2(0, 0));
    _emitter->setPosVar(Vec2(10, 10));
    
    _emitter->setLife(1.0);
    _emitter->setLifeVar(0.2);
    
    if(isIn){
        _emitter->setStartSize(10);
        _emitter->setStartSizeVar(2);
        _emitter->setEndSize(3);
        _emitter->setEndSizeVar(2);
    }else{
        _emitter->setStartSize(3);
        _emitter->setStartSizeVar(2);
        _emitter->setEndSize(10);
        _emitter->setEndSizeVar(2);
    }
    //        BlendFunc blend;
    _emitter->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    // emits per second
    _emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());
    
    switch (colorType) {
            
        case 2019: // 青色
            
            _emitter->setStartColor(Color4F(Color4B(154, 255, 247, 255)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 1019: // 青色
            
            _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(154, 255, 247, 255)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 2020: // 紫色
            
            _emitter->setStartColor(Color4F(Color4B(182, 171, 255, 255)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 1020: // 紫色
            
            _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(182, 171, 255, 255)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 2021: // 橙色
            
            _emitter->setStartColor(Color4F(Color4B(255, 226, 142, 255)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 1021: // 橙色
            
            _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(255, 226, 142, 255)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 2022: // 绿色
            
            _emitter->setStartColor(Color4F(Color4B(224, 255, 202, 255)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        case 1022: // 绿色
            
            _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(224, 255, 202, 255)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            
            
            break;
        default:
            
            _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 255)));
            _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
            _emitter->setEndColor(Color4F(Color4B(230, 230, 230, 0)));
            _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));
            break;
    }
    
    node->addChild(_emitter, 10);

    return node;
}

Node* JsonActionLayer::createParticleSkill(void)
{
    Node* node = Node::create();
    
    auto _emitter = ParticleSystemQuad::createWithTotalParticles(15);
    node->addChild(_emitter, 1);
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/ball/effect/liziEffect/gsc_guangxiao2.png"));
    _emitter->setAnchorPoint(Point(0, 0));
    // duration
    _emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    
    // gravity mode
    _emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
//    _emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
    _emitter->setPositionType(ParticleSystem::PositionType::FREE);
    
//    _emitter->setStartRadius(50);
//    _emitter->setStartRadiusVar(3);
//    _emitter->setEndRadius(50);
//    _emitter->setEndRadiusVar(3);
    
    
    // emitter position
    
    // life of particles
    _emitter->setLife(0.3);
    _emitter->setLifeVar(0.2);
    
    // spin of particles
    _emitter->setStartSpin(20);
    _emitter->setStartSpinVar(10);
    _emitter->setEndSpin(100);
    _emitter->setEndSpinVar(20);
    
    _emitter->setStartColor(Color4F(Color4B(153, 255, 234, 255)));
    _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
    _emitter->setEndColor(Color4F(Color4B(220, 255, 255, 0)));
    _emitter->setEndColorVar(Color4F(Color4B(10, 10, 10, 0)));
    
    // size, in pixels
    _emitter->setStartSize(20);
    _emitter->setStartSizeVar(10);
    _emitter->setEndSize(50);
    _emitter->setEndSize(30);
    // emits per second
    _emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());

    
    
    return node;
}

// 传送门粒子效果
Node* JsonActionLayer::createRoleSkillGuideEffect(Vec2 gravity){
    
    auto node = Node::create();
    
    auto _emitter = ParticleSystemQuad::createWithTotalParticles(30);
    
    _emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
    _emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
    _emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("ui/guideRes/yd_guangdian.png"));
    
    // duration
    _emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    _emitter->setGravity(gravity);
    _emitter->setContentSize(Size(10, 0));
    // angle
    _emitter->setAngle(90);
    _emitter->setAngleVar(90);
    
    // emitter position
    _emitter->setPosition(Vec2(0, 0));
    _emitter->setPosVar(Vec2(50, 10));
    
    _emitter->setLife(0.7);
    _emitter->setLifeVar(0.2);
    
    _emitter->setStartSize(30);
    _emitter->setStartSizeVar(5);
    _emitter->setEndSize(3);
    _emitter->setEndSizeVar(2);

    //        BlendFunc blend;
    _emitter->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    // emits per second
    _emitter->setEmissionRate(_emitter->getTotalParticles() / _emitter->getLife());
    

            
    _emitter->setStartColor(Color4F(Color4B(230, 230, 230, 0)));
    _emitter->setStartColorVar(Color4F(Color4B(10, 10, 10, 0)));
    _emitter->setEndColor(Color4F(Color4B(255, 226, 142, 255)));
    _emitter->setEndColorVar(Color4F(Color4B(20, 20, 20, 0)));

    
    node->addChild(_emitter, 10);
    
    return node;
}









