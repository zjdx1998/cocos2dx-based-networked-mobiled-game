#pragma once
#include "st_Player.h"
#include "st_monster.h"
#include <boost/serialization/vector.hpp>

/*
Description : 用于一些全局变量设置
Author : 张建东
Date : 2018/8/20
*/

struct st_CommonSetting {
	st_Player st_Player[5];
	std::vector<int> collect_items;
	std::vector<st_Monster> monsters;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & st_Player;
		ar & collect_items;
		ar & monsters;
	}

	friend class boost::serialization::access;

	void operator=(st_CommonSetting& s)
	{
		for (int i = 0; i < 4; i++)
		{
			st_Player[i + 1] = s.st_Player[i + 1];
		}
		monsters.clear();
		for (auto mon : s.monsters)
		{
			monsters.push_back(mon);
		}
	}
};
