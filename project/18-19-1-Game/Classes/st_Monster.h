#pragma once
#include<string>
#include <boost/serialization/access.hpp>
using std::string;
struct st_Monster
{
	int monsterID;
	float m_fSpeed, m_fHP, m_fDamage, m_fDegree;
	string m_strNowDirection, m_strNextDirection;
	float x, y;
	bool m_bIsAttack;

	st_Monster():m_fSpeed(0), m_fHP(0), m_fDamage(0), m_fDegree(0), x(0), y(0), monsterID(0), m_bIsAttack(false){}

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & monsterID;
		ar & m_fSpeed;
		ar & m_fHP;
		ar & m_fDamage;
		ar & m_strNowDirection;
		ar & m_strNextDirection;
		ar & x;
		ar & y;
		ar & m_bIsAttack;
	}

	bool equals(const st_Monster& target)
	{
		return monsterID == target.monsterID;
	}

	bool operator<(const struct st_Monster & right)const
	{
		if (this == &right)
			return false;
		else
			return this->monsterID < right.monsterID;
	}

};