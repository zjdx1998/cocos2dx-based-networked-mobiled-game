#pragma once
#include"st_Player.h"
#include"st_CommonSetting.h"
#include"st_monster.h"
#include "vector"
#include <boost/serialization/vector.hpp>

struct client_message
{
	st_Player Player;
	int networkID;
	std::vector<int> collect_items;
	std::vector<st_Monster> monsters;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & Player;
		ar & networkID;
		ar & collect_items;
	}
	friend class boost::serialization::access;

	client_message(st_Player p) :Player(p) {};
	client_message() {};
};
struct broadcast_message
{
	st_CommonSetting cs;


	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & cs;
	}

	broadcast_message(st_CommonSetting cs_)
	{
		for (int i = 0; i < 4; i++)
		{
			cs.st_Player[i + 1] = cs_.st_Player[i + 1];
		}
	}
	broadcast_message() {};

	friend class boost::serialization::access;
};