#pragma once
#include "item.h"
#include "Bullet.h"
#include <vector>
class gun :
	public item
{
public:
	gun();
	gun(int, const string &, int, const string &, const string &, int, int, const string &,int);
	gun(const gun &);
	virtual ~gun();
	string getbullet_Type()const;
    int getgun_Damage()const;
	static vector<gun> initGunList();
	void useItem();
	
private:
	string bullet_Type;
    int gun_Damage;
};

