#pragma once
#include "item.h"
#include <vector>
class armor :
	public item
{
public:
	armor();
	armor(int, const string &, int, const string &, const string &, int, int,int,int,int,int,int,int);
	armor(const armor &);
	virtual ~armor();

	int DmgCalculate() {//伤害计算公式
		//(攻击方武器动作值+攻击方防具固定增伤量-受击方防具固定减伤量）*（1+攻击方防具增伤系数）*(1+受击方防具减伤系数）*（1+buff增伤系数）+buff增伤值-buff减伤值
	}

	static vector<armor> initArmorList();

	void useItem();


private:
	int armor_DefRate;//减伤系数，如0.3为减少30%受到伤害，-0.2为加深20%受到伤害
	int armor_DefCount;//固定减伤量,10为减少10点受到伤害
	int armor_HpUp;//增加血量上限
	int armor_SpdUp;//加速，固定值
	int armor_AtkRate;//增伤系数
	int armor_AtkCount;//固定增伤
};

