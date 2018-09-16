#include "gun.h"

#include "CommonSetting.h"

using namespace cocos2d;

gun::gun()
    :item(0, "", 1, "", "", 1, 1), bullet_Type(""), gun_Damage(0)
{
}

gun::gun(int id, const string &name, int type, const string &icon, const string &intro, int level, int player, const string &btype,int damage)
	:item(id, name, type, icon, intro, level, player), bullet_Type(btype),gun_Damage(damage)
{
}

gun::gun(const gun &g) : item(g.getitem_Id(), g.getitem_Name(), g.getitem_Type(), g.getitem_Icon(),
	g.getitem_Introduction(), g.getitem_Level(), g.getitem_PlayerId()), bullet_Type(g.getbullet_Type()), gun_Damage(g.getgun_Damage())
{
}


gun::~gun()
{
}

string gun::getbullet_Type() const
{
	return bullet_Type;
}

int gun::getgun_Damage() const
{
    return gun_Damage;
}

vector<gun> gun::initGunList()
{
	vector<gun> gunList;
	gunList.push_back(gun(1, "Revolver", 1, "Revolver.png", "An old gun.", 2, 0, "default", 5));
	gunList.push_back(gun(2, "Short Gun", 1, "Shotgun.png", "A short gun.", 3, 0, "BLACK", 15));
	gunList.push_back(gun(7, "Pistol", 1, "Pistol.png", "Quick,and smart.", 3, 0, "GREEN", 10));
	gunList.push_back(gun(8, "Sniper", 1, "Sniper.png", "One shot,one kill.", 3, 0, "BLUE", 20));
	gunList.push_back(gun(9, "Rifle", 1, "Rifle.png", "Stronger power.", 3, 0, "YELLOW", 25));
	gunList.push_back(gun(10, "GoldenGun", 1, "GoldenGun.png", "Very expensive.", 4, 0, "X_RAY", 40));

	return gunList;
}


void gun::useItem()
{
	CSET->player[CSETN].setBulletType(bullet_Type);
}


