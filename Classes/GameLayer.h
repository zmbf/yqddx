#ifndef __GameLayer__H__
#define __GameLayer__H__

#include "cocos2d.h"
#include "GameData.h"
#include <vector>
#include "cocos/ui/UIImageView.h"
#include "cocos/ui/UILayout.h"
#include "cocos/ui/UITextAtlas.h"
#include "BallFactory.h"
#include "Sprite.h"
#include <map>
#include "AppDelegate.h"
#include "PauseLayer.h"
#include "TargetShowLayer.h"
#include "GameOverShowLayer.h"
#include <stdio.h>
class GameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(const CKF_GameData& _data);
    const CKF_GameData & getData(){return m_data;};
private:
    void analyzeCoinfig(); //读取配置文件
    void initUI();//加载显示的UI
    void initTargetUI(); //目标数据初始化
    void analyzeTileMap();//解析地图
    void supplyBall(const int & count = 1);//补充球
    int getRandBallType();  //随机一个球 type
    void setStep(const int & step); //设置步数
    inline const int & getStep(){return m_step;};//获取步数
    void gameSuccess();//游戏成功
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    void getlinkBallVec(std::vector<BallOrdinary*>& ballCanLinkVec,BallOrdinary* &  ball); // 获取m_BallOrdinaryMap 中可以与这个球连在一起的球(包括自身) 传入空vector
    void getlinkBallVec(std::vector<BallOrdinary*>& ballCanLinkVec,int minCount = 2/* 最少连球 */);// 查找 m_BallOrdinaryMap 中可以连选的球 传入空vector
    void ballSelectHelp(float dt); //选球提示
    void ballSelectHelpClose(float dt); //关闭选球提示
    float removeSelectBall(std::vector<BallOrdinary*> & vec);//移除选中的球
    void selectCancelled();//取消选球
    void showEncourageWord(int id);
    void boomArea(cocos2d::Vec2 pointCenter,int grade);//区域爆炸
    void crossArea(cocos2d::Vec2 pointCenter,int grade);//十字区域消除
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);//键盘事件监听
    //周期函数
    GameLayer();
    virtual ~GameLayer();
    virtual bool init();
    void  onEnter();
    void onEnterTransitionDidFinish();
    void update(float dt);
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
    void onExitTransitionDidStart();
    void onExit();
    void cleanup(); //取消cleanup() 功能
    CREATE_FUNC(GameLayer);
private:
    bool isFirstInser;//是否第一次进入游戏
    static GameLayer * m_gameLayer;
    int needAddBallCount;
    //目标组合
    struct targetAssemble{
        cocos2d::ui::ImageView* image;  //图片
        cocos2d::ui::TextAtlas* number;//达成目标数
        cocos2d::ui::ImageView* reachImage; //达成图片
        int ballType;
    } * m_targetAssemble;
    std::map<std::string, sXDLCMissionSuccessCondition> m_missionSuccess;//目标完成条件map
    
    cocos2d::EventListenerKeyboard* m_keyListen;
    cocos2d::EventListenerTouchOneByOne * m_touchListen;
    cocos2d::Size visibleSize;  //游戏窗口大小
    //传入数据
    CKF_GameData m_data;
    //配置读取数据
    int m_ball_count;  //总的球数
    int m_step;       //步数
    std::vector<int> m_mission_types; //步数
    float m_ballCountMaxPer[5]; //生成球的两个百分比
    float m_ballCountMinPer[5];
    
   
    // ParticleBatchNode
    // cocos2d::ParticleBatchNode* m_ball_par;
    // cocos2d::ParticleBatchNode* m_ice_par;
    
    //地图信息
    cocos2d::TMXTiledMap* m_map;  //tilemap 地图
    cocos2d::Vec2 m_Map_point;     //保存地图需要在的地图位置
    //effect layer 读取到的点位置
    cocos2d::Vec2 m_left_bottom;
    cocos2d::Vec2 m_right_bottom;
    cocos2d::Vec2 m_top;
    float  m_mapWidth_left;
    float  m_mapWidth_Right;
    
    std::vector<Node*> m_needRemoveNodeVec;
    
    //地图上的三角蒙板
//    cocos2d::Vector<cocos2d::Sprite *>m_vblock;
//    cocos2d::Vector<cocos2d::ui::Widget *>m_vbg;
//    cocos2d::Sprite * m_mask;
    
    //ui
    const int c_mUiLocalZorder;//需要显示在上面的 UI zorder
    cocos2d::ui::Layout* m_root; //上部UI
    cocos2d::ui::Layout* m_bottom;  //下部UI
    cocos2d::ui::TextAtlas* m_scoreLabel;  //分数 TextAtlas
    cocos2d::ui::TextAtlas* m_touchAtlas;  //点击数
    //float  m_mapWidth_left;
    //float  m_mapWidth_Right;
    
    cocos2d::Node* m_ballBatchNode; //球的父节点  
    cocos2d::ui::Layout* m_exitLayer;  //退出界面
    PauseLayer* m_pauseLayer; //暂停界面
    GameOverShowLayer * m_gameOverShowLayer;
    cocos2d::ui::Button* m_pauseButton; //暂停按钮
    TargetShowLayer * m_targetshowLayer;
    std::vector<std::vector<BallOrdinary*>*>* m_BallOrdinaryMap;  //地图上所有的球 key 是球 type
    std::vector<BallOrdinary*>* m_BallOrdinarySelect;                       //被选中的球集合
    std::vector<BallOrdinary*>* selectHelpVec;
    
};

#endif // __GameLayer_SCENE_H__
