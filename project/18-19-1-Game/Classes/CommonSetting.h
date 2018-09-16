#pragma once

#ifndef _COMMON_SETTING_H
#define _COMMON_SETTING_H

#include "cocos2d.h"
#include "Player.h"
#include "monster.h"
#include "item.h"
#include "armor.h"
#include "consum.h"
#include "gun.h"
#include <vector>

/*
    Description : 用于一些全局变量设置
    Author : 张建东
    Date : 2018/8/20
*/
#define CSET CommonSetting::getInstance()
#define CSETN CSET->getNetworkID()
#define CSETP CSET->player[CSETN]
#define netP(i) CSET->player[i]
const float dis_collider = 30.0f / Director::getInstance()->getContentScaleFactor();
const float dis_close = 100.0f / Director::getInstance()->getContentScaleFactor();
const float dis_distant = 300.0f / Director::getInstance()->getContentScaleFactor();
const int ExpLevel[50] = { 4,5,7,10,15,22,28,40,53,64,75,95,111,124,
139,158,188,214,245,272,298,327,351,376,401,440,486,522,567,596,632,
669,725,787,844,912,951,1001,1055,1104,1163,1222,1284,1340,1425,1491,1565,1648,1719 };
const float const_pi = acos(-1);
class CommonSetting : public cocos2d::Ref {
private:
    CommonSetting();
    static CommonSetting* instance;//单例模式
    virtual ~CommonSetting();
	int m_iNetworkID = 1; //networkID，方便网络开发
    int m_iNetworkCnt = 1;//networkCnt，记载房间人数(<=4)
public:
    int getNetworkCnt();
    void setNetworkID(const int &);
    int getNetworkID();
    void setNetworkCnt(const int&);
    static CommonSetting* getInstance();
    const static std::string Direction[8];
    const static std::string bossDirection[8];
    //我在设计该类的时候，是将该类作为公共类产生单例，
    //故以下三成员为保证程序可读性均不采用匈牙利命名法
    //也不单独设置get与set
    Player player[5];//四个玩家
    Monster* monster[120];//100多个怪物
    //怪物序号记录散列表，优化怪物搜索时间
    vector<int> mHashId;//怪物散列表,优化搜索效率
    item* item[150];//100多个物品
    vector<int> iHashId;
};

#endif