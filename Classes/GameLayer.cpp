#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include "OpenAnimationLayer.h"
#include "CKF_Sound.h"
#include "ToolFunction.h"
#include "cocos/physics3d/CCPhysics3DWorld.h"
#include "JsonActionLayer.h"

GameLayer * GameLayer::m_gameLayer = nullptr;
cocos2d::Scene* GameLayer::createScene(const CKF_GameData& _data)
{
    auto scene = cocos2d::Scene::createWithPhysics();
    if(!m_gameLayer){
        m_gameLayer = GameLayer::create();
        m_gameLayer->setName("gameLayer");
        m_gameLayer->retain();
    }
    m_gameLayer->m_data = _data;
    //scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(cocos2d::Vect(0,-1000));//设置重力参数
    auto createGameLayerFunc=[scene](){
        if(m_gameLayer->getParent()){
            m_gameLayer->removeFromParentAndCleanup(true);
        }
        scene->addChild(m_gameLayer);
    };
    //开场动画
    if (false&&cocos2d::UserDefault::getInstance()->getIntegerForKey("OpenAnimationLayer",0) == 0 &&( _data.GeneralID == 1|| _data.GeneralID == 5))
    {
        auto OpenAnimationLayer = OpenAnimationLayer::create();
        OpenAnimationLayer->setEndCallFunc(createGameLayerFunc);
        scene->addChild(OpenAnimationLayer);
    }else{
        createGameLayerFunc();
    }
    return scene;
}
GameLayer::GameLayer():c_mUiLocalZorder(20){ //常量数据初始化
   
}
//数据清理
void GameLayer::cleanup(){
    cocos2d::Layer::cleanup();
    m_BallOrdinarySelect->clear();
    selectHelpVec->clear();
    for(auto & vec : *m_BallOrdinaryMap){
        for(auto & ball :*vec){
            BallFactory::revertBall(ball);
        }
        vec->clear();
    }
}
GameLayer::~GameLayer(){
    //vector内存释放
    for(auto & vec : *m_BallOrdinaryMap){
        delete vec;
    }
    m_BallOrdinaryMap->clear();
    CKF_Sound::stopBackGround();
    delete m_BallOrdinaryMap;
    m_BallOrdinaryMap = nullptr;
    delete selectHelpVec;
    selectHelpVec = nullptr ;
    delete m_BallOrdinarySelect;
    m_BallOrdinarySelect = nullptr;
    delete [] m_targetAssemble;
    m_targetAssemble = nullptr;
}
bool GameLayer::init(){
    //只需初始化一次的数据
    cocos2d::Layer::init();
    m_needRemoveNodeVec = std::vector<Node*>();
    m_needRemoveNodeVec.reserve(20);
    m_BallOrdinaryMap = new(std::nothrow) std::vector<std::vector<BallOrdinary*>*>(BALLTYPECOUNT);
    for(int i = 0;i<BALLTYPECOUNT;i++){
        auto ballVec =  new(std::nothrow) std::vector<BallOrdinary*>();
        ballVec->reserve(MAXBALLCOUNT/2);//初始一个容器大小
        m_BallOrdinaryMap->at(i) = ballVec;
    }
    m_BallOrdinarySelect = new(std::nothrow) std::vector<BallOrdinary*>();
    m_BallOrdinarySelect->reserve(MAXBALLCOUNT/2); //初始一个容器大小
    
    selectHelpVec = new(std::nothrow) std::vector<BallOrdinary*>();
    selectHelpVec->reserve(20);//初始一个容器大小
    
    isFirstInser = true;
    m_keyListen = nullptr;
    m_touchListen = nullptr;
    m_pauseLayer = nullptr;
    m_exitLayer = nullptr;
    m_gameOverShowLayer = nullptr;
    m_targetshowLayer = nullptr;
    m_targetAssemble = nullptr;
    m_map = nullptr;
    //数据初始化
    visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    //ui
    initUI();
    //目标板
    m_targetshowLayer = TargetShowLayer::create();
    m_targetshowLayer->retain();
    return true;
}
void GameLayer::onEnter(){
    cocos2d::Layer::onEnter();
}
void GameLayer::onEnterTransitionDidFinish(){
    cocos2d::Layer::onEnterTransitionDidFinish();
    //每次都需要初始化的数据
    needAddBallCount = 0;
    //框架数据初始化
#ifndef NOT_USE_SELF_FRAME
    // 关卡目标数据
    CXDLC_GameTestApp::getInstance()->m_iMissionSystem->getMissionSuccessCondition(CXDLC_GameTestApp::getInstance()->game.iCurrenMission, m_missionSuccess);
#else
    m_missionSuccess = AppDelegate::getmissionSuccessData();
#endif
    if(!m_targetAssemble){
        //目前只有三个目标
        m_targetAssemble = new(std::nothrow) targetAssemble[m_missionSuccess.size()];
    }
    //播放背景音
    CKF_Sound::playBackGround("bgMusic");
    //读取配置文件 配置配置文件数据
    this->analyzeCoinfig();
    //显示目标板
    cocos2d::Director::getInstance()->getRunningScene()->addChild(m_targetshowLayer);
    m_targetshowLayer->onShow(m_missionSuccess);
    
    //顶部UI 数据初始化
    initTargetUI();
    //初始化地图信息（地图框等）
    this->analyzeTileMap();
    
    //地图下移游戏开始
    m_map->runAction(cocos2d::Sequence::create(cocos2d::MoveTo::create(0.5f,m_Map_point) ,cocos2d::CallFunc::create([=](){ // 创建球
        //游戏开始补充球
        this->supplyBall(m_ball_count);
    }) ,NULL) );
    //开启UPDATE
    this->scheduleUpdate();
    //开启定时提示
    this->schedule(schedule_selector(GameLayer::ballSelectHelp), 5,CC_REPEAT_FOREVER,5);
    //不是第一次进入游戏 事件需要重新注册(cleanup 造成的  exit 会pause)
    if(!isFirstInser){
        m_pauseButton->setTouchEnabled(false);
        m_pauseButton->setTouchEnabled(true);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_keyListen, this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_touchListen, this);
        this->resume();
    }else{
        isFirstInser = false;
        //键盘监听
        m_keyListen = cocos2d::EventListenerKeyboard::create();
        m_keyListen->retain();
        m_keyListen->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased,this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_keyListen, this);
        
        //触控监听
        m_touchListen = cocos2d::EventListenerTouchOneByOne::create();
        m_touchListen->retain();
        m_touchListen->setSwallowTouches(false);//
        m_touchListen->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
        m_touchListen->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved,this);
        m_touchListen->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
        m_touchListen->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled,this);
        cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_touchListen, this);
    }
}
void GameLayer::analyzeCoinfig(){
    //读取 Mission_General_Config.plist 配置文件数据
    cocos2d::ValueMap data = cocos2d::FileUtils::getInstance()->getValueMapFromFile(m_data.GeneralName);
    cocos2d::ValueMap mission_config =data[cocos2d::StringUtils::format("%d",m_data.GeneralID).c_str()].asValueMap();
    m_ball_count = mission_config["ball_count"].asInt();  //本关全屏总球数
    m_step = mission_config["touch_number"].asInt();
    for(int i =0;i<BALLTYPECOUNT-1;i++){
        m_ballCountMaxPer[i] = mission_config[cocos2d::StringUtils::format("ball_%d_max",i+1)].asFloat();
        m_ballCountMinPer[i] = mission_config[cocos2d::StringUtils::format("ball_%d_min",i+1)].asFloat();
    }
}
void GameLayer::initUI(){
    auto bg = Sprite::create("ui/textures/DDD_ditu2.png");
    this->addChild(bg);
    bg->setPosition(visibleSize.width*0.5,visibleSize.height*0.5);
    //    auto midBg = static_cast<cocos2d::ui::Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(midBgName.c_str()));
    //    this->addChild(midBg);
    
    //顶部UI
    m_root = static_cast<cocos2d::ui::Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/Blue_Internal_Get on/Blue_Internal_Get on.json"));
    this->addChild(m_root,c_mUiLocalZorder);
    // 暂停按钮
    m_pauseButton = static_cast<cocos2d::ui::Button*>(m_root->getChildByName("Button_Setup"));
    m_pauseButton->addClickEventListener([this](cocos2d::Ref* ref){
        if(!m_pauseLayer){
            m_pauseLayer = PauseLayer::create();
            m_pauseLayer->retain();
        }else{
            m_pauseLayer->stopAllActions();
            m_pauseLayer->setPosition(cocos2d::Vec2(0.0f,0.0f));
            m_pauseLayer->setVisible(true);
        }
        cocos2d::Director::getInstance()->getRunningScene()->addChild(m_pauseLayer);
    });
    auto image =static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName("Image_Frame_2"));
    m_scoreLabel = static_cast<cocos2d::ui::TextAtlas*>(image->getChildByName("AtlasLabel_Score"));
    m_scoreLabel->setString("0");
    image =static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName("Image_Frame_6"));
    m_touchAtlas = static_cast<cocos2d::ui::TextAtlas*>(image->getChildByName("AtlasLabel_Step Number"));
    //关卡id
    auto lv = static_cast<cocos2d::ui::TextAtlas*>(image->getChildByName("AtlasLabel_1"));
    lv->setString(cocos2d::StringUtils::format("%d",m_data.GeneralID));
    // 底部UI
    m_bottom = static_cast<cocos2d::ui::Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/game_ui/game_item/Blue_Internal_Down_0.json"));
    m_bottom->setContentSize(visibleSize);
    this->addChild(m_bottom,c_mUiLocalZorder);
}
void GameLayer::initTargetUI(){
    //步数初始化
    m_touchAtlas->setString(cocos2d::StringUtils::format("%d",this->getStep()));
    //目标数据初始化
    int i = 0;
    for(auto & condition : m_missionSuccess){
        targetAssemble target = targetAssemble();
        target.image = static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName(cocos2d::StringUtils::format("Image_Frame_%d",i+3))->getChildByName(cocos2d::StringUtils::format("Image_%d",i+1)));
        target.number = static_cast<cocos2d::ui::TextAtlas*>(m_root->getChildByName(cocos2d::StringUtils::format("Image_Frame_%d",i+3))->getChildByName("AtlasLabel_1"));
        target.reachImage = static_cast<cocos2d::ui::ImageView*>(m_root->getChildByName(cocos2d::StringUtils::format("Image_Frame_%d",i+3))->getChildByName(cocos2d::StringUtils::format("Image_2%d",i+3)));
       
        
        target.number->setString(condition.second.limitedValue.c_str());
        target.reachImage->setVisible(false);
        if(!condition.second.valueVar.compare("RedCount")){
            target.ballType = 1;
        }else if(!condition.second.valueVar.compare("YellowCount")){
            target.ballType = 2;
        }else if(!condition.second.valueVar.compare("BlueCount")){
            target.ballType = 3;
        }else if(!condition.second.valueVar.compare("GreenCount")){
            target.ballType = 4;
        }else if(!condition.second.valueVar.compare("PurpleCount")){
            target.ballType = 5;
        }
        std::string fileName = cocos2d::StringUtils::format("ball%d-1.png", target.ballType);
        auto frame = cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);
        if(frame){
            target.image->loadTexture(fileName,cocos2d::ui::TextureResType::PLIST);
        }else{
            target.image->loadTexture(fileName);
        }
         m_targetAssemble[i] = target;
        i++;//下标增加1
    }
}
void GameLayer::getlinkBallVec(std::vector<BallOrdinary*> & ballCanLinkVec,BallOrdinary* &  ball){
    const int type = ball->getType();
    auto ballVec = m_BallOrdinaryMap->at(type);
    ballCanLinkVec.push_back(ball);
    for(int i = 0;i<ballCanLinkVec.size();i++){
        for(auto ball : * ballVec){
            if(std::find(ballCanLinkVec.begin(),ballCanLinkVec.end(),ball)!=ballCanLinkVec.end()){
                continue;
            }
            if(ball->getPosition().distance(ballCanLinkVec.at(i)->getPosition())<ball->getBoundingBox().size.width * 1.20f){
                ballCanLinkVec.push_back(ball);
            }
        }
    }
}
void GameLayer::getlinkBallVec(std::vector<BallOrdinary*>& ballCanLinkVec,int minCount/* 最少连球 */){
    std::vector<BallOrdinary*> vecHasBeinUse = std::vector<BallOrdinary*>();
    for(auto & vec :* m_BallOrdinaryMap){
        for(auto & ball : * vec){
            ballCanLinkVec.clear();
            this->getlinkBallVec(ballCanLinkVec, ball);
            if(ballCanLinkVec.size()>minCount){
                return;
            }
        }
    }
}
void GameLayer::selectCancelled(){
    for(auto & ball : * m_BallOrdinarySelect){
        ball->setIsSelect(false);
    }
    m_BallOrdinarySelect->clear();
}
float GameLayer::removeSelectBall(std::vector<BallOrdinary*> & vec){
    int i = 0;
    float delayTime = 0.0f;
    auto delay = 0.1f;
    for(auto & ball:vec){
        i++;
        auto ballVec = m_BallOrdinaryMap->at(ball->getType());
        auto itr = std::find(ballVec->begin(),ballVec->end(),ball);
      //  if(itr != ballVec->end()){//避免球已经不再地图上了
            ballVec->erase(itr);     //从 m_BallOrdinaryMap 中移除
            delayTime += delay;
            delay = delay>0.05?delay-0.01:0.05;//延时逐级 减0.01 不低于0.05
            ball->runAction(cocos2d::Spawn::create(cocos2d::Sequence::create(cocos2d::DelayTime::create(delayTime),cocos2d::CallFunc::create([=](){
                for(int i = 0;i<sizeof(m_targetAssemble[0])/sizeof(m_targetAssemble);i++){
                    auto & target = m_targetAssemble[i];
                    //目标数减一
                    if(ball->getType()==target.ballType){
                       
                        auto  count = std::atoi(target.number->getString().c_str());
                      
                        if(count > 0){
                            //飞向 目标动作
                            std::string fileName = cocos2d::StringUtils::format("ball%d-1.png",target.ballType);
                            auto sp = Sprite::create(fileName);
                            this->addChild(sp,c_mUiLocalZorder);
                            sp->setPosition(ball->getPosition());
                            auto bezierto = ToolFunction::getBzier(sp->getPosition(),target.image->getParent()->convertToWorldSpace(target.image->getPosition()),0,1.0f);
                            sp->runAction(cocos2d::Sequence::create(bezierto,cocos2d::DelayTime::create(0.1f),cocos2d::RemoveSelf::create(),nullptr));
                            target.number->setString(cocos2d::StringUtils::format("%d",count-1));
                            if(count-1==0){
                                 target.reachImage->setVisible(true);
                            }
                            //检测游戏是否胜利
                            for(int j = 0;j<sizeof(m_targetAssemble[0])/sizeof(m_targetAssemble);j++){
                                auto & target = m_targetAssemble[j];
                                if(!target.reachImage->isVisible()){
                                    break;
                                }
                            }
                            //游戏胜利
                        }
                        break;
                    }
                }
                ball->boomAction([=](){
                    BallFactory::revertBall(ball);  //退回ball
                    this->supplyBall();             //补充ball
                });
            }), nullptr),nullptr));
       // }
    }
    vec.clear();//清空
    return delayTime;
}
bool GameLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    this->selectCancelled(); // 安卓默认多点触控  后面的会顶了前面的  并且不走cancel
    this->ballSelectHelpClose(0.0f);
    this->unschedule(schedule_selector(GameLayer::ballSelectHelp));
    for(auto & vec : *m_BallOrdinaryMap){
        for(auto & ball : *vec){
            auto a = ball->getBoundingBox();
            if(ball->getBoundingBox().containsPoint(touch->getLocation())){
                this->getlinkBallVec(*m_BallOrdinarySelect,ball);
                for(auto & ball : *m_BallOrdinarySelect){
                    ball->setIsSelect(true);
                }
                return true;
            }
        }
    }
    return true;
}
void GameLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event){
    for(auto & ball : *m_BallOrdinarySelect){
        if(ball->getBoundingBox().containsPoint(touch->getLocation())){
            return;
        }
    }
    this->selectCancelled();
}
void GameLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    this->gameSuccess();
    this->schedule(schedule_selector(GameLayer::ballSelectHelp), 5,CC_REPEAT_FOREVER,5);
    if(m_BallOrdinarySelect->size()>1){ //大于1个球  并且手指未离开第一个球
        //步数减一
        this->setStep(this->getStep()-1);
        
        //计算可以得的分数
        int score = 20;
        for(int i = 3;i <= m_BallOrdinarySelect->size();i++){ //需要 = 于size
            int x = 10 * i;
            score += x;
        }
        //分数标签  有对应球的type
        auto label =cocos2d::ui::TextAtlas::create(cocos2d::StringUtils::format("%d",score),cocos2d::StringUtils::format("fonts/gsc_szbq_%d.png",m_BallOrdinarySelect->at(0)->getType()), 29, 39, "0");
        //根据消除球数量 放大label
        float scale = 0.5+(m_BallOrdinarySelect->size()-2)*0.05;
        scale = scale>1.5?1.5:scale;
        label->setScale(scale);
        
        label->setPosition(m_BallOrdinarySelect->at(0)->getPosition());
        this->addChild(label,c_mUiLocalZorder);
        label->runAction(cocos2d::Sequence::create(cocos2d::MoveBy::create(1.0f,cocos2d::Vec2(0,100)),cocos2d::RemoveSelf::create(), nullptr));
        auto startPosition = m_BallOrdinarySelect->at(0)->getPosition();
        int size =(int)m_BallOrdinarySelect->size();
        
        
        float delta = removeSelectBall(*m_BallOrdinarySelect);//移除被选的球 返回需要的延时
        
        
        //鼓励语  每8个加一个等级 所有球移除 播放
        if(size>=8){
            auto encourageGrade = size / 8 > 5?5: size / 8 ;
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delta),cocos2d::CallFunc::create([encourageGrade](){
                CKF_Sound::playEffect(cocos2d::StringUtils::format("voice_0%d",encourageGrade));
            }),nullptr));
        }
    }else{
        this->selectCancelled();
    }
}
void GameLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event){
    this->selectCancelled();
}
void GameLayer::gameSuccess(){
    return;
    //取消touch监听
    _eventDispatcher->removeEventListener(m_touchListen);
    if(!m_gameOverShowLayer){
        m_gameOverShowLayer = GameOverShowLayer::create();
        m_gameOverShowLayer->retain();
    }
    this->addChild(m_gameOverShowLayer,c_mUiLocalZorder);
    //步数变成奖励效果
    int count = 0;
    for(auto & vec : *m_BallOrdinaryMap){
        count+=vec->size();
    }
    int i =0;
    std::vector<BallOrdinary*> ballVec = std::vector<BallOrdinary*>();
    ballVec.resize(count);
    for(auto & vec : *m_BallOrdinaryMap){
        for(auto & ball :* vec){
            ballVec.at(i) = ball;
            i++;
        }
    }
    //乱序
    std::random_shuffle(ballVec.begin(),ballVec.end());
    //对count 前count位 按位置排序
    count = m_step > count ? count:m_step;
    auto itr = ballVec.begin();
    for(int i = 0;i<count - 1;i++){
        itr++;
    }
    std::sort(ballVec.begin(), itr,[](BallOrdinary* ball1,BallOrdinary* ball2){
        return ball1->getPositionY()<ball2->getPositionY();
    });
    //特效
    for(int i = 0;i<count-1;i++) {
        auto ball = ballVec.at(i);
        auto tempspr = Sprite::create("ui/xing1.png");
        tempspr->setRotation(180);
        tempspr->setPosition(cocos2d::Vec2(DESIGNX_TO_DESIGNX(100+40),visibleSize.height - DESIGNY_TO_DESIGNY(100)));
        tempspr->setVisible(false);
        tempspr->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        tempspr->setScale(0.5f);
        
        // 拖尾粒子
        auto lizi = JsonActionLayer::createTuoWeiEffect(2);
        lizi->setPosition(cocos2d::Vec2(tempspr->getBoundingBox().size.width / 2, tempspr->getBoundingBox().size.height / 2));
        tempspr->addChild(lizi);
        
        // 拖尾
        auto tuowei = cocos2d::MotionStreak::create(0.3, 1.0, 35.0, cocos2d::Color3B(255,255,255), "ui/gsc_xingtuowei.png"); // Color3B(254,201,76)
        tuowei->setPosition(cocos2d::Vec2(100+40,visibleSize.height -DESIGNY_TO_DESIGNY(100)));
        this->addChild(tempspr, c_mUiLocalZorder);
        this->addChild(tuowei, c_mUiLocalZorder);
        
        
    }
    
}
void GameLayer::setStep(const int & step){
    m_step = step;
    m_touchAtlas->setString(cocos2d::StringUtils::format("%d",m_step));
    //步数小于0
    if(m_step<=0){
        //this->removeFromParentAndCleanup(true);
    }
}
void GameLayer::boomArea(cocos2d::Vec2 pointCenter,int grade){
    auto animation = cocos2d::AnimationCache::getInstance()->getAnimation("boomArea");
    if(!animation){
        cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect/boom/baozha.plist","effect/boom/baozha.png");
        animation = ToolFunction::createFrameAnimation(1,12,"baozha_%d.png","boomArea",0.1f,false);
    }
    auto sp = cocos2d::Sprite::create();
    m_ballBatchNode->addChild(sp);
    sp->setPosition(pointCenter);
    auto animate = cocos2d::Animate::create(animation);
    sp->runAction(cocos2d::Sequence::create(animate,cocos2d::RemoveSelf::create(),nullptr));
    //消除球
    std::vector<BallOrdinary*> ballSelecTemp;
    for(auto & vec : *m_BallOrdinaryMap){
        for(auto & ball : *vec)
        {
            if(ball->getPosition().getDistance(pointCenter)<= BALL_DATA_PHYSICS_BODY_RADIUS * grade){
                ballSelecTemp.push_back(ball);
            }
        }
    }
    removeSelectBall(ballSelecTemp);
}
void GameLayer::showEncourageWord(int id){
    std::string fileName;
    switch (id) {
        case 1:
            fileName = "game_word/game_word1.png";//哎呦不错哦
            break;
        case 2:
            fileName = "game_word/game_word2.png";//太厉害了
            break;
        case 3:
            fileName = "game_word/game_word3.png";//真是棒棒的
            break;
        case 4:
            fileName = "game_word/game_word4.png";//完美的连接
            break;
        case 5:
            fileName = "game_word/game_word5.png";//只有5步了
            break;
        default:
            break;
    }
    
    Sprite* word = Sprite::create(fileName);
    this->addChild(word,c_mUiLocalZorder);
    word->setOpacity(0);
    word->setPosition(cocos2d::Vec2(visibleSize.width*0.5,visibleSize.height*0.5));
    auto fadeIn = cocos2d::FadeIn::create(0.5);
    auto move = cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0,DESIGNY_TO_DESIGNY(200)));
    auto spawn = cocos2d::Spawn::create(fadeIn,move,nullptr);
    word->runAction(cocos2d::Sequence::create(spawn,cocos2d::FadeOut::create(0.5f),nullptr));
}
void GameLayer::supplyBall(const int & count){
    needAddBallCount+=count;
}
int GameLayer::getRandBallType()
{
    float r = CCRANDOM_0_1();
    for(int i =0;i<5;i++){
        if(r<m_ballCountMaxPer[i]&&r>m_ballCountMinPer[i]){
            return i+1;
        }
    }
    return 0;
}
void GameLayer::update(float dt){
    //分帧添加球
    if(needAddBallCount>0){
        needAddBallCount--;
        cocos2d::Vec2 pos ;
        if(m_mapWidth_left <= 0 && m_mapWidth_Right <= 0){
            pos = cocos2d::Vec2(CCRANDOM_0_1() * (m_right_bottom.x - m_left_bottom.x -50) + (m_left_bottom.x+20), visibleSize.height - 100);
        }else{
            pos = cocos2d::Vec2( (m_mapWidth_left + rand() % (static_cast<int>( m_mapWidth_Right - m_mapWidth_left)))   , visibleSize.height - 100);
            if(pos .x < m_mapWidth_left+100)
                pos .x += 100;
            if(pos .x >m_mapWidth_Right -100)
                pos .x -= 100;
        }
        int type = getRandBallType();//获取要补充的type
        auto ball = BallFactory::getBallOrdinary(type);
        m_BallOrdinaryMap->at(type)->push_back(ball);
        ball->setPosition(DESIGN_TO_DESIGN(pos));
        m_ballBatchNode->addChild(ball);
    }
}
void GameLayer::ballSelectHelp(float dt){
    if(selectHelpVec->size()>0){ //已经存在 selectHelpVec 了
        auto ball = selectHelpVec->at(0);
        ballSelectHelpClose(0.0f);
        getlinkBallVec(*selectHelpVec,ball);
    }else{
        getlinkBallVec(*selectHelpVec);
    }
    for(auto & ball:*selectHelpVec){
        ball->setIsSelect(true); //提示也用select图片了
    }
    //一秒后图片变回未选定状态
    this->scheduleOnce(schedule_selector(GameLayer::ballSelectHelpClose),1.0f);
}
void GameLayer::ballSelectHelpClose(float dt){
    for(auto & ball : * selectHelpVec){
        ball->setIsSelect(false);
    }
    selectHelpVec->clear();
}
void GameLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags){
    
}
void GameLayer::onExitTransitionDidStart(){
    cocos2d::Layer::onExitTransitionDidStart();
    for(auto & node : m_needRemoveNodeVec) {
        node->removeFromParentAndCleanup(true);
    }
    m_needRemoveNodeVec.clear();
}

void GameLayer::onExit(){
    cocos2d::Layer::onExit();
}
void GameLayer::analyzeTileMap(){
    //地图 TMXTiledMap anchorPoint 在 0,0
    m_map = cocos2d::TMXTiledMap::create(cocos2d::StringUtils::format("ui/tmx/tmx%d/%s",m_data.ThemeID,m_data.TiledName.c_str()));
    this->addChild(m_map);
    m_Map_point = DESIGN_TO_DESIGN(25, TMX_OFFSET);
    m_map->setPosition(DESIGN_TO_DESIGN(25,TMX_OFFSET + m_map->getContentSize().height));
    
    m_needRemoveNodeVec.push_back(m_map);//需要在离开场景时移除m_map
    //创建物理墙
    
    cocos2d::Size size = m_map->getMapSize();
    //get mengban
    //创建蒙版图片
    cocos2d::TMXObjectGroup* meng = m_map->getObjectGroup("meng");
    auto meng_objs = meng->getObjects();
    //    for(auto p_block:m_vblock){
    //        p_block->setVisible(true);
    //    }
    //    m_mask = Sprite::create();
    for (auto obj : meng_objs)
    {
        auto cube = obj.asValueMap();
        float x   = cube["x"].asFloat(); //起点x y坐标 (正常cocos坐标系坐标)
        float y   = cube["y"].asFloat();
        //physics对象，多边形的折现点相对起始坐标的相对坐标
        auto va = cube["polylinePoints"].asValueVector();
        
        cocos2d::Vec2 *vecPois = new(std::nothrow) cocos2d::Vec2[va.size() - 1];
        int numPois = 0;
        for (auto p : va)
        {
            if (numPois < va.size()-1 )
            {
                auto points = p.asValueMap();
                auto xoff = points["x"].asFloat();
                auto yoff = points["y"].asFloat();//以起点为基准 （Y 轴向上为负   向下为正（相反）  x轴向左为负  向右为正(正常) ）
                float newx = x + xoff;
                float newy = y - yoff;
                vecPois[numPois] = DESIGN_TO_DESIGN(newx, newy);
                numPois++;
            }
            
        }
        cocos2d::ClippingNode* nodeClip = cocos2d::ClippingNode::create();
        nodeClip->addChild(cocos2d::LayerColor::create(cocos2d::Color4B(0, 1, 16, 200)));//球区 背景色
        //nodeClip->setContentSize(cocos2d::Size(visibleSize.height, 0));
        nodeClip->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
        cocos2d::DrawNode* stencil = cocos2d::DrawNode::create();
        stencil->drawPolygon(vecPois, numPois, cocos2d::Color4F(1, 0, 0, 1), 0.001, cocos2d::Color4F(0, 1, 0, 1));//不可见
        nodeClip->setStencil(stencil);
        nodeClip->setInverted(false);
        delete[] vecPois;
        m_map->addChild(nodeClip);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //get physics line
    //创建物理边界
    cocos2d::TMXObjectGroup* line = m_map->getObjectGroup("physics");  //多边形框
    auto objs = line->getObjects();
    
    auto val = line->getProperties();
    
    m_mapWidth_left  =  val["widthl"].asFloat();
    m_mapWidth_Right =  val["widthr"].asFloat();
    
    
    for (auto obj : objs)
    {
        auto valuemap = obj.asValueMap();
        
        //physics对象，多边形起始点坐标(坐标系原点在左上)
        float x = valuemap["x"].asFloat();
        float y = valuemap["y"].asFloat();
        
        //physics对象，多边形的折现点相对起始坐标的相对坐标
        auto va = valuemap["polylinePoints"].asValueVector();
        cocos2d::PointArray *location = cocos2d::PointArray::create(0);
        for (auto p : va)
        {
            auto points = p.asValueMap();
            auto xoff = points["x"].asFloat();
            auto yoff = points["y"].asFloat();
            float newx = x + xoff;
            float newy = y - yoff;
            location->addControlPoint(DESIGN_TO_DESIGN(newx+12, newy));
        }
        for (int i=0; i<location->count()-1; i++)
        {
            cocos2d::Vec2 p1 = location->getControlPointAtIndex(i);
            cocos2d::Vec2 p2 = location->getControlPointAtIndex(i+1);
            auto node = Node::create();
            auto body = cocos2d::PhysicsBody::createEdgeSegment(p1, p2);
            
            node->setPhysicsBody(body);
            node->setPosition(DESIGN_TO_DESIGN(12, TMX_OFFSET));
            this->addChild(node);
            m_needRemoveNodeVec.push_back(node);
        }
    }
    
    m_mapWidth_left  = 0;
    m_mapWidth_Right =  visibleSize.width;
    //底部
    if(isFirstInser){
        cocos2d::Vec2 p1 = cocos2d::Vec2::ZERO;
        cocos2d::Vec2 p2 = cocos2d::Vec2(visibleSize.width, 0);
        auto node1 = Node::create();
        auto body1 = cocos2d::PhysicsBody::createEdgeSegment(p1, p2);
        body1->setName("wall_1");
        node1->setPhysicsBody(body1);
        node1->setPosition(DESIGN_TO_DESIGN(12, TMX_OFFSET));
        this->addChild(node1, c_mUiLocalZorder - 1);
        //右
        p1 = cocos2d::Vec2(visibleSize.width, 0);
        p2 = cocos2d::Vec2(visibleSize.width, visibleSize.height);
        auto node2 = cocos2d::Node::create();
        auto body2 = cocos2d::PhysicsBody::createEdgeSegment(p1, p2);
        body2->setName("wall_2");
        node2->setPhysicsBody(body2);
        node2->setPosition(DESIGN_TO_DESIGN(12, TMX_OFFSET));
        this->addChild(node2, c_mUiLocalZorder - 1);
        //上
        p1 = cocos2d::Vec2(visibleSize.width, visibleSize.height);
        p2 = cocos2d::Vec2(0, visibleSize.height);
        auto node3 = cocos2d::Node::create();
        auto body3 = cocos2d::PhysicsBody::createEdgeSegment(p1, p2);
        body3->setName("wall_3");
        node3->setPhysicsBody(body3);
        node3->setPosition(DESIGN_TO_DESIGN(12, TMX_OFFSET));
        this->addChild(node3, c_mUiLocalZorder);
        //左
        p1 = cocos2d::Vec2(0, visibleSize.height);
        p2 = cocos2d::Vec2(0, 0);
        auto node4 = cocos2d::Node::create();
        auto body4 = cocos2d::PhysicsBody::createEdgeSegment(p1, p2);
        body4->setName("wall_4");
        node4->setPhysicsBody(body4);
        node4->setPosition(DESIGN_TO_DESIGN(12, TMX_OFFSET));
        this->addChild(node4, c_mUiLocalZorder - 1);
    }
    
    
    //create cliping node
    //创建裁剪范围
    cocos2d::TMXObjectGroup* effect = m_map->getObjectGroup("effect"); //方形框
    auto effect_objs = effect->getObjects();
    cocos2d::Vec2 left_top = cocos2d::Vec2::ZERO;
    cocos2d::Vec2 left_bottom = cocos2d::Vec2::ZERO;
    cocos2d::Vec2 right_top = cocos2d::Vec2::ZERO;
    cocos2d::Vec2 right_bottom = cocos2d::Vec2::ZERO;
    for (auto p : effect_objs)
    {
        auto cube = p.asValueMap();
        left_bottom = cocos2d::Vec2(cube["x"].asFloat(), cube["y"].asFloat());
        right_bottom = cocos2d::Vec2(cube["x"].asFloat() + cube["width"].asFloat(), cube["y"].asFloat());
        left_top = cocos2d::Vec2(cube["x"].asFloat(), cube["y"].asFloat() + cube["height"].asFloat());
        right_top = cocos2d::Vec2(cube["x"].asFloat() + cube["width"].asFloat(), cube["y"].asFloat() + cube["height"].asFloat());
        
        m_left_bottom = left_bottom;
        m_right_bottom = right_bottom;
        m_top = left_top.y>right_top.y?left_top:right_top;
    }
    auto clip_width = right_bottom.x - left_bottom.x;
    auto clip_height = right_top.y - right_bottom.y;
    auto location =  left_bottom + cocos2d::Vec2(0, TMX_OFFSET);
    //m_ballBatchNode = SpriteBatchNode::create("ui/ball/ui_image.png");
    m_ballBatchNode = Node::create(); //
    location.x +=20;
    m_ballBatchNode->setPosition(DESIGN_TO_DESIGN(-location));
    
    //create batch node
    auto clip = cocos2d::ClippingNode::create();
    clip->setContentSize(cocos2d::Size(clip_width, clip_height));
    clip->setPosition( location );
    clip->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    auto stencil = cocos2d::DrawNode::create();
    cocos2d::Vec2 rectangle[4];
    rectangle[0] = cocos2d::Vec2(0, 0);
    rectangle[1] = DESIGN_TO_DESIGN(clip->getContentSize().width, 0);
    rectangle[2] = DESIGN_TO_DESIGN(clip->getContentSize().width, clip->getContentSize().height);
    rectangle[3] = DESIGN_TO_DESIGN(0, clip->getContentSize().height);
    cocos2d::Color4F white = cocos2d::Color4F::WHITE;
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    clip->setStencil(stencil);
    clip->setInverted(false);
    clip->addChild(m_ballBatchNode,1);
    this->addChild(clip);
    m_needRemoveNodeVec.push_back(clip);
    //创建地图物理底部
    //create down physics line ,because Debris mode is use.
    auto bottom_line = cocos2d::Node::create();
    auto bottom_body = cocos2d::PhysicsBody::createEdgeSegment(cocos2d::Vec2(0, 0), DESIGN_TO_DESIGN(right_bottom.x - left_bottom.x, 0),
                                                               cocos2d::PHYSICSSHAPE_MATERIAL_DEFAULT,2);
    bottom_line->setPhysicsBody(bottom_body);
    bottom_line->setTag(5490);
    bottom_body->setCategoryBitmask(1);
    bottom_body->setContactTestBitmask(3);
    bottom_line->setPosition(DESIGN_TO_DESIGN(left_bottom + cocos2d::Vec2(0, TMX_OFFSET)));
    this->addChild(bottom_line);
    m_needRemoveNodeVec.push_back(bottom_line);
}
void GameLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event){
    switch (keyCode)
    {
            //返回按钮事件
        case cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE:
        case cocos2d::EventKeyboard::KeyCode::KEY_BACK:
        {
#ifndef NOT_USE_SELF_FRAME
            if(CXDLC_GameTestApp::getInstance()->m_iOptionSystem->IsHaveThirdParthExitGame())
            {
                CXDLC_GameTestApp::getInstance()->m_iOptionSystem->App_ExitGame();
            }else{
#endif
                if (!m_exitLayer) {
                    m_exitLayer = static_cast<cocos2d::ui::Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("UInew/json_exit.json"));
                    this->addChild(m_exitLayer, c_mUiLocalZorder + 1);
                    auto Image_1 = static_cast<cocos2d::ui::ImageView*>(m_exitLayer->getChildByName("Image_1"));
                    auto yes = static_cast<cocos2d::ui::Button*>(Image_1->getChildByName("Button_yes"));
                    auto no = static_cast<cocos2d::ui::Button*>(Image_1->getChildByName("Button_no"));
                    yes->addClickEventListener([](cocos2d::Ref* ref){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#ifndef NOT_USE_SELF_FRAME
                        string app = CXDLC_GameTestApp::getInstance()->m_iUpdateSystem->app_getAppVersion();
                        int  version = atoi(app.substr(app.rfind(".")+1,string::npos).c_str());
                        
                        //   华为   魅族
                        if (version == 40 || version == 28 || version == 16 || version == 41 || version == 13)
                        {
                            callJniCusotm1();
                        }
                        else
                        {
                            CXDLC_GameTestApp::getInstance()->exitGame();
                        }
#else
                        cocos2d::Director::getInstance()->end();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                        cocos2d::Director::getInstance()->end();
                        exit(0);
#endif
#endif
                    });
                    auto exit = m_exitLayer;//不拷贝this
                    no->addClickEventListener([exit](cocos2d::Ref*ref){
                        exit->setVisible(false);
                    });
                }else{
                    m_exitLayer->setVisible(true);
                }
#ifndef NOT_USE_SELF_FRAME
            }
#endif
        }
            break;
    }
}
