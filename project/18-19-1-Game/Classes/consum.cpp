#include "consum.h"

#include "CommonSetting.h"

using namespace cocos2d;


consum::consum()
    :item(0, "", 3, "", "", 1, 0), amount(0), consum_Type(0)
{
}

consum::consum(int id, const string &name, int type, const string &icon, const string &intro, int level, int player, 
	int amount,int ctype):item(id, name, type, icon, intro, level, player),amount(amount),consum_Type(ctype)
{
}

consum::consum(const consum &g):item(g.getitem_Id(), g.getitem_Name(), g.getitem_Type(), g.getitem_Icon(),
	g.getitem_Introduction(), g.getitem_Level(), g.getitem_PlayerId()),amount(g.amount),consum_Type(g.consum_Type)
{
}


consum::~consum()
{
}

int consum::getAmount() const
{
	return amount;
}

void consum::setAmount(int x)
{
	amount = x;
}

int consum::getconsum_Type() const
{
	return consum_Type;
}

void consum::setconsum_Type(int x)
{
	consum_Type = x;
}

void consum::useItem()
{
	//if (amount == 0) {
	//	//提示物品用完
	//	return ;
	//}
	//--amount;//剩余数量减一
	if (consum_Type == 1) {
		//不同物品的效果

		int playerhp = CSET->player[CSETN].getHP();
		int hpup = 50;
		int maxhp = CSET->player[CSETN].getLevel() * 1000 + 100;
		if (playerhp + hpup > maxhp) {
			CSET->player[CSETN].setHP(maxhp);
		}
		else {
			CSET->player[CSETN].setHP(playerhp + hpup);
		}

	}else if(consum_Type==2){
	
		float playerlevel = CSET->player[CSETN].getLevel();
		if (playerlevel < 4.8) {
			CSET->player[CSETN].setLevel(playerlevel+0.1);
		}
	
	}

}

vector<consum> consum::initConsumList()
{
	vector<consum> consumList;
	consumList.push_back(consum(5, "Burger", 3, "Burger.png", "Food", 2, 0, 1, 1));
	consumList.push_back(consum(6, "Scroll", 3, "Scroll.png", "Level up", 2, 0, 1, 2));

	return consumList;
}
