/*
    Description ： 游戏场景设置(setting for gameScene)
    Author : 张建东(ZhangJiandong 80%)
    Date : 2018-8-26
*/

#ifndef __JOYSTICK_SCENE_H__
#define __JOYSTICK_SCENE_H__

#include "hall.h"
#include "BagWindow.h"
#include "cocos2d.h"
#include "SceneTemplate.h"
#include "YGMoudle.h"
#include "fstream"
#include "broadcast_server.h"
#include "broadcast_client.h"
#include "boost/random.hpp"
#include "Bullet.h"
#include "SimpleAudioEngine.h"
#define CSET CommonSetting::getInstance()
#define CSETN CSET->getNetworkID()
#define CSETP CSET->player[CSETN]
USING_NS_FGUI;

class GameScene : public SceneTemplate
{
public:
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

//    static void mySchedule(Player*);
    //调度器---(人物重生）
    void setDietoBorn(float);
    ~GameScene();
    //获得两点间距离
    static float getDis(Vec2, Vec2);
    //渲染主UI界面
    void solveUIRender();
    //添加玩家
    void addPlayer();
    //预加载动画
    void preAnim();
    // 移动怪物
    void moveMonster(float);
    //每秒帧更新
    void update(float) override;
    //屏幕跟随
    void setViewPointCenter(cocos2d::Point position);
    //添加怪物
    void addMonster();
	void removeItem(int i);
	//添加子弹
	void addBullet();

    void showPickUpMenu(float);

    void addItem(float dt);
    //手动加载背包
    void testBag(float);

	st_CommonSetting st_cs;	//CommonSetting的简化结构体，每50ms更新一次

    void refresh_stcs_host(st_CommonSetting st_cs_, BroadcastServer *serverptr);
    void refresh_monsters(vector<st_Monster> smonvec);
    //ˢ��st_cs��ʹ�õ�ǰCommonSetting������һ֡����
    void refresh_stcs_client(st_CommonSetting st_cs_, BroadcastClient *clientptr);	//client�õģ��������Ǹ��÷�һ��
protected:
    virtual void continueInit() override;

private:
    //界面
    GComponent* m_gComPtrView;
    YGMoudle* m_clsPtrUI;
    BagWindow* m_winPtrBagWin;
    cocos2d::TMXTiledMap *m_tmxPtrTileMap;//地图
    cocos2d::TMXLayer *m_tmxLayerPtrBlock1;//障碍1
    cocos2d::TMXLayer *m_tmxLayerPtrBlock2;//障碍2
    Bullet * bulletLayer;
    //动画区域-------------------------------------------
    //为程序可读性，以下数组均不采用匈牙利命名法
    //且由于GameScene的特殊性，决定人为破坏其封装性。
    cocos2d::Vector<SpriteFrame*> player_move[8];
    cocos2d::Vector<SpriteFrame*> monster1_move[8];
    cocos2d::Vector<SpriteFrame*> monster2_move[8];
    cocos2d::Vector<SpriteFrame*> player_attack[8];
    cocos2d::Vector<SpriteFrame*> boss_move[8];
    cocos2d::Vector<SpriteFrame*> boss_attack[8];
    cocos2d::Vector<SpriteFrame*> boss_die[8];
    cocos2d::Vector<SpriteFrame*> monster1_attack[8];
    //--------------------------------------------------
	bool ishost;	//是否是host
	void refresh_commonsetting(int id);
    static  CocosDenshion::SimpleAudioEngine* audio;

};

#endif
