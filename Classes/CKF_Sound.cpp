#include "CKF_Sound.h"
#include "cocos2d.h"
USING_NS_CC;
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#else
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif
int CKF_Sound::backGroundAudioId = -1;
float CKF_Sound::effectVolume = 1.0f;
float CKF_Sound::musicVolume = 1.0f;
std::string CKF_Sound::formatFileName(std::string & fileName){
    std::string fullName;
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_WP8 ) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32))
    fullName = cocos2d::StringUtils::format("sound/wp/%s.wav",fileName.c_str());
#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
    fullName = cocos2d::StringUtils::format("sound/ios/%s.mp3",fileName.c_str());
#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID))
    fullName = cocos2d::StringUtils::format("sound/android/%s.ogg",fileName.c_str());
#else
    CCASSERT(0, "CKF_Sound::playEffect Not Found Platform Sound.");
#endif
    return fullName;
}
int CKF_Sound::playEffect(std::string filename, bool bLoop)
{
        std::string fullName = formatFileName(filename);
#if USE_AUDIO_ENGINE
        return AudioEngine::play2d(fullName,bLoop,effectVolume);
#elif USE_SIMPLE_AUDIO_ENGINE
        return SimpleAudioEngine::getInstance()->playEffect(fullName,bLoop);
#endif
}

void CKF_Sound::playBackGround(std::string filename)
{
    std::string fullName;
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_WP8 ) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32))
    fullName = cocos2d::StringUtils::format("sound/wp/%s.wav",filename.c_str());
#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
    fullName = cocos2d::StringUtils::format("sound/ios/%s.mp3",filename.c_str());
#elif ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID))
    fullName = cocos2d::StringUtils::format("sound/android/%s.mp3",filename.c_str());
#endif
    
#if USE_AUDIO_ENGINE
        if(backGroundAudioId!=-1){
            AudioEngine::stop(backGroundAudioId);
        }
        backGroundAudioId = AudioEngine::play2d(fullName,true,musicVolume);
        AudioEngine::setFinishCallback(backGroundAudioId, [=](int audioId,const std::string &  fileName){
            backGroundAudioId = -1;
        });
#elif USE_SIMPLE_AUDIO_ENGINE
        SimpleAudioEngine::getInstance()->playBackgroundMusic(fullName,true);
#endif
}
void CKF_Sound::preloadEffectSound(std::string filename)
{
        std::string fullName = formatFileName(filename);
#if USE_AUDIO_ENGINE
        AudioEngine::preload(fullName);
#elif USE_SIMPLE_AUDIO_ENGINE
        SimpleAudioEngine::getInstance()->preloadEffect(fullName);
#endif
}

void CKF_Sound::preloadMusicSound(std::string filename)
{
    std::string fullName = formatFileName(filename);
#if USE_AUDIO_ENGINE
        AudioEngine::preload(fullName);
#elif USE_SIMPLE_AUDIO_ENGINE
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(fullName);
#endif
}
void  CKF_Sound::setEffectsVolume(float volume){
#if USE_AUDIO_ENGINE
    effectVolume = volume;
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
#endif
}
float CKF_Sound::getEffectsVolume(){
#if USE_AUDIO_ENGINE
    return effectVolume;
#elif USE_SIMPLE_AUDIO_ENGIN
    return SimpleAudioEngine::getInstance()->getEffectsVolume();
#endif
}
void CKF_Sound::setBackGroundVolume(float volume){
#if USE_AUDIO_ENGINE
    musicVolume = volume;
    AudioEngine::setVolume(backGroundAudioId, volume);
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
#endif
}
float CKF_Sound::getBackGroundVolume(){
#if USE_AUDIO_ENGINE
    return musicVolume;
#elif USE_SIMPLE_AUDIO_ENGIN
    return SimpleAudioEngine::getInstance()->getBackGroundVolume();
#endif
}
void  CKF_Sound::stopBackGround(){
#if USE_AUDIO_ENGINE
    if(backGroundAudioId!=-1){
        AudioEngine::stop(backGroundAudioId);
        backGroundAudioId = -1;
    }
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#endif
}
void CKF_Sound::stopEffect(int _soundID)
{
#if USE_AUDIO_ENGINE
    AudioEngine::stop(_soundID);
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->stopEffect(_soundID);
#endif
}
void CKF_Sound::pauseBackGround(){
#if USE_AUDIO_ENGINE
    if(backGroundAudioId!=-1){
        AudioEngine::pause(backGroundAudioId);
    }
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
}

void CKF_Sound::resumeBackGround(){
#if USE_AUDIO_ENGINE
    if(backGroundAudioId!=-1){
        AudioEngine::resume(backGroundAudioId);
    }
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
}

void CKF_Sound::stopAllEffect(){
#if USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->stopAllEffects();
#endif
}
void CKF_Sound::resumeAllEffect(){
#if USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->setEffectsVolume(80);
#endif
}
void CKF_Sound::pauseAllEffect(){
#if USE_AUDIO_ENGINE
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->setEffectsVolume(0);
#endif
}
