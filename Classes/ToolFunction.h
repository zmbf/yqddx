//
//  ToolFunction.hpp
//  yqxxl
//
//  Created by xdlc on 2018/4/21.
//
//

#ifndef ToolFunction_hpp
#define ToolFunction_hpp
#include "cocos2d.h"
#define calculateDeltaTime(d) ToolFunction::calculateDeltaTimeFunc(d)
//
cocos2d::Vec2 DESIGN_TO_DESIGN(float x,float y);  //将以设计时 designResolutionSize 的point 转为动态designResolutionSize 为基准的  位置
cocos2d::Vec2 DESIGN_TO_DESIGN(cocos2d::Vec2(p));
float DESIGNY_TO_DESIGNY(float y);
float DESIGNX_TO_DESIGNX(float x);

class ToolFunction{
public:
    static cocos2d::Animation* createFrameAnimation(int indexStar/* 开始下标*/,int indexEnd/*结束下标*/,std::string urlFormat /* format字符串 */,std::string nameForSave = ""/*保存的名字 有名字保存 没名字不保存 */,float perUnit=0.1f /*每一帧时间*/ ,bool isRestoreOriginalFrame = true /* 是否返回到第一帧 */);
    static inline void calculateDeltaTimeFunc(std::string id); //前一次调用到本次调用的耗时
    static cocos2d::BezierTo* getBzier(cocos2d::Vec2 _startPoint, cocos2d::Vec2 _endPoint,int height,float deltime);
private:
    static std::chrono::steady_clock::time_point _lastUpdate;
    
};


#endif /* ToolFunction_hpp */
