#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
#include <string>
#define NOT_USE_SELF_FRAME
/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/

enum class XDLCAppMissionSucessConditionMode
{
    AppSucessConditionMode_mustReached,
    AppSucessConditionMode_orReached,
    AppSucessConditionMode_selectedReadched,
};

struct sXDLCMissionSuccessCondition
{
    std::string conditionName;
    std::string soperator;
    std::string limitedValue;
    std::string valueVar;
    bool isLastGot;
    XDLCAppMissionSucessConditionMode mode;
};

class  AppDelegate : private cocos2d::Application
{
public:
    static const std::map<std::string, sXDLCMissionSuccessCondition> & getmissionSuccessData(){
        return m_missionSuccess;
    };
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
private:
   static std::map<std::string, sXDLCMissionSuccessCondition> m_missionSuccess;//目标完成条件map
};

#endif // _APP_DELEGATE_H_

