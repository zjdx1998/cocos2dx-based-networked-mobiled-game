#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "cocos2d.h"
#include "BulletSprite.h"
#include <vector>

USING_NS_CC;
using namespace std;

//武器种类
enum type{BLACK,GREEN,BLUE,YELLOW,X_RAY};
//const int INITBULLET = 10;
//武器发射距离
const float DISTANCE = 40.0f;


class Bullet:public cocos2d::Layer
{
private:
   // static Bullet* instance;
	float speed;//速度
	type thisType;//武器种类
	string picName;//图片名称 
	Texture2D* nowTexture, *bullet1, *bullet2, *bullet3, *bullet4, *bullet5, *bulletFire;
	Vec2 direction;//释放武器的方向
	float distance;//射程
	float power;//武器威力

	vector<int> monsterID;
	vector<int> minDisPlayerID;

	bool canAttack[5];

	double monsterDt, playerDt;//怪物和玩家可以发射子弹的间隔

public:
	Vector<BulletSprite*> bulletVector;
	Vector<BulletSprite*> monsterBullet;
//    static Bullet* getInstance();

	~Bullet();

	CREATE_FUNC(Bullet);
	bool init();

	void setMonsterDt(const float);
	void setPlayerDt(const float);

	void setSpeed(float);
	float getSpeed()const;

	void setType(type);
	void setType(const string&);
	type getType() const;

	void setPicName(std::string);
	std::string getPicName();

	void setPower(float);
	float getPower()const;

	void setDistance(float);
	float getDistance() const;

	//清除子弹
	void removeBullet(BulletSprite * bullet);
	//判断碰撞
	void ifCollide(float dt);
	//人物攻击
	void playerAttack(int);
	void playerCanAttack(float);
	
	//怪物攻击
	void monsterAttack(float);
	void startMonsterAttack(int, int);
	void stopMonsterAttack(int, int);
	
	//计算攻击所需数据
	float getDegree(Vec2 A, Vec2 B);
	float getDis(Vec2 A, Vec2 B);
};




#endif // !BULLET_H

