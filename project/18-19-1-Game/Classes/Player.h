#pragma once
/*
    Description : 游戏人物设定(game player setting)
    Author : 张建东(Zhang Jiandong)
    Date : 2018/8/20
*/

#include "cocos2d.h"
#include "BloodBar.h"
const float eps = 1e-6;
using namespace cocos2d;
using namespace std;

class Player {
public:
    static void CreatePlayer(int);
    //升级动画
    void LevelUpAnim();
    Player();
    //获得摇杆角度制（以正西为正方向）
    float getDegree()const;
    void setDegree(const float&);
    //速度
    float getSpeed() const;
    void setSpeed(const float&);
    //血量
    float getHP() const;
    void setHP(const float&);
    //伤害
    virtual float getDamage() const;
    void setDamage(const float&);
    //名称
    string getName() const;
    void setName(const string&);
    //下一步方向
    string getNextDirection() const;
    void setNextDirection(const string&);
    //当前方向
    string getNowDirection() const;
    void setNowDirection(const string&);
    //获得游戏角色
    void setPlayer(Sprite*);
    Sprite* getPlayer();
    virtual void Move(int,Sprite*,cocos2d::TMXTiledMap*,TMXLayer*,TMXLayer*);
    virtual void UpdateAnim();
    //设置动画结束
    bool getAnimDone()const;
    void setAnimDone(const bool&);
    Point tileCoordToPosition(cocos2d::TMXTiledMap* tileMap, Vec2 pos);
    //等级
    float getLevel() const;
    void setLevel(const float &);
    //分数
    float getScore() const;
    void setScore(const float&);
    //void setDietoBorn(float dt);
    //玩家死亡函数
    void PlayerDie();
    //判断是否死亡
    bool isDie() const;
    //设置子弹种类
    void setBulletType(const string&);
    string getBulletType() const;
    //判断是否攻击
    bool isAttack()const;
    void setIsAttack(const bool &x);
    //添加基本信息
    virtual void addBasicInfo();
    //优化技巧: 近身怪物vector
    vector<int> closeMonster;
    //血槽
    BloodBar* m_bloodProgress;
    //基本信息显示label
    LabelTTF* m_proShow;
    vector<pair<int, int>> iHashID;
    virtual float getMaxHP()const;
    Vec2 getDestination() const;
    void setDestination(const Vec2 &);
    void setIsScheduing(const bool& x);
    bool getIsScheduing()const;
protected:
    float m_fSpeed,m_fHP,m_fDamage,m_fDegree;//速度、血量、伤害
    bool m_bIsAnimDone,m_bIsAttack,m_bIsScheduing;//动画判断,攻击判断,计时器
    string m_strName, m_strNowDirection,m_strNextDirection;//姓名,现在方向，下一步方向
    string bulletType;
    Vec2 m_pDestination;//终点
    pair<int,int> directionScale[8] = { {-10,0} ,{-7,7},
                        {0,10},{7,7},{10,0},
                        {7,-7},{0,-10},
                        {-7,-7} };
private:
    Sprite* m_spPtrPlayer;//玩家_精灵
    float m_fLevel, m_fScore;//等级,分数
};