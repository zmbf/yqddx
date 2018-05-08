#pragma once
#define USE_AUDIO_ENGINE 1
#include<string>
class CKF_Sound
{
public:
    static int  playEffect(std::string filename, bool bLoop = false);
    static void playBackGround(std::string filename);
    static void preloadEffectSound(std::string filename);
    static void preloadMusicSound(std::string filename);
    static void stopEffect(int _soundID);
    static void stopAllEffect();
    static void pauseAllEffect();
    static void pauseBackGround();
    static void resumeBackGround();
    static void stopBackGround();
    static void resumeAllEffect();
    static void setEffectsVolume(float volume);
    static float getEffectsVolume();
    static void setBackGroundVolume(float volume);
    static float getBackGroundVolume();
private:
    static std::string formatFileName(std::string & fileName);
private:
    static int backGroundAudioId;//缓存背景音乐ID 区分音效
    static float effectVolume;
    static float musicVolume;
};
