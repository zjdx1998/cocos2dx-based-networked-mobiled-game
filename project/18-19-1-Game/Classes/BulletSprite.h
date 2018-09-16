#pragma once

#ifndef BULLET_SPRITE_H
#define BULLET_SPRITE_H

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class BulletSprite :public Sprite
{
public:
	
	void setPlayerID(int);
	int getPlayerID() const;

	void setMonsterID(int);
	int getMonsterID() const;

	void setDamage(float);
	float getDamage() const;

	static BulletSprite * create(const string&);
	static BulletSprite* createWithTexture(Texture2D *texture);
	bool operator==(BulletSprite&);
private:
	//记录发出子弹的玩家，如果子弹是怪物发出的，记为0
	int playerID;
	//记录发出子弹的怪物，如果子弹是玩家发出的，记为0
	int monsterID;
	//记录当前武器的伤害加成
	float damage;
};

#endif // !BULLET_SPRITE_H

