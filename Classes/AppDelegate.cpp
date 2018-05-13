#include "AppDelegate.h"
#include "GameLayer.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your GameScene!"
#endif
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(720, 1280);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
std::map<std::string, sXDLCMissionSuccessCondition> AppDelegate::m_missionSuccess = std::map<std::string, sXDLCMissionSuccessCondition>();
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("yqxxl", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("yqxxl");
#endif
        director->setOpenGLView(glview);
    }
    FileUtils::getInstance()->addSearchPath("ui/textures/ball");
    //FileUtils::getInstance()->addSearchPath("ui/game_ui");
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::EXACT_FIT);
//    auto frameSize = glview->getFrameSize();
//    // if the frame's height is larger than the height of medium size.
//    if (frameSize.height > mediumResolutionSize.height)
//    {
//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is larger than the height of small size.
//    else if (frameSize.height > smallResolutionSize.height)
//    {
//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is smaller than the height of medium size.
//    else
//    {
//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//    }
    srand((int)time(NULL));
    register_all_packages();
    CKF_GameData data = CKF_GameData{
        0x7fffffff,//int GameTime;
        120,//int BallScore;
        12,//int HitScore;
        1,//int ThemeID;
        "config/Mission_Scene_Config.plist",//std::string ThemeName;
        "gameTest_1.tmx",//std::string TiledName;
        "config/Mission_General_Config.plist",//std::string GeneralName;
        5,//int GeneralID;
        1,//int StarOne;
        0x7fffffff,//int StarTwo;
        1,//int StarThree;
        1, //int Need_Star;
        1,//int Need_Red;
        1,//int Need_Yellow;
        1,//int Need_Blue;
        1,//int Need_Green;
        1,//int Need_Purple;
        1,//int Need_Ice;
        1,//int Need_Debris;
        1//int bossBlood;
    };
    
    m_missionSuccess = std::map<std::string, sXDLCMissionSuccessCondition>();
    sXDLCMissionSuccessCondition condition = sXDLCMissionSuccessCondition();
    condition.conditionName = "消除蓝色球";
    condition.soperator = ">=";
    condition.limitedValue = "2";
    condition.valueVar ="BlueCount";
    condition.isLastGot = false;
    condition.mode = XDLCAppMissionSucessConditionMode::AppSucessConditionMode_mustReached;
    m_missionSuccess.insert(std::pair<std::string,sXDLCMissionSuccessCondition>(condition.valueVar,condition));
    
    condition.conditionName = "消除红色球";
    condition.soperator = ">=";
    condition.limitedValue = "2";
    condition.valueVar ="RedCount";
    condition.isLastGot = false;
    condition.mode = XDLCAppMissionSucessConditionMode::AppSucessConditionMode_mustReached;
    m_missionSuccess.insert(std::pair<std::string,sXDLCMissionSuccessCondition>(condition.valueVar,condition));
    
    condition.conditionName = "消除蓝色球";
    condition.soperator = ">=";
    condition.limitedValue = "2";
    condition.valueVar ="YellowCount";
    condition.isLastGot = false;
    condition.mode = XDLCAppMissionSucessConditionMode::AppSucessConditionMode_mustReached;
    m_missionSuccess.insert(std::pair<std::string,sXDLCMissionSuccessCondition>(condition.valueVar,condition));
    
    /*
     合图加载
     */
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/textures/merger/ball.plist");
    // create a scene. it's an autorelease object
    auto scene = GameLayer::createScene(data);
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
