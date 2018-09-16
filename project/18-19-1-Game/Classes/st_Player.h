#pragma once
#include<string>
#include <boost/serialization/access.hpp>
/*
Description : 游戏人物设定
Author : 张建东
Date : 2018/8/20
*/

using namespace std;

struct st_Player {
    float Level;
    float score;//网络ID,等级,分数
	float speed, HP, damage;//速度、血量、伤害
	bool animDone,is_attack;//动画判断
	string name, nowDirection, nextDirection;
	string bulletType;
	float x, y;
	float degree;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & x;
		ar & y;
		ar & Level;
		ar & score;
		ar & speed;
		ar & HP;
		ar & damage;
		ar & animDone;
		ar & name;
		ar & nowDirection;
		ar & nextDirection;
		ar & is_attack;
		ar & degree;
		ar & bulletType;
	}

	friend class boost::serialization::access;
	st_Player() :Level(0), score(0), speed(0), HP(0), 
		damage(0), animDone(0),x(0),y(0),is_attack(0),degree(0) {}
};