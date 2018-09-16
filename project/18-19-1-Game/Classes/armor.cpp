#include "armor.h"

#include "CommonSetting.h"

using namespace cocos2d;

armor::armor()
    :item(0, "", 2, "", "", 1, 0), armor_DefRate(0), armor_DefCount(0),
    armor_HpUp(0), armor_SpdUp(0), armor_AtkRate(0), armor_AtkCount(0)
{
}

armor::armor(int id, const string &name, int type, const string &icon, const string &intro, int level, int player,
	int defr, int defc, int hp, int spd, int atkr, int atkc)
	:item(id, name, type, icon, intro, level, player), armor_DefRate(defr), armor_DefCount(defc),
	armor_HpUp(hp), armor_SpdUp(spd), armor_AtkRate(atkr), armor_AtkCount(atkc)
{
}

armor::armor(const armor &g)
	: item(g.getitem_Id(), g.getitem_Name(), g.getitem_Type(), g.getitem_Icon(),
		g.getitem_Introduction(), g.getitem_Level(), g.getitem_PlayerId()), armor_DefRate(g.armor_DefRate), armor_DefCount(g.armor_DefCount),
	armor_HpUp(g.armor_HpUp), armor_SpdUp(g.armor_SpdUp), armor_AtkRate(g.armor_AtkRate), armor_AtkCount(g.armor_AtkCount)
{
}


armor::~armor()
{
}

vector<armor> armor::initArmorList()
{
	vector<armor> armorList;
	string str1 = "Old Armor";
	armorList.push_back(armor(3, "Old Armor", 2, "OldArmor.png", "An old armor", 2, 0, 10, 10, 10, 0, 0, 0));
	armorList.push_back(armor(4, "Energy boots", 2, "EnergyBoots.png", "Best boots", 3, 0, 0, 0, 0, 20, 10, 10));

	return armorList;
}
void armor::useItem()
{
	if (CSET->player[CSETN].getSpeed() < 0.4) {
		CSET->player[CSETN].setSpeed(CSET->player[CSETN].getSpeed()*(1 + armor_SpdUp * 0.002));
	}
	CSET->player[CSETN].setHP(CSET->player[CSETN].getHP() + armor_HpUp);
}
