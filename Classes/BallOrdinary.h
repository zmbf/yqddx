//
//  BallOrdinary.hpp
//  yqddx-mobile
//
//  Created by xdlc on 2018/4/25.
//
//普通球
#ifndef BallOrdinary_hpp
#define BallOrdinary_hpp
#define BALLTYPECOUNT (6) //球种类数+1 type是从1开始的
#define MAXBALLCOUNT (80) //最多80个球
#include "Sprite.h"
#include "cocos/editor-support/cocostudio/CocoStudio.h"
#include "cocos/ui/CocosGUI.h"
class BallOrdinary : public Sprite{
public:
    inline const int & getType(){return m_type;};
    inline void setType(const int & type){m_type = type;};
    void setIsSelect(const bool & isSelect);
    void helpAction(const bool & isHelp); //开启 关闭  help
    void eliminate(const std::function<void(void)> & callback);//消除
private:
    static BallOrdinary* create(const int & type);
    virtual bool init(const int & type);
private:
    int m_type;
    bool m_isInUse;
    std::function<void(void)> m_callFunc;
    virtual ~BallOrdinary(){};
    friend class BallFactory;
    bool isInhelp;
    bool isInselect;
/*
 
 */
};
#endif /* BallOrdinary_hpp */
